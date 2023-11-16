#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_BASE
#define WMKC_CPP_BASE
#include <wmkc_base64.h>
#include <wmkc_exception.hpp>

class wmkcBase64 {
    private:
        wmkcErr_obj error;

    public:
        std::string encode(std::string content);
        std::string decode(std::string content);
};

#endif /* WMKC_CPP_BASE */
#endif /* WMKC_SUPPORT */
