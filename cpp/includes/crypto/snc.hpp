#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_SNC
#define WMKC_CPP_SNC
#include <wmkc_exception.hpp>
#include <crypto/snc.h>

typedef enum {
    ECB, CBC, CTR, CFB
} SNC_Xcrypt_Mode;

class wmkcSNC {
    private:
        wmkcSNC_obj *snc;
        wmkcErr_obj error;
        SNC_Xcrypt_Mode xcryptoMode;

    public:
        wmkc_u32 segmentSize;
        wmkcSNC(const wmkcByte *key, const wmkcByte *iv, SNC_mode mode, wmkc_u32 segment_size = 256,
            SNC_Xcrypt_Mode xcryptMode = SNC_Xcrypt_Mode::ECB)
        : snc(), error(), xcryptoMode(xcryptMode), segmentSize(segment_size)
        {
            if((this->error = wmkcSNC_new(&this->snc, mode)).code) {
                wmkcErr_exception(this->error);
            }
            if((this->error = wmkcSNC_init(this->snc, key, iv)).code) {
                wmkcErr_exception(this->error);
            }
        }

        ~wmkcSNC()
        {
            wmkcSNC_free(&this->snc);
        }

        void encrypt(wmkcByte *content, wmkcSize length);
        void decrypt(wmkcByte *content, wmkcSize length);
};

#endif /* WMKC_CPP_SNC */
#endif /* WMKC_SUPPORT */
