#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_BINASCII
#define WMKC_CPP_BINASCII
#include <config/exception.hpp>

namespace wmkc {
    class LIBWMKC_API Binascii {
    private:
        wByte to_top(wByte c);
        wByte to_bot(wByte c);
    public:
        char *b2a_hex(const wByte *buffer, wSize &length);
        wByte *a2b_hex(const char *buffer, wSize &length);

        std::string b2a_hex(std::string buffer);
        std::string a2b_hex(std::string buffer);
    };
}

#endif /* WMKC_BINASCII */
#endif /* WMKC_SUPPORT */
