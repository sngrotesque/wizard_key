#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <config/WukEndianness.hh>
#include <crypto/WukCommon.hh>
#include <WukMemory.hh>

namespace wuk {
    namespace crypto {
        constexpr wU32 WukOP4_BL = 16; // Block length
        constexpr wU32 WukOP4_KL = 32; // Key length
        constexpr wU32 WukOP4_NL = 12; // Nonce length
        constexpr wU32 WukOP4_NK = 4;  // Key word length
        constexpr wU32 WukOP4_NR = 8;  // Number of rounds
        constexpr wU32 WukOP4_RKL = WukOP4_BL * WukOP4_NR; // Length of the round key

        class LIBWUK_API WukOP4 {
        private:
            alignas(16) wByte round_key[WukOP4_RKL]{0};
            wU32 counter = 0U;

        public:
            WukOP4() = default;
            WukOP4(const wByte key[WukOP4_KL], wU32 counter = 0U);

        public:
            void ecb_encrypt(wByte *out, const wByte *in, wSize length);
            void ecb_decrypt(wByte *out, const wByte *in, wSize length);

            void cbc_encrypt(wByte *out, const wByte *in, wSize length,
                       const wByte iv[WukOP4_BL]);
            void cbc_decrypt(wByte *out, const wByte *in, wSize length,
                       const wByte iv[WukOP4_BL]);

            void ofb_stream(wByte *out, const wByte *in, wSize length,
                      const wByte iv[WukOP4_NL]);

            void ctr_stream(wByte *out, const wByte *in, wSize length,
                      const wByte nonce[WukOP4_NL]);

        public:
            void set_counter(const wU32 &counter) noexcept
            {
                this->counter = counter;
            }

            const wByte *get_roundkey() const noexcept
            {
                return this->round_key;
            }
        };
    }
}

#endif
