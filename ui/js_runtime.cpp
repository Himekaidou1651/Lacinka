#include "js_runtime.h"
#include "resource.h"

#include <initguid.h>
#include <activscp.h>
#include <oleauto.h>
#include <vector>

#include "../core/transform/hellas.h"
#include "../core/transform/jugoslav.h"

static const CLSID LacinkaCLSIDJScript =
    {0xf414c260, 0x6ac0, 0x11cf, {0xb6, 0xd1, 0x00, 0xaa, 0x00, 0xbb, 0xbb, 0x58}};

namespace {

std::string wideToUtf8(const std::wstring& w) {
    if (w.empty()) {
        return {};
    }
    int size = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::vector<char> buffer(size);
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), buffer.data(), size, nullptr, nullptr);
    return std::string(buffer.begin(), buffer.end());
}

std::wstring utf8ToWide(const std::string& s) {
    if (s.empty()) {
        return {};
    }
    int size = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
    std::vector<wchar_t> buffer(size);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), buffer.data(), size);
    return std::wstring(buffer.begin(), buffer.end());
}

std::string transformNative(const std::string& input, int mode) {
    if (mode == 1) {
        serbiaLatin t(input);
        t.transform();
        return t.getOutput();
    }

    hellasLatin t(input);
    t.transform();
    return t.getOutput();
}

bool loadScriptResource(HINSTANCE instance, std::wstring& script) {
    HRSRC resource = FindResourceW(instance, MAKEINTRESOURCEW(IDR_LACINKA_UI_JS), MAKEINTRESOURCEW(10));
    if (!resource) {
        return false;
    }

    HGLOBAL loaded = LoadResource(instance, resource);
    DWORD size = SizeofResource(instance, resource);
    if (!loaded || size == 0) {
        return false;
    }

    const char* bytes = static_cast<const char*>(LockResource(loaded));
    if (!bytes) {
        return false;
    }

    int chars = MultiByteToWideChar(CP_UTF8, 0, bytes, (int)size, nullptr, 0);
    if (chars <= 0) {
        return false;
    }

    std::vector<wchar_t> buffer(chars);
    MultiByteToWideChar(CP_UTF8, 0, bytes, (int)size, buffer.data(), chars);
    script.assign(buffer.begin(), buffer.end());
    return true;
}

class NativeBridge : public IDispatch {
public:
    ULONG STDMETHODCALLTYPE AddRef() override {
        return InterlockedIncrement(&refs);
    }

    ULONG STDMETHODCALLTYPE Release() override {
        ULONG count = InterlockedDecrement(&refs);
        return count;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** object) override {
        if (!object) {
            return E_POINTER;
        }
        if (riid == IID_IUnknown || riid == IID_IDispatch) {
            *object = static_cast<IDispatch*>(this);
            AddRef();
            return S_OK;
        }
        *object = nullptr;
        return E_NOINTERFACE;
    }

    HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo) override {
        if (pctinfo) {
            *pctinfo = 0;
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT, LCID, ITypeInfo**) override {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID, LPOLESTR* names, UINT count, LCID, DISPID* ids) override {
        if (!names || !ids || count == 0) {
            return E_INVALIDARG;
        }

        if (lstrcmpiW(names[0], L"transform") == 0) {
            ids[0] = 1;
            return S_OK;
        }
        return DISP_E_UNKNOWNNAME;
    }

    HRESULT STDMETHODCALLTYPE Invoke(DISPID id, REFIID, LCID, WORD flags, DISPPARAMS* params,
                                      VARIANT* result, EXCEPINFO*, UINT*) override {
        if (id != 1 || !(flags & DISPATCH_METHOD) || !params || params->cArgs < 2) {
            return DISP_E_MEMBERNOTFOUND;
        }

        VARIANT modeValue;
        VARIANT inputValue;
        VariantInit(&modeValue);
        VariantInit(&inputValue);

        HRESULT modeHr = VariantChangeType(&modeValue, &params->rgvarg[0], 0, VT_BSTR);
        HRESULT inputHr = VariantChangeType(&inputValue, &params->rgvarg[1], 0, VT_BSTR);
        if (FAILED(modeHr) || FAILED(inputHr)) {
            VariantClear(&modeValue);
            VariantClear(&inputValue);
            return DISP_E_TYPEMISMATCH;
        }

        int mode = _wtoi(modeValue.bstrVal);
        std::string input = wideToUtf8(inputValue.bstrVal ? inputValue.bstrVal : L"");
        std::wstring output = utf8ToWide(transformNative(input, mode));

        if (result) {
            VariantInit(result);
            result->vt = VT_BSTR;
            result->bstrVal = SysAllocString(output.c_str());
        }

        VariantClear(&modeValue);
        VariantClear(&inputValue);
        return S_OK;
    }

private:
    volatile LONG refs = 1;
};

