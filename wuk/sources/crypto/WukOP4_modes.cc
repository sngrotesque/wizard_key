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
    wuk::u32 v[8]{};
    wuk::u32 t[8]{};

    // Introduce a diffusion mechanism for key
    t[0] = (v[0] = wuk::crypto::load32le(k     ));
    t[1] = (v[1] = wuk::crypto::load32le(k +  4));
    t[2] = (v[2] = wuk::crypto::load32le(k +  8));
    t[3] = (v[3] = wuk::crypto::load32le(k + 12));
    t[4] = (v[4] = wuk::crypto::load32le(k + 16));
    t[5] = (v[5] = wuk::crypto::load32le(k + 20));
    t[6] = (v[6] = wuk::crypto::load32le(k + 24));
    t[7] = (v[7] = wuk::crypto::load32le(k + 28));

    t[7] += wuk::crypto::rotl32((v[0] ^ v[7]) + v[6], 15);
    t[6] += wuk::crypto::rotl32((v[7] ^ v[6]) + v[5], 19);
    t[5] += wuk::crypto::rotl32((v[6] ^ v[5]) + v[4], 21);
    t[4] += wuk::crypto::rotl32((v[5] ^ v[4]) + v[3], 29);
    t[3] += wuk::crypto::rotl32((v[4] ^ v[3]) + v[2], 13);
    t[2] += wuk::crypto::rotl32((v[3] ^ v[2]) + v[1], 7);
    t[1] += wuk::crypto::rotl32((v[2] ^ v[1]) + v[0], 23);
    t[0] += wuk::crypto::rotl32((v[1] ^ v[0]) + v[7], 17);

    wuk::crypto::pack32le(k,      t[0]);
    wuk::crypto::pack32le(k + 4,  t[1]);
    wuk::crypto::pack32le(k + 8,  t[2]);
    wuk::crypto::pack32le(k + 12, t[3]);
    wuk::crypto::pack32le(k + 16, t[4]);
    wuk::crypto::pack32le(k + 20, t[5]);
    wuk::crypto::pack32le(k + 24, t[6]);
    wuk::crypto::pack32le(k + 28, t[7]);
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
        : m_counter(counter)
    {
        if (!key) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::OP4::OP4",
                "key is nullptr.");
        }
        key_extension(key, this->m_round_key);
    }

    OP4::~OP4()
    {
        wuk::memory_secure(this->m_round_key, sizeof(this->m_round_key));
    }

    void OP4::ecb_encrypt(wuk::byte *out, const wuk::byte *in, wuk::ulong length) const
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
            cipher(out + i, in + i, this->m_round_key);
        }
    }

    void OP4::ecb_decrypt(wuk::byte *out, const wuk::byte *in, wuk::ulong length) const
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
            inv_cipher(out + i, in + i, this->m_round_key);
        }
    }

    void OP4::cbc_encrypt(wuk::byte *out, const wuk::byte *in,
                          wuk::ulong length, const wuk::byte iv[OP4_BL]) const
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
            cipher(out + i, buffer, this->m_round_key);
            memcpy(buffer, out + i, OP4_BL);
        }
    }

    void OP4::cbc_decrypt(wuk::byte *out, const wuk::byte *in,
                          wuk::ulong length, const wuk::byte iv[OP4_BL]) const
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
            inv_cipher(buffer, in + i, this->m_round_key);
            xor_with_iv(out + i, buffer, prev);
            memcpy(prev, in + i, OP4_BL);
        }
    }

    void OP4::ofb_stream(wuk::byte *out, const wuk::byte *in,
                         wuk::ulong length, const wuk::byte iv[OP4_BL]) const
    {
        if(!out || !in || !iv) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::crypto::OP4::ofb_stream",
                "out/in/iv is nullptr.");
        }
        wuk::byte feedback[OP4_BL]{0};
        memcpy(feedback, iv, OP4_BL);

        size_t remaining = length;
        while (remaining >= OP4_BL) {
            cipher(feedback, feedback, this->m_round_key);
            xor_with_iv(out, in, feedback);
            out += OP4_BL;
            in += OP4_BL;
            remaining -= OP4_BL;
        }
        if (remaining > 0) {
            cipher(feedback, feedback, this->m_round_key);
            for (size_t i = 0; i < remaining; i++) {
                out[i] = in[i] ^ feedback[i];
            }
        }
    }

    void OP4::ctr_stream(wuk::byte *out, const wuk::byte *in,
                         wuk::ulong length, const wuk::byte nonce[OP4_NL])
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
            pack32le(keystream + OP4_NL, this->m_counter++);
            cipher(state, keystream, this->m_round_key);
            xor_with_iv(out, in, state);
            out += OP4_BL;
            in += OP4_BL;
            remaining -= OP4_BL;
        }
        if (remaining > 0) {
            pack32le(keystream + OP4_NL, this->m_counter++);
            cipher(state, keystream, this->m_round_key);
            for (size_t i = 0; i < remaining; i++) {
                out[i] = in[i] ^ state[i];
            }
        }
    }
}