#ifndef WMKC_KEY
#define WMKC_KEY

#include <wmkc_conf.h>
#include <wmkc_file.h>
#include <wmkc_random.h>
#include <wmkc_base.h>

#define WMKC_KEY_ROUND 13
#define WMKC_KEY_GROUP 64
#define WMKC_KEY_MODE_ENCRYPT true
#define WMKC_KEY_MODE_DECRYPT false
#define WMKC_KEY_END_PK "-----END SN PUBLIC KEY-----"
#define WMKC_KEY_END_SK "-----END SN PRIVATE KEY-----"
#define WMKC_KEY_BEGIN_PK "-----BEGIN SN PUBLIC KEY-----"
#define WMKC_KEY_BEGIN_SK "-----BEGIN SN PRIVATE KEY-----"

typedef wmkcByte wmkcKey_state[2][2];
typedef struct {
    wmkcByte *key; // 密钥指针
    wmkcByte size; // 密钥长度
    wmkcBool mode; // true: Encrypt, false: Decrypt
} wmkcKey_ctx;

// 初始化wmkcKey对象
WMKC_PUBLIC(wmkcErr_ctx) wmkcKey_new WMKC_OPEN_API
WMKC_OF((wmkcKey_ctx **obj, wmkcByte *key, wmkcSize keySize, wmkcBool mode));

// 释放wmkcKey对象
WMKC_PUBLIC(wmkcErr_ctx) wmkcKey_free WMKC_OPEN_API
WMKC_OF((wmkcKey_ctx **obj));

// 密钥加密函数（只能处理4的倍数长度的密钥）
WMKC_PUBLIC(wmkcErr_ctx) wmkcKey_CryptKey WMKC_OPEN_API
WMKC_OF((wmkcKey_ctx *obj));

// 导入密钥
WMKC_PUBLIC(wmkcErr_ctx) wmkcKey_loadKey WMKC_OPEN_API
WMKC_OF((wmkcKey_ctx *obj, wmkcString fn));

// 保存密钥
WMKC_PUBLIC(wmkcErr_ctx) wmkcKey_saveKey WMKC_OPEN_API
WMKC_OF((wmkcKey_ctx *obj, wmkcString fn));

#endif
