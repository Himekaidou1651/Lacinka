#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include "../core/transform/hellas.h"
#include "../core/transform/jugoslav.h"

#pragma comment(lib, "Comctl32.lib")

static const int IDC_INPUT = 1001;
static const int IDC_OUTPUT = 1002;
static const int IDC_MODE = 1003;
static const int IDC_RUN = 1004;
static const int IDC_CLEAR = 1005;

static HWND gInput = nullptr;
static HWND gOutput = nullptr;
static HWND gMode = nullptr;

static std::string wideToUtf8(const std::wstring& w) {
    if (w.empty()) return {};
    int size = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::string out(size, '\0');
    std::vector<char> buffer(size);
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), buffer.data(), size, nullptr, nullptr);
    out.assign(buffer.begin(), buffer.end());
    return out;
}

static std::wstring utf8ToWide(const std::string& s) {
    if (s.empty()) return {};
    int size = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
    std::vector<wchar_t> buffer(size);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), buffer.data(), size);
    return std::wstring(buffer.begin(), buffer.end());
}

static std::wstring getWindowTextString(HWND h) {
    int len = GetWindowTextLengthW(h);
    std::wstring text(len, L'\0');
    std::vector<wchar_t> buffer(len + 1);
    GetWindowTextW(h, buffer.data(), len + 1);
    return std::wstring(buffer.data());
}

static void setWindowTextString(HWND h, const std::wstring& text) {
    SetWindowTextW(h, text.c_str());
}

static void runTransform() {
    std::wstring wideInput = getWindowTextString(gInput);
    std::string input = wideToUtf8(wideInput);
    int mode = (int)SendMessageW(gMode, CB_GETCURSEL, 0, 0);

    std::string output;
    if (mode == 0) {
        hellasLatin t(input);
        t.transform();
        output = t.getOutput();
    } else {
        serbiaLatin t(input);
        t.transform();
        output = t.getOutput();
    }

    setWindowTextString(gOutput, utf8ToWide(output));
}

static void clearFields() {
    SetWindowTextW(gInput, L"");
    SetWindowTextW(gOutput, L"");
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        CreateWindowW(L"STATIC", L"输入", WS_CHILD | WS_VISIBLE, 16, 12, 80, 20, hwnd, nullptr, nullptr, nullptr);
        gMode = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
                              16, 36, 200, 200, hwnd, (HMENU)IDC_MODE, nullptr, nullptr);
        SendMessageW(gMode, CB_ADDSTRING, 0, (LPARAM)L"Hellas");
        SendMessageW(gMode, CB_ADDSTRING, 0, (LPARAM)L"Jugoslav");
        SendMessageW(gMode, CB_SETCURSEL, 0, 0);

        gInput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                                 WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VSCROLL,
                                 16, 70, 520, 180, hwnd, (HMENU)IDC_INPUT, nullptr, nullptr);
        gOutput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                                  WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY | WS_VSCROLL,
                                  16, 280, 520, 180, hwnd, (HMENU)IDC_OUTPUT, nullptr, nullptr);
        HWND runBtn = CreateWindowW(L"BUTTON", L"转写", WS_CHILD | WS_VISIBLE,
                                    240, 36, 90, 28, hwnd, (HMENU)IDC_RUN, nullptr, nullptr);
        HWND clearBtn = CreateWindowW(L"BUTTON", L"清空", WS_CHILD | WS_VISIBLE,
                                      340, 36, 90, 28, hwnd, (HMENU)IDC_CLEAR, nullptr, nullptr);

        SendMessageW(gInput, WM_SETFONT, (WPARAM)font, TRUE);
        SendMessageW(gOutput, WM_SETFONT, (WPARAM)font, TRUE);
        SendMessageW(gMode, WM_SETFONT, (WPARAM)font, TRUE);
        SendMessageW(runBtn, WM_SETFONT, (WPARAM)font, TRUE);
        SendMessageW(clearBtn, WM_SETFONT, (WPARAM)font, TRUE);
        return 0;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_RUN:
            runTransform();
            return 0;
        case IDC_CLEAR:
            clearFields();
            return 0;
        }
        break;
    case WM_SIZE:
        return 0;
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
        CW_USEDEFAULT, CW_USEDEFAULT, 580, 520,
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
