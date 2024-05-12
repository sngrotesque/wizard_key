#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_BINASCII
#define WMKC_CPP_BINASCII
#include <config/exception.hpp>

namespace wmkc {
    class binascii {
        private:
            wByte toTop(wByte c);
            wByte toBot(wByte c);
        public:
            std::string b2a_hex(std::string content);
            std::string a2b_hex(std::string content);
    };
}


#endif /* WMKC_BINASCII */
#endif /* WMKC_SUPPORT */
