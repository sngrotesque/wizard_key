#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_DNS
#define WMKC_CPP_DNS
#include <network/socket.hpp>
#include <vector>
#include <sstream>

namespace wmkc {
    namespace net {
        std::string DNS(std::string hostname, std::string dnsServer);
    }
}

#endif
#endif
