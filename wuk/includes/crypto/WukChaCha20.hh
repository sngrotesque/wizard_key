#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <atomic>

namespace wuk::crypto {
    constexpr wU32 WukCC20_KL  = 32; // Key length
    constexpr wU32 WukCC20_NL  = 12; // Nonce length
    constexpr wU32 WukCC20_KSL = 64; // Keystream length

    class LIBWUK_API ChaCha20 {
    private:
        bool use_libsodium = false;
    private:
        alignas(16) wuk::byte key[WukCC20_KL]{0};
        std::atomic<wuk::u32> counter = 0U;

    private:
        alignas(16) wuk::u32 state[16]{0};

        void rfc8439_crypto_stream(wByte *out, const wByte *in, wSize length,
                             const wByte nonce[WukCC20_NL]);

    public:
        ChaCha20() = default;
        ChaCha20(const wByte key[WukCC20_KL], wU32 counter = 0U);
        ~ChaCha20();

        void crypto_stream(wByte *out, const wByte *in, wSize length,
                     const wByte nonce[WukCC20_NL]);
    };
}

#endif
