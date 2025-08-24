#include <crypto/WukOP4.hh>

#include <core/WukEndianness.hh>
#include <core/WukException.hh>
#include <crypto/WukCommon.hh>
#include <WukMemory.hh>
#include <WukSIMD.hh>

#define OP4_SI(T) static inline T

alignas(16) OP4_SI(constexpr wuk::u32) MUL_COEFFS[4] = {
    0x71e961d3U, 0x47dff135U, 0x172f4f25U, 0x49c1e3bfU
};

alignas(16) OP4_SI(constexpr wuk::u32) INV_MUL_COEFFS[4] = {
    0x756e9e5bU, 0xd2b5991dU, 0x8ce434adU, 0x34640c3fU
};

OP4_SI(void) xor_with_iv(wuk::byte state[wuk::crypto::OP4_BL],
                   const wuk::byte iv[wuk::crypto::OP4_BL])
{
    wuk::simd::sse::storeu128(state,
        wuk::simd::sse::xor128(
            wuk::simd::sse::loadu128(state),
            wuk::simd::sse::loadu128(iv)
        )
    );
}

OP4_SI(void) xor_with_iv(wuk::byte state[wuk::crypto::OP4_BL],
                   const wuk::byte a[wuk::crypto::OP4_BL],
                   const wuk::byte b[wuk::crypto::OP4_BL])
{
    wuk::simd::sse::storeu128(state,
        wuk::simd::sse::xor128(
            wuk::simd::sse::loadu128(a),
            wuk::simd::sse::loadu128(b)
        )
    );
}

OP4_SI(void) cipher(wuk::byte state[wuk::crypto::OP4_BL],
              const wuk::byte input[wuk::crypto::OP4_BL],
              const wuk::byte round_key[wuk::crypto::OP4_RKL])
{
    wuk::simd::sse::s128 temp;
    wuk::simd::sse::s128 rk[8] = {
        wuk::simd::sse::loadu128(round_key + 0 * wuk::crypto::OP4_BL),
        wuk::simd::sse::loadu128(round_key + 1 * wuk::crypto::OP4_BL),
        wuk::simd::sse::loadu128(round_key + 2 * wuk::crypto::OP4_BL),
        wuk::simd::sse::loadu128(round_key + 3 * wuk::crypto::OP4_BL),
        wuk::simd::sse::loadu128(round_key + 4 * wuk::crypto::OP4_BL),
        wuk::simd::sse::loadu128(round_key + 5 * wuk::crypto::OP4_BL),
        wuk::simd::sse::loadu128(round_key + 6 * wuk::crypto::OP4_BL),
        wuk::simd::sse::loadu128(round_key + 7 * wuk::crypto::OP4_BL),
    };
    wuk::simd::sse::s128 mul = wuk::simd::sse::loadu128(MUL_COEFFS);

    temp = wuk::simd::sse::loadu128(input);
    for (wuk::u32 r = 0; r < wuk::crypto::OP4_NR; ++r) {
        // shift bit add
        wuk::u32 v0 = wuk::simd::sse::extract<wuk::simd::simd_size::epi32, 0>(temp);
        wuk::u32 v1 = wuk::simd::sse::extract<wuk::simd::simd_size::epi32, 1>(temp);
        wuk::u32 v2 = wuk::simd::sse::extract<wuk::simd::simd_size::epi32, 2>(temp);
        wuk::u32 v3 = wuk::simd::sse::extract<wuk::simd::simd_size::epi32, 3>(temp);
        v0 = wuk::crypto::rotl32(v0, 13) + v1 + v2 + v3;
        v1 = wuk::crypto::rotl32(v1, 7)  + v2 + v3 + v0;
        v2 = wuk::crypto::rotl32(v2, 11) + v3 + v0 + v1;
        v3 = wuk::crypto::rotl32(v3, 15) + v0 + v1 + v2;
        temp = _mm_set_epi32(v3, v2, v1, v0);
        // multiply
        temp = wuk::simd::sse::mullo<wuk::simd::simd_size::epi32>(temp, mul);
        // round key add
        temp = wuk::simd::sse::xor128(temp, rk[r]);
    }
    wuk::simd::sse::storeu128(state, temp);
}

