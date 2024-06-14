#include <crypto/fea.hpp>

void wmkc::crypto::FEA::ecb_encrypt(wByte *p)
{
    this->cipher(p, this->roundKey);
}

void wmkc::crypto::FEA::ecb_decrypt(wByte *c)
{
    this->invCipher(c, this->roundKey);
}
