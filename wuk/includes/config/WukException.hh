#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <sstream>
#include <cerrno>

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
        wI32 code {wuk::Error::OK};
        std::string func;
        std::string msg;
        std::string err_msg;

    private:
        void set(wI32 code, const std::string &func, const std::string &message)
        {
            auto get_err_message = [&]() -> std::string {
                std::stringstream ss;
                ss  << func
                    << "[" << std::to_string(code) << "]: "
                    << message;
                return ss.str();
            };

            this->code = code;
            this->func = func;
            this->msg = message;
            this->err_msg = get_err_message();
        }

    public:
        template <typename T>
        Exception(T code, const std::string &function, const std::string &message)
        {
            this->set(static_cast<wI32>(code), function, message);
        }

        inline const wI32 &get_err_code() const noexcept
        {
            return this->code;
        }

        inline const std::string &get_err_func() const noexcept
        {
            return this->func;
        }

        inline const std::string &get_err_msg() const noexcept
        {
            return this->msg;
        }

        inline const std::string &what() const noexcept
        {
            return this->err_msg;
        }
    };
}

#endif /* WUK_SUPPORT */
