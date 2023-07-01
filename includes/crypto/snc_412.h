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

#define SNC_BLOCKLEN 32
#define SNC_NB 8
#define SNC_NK 4

WMKC_PRIVATE_CONST(wmkcByte) SNC_NR[3] = {
    0x9,
    0xb,
    0xd 
};

WMKC_PRIVATE_CONST(wmkcByte) SNC_KN[3] = {
    0x20,
    0x40,
    0x60 
};

typedef enum {
    SNC_256,
    SNC_512,
    SNC_768 
} SNC_mode;

typedef wmkcByte sncState_t[SNC_NB][SNC_NK];
typedef struct {
    wmkc_u16 NR;
    wmkc_u16 KN;
    SNC_mode mode;
    wmkcByte **rk;
    wmkcByte iv[SNC_BLOCKLEN];
} SNC_ctx;

WMKC_PUBLIC(wmkcVoid) WMKC_OPEN_API SNC_malloc_init WMKC_OF((SNC_ctx **ctx, SNC_mode mode));
WMKC_PUBLIC(wmkcVoid) WMKC_OPEN_API SNC_release     WMKC_OF((SNC_ctx **ctx));
WMKC_PUBLIC(wmkcVoid)  WMKC_OPEN_API SNC_init_ctx    WMKC_OF((SNC_ctx *ctx, wmkcByte *keyBuf, wmkcByte *ivBuf));
WMKC_PUBLIC(wmkcVoid)  WMKC_OPEN_API SNC_ECB_Encrypt WMKC_OF((SNC_ctx *ctx, wmkcByte *buf, wmkcSize size));
WMKC_PUBLIC(wmkcVoid)  WMKC_OPEN_API SNC_ECB_Decrypt WMKC_OF((SNC_ctx *ctx, wmkcByte *buf, wmkcSize size));
WMKC_PUBLIC(wmkcVoid)  WMKC_OPEN_API SNC_CBC_Encrypt WMKC_OF((SNC_ctx *ctx, wmkcByte *buf, wmkcSize size));
WMKC_PUBLIC(wmkcVoid)  WMKC_OPEN_API SNC_CBC_Decrypt WMKC_OF((SNC_ctx *ctx, wmkcByte *buf, wmkcSize size));

#ifdef __cplusplus
}
#endif

#endif