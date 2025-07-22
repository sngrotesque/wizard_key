#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <network/WukNetwork.hh>

namespace wuk {
    namespace net {
        enum class SD_SW {
#           if defined(WUK_PLATFORM_LINUX)
            RECV = SHUT_RD,
            SEND = SHUT_WR,
            BOTH = SHUT_RDWR
#           elif defined(WUK_PLATFORM_WINOS)
            RECV = SD_RECEIVE,
            SEND = SD_SEND,
            BOTH = SD_BOTH
#           endif
        };

        class LIBWUK_API SockOpt {
        public:
            char *val;
            socklen_t val_len;

            SockOpt();
            SockOpt(char *val, socklen_t val_len);
            SockOpt(socklen_t val);
            SockOpt(std::string val);
        };
    }
}

#endif
