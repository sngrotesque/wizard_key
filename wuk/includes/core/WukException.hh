#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <fmt/core.h>
#include <sstream>
#include <string>

namespace wuk {
    enum class Error {
        OK     = 0, // 一切正常，无异常
        ERR    = 1, // 通用的异常
        NPTR   = 2, // 调用空指针异常（Null pointer）
        NODAT  = 3, // 空数据异常（No data）
        MEMORY = 4, // 内存管理异常（Memory)
        FNOTF  = 5, // 文件未找到（File not found）
        FTOFO  = 6, // 文件无法打开（Failed to file opening）
        UNIMPL = 7, // 功能还未实现（Unimplemented）

        TIMEO  = 30, // 超时（网络超时，响应超时等）
    };

    class LIBWUK_API Exception {
    private:
        wuk::i32    code = 0;
        std::string func;
        std::string msg;
        std::string err_msg;

    private:
        void set(wuk::i32 code, const std::string &func, const std::string &message) noexcept
        {
            this->code = code;
            this->func = func;
            this->msg = message;
            this->err_msg = fmt::format("{0}[{1}]: {2}", func, code, message);
        }

    public:
        template <typename T>
        Exception(const T &code, const std::string &function, const std::string &message) noexcept
        {
            this->set(static_cast<wuk::i32>(code), function, message);
        }

        inline wuk::i32 get_err_code() const noexcept
        {
            return this->code;
        }

        inline std::string get_err_func() const noexcept
        {
            return this->func;
        }

        inline std::string get_err_msg() const noexcept
        {
            return this->msg;
        }

        inline std::string what() const noexcept
        {
            return this->err_msg;
        }
    };
}

#endif /* WUK_SUPPORT */
