#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_BASE
#define WMKC_CPP_BASE
#include <wmkc_exception.hpp>
#include <wmkc_memory.hpp>

// Base64的填充值
#define BASE64_PAD '='

class wmkcBase64 {
    private:
        wmkcSize encode_size(wmkcSize size);
        wmkcSize decode_size(std::string content);
    public:
        std::string encode(std::string content);
        std::string decode(std::string content);
};

#endif /* WMKC_CPP_BASE */
#endif /* WMKC_SUPPORT */
