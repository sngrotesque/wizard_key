#include <crypto/WukSSC.hh>

/*
static constexpr wByte sbox[256] = {
    0x80, 0x30, 0x4f, 0x85, 0x65, 0x8c, 0x6a, 0xaa, 0x1b, 0x87, 0xcd, 0xb1, 0x53, 0x6e, 0x39, 0x28,
    0x8d, 0xb3, 0xca, 0x0d, 0xa0, 0xfa, 0x72, 0x10, 0x44, 0x35, 0x31, 0x41, 0x9e, 0x79, 0xba, 0x9f,
    0x15, 0xbc, 0xf6, 0x26, 0x27, 0xf9, 0x8f, 0xc9, 0xb7, 0x7f, 0x70, 0x5a, 0x60, 0x24, 0x40, 0x1d,
    0x13, 0xd3, 0x89, 0x5c, 0xe6, 0x81, 0xf2, 0x0e, 0x2d, 0xb2, 0xd8, 0x04, 0xda, 0x2e, 0x50, 0x47,
    0x71, 0x06, 0x02, 0xe9, 0xea, 0xa1, 0xe7, 0xf8, 0xfb, 0x18, 0x7c, 0x7a, 0xbb, 0xfc, 0xa3, 0xb8,
    0x63, 0x3f, 0xb6, 0x97, 0xc2, 0x01, 0x6c, 0x5d, 0x3c, 0xa7, 0x58, 0xf4, 0xa2, 0x46, 0x82, 0xa9,
    0xce, 0xf3, 0xcf, 0x93, 0x45, 0x2c, 0xbf, 0xeb, 0xe8, 0x7d, 0x3a, 0xbe, 0x2f, 0x11, 0xac, 0xd9,
    0x95, 0xee, 0x9c, 0x32, 0xb5, 0xc3, 0x0a, 0x1f, 0x8e, 0x98, 0xe2, 0x1e, 0xd6, 0x17, 0xe1, 0xae,
    0x43, 0x56, 0xe4, 0x3e, 0xf1, 0xd4, 0x48, 0xcc, 0xe5, 0xc6, 0x49, 0xa4, 0x55, 0x1c, 0x6b, 0x77,
    0xcb, 0xef, 0x7b, 0x36, 0x8a, 0xc8, 0x23, 0xb0, 0x99, 0x29, 0xd1, 0xc5, 0x52, 0x3d, 0x05, 0x33,
    0x4a, 0xc7, 0x22, 0x94, 0xf5, 0x3b, 0x69, 0xff, 0xe3, 0x59, 0x5f, 0xd7, 0x96, 0x4e, 0x64, 0x00,
    0x25, 0x66, 0xb4, 0x88, 0x9b, 0xc4, 0x54, 0x37, 0xfe, 0xa5, 0x5b, 0x4b, 0x57, 0x09, 0xd5, 0x62,
    0xe0, 0x8b, 0xad, 0xdb, 0x0c, 0x73, 0x0f, 0x2a, 0xfd, 0x90, 0x42, 0x2b, 0x21, 0x9d, 0x0b, 0x20,
    0x6f, 0xdc, 0xab, 0x92, 0x08, 0xde, 0x68, 0x74, 0xec, 0xaf, 0x1a, 0x51, 0x19, 0x76, 0xc0, 0x7e,
    0x38, 0x78, 0x61, 0x86, 0xd2, 0xf0, 0x12, 0x84, 0x75, 0xdf, 0x34, 0xb9, 0x91, 0x67, 0xed, 0xdd,
    0x4d, 0xbd, 0x14, 0xd0, 0x5e, 0x16, 0xa6, 0x6d, 0x03, 0xc1, 0x4c, 0x9a, 0xa8, 0xf7, 0x83, 0x07
};

static inline uint8_t get_sbox(uint8_t x) noexcept
{
    return sbox[x];
}

static inline void keystream_sub_bytes(wByte *keystream) noexcept
{
    for (wU32 i = 0; i < wuk::crypto::WUK_SSC_KSLEN; i += 8) {
        keystream[i]      = get_sbox(keystream[i]);
        keystream[i + 1]  = get_sbox(keystream[i + 1]);
        keystream[i + 2]  = get_sbox(keystream[i + 2]);
        keystream[i + 3]  = get_sbox(keystream[i + 3]);
        keystream[i + 4]  = get_sbox(keystream[i + 4]);
        keystream[i + 5]  = get_sbox(keystream[i + 5]);
        keystream[i + 6]  = get_sbox(keystream[i + 6]);
        keystream[i + 7]  = get_sbox(keystream[i + 7]);
    }
}
*/

/*
static inline uint8_t swap_4bits(uint8_t x) noexcept
{
    return ((x << 4) & 0xf0U) | (x >> 4);
}

static inline void keystream_bits_swap(wByte *keystream) noexcept
{
    for (wU32 i = 0; i < wuk::crypto::WUK_SSC_KSLEN; i += 8) {
        keystream[i]      = swap_4bits(keystream[i]);
        keystream[i + 1]  = swap_4bits(keystream[i + 1]);
        keystream[i + 2]  = swap_4bits(keystream[i + 2]);
        keystream[i + 3]  = swap_4bits(keystream[i + 3]);
        keystream[i + 4]  = swap_4bits(keystream[i + 4]);
        keystream[i + 5]  = swap_4bits(keystream[i + 5]);
        keystream[i + 6]  = swap_4bits(keystream[i + 6]);
        keystream[i + 7]  = swap_4bits(keystream[i + 7]);
    }
}
*/

