#include <crypto/WukOP4.hh>

#include <core/WukEndianness.hh>
#include <core/WukException.hh>
#include <crypto/WukCommon.hh>
#include <WukMemory.hh>

#define OP4_SI(T) static inline T

alignas(16) OP4_SI(constexpr wuk::u32) MUL_COEFFS[4] = {
    0x71e961ddU, 0x47dff15dU, 0x172f4f2fU, 0xf9c1e3c7U
};

alignas(16) OP4_SI(constexpr wuk::u32) INV_MUL_COEFFS[4] = {
    0xf6e1fe75U, 0x185beaf5U, 0xfb0a57cfU, 0x4d82edf7U
};

OP4_SI(void) shift_bits_add(wuk::byte state[wuk::crypto::OP4_BL]) noexcept
{
    wuk::u32 v0, v1, v2, v3;

    v0 = wuk::crypto::load32le(state);
    v1 = wuk::crypto::load32le(state + 4);
    v2 = wuk::crypto::load32le(state + 8);
    v3 = wuk::crypto::load32le(state + 12);

    v0 = wuk::crypto::rotl32(v0, 13) + v1 + v2 + v3;
    v1 = wuk::crypto::rotl32(v1, 7)  + v2 + v3 + v0;
    v2 = wuk::crypto::rotl32(v2, 11) + v3 + v0 + v1;
    v3 = wuk::crypto::rotl32(v3, 15) + v0 + v1 + v2;

    wuk::crypto::pack32le(state,      v0);
    wuk::crypto::pack32le(state + 4,  v1);
    wuk::crypto::pack32le(state + 8,  v2);
    wuk::crypto::pack32le(state + 12, v3);
}

OP4_SI(void) shift_bits_sub(wuk::byte state[wuk::crypto::OP4_BL]) noexcept
{
    wuk::u32 v0, v1, v2, v3;

    v0 = wuk::crypto::load32le(state);
    v1 = wuk::crypto::load32le(state + 4);
    v2 = wuk::crypto::load32le(state + 8);
    v3 = wuk::crypto::load32le(state + 12);

    v3 = wuk::crypto::rotr32(v3 - v0 - v1 - v2, 15);
    v2 = wuk::crypto::rotr32(v2 - v3 - v0 - v1, 11);
    v1 = wuk::crypto::rotr32(v1 - v2 - v3 - v0, 7);
    v0 = wuk::crypto::rotr32(v0 - v1 - v2 - v3, 13);

    wuk::crypto::pack32le(state,      v0);
    wuk::crypto::pack32le(state + 4,  v1);
    wuk::crypto::pack32le(state + 8,  v2);
    wuk::crypto::pack32le(state + 12, v3);
}

OP4_SI(void) multiply(wuk::byte state[wuk::crypto::OP4_BL]) noexcept
{
    wuk::crypto::pack32le(state,      wuk::crypto::load32le(state)      * MUL_COEFFS[0]);
    wuk::crypto::pack32le(state + 4,  wuk::crypto::load32le(state + 4)  * MUL_COEFFS[1]);
    wuk::crypto::pack32le(state + 8,  wuk::crypto::load32le(state + 8)  * MUL_COEFFS[2]);
    wuk::crypto::pack32le(state + 12, wuk::crypto::load32le(state + 12) * MUL_COEFFS[3]);
}

OP4_SI(void) inv_multiply(wuk::byte state[wuk::crypto::OP4_BL]) noexcept
{
    wuk::crypto::pack32le(state,      wuk::crypto::load32le(state)      * INV_MUL_COEFFS[0]);
    wuk::crypto::pack32le(state + 4,  wuk::crypto::load32le(state + 4)  * INV_MUL_COEFFS[1]);
    wuk::crypto::pack32le(state + 8,  wuk::crypto::load32le(state + 8)  * INV_MUL_COEFFS[2]);
    wuk::crypto::pack32le(state + 12, wuk::crypto::load32le(state + 12) * INV_MUL_COEFFS[3]);
}

OP4_SI(void) xor_with_iv(wuk::byte state[wuk::crypto::OP4_BL],
                   const wuk::byte iv[wuk::crypto::OP4_BL]) noexcept
{
    for (wuk::u32 i = 0; i < wuk::crypto::OP4_BL; ++i) {
        state[i] ^= iv[i];
    }
}

OP4_SI(void) xor_with_iv(wuk::byte state[wuk::crypto::OP4_BL],
                   const wuk::byte a[wuk::crypto::OP4_BL],
                   const wuk::byte b[wuk::crypto::OP4_BL]) noexcept
{
    for (wuk::u32 i = 0; i < wuk::crypto::OP4_BL; ++i) {
        state[i] = a[i] ^ b[i];
    }
}

OP4_SI(void) cipher(wuk::byte state[wuk::crypto::OP4_BL],
              const wuk::byte input[wuk::crypto::OP4_BL],
              const wuk::byte round_key[wuk::crypto::OP4_RKL]) noexcept
{
    memcpy(state, input, wuk::crypto::OP4_BL);
    for (wuk::u32 r = 0; r < wuk::crypto::OP4_NR; ++r) {
        shift_bits_add(state);
        multiply(state);
        xor_with_iv(state, round_key + wuk::crypto::OP4_BL * r);
    }
}

OP4_SI(void) inv_cipher(wuk::byte state[wuk::crypto::OP4_BL],
                  const wuk::byte input[wuk::crypto::OP4_BL],
                  const wuk::byte round_key[wuk::crypto::OP4_RKL]) noexcept
{
    memcpy(state, input, wuk::crypto::OP4_BL);
    for (wuk::u32 r = wuk::crypto::OP4_NR; r-- > 0; ) {
        xor_with_iv(state, round_key + wuk::crypto::OP4_BL * r);
        inv_multiply(state);
        shift_bits_sub(state);
    }
}

OP4_SI(void) prevent_zero_key(wuk::byte key[wuk::crypto::OP4_KL]) noexcept
{
    // Prevent weak keys
    for (wuk::u32 ki = 0; ki < wuk::crypto::OP4_KL; ++ki) {
        key[ki] ^= (((key[ki] + ki) - key[ki]) ^ (key[ki] << 1) ^ (key[ki] >> 4));
    }
}

OP4_SI(void) key_obfuscation(wuk::byte k[wuk::crypto::OP4_KL]) noexcept
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

OP4_SI(void) key_schedule_transformation(wuk::byte key[wuk::crypto::OP4_KL]) noexcept
{
    for (wuk::u32 r = 0; r < wuk::crypto::OP4_NR; ++r) {
        prevent_zero_key(key);
        key_obfuscation(key);
    }
}

OP4_SI(void) key_extension(
    const wuk::byte key[wuk::crypto::OP4_KL],
          wuk::byte round_key[wuk::crypto::OP4_RKL]) noexcept
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

    OP4::~OP4()
    {
        wuk::memory_secure(this->round_key, sizeof(this->round_key));
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
