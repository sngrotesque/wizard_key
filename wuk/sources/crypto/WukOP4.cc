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

#include "WukOP4_modes.cc"
