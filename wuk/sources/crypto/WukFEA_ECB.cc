#include <crypto/WukFEA.hh>

void wuk::crypto::FEA::ecb_encrypt(wByte *p)
{
    this->cipher(p, this->roundKey);
}

void wuk::crypto::FEA::ecb_decrypt(wByte *c)
{
    this->inv_cipher(c, this->roundKey);
}
