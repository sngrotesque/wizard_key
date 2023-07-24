/*
* 警告！！！（Warning!!!!!）
* 这是一个过时的版本，请不要使用它。
*/
#ifndef __SNC_H__
#define __SNC_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <wmkc_conf.h>

#define SNC_BLOCKLEN 32 // 区块长度（Block size）
#define SNC_NB 8 // 纵向长度（Longitudinal length）
#define SNC_NK 4 // 横向长度（Transverse length）


//* 加解密轮数（Encryption or Decryption number of rounds.）
WMKC_PRIVATE_CONST(wmkcByte) SNC_NR[3] = {
    0x9, //  9 round
    0xb, // 11 round
    0xd  // 13 round
};

//* 密钥长度（Key Length.）
WMKC_PRIVATE_CONST(wmkcByte) SNC_KN[3] = {
    0x20, // 32 bytes
    0x40, // 64 bytes
    0x60  // 96 bytes
};

typedef enum {
    SNC_256, // 密钥长度：32字节（Key length: 32 bytes）
    SNC_512, // 密钥长度：64字节（Key length: 64 bytes）
    SNC_768  // 密钥长度：96字节（Key length: 96 bytes）
} SNC_mode;

typedef wmkcByte sncState_t[SNC_NB][SNC_NK]; // 声明区块类型（declare block type）
typedef struct {
    wmkc_u32 NR;
    wmkc_u32 KN;
    SNC_mode mode;
    wmkcByte **rk;
    wmkcByte iv[SNC_BLOCKLEN];
} SNC_ctx; // 声明SNC算法的数据结构（declare SNC structure）

WMKC_PUBLIC(wmkc_u32)  SNC_malloc_init WMKC_OF((SNC_ctx **ctx, SNC_mode mode));
WMKC_PUBLIC(wmkc_u32)  SNC_release     WMKC_OF((SNC_ctx **ctx));
WMKC_PUBLIC(wmkcVoid)   SNC_init_ctx    WMKC_OF((SNC_ctx *ctx, wmkcByte *keyBuf, wmkcByte *ivBuf));
WMKC_PUBLIC(wmkcVoid)   SNC_ECB_Encrypt WMKC_OF((SNC_ctx *ctx, wmkcByte *buf, wmkcSize size));
WMKC_PUBLIC(wmkcVoid)   SNC_ECB_Decrypt WMKC_OF((SNC_ctx *ctx, wmkcByte *buf, wmkcSize size));
WMKC_PUBLIC(wmkcVoid)   SNC_CBC_Encrypt WMKC_OF((SNC_ctx *ctx, wmkcByte *buf, wmkcSize size));
WMKC_PUBLIC(wmkcVoid)   SNC_CBC_Decrypt WMKC_OF((SNC_ctx *ctx, wmkcByte *buf, wmkcSize size));

#ifdef __cplusplus
}
#endif

#endif