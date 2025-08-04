#include <crypto/WukOP4.hh>

#include <config/WukEndianness.hh>
#include <crypto/WukCommon.hh>
#include <WukMemory.hh>

#define OP4_SI(T) static inline T

alignas(16) OP4_SI(constexpr wU32) MUL_COEFFS[4] = {
    0x71e961d3U, 0x47dff135U, 0x172f4f25U, 0x49c1e3bfU
};

alignas(16) OP4_SI(constexpr wU32) INV_MUL_COEFFS[4] = {
    0x756e9e5bU, 0xd2b5991dU, 0x8ce434adU, 0x34640c3fU
};

OP4_SI(void) shift_bits_add(wByte state[wuk::crypto::WukOP4_BL])
{
    wU32 v0, v1, v2, v3;

    v0 = wuk::crypto::load32le(state);
    v1 = wuk::crypto::load32le(state + 4);
    v2 = wuk::crypto::load32le(state + 8);
    v3 = wuk::crypto::load32le(state + 12);

    v0 = wuk::crypto::rotl32(v0, 13) + v1 + v2 + v3;
    v1 = wuk::crypto::rotl32(v1, 19) + v2 + v3 + v0;
    v2 = wuk::crypto::rotl32(v2, 11) + v3 + v0 + v1;
    v3 = wuk::crypto::rotl32(v3, 17) + v0 + v1 + v2;

    wuk::crypto::pack32le(state,      v0);
    wuk::crypto::pack32le(state + 4,  v1);
    wuk::crypto::pack32le(state + 8,  v2);
    wuk::crypto::pack32le(state + 12, v3);
}

OP4_SI(void) shift_bits_sub(wByte state[wuk::crypto::WukOP4_BL])
{
    wU32 v0, v1, v2, v3;

    v0 = wuk::crypto::load32le(state);
    v1 = wuk::crypto::load32le(state + 4);
    v2 = wuk::crypto::load32le(state + 8);
    v3 = wuk::crypto::load32le(state + 12);

    v3 = wuk::crypto::rotr32(v3 - v0 - v1 - v2, 17);
    v2 = wuk::crypto::rotr32(v2 - v3 - v0 - v1, 11);
    v1 = wuk::crypto::rotr32(v1 - v2 - v3 - v0, 19);
    v0 = wuk::crypto::rotr32(v0 - v1 - v2 - v3, 13);

    wuk::crypto::pack32le(state,      v0);
    wuk::crypto::pack32le(state + 4,  v1);
    wuk::crypto::pack32le(state + 8,  v2);
    wuk::crypto::pack32le(state + 12, v3);
}

OP4_SI(void) multiply(wByte state[wuk::crypto::WukOP4_BL])
{
    wuk::crypto::pack32le(state,      wuk::crypto::load32le(state)      * MUL_COEFFS[0]);
    wuk::crypto::pack32le(state + 4,  wuk::crypto::load32le(state + 4)  * MUL_COEFFS[1]);
    wuk::crypto::pack32le(state + 8,  wuk::crypto::load32le(state + 8)  * MUL_COEFFS[2]);
    wuk::crypto::pack32le(state + 12, wuk::crypto::load32le(state + 12) * MUL_COEFFS[3]);
}

OP4_SI(void) inv_multiply(wByte state[wuk::crypto::WukOP4_BL])
{
    wuk::crypto::pack32le(state,      wuk::crypto::load32le(state)      * INV_MUL_COEFFS[0]);
    wuk::crypto::pack32le(state + 4,  wuk::crypto::load32le(state + 4)  * INV_MUL_COEFFS[1]);
    wuk::crypto::pack32le(state + 8,  wuk::crypto::load32le(state + 8)  * INV_MUL_COEFFS[2]);
    wuk::crypto::pack32le(state + 12, wuk::crypto::load32le(state + 12) * INV_MUL_COEFFS[3]);
}

OP4_SI(void) xor_with_iv(wByte state[wuk::crypto::WukOP4_BL],
                   const wByte iv[wuk::crypto::WukOP4_BL])
{
    for (wU32 i = 0; i < wuk::crypto::WukOP4_BL; ++i) {
        state[i] ^= iv[i];
    }
}

