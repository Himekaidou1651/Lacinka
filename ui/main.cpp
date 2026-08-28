#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shellapi.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>

#include "js_runtime.h"

typedef HANDLE DPI_AWARENESS_CONTEXT;
#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
#endif
#ifndef PROCESS_PER_MONITOR_DPI_AWARE
#define PROCESS_PER_MONITOR_DPI_AWARE 2
#endif

static const int IDC_INPUT = 1001;
static const int IDC_OUTPUT = 1002;
static const int IDC_RUN = 1003;
static const int IDC_SWAP = 1004;
static const int IDC_CLEAR = 1005;
static const int IDC_COPY = 1006;
static const int IDC_DOWNLOAD = 1007;
static const int IDC_MODE_HELLAS = 1008;
static const int IDC_MODE_JUGOSLAV = 1009;
static const int IDC_SAMPLE_HELLAS = 1010;
static const int IDC_SAMPLE_JUGOSLAV = 1011;

static const UINT_PTR TOAST_TIMER = 1;

static HWND gMain = nullptr;
static HWND gInput = nullptr;
static HWND gOutput = nullptr;
static HWND gRunBtn = nullptr;
static HWND gHellas = nullptr;
static HWND gJugoslav = nullptr;

static JsRuntime gJs;

static HFONT gTitleFont = nullptr;
static HFONT gBaseFont = nullptr;
static HFONT gSmallFont = nullptr;
static HFONT gMonoFont = nullptr;

static COLORREF gBg, gSurface, gSurfaceAlt, gText, gMuted, gBorder;
static COLORREF gPrimary, gPrimarySoft, gSuccess, gWarning, gDanger;
static COLORREF gOutputBg, gInputBg, gPillReady, gPillWorking;

static HBRUSH gBgBrush = nullptr;
static HBRUSH gSurfaceBrush = nullptr;
static HBRUSH gInputBrush = nullptr;
static HBRUSH gOutputBrush = nullptr;

static int gMode = 0;
static std::wstring gTitle, gStatus, gPill, gLastRun, gErrorText, gToast;
static COLORREF gPillColor = RGB(56, 161, 105);

static RECT gHeaderRect{}, gFooterRect{};
static RECT gTitleRect{}, gPillRect{}, gStatusRect{};
static RECT gErrorRect{}, gLastRunRect{};
static RECT gInputLabelRect{}, gOutputLabelRect{};
static RECT gInputCountRect{}, gOutputCountRect{};

static std::string wideToUtf8(const std::wstring& w) {
    if (w.empty()) return {};
    int size = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::vector<char> buffer(size);
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), buffer.data(), size, nullptr, nullptr);
    return std::string(buffer.begin(), buffer.end());
}

static std::wstring utf8ToWide(const std::string& s) {
    if (s.empty()) return {};
    int size = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
    std::vector<wchar_t> buffer(size);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), buffer.data(), size);
    return std::wstring(buffer.begin(), buffer.end());
}

static std::wstring getText(HWND h) {
    int len = GetWindowTextLengthW(h);
    std::vector<wchar_t> buf(len + 1);
    GetWindowTextW(h, buf.data(), len + 1);
    return std::wstring(buf.data(), len);
}

static void setText(HWND h, const std::wstring& s) {
    SetWindowTextW(h, s.c_str());
}

static std::wstring nowText() {
    SYSTEMTIME t;
    GetLocalTime(&t);
    wchar_t buf[32];
    swprintf(buf, 32, L"%02u:%02u:%02u", t.wHour, t.wMinute, t.wSecond);
    return buf;
}

static int dpiY() {
    HDC hdc = GetDC(nullptr);
    int dpi = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(nullptr, hdc);
    return dpi > 0 ? dpi : 96;
}

static HFONT makeFont(int px, int weight, const wchar_t* face) {
    return CreateFontW(-MulDiv(px, dpiY(), 96), 0, 0, 0, weight, FALSE, FALSE, FALSE,
                       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                       CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, face);
}

static COLORREF parseHex(const std::wstring& s) {
    if (s.size() < 7 || s[0] != L'#') return RGB(0, 0, 0);
    unsigned long v = wcstoul(s.c_str() + 1, nullptr, 16);
    return RGB((v >> 16) & 0xFF, (v >> 8) & 0xFF, v & 0xFF);
}

static COLORREF jsColor(const wchar_t* key, COLORREF fallback) {
    std::wstring s = gJs.color(key, L"");
    return s.empty() ? fallback : parseHex(s);
}

