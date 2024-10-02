#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_CRYPTO
#include <vector>
#include <algorithm>
#include <random>

namespace wuk {
    namespace crypto {
        LIBWUK_API void generate_sbox(wByte *sbox);
        LIBWUK_API void generate_rsbox(wByte *rsbox, wByte *sbox);
    }
}

#endif
#endif
