#ifndef WMKC_OBJECT
#define WMKC_OBJECT

#include <wmkc_conf.h>

typedef struct {
    wmkcByte *buf;
    wmkcSize size;
    wmkcSize memSize;
} wmkcObject;

// 初始化wmkcObject对象
WMKC_PUBLIC(wmkcErr_obj) wmkcObject_new WMKC_OPEN_API
WMKC_OF((wmkcObject **obj, wmkcSize _memory_size, wmkcBool _Clear_memory));

// 释放wmkcObject对象
WMKC_PUBLIC(wmkcErr_obj) wmkcObject_free WMKC_OPEN_API
WMKC_OF((wmkcObject **obj));

#endif
