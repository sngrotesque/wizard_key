#include <crypto/snc.h>
#include <wmkc_conf.hpp>

typedef enum {
    SNC_ECB, SNC_CBC, SNC_CTR, SNC_CFB
} wmkcSNC_Xcrypt_Mode;

class wmkcSNC {
    private:
        wmkcSNC_obj *snc_ctx = wmkcNull;
        wmkcErr_obj error;
    public:
        wmkc_u32 segmentSize;
        wmkcSNC(const wmkcByte *key, const wmkcByte *iv, SNC_mode KeyMode, wmkc_u32 segmentSize = 256)
        : segmentSize(segmentSize)
        {
            if((error = wmkcSNC_new(&this->snc_ctx, KeyMode)).code ||
                (error = wmkcSNC_init(this->snc_ctx, key, iv)).code) {
                throw runtime_error(format("{}[{}]: {}", error.func, error.code, error.message));
            }
        }

        ~wmkcSNC() {
            wmkcSNC_free(&this->snc_ctx);
        }

        wmkcVoid encrypt(wmkcByte *content, wmkcSize size, wmkcSNC_Xcrypt_Mode EncryptMode) {
            switch(EncryptMode) {
                case SNC_ECB:
                    wmkcSNC_ecb_encrypt(this->snc_ctx, content, size); break;
                case SNC_CBC:
                    wmkcSNC_cbc_encrypt(this->snc_ctx, content, size); break;
                case SNC_CTR:
                    wmkcSNC_ctr_xcrypt(this->snc_ctx, content, size); break;
                case SNC_CFB:
                    wmkcSNC_cfb_encrypt(this->snc_ctx, content, size, this->segmentSize); break;
            }
        }

        wmkcVoid decrypt(wmkcByte *content, wmkcSize size, wmkcSNC_Xcrypt_Mode EncryptMode) {
            switch(EncryptMode) {
                case SNC_ECB:
                    wmkcSNC_ecb_decrypt(this->snc_ctx, content, size); break;
                case SNC_CBC:
                    wmkcSNC_cbc_decrypt(this->snc_ctx, content, size); break;
                case SNC_CTR:
                    wmkcSNC_ctr_xcrypt(this->snc_ctx, content, size); break;
                case SNC_CFB:
                    wmkcSNC_cfb_decrypt(this->snc_ctx, content, size, this->segmentSize); break;
            }
        }
};
