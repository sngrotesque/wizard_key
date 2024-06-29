#include <config/wuk.hpp>

#if WUK_SUPPORT
#ifndef WUK_CPP_FEA
#define WUK_CPP_FEA
#include <memory.hpp>
#include <config/exception.hpp>

#include <crypto/utils.hpp>

#define WUK_FEA_NB 4
#define WUK_FEA_NK 4
#define WUK_FEA_NR 4

#define WUK_FEA_BL 16 // WUK FEA Block length

namespace wuk {
    namespace crypto {
        enum class xcryptMode {
            ECB,  // Electronic Codebook
            CBC,  // Cipher block chaining
            CFB,  // Cipher feedback
            CTR}; // Counter

        /*
        * 此算法，使用任意加密模式的情况下，即使提供了对应模式所需的数据，也不能缺失IV。
        * 比如，使用CTR模式的情况下，即使提供了Nonce，但是也不能没有IV，因为RoundKey是经过keyExtension函数由Key和IV共同生成的。
        * 如果缺失了IV或者传入的IV实际长度不是$(WUK_FEA_BL)，那么加密结果将变得不可控。
        * 
        * 与AES算法不同，此算法，需要IV与Key共同作用生成加/解密所需的密钥，所以，即使是使用CTR模式，也请加上IV，并确保IV的
        * 长度等于$(WUK_FEA_BL)。
        * 
        * 当然，你在使用CTR模式的情况下，自然也可以不加入Nonce，同样可以正常加/解密，但是吧，密文的安全性，由你自己保证。
        */
        class LIBWUK_API FEA {
            private:
                wuk::crypto::Counter counter;
                wU32 segmentSize;

                void sub_bytes(wByte *block);
                void shift_bits(wByte *block);

                void inv_sub_bytes(wByte *block);
                void inv_shift_bits(wByte *block);

                void shift_rows(wByte *block);
                void inv_shift_rows(wByte *block);

                void xor_with_iv(wByte *block, wByte *iv);

                void cipher(wByte *p, wByte *roundKey);
                void inv_cipher(wByte *c, wByte *roundKey);

                void key_extension(const wByte *key, const wByte *iv);

                void ecb_encrypt(wByte *p);
                void ecb_decrypt(wByte *c);

                void cbc_encrypt(wByte *p, wSize n);
                void cbc_decrypt(wByte *c, wSize n);

                void ctr_xcrypt(wByte *d, wSize n);

                void cfb_encrypt(wByte *p, wSize n, wU32 segmentSize);
                void cfb_decrypt(wByte *c, wSize n, wU32 segmentSize);

            public:
                wByte key[WUK_FEA_BL << 1];
                wByte iv[WUK_FEA_BL];
                wByte roundKey[sizeof(key) * WUK_FEA_NR];

                FEA() {}
                // 不是哥们？为什么不为counter添加一个参数默认值？
                FEA(const wByte *key, const wByte *iv, wuk::crypto::Counter counter = {}, const wU32 segmentSize = 128);
                ~FEA();
                void encrypt(wByte *content, wSize size, xcryptMode mode);
                void decrypt(wByte *content, wSize size, xcryptMode mode);
        };
    }
}

#endif
#endif