OP4_SI(void) inv_cipher(wuk::byte state[wuk::crypto::OP4_BL],
                  const wuk::byte input[wuk::crypto::OP4_BL],
                  const wuk::byte round_key[wuk::crypto::OP4_RKL])
{
    wuk::simd::sse::s128 temp;
    wuk::simd::sse::s128 rk[8] = {
        wuk::simd::sse::loadu128((round_key + 7 * wuk::crypto::OP4_BL)),
        wuk::simd::sse::loadu128((round_key + 6 * wuk::crypto::OP4_BL)),
        wuk::simd::sse::loadu128((round_key + 5 * wuk::crypto::OP4_BL)),
        wuk::simd::sse::loadu128((round_key + 4 * wuk::crypto::OP4_BL)),
        wuk::simd::sse::loadu128((round_key + 3 * wuk::crypto::OP4_BL)),
        wuk::simd::sse::loadu128((round_key + 2 * wuk::crypto::OP4_BL)),
        wuk::simd::sse::loadu128((round_key + 1 * wuk::crypto::OP4_BL)),
        wuk::simd::sse::loadu128((round_key + 0 * wuk::crypto::OP4_BL))
    };
    wuk::simd::sse::s128 inv_mul = wuk::simd::sse::loadu128(INV_MUL_COEFFS);

    temp = wuk::simd::sse::loadu128(input);
    for (wuk::u32 r = 0; r < wuk::crypto::OP4_NR; ++r) {
        // round key add
        temp = wuk::simd::sse::xor128(temp, rk[r]);
        // inv multiply
        temp = wuk::simd::sse::mullo<wuk::simd::simd_size::epi32>(temp, inv_mul);
        // shift bits sub
        wuk::u32 v0 = wuk::simd::sse::extract<wuk::simd::simd_size::epi32, 0>(temp);
        wuk::u32 v1 = wuk::simd::sse::extract<wuk::simd::simd_size::epi32, 1>(temp);
        wuk::u32 v2 = wuk::simd::sse::extract<wuk::simd::simd_size::epi32, 2>(temp);
        wuk::u32 v3 = wuk::simd::sse::extract<wuk::simd::simd_size::epi32, 3>(temp);
        v3 = wuk::crypto::rotr32(v3 - v0 - v1 - v2, 15);
        v2 = wuk::crypto::rotr32(v2 - v3 - v0 - v1, 11);
        v1 = wuk::crypto::rotr32(v1 - v2 - v3 - v0, 7);
        v0 = wuk::crypto::rotr32(v0 - v1 - v2 - v3, 13);
        temp = _mm_set_epi32(v3, v2, v1, v0);
    }
    wuk::simd::sse::storeu128(state, temp);
}

OP4_SI(void) prevent_zero_key(wuk::byte key[wuk::crypto::OP4_KL])
{
    // Prevent weak keys
    for (wuk::u32 ki = 0; ki < wuk::crypto::OP4_KL; ++ki) {
        key[ki] ^= (((key[ki] + ki) - key[ki]) ^ (key[ki] << 1) ^ (key[ki] >> 4));
    }
}

OP4_SI(void) key_obfuscation(wuk::byte k[wuk::crypto::OP4_KL])
{
    // Process the 0, 4, 8, and 12 bytes each time.
    for (wuk::u32 i = 0; i < wuk::crypto::OP4_KL; i += 4) {
        k[i] += wuk::crypto::rotl8(k[i] ^ k[i+1] ^ k[i+2] ^ k[i+3], 5);
    }
    wuk::u32 v0, v1, v2, v3, v4, v5, v6, v7;
    wuk::u32 t0, t1, t2, t3, t4, t5, t6, t7;

    // Introduce a diffusion mechanism for key
    v0 = wuk::crypto::load32le(k     ); t0 = v0;
    v1 = wuk::crypto::load32le(k +  4); t1 = v1;
    v2 = wuk::crypto::load32le(k +  8); t2 = v2;
    v3 = wuk::crypto::load32le(k + 12); t3 = v3;
    v4 = wuk::crypto::load32le(k + 16); t4 = v4;
    v5 = wuk::crypto::load32le(k + 20); t5 = v5;
    v6 = wuk::crypto::load32le(k + 24); t6 = v6;
    v7 = wuk::crypto::load32le(k + 28); t7 = v7;

    t7 += wuk::crypto::rotl32((v0 ^ v7) + v6, 15);
    t6 += wuk::crypto::rotl32((v7 ^ v6) + v5, 19);
    t5 += wuk::crypto::rotl32((v6 ^ v5) + v4, 21);
    t4 += wuk::crypto::rotl32((v5 ^ v4) + v3, 29);
    t3 += wuk::crypto::rotl32((v4 ^ v3) + v2, 13);
    t2 += wuk::crypto::rotl32((v3 ^ v2) + v1, 7);
    t1 += wuk::crypto::rotl32((v2 ^ v1) + v0, 23);
    t0 += wuk::crypto::rotl32((v1 ^ v0) + v7, 17);

    wuk::crypto::pack32le(k,      t0);
    wuk::crypto::pack32le(k + 4,  t1);
    wuk::crypto::pack32le(k + 8,  t2);
    wuk::crypto::pack32le(k + 12, t3);
    wuk::crypto::pack32le(k + 16, t4);
    wuk::crypto::pack32le(k + 20, t5);
    wuk::crypto::pack32le(k + 24, t6);
    wuk::crypto::pack32le(k + 28, t7);
}

OP4_SI(void) key_schedule_transformation(wuk::byte key[wuk::crypto::OP4_KL])
{
    for (wuk::u32 r = 0; r < wuk::crypto::OP4_NR; ++r) {
        prevent_zero_key(key);
        key_obfuscation(key);
    }
}

