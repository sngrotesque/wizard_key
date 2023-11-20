#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_CRYPTO
#include <wmkc_random.hpp>

namespace wmkcCrypto {
    wmkcVoid generate_sbox(wmkcByte *sbox);
    wmkcVoid generate_rsbox(wmkcByte *rsbox, wmkcByte *sbox);
}

#endif
#endif
