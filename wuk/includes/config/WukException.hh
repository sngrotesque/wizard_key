#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <stdexcept>
#include <cerrno>

#ifdef WUK_STD_CPP_20
#   include <format>
#else
#   if defined(WUK_PLATFORM_WINOS) && defined(_MSC_VER)
#       pragma comment(lib, "msvcprt")
#   endif
#endif

namespace wuk {
    typedef enum : wI32 {
        OK     = 0, // 一切正常，无异常
        ERR    = 1, // 一切暂时无法定义的异常
        NPTR   = 2, // 调用空指针异常
        NODAT  = 3, // 空数据异常
        MEMORY = 4, // 内存管理异常
        FNOTF  = 5, // 文件未找到（File not found）
    } Error;

    class LIBWUK_API Exception {
    private:
        std::string _msg;

        std::string get_msg(wI32 code, const char *f, const char *m)
        {
#           ifdef WUK_STD_CPP_20
            return std::format("{0}[{1}]: {2}", f, code, m);
#           else
            return std::string{f} + "[" + std::to_string(code) + \
                         "]: " + m;
#           endif
        }

    public:
        Exception() = default;

        Exception(wuk::Error code, const char *function, const char *message)
        {
            this->_msg = this->get_msg(static_cast<wI32>(code), function, message);
        }

        Exception(wI32 code, const char *function, const char *message)
        {
            this->_msg = this->get_msg(code, function, message);
        }

        const char *what() const noexcept
        {
            return this->_msg.c_str();
        }
    };
}

#endif /* WUK_SUPPORT */
