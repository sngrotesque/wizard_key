#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>
#include <WukBuffer.hh>

namespace wuk {
    class LIBWUK_API Base64 {
    private:
        bool strict_mode;

    private:
        wSize get_encode_length(wSize length);
        wSize get_decode_length(wSize length);

    public:
        Base64(bool strict_mode = false);

    public:
        char *encode(const wByte *buffer, wSize &length);
        wByte *decode(const char *buffer, wSize &length);

        std::string encode(std::string buffer);
        std::string decode(std::string buffer);

        wuk::Buffer encode(wuk::Buffer buffer);
        wuk::Buffer decode(wuk::Buffer buffer);
    };
}

#endif
