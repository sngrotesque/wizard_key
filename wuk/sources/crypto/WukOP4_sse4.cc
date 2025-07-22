#include <crypto/WukOP4.hh>
#include <WukSIMD.hh>

using namespace wuk::simd;
#define OP4_SI(T) static inline T

alignas(16) OP4_SI(constexpr wU32) MUL_COEFFS[4] = {
    0x71e961d3U, 0x47dff135U, 0x172f4f25U, 0x49c1e3bfU
};

alignas(16) OP4_SI(constexpr wU32) INV_MUL_COEFFS[4] = {
    0x756e9e5bU, 0xd2b5991dU, 0x8ce434adU, 0x34640c3fU
};

OP4_SI(void) xor_with_iv(wByte state[wuk::crypto::WukOP4_BL],
                   const wByte iv[wuk::crypto::WukOP4_BL])
{
    sse::s128 src = sse::loadu128(state);
    sse::s128 val = sse::loadu128(iv);
    sse::s128 dst = sse::xor128(src, val);
    sse::storeu128(state, dst);
}

OP4_SI(void) xor_with_iv(wByte state[wuk::crypto::WukOP4_BL],
                   const wByte a[wuk::crypto::WukOP4_BL],
                   const wByte b[wuk::crypto::WukOP4_BL])
{
    sse::s128 dst = sse::xor128(sse::loadu128(a), sse::loadu128(b));
    sse::storeu128(state, dst);
}

OP4_SI(void) cipher(wByte state[wuk::crypto::WukOP4_BL],
              const wByte input[wuk::crypto::WukOP4_BL],
              const wByte round_key[wuk::crypto::WukOP4_RKL])
{
    sse::s128 temp;
    sse::s128 rk[8] = {
        sse::loadu128(round_key + 0 * wuk::crypto::WukOP4_BL),
        sse::loadu128(round_key + 1 * wuk::crypto::WukOP4_BL),
        sse::loadu128(round_key + 2 * wuk::crypto::WukOP4_BL),
        sse::loadu128(round_key + 3 * wuk::crypto::WukOP4_BL),
        sse::loadu128(round_key + 4 * wuk::crypto::WukOP4_BL),
        sse::loadu128(round_key + 5 * wuk::crypto::WukOP4_BL),
        sse::loadu128(round_key + 6 * wuk::crypto::WukOP4_BL),
        sse::loadu128(round_key + 7 * wuk::crypto::WukOP4_BL),
    };
    sse::s128 mul = sse::loadu128(MUL_COEFFS);

    temp = sse::loadu128(input);
    for (wU32 r = 0; r < wuk::crypto::WukOP4_NR; ++r) {
        // shift bit add
        wU32 v0 = sse::extract<simd_size::epi32, 0>(temp);
        wU32 v1 = sse::extract<simd_size::epi32, 1>(temp);
        wU32 v2 = sse::extract<simd_size::epi32, 2>(temp);
        wU32 v3 = sse::extract<simd_size::epi32, 3>(temp);
        v0 = rotl32(v0, 13) + v1 + v2 + v3;
        v1 = rotl32(v1, 19) + v2 + v3 + v0;
        v2 = rotl32(v2, 11) + v3 + v0 + v1;
        v3 = rotl32(v3, 17) + v0 + v1 + v2;
        temp = _mm_set_epi32(v3, v2, v1, v0);
        // multiply
        temp = sse::mullo<simd_size::epi32>(temp, mul);
        // round key add
        temp = sse::xor128(temp, rk[r]);
    }
    sse::storeu128(state, temp);
}

