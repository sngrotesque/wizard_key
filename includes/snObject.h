#ifndef __SN_OBJECT__
#define __SN_OBJECT__

#include <snConf.h>

typedef struct {
    snByte *buf;
    snSize size;
    snSize memSize;
} snObject;

// 初始化snObject对象
SN_PUBLIC(snError) snObject_new SN_OPEN_API
SN_FUNC_OF((snObject **obj, snSize _memory_size, snBool _Clear_memory));

// 释放snObject对象
SN_PUBLIC(snError) snObject_free SN_OPEN_API
SN_FUNC_OF((snObject **obj));

#endif // #ifndef __SN_OBJECT__
