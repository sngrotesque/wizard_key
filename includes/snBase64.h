#ifndef __BASE64_H__
#define __BASE64_H__
#include <snConf.h>
#include <snObject.h>

#ifndef BASE64_PAD
#define BASE64_PAD '='
#endif

typedef struct {
    snObject *src;
    snObject *dst;
} snBase64_ctx;

// 求出Base64编码后的长度
WMKC_PUBLIC(wmkcSize) snBase64_encode_size WMKC_OPEN_API
WMKC_OF((snObject *src));

// 求出Base64解码后的长度
WMKC_PUBLIC(wmkcSize) snBase64_decode_size WMKC_OPEN_API
WMKC_OF((snObject *src));

// 初始化snBase64对象
WMKC_PUBLIC(snErr_ctx) snBase64_new WMKC_OPEN_API
WMKC_OF((snBase64_ctx **obj));

// 释放snBase64对象
WMKC_PUBLIC(snErr_ctx) snBase64_free WMKC_OPEN_API
WMKC_OF((snBase64_ctx **obj));

// Base64编码
WMKC_PUBLIC(snErr_ctx) snBase64_Encode WMKC_OPEN_API
WMKC_OF((snObject *dst, snObject *src));

// Base64解码
WMKC_PUBLIC(snErr_ctx) snBase64_Decode WMKC_OPEN_API
WMKC_OF((snObject *dst, snObject *src));

#endif
