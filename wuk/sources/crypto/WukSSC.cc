#include <crypto/WukSSC.hh>

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

    keystream_mixture(this->state);
    keystream_mixture(this->state);

    // keystream_bits_swap(this->keystream);

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
    * 3 | I I I I I I I I C C C C C C C C | (IV       32 ~ 39, Constant 40 ~ 47)
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
