#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_BASE64_CPP
#define WMKC_BASE64_CPP
#include <config/exception.hpp>
#include <memory.hpp>

namespace wmkc {
    class LIBWMKC_API Base64 {
    private:
        wSize get_encode_length(wSize length);
        wSize get_decode_length(wSize length);
    public:
        wBool strict_mode = false;
        char *encode(const wByte *buffer, wSize &length);
        wByte *decode(const char *buffer, wSize &length);

        std::string encode(std::string buffer);
        std::string decode(std::string buffer);
    };
}

#endif
#endif
