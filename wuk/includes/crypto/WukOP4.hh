#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <atomic>
#include <WukBuffer.hh>

namespace wuk::crypto {
    constexpr wuk::u32 OP4_BL  = 16; // Block length
    constexpr wuk::u32 OP4_KL  = 32; // Key length
    constexpr wuk::u32 OP4_NL  = 12; // Nonce length
    constexpr wuk::u32 OP4_NK  = 4;  // Key word length
    constexpr wuk::u32 OP4_NR  = 8;  // Number of rounds
    constexpr wuk::u32 OP4_RKL = OP4_BL * OP4_NR; // Length of the round key

    class LIBWUK_API OP4 {
    private:
        alignas(16) wuk::byte m_round_key[OP4_RKL]{0};
        std::atomic<wuk::u32> m_counter = 0U;

    public:
        OP4() = default;
        OP4(const wuk::byte key[OP4_KL], wuk::u32 counter = 0U);
        OP4(const wuk::Buffer &key, wuk::u32 counter = 0U);
        ~OP4();

        OP4(const OP4 &other) = default;
        OP4 &operator=(const OP4 &other) = default;

    public:
        void ecb_encrypt(wuk::byte *out, const wuk::byte *in,
                         wuk::ulong length) const;
        void ecb_decrypt(wuk::byte *out, const wuk::byte *in,
                         wuk::ulong length) const;

        void cbc_encrypt(wuk::byte *out, const wuk::byte *in,
                         wuk::ulong length, const wuk::byte iv[OP4_BL]) const;
        void cbc_decrypt(wuk::byte *out, const wuk::byte *in,
                         wuk::ulong length, const wuk::byte iv[OP4_BL]) const;

        void ofb_stream(wuk::byte *out, const wuk::byte *in,
                        wuk::ulong length, const wuk::byte iv[OP4_BL]) const;

        void ctr_stream(wuk::byte *out, const wuk::byte *in,
                        wuk::ulong length, const wuk::byte nonce[OP4_NL]);

        wuk::Buffer ctr_stream(const wuk::Buffer &buffer, const wuk::Buffer &nonce);

    public:
        void set_counter(wuk::u32 counter) noexcept
        {
            this->m_counter = counter;
        }

        void reset_counter() noexcept
        {
            this->m_counter = 0;
        }

        wuk::u32 get_counter() const noexcept
        {
            return this->m_counter;
        }

        const wuk::byte *get_round_key() const noexcept
        {
            return this->m_round_key;
        }
    };
}

#endif
