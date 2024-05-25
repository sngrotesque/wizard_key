#include <crypto/fea.hpp>

wVoid wmkc::crypto::FEA::ecb_encrypt(wByte *p)
{
    this->cipher(p, this->roundKey);
}

wVoid wmkc::crypto::FEA::ecb_decrypt(wByte *c)
{
    this->invCipher(c, this->roundKey);
}
