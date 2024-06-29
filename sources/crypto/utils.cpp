#include <crypto/utils.hpp>

wuk::crypto::Counter::Counter()
{

}

wuk::crypto::Counter::Counter(wByte *nonce, wU32 size, wSize begin)
: counter()
{
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
// Counter -> Counter(wByte *nonce, wU32 size, wSize begin)
: Counter((wByte *)nonce, size, begin)
{

}

wuk::crypto::Counter::Counter(std::string nonce, wSize begin)
// Counter -> Counter(const char *nonce, wU32 size, wSize begin)
: Counter(nonce.c_str(), nonce.size(), begin)
{

}
