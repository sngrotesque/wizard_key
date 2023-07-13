/*
* ECB is considered insecure for most uses.
*
* CBC and ECB modes need block padding, otherwise they cannot be used.
* Suggest https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7 for padding scheme
* NOTES: you need to set IV in ctx via AES_init_ctx_iv() or AES_ctx_set_iv()
*        no IV should ever be reused with the same key
*
* This is an implementation of the AES algorithm, specifically ECB, CTR and CBC mode.
* Block size can be chosen in aes.h - available choices are AES128, AES192, AES256.
*
* The implementation is verified against the test vectors in:
*     National Institute of Standards and Technology Special Publication 800-38A 2001 ED
*
* ECB-AES128
* ----------
*
* plain-text:
*     6bc1bee22e409f96e93d7e117393172a
*     ae2d8a571e03ac9c9eb76fac45af8e51
*     30c81c46a35ce411e5fbc1191a0a52ef
*     f69f2445df4f9b17ad2b417be66c3710
* key:
*     2b7e151628aed2a6abf7158809cf4f3c
* resulting cipher
*     3ad77bb40d7a3660a89ecaf32466ef97
*     f5d3d58503b9699de785895a96fdbaaf
*     43b1cd7f598ece23881b00e3ed030688
*     7b0c785e27e8ad3f8223207104725dd4
*
* NOTE:
*     String length must be evenly divisible by 16byte (str_len % 16 == 0)
*     You should pad the end of the string with zeros if this is not the case.
*     For AES192/256 the key size is proportionally larger.
*/

#ifndef _AES_H_
#define _AES_H_

#include <string.h>

typedef unsigned char uint8_t;

#ifndef CBC
#    define CBC 1
#endif

#ifndef ECB
#    define ECB 1
#endif

#ifndef CTR
#    define CTR 1
#endif

//#define AES128 1
//#define AES192 1
#define AES256 1

#define AES_BLOCKLEN 16

#if defined(AES256) && (AES256 == 1)
#    define AES_KEYLEN 32
#    define AES_keyExpSize 240
#elif defined(AES192) && (AES192 == 1)
#    define AES_KEYLEN 24
#    define AES_keyExpSize 208
#else
#    define AES_KEYLEN 16
#    define AES_keyExpSize 176
#endif

typedef struct {
    uint8_t RoundKey[AES_keyExpSize];
#   if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))
    uint8_t Iv[AES_BLOCKLEN];
#   endif
} AES_ctx;

void AES_init_ctx(AES_ctx *ctx, const uint8_t *key);

#if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))
void AES_init_ctx_iv(AES_ctx *ctx, const uint8_t *key, const uint8_t *iv);
void AES_ctx_set_iv(AES_ctx *ctx, const uint8_t *iv);
#endif // #if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))

#if defined(ECB) && (ECB == 1)
void AES_ECB_encrypt(const AES_ctx *ctx, uint8_t *buf);
void AES_ECB_decrypt(const AES_ctx *ctx, uint8_t *buf);
#endif  // #if defined(ECB) && (ECB == !)

#if defined(CBC) && (CBC == 1)
void AES_CBC_encrypt(AES_ctx *ctx, uint8_t *buf, size_t length);
void AES_CBC_decrypt(AES_ctx *ctx, uint8_t *buf, size_t length);
#endif  // #if defined(CBC) && (CBC == 1)

#if defined(CTR) && (CTR == 1)
void AES_CTR_xcrypt(AES_ctx *ctx, uint8_t *buf, size_t length);
#endif  // #if defined(CTR) && (CTR == 1)

#endif  // _AES_H_
