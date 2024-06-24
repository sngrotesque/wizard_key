#include <crypto/utils.hpp>

wmkc::crypto::Counter::Counter()
{

}

wmkc::crypto::Counter::Counter(wByte *nonce, wU32 size, wSize begin)
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

wmkc::crypto::Counter::Counter(const char *nonce, wU32 size, wSize begin)
// Counter -> Counter(wByte *nonce, wU32 size, wSize begin)
: Counter((wByte *)nonce, size, begin)
{

}

wmkc::crypto::Counter::Counter(std::string nonce, wSize begin)
// Counter -> Counter(const char *nonce, wU32 size, wSize begin)
: Counter(nonce.c_str(), nonce.size(), begin)
{

}
