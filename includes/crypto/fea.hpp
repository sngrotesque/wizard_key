#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_FEA
#define WMKC_CPP_FEA
#include <memory.hpp>
#include <config/exception.hpp>

#define WMKC_FEA_NB 4
#define WMKC_FEA_NK 4
#define WMKC_FEA_NR 4

#define WMKC_FEA_BL 16 // WMKC FEA Block length

namespace wmkc {
    namespace crypto {
        enum class xcryptMode {ECB, CBC, CTR, CFB};
        class fea {
            private:
                wVoid subBytes(wByte *block);
                wVoid shiftBits(wByte *block);

                wVoid invSubBytes(wByte *block);
                wVoid invShiftBits(wByte *block);

                wVoid shiftRows(wByte *block);
                wVoid invShiftRows(wByte *block);

                wVoid xorWithIV(wByte *block, wByte *iv);
                wVoid keyExtension(wByte *key, wByte *iv);

                wVoid cipher(wByte *p, wByte *roundKey);
                wVoid invCipher(wByte *c, wByte *roundKey);

                wVoid ecb_encrypt(wByte *p);
                wVoid ecb_decrypt(wByte *c);

                wVoid cbc_encrypt(wByte *p, wSize n);
                wVoid cbc_decrypt(wByte *c, wSize n);

                wVoid ctr_xcrypt(wByte *d, wSize n);

                wVoid cfb_encrypt(wByte *p, wSize n, wU32 segmentSize);
                wVoid cfb_decrypt(wByte *c, wSize n, wU32 segmentSize);

            public:
                wByte key[WMKC_FEA_BL << 1];
                wByte iv[WMKC_FEA_BL];
                wByte nonce[WMKC_FEA_BL >> 1];
                wByte roundKey[sizeof(key) * WMKC_FEA_NR]; // len(key) * WMKC_FEA_NR
                wU32 segmentSize;

                //////////////////////////////////////////////////////////////////

                fea(const wByte *key, const wByte *iv, const wU32 segmentSize = 128);
                ~fea();
                void encrypt(wByte *content, wSize size, xcryptMode mode);
                void decrypt(wByte *content, wSize size, xcryptMode mode);
        };
    }
}

#endif
#endif
