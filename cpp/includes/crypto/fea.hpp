#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_FEA
#define WMKC_CPP_FEA

#define WMKC_FEA_NB 4
#define WMKC_FEA_NK 4
#define WMKC_FEA_BLOCKLEN 16

namespace wmkcCrypto {
    class wmkcFEA {
        private:
            wmkcByte key[64];
            wmkcByte nonce[16];
            wmkcByte iv[16];

        public:
            // private BEGIN
            wmkcByte subBytes(wmkcByte x);

            wmkcByte invSubBytes(wmkcByte x);
            // private END

            wmkcFEA();
            ~wmkcFEA();
            void encrypt(wmkcByte *content, wmkcSize size);
            void decrypt(wmkcByte *content, wmkcSize size);
    };
}

#endif
#endif
