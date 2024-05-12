#include <crypto/fea.hpp>

wVoid wmkc::crypto::fea::ecb_encrypt(wByte *p)
{
    this->cipher(p, this->roundKey);
}

wVoid wmkc::crypto::fea::ecb_decrypt(wByte *c)
{
    this->invCipher(c, this->roundKey);
}
