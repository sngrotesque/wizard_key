#ifndef __SNT_H__
#define __SNT_H__

#include <snConf.h>

#define SNT_BLOCKLEN 32
#define SNT_NB 8 // 纵向长度
#define SNT_NK 4 // 横向长度

SN_PRIVATE_CONST(snByte) SNT_NR[3] = {0x07, 0x09, 0x0b};
SN_PRIVATE_CONST(snByte) SNT_KN[3] = {0x20, 0x40, 0x60};
typedef enum {
    SNT_256,
    SNT_512,
    SNT_768
} SNT_mode;

typedef snByte SNT_State[SNT_NB][SNT_NK];
typedef struct {
    sn_u16 NR;
    sn_u16 KN;
    SNT_mode mode;
    snByte iv[SNT_BLOCKLEN];
    snByte roundKey[1056];
} SNT_ctx;

#endif
