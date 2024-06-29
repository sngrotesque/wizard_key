#include <config/wuk.hpp>

#if WUK_SUPPORT
#ifndef WUK_CPP_BINASCII
#define WUK_CPP_BINASCII
#include <config/exception.hpp>

namespace wuk {
    class LIBWUK_API Binascii {
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

#endif /* WUK_BINASCII */
#endif /* WUK_SUPPORT */
