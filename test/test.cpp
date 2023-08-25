#include <iostream>
#include <fstream>

#include <crypto/snc.c>
#include <wmkc_memory.c>
#include <wmkc_misc.c>
#include <wmkc_time.c>

#if 1
WMKC_PRIVATE_CONST(wmkcByte) SNC_TEST_KEY[96] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
WMKC_PRIVATE_CONST(wmkcByte) SNC_TEST_IV[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

using namespace std;

int file_read(const char *fn, string *FileStream)
{
    fstream *fp = new fstream(fn, ios::in | ios::binary);
    streamsize fileReadSize;
    wmkcChar _tmp_buffer[2048];

    if(!fp->is_open()) {
        cout << "Failed to open file." << endl;
        return EOF;
    }

    do {
        wmkcMem_zero(_tmp_buffer, sizeof(_tmp_buffer));
        fileReadSize = fp->read(_tmp_buffer, sizeof(_tmp_buffer)).gcount();
        FileStream->append(_tmp_buffer, fileReadSize);
    } while(fileReadSize);

    fp->close();

    delete fp;
    return 0;
}

class SNC_class {
    private:
        SNC_mode mode;
        wmkcSNC_obj *snc = wmkcNull;
        const wmkcByte *key = wmkcNull;
        const wmkcByte *iv  = wmkcNull;

    public:
        wmkcErr_obj error;

        SNC_class(SNC_mode mode, const wmkcByte *key, const wmkcByte *iv): error() {
            this->mode = mode;
            this->snc = snc;
            this->key = key;
            this->iv = iv;

            if((error = wmkcSNC_new(&this->snc, this->mode)).code) {
                return;
            }
            if((error = wmkcSNC_init(this->snc, this->key, this->iv)).code) {
                return;
            }
        }

        ~SNC_class() {
            if((error = wmkcSNC_free(&this->snc)).code) {
                return;
            }
        }

        void ecb_encrypt(wmkcByte *content, wmkcSize size) {
            wmkcSNC_ecb_encrypt(this->snc, content, size);
        }

        void ecb_decrypt(wmkcByte *content, wmkcSize size) {
            wmkcSNC_ecb_decrypt(this->snc, content, size);
        }

        void cbc_encrypt(wmkcByte *content, wmkcSize size) {
            wmkcSNC_cbc_encrypt(this->snc, content, size);
        }

        void cbc_decrypt(wmkcByte *content, wmkcSize size) {
            wmkcSNC_cbc_decrypt(this->snc, content, size);
        }

        void ctr_encrypt(wmkcByte *content, wmkcSize size) {
            wmkcSNC_ctr_xcrypt(this->snc, content, size);
        }

        void cfb_encrypt(wmkcByte *content, wmkcSize size, wmkc_u16 segment_size = 256) {
            wmkcSNC_cfb_encrypt(this->snc, content, size, segment_size);
        }

        void cfb_decrypt(wmkcByte *content, wmkcSize size, wmkc_u16 segment_size = 256) {
            wmkcSNC_cfb_decrypt(this->snc, content, size, segment_size);
        }
};

int main()
{
    SNC_class *snc = new SNC_class(SNC_512, SNC_TEST_KEY, SNC_TEST_IV);
    // cout << "SNC_class error code: " << snc->error.code << endl
    //     << "SNC_class error func: " << snc->error.func << endl
    //     << "SNC_class error msg: " << snc->error.message << endl;

    string content = "0123456789Abcdef0123456789abcdeF";
    snc->cfb_encrypt((wmkcByte *)content.c_str(), content.size());

    wmkcMisc_PRINT((wmkcByte *)content.c_str(), content.size(), 32, 1, 0);

    delete snc;
    return 0;
}
