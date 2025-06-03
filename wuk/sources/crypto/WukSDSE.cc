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

static inline void keystream_mixture(wU32 state[16]) noexcept
{
    // 向左循环4值混合
    state[0]  ^= state[15] ^ state[14] ^ state[13];
    state[1]  ^= state[12] ^ state[11] ^ state[10];
    state[2]  ^= state[9]  ^ state[8]  ^ state[7];
    state[3]  ^= state[6]  ^ state[5]  ^ state[4];

    // 斜角混合
    state[0]  += ROTL32(state[5], 3);
    state[5]  += ROTL32(state[10], 5);
    state[10] += ROTL32(state[15], 7);
    state[15] += state[0];

    state[4]  += ROTL32(state[9], 11);
    state[9]  += ROTL32(state[14], 13);
    state[14] += ROTL32(state[3], 17);
    state[3]  += state[4];

    state[8]  += ROTL32(state[13], 19);
    state[13] += ROTL32(state[2], 23);
    state[2]  += ROTL32(state[7], 29);
    state[7]  += state[8];

    state[12] += ROTL32(state[1], 31);
    state[1]  += ROTL32(state[6],  1);
    state[6]  += ROTL32(state[11], 2);
    state[11] += state[12];
}

wuk::crypto::SDSE::SDSE(const wByte *key, const wByte *iv, const wuk::crypto::Counter counter)
{
    if (!key || !iv) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::SDSE::SDSE",
            "key or iv is nullptr.");
    }

    // Initialize the keystream
    this->init(key, iv, counter);
}

wuk::crypto::SDSE::~SDSE()
{
    // Clean the state securely
    this->clean();
}

void wuk::crypto::SDSE::init(const wByte *key, const wByte *iv, wuk::crypto::Counter counter)
{
    if (!key || !iv) {
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
    this->state[8] = load32_le(iv);
    this->state[9] = load32_le(iv + 4);

    // Initialize the constant values
    this->state[10] = 0xd2436335U;
    this->state[11] = 0xb2052534U;

    // Initialize the counter
    const wByte *counter_ptr = counter.get();
    this->state[12] = load32_le(counter_ptr);
    this->state[13] = load32_le(counter_ptr + 4);
    this->state[14] = load32_le(counter_ptr + 8);
    this->state[15] = load32_le(counter_ptr + 12);
}

void wuk::crypto::SDSE::xcrypt(wByte *buffer, wSize length)
{
    if (!buffer) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::SDSE::xcrypt",
            "The buffer is nullptr.");
    }
    wU32 tmp[16]{0};
    wByte ks[WUK_SDSE_KSLEN]{0};

    for (wSize i = 0, ks_i = WUK_SDSE_KSLEN; i < length; ++i, ++ks_i) {
        if (ks_i == WUK_SDSE_KSLEN) {
            // Generate a new keystream
            // Copy the current state to a temporary buffer
            memcpy(tmp, this->state, WUK_SDSE_KSLEN);

            // Update the keystream (10 Rounds of mixing)
            keystream_mixture(tmp);
            keystream_mixture(tmp);
            keystream_mixture(tmp);
            keystream_mixture(tmp);
            keystream_mixture(tmp);

            keystream_mixture(tmp);
            keystream_mixture(tmp);
            keystream_mixture(tmp);
            keystream_mixture(tmp);
            keystream_mixture(tmp);

            // Copy the keystream to the ks array
            memcpy(ks, tmp, WUK_SDSE_KSLEN);

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
