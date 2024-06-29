#include <config/wuk.hpp>

#if WUK_SUPPORT
#ifndef WUK_CPP_UTILS
#define WUK_CPP_UTILS

namespace wuk {
    namespace crypto {
        class LIBWUK_API Counter {
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
