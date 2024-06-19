#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_BASE64
#define WMKC_CPP_BASE64
#include <config/exception.hpp>
#include <memory.hpp>

// Intel Base64 dll_export...
/*
* https://github.com/intel/base64/blob/master/src/base64.h#L35
* 
* #ifndef LIBBASE64_API
* #  ifdef base64_EXPORTS
* #    ifdef _WIN32
* #      define LIBBASE64_API     __declspec(dllexport)
* #    elif defined(__ELF__)
* #      define LIBBASE64_API     __attribute__((visibility("protected")))
* #    else
* #      define LIBBASE64_API     __attribute__((visibility("default")))
* #    endif
* #  else
* #    ifdef _WIN32
* #      define LIBBASE64_API     __declspec(dllimport)
* #    else
* #      define LIBBASE64_API     __attribute__((visibility("default")))
* #    endif
* #  endif
* #endif
*/

namespace wmkc {
    class Base64 {
    private:
        wSize get_encode_length(wSize length);
        wSize get_decode_length(const wByte *buffer, wSize length);
    public:
        char *encode(const wByte *buffer, wSize &length);
        wByte *decode(const char *buffer, wSize &length);

        std::string encode(std::string buffer);
        std::string decode(std::string buffer);
    };
}

#endif
#endif
