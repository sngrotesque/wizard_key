#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_BASE
#define WMKC_BASE
#include <wmkc_object.h>

// Base64的填充值
#define WMKC_BASE64_PAD '='

// wmkcBase64对象
typedef struct {
    wmkcObject *src;
    wmkcObject *dst;
} wmkcBase64_obj;

// 求出Base64编码后的长度
WMKC_PUBLIC(wmkcSize) wmkcBase64_encode_size WMKC_OPEN_API
WMKC_OF((wmkcObject *src));

// 求出Base64解码后的长度
WMKC_PUBLIC(wmkcSize) wmkcBase64_decode_size WMKC_OPEN_API
WMKC_OF((wmkcObject *src));

// 初始化wmkcBase64对象
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_new WMKC_OPEN_API
WMKC_OF((wmkcBase64_obj **obj));

// 释放wmkcBase64对象
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_free WMKC_OPEN_API
WMKC_OF((wmkcBase64_obj **obj));

// Base64编码
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_Encode WMKC_OPEN_API
WMKC_OF((wmkcObject *dst, wmkcObject *src));

// Base64解码
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_Decode WMKC_OPEN_API
WMKC_OF((wmkcObject *dst, wmkcObject *src));

#endif /* WMKC_BASE */
#endif /* WMKC_SUPPORT */
