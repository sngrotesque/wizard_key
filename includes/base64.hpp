#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_BASE
#define WMKC_CPP_BASE
#include <config/exception.hpp>
#include <memory.hpp>

#define BASE64_PAD '='

namespace wmkc {
    class Base64 {
        private:
            wSize encode_size(wSize size);
            wSize decode_size(std::string content);
        public:
            std::string encode(std::string content);
            std::string decode(std::string content);
    };
}

#endif /* WMKC_CPP_BASE */
#endif /* WMKC_SUPPORT */
