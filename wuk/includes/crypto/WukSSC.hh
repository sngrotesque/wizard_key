#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <WukMemory.hh>
#include <config/WukException.hh>
#include <crypto/WukUtils.hh>

/**
 * Key stream structure:
 * 0 1 2 3 4 5 6 7 8 9 a b c d e f (seq, does not contain)
 * - - - - - - - - - - - - - - - -
 * K K K K K K K K K K K K K K K K
 * K K K K K K K K K K K K K K K K
 * C C C C C C C C C C C C C C C C
 * I I I I I I I I I I I I I I I I
 */

namespace wuk {
    namespace crypto {
        constexpr wU32 WUK_SSC_KSLEN    = 64; // key stream length
        constexpr wU32 WUK_SSC_KEYLEN   = 32; // key length
        constexpr wU32 WUK_SSC_IVLEN    = 16; // Initialization vector length

        // SN Stream Cipher
        class LIBWUK_API SSC {
        public:
        // private:
            wByte keystream[WUK_SSC_KSLEN]{};

            wByte root_key[WUK_SSC_KEYLEN]{};
            wByte root_iv[WUK_SSC_IVLEN]{};
            wuk::crypto::Counter counter;

            wU32 *state = nullptr;

        // private:
            void keystream_init();

            void keystream_update();

        // public:
            SSC() = default;
            SSC(const wByte *key, const wByte *iv, const wuk::crypto::Counter counter);

        // public:
            void xcrypt(wByte *buffer, wSize length);
        };
    }
}

#endif
