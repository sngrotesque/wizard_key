#ifndef __SN_KEY__
#define __SN_KEY__

#include <snConf.h>
#include <snFile.h>
#include <snRand.h>
#include <snBase64.h>

#define SN_KEY_ROUND 13
#define SN_KEY_GROUP 64
#define SN_KEY_MODE_ENCRYPT true
#define SN_KEY_MODE_DECRYPT false
#define SN_KEY_END_PK "-----END SN PUBLIC KEY-----"
#define SN_KEY_END_SK "-----END SN PRIVATE KEY-----"
#define SN_KEY_BEGIN_PK "-----BEGIN SN PUBLIC KEY-----"
#define SN_KEY_BEGIN_SK "-----BEGIN SN PRIVATE KEY-----"

typedef snByte snKey_state[2][2];
typedef struct {
    snByte *key; // 密钥指针
    snByte size; // 密钥长度
    snBool mode;
    // true: Encrypt
    // false: Decrypt
} snKey_ctx;

// 初始化snKey对象
SN_PUBLIC(snError) snKey_new SN_OPEN_API
SN_FUNC_OF((snKey_ctx **obj, snByte *key, snSize keySize, snBool mode));

// 释放snKey对象
SN_PUBLIC(snError) snKey_release SN_OPEN_API
SN_FUNC_OF((snKey_ctx **obj, sn_u32 instruction));

// 密钥加密函数（只能处理4的倍数长度的密钥）
SN_PUBLIC(snVoid) snKey_CryptKey SN_OPEN_API
SN_FUNC_OF((snKey_ctx *obj));

// 导入密钥
SN_PUBLIC(snError) snKey_loadKey SN_OPEN_API
SN_FUNC_OF((snKey_ctx *obj, const snChar *fn));

// 保存密钥
SN_PUBLIC(snError) snKey_saveKey SN_OPEN_API
SN_FUNC_OF((snKey_ctx *obj, const snChar *fn));
#endif