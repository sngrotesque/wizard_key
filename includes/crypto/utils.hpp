#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_UTILS
#define WMKC_CPP_UTILS

namespace wmkc {
    namespace crypto {
        class LIBWMKC_API Counter {
            public:
                wByte counter[16];
                Counter();
                Counter(wByte *nonce, wU32 size, wSize begin);
                Counter(const char *nonce, wU32 size, wSize begin);
                Counter(std::string nonce, wSize begin);
        };
    }
}

#endif
#endif
