#include <crypto/fea.hpp>

wmkcVoid wmkcCrypto::wmkcFEA::ecb_encrypt(wmkcByte *p)
{
    this->cipher(p, this->roundKey);
}

wmkcVoid wmkcCrypto::wmkcFEA::ecb_decrypt(wmkcByte *c)
{
    this->invCipher(c, this->roundKey);
}