OP4_SI(void) xor_with_iv(wByte state[wuk::crypto::WukOP4_BL],
                   const wByte a[wuk::crypto::WukOP4_BL],
                   const wByte b[wuk::crypto::WukOP4_BL])
{
    for (wU32 i = 0; i < wuk::crypto::WukOP4_BL; ++i) {
        state[i] = a[i] ^ b[i];
    }
}

OP4_SI(void) cipher(wByte state[wuk::crypto::WukOP4_BL],
              const wByte input[wuk::crypto::WukOP4_BL],
              const wByte round_key[wuk::crypto::WukOP4_RKL])
{
    memcpy(state, input, wuk::crypto::WukOP4_BL);
    for (wU32 r = 0; r < wuk::crypto::WukOP4_NR; ++r) {
        shift_bits_add(state);
        multiply(state);
        xor_with_iv(state, round_key + wuk::crypto::WukOP4_BL * r);
    }
}

OP4_SI(void) inv_cipher(wByte state[wuk::crypto::WukOP4_BL],
                  const wByte input[wuk::crypto::WukOP4_BL],
                  const wByte round_key[wuk::crypto::WukOP4_RKL])
{
    memcpy(state, input, wuk::crypto::WukOP4_BL);
    for (wU32 r = wuk::crypto::WukOP4_NR; r-- > 0; ) {
        xor_with_iv(state, round_key + wuk::crypto::WukOP4_BL * r);
        inv_multiply(state);
        shift_bits_sub(state);
    }
}

OP4_SI(void) prevent_zero_key(wByte key[wuk::crypto::WukOP4_KL])
{
    // Prevent weak keys
    for (wU32 ki = 0; ki < wuk::crypto::WukOP4_KL; ++ki) {
        key[ki] ^= (((key[ki] + ki) - key[ki]) ^ (key[ki] << 1) ^ (key[ki] >> 4));
    }
}

OP4_SI(void) key_obfuscation(wByte k[wuk::crypto::WukOP4_KL])
{
    // Process the 0, 4, 8, and 12 bytes each time.
    for (wU32 i = 0; i < wuk::crypto::WukOP4_KL; i += 4) {
        k[i] += wuk::crypto::rotl8(k[i] ^ k[i+1] ^ k[i+2] ^ k[i+3], 5);
    }
    wU32 v0, v1, v2, v3, v4, v5, v6, v7;
    wU32 t0, t1, t2, t3, t4, t5, t6, t7;

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

OP4_SI(void) key_schedule_transformation(wByte key[wuk::crypto::WukOP4_KL])
{
    for (wU32 r = 0; r < wuk::crypto::WukOP4_NR; ++r) {
        prevent_zero_key(key);
        key_obfuscation(key);
    }
}

OP4_SI(void) key_extension(const wByte key[wuk::crypto::WukOP4_KL],
                                 wByte round_key[wuk::crypto::WukOP4_RKL])
{
    wByte copy_key[wuk::crypto::WukOP4_KL]{0};
    memcpy(copy_key, key, wuk::crypto::WukOP4_KL);

    for (wU32 i = 0; i < wuk::crypto::WukOP4_NK; ++i) {
        key_schedule_transformation(copy_key);
        memcpy(round_key + i * wuk::crypto::WukOP4_KL, copy_key,
            wuk::crypto::WukOP4_KL);
    }

    wuk::memory_secure(copy_key, wuk::crypto::WukOP4_KL);
}

wuk::crypto::WukOP4::WukOP4(const wByte key[WukOP4_KL], wU32 counter)
: counter(counter)
{
    if (!key) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::WukOP4::WukOP4",
            "key is nullptr.");
    }
    key_extension(key, this->round_key);
}

void wuk::crypto::WukOP4::ecb_encrypt(wByte *out, const wByte *in, wSize length)
{
    if(!out || !in) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::WukFEA::ecb_encrypt",
            "ciphertext or plaintext is nullptr.");
    }
    if(length % WukOP4_BL) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::crypto::WukFEA::ecb_encrypt",
            "length must be a multiple of block length.");
    }

    for (wSize i = 0; i < length; i += WukOP4_BL) {
        cipher(out + i, in + i, this->round_key);
    }
}

