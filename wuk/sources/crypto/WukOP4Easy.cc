#include <crypto/WukOP4Easy.hh>

#include <crypto/WukCommon.hh>
#include <WukSIMD.hh>

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

OP4_SI(void) cipher(wuk::byte state[wuk::crypto::OP4_BL],
              const wuk::byte input[wuk::crypto::OP4_BL]) noexcept
{
    memcpy(state, input, wuk::crypto::OP4_BL);
    for (wuk::u32 r = 0; r < wuk::crypto::OP4_NR; ++r) {
        shift_bits_add(state);
        multiply(state);
    }
}

OP4_SI(void) inv_cipher(wuk::byte state[wuk::crypto::OP4_BL],
                  const wuk::byte input[wuk::crypto::OP4_BL]) noexcept
{
    memcpy(state, input, wuk::crypto::OP4_BL);
    for (wuk::u32 r = wuk::crypto::OP4_NR; r-- > 0; ) {
        inv_multiply(state);
        shift_bits_sub(state);
    }
}

namespace wuk::crypto {
    bool OP4Easy::crypto_stream(wuk::byte *out, const wuk::byte *in, wuk::ulong length)
    {
        if(!out || !in) {
            return false;
        }
        wuk::ulong remaining = length;
        while (remaining >= OP4_BL) {
            cipher(out, in);
            out += OP4_BL;
            in += OP4_BL;
            remaining -= OP4_BL;
        }
        if (remaining > 0) {
            wuk::byte state[OP4_BL]{0};
            wuk::byte input_block[OP4_BL]{0};

            memcpy(input_block, in, remaining); 
            cipher(state, input_block);

            for (wuk::ulong i = 0; i < remaining; i++) {
                out[i] = in[i] ^ state[i];
            }
        }
        return true;
    }
}
