#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <string>
#include <vector>
#include <ctime>
#include "../core/transform/hellas.h"
#include "../core/transform/jugoslav.h"

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Shell32.lib")

static const int IDC_INPUT = 1001;
static const int IDC_OUTPUT = 1002;
static const int IDC_MODE_HELLAS = 1010;
static const int IDC_MODE_JUGOSLAV = 1011;
static const int IDC_RUN = 1004;
static const int IDC_CLEAR = 1005;
static const int IDC_SWAP = 1006;
static const int IDC_COPY = 1007;
static const int IDC_DOWNLOAD = 1008;
static const int IDC_SAMPLE_HELLAS = 1020;
static const int IDC_SAMPLE_JUGOSLAV = 1021;
static const int IDC_STATUS = 1030;
static const int IDC_STATUS_PILL = 1031;
static const int IDC_ERROR = 1032;
static const int IDC_INPUT_COUNT = 1033;
static const int IDC_OUTPUT_COUNT = 1034;
static const int IDC_LAST_RUN = 1035;

static HWND gInput = nullptr;
static HWND gOutput = nullptr;
static HWND gRunBtn = nullptr;
static HWND gStatus = nullptr;
static HWND gStatusPill = nullptr;
static HWND gError = nullptr;
static HWND gInputCount = nullptr;
static HWND gOutputCount = nullptr;
static HWND gLastRun = nullptr;
static HWND gHellas = nullptr;
static HWND gJugoslav = nullptr;

static std::wstring gOutputCache;
static COLORREF gStatusColor = RGB(47, 133, 90);

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
    std::vector<wchar_t> buffer(len + 1);
    GetWindowTextW(h, buffer.data(), len + 1);
    return std::wstring(buffer.data());
}

static void setText(HWND h, const std::wstring& text) {
    SetWindowTextW(h, text.c_str());
}

static int countChars(const std::wstring& text) {
    return (int)text.size();
}

static std::wstring nowText() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    wchar_t buf[32];
    swprintf(buf, 32, L"%02u:%02u:%02u", st.wHour, st.wMinute, st.wSecond);
    return buf;
}

static void setStatus(const std::wstring& text, const std::wstring& pill, COLORREF color, const std::wstring& error = L"") {
    setText(gStatus, text);
    setText(gStatusPill, pill);
    gStatusColor = color;
    InvalidateRect(gStatusPill, nullptr, TRUE);
    setText(gError, error);
}

static void updateCounts() {
    setText(gInputCount, std::to_wstring(countChars(getText(gInput))) + L" 字符");
    setText(gOutputCount, std::to_wstring(countChars(getText(gOutput))) + L" 字符");
}

static int currentMode() {
    return SendMessageW(gJugoslav, BM_GETCHECK, 0, 0) == BST_CHECKED ? 1 : 0;
}

static void setMode(int mode) {
    SendMessageW(gHellas, BM_SETCHECK, mode == 0 ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessageW(gJugoslav, BM_SETCHECK, mode == 1 ? BST_CHECKED : BST_UNCHECKED, 0);
}

static void applyOutput(const std::string& out) {
    gOutputCache = utf8ToWide(out);
    setText(gOutput, gOutputCache);
    updateCounts();
}

static void runTransform() {
    SetWindowTextW(gRunBtn, L"处理中...");
    EnableWindow(gRunBtn, FALSE);
    setStatus(L"处理中...", L"工作中", RGB(183, 121, 31));

    std::string input = wideToUtf8(getText(gInput));
    std::string output;
    if (currentMode() == 0) {
        hellasLatin t(input);
        t.transform();
        output = t.getOutput();
    } else {
        serbiaLatin t(input);
        t.transform();
        output = t.getOutput();
    }

    applyOutput(output);
    setStatus(L"转写完成", L"就绪", RGB(47, 133, 90));
    setText(gLastRun, L"上次运行：" + nowText());
    SetWindowTextW(gRunBtn, L"转写");
    EnableWindow(gRunBtn, TRUE);
}

static void clearAll() {
    SetWindowTextW(gInput, L"");
    SetWindowTextW(gOutput, L"");
    setText(gError, L"");
    setText(gStatus, L"等待输入");
    setText(gStatusPill, L"就绪");
    gStatusColor = RGB(47, 133, 90);
    setText(gLastRun, L"上次运行：-");
    updateCounts();
}

static void swapText() {
    std::wstring input = getText(gInput);
    std::wstring output = getText(gOutput);
    setText(gInput, output);
    setText(gOutput, input);
    updateCounts();
}

static void fillSample(bool hellas) {
    if (hellas) {
        setText(gInput, L"Αυτή είναι μια δοκιμή.");
        setMode(0);
    } else {
        setText(gInput, L"Љубав, њега, џем.");
        setMode(1);
    }
    updateCounts();
}

static void copyOutput() {
    std::wstring text = getText(gOutput);
    if (!OpenClipboard(nullptr)) return;
    EmptyClipboard();
    size_t bytes = (text.size() + 1) * sizeof(wchar_t);
    HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, bytes);
    if (mem) {
        void* ptr = GlobalLock(mem);
        memcpy(ptr, text.c_str(), bytes);
        GlobalUnlock(mem);
        SetClipboardData(CF_UNICODETEXT, mem);
    }
    CloseClipboard();
    setStatus(L"已复制输出", L"就绪", RGB(47, 133, 90));
}

