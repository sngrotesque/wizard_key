#ifndef __SN_KEY__
#define __SN_KEY__

#include <snConf.h>
#include <wmkcFile.h>
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

typedef wmkcByte snKey_state[2][2];
typedef struct {
    wmkcByte *key; // 密钥指针
    wmkcByte size; // 密钥长度
    wmkcBool mode;
    // true: Encrypt
    // false: Decrypt
} snKey_ctx;

// 初始化snKey对象
WMKC_PUBLIC(snErr_ctx) snKey_new WMKC_OPEN_API
WMKC_OF((snKey_ctx **obj, wmkcByte *key, wmkcSize keySize, wmkcBool mode));

// 释放snKey对象
WMKC_PUBLIC(snErr_ctx) snKey_free WMKC_OPEN_API
WMKC_OF((snKey_ctx **obj));

// 密钥加密函数（只能处理4的倍数长度的密钥）
WMKC_PUBLIC(snErr_ctx) snKey_CryptKey WMKC_OPEN_API
WMKC_OF((snKey_ctx *obj));

// 导入密钥
WMKC_PUBLIC(snErr_ctx) snKey_loadKey WMKC_OPEN_API
WMKC_OF((snKey_ctx *obj, wmkcString fn));

// 保存密钥
WMKC_PUBLIC(snErr_ctx) snKey_saveKey WMKC_OPEN_API
WMKC_OF((snKey_ctx *obj, wmkcString fn));
#endif