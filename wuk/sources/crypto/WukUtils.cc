#include <crypto/WukUtils.hh>

wuk::crypto::Counter::Counter(const wByte *nonce, wU32 size, wSize begin)
{
    if (size >= COUNTER_BLOCK_SIZE) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::crypto::Counter::Counter",
                            "Nonce is too long.");
    }
    memcpy(this->counter, nonce, size);

    this->counter[8]  |= (begin >> 56) & 0xff;
    this->counter[9]  |= (begin >> 48) & 0xff;
    this->counter[10] |= (begin >> 40) & 0xff;
    this->counter[11] |= (begin >> 32) & 0xff;
    this->counter[12] |= (begin >> 24) & 0xff;
    this->counter[13] |= (begin >> 16) & 0xff;
    this->counter[14] |= (begin >> 8)  & 0xff;
    this->counter[15] |= begin         & 0xff;
}

wuk::crypto::Counter::Counter(const char *nonce, wU32 size, wSize begin)
: Counter(reinterpret_cast<const wByte *>(nonce), size, begin)
{

}

wuk::crypto::Counter::Counter(std::string nonce, wSize begin)
: Counter(nonce.c_str(), nonce.size(), begin)
{

}

wByte *wuk::crypto::Counter::get() noexcept
{
    return this->counter;
}

void wuk::crypto::Counter::clean() noexcept
{
    wuk::memory_zero(this->counter, sizeof(counter));
}

void wuk::crypto::Counter::step_up() noexcept
{
    for (wI32 i = (COUNTER_BLOCK_SIZE - 1); i >= 0; --i) {
        if (*(this->counter + i) != 0xff) {
            ++(*(this->counter + i));
            break;
        }
        *(this->counter + i) = 0x00;
    }
}