void wuk::crypto::WukOP4::ecb_decrypt(wByte *out, const wByte *in, wSize length)
{
    if(!out || !in) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::WukFEA::ecb_encrypt",
            "ciphertext or plaintext is nullptr.");
    }
    if(length % WukOP4_BL) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::crypto::WukFEA::ecb_encrypt",
            "length must be a multiple of block length.");
    }

    for (wSize i = 0; i < length; i += WukOP4_BL) {
        inv_cipher(out + i, in + i, this->round_key);
    }
}

void wuk::crypto::WukOP4::cbc_encrypt(wByte *out, const wByte *in,
                                      wSize length, const wByte iv[WukOP4_BL])
{
    if(!out || !in || !iv) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::WukFEA::cbc_encrypt",
            "out/in/iv is nullptr.");
    }
    if(length % WukOP4_BL) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::crypto::WukFEA::cbc_encrypt",
            "length must be a multiple of block length.");
    }
    wByte buffer[WukOP4_BL]{0};
    memcpy(buffer, iv, WukOP4_BL);
    
    for (wSize i = 0; i < length; i += WukOP4_BL) {
        xor_with_iv(buffer, in + i);
        cipher(out + i, buffer, this->round_key);
        memcpy(buffer, out + i, WukOP4_BL);
    }
}

void wuk::crypto::WukOP4::cbc_decrypt(wByte *out, const wByte *in,
                                      wSize length, const wByte iv[WukOP4_BL])
{
    if(!out || !in || !iv) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::WukFEA::cbc_decrypt",
            "out/in/iv is nullptr.");
    }
    if(length % WukOP4_BL) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::crypto::WukFEA::cbc_decrypt",
            "length must be a multiple of block length.");
    }
    wByte buffer[WukOP4_BL]{0}, prev[WukOP4_BL]{0};
    memcpy(prev, iv, WukOP4_BL);

    for (wSize i = 0; i < length; i += WukOP4_BL) {
        inv_cipher(buffer, in + i, this->round_key);
        xor_with_iv(out + i, buffer, prev);
        memcpy(prev, in + i, WukOP4_BL);
    }
}

void wuk::crypto::WukOP4::ofb_stream(wByte *out, const wByte *in, wSize length,
                               const wByte iv[WukOP4_NL])
{
    if(!out || !in || !iv) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::WukFEA::ofb_xcrypt",
            "out/in/iv is nullptr.");
    }
    wByte feedback[WukOP4_BL]{0};
    memcpy(feedback, iv, WukOP4_BL);

    size_t remaining = length;
    while (remaining >= WukOP4_BL) {
        cipher(feedback, feedback, this->round_key);
        xor_with_iv(out, in, feedback);
        out += WukOP4_BL;
        in += WukOP4_BL;
        remaining -= WukOP4_BL;
    }
    if (remaining > 0) {
        cipher(feedback, feedback, this->round_key);
        for (size_t i = 0; i < remaining; i++) {
            out[i] = in[i] ^ feedback[i];
        }
    }
}

void wuk::crypto::WukOP4::ctr_stream(wByte *out, const wByte *in, wSize length,
                               const wByte nonce[WukOP4_NL])
{
    if(!out || !in || !nonce) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::WukFEA::ofb_xcrypt",
            "out/in/nonce is nullptr.");
    }
    wByte keystream[WukOP4_BL]{0};
    wByte state[WukOP4_BL]{0};
    memcpy(keystream, nonce, WukOP4_NL);

    size_t remaining = length;
    while (remaining >= WukOP4_BL) {  // 完整块处理
        wuk::crypto::pack32le(keystream + WukOP4_NL, this->counter++);
        cipher(state, keystream, this->round_key);
        xor_with_iv(out, in, state);
        out += WukOP4_BL;
        in += WukOP4_BL;
        remaining -= WukOP4_BL;
    }
    if (remaining > 0) {  // 处理末尾部分块
        wuk::crypto::pack32le(keystream + WukOP4_NL, this->counter++);
        cipher(state, keystream, this->round_key);
        for (size_t i = 0; i < remaining; i++) {  // 逐字节处理
            out[i] = in[i] ^ state[i];
        }
    }
}
