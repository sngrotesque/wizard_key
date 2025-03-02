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

/* 
* State:
* |------------------------------------------|
* | 00000000 | 00000001 | 00000002 | 00000003|
* |------------------------------------------|
* | 00000004 | 00000005 | 00000006 | 00000007|
* |------------------------------------------|
* | 00000008 | 00000009 | 0000000a | 0000000b|
* |------------------------------------------|
* | 0000000c | 0000000d | 0000000e | 0000000f|
* |------------------------------------------|
*/

namespace wuk {
    namespace crypto {
        constexpr wU32 WUK_SSC_KSLEN    = 64; // key stream length
        constexpr wU32 WUK_SSC_KEYLEN   = 32; // key length
        constexpr wU32 WUK_SSC_IVLEN    = 16; // Initialization vector length

        // SN Stream Cipher
        class LIBWUK_API SSC {
        private:
            wByte keystream[WUK_SSC_KSLEN]{};

            wByte root_key[WUK_SSC_KEYLEN]{};
            wByte root_iv[WUK_SSC_IVLEN]{};
            wuk::crypto::Counter counter;

            wU32 *state = reinterpret_cast<wU32 *>(this->keystream);

        private:
            void keystream_sub_bytes();
            void keystream_bit_swap();
            void keystream_4value_mixture();
            void keystream_oblique_angle_mixing();

        private:
            void keystream_init();

        public: // temporary public
            void keystream_update();

        public:
            SSC() = default;
            SSC(const wByte *key, const wByte *iv, const wuk::crypto::Counter counter);

        public:
            void xcrypt(wByte *buffer, wSize length);
            const wByte *get_keystream() const;
        };
    }
}

#endif
