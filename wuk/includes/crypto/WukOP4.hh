#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <atomic>

namespace wuk::crypto {
    constexpr wU32 OP4_BL  = 16; // Block length
    constexpr wU32 OP4_KL  = 32; // Key length
    constexpr wU32 OP4_NL  = 12; // Nonce length
    constexpr wU32 OP4_NK  = 4;  // Key word length
    constexpr wU32 OP4_NR  = 8;  // Number of rounds
    constexpr wU32 OP4_RKL = OP4_BL * OP4_NR; // Length of the round key

    class LIBWUK_API OP4 {
    private:
        alignas(16) wByte round_key[OP4_RKL]{0};
        std::atomic<wU32> counter = 0U;

    public:
        OP4() = default;
        OP4(const wByte key[OP4_KL], wU32 counter = 0U);

    public:
        void ecb_encrypt(wByte *out, const wByte *in, wSize length);
        void ecb_decrypt(wByte *out, const wByte *in, wSize length);

        void cbc_encrypt(wByte *out, const wByte *in, wSize length,
                   const wByte iv[OP4_BL]);
        void cbc_decrypt(wByte *out, const wByte *in, wSize length,
                   const wByte iv[OP4_BL]);

        void ofb_stream(wByte *out, const wByte *in, wSize length,
                  const wByte iv[OP4_NL]);

        void ctr_stream(wByte *out, const wByte *in, wSize length,
                  const wByte nonce[OP4_NL]);

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

#endif
