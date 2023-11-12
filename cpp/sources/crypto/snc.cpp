#include <crypto/snc.hpp>

void wmkcSNC::encrypt(wmkcByte *content, wmkcSize length)
{
    switch(this->xcryptoMode) {
        case SNC_Xcrypt_Mode::ECB:
            wmkcSNC_ecb_encrypt(this->snc, content, length); break;
        case SNC_Xcrypt_Mode::CBC:
            wmkcSNC_cbc_encrypt(this->snc, content, length); break;
        case SNC_Xcrypt_Mode::CTR:
            wmkcSNC_ctr_xcrypt(this->snc, content, length); break;
        case SNC_Xcrypt_Mode::CFB:
            wmkcSNC_cfb_encrypt(this->snc, content, length, this->segmentSize); break;
    }
}

void wmkcSNC::decrypt(wmkcByte *content, wmkcSize length)
{
    switch(this->xcryptoMode) {
        case SNC_Xcrypt_Mode::ECB:
            wmkcSNC_ecb_decrypt(this->snc, content, length); break;
        case SNC_Xcrypt_Mode::CBC:
            wmkcSNC_cbc_decrypt(this->snc, content, length); break;
        case SNC_Xcrypt_Mode::CTR:
            wmkcSNC_ctr_xcrypt(this->snc, content, length); break;
        case SNC_Xcrypt_Mode::CFB:
            wmkcSNC_cfb_decrypt(this->snc, content, length, this->segmentSize); break;
    }
}
