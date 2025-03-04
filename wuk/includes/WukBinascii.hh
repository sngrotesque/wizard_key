#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>

namespace wuk {
    class LIBWUK_API Binascii {
    private:
        wByte to_top(wByte c);
        wByte to_bot(wByte c);

    public:
        Binascii() = default;

        char *b2a_hex(const wByte *buffer, wSize &length);
        wByte *a2b_hex(const char *buffer, wSize &length);

        std::string b2a_hex(std::string buffer);
        std::string a2b_hex(std::string buffer);
    };
}

#endif /* WUK_SUPPORT */
