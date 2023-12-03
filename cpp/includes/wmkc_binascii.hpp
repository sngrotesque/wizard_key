#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_BINASCII
#define WMKC_CPP_BINASCII
#include <wmkc_exception.hpp>

class wmkcBinascii {
    private:
        wmkcByte toTop(wmkcByte c);
        wmkcByte toBot(wmkcByte c);
    public:
        std::string b2a_hex(std::string content);
        std::string a2b_hex(std::string content);
};

#endif /* WMKC_BINASCII */
#endif /* WMKC_SUPPORT */
