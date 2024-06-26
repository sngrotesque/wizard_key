#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_CRYPTO
#include <vector>
#include <algorithm>
#include <random>

namespace wmkc {
    namespace crypto {
        LIBWMKC_API void generate_sbox(wByte *sbox);
        LIBWMKC_API void generate_rsbox(wByte *rsbox, wByte *sbox);
    }
}

#endif
#endif
