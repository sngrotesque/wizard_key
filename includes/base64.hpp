#include <config/wuk.hpp>

#if WUK_SUPPORT
#ifndef WUK_BASE64_CPP
#define WUK_BASE64_CPP
#include <config/exception.hpp>
#include <memory.hpp>

namespace wuk {
    class LIBWUK_API Base64 {
    private:
        wSize get_encode_length(wSize length);
        wSize get_decode_length(wSize length);
    public:
        bool strict_mode = false;
        char *encode(const wByte *buffer, wSize &length);
        wByte *decode(const char *buffer, wSize &length);

        std::string encode(std::string buffer);
        std::string decode(std::string buffer);
    };
}

#endif
#endif