OP4_SI(void) inv_cipher(wByte state[wuk::crypto::WukOP4_BL],
                  const wByte input[wuk::crypto::WukOP4_BL],
                  const wByte round_key[wuk::crypto::WukOP4_RKL])
{
    sse::s128 temp;
    sse::s128 rk[8] = {
        sse::loadu128((round_key + 7 * wuk::crypto::WukOP4_BL)),
        sse::loadu128((round_key + 6 * wuk::crypto::WukOP4_BL)),
        sse::loadu128((round_key + 5 * wuk::crypto::WukOP4_BL)),
        sse::loadu128((round_key + 4 * wuk::crypto::WukOP4_BL)),
        sse::loadu128((round_key + 3 * wuk::crypto::WukOP4_BL)),
        sse::loadu128((round_key + 2 * wuk::crypto::WukOP4_BL)),
        sse::loadu128((round_key + 1 * wuk::crypto::WukOP4_BL)),
        sse::loadu128((round_key + 0 * wuk::crypto::WukOP4_BL))
    };
    sse::s128 inv_mul = sse::loadu128(INV_MUL_COEFFS);

    temp = sse::loadu128(input);
    for (wU32 r = 0; r < wuk::crypto::WukOP4_NR; ++r) {
        // round key add
        temp = sse::xor128(temp, rk[r]);
        // inv multiply
        temp = sse::mullo<simd_size::epi32>(temp, inv_mul);
        // shift bits sub
        wU32 v0 = sse::extract<simd_size::epi32, 0>(temp);
        wU32 v1 = sse::extract<simd_size::epi32, 1>(temp);
        wU32 v2 = sse::extract<simd_size::epi32, 2>(temp);
        wU32 v3 = sse::extract<simd_size::epi32, 3>(temp);
        v3 = rotr32(v3 - v0 - v1 - v2, 17);
        v2 = rotr32(v2 - v3 - v0 - v1, 11);
        v1 = rotr32(v1 - v2 - v3 - v0, 19);
        v0 = rotr32(v0 - v1 - v2 - v3, 13);
        temp = _mm_set_epi32(v3, v2, v1, v0);
    }
    sse::storeu128(state, temp);
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
        k[i] += rotl8(k[i] ^ k[i+1] ^ k[i+2] ^ k[i+3], 5);
    }
    wU32 v0, v1, v2, v3, v4, v5, v6, v7;
    wU32 t0, t1, t2, t3, t4, t5, t6, t7;

    // Introduce a diffusion mechanism for key
    v0 = load32le(k     ); t0 = v0;
    v1 = load32le(k +  4); t1 = v1;
    v2 = load32le(k +  8); t2 = v2;
    v3 = load32le(k + 12); t3 = v3;
    v4 = load32le(k + 16); t4 = v4;
    v5 = load32le(k + 20); t5 = v5;
    v6 = load32le(k + 24); t6 = v6;
    v7 = load32le(k + 28); t7 = v7;

    t7 += rotl32((v0 ^ v7) + v6, 15);
    t6 += rotl32((v7 ^ v6) + v5, 19);
    t5 += rotl32((v6 ^ v5) + v4, 21);
    t4 += rotl32((v5 ^ v4) + v3, 29);
    t3 += rotl32((v4 ^ v3) + v2, 13);
    t2 += rotl32((v3 ^ v2) + v1, 7);
    t1 += rotl32((v2 ^ v1) + v0, 23);
    t0 += rotl32((v1 ^ v0) + v7, 17);

    pack32le(k,      t0);
    pack32le(k + 4,  t1);
    pack32le(k + 8,  t2);
    pack32le(k + 12, t3);
    pack32le(k + 16, t4);
    pack32le(k + 20, t5);
    pack32le(k + 24, t6);
    pack32le(k + 28, t7);
}

OP4_SI(void) key_schedule_transformation(wByte key[wuk::crypto::WukOP4_KL])
{
    for (wU32 r = 0; r < wuk::crypto::WukOP4_NR; ++r) {
        prevent_zero_key(key);
        key_obfuscation(key);
    }
}

OP4_SI(void) key_extension(wByte key[wuk::crypto::WukOP4_KL],
                           wByte round_key[wuk::crypto::WukOP4_RKL])
{
    for (wU32 i = 0; i < wuk::crypto::WukOP4_NK; ++i) {
        key_schedule_transformation(key);
        memcpy(round_key + i * wuk::crypto::WukOP4_KL, key,
            wuk::crypto::WukOP4_KL);
    }
}

wuk::crypto::WukOP4::WukOP4(const wByte key[WukOP4_KL], wU32 counter)
: counter(counter)
{
    if (!key) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::WukOP4::WukOP4",
            "key is nullptr.");
    }
    wByte copy_key[WukOP4_KL]{};

    memcpy(copy_key, key, WukOP4_KL);
    key_extension(copy_key, this->round_key);

    wuk::memory_secure(copy_key, WukOP4_KL); // Clear sensitive data
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
        pack32le(keystream + WukOP4_NL, this->counter++);
        cipher(state, keystream, this->round_key);
        xor_with_iv(out, in, state);
        out += WukOP4_BL;
        in += WukOP4_BL;
        remaining -= WukOP4_BL;
    }
    if (remaining > 0) {  // 处理末尾部分块
        pack32le(keystream + WukOP4_NL, this->counter++);
        cipher(state, keystream, this->round_key);
        for (size_t i = 0; i < remaining; i++) {  // 逐字节处理
            out[i] = in[i] ^ state[i];
        }
    }
}
