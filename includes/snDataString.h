#ifndef __SN_STRING__
#define __SN_STRING__

#include <snConf.h>
#include <snObject.h>

/**
 *  此函数：内容拼接（后续可以参考strncat_s函数）
 *  dst            可以不用申请内存空间，传入之后会自动申请。
 *  left, right    要拼接的两个内容
*/
WMKC_PUBLIC(snErr_ctx) snString_Splicing WMKC_OPEN_API
WMKC_OF((snObject **dst, snObject *left, snObject *right));

/**
 *  此函数：内容切片
 *  dst    可以不用申请内存空间，传入之后会自动申请。
 *  src    需要切片的内容
 *  start  切片开始处
*/
WMKC_PUBLIC(snErr_ctx) snString_Slice WMKC_OPEN_API
WMKC_OF((snObject **dst, snObject *src, wmkcSSize start, wmkcSSize end));

/**
 *  此函数：内容输入
 *  buf     缓冲区
 *  size    缓冲区长度
*/
WMKC_PUBLIC(snErr_ctx) snString_Scanf WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size));

/**
 *  此函数：内容倒转
 *  buf     缓冲区
 *  size    缓冲区长度
*/
WMKC_PUBLIC(snErr_ctx) snString_Reverse WMKC_OPEN_API
WMKC_OF((wmkcByte *data, wmkcSize size));

/**
 *  此函数：求出数字的二进制值
 *  dst     结果缓冲区，不用手动申请内存空间
 *  src     数字
*/
WMKC_PUBLIC(snErr_ctx) snString_Binary WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcSize src));

/**
 *  此函数：判断两个内容是否相同
 *  src1     第一个内容
 *  src2     第二个内容
 *  size     内容长度
 *  
 *  相同则返回true
*/
WMKC_PUBLIC(wmkcBool) snString_Compare WMKC_OPEN_API
WMKC_OF((wmkcByte *src1, wmkcByte *src2, wmkcSize size));

// 计算数字的长度
WMKC_PUBLIC(wmkc_u32) snString_numlen WMKC_OPEN_API
WMKC_OF((wmkcSize _n));

#endif // #ifndef __SN_STRING__
