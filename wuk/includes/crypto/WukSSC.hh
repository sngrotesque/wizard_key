#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <WukMemory.hh>
#include <config/WukException.hh>
#include <crypto/WukUtils.hh>

namespace wuk {
    namespace crypto {
        constexpr wU32 WUK_SSC_KSLEN    = 64; // key stream length
        constexpr wU32 WUK_SSC_KEYLEN   = 32; // key length
        constexpr wU32 WUK_SSC_IVLEN    = 8;  // Initialization vector length

        // SN Stream Cipher
        class LIBWUK_API SSC {
        private:
            wByte keystream[WUK_SSC_KSLEN]{};

            wuk::crypto::Counter counter;

            wU32 *state = reinterpret_cast<wU32 *>(this->keystream);

        private:
            void keystream_sub_bytes();
            void keystream_bits_swap();
            void keystream_mixture();

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
