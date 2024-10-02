#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_DNS
#define WUK_CPP_DNS
#include <network/WukSocket.hh>
#include <vector>
#include <sstream>

// https://learn.microsoft.com/en-us/windows/win32/dns/dns-standards-documents
// 需要根据RFC 1035标准来构造和解析DNS报文

namespace wuk {
    namespace net {
        LIBWUK_API std::string DNS(std::string hostname, std::string dnsServer);
    }
}

#endif
#endif
