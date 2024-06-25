#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_DNS
#define WMKC_CPP_DNS
#include <network/socket.hpp>
#include <vector>
#include <sstream>

// https://learn.microsoft.com/en-us/windows/win32/dns/dns-standards-documents
// 需要根据RFC 1035标准来构造和解析DNS报文

namespace wmkc {
    namespace net {
        LIBWMKC_API std::string DNS(std::string hostname, std::string dnsServer);
    }
}

#endif
#endif