static COLORREF mixColor(COLORREF a, COLORREF b, int t) {
    if (t < 0) t = 0;
    if (t > 255) t = 255;
    int r = GetRValue(a) + (GetRValue(b) - GetRValue(a)) * t / 255;
    int g = GetGValue(a) + (GetGValue(b) - GetGValue(a)) * t / 255;
    int bl = GetBValue(a) + (GetBValue(b) - GetBValue(a)) * t / 255;
    return RGB(r, g, bl);
}

static COLORREF lighten(COLORREF c, int amount) {
    return mixColor(c, RGB(255, 255, 255), amount);
}

static COLORREF darken(COLORREF c, int amount) {
    return mixColor(c, RGB(0, 0, 0), amount);
}

static void fillRoundRect(HDC hdc, RECT rc, int radius, COLORREF c) {
    int h = rc.bottom - rc.top;
    if (h <= 0) return;
    if (radius > h / 2) radius = h / 2;
    if (radius <= 0) {
        HBRUSH br = CreateSolidBrush(c);
        FillRect(hdc, &rc, br);
        DeleteObject(br);
        return;
    }
    HRGN rgn = CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, radius * 2, radius * 2);
    HBRUSH br = CreateSolidBrush(c);
    FillRgn(hdc, rgn, br);
    DeleteObject(br);
    DeleteObject(rgn);
}

static void frameRoundRect(HDC hdc, RECT rc, int radius, COLORREF c, int widthPx) {
    int h = rc.bottom - rc.top;
    if (h <= 0) return;
    if (radius > h / 2) radius = h / 2;
    HRGN rgn = CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, radius * 2, radius * 2);
    HBRUSH br = CreateSolidBrush(c);
    FrameRgn(hdc, rgn, br, widthPx, widthPx);
    DeleteObject(br);
    DeleteObject(rgn);
}

static void gradientFillRound(HDC hdc, RECT rc, int radius, COLORREF top, COLORREF bottom) {
    int h = rc.bottom - rc.top;
    if (h <= 0) return;
    if (radius > h / 2) radius = h / 2;
    HRGN rgn = CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, radius * 2, radius * 2);
    int saved = SaveDC(hdc);
    SelectClipRgn(hdc, rgn);
    for (int y = 0; y < h; ++y) {
        int t = h > 1 ? y * 255 / (h - 1) : 255;
        COLORREF c = mixColor(top, bottom, t);
        RECT row{rc.left, rc.top + y, rc.right, rc.top + y + 1};
        HBRUSH br = CreateSolidBrush(c);
        FillRect(hdc, &row, br);
        DeleteObject(br);
    }
    RestoreDC(hdc, saved);
    DeleteObject(rgn);
}

static void drawText(HDC hdc, const wchar_t* text, RECT rc, HFONT font, COLORREF color, UINT fmt) {
    HFONT old = (HFONT)SelectObject(hdc, font ? font : gBaseFont);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, color);
    DrawTextW(hdc, text, -1, &rc, fmt);
    SelectObject(hdc, old);
}

static void refreshHeaderFooter() {
    InvalidateRect(gMain, &gHeaderRect, FALSE);
    InvalidateRect(gMain, &gFooterRect, FALSE);
}

