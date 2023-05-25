#ifndef __SN_OBJECT__
#define __SN_OBJECT__

#include <snConf.h>

typedef struct {
    wmkcByte *buf;
    wmkcSize size;
    wmkcSize memSize;
} snObject;

// 初始化snObject对象
WMKC_PUBLIC(snErr_ctx) snObject_new WMKC_OPEN_API
WMKC_OF((snObject **obj, wmkcSize _memory_size, wmkcBool _Clear_memory));

// 释放snObject对象
WMKC_PUBLIC(snErr_ctx) snObject_free WMKC_OPEN_API
WMKC_OF((snObject **obj));

#endif // #ifndef __SN_OBJECT__