/////////////////////////////////////////////////////////

static inline uint32_t move_bits_left(const uint32_t x, const uint32_t n) noexcept
{
    return (((x >> (32 - n)) | (x << n)) & 0xffffffffU);
}

static inline void keystream_mixture(wU32 *state) noexcept
{
    /* 
    * State:
    * |------------------------------------------|
    * | 00000000 | 00000001 | 00000002 | 00000003|
    * |------------------------------------------|
    * | 00000004 | 00000005 | 00000006 | 00000007|
    * |------------------------------------------|
    * | 00000008 | 00000009 | 0000000a | 0000000b|
    * |------------------------------------------|
    * | 0000000c | 0000000d | 0000000e | 0000000f|
    * |------------------------------------------|
    */
    // 向左循环4值混合
    state[0]  ^= state[15] ^ state[14] ^ state[13];
    state[1]  ^= state[12] ^ state[11] ^ state[10];
    state[2]  ^= state[9]  ^ state[8]  ^ state[7];
    state[3]  ^= state[6]  ^ state[5]  ^ state[4];

    // 斜角混合
    state[0]  += move_bits_left(state[5], 3);
    state[5]  += move_bits_left(state[10], 5);
    state[10] += move_bits_left(state[15], 7);
    state[15] += state[0];

    state[4]  += move_bits_left(state[9], 11);
    state[9]  += move_bits_left(state[14], 13);
    state[14] += move_bits_left(state[3], 17);
    state[3]  += state[4];

    state[8]  += move_bits_left(state[13], 19);
    state[13] += move_bits_left(state[2], 23);
    state[2]  += move_bits_left(state[7], 29);
    state[7]  += state[8];

    state[12] += move_bits_left(state[1], 31);
    state[1]  += move_bits_left(state[6],  1);
    state[6]  += move_bits_left(state[11], 2);
    state[11] += state[12];
}

void wuk::crypto::SSC::keystream_update() noexcept
{
    // 密钥流混合
    keystream_mixture(this->state);
    keystream_mixture(this->state);
    keystream_mixture(this->state);
    keystream_mixture(this->state);

    // 密钥流混合
    keystream_mixture(this->state);
    keystream_mixture(this->state);
    keystream_mixture(this->state);
    keystream_mixture(this->state);
}

wuk::crypto::SSC::SSC(const wByte *key, const wByte *iv, const wuk::crypto::Counter counter)
{
    this->keystream_init(key, iv, counter);
}

void wuk::crypto::SSC::keystream_init(const wByte *key, const wByte *iv, wuk::crypto::Counter counter)
{
    if (!key || !iv) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::SSC::SSC",
            "key or iv is nullptr.");
    }

    /*
    * Key stream structure:
    * 
    * \   0 1 2 3 4 5 6 7 8 9 a b c d e f
    *   - - - - - - - - - - - - - - - - - -
    * 0 | K K K K K K K K K K K K K K K K | (Key       0 ~ 15)
    * 1 | K K K K K K K K K K K K K K K K | (Key      16 ~ 31)
    * 3 | I I I I I I I I C C C C C C C C | (IV       32 ~ 39, Constant    40 ~ 47)
    * 2 | C C C C C C C C C C C C C C C C | (Counter  48 ~ 63)
    *   - - - - - - - - - - - - - - - - - -
    */

    wByte *keystream_ptr = this->keystream;

    // Key
    for (wU32 i = 0; i < WUK_SSC_KEYLEN; ++i) {
        *keystream_ptr++ = key[i];
    }
    // IV
    for (wU32 i = 0; i < WUK_SSC_IVLEN; ++i) {
        *keystream_ptr++ = iv[i];
    }

    // Constant
    *keystream_ptr++ = 0x53;
    *keystream_ptr++ = 0x36;
    *keystream_ptr++ = 0x34;
    *keystream_ptr++ = 0x2d;
    *keystream_ptr++ = 0x43;
    *keystream_ptr++ = 0x52;
    *keystream_ptr++ = 0x50;
    *keystream_ptr++ = 0x2b;

    // Counter
    const wByte *counter_ptr = counter.get();
    for (wU32 i = 0; i < wuk::crypto::COUNTER_BLOCK_SIZE; ++i) {
        *keystream_ptr++ = counter_ptr[i];
    }
}

void wuk::crypto::SSC::xcrypt(wByte *buffer, wSize length)
{
    if (!buffer) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::SSC::xcrypt",
            "The buffer is nullptr.");
    }

    for (wSize i = 0, ks_i = WUK_SSC_KSLEN; i < length; ++i, ++ks_i) {
        if (ks_i == WUK_SSC_KSLEN) {
            this->keystream_update();
            ks_i = 0;
        }
        buffer[i] ^= this->keystream[ks_i];
    }
}

const wByte *wuk::crypto::SSC::get_keystream() const
{
    return this->keystream;
}
