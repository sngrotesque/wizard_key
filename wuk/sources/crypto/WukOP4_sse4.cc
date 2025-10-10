#include <crypto/WukOP4.hh>

#include <core/WukEndianness.hh>
#include <core/WukException.hh>
#include <crypto/WukCommon.hh>
#include <WukMemory.hh>
#include <WukSIMD.hh>

#define OP4_SI(T) static inline T

alignas(16) OP4_SI(constexpr wuk::u32) MUL_COEFFS[4] = {
    0x71e961ddU, 0x47dff15dU, 0x172f4f2fU, 0xf9c1e3c7U
};

alignas(16) OP4_SI(constexpr wuk::u32) INV_MUL_COEFFS[4] = {
    0xf6e1fe75U, 0x185beaf5U, 0xfb0a57cfU, 0x4d82edf7U
};

OP4_SI(void) xor_with_iv(wuk::byte state[wuk::crypto::OP4_BL],
                   const wuk::byte iv[wuk::crypto::OP4_BL]) noexcept
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
                   const wuk::byte b[wuk::crypto::OP4_BL]) noexcept
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
              const wuk::byte round_key[wuk::crypto::OP4_RKL]) noexcept
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
                  const wuk::byte round_key[wuk::crypto::OP4_RKL]) noexcept
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

#include "WukOP4_modes.cc"