static void downloadOutput(bool asJson) {
    std::wstring text = getText(gOutput);
    std::wstring path = asJson ? L"Lacinka_output.json" : L"Lacinka_output.txt";
    HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return;
    std::string data = asJson ? ("{\"output\":\"" + wideToUtf8(text) + "\"}") : wideToUtf8(text);
    DWORD written = 0;
    WriteFile(file, data.c_str(), (DWORD)data.size(), &written, nullptr);
    CloseHandle(file);
    setStatus(L"已下载输出", L"就绪", RGB(47, 133, 90));
}

static LRESULT CALLBACK StatusPillProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);
        HBRUSH brush = CreateSolidBrush(gStatusColor);
        FillRect(hdc, &rc, brush);
        DeleteObject(brush);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        DrawTextW(hdc, getText(hwnd).c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

static void layout(HWND hwnd) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;
    int pad = 16;
    int headerH = 66;
    int footerH = 44;
    int top = pad + headerH;
    int contentH = h - top - footerH - pad;

    if (w >= 1280) {
        int ctrlW = 230;
        int leftW = (w - pad * 4 - ctrlW) / 2;
        int rightW = w - pad * 4 - ctrlW - leftW;
        MoveWindow(gInput, pad, top, leftW, contentH, TRUE);
        MoveWindow(gStatus, 180, 16, 260, 24, TRUE);
        MoveWindow(gStatusPill, 452, 14, 84, 28, TRUE);
        MoveWindow(gHellas, pad * 2 + leftW, top, ctrlW, 36, TRUE);
        MoveWindow(gJugoslav, pad * 2 + leftW, top + 42, ctrlW, 36, TRUE);
        MoveWindow(gRunBtn, pad * 2 + leftW, top + 94, ctrlW, 46, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_SWAP), pad * 2 + leftW, top + 152, ctrlW, 32, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_COPY), pad * 2 + leftW, top + 198, ctrlW / 2 - 6, 30, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_DOWNLOAD), pad * 2 + leftW + ctrlW / 2 + 6, top + 198, ctrlW / 2 - 6, 30, TRUE);
        MoveWindow(gOutput, pad * 3 + leftW + ctrlW, top, rightW, contentH, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_SAMPLE_HELLAS), pad, top + contentH - 34, 92, 26, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_SAMPLE_JUGOSLAV), pad + 100, top + contentH - 34, 92, 26, TRUE);
        MoveWindow(gInputCount, pad + leftW - 90, top + contentH - 34, 80, 24, TRUE);
        MoveWindow(gOutputCount, pad * 3 + leftW + ctrlW + rightW - 90, top + contentH - 34, 80, 24, TRUE);
        MoveWindow(gLastRun, w - 220, 18, 200, 22, TRUE);
        MoveWindow(gError, pad, h - footerH - 30, w - 32, 18, TRUE);
    } else {
        int columnW = w - pad * 2;
        int inputH = 190;
        int ctrlH = 170;
        int outputH = h - top - footerH - pad - inputH - ctrlH - 20;
        MoveWindow(gInput, pad, top, columnW, inputH, TRUE);
        MoveWindow(gHellas, pad, top + inputH + 10, columnW / 2 - 4, 32, TRUE);
        MoveWindow(gJugoslav, pad + columnW / 2 + 4, top + inputH + 10, columnW / 2 - 4, 32, TRUE);
        MoveWindow(gRunBtn, pad, top + inputH + 52, columnW, 42, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_SWAP), pad, top + inputH + 100, columnW, 30, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_COPY), pad, top + inputH + 134, columnW / 2 - 4, 28, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_DOWNLOAD), pad + columnW / 2 + 4, top + inputH + 134, columnW / 2 - 4, 28, TRUE);
        MoveWindow(gOutput, pad, top + inputH + ctrlH, columnW, outputH, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_SAMPLE_HELLAS), pad, top + inputH - 34, 92, 26, TRUE);
        MoveWindow(GetDlgItem(hwnd, IDC_SAMPLE_JUGOSLAV), pad + 100, top + inputH - 34, 92, 26, TRUE);
        MoveWindow(gInputCount, w - 110, top + inputH - 34, 80, 24, TRUE);
        MoveWindow(gOutputCount, w - 110, top + inputH + ctrlH + outputH - 34, 80, 24, TRUE);
        MoveWindow(gStatus, 170, 16, 220, 24, TRUE);
        MoveWindow(gStatusPill, 400, 14, 84, 28, TRUE);
        MoveWindow(gLastRun, 500, 18, 200, 22, TRUE);
        MoveWindow(gError, pad, h - footerH - 30, w - 32, 18, TRUE);
    }
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        CreateWindowW(L"STATIC", L"转写工具", WS_CHILD | WS_VISIBLE, 16, 14, 120, 24, hwnd, nullptr, nullptr, nullptr);
        gStatus = CreateWindowW(L"STATIC", L"等待输入", WS_CHILD | WS_VISIBLE, 140, 14, 220, 24, hwnd, (HMENU)IDC_STATUS, nullptr, nullptr);
        gStatusPill = CreateWindowW(L"STATIC", L"就绪", WS_CHILD | WS_VISIBLE, 0, 0, 80, 26, hwnd, (HMENU)IDC_STATUS_PILL, nullptr, nullptr);
        gLastRun = CreateWindowW(L"STATIC", L"上次运行：-", WS_CHILD | WS_VISIBLE, 0, 0, 160, 24, hwnd, (HMENU)IDC_LAST_RUN, nullptr, nullptr);

        gInput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                                 WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
                                 0, 0, 0, 0, hwnd, (HMENU)IDC_INPUT, nullptr, nullptr);
        gOutput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                                  WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL,
                                  0, 0, 0, 0, hwnd, (HMENU)IDC_OUTPUT, nullptr, nullptr);

        gHellas = CreateWindowW(L"BUTTON", L"Hellas", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
                                0, 0, 0, 0, hwnd, (HMENU)IDC_MODE_HELLAS, nullptr, nullptr);
        gJugoslav = CreateWindowW(L"BUTTON", L"Jugoslav", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                                  0, 0, 0, 0, hwnd, (HMENU)IDC_MODE_JUGOSLAV, nullptr, nullptr);
        SendMessageW(gHellas, BM_SETCHECK, BST_CHECKED, 0);

        gRunBtn = CreateWindowW(L"BUTTON", L"转写", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_RUN, nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"清空", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_CLEAR, nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"交换", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_SWAP, nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"复制", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_COPY, nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"下载TXT", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_DOWNLOAD, nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"希腊示例", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_SAMPLE_HELLAS, nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"塞尔维亚示例", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_SAMPLE_JUGOSLAV, nullptr, nullptr);
        gInputCount = CreateWindowW(L"STATIC", L"0 字符", WS_CHILD | WS_VISIBLE, 0, 0, 80, 20, hwnd, (HMENU)IDC_INPUT_COUNT, nullptr, nullptr);
        gOutputCount = CreateWindowW(L"STATIC", L"0 字符", WS_CHILD | WS_VISIBLE, 0, 0, 80, 20, hwnd, (HMENU)IDC_OUTPUT_COUNT, nullptr, nullptr);
        gError = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE, 16, 0, 100, 20, hwnd, (HMENU)IDC_ERROR, nullptr, nullptr);

        HWND kids[] = {gStatus, gStatusPill, gLastRun, gInput, gOutput, gHellas, gJugoslav, gRunBtn,
                       GetDlgItem(hwnd, IDC_CLEAR), GetDlgItem(hwnd, IDC_SWAP), GetDlgItem(hwnd, IDC_COPY),
                       GetDlgItem(hwnd, IDC_DOWNLOAD), GetDlgItem(hwnd, IDC_SAMPLE_HELLAS), GetDlgItem(hwnd, IDC_SAMPLE_JUGOSLAV),
                       gInputCount, gOutputCount, gError};
        for (HWND h : kids) {
            SendMessageW(h, WM_SETFONT, (WPARAM)font, TRUE);
        }

        SetWindowSubclass(gStatusPill, StatusPillProc, 0, 0);
        setStatus(L"等待输入", L"就绪", RGB(47, 133, 90));
        layout(hwnd);
        updateCounts();
        return 0;
    }
    case WM_SIZE:
        layout(hwnd);
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_MODE_HELLAS:
            if (HIWORD(wParam) == BN_CLICKED) setMode(0);
            return 0;
        case IDC_MODE_JUGOSLAV:
            if (HIWORD(wParam) == BN_CLICKED) setMode(1);
            return 0;
        case IDC_RUN:
            runTransform();
            return 0;
        case IDC_CLEAR:
            clearAll();
            return 0;
        case IDC_SWAP:
            swapText();
            return 0;
        case IDC_COPY:
            copyOutput();
            return 0;
        case IDC_DOWNLOAD:
            downloadOutput(false);
            return 0;
        case IDC_SAMPLE_HELLAS:
            fillSample(true);
            return 0;
        case IDC_SAMPLE_JUGOSLAV:
            fillSample(false);
            return 0;
        case IDC_INPUT:
            if (HIWORD(wParam) == EN_CHANGE) updateCounts();
            return 0;
        case IDC_OUTPUT:
            if (HIWORD(wParam) == EN_CHANGE) updateCounts();
            return 0;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    INITCOMMONCONTROLSEX icc{sizeof(icc), ICC_STANDARD_CLASSES};
    InitCommonControlsEx(&icc);

    const wchar_t CLASS_NAME[] = L"LacinkaWindow";
    WNDCLASSW wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, L"Lacinka",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 780,
        nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
