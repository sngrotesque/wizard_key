#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>
#include <WukBuffer.hh>

namespace wuk {
    class LIBWUK_API Binascii {
    public:
        Binascii() = default;

        char *b2a_hex(const wByte *buffer, wSize &length);
        wByte *a2b_hex(const char *buffer, wSize &length);

        std::string b2a_hex(std::string buffer);
        std::string a2b_hex(std::string buffer);

        wuk::Buffer b2a_hex(wuk::Buffer buffer);
        wuk::Buffer a2b_hex(wuk::Buffer buffer);
    };
}

#endif /* WUK_SUPPORT */
