#pragma once

#include <windows.h>
#include <string>

class JsRuntime {
public:
    JsRuntime();
    ~JsRuntime();

    bool initialize(HINSTANCE instance);
    bool ready() const;

    std::wstring text(const std::wstring& key, const std::wstring& fallback);
    int number(const std::wstring& key, int fallback);
    std::wstring modeLabel(int mode, const std::wstring& fallback);
    std::wstring sample(int mode, const std::wstring& fallback);
    std::wstring color(const std::wstring& key, const std::wstring& fallback);
    std::string transform(const std::string& input, int mode);
    std::wstring lastError() const;

private:
    class Impl;
    Impl* impl;
};
