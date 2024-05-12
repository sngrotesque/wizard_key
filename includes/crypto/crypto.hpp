#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_CRYPTO
#include <vector>
#include <algorithm>
#include <random>

namespace wmkc {
    namespace crypto {
        wVoid generateSbox(wByte *sbox);
        wVoid generateRSbox(wByte *rsbox, wByte *sbox);
    }
}

#endif
#endif
