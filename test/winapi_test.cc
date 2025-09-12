#include <core/WukConfig.hh>
#include <core/WukException.hh>

#ifndef WUK_PLATFORM_WINOS
#   error "This code must be executed on the Windows system."
#endif
#include <windows.h>

#include <optional>
#include <iostream>
#include <algorithm>

inline wuk::f64 lerp(wuk::i32 min_val, wuk::i32 max_val, wuk::f64 val)
{
    if ((val < 0) || (val > 100) || (min_val >= max_val)) {
        return -1;
    }
    return min_val + (max_val - min_val) * val / 100.0;
}

static inline DWORD err_code()
{
    return GetLastError();
}

static inline std::string err_message(DWORD code)
{
    char *msg_buf = nullptr;

    DWORD len = FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                                FORMAT_MESSAGE_FROM_SYSTEM     |
                                FORMAT_MESSAGE_IGNORE_INSERTS,
                                nullptr,
                                code,
                                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                (LPSTR)&msg_buf,
                                0,
                                nullptr);

    std::string result((len && msg_buf) ? std::string(msg_buf) : "Unknown error");
    if(msg_buf) {
        LocalFree(msg_buf);
    }
    return result;
}

inline std::optional<std::wstring> to_wstring(std::nullptr_t)
{
    return std::nullopt;
}

inline std::optional<std::wstring> to_wstring(const char* s)
{
    if (!s) return std::nullopt; // 空指针
    if (*s == '\0') return std::nullopt; // 空字符串
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, s, -1, nullptr, 0);
    std::wstring wstr(size_needed - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s, -1, wstr.data(), size_needed);
    return wstr;
}

inline std::optional<std::wstring> to_wstring(const wchar_t* ws)
{
    if (!ws) return std::nullopt;
    if (*ws == L'\0') return std::nullopt;
    return std::wstring(ws);
}

inline std::optional<std::wstring> to_wstring(const std::string& s)
{
    if (s.empty()) return std::nullopt;
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    std::wstring wstr(size_needed, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), wstr.data(), size_needed);
    return wstr;
}

inline std::optional<std::wstring> to_wstring(const std::wstring& ws)
{
    if (ws.empty()) return std::nullopt;
    return ws;
}

template<typename T1, typename T2>
void transparent_window(const T1& className, const T2& windowName, wuk::f64 bAlpha)
{
    BYTE alpha = static_cast<BYTE>(lerp(0, 255, bAlpha * 100));
    if (alpha == -1) {
        throw wuk::Exception(wuk::Error::ERR, "transparent_window",
            "The value of 'bAlpha' must be 0 to 1.");
    }
    auto wClassOpt = to_wstring(className);
    auto wNameOpt  = to_wstring(windowName);

    const wchar_t* wClassPtr = wClassOpt ? wClassOpt->c_str() : nullptr;
    const wchar_t* wNamePtr  = wNameOpt  ? wNameOpt->c_str()  : nullptr;

    HWND handle = FindWindowW(wClassPtr, wNamePtr);
    if (!handle) {
        throw wuk::Exception(wuk::Error::ERR, "transparent_window",
            "The handle was not found. Please check the window name.");
    }

    if (!SetWindowLongW(handle, GWL_EXSTYLE, GetWindowLongW(handle, GWL_EXSTYLE) | WS_EX_LAYERED)) {
        auto code = err_code();
        throw wuk::Exception(code, "transparent_window", err_message(code));
    }

    if (!SetLayeredWindowAttributes(handle, 0, alpha, LWA_ALPHA)) {
        auto code = err_code();
        throw wuk::Exception(code, "transparent_window", err_message(code));
    }
}

int main()
{
    try {
        transparent_window(L"UnrealWindow", L"卡拉彼丘  ", 0.5);
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
