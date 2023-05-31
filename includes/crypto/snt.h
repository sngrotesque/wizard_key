/**
 * 这只是一个用来测试SNC加密算法的测试算法，不会提供解密函数。
 * 并且也请不要将它在任何实际项目中进行使用，此算法的安全性并没有得到验证。
 * 并且这个算法的效率低下。
*/
#ifndef WMKC_SNT
#define WMKC_SNT

#include <wmkc_conf.h>

#define SNT_BLOCKLEN 32
#define SNT_NB 8 // 纵向长度
#define SNT_NK 4 // 横向长度

WMKC_PRIVATE_CONST(wmkcByte) SNT_NR[3] = {0x07, 0x09, 0x0b};
WMKC_PRIVATE_CONST(wmkcByte) SNT_KN[3] = {0x20, 0x40, 0x60};
typedef enum {
    SNT_256,
    SNT_512,
    SNT_768
} SNT_mode;

typedef wmkcByte SNT_State[SNT_NB][SNT_NK];
typedef struct {
    wmkc_u16 NR;
    wmkc_u16 KN;
    SNT_mode mode;
    wmkcByte iv[SNT_BLOCKLEN];
    wmkcByte roundKey[1056];
} SNT_ctx;

#endif