static void drawCount(HDC hdc, RECT rc, size_t n) {
    COLORREF c = gMuted;
    if (n > 5000) c = gDanger;
    else if (n > 1000) c = gWarning;
    std::wstring s = std::to_wstring((unsigned long)n) + gJs.text(L"charactersSuffix", L" 字符");
    drawText(hdc, s.c_str(), rc, gSmallFont, c, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
}

static void drawHeader(HDC hdc) {
    HBRUSH br = CreateSolidBrush(gPrimary);
    FillRect(hdc, &gHeaderRect, br);
    DeleteObject(br);

    drawText(hdc, gTitle.c_str(), gTitleRect, gTitleFont, RGB(255, 255, 255), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    fillRoundRect(hdc, gPillRect, gJs.number(L"pill", 999), gPillColor);
    drawText(hdc, gPill.c_str(), gPillRect, gSmallFont, RGB(255, 255, 255), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawText(hdc, gStatus.c_str(), gStatusRect, gBaseFont, RGB(230, 240, 255), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

static void drawFooter(HDC hdc) {
    FillRect(hdc, &gFooterRect, gBgBrush);
    RECT line{gFooterRect.left, gFooterRect.top, gFooterRect.right, gFooterRect.top + 1};
    HBRUSH br = CreateSolidBrush(gBorder);
    FillRect(hdc, &line, br);
    DeleteObject(br);

    drawText(hdc, gErrorText.c_str(), gErrorRect, gSmallFont, gDanger, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    drawText(hdc, gLastRun.c_str(), gLastRunRect, gSmallFont, gMuted, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
}

static void drawBody(HDC hdc) {
    drawText(hdc, gJs.text(L"inputLabel", L"输入").c_str(), gInputLabelRect, gSmallFont, gMuted, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    drawText(hdc, gJs.text(L"outputLabel", L"输出").c_str(), gOutputLabelRect, gSmallFont, gMuted, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    drawCount(hdc, gInputCountRect, (size_t)GetWindowTextLengthW(gInput));
    drawCount(hdc, gOutputCountRect, (size_t)GetWindowTextLengthW(gOutput));
}

static void drawToast(HDC hdc) {
    if (gToast.empty()) return;
    RECT rc;
    GetClientRect(gMain, &rc);
    int w = rc.right - rc.left;
    int tw = 240, th = 34;
    RECT r{w / 2 - tw / 2, gFooterRect.top - th - 10, w / 2 + tw / 2, gFooterRect.top - 10};
    fillRoundRect(hdc, r, 6, darken(gText, 30));
    drawText(hdc, gToast.c_str(), r, gSmallFont, RGB(255, 255, 255), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

static void showToast(const std::wstring& msg) {
    gToast = msg;
    InvalidateRect(gMain, nullptr, FALSE);
    SetTimer(gMain, TOAST_TIMER, 2200, nullptr);
}

static void setStatus(const std::wstring& status, const std::wstring& pill, COLORREF color, const std::wstring& error) {
    gStatus = status;
    gPill = pill;
    gPillColor = color;
    gErrorText = error;
    refreshHeaderFooter();
}

static void setMode(int mode) {
    gMode = mode;
    InvalidateRect(gHellas, nullptr, FALSE);
    InvalidateRect(gJugoslav, nullptr, FALSE);
}

static void updateCounts() {
    InvalidateRect(gMain, &gInputCountRect, FALSE);
    InvalidateRect(gMain, &gOutputCountRect, FALSE);
}

static void runTransform() {
    std::wstring input = getText(gInput);
    if (input.empty()) {
        setStatus(gJs.text(L"statusWaiting", L"等待输入"), gJs.text(L"pillReady", L"就绪"), gPillReady,
                  gJs.text(L"emptyWarning", L"请先输入待转写内容"));
        return;
    }

    setStatus(gJs.text(L"statusProcessing", L"正在转写"), gJs.text(L"pillWorking", L"工作中"), gPillWorking, L"");
    SetWindowTextW(gRunBtn, gJs.text(L"processingButton", L"处理中...").c_str());
    EnableWindow(gRunBtn, FALSE);
    InvalidateRect(gRunBtn, nullptr, FALSE);

    bool failed = false;
    std::wstring failMsg;
    try {
        std::string out = gJs.transform(wideToUtf8(input), gMode);
        setText(gOutput, utf8ToWide(out));
    } catch (const std::exception& e) {
        failed = true;
        failMsg = utf8ToWide(e.what());
    } catch (...) {
        failed = true;
    }

    SetWindowTextW(gRunBtn, gJs.text(L"runButton", L"转写").c_str());
    EnableWindow(gRunBtn, TRUE);
    InvalidateRect(gRunBtn, nullptr, FALSE);

    if (failed) {
        setStatus(gJs.text(L"statusWaiting", L"等待输入"), gJs.text(L"pillReady", L"就绪"), gPillReady,
                  L"转写失败" + (failMsg.empty() ? L"" : (L": " + failMsg)));
    } else {
        setStatus(gJs.text(L"statusDone", L"转写完成"), gJs.text(L"pillReady", L"就绪"), gPillReady, L"");
    }
    gLastRun = gJs.text(L"lastRunPrefix", L"上次运行: ") + nowText();
    refreshHeaderFooter();
    updateCounts();
}

static void clearAll() {
    setText(gInput, L"");
    setText(gOutput, L"");
    gErrorText.clear();
    gStatus = gJs.text(L"statusWaiting", L"等待输入");
    gPill = gJs.text(L"pillReady", L"就绪");
    gPillColor = gPillReady;
    gLastRun = gJs.text(L"lastRunPrefix", L"上次运行: ") + gJs.text(L"neverRun", L"--");
    refreshHeaderFooter();
    updateCounts();
}

static void swapText() {
    std::wstring in = getText(gInput);
    std::wstring out = getText(gOutput);
    setText(gInput, out);
    setText(gOutput, in);
    updateCounts();
}

static void fillSample(bool hellas) {
    int mode = hellas ? 0 : 1;
    setText(gInput, gJs.sample(mode, L""));
    setMode(mode);
    gErrorText.clear();
    gStatus = gJs.text(L"statusWaiting", L"等待输入");
    gPill = gJs.text(L"pillReady", L"就绪");
    gPillColor = gPillReady;
    refreshHeaderFooter();
    updateCounts();
}

static void copyOutput() {
    std::wstring text = getText(gOutput);
    if (text.empty()) {
        showToast(L"没有可复制的内容");
        return;
    }
    if (!OpenClipboard(gMain)) {
        showToast(L"无法打开剪贴板");
        return;
    }
    EmptyClipboard();
    size_t bytes = (text.size() + 1) * sizeof(wchar_t);
    HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, bytes);
    if (mem) {
        void* p = GlobalLock(mem);
        memcpy(p, text.c_str(), bytes);
        GlobalUnlock(mem);
        SetClipboardData(CF_UNICODETEXT, mem);
    }
    CloseClipboard();

    setStatus(gJs.text(L"statusCopied", L"已复制输出"), gJs.text(L"pillReady", L"就绪"), gPillReady, L"");
    showToast(gJs.text(L"statusCopied", L"已复制输出"));
}

static void downloadOutput() {
    std::wstring text = getText(gOutput);
    if (text.empty()) {
        showToast(L"没有可下载的内容");
        return;
    }

    wchar_t file[1024] = L"Lacinka_output.txt";
    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = gMain;
    ofn.lpstrFilter = L"Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";
    ofn.lpstrFile = file;
    ofn.nMaxFile = 1024;
    ofn.lpstrDefExt = L"txt";
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

    if (!GetSaveFileNameW(&ofn)) {
        setStatus(gJs.text(L"statusWaiting", L"等待输入"), gJs.text(L"pillReady", L"就绪"), gPillReady,
                  gJs.text(L"downloadCancelled", L"已取消保存"));
        return;
    }

    HANDLE f = CreateFileW(file, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (f == INVALID_HANDLE_VALUE) {
        setStatus(gJs.text(L"statusWaiting", L"等待输入"), gJs.text(L"pillReady", L"就绪"), gPillReady, L"保存失败，无法写入文件");
        return;
    }
    std::string data = wideToUtf8(text);
    DWORD written = 0;
    WriteFile(f, data.c_str(), (DWORD)data.size(), &written, nullptr);
    CloseHandle(f);

    setStatus(gJs.text(L"statusDownloaded", L"已下载输出"), gJs.text(L"pillReady", L"就绪"), gPillReady, L"");
    showToast(gJs.text(L"downloadSuccess", L"已保存文件"));
}

static void drawButton(DRAWITEMSTRUCT* dis) {
    HDC hdc = dis->hDC;
    RECT rc = dis->rcItem;
    HWND btn = dis->hwndItem;
    int id = dis->CtlID;

    bool hover = GetPropW(btn, L"hover") != nullptr;
    bool pressed = (dis->itemState & ODS_SELECTED) != 0;
    bool disabled = (dis->itemState & ODS_DISABLED) != 0;

    wchar_t label[512];
    GetWindowTextW(btn, label, 512);

    int rPanel = gJs.number(L"panel", 6);
    int rBtn = gJs.number(L"button", 6);
    int rPill = gJs.number(L"pill", 999);

    COLORREF fill = gSurface, border = gBorder, text = gText;
    HFONT font = gBaseFont;
    int radius = rBtn;

    switch (id) {
    case IDC_RUN: {
        radius = rBtn;
        COLORREF base = pressed ? darken(gPrimary, 45) : (hover ? lighten(gPrimary, 15) : gPrimary);
        if (disabled) base = mixColor(gPrimary, gSurface, 110);
        gradientFillRound(hdc, rc, radius, lighten(base, 45), base);
        frameRoundRect(hdc, rc, radius, darken(base, 80), 1);
        text = RGB(255, 255, 255);
        break;
    }
    case IDC_MODE_HELLAS:
    case IDC_MODE_JUGOSLAV: {
        bool selected = (id == IDC_MODE_HELLAS) ? (gMode == 0) : (gMode == 1);
        radius = rPanel;
        fill = selected ? gPrimarySoft : (hover ? gSurfaceAlt : gSurface);
        border = selected ? gPrimary : gBorder;
        text = selected ? gPrimary : gText;
        fillRoundRect(hdc, rc, radius, fill);
        frameRoundRect(hdc, rc, radius, border, selected ? 2 : 1);
        break;
    }
    case IDC_SAMPLE_HELLAS:
    case IDC_SAMPLE_JUGOSLAV: {
        radius = rPill;
        fill = hover ? gPrimarySoft : gSurface;
        border = gBorder;
        text = gPrimary;
        fillRoundRect(hdc, rc, radius, fill);
        frameRoundRect(hdc, rc, radius, border, 1);
        break;
    }
    case IDC_CLEAR: {
        radius = rBtn;
        fill = hover ? lighten(gPrimary, 40) : lighten(gPrimary, 12);
        border = fill;
        text = RGB(255, 255, 255);
        fillRoundRect(hdc, rc, radius, fill);
        break;
    }
    default: {
        radius = rBtn;
        fill = hover ? gSurfaceAlt : gSurface;
        border = gBorder;
        text = gText;
        fillRoundRect(hdc, rc, radius, fill);
        frameRoundRect(hdc, rc, radius, border, 1);
        break;
    }
    }

    HFONT old = (HFONT)SelectObject(hdc, font);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, text);
    DrawTextW(hdc, label, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc, old);

    if ((dis->itemState & ODS_FOCUS) && id != IDC_SAMPLE_HELLAS && id != IDC_SAMPLE_JUGOSLAV) {
        RECT fr = rc;
        InflateRect(&fr, -4, -4);
        DrawFocusRect(hdc, &fr);
    }
}

static LRESULT CALLBACK OwnerBtnProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
    switch (msg) {
    case WM_MOUSEMOVE:
        if (GetPropW(hwnd, L"hover") == nullptr) {
            SetPropW(hwnd, L"hover", (HANDLE)1);
            TRACKMOUSEEVENT tme{sizeof(tme), TME_LEAVE, hwnd, 0};
            TrackMouseEvent(&tme);
            InvalidateRect(hwnd, nullptr, FALSE);
        }
        return 0;
    case WM_MOUSELEAVE:
        RemovePropW(hwnd, L"hover");
        InvalidateRect(hwnd, nullptr, FALSE);
        return 0;
    }
    return DefSubclassProc(hwnd, msg, wParam, lParam);
}

static void layout(HWND hwnd) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;

    int pad = gJs.number(L"padding", 16);
    int gapSm = gJs.number(L"sm", 8);
    int gapMd = gJs.number(L"md", 12);
    int gapLg = gJs.number(L"lg", 16);
    int headerH = gJs.number(L"headerHeight", 68);
    int footerH = gJs.number(L"footerHeight", 44);
    int ctrlW = gJs.number(L"controlWidth", 240);
    int labelH = 22;

    gHeaderRect = {0, 0, w, headerH};
    gFooterRect = {0, h - footerH, w, footerH};

    int clearW = 72, clearH = 30;
    int clearX = w - pad - clearW;
    MoveWindow(GetDlgItem(hwnd, IDC_CLEAR), clearX, (headerH - clearH) / 2, clearW, clearH, TRUE);

    gTitleRect = {pad, 0, pad + 240, headerH};
    int pillX = pad + 240 + gapSm;
    gPillRect = {pillX, (headerH - 24) / 2, pillX + 76, (headerH - 24) / 2 + 24};
    gStatusRect = {pillX + 84, 0, clearX - gapSm, headerH};

    gErrorRect = {pad, gFooterRect.top + 10, w - pad - 220, gFooterRect.bottom - 10};
    gLastRunRect = {w - pad - 220, gFooterRect.top + 10, w - pad, gFooterRect.bottom - 10};

    int bodyTop = headerH + pad;
    int bodyH = h - headerH - footerH - pad * 2;

    if (w >= gJs.number(L"desktopBreakpoint", 1200)) {
        int totalGaps = gapLg * 2;
        int leftW = (w - pad * 2 - ctrlW - totalGaps) / 2;
        int rightW = w - pad * 2 - ctrlW - totalGaps - leftW;
        int ctrlX = pad + leftW + gapLg;
        int outX = ctrlX + ctrlW + gapLg;

        gInputLabelRect = {pad, bodyTop, pad + leftW, bodyTop + labelH};
        int inEditTop = bodyTop + labelH + 4;
        int bottomRowH = 34;
        int inEditBottom = bodyTop + bodyH - bottomRowH - 4;
        MoveWindow(gInput, pad, inEditTop, leftW, inEditBottom - inEditTop, TRUE);
        gInputCountRect = {pad + leftW - 110, inEditBottom + 4, pad + leftW, inEditBottom + 24};

        int chipY = inEditBottom + 4;
        MoveWindow(GetDlgItem(hwnd, IDC_SAMPLE_HELLAS), pad, chipY, 84, 22, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_SAMPLE_JUGOSLAV), pad + 84 + gapSm, chipY, 112, 22, TRUE);

        gOutputLabelRect = {outX, bodyTop, outX + rightW, bodyTop + labelH};
        int outEditTop = bodyTop + labelH + 4;
        MoveWindow(gOutput, outX, outEditTop, rightW, bodyTop + bodyH - outEditTop, TRUE);
        gOutputCountRect = {outX + rightW - 110, bodyTop + bodyH - 22, outX + rightW, bodyTop + bodyH - 2};

        int cy = bodyTop + labelH + 4;
        int cardH = 40;
        MoveWindow(gHellas, ctrlX, cy, ctrlW, cardH, TRUE);
        MoveWindow(gJugoslav, ctrlX, cy + cardH + gapSm, ctrlW, cardH, TRUE);
        int runY = cy + cardH * 2 + gapSm * 2 + gapMd;
        MoveWindow(gRunBtn, ctrlX, runY, ctrlW, 46, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_SWAP), ctrlX, runY + 46 + gapSm, ctrlW, 32, TRUE);
        int half = (ctrlW - gapSm) / 2;
        int rowY = runY + 46 + gapSm + 32 + gapSm;
        MoveWindow(GetDlgItem(hwnd, IDC_COPY), ctrlX, rowY, half, 30, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_DOWNLOAD), ctrlX + half + gapSm, rowY, ctrlW - half - gapSm, 30, TRUE);
    } else {
        int columnW = w - pad * 2;
        int inputH = 180;
        gInputLabelRect = {pad, bodyTop, pad + columnW, bodyTop + labelH};
        int inTop = bodyTop + labelH + 4;
        MoveWindow(gInput, pad, inTop, columnW, inputH, TRUE);
        gInputCountRect = {pad + columnW - 110, inTop + inputH + 2, pad + columnW, inTop + inputH + 22};
        MoveWindow(GetDlgItem(hwnd, IDC_SAMPLE_HELLAS), pad, inTop + inputH + 2, 84, 22, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_SAMPLE_JUGOSLAV), pad + 92, inTop + inputH + 2, 112, 22, TRUE);

        int cy = inTop + inputH + 34;
        MoveWindow(gHellas, pad, cy, columnW / 2 - 4, 36, TRUE);
        MoveWindow(gJugoslav, pad + columnW / 2 + 4, cy, columnW / 2 - 4, 36, TRUE);
        MoveWindow(gRunBtn, pad, cy + 44, columnW, 42, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_SWAP), pad, cy + 92, columnW, 30, TRUE);
        int half = (columnW - gapSm) / 2;
        MoveWindow(GetDlgItem(hwnd, IDC_COPY), pad, cy + 126, half, 28, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_DOWNLOAD), pad + half + gapSm, cy + 126, columnW - half - gapSm, 28, TRUE);

        int outTop = cy + 160;
        gOutputLabelRect = {pad, outTop, pad + columnW, outTop + labelH};
        int outEditTop = outTop + labelH + 4;
        int outEditBottom = h - footerH - pad;
        MoveWindow(gOutput, pad, outEditTop, columnW, outEditBottom - outEditTop, TRUE);
        gOutputCountRect = {pad + columnW - 110, outEditBottom - 22, pad + columnW, outEditBottom - 2};
    }
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        gMain = hwnd;
        gTitle = gJs.text(L"appTitle", L"Lacinka 转写工具");
        SetWindowTextW(hwnd, gJs.text(L"windowTitle", L"Lacinka").c_str());

        gInput = CreateWindowExW(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
                                 0, 0, 0, 0, hwnd, (HMENU)IDC_INPUT, nullptr, nullptr);
        gOutput = CreateWindowExW(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL,
                                  0, 0, 0, 0, hwnd, (HMENU)IDC_OUTPUT, nullptr, nullptr);
        SendMessageW(gInput, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(8, 8));
        SendMessageW(gOutput, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(8, 8));
        SendMessageW(gInput, WM_SETFONT, (WPARAM)gBaseFont, TRUE);
        SendMessageW(gOutput, WM_SETFONT, (WPARAM)gMonoFont, TRUE);

        gHellas = CreateWindowW(L"BUTTON", gJs.modeLabel(0, L"Hellas").c_str(), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
                                0, 0, 0, 0, hwnd, (HMENU)IDC_MODE_HELLAS, nullptr, nullptr);
        gJugoslav = CreateWindowW(L"BUTTON", gJs.modeLabel(1, L"Jugoslav").c_str(), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
                                  0, 0, 0, 0, hwnd, (HMENU)IDC_MODE_JUGOSLAV, nullptr, nullptr);
        gRunBtn = CreateWindowW(L"BUTTON", gJs.text(L"runButton", L"转写").c_str(), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
                                0, 0, 0, 0, hwnd, (HMENU)IDC_RUN, nullptr, nullptr);
        CreateWindowW(L"BUTTON", gJs.text(L"clearButton", L"清空").c_str(), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
                      0, 0, 0, 0, hwnd, (HMENU)IDC_CLEAR, nullptr, nullptr);
        CreateWindowW(L"BUTTON", gJs.text(L"swapButton", L"交换").c_str(), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
                      0, 0, 0, 0, hwnd, (HMENU)IDC_SWAP, nullptr, nullptr);
        CreateWindowW(L"BUTTON", gJs.text(L"copyButton", L"复制").c_str(), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
                      0, 0, 0, 0, hwnd, (HMENU)IDC_COPY, nullptr, nullptr);
        CreateWindowW(L"BUTTON", gJs.text(L"downloadButton", L"下载TXT").c_str(), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
                      0, 0, 0, 0, hwnd, (HMENU)IDC_DOWNLOAD, nullptr, nullptr);
        CreateWindowW(L"BUTTON", gJs.text(L"hellasSampleButton", L"希腊示例").c_str(), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                      0, 0, 0, 0, hwnd, (HMENU)IDC_SAMPLE_HELLAS, nullptr, nullptr);
        CreateWindowW(L"BUTTON", gJs.text(L"jugoslavSampleButton", L"塞尔维亚示例").c_str(), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                      0, 0, 0, 0, hwnd, (HMENU)IDC_SAMPLE_JUGOSLAV, nullptr, nullptr);

        HWND subs[] = {gHellas, gJugoslav, gRunBtn,
                       GetDlgItem(hwnd, IDC_CLEAR), GetDlgItem(hwnd, IDC_SWAP),
                       GetDlgItem(hwnd, IDC_COPY), GetDlgItem(hwnd, IDC_DOWNLOAD),
                       GetDlgItem(hwnd, IDC_SAMPLE_HELLAS), GetDlgItem(hwnd, IDC_SAMPLE_JUGOSLAV)};
        for (HWND b : subs) SetWindowSubclass(b, OwnerBtnProc, 0, 0);

        gMode = 0;
        gStatus = gJs.text(L"statusWaiting", L"等待输入");
        gPill = gJs.text(L"pillReady", L"就绪");
        gPillColor = gPillReady;
        gLastRun = gJs.text(L"lastRunPrefix", L"上次运行: ") + gJs.text(L"neverRun", L"--");

        layout(hwnd);
        return 0;
    }

    case WM_SIZE:
        layout(hwnd);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, gBgBrush);
        drawHeader(hdc);
        drawBody(hdc);
        drawFooter(hdc);
        drawToast(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_CTLCOLOREDIT: {
        HDC hdc = (HDC)wParam;
        HWND ctl = (HWND)lParam;
        if (ctl == gOutput) {
            SetTextColor(hdc, RGB(245, 248, 252));
            SetBkColor(hdc, gOutputBg);
            return (LRESULT)gOutputBrush;
        }
        SetTextColor(hdc, gText);
        SetBkColor(hdc, gInputBg);
        return (LRESULT)gInputBrush;
    }

    case WM_DRAWITEM:
        if (((LPDRAWITEMSTRUCT)lParam)->CtlType == ODT_BUTTON) {
            drawButton((LPDRAWITEMSTRUCT)lParam);
            return TRUE;
        }
        break;

    case WM_TIMER:
        if (wParam == TOAST_TIMER) {
            KillTimer(hwnd, TOAST_TIMER);
            gToast.clear();
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        break;

    case WM_COMMAND: {
        int id = LOWORD(wParam);
        switch (id) {
        case IDC_MODE_HELLAS: setMode(0); return 0;
        case IDC_MODE_JUGOSLAV: setMode(1); return 0;
        case IDC_RUN: runTransform(); return 0;
        case IDC_CLEAR: clearAll(); return 0;
        case IDC_SWAP: swapText(); return 0;
        case IDC_COPY: copyOutput(); return 0;
        case IDC_DOWNLOAD: downloadOutput(); return 0;
        case IDC_SAMPLE_HELLAS: fillSample(true); return 0;
        case IDC_SAMPLE_JUGOSLAV: fillSample(false); return 0;
        case IDC_INPUT:
            if (HIWORD(wParam) == EN_CHANGE) updateCounts();
            return 0;
        case IDC_OUTPUT:
            if (HIWORD(wParam) == EN_CHANGE) updateCounts();
            return 0;
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

static void enableDpiAwareness() {
    HMODULE u32 = GetModuleHandleW(L"user32.dll");
    typedef BOOL(WINAPI* SetDpiCtxFn)(DPI_AWARENESS_CONTEXT);
    typedef HRESULT(WINAPI* SetDpiAwareFn)(int);
    auto setCtx = (SetDpiCtxFn)(void*)GetProcAddress(u32, "SetProcessDpiAwarenessContext");
    if (setCtx && setCtx(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) return;
    auto setAware = (SetDpiAwareFn)(void*)GetProcAddress(u32, "SetProcessDpiAwareness");
    if (setAware && SUCCEEDED(setAware(PROCESS_PER_MONITOR_DPI_AWARE))) return;
    typedef BOOL(WINAPI* SetDpiAwareLegacyFn)();
    auto legacy = (SetDpiAwareLegacyFn)(void*)GetProcAddress(u32, "SetProcessDPIAware");
    if (legacy) legacy();
}

static void initTheme() {
    gBg = jsColor(L"background", RGB(247, 250, 252));
    gSurface = jsColor(L"surface", RGB(255, 255, 255));
    gSurfaceAlt = jsColor(L"surfaceAlt", RGB(237, 242, 247));
    gText = jsColor(L"text", RGB(26, 32, 44));
    gMuted = jsColor(L"muted", RGB(74, 85, 104));
    gBorder = jsColor(L"border", RGB(203, 213, 224));
    gPrimary = jsColor(L"primary", RGB(43, 108, 176));
    gPrimarySoft = jsColor(L"primarySoft", RGB(235, 244, 255));
    gSuccess = jsColor(L"success", RGB(56, 161, 105));
    gWarning = jsColor(L"warning", RGB(214, 158, 46));
    gDanger = jsColor(L"danger", RGB(229, 62, 62));
    gOutputBg = jsColor(L"output", RGB(26, 32, 44));
    gInputBg = jsColor(L"input", RGB(255, 255, 255));
    gPillReady = jsColor(L"pillReady", RGB(56, 161, 105));
    gPillWorking = jsColor(L"pillWorking", RGB(214, 158, 46));

    gBgBrush = CreateSolidBrush(gBg);
    gSurfaceBrush = CreateSolidBrush(gSurface);
    gInputBrush = CreateSolidBrush(gInputBg);
    gOutputBrush = CreateSolidBrush(gOutputBg);

    std::wstring ff = gJs.text(L"fontFamily", L"Segoe UI");
    std::wstring mf = gJs.text(L"monoFontFamily", L"Consolas");
    gTitleFont = makeFont(gJs.number(L"titleSize", 17), FW_SEMIBOLD, ff.c_str());
    gBaseFont = makeFont(gJs.number(L"baseSize", 14), FW_NORMAL, ff.c_str());
    gSmallFont = makeFont(gJs.number(L"smallSize", 12), FW_NORMAL, ff.c_str());
    gMonoFont = makeFont(gJs.number(L"baseSize", 14), FW_NORMAL, mf.c_str());
}

static void destroyTheme() {
    if (gBgBrush) DeleteObject(gBgBrush);
    if (gSurfaceBrush) DeleteObject(gSurfaceBrush);
    if (gInputBrush) DeleteObject(gInputBrush);
    if (gOutputBrush) DeleteObject(gOutputBrush);
    if (gTitleFont) DeleteObject(gTitleFont);
    if (gBaseFont) DeleteObject(gBaseFont);
    if (gSmallFont) DeleteObject(gSmallFont);
    if (gMonoFont) DeleteObject(gMonoFont);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    enableDpiAwareness();

    INITCOMMONCONTROLSEX icc{sizeof(icc), ICC_STANDARD_CLASSES};
    InitCommonControlsEx(&icc);

    gJs.initialize(hInstance);
    initTheme();

    const wchar_t CLASS_NAME[] = L"LacinkaWindow";
    WNDCLASSW wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = gBgBrush;
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, gJs.text(L"windowTitle", L"Lacinka").c_str(),
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 780,
        nullptr, nullptr, hInstance, nullptr);

    ACCEL accel[] = {{FVIRTKEY | FCONTROL, VK_RETURN, IDC_RUN}};
    HACCEL hAccel = CreateAcceleratorTableW(accel, 1);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        if (!TranslateAcceleratorW(hwnd, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    if (hAccel) DestroyAcceleratorTable(hAccel);
    destroyTheme();
    return 0;
}