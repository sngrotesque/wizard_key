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

// 初始化snBase64对象
SN_PUBLIC(snError) snBase64_new SN_OPEN_API
SN_FUNC_OF((snBase64_ctx **obj));

/*
 *  instruction:
 *      SN_RELEASE_SRC                     释放掉obj->src指针
 *      SN_RELEASE_DST                     释放掉obj->dst指针
 *      SN_RELEASE_SRC | SN_RELEASE_DST    同时释放两个指针
*/
SN_PUBLIC(snError) snBase64_free SN_OPEN_API
SN_FUNC_OF((snBase64_ctx **obj));

// 求出Base64编码后的长度
SN_PUBLIC(snSize) snB64E_size SN_OPEN_API
SN_FUNC_OF((snObject *src));

// 求出Base64解码后的长度
SN_PUBLIC(snSize) snB64D_size SN_OPEN_API
SN_FUNC_OF((snObject *src));

// Base64编码
SN_PUBLIC(snError) snBase64Encode SN_OPEN_API
SN_FUNC_OF((snObject *dst, snObject *src));

// Base64解码
SN_PUBLIC(snError) snBase64Decode SN_OPEN_API
SN_FUNC_OF((snObject *dst, snObject *src));

#endif
