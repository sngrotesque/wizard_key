#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <config/WukEndianness.hh>
#include <crypto/WukCommon.hh>
#include <WukMemory.hh>

#include <sodium.h>

namespace wuk {
    namespace crypto {
        constexpr wU32 WukCC20_KL  = 32; // Key length
        constexpr wU32 WukCC20_NL  = 12; // Nonce length
        constexpr wU32 WukCC20_KSL = 64; // Keystream length

        class LIBWUK_API WukChaCha20 {
        private:
            wByte key[WukCC20_KL]{0};
            wU32 counter = 0U;

        public:
            WukChaCha20() = default;
            WukChaCha20(const wByte key[WukCC20_KL], wU32 counter = 0U);
            ~WukChaCha20();

            void crypto_stream(wByte *out, const wByte *in, wSize length, wByte nonce[WukCC20_NL]);
        };

        class LIBWUK_API WukRFC8439 {
        private:
            alignas(16) wU32 state[16]{0};
            wByte key[WukCC20_KL]{0};
            wU32 counter = 0U;
        
        public:
            WukRFC8439() = default;
            WukRFC8439(const wByte key[WukCC20_KL], wU32 counter = 0U);
            ~WukRFC8439();

            void crypto_stream(wByte *out, const wByte *in, wSize length, wByte nonce[WukCC20_NL]);
        };
    }
}

#endif
