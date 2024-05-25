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
        class Nonce_CTX {
            public:
                wByte nonce[WMKC_FEA_BL];
                wU32 size;
                Nonce_CTX(wByte *nonce, const wU32 size)
                : nonce(), size(size)
                {
                    memcpy(this->nonce, nonce, size);
                }

                Nonce_CTX(std::string nonce)
                : nonce(), size(nonce.size())
                {
                    memcpy(this->nonce, nonce.data(), size);
                }
        };

        /*
        * 此算法，使用任意加密模式的情况下，即使提供了对应模式所需的数据，也不能缺失IV。
        * 比如，使用CTR模式的情况下，即使提供了Nonce，但是也不能没有IV，因为RoundKey是经过keyExtension函数由Key和IV共同生成的。
        * 如果缺失了IV或者传入的IV实际长度不是$(WMKC_FEA_BL)，那么加密结果将变得不可控。
        * 
        * 与AES算法不同，此算法，需要IV与Key共同作用生成加/解密所需的密钥，所以，即使是使用CTR模式，也请加上IV，并确保IV的
        * 长度等于$(WMKC_FEA_BL)。
        */
        class FEA {
            private:
                wVoid subBytes(wByte *block);
                wVoid shiftBits(wByte *block);

                wVoid invSubBytes(wByte *block);
                wVoid invShiftBits(wByte *block);

                wVoid shiftRows(wByte *block);
                wVoid invShiftRows(wByte *block);

                wVoid xorWithIV(wByte *block, wByte *iv);

                wVoid cipher(wByte *p, wByte *roundKey);
                wVoid invCipher(wByte *c, wByte *roundKey);

                wVoid keyExtension(const wByte *key, const wByte *iv);

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
                wByte roundKey[sizeof(key) * WMKC_FEA_NR]; // len(key) * WMKC_FEA_NR

                Nonce_CTX nonce;
                wU32 segmentSize;

                //////////////////////////////////////////////////////////////////

                FEA(const wByte *key, const wByte *iv, Nonce_CTX nonce = {nullptr, 0}, const wU32 segmentSize = 128);
                ~FEA();
                void encrypt(wByte *content, wSize size, xcryptMode mode);
                void decrypt(wByte *content, wSize size, xcryptMode mode);
        };
    }
}

#endif
#endif
