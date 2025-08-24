#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <atomic>

namespace wuk::crypto {
    constexpr wuk::u32 WukCC20_KL  = 32; // Key length
    constexpr wuk::u32 WukCC20_NL  = 12; // Nonce length
    constexpr wuk::u32 WukCC20_KSL = 64; // Keystream length

    class LIBWUK_API ChaCha20 {
    private:
        bool use_libsodium = false;
    private:
        alignas(16) wuk::byte key[WukCC20_KL]{0};
        std::atomic<wuk::u32> counter = 0U;

    private:
        alignas(16) wuk::u32 state[16]{0};

        void rfc8439_crypto_stream(wuk::byte *out, const wuk::byte *in, wuk::ulong length,
                             const wuk::byte nonce[WukCC20_NL]);

    public:
        ChaCha20() = default;
        ChaCha20(const wuk::byte key[WukCC20_KL], wuk::u32 counter = 0U);
        ~ChaCha20();

        void crypto_stream(wuk::byte *out, const wuk::byte *in, wuk::ulong length,
                     const wuk::byte nonce[WukCC20_NL]);
    };
}

#endif
