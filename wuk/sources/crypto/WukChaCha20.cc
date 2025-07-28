#include <crypto/WukChaCha20.hh>

wuk::crypto::WukChaCha20::WukChaCha20(const wByte key[WukCC20_KL], wU32 counter)
: counter(counter)
{
    if (!key) {
        wuk::Exception(wuk::Error::NPTR, "wuk::crypto::WukChaCha20::WukChaCha20",
            "key is nullptr.");
    }
    memcpy(this->key, key, WukCC20_KL);
}

wuk::crypto::WukChaCha20::~WukChaCha20()
{
    wuk::memory_secure(this->key, WukCC20_KL);
}

void wuk::crypto::WukChaCha20::crypto_stream(wByte *out, const wByte *in, wSize length,
                                             wByte nonce[WukCC20_NL])
{
    crypto_stream_chacha20_ietf_xor_ic(out, in, length,
        nonce, this->counter, this->key);
    this->counter += ((length + WukCC20_KSL - 1) / WukCC20_KSL);
}
