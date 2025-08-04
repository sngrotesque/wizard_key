#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#if defined(WUK_PLATFORM_WINOS)
#   include <WS2tcpip.h>
#else
#   include <errno.h>
#   include <netdb.h>
#endif

#ifndef NETERROR
#   ifdef WUK_PLATFORM_WINOS
#       define NETERROR SOCKET_ERROR
#   else
#       define NETERROR EOF
#   endif
#endif

namespace wuk::net {
    class SystemError {
    public:
        // 获取当前错误码
        static inline int code()
        {
#           if defined(WUK_PLATFORM_WINOS)
            return WSAGetLastError();  // 优先使用 Winsock 错误
#           else
            return errno;
#           endif
        }

        // 获取指定错误码的描述
        static inline std::string message(int code)
        {
#           if defined(WUK_PLATFORM_WINOS)
            char *msg_buf = nullptr;
            DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;

            DWORD len = FormatMessageA(flags,
                nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPSTR)&msg_buf, 0, nullptr);

            std::string result = (len && msg_buf) ? std::string(msg_buf) : "Unknown error";
            if(msg_buf) {
                LocalFree(msg_buf);
            }
            return result;
#           else
            return std::string(strerror(code));
#           endif
        }

        // 获取当前错误码的描述
        static inline std::string last_message()
        {
            return message(code());
        }
    };

}  // namespace wuk::net
#endif
