#include <fstream>
#include <stdexcept>

#include <iostream>
#include <filesystem>
#include <format>

#include <network/wmkc_net.c>
#include <network/wmkc_ssl.c>
#include <crypto/snc.c>
#include <wmkc_memory.c>
#include <wmkc_struct.c>
#include <wmkc_random.c>
#include <wmkc_basic.c>
#include <wmkc_file.c>
#include <wmkc_misc.c>
#include <wmkc_time.c>
#include <wmkc_pad.c>

using namespace std;

class SNC_Object {
    private:
        wmkcSNC_obj *snc;
        wmkcErr_obj error;

    public:
        wmkc_u32 segmentSize;
        enum SNC_Xcrypt_Mode {
            SNC_ECB, SNC_CBC, SNC_CFB, SNC_CTR
        };

        SNC_Object(const wmkcByte *key, const wmkcByte *iv, SNC_mode mode)
        : snc(), error(), segmentSize(256)
        {
            switch(mode) {
                case SNC_256:
                case SNC_512:
                case SNC_768: break;
                default:
                    throw runtime_error("Illegal mode, only supported: SNC_256, SNC_512, SNC_768");
            }
            if((error = wmkcSNC_new(&this->snc, mode)).code) {
                throw runtime_error(format("{0}[{1}]:{2}", error.func, error.code, error.message));
            }
        }

        ~SNC_Object() {
            wmkcSNC_free(&this->snc);
        }

        void encrypt(wmkcByte *content, wmkcSize size, SNC_Xcrypt_Mode mode)
        {
            switch(mode) {
                case SNC_ECB:
                    wmkcSNC_ecb_encrypt(this->snc, content, size); break;
                case SNC_CBC:
                    wmkcSNC_cbc_encrypt(this->snc, content, size); break;
                case SNC_CFB:
                    wmkcSNC_cfb_encrypt(this->snc, content, size, this->segmentSize); break;
                case SNC_CTR:
                    wmkcSNC_ctr_xcrypt(this->snc, content, size); break;
            }
        }

        void decrypt(wmkcByte *content, wmkcSize size, SNC_Xcrypt_Mode mode)
        {
            switch(mode) {
                case SNC_ECB:
                    wmkcSNC_ecb_decrypt(this->snc, content, size); break;
                case SNC_CBC:
                    wmkcSNC_cbc_decrypt(this->snc, content, size); break;
                case SNC_CFB:
                    wmkcSNC_cfb_decrypt(this->snc, content, size, this->segmentSize); break;
                case SNC_CTR:
                    wmkcSNC_ctr_xcrypt(this->snc, content, size); break;
            }
        }
};

WMKC_PRIVATE_CONST(wmkcByte) SNC_TEST_KEY[96] = {
    0xf1, 0x4b, 0xac, 0x47, 0x97, 0x28, 0x9b, 0x2b, 0xac, 0x54, 0xa8, 0xc7, 0xd4, 0xe2, 0xe8, 0xa2,
    0xfc, 0x8b, 0x25, 0xf5, 0xc0, 0x5d, 0x86, 0x2e, 0xb7, 0x6c, 0x8f, 0xa7, 0x7f, 0xf4, 0xe1, 0x47,
    0x28, 0x6f, 0x69, 0xd3, 0xca, 0x11, 0x9e, 0xe4, 0x96, 0xb3, 0xda, 0x23, 0x17, 0x00, 0xc5, 0x74,
    0x07, 0x1d, 0x43, 0x00, 0x30, 0x80, 0xd0, 0xfb, 0x5a, 0xa1, 0xa0, 0xe0, 0x2f, 0x5e, 0xe9, 0x4c,
    0xdc, 0x6b, 0x67, 0x57, 0xad, 0xe1, 0xd7, 0xb2, 0x7b, 0x48, 0xb4, 0x5f, 0x07, 0xe8, 0xac, 0x00,
    0x7a, 0xc5, 0x4d, 0x6c, 0x42, 0xb1, 0xb1, 0xee, 0x72, 0x75, 0xe3, 0xd7, 0x2a, 0x84, 0x8a, 0xd8};
WMKC_PRIVATE_CONST(wmkcByte) SNC_TEST_IV[32] = {
    0x73, 0x8b, 0x58, 0x77, 0x1f, 0x86, 0xa7, 0x73, 0xe3, 0x30, 0x03, 0xd2, 0x7b, 0x20, 0xba, 0xb9,
    0x26, 0x34, 0xb7, 0x98, 0xa1, 0xd5, 0x21, 0xed, 0xa5, 0x2d, 0x3f, 0x18, 0x4b, 0x1e, 0x61, 0xee};

void sncObject_test()
{
    SNC_Object *SNC = wmkcNull;
    wmkcChar _tmp[2048] = {"hello, world.\n"};
    wmkcByte *buf = (wmkcByte *)_tmp;
    wmkcSize size = strlen(_tmp);

    wmkcPad_add(buf, &size, SNC_BLOCKLEN, false);

    SNC = new SNC_Object(SNC_TEST_KEY, SNC_TEST_IV, SNC_768);
    SNC->segmentSize = 32;

    cout << "Plaintext" << endl;
    wmkcMisc_PRINT(buf, size, 32, 1, 0);

    SNC->encrypt(buf, size, SNC_Object::SNC_CFB);

    cout << "Ciphertext" << endl;
    wmkcMisc_PRINT(buf, size, 32, 1, 0);

    delete SNC;
}

int main(int argc, char **argv)
{
    sncObject_test();
    return 0;
}
