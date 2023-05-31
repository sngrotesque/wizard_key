#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_BASE
#define WMKC_BASE
#include <wmkc_object.h>

// Base64的填充值
#define WMKC_BASE64_PAD '='

// wmkcBase64对象
typedef struct {
    wmkc_obj     *src;
    wmkc_obj     *dst;
    wmkcErr_obj (*encode)(wmkc_obj *dst, wmkc_obj *src);
    wmkcErr_obj (*decode)(wmkc_obj *dst, wmkc_obj *src);
    wmkcSize    (*encodeSize)(wmkc_obj *src);
    wmkcSize    (*decodeSize)(wmkc_obj *src);
} wmkcBase64_obj;

WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_new WMKC_OPEN_API
WMKC_OF((wmkcBase64_obj **obj));

WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_free WMKC_OPEN_API
WMKC_OF((wmkcBase64_obj **obj));

#endif /* WMKC_BASE */
#endif /* WMKC_SUPPORT */
