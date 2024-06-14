#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_CRYPTO
#include <vector>
#include <algorithm>
#include <random>

namespace wmkc {
    namespace crypto {
        void generateSbox(wByte *sbox);
        void generateRSbox(wByte *rsbox, wByte *sbox);
    }
}

#endif
#endif
