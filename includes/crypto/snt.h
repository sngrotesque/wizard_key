#ifndef __SNT_H__
#define __SNT_H__

#include <snConf.h>

#define SNT_BLOCKLEN 32
#define SNT_NB 8 // 纵向长度
#define SNT_NK 4 // 横向长度

typedef snByte SNT_State[SNT_NB][SNT_NK];
typedef struct {
    snByte iv[SNT_BLOCKLEN];
    snByte key[SNT_BLOCKLEN];
} SNT_ctx;

#endif