class ScriptSite : public IActiveScriptSite {
public:
    explicit ScriptSite(NativeBridge* bridge) : native(bridge) {}

    ULONG STDMETHODCALLTYPE AddRef() override {
        return InterlockedIncrement(&refs);
    }

    ULONG STDMETHODCALLTYPE Release() override {
        ULONG count = InterlockedDecrement(&refs);
        return count;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** object) override {
        if (!object) {
            return E_POINTER;
        }
        if (riid == IID_IUnknown || riid == IID_IActiveScriptSite) {
            *object = static_cast<IActiveScriptSite*>(this);
            AddRef();
            return S_OK;
        }
        *object = nullptr;
        return E_NOINTERFACE;
    }

    HRESULT STDMETHODCALLTYPE GetLCID(LCID* lcid) override {
        if (lcid) {
            *lcid = GetUserDefaultLCID();
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetItemInfo(LPCOLESTR name, DWORD mask, IUnknown** unknown, ITypeInfo** typeInfo) override {
        if (typeInfo) {
            *typeInfo = nullptr;
        }
        if (unknown) {
            *unknown = nullptr;
        }

        if (lstrcmpiW(name, L"native") != 0) {
            return TYPE_E_ELEMENTNOTFOUND;
        }

        if ((mask & SCRIPTINFO_IUNKNOWN) && unknown) {
            native->AddRef();
            *unknown = native;
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetDocVersionString(BSTR* version) override {
        if (version) {
            *version = SysAllocString(L"1");
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnScriptTerminate(const VARIANT*, const EXCEPINFO*) override {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnStateChange(SCRIPTSTATE) override {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnScriptError(IActiveScriptError* error) override {
        EXCEPINFO info{};
        DWORD context = 0;
        ULONG line = 0;
        LONG column = 0;
        if (error) {
            error->GetSourcePosition(&context, &line, &column);
            error->GetExceptionInfo(&info);
        }

        wchar_t buffer[512];
        swprintf(buffer, 512, L"JS error at %lu:%ld: %s", line + 1, column + 1,
                 info.bstrDescription ? info.bstrDescription : L"unknown");
        errorText = buffer;
        SysFreeString(info.bstrSource);
        SysFreeString(info.bstrDescription);
        SysFreeString(info.bstrHelpFile);
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnEnterScript() override {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnLeaveScript() override {
        return S_OK;
    }

    std::wstring lastError() const {
        return errorText;
    }

    void clearError() {
        errorText.clear();
    }

private:
    volatile LONG refs = 1;
    NativeBridge* native;
    std::wstring errorText;
};

} // namespace

class JsRuntime::Impl {
public:
    Impl() : site(&native) {}

    ~Impl() {
        if (script) {
            script->Close();
        }
        if (dispatch) {
            dispatch->Release();
        }
        if (script) {
            script->Release();
        }
        if (coInitialized) {
            CoUninitialize();
        }
    }

    bool initialize(HINSTANCE instance) {
        std::wstring scriptText;
        if (!loadScriptResource(instance, scriptText)) {
            error = L"Unable to load embedded ui/script/app.js";
            return false;
        }

        HRESULT co = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        coInitialized = SUCCEEDED(co);
        if (FAILED(co) && co != RPC_E_CHANGED_MODE) {
            error = L"Unable to initialize COM";
            return false;
        }

        HRESULT hr = CoCreateInstance(LacinkaCLSIDJScript, nullptr, CLSCTX_INPROC_SERVER, IID_IActiveScript,
                                      reinterpret_cast<void**>(&script));
        if (FAILED(hr) || !script) {
            error = L"Unable to create JScript engine";
            return false;
        }

        hr = script->SetScriptSite(&site);
        if (FAILED(hr)) {
            error = L"Unable to attach JScript site";
            return false;
        }

        hr = script->AddNamedItem(L"native", SCRIPTITEM_ISVISIBLE);
        if (FAILED(hr)) {
            error = L"Unable to expose native bridge";
            return false;
        }

        IActiveScriptParse* parser = nullptr;
        hr = script->QueryInterface(IID_IActiveScriptParse, reinterpret_cast<void**>(&parser));
        if (FAILED(hr) || !parser) {
            error = L"Unable to access JScript parser";
            return false;
        }

        hr = parser->InitNew();
        if (SUCCEEDED(hr)) {
            EXCEPINFO info{};
            hr = parser->ParseScriptText(scriptText.c_str(), nullptr, nullptr, nullptr, 0, 0,
                                         SCRIPTTEXT_ISVISIBLE, nullptr, &info);
            SysFreeString(info.bstrSource);
            SysFreeString(info.bstrDescription);
            SysFreeString(info.bstrHelpFile);
        }
        parser->Release();

        if (FAILED(hr)) {
            error = site.lastError().empty() ? L"Unable to parse embedded JS" : site.lastError();
            return false;
        }

        hr = script->SetScriptState(SCRIPTSTATE_CONNECTED);
        if (FAILED(hr)) {
            error = site.lastError().empty() ? L"Unable to start embedded JS" : site.lastError();
            return false;
        }

        hr = script->GetScriptDispatch(nullptr, &dispatch);
        if (FAILED(hr) || !dispatch) {
            error = L"Unable to access JScript global object";
            return false;
        }

        initialized = true;
        error.clear();
        return true;
    }

    bool ready() const {
        return initialized && dispatch;
    }

    std::wstring text(const std::wstring& key, const std::wstring& fallback) {
        std::wstring value;
        return callString(L"lacinkaText", {key}, value) ? value : fallback;
    }

    int number(const std::wstring& key, int fallback) {
        VARIANT keyArg;
        VariantInit(&keyArg);
        keyArg.vt = VT_BSTR;
        keyArg.bstrVal = SysAllocString(key.c_str());

        VARIANT result;
        VariantInit(&result);
        bool ok = call(L"lacinkaNumber", &keyArg, 1, &result);
        VariantClear(&keyArg);
        if (!ok) {
            return fallback;
        }

        VARIANT asInt;
        VariantInit(&asInt);
        HRESULT hr = VariantChangeType(&asInt, &result, 0, VT_I4);
        VariantClear(&result);
        if (FAILED(hr)) {
            return fallback;
        }

        int value = asInt.lVal;
        VariantClear(&asInt);
        return value > 0 ? value : fallback;
    }

    std::wstring modeLabel(int mode, const std::wstring& fallback) {
        std::wstring value;
        return callString(L"lacinkaModeLabel", {std::to_wstring(mode)}, value) ? value : fallback;
    }

    std::wstring sample(int mode, const std::wstring& fallback) {
        std::wstring value;
        return callString(L"lacinkaSample", {std::to_wstring(mode)}, value) ? value : fallback;
    }

    std::wstring color(const std::wstring& key, const std::wstring& fallback) {
        std::wstring value;
        return callString(L"lacinkaColor", {key}, value) ? value : fallback;
    }

    std::string transform(const std::string& input, int mode) {
        if (!ready()) {
            return transformNative(input, mode);
        }

        std::wstring output;
        if (!callString(L"lacinkaTransform", {utf8ToWide(input), std::to_wstring(mode)}, output)) {
            return transformNative(input, mode);
        }
        return wideToUtf8(output);
    }

    std::wstring lastError() const {
        return error.empty() ? site.lastError() : error;
    }

private:
    bool callString(const std::wstring& functionName, const std::vector<std::wstring>& args, std::wstring& output) {
        std::vector<VARIANT> normal(args.size());
        for (size_t i = 0; i < args.size(); ++i) {
            VariantInit(&normal[i]);
            normal[i].vt = VT_BSTR;
            normal[i].bstrVal = SysAllocString(args[i].c_str());
        }

        VARIANT result;
        VariantInit(&result);
        bool ok = call(functionName, normal.data(), (UINT)normal.size(), &result);

        for (VARIANT& arg : normal) {
            VariantClear(&arg);
        }

        if (!ok) {
            return false;
        }

        VARIANT asString;
        VariantInit(&asString);
        HRESULT hr = VariantChangeType(&asString, &result, 0, VT_BSTR);
        VariantClear(&result);
        if (FAILED(hr)) {
            VariantClear(&asString);
            return false;
        }

        output = asString.bstrVal ? asString.bstrVal : L"";
        VariantClear(&asString);
        return true;
    }

    bool call(const std::wstring& functionName, VARIANT* normalArgs, UINT argCount, VARIANT* result) {
        if (!ready()) {
            return false;
        }

        site.clearError();
        OLECHAR* name = const_cast<OLECHAR*>(functionName.c_str());
        DISPID id = 0;
        HRESULT hr = dispatch->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &id);
        if (FAILED(hr)) {
            error = std::wstring(L"Missing JS function: ") + functionName;
            return false;
        }

        std::vector<VARIANT> reversed(argCount);
        for (UINT i = 0; i < argCount; ++i) {
            VariantInit(&reversed[i]);
            VariantCopy(&reversed[i], &normalArgs[argCount - i - 1]);
        }

        DISPPARAMS params{};
        params.rgvarg = reversed.empty() ? nullptr : reversed.data();
        params.cArgs = argCount;

        EXCEPINFO info{};
        hr = dispatch->Invoke(id, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, result, &info, nullptr);

        for (VARIANT& arg : reversed) {
            VariantClear(&arg);
        }

        if (FAILED(hr)) {
            error = site.lastError().empty() ? (std::wstring(L"JS call failed: ") + functionName) : site.lastError();
            SysFreeString(info.bstrSource);
            SysFreeString(info.bstrDescription);
            SysFreeString(info.bstrHelpFile);
            return false;
        }

        error.clear();
        return true;
    }

    bool initialized = false;
    bool coInitialized = false;
    NativeBridge native;
    ScriptSite site;
    IActiveScript* script = nullptr;
    IDispatch* dispatch = nullptr;
    std::wstring error;
};

JsRuntime::JsRuntime() : impl(new Impl()) {}

JsRuntime::~JsRuntime() {
    delete impl;
}

bool JsRuntime::initialize(HINSTANCE instance) {
    return impl->initialize(instance);
}

bool JsRuntime::ready() const {
    return impl->ready();
}

std::wstring JsRuntime::text(const std::wstring& key, const std::wstring& fallback) {
    return impl->text(key, fallback);
}

int JsRuntime::number(const std::wstring& key, int fallback) {
    return impl->number(key, fallback);
}

std::wstring JsRuntime::modeLabel(int mode, const std::wstring& fallback) {
    return impl->modeLabel(mode, fallback);
}

std::wstring JsRuntime::sample(int mode, const std::wstring& fallback) {
    return impl->sample(mode, fallback);
}

std::wstring JsRuntime::color(const std::wstring& key, const std::wstring& fallback) {
    return impl->color(key, fallback);
}

std::string JsRuntime::transform(const std::string& input, int mode) {
    return impl->transform(input, mode);
}

std::wstring JsRuntime::lastError() const {
    return impl->lastError();
}
