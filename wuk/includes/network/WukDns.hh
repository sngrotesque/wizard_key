#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <network/WukNetwork.hh>
#include <network/WukIPEndPoint.hh>

namespace wuk {
    namespace net {
        class LIBWUK_API Dns {
        public:
            Dns() = default;
            IPEndPoint get_host_address(std::string server_name);
            IPEndPoint get_addr_info(std::string server_name, std::string service_name);
            IPEndPoint get_name_info(ADDRINFO content);
        };
    }
}

#endif
