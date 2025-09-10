#include <crypto/WukChaCha20.hh>

#include <core/WukException.hh>
#include <core/WukEndianness.hh>
#include <crypto/WukCommon.hh>
#include <WukMemory.hh>

#ifdef LIBSODIUM_SUPPORT
#   include <sodium.h>
#endif

#define U32C(x) x##U
#define QUARTERROUND(a, b, c, d) \
    a += b; d ^= a; d = wuk::crypto::rotl32(d, 16); \
    c += d; b ^= c; b = wuk::crypto::rotl32(b, 12); \
    a += b; d ^= a; d = wuk::crypto::rotl32(d, 8);  \
    c += d; b ^= c; b = wuk::crypto::rotl32(b, 7);


static inline void
state_recombination(wuk::u32 keystream[16], const wuk::u32 state[16]) noexcept
{
    for (wuk::u32 i = 0; i < 16; ++i) {
        keystream[i] += state[i];
    }
}

static inline void
state_set_key(wuk::u32 state[16], const wuk::byte key[wuk::crypto::WukCC20_KL]) noexcept
{
    state[0]  = U32C(0x61707865);
    state[1]  = U32C(0x3320646e);
    state[2]  = U32C(0x79622d32);
    state[3]  = U32C(0x6b206574);

    state[4]  = wuk::crypto::load32le(key + 0);
    state[5]  = wuk::crypto::load32le(key + 4);
    state[6]  = wuk::crypto::load32le(key + 8);
    state[7]  = wuk::crypto::load32le(key + 12);
    state[8]  = wuk::crypto::load32le(key + 16);
    state[9]  = wuk::crypto::load32le(key + 20);
    state[10] = wuk::crypto::load32le(key + 24);
    state[11] = wuk::crypto::load32le(key + 28);
}

static inline void
state_set_iv(wuk::u32 state[16],
       const wuk::byte nonce[wuk::crypto::WukCC20_NL],
       const wuk::byte counter[4]) noexcept
{
    state[12] = wuk::crypto::load32le(counter);
    state[13] = wuk::crypto::load32le(nonce);
    state[14] = wuk::crypto::load32le(nonce + 4);
    state[15] = wuk::crypto::load32le(nonce + 8);
}

static inline void
state_init(wuk::u32 state[16],
    const wuk::byte key[wuk::crypto::WukCC20_KL],
    const wuk::byte nonce[wuk::crypto::WukCC20_NL],
    const wuk::u32 &counter) noexcept
{
    wuk::byte ic[4] {0};

    wuk::crypto::pack32le(ic, counter);

    state_set_key(state, key);
    state_set_iv(state, nonce, ic);
}

namespace wuk::crypto {
    ChaCha20::ChaCha20(const wuk::byte key[WukCC20_KL], wuk::u32 counter)
    : counter(counter)
    {
        if (!key) {
            wuk::Exception(wuk::Error::NPTR, "ChaCha20::ChaCha20",
                "key is nullptr.");
        }
        memcpy(this->key, key, WukCC20_KL);
    #   ifdef LIBSODIUM_SUPPORT
        this->use_libsodium = (sodium_init() >= 0);
    #   endif
    }

    ChaCha20::~ChaCha20()
    {
        wuk::memory_secure(this->state, WukCC20_KSL);
        wuk::memory_secure(this->key, WukCC20_KL);
    }

    void ChaCha20::rfc8439_crypto_stream(wuk::byte *out, const wuk::byte *in, wuk::ulong length,
                                   const wuk::byte nonce[WukCC20_NL]) noexcept
    {
        state_init(this->state, this->key, nonce, this->counter);

        wuk::u32 keystream[16] {0};
        wuk::byte *ksp = reinterpret_cast<wuk::byte *>(keystream);

        for (wuk::ulong i = 0, ki = WukCC20_KSL; i < length; ++i, ++ki) {
            if (ki == WukCC20_KSL) {
                memcpy(keystream, this->state, WukCC20_KSL);

                for (wuk::u32 r = 0; r < 10; ++r) {
                    QUARTERROUND(keystream[0],  keystream[4],
                                 keystream[8],  keystream[12]);
                    QUARTERROUND(keystream[1],  keystream[5],
                                 keystream[9],  keystream[13]);
                    QUARTERROUND(keystream[2],  keystream[6],
                                 keystream[10], keystream[14]);
                    QUARTERROUND(keystream[3],  keystream[7],
                                 keystream[11], keystream[15]);

                    QUARTERROUND(keystream[0],  keystream[5],
                                 keystream[10], keystream[15]);
                    QUARTERROUND(keystream[1],  keystream[6],
                                 keystream[11], keystream[12]);
                    QUARTERROUND(keystream[2],  keystream[7],
                                 keystream[8],  keystream[13]);
                    QUARTERROUND(keystream[3],  keystream[4],
                                 keystream[9],  keystream[14]);
                }
                state_recombination(keystream, this->state);

                this->state[12]++;

                ki = 0;
            }

            out[i] = in[i] ^ ksp[ki];
        }

        this->counter += ((length + WukCC20_KSL - 1) / WukCC20_KSL);
    }

    void ChaCha20::crypto_stream(wuk::byte *out, const wuk::byte *in, wuk::ulong length,
                           const wuk::byte nonce[WukCC20_NL]) noexcept
    {
        if (this->use_libsodium) {
    #       ifdef LIBSODIUM_SUPPORT
            // 此处的宏仅仅是为了避免在未使用Libsodium库时出现此函数未定义的情况
            crypto_stream_chacha20_ietf_xor_ic(out, in, length,
                nonce, this->counter, this->key);
    #       endif
        } else {
            this->rfc8439_crypto_stream(out, in, length, nonce);
        }
        this->counter += ((length + WukCC20_KSL - 1) / WukCC20_KSL);
    }
}
