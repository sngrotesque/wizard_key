#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>

#if defined(WUK_PLATFORM_LINUX)
#include <errno.h>
#include <netdb.h>
#elif defined(WUK_PLATFORM_WINOS)
#include <WS2tcpip.h>
#endif

namespace wuk {
    namespace net {
        class LIBWUK_API Exception {
        private:
            std::string error_message;

            std::string get_msg(wI32 code, const char *f, const char *m)
            {
#               ifdef WUK_STD_CPP_20
                    return std::format("{0}[{1}]: {2}", f, code, m);
#               else
                return std::string{f} + "[" + std::to_string(code) + \
                        "]: " + m;
#               endif
            }

            wI32 get_code() const noexcept
            {
#               if defined(WUK_PLATFORM_WINOS)
                return WSAGetLastError();
#               elif defined(WUK_PLATFORM_LINUX)
                return errno;
#               endif
            }

            std::string get_msg_from_code(wI32 code)
            {
                char *message = nullptr;

#               if defined(WUK_PLATFORM_WINOS)
                DWORD dwFlag =  FORMAT_MESSAGE_ALLOCATE_BUFFER |
                                FORMAT_MESSAGE_FROM_SYSTEM     |
                                FORMAT_MESSAGE_IGNORE_INSERTS;

                FormatMessageA(dwFlag, nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                            reinterpret_cast<LPSTR>(&message), 0, nullptr);
#               elif defined(WUK_PLATFORM_LINUX)
                message = strerror(code);
#               endif

                std::string result{message};
#               ifdef WUK_PLATFORM_WINOS
                LocalFree(message);
#               endif

                return result;
            }

        public:
            Exception() = default;

            Exception(wI32 code, const char *function, const char *message)
            {
                this->error_message = this->get_msg(code, function, message);
            }

            Exception(wuk::Error code, const char *function, const char *message)
            {
                this->error_message = this->get_msg(static_cast<wI32>(code), function, message);
            }

            Exception(wI32 code, const char *function)
            {
                this->error_message = this->get_msg(code, function, this->get_msg_from_code(code).c_str());
            }

            Exception(const char *function, const char *message)
            {
                this->error_message = this->get_msg(this->get_code(), function, message);
            }

            Exception(const char *function)
            {
                wI32 code = this->get_code();
                this->error_message = this->get_msg(code, function, this->get_msg_from_code(code).c_str());
            }

            const char *what() const noexcept
            {
                return this->error_message.c_str();
            }
        };
    }
}

#endif
