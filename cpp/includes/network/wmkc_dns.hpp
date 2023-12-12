#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_DNS
#define WMKC_CPP_DNS
#include <network/wmkc_net.hpp>
#include <vector>
#include <sstream>

namespace wmkcNet {
    std::string wmkcDNS(std::string hostname, std::string dnsServer);
}

#endif
#endif