OP4_SI(void) key_extension(const wuk::byte key[wuk::crypto::OP4_KL],
                                 wuk::byte round_key[wuk::crypto::OP4_RKL])
{
    wuk::byte copy_key[wuk::crypto::OP4_KL]{0};
    memcpy(copy_key, key, wuk::crypto::OP4_KL);

    for (wuk::u32 i = 0; i < wuk::crypto::OP4_NK; ++i) {
        key_schedule_transformation(copy_key);
        memcpy(round_key + i * wuk::crypto::OP4_KL, copy_key,
            wuk::crypto::OP4_KL);
    }

    wuk::memory_secure(copy_key, wuk::crypto::OP4_KL);
}

namespace wuk::crypto {
    OP4::OP4(const wuk::byte key[OP4_KL], wuk::u32 counter)
    : counter(counter)
    {
        if (!key) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::OP4::OP4",
                "key is nullptr.");
        }
        key_extension(key, this->round_key);
    }

    void OP4::ecb_encrypt(wuk::byte *out, const wuk::byte *in, wuk::ulong length)
    {
        if(!out || !in) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::OP4::ecb_encrypt",
                "out/in is nullptr.");
        }
        if(length % OP4_BL) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::crypto::OP4::ecb_encrypt",
                "length must be a multiple of block length.");
        }

        for (wuk::ulong i = 0; i < length; i += OP4_BL) {
            cipher(out + i, in + i, this->round_key);
        }
    }

    void OP4::ecb_decrypt(wuk::byte *out, const wuk::byte *in, wuk::ulong length)
    {
        if(!out || !in) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::OP4::ecb_decrypt",
                "out/in is nullptr.");
        }
        if(length % OP4_BL) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::crypto::OP4::ecb_decrypt",
                "length must be a multiple of block length.");
        }

        for (wuk::ulong i = 0; i < length; i += OP4_BL) {
            inv_cipher(out + i, in + i, this->round_key);
        }
    }

    void OP4::cbc_encrypt(wuk::byte *out, const wuk::byte *in, wuk::ulong length,
                    const wuk::byte iv[OP4_BL])
    {
        if(!out || !in || !iv) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::OP4::cbc_encrypt",
                "out/in/iv is nullptr.");
        }
        if(length % OP4_BL) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::crypto::OP4::cbc_encrypt",
                "length must be a multiple of block length.");
        }
        wuk::byte buffer[OP4_BL]{0};
        memcpy(buffer, iv, OP4_BL);
        
        for (wuk::ulong i = 0; i < length; i += OP4_BL) {
            xor_with_iv(buffer, in + i);
            cipher(out + i, buffer, this->round_key);
            memcpy(buffer, out + i, OP4_BL);
        }
    }

    void OP4::cbc_decrypt(wuk::byte *out, const wuk::byte *in, wuk::ulong length,
                    const wuk::byte iv[OP4_BL])
    {
        if(!out || !in || !iv) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::OP4::cbc_decrypt",
                "out/in/iv is nullptr.");
        }
        if(length % OP4_BL) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::crypto::OP4::cbc_decrypt",
                "length must be a multiple of block length.");
        }
        wuk::byte buffer[OP4_BL]{0}, prev[OP4_BL]{0};
        memcpy(prev, iv, OP4_BL);

        for (wuk::ulong i = 0; i < length; i += OP4_BL) {
            inv_cipher(buffer, in + i, this->round_key);
            xor_with_iv(out + i, buffer, prev);
            memcpy(prev, in + i, OP4_BL);
        }
    }

    void OP4::ofb_stream(wuk::byte *out, const wuk::byte *in, wuk::ulong length,
                   const wuk::byte iv[OP4_NL])
    {
        if(!out || !in || !iv) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::OP4::ofb_stream",
                "out/in/iv is nullptr.");
        }
        wuk::byte feedback[OP4_BL]{0};
        memcpy(feedback, iv, OP4_BL);

        size_t remaining = length;
        while (remaining >= OP4_BL) {
            cipher(feedback, feedback, this->round_key);
            xor_with_iv(out, in, feedback);
            out += OP4_BL;
            in += OP4_BL;
            remaining -= OP4_BL;
        }
        if (remaining > 0) {
            cipher(feedback, feedback, this->round_key);
            for (size_t i = 0; i < remaining; i++) {
                out[i] = in[i] ^ feedback[i];
            }
        }
    }

    void OP4::ctr_stream(wuk::byte *out, const wuk::byte *in, wuk::ulong length,
                   const wuk::byte nonce[OP4_NL])
    {
        if(!out || !in || !nonce) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::OP4::ctr_stream",
                "out/in/nonce is nullptr.");
        }
        wuk::byte keystream[OP4_BL]{0};
        wuk::byte state[OP4_BL]{0};
        memcpy(keystream, nonce, OP4_NL);

        size_t remaining = length;
        while (remaining >= OP4_BL) {
            pack32le(keystream + OP4_NL, this->counter++);
            cipher(state, keystream, this->round_key);
            xor_with_iv(out, in, state);
            out += OP4_BL;
            in += OP4_BL;
            remaining -= OP4_BL;
        }
        if (remaining > 0) {
            pack32le(keystream + OP4_NL, this->counter++);
            cipher(state, keystream, this->round_key);
            for (size_t i = 0; i < remaining; i++) {
                out[i] = in[i] ^ state[i];
            }
        }
    }
}
