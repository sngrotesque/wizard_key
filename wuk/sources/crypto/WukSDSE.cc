#include <crypto/WukSDSE.hh>

#ifdef WUK_COMPILER_MSVC
#   define ROTL32(x, n) _rotl(x, n)
#else
#   define ROTL32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#endif

static inline wU32 load32_le(const wByte d[4]) noexcept
{
    wU32 w;
#   if WUK_IS_LITTLE_ENDIAN()
    memcpy(&w, d, sizeof w);
#   else
    w =  (wU32) d[0];
    w |= (wU32) d[1] <<  8;
    w |= (wU32) d[2] << 16;
    w |= (wU32) d[3] << 24;
#   endif
    return w;
}

static inline void store32_le(wByte dst[4], wU32 w)
{
#   if WUK_IS_LITTLE_ENDIAN()
    memcpy(dst, &w, sizeof w);
#   else
    dst[0] = (uint8_t) w; w >>= 8;
    dst[1] = (uint8_t) w; w >>= 8;
    dst[2] = (uint8_t) w; w >>= 8;
    dst[3] = (uint8_t) w;
#   endif
}

static inline void keystream_mixture(wU32 state[16]) noexcept
{
    state[0]  += ROTL32(state[5], 3);
    state[5]  ^= ROTL32(state[10], 5);
    state[10] += ROTL32(state[15], 7);
    state[15] ^= state[0];

    state[0]  ^= (state[15] - state[14]) ^ state[13];

    state[4]  += ROTL32(state[9], 11);
    state[9]  ^= ROTL32(state[14], 13);
    state[14] += ROTL32(state[3], 17);
    state[3]  ^= state[4];

    state[1]  ^= (state[12] ^ state[11]) - state[10];

    state[8]  ^= ROTL32(state[13], 19);
    state[13] += ROTL32(state[2], 23);
    state[2]  ^= ROTL32(state[7], 29);
    state[7]  += state[8];

    state[2]  ^= (state[9]  - state[8])  ^ state[7];

    state[12] ^= ROTL32(state[1], 31);
    state[1]  += ROTL32(state[6],  1);
    state[6]  ^= ROTL32(state[11], 2);
    state[11] += state[12];

    state[3]  ^= (state[6]  ^ state[5])  - state[4];
}

/*
static inline void keystream_mixture_reverse(wU32 state[16]) noexcept
{
    // 逆向最后一步异或操作
    state[3]  ^= (state[6] ^ state[5]) - state[4];

    // 逆向斜角混合（从下往上回退）
    state[11] -= state[12];
    state[6]  ^= ROTL32(state[11], 2);
    state[1]  -= ROTL32(state[6], 1);
    state[12] ^= ROTL32(state[1], 31);

    state[2]  ^= (state[9] - state[8]) ^ state[7];

    state[7]  -= state[8];
    state[2]  ^= ROTL32(state[7], 29);
    state[13] -= ROTL32(state[2], 23);
    state[8]  ^= ROTL32(state[13], 19);

    state[1]  ^= (state[12] ^ state[11]) - state[10];

    state[3]  ^= state[4];
    state[14] -= ROTL32(state[3], 17);
    state[9]  ^= ROTL32(state[14], 13);
    state[4]  -= ROTL32(state[9], 11);

    state[0]  ^= (state[15] - state[14]) ^ state[13];

    state[15] ^= state[0];
    state[10] -= ROTL32(state[15], 7);
    state[5]  ^= ROTL32(state[10], 5);
    state[0]  -= ROTL32(state[5], 3);
}
*/

wuk::crypto::SDSE::SDSE(const wByte key[32], const wByte nonce[20], wU32 counter)
{
    this->init(key, nonce, counter);
}

wuk::crypto::SDSE::~SDSE()
{
    this->clean(); // Clean the state securely
}

void wuk::crypto::SDSE::init(const wByte key[32], const wByte nonce[20], wU32 counter)
{
    if (!key || !nonce) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::SDSE::init",
            "key or iv is nullptr.");
    }

    // Initialize the state with the key and IV
    this->state[0] = load32_le(key);
    this->state[1] = load32_le(key + 4);
    this->state[2] = load32_le(key + 8);
    this->state[3] = load32_le(key + 12);
    this->state[4] = load32_le(key + 16);
    this->state[5] = load32_le(key + 20);
    this->state[6] = load32_le(key + 24);
    this->state[7] = load32_le(key + 28);
    this->state[8] = load32_le(nonce);
    this->state[9] = load32_le(nonce + 4);

    // Initialize the constant values
    this->state[10] = 0xd2436335U;
    this->state[11] = 0xb2052534U;

    // Initialize the counter
    this->state[12] = load32_le(nonce + 8);
    this->state[13] = load32_le(nonce + 12);
    this->state[14] = load32_le(nonce + 16);

    wByte counter_array[4]{0};
    store32_le(counter_array, counter);
    this->state[15] = load32_le(counter_array);
}

void wuk::crypto::SDSE::xcrypt(wByte *buffer, wSize length)
{
    if (!buffer) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::SDSE::xcrypt",
            "The buffer is nullptr.");
    }
    wU32 tmp[16]{0};
    wByte *ks = reinterpret_cast<wByte *>(tmp);

    for (wSize i = 0, ks_i = WUK_SDSE_KSLEN; i < length; ++i, ++ks_i) {
        if (ks_i == WUK_SDSE_KSLEN) {
            // Generate a new keystream, Copy the current state to a temporary buffer
            wU32 t1  = this->state[0];
            wU32 t2  = this->state[1];
            wU32 t3  = this->state[2];
            wU32 t4  = this->state[3];
            wU32 t5  = this->state[4];
            wU32 t6  = this->state[5];
            wU32 t7  = this->state[6];
            wU32 t8  = this->state[7];
            wU32 t9  = this->state[8];
            wU32 t10 = this->state[9];
            wU32 t11 = this->state[10];
            wU32 t12 = this->state[11];
            wU32 t13 = this->state[12];
            wU32 t14 = this->state[13];
            wU32 t15 = this->state[14];
            wU32 t16 = this->state[15];

            store32_le(ks,      t1);
            store32_le(ks + 4,  t2);
            store32_le(ks + 8,  t3);
            store32_le(ks + 12, t4);
            store32_le(ks + 16, t5);
            store32_le(ks + 20, t6);
            store32_le(ks + 24, t7);
            store32_le(ks + 28, t8);
            store32_le(ks + 32, t9);
            store32_le(ks + 36, t10);
            store32_le(ks + 40, t11);
            store32_le(ks + 44, t12);
            store32_le(ks + 48, t13);
            store32_le(ks + 52, t14);
            store32_le(ks + 56, t15);
            store32_le(ks + 60, t16);

            // Update the keystream (10 Rounds of mixing)
            // 请勿将此循环修改为10次的单次循环，否则将影响性能
            for (wU32 j = 0; j < 5; ++j) {
                keystream_mixture(tmp);
                keystream_mixture(tmp);
            }

            // Update the state for the next keystream generation
            if (this->state[15]++ == 0) {
                ++this->state[14];
            }

            // Reset the keystream index
            ks_i = 0;
        }

        buffer[i] ^= ks[ks_i];
    }
}
