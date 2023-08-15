#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_OBJECT
#define WMKC_OBJECT
#include <wmkc_error.h>
#include <wmkc_memory.h>

#ifdef __cplusplus
extern "C" {
#endif

// wmkc对象（后续可能还需要添加一些成员）
typedef struct {
    wmkcByte *buf; // 缓冲区数据
    wmkcSize size; // 缓冲区长度
} wmkc_obj;

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_new WMKC_OPEN_API
WMKC_OF((wmkc_obj **obj));

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_free WMKC_OPEN_API
WMKC_OF((wmkc_obj **obj));

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_append WMKC_OPEN_API
WMKC_OF((wmkc_obj *obj, wmkcCSTR content));

#ifdef __cplusplus
}
#endif

#endif /* WMKC_OBJECT */
#endif /* WMKC_SUPPORT */
