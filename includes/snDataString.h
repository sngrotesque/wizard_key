#ifndef __SN_STRING__
#define __SN_STRING__

#include <snConf.h>
#include <snObject.h>

/**
 *  此函数：内容拼接（后续可以参考strncat_s函数）
 *  dst            可以不用申请内存空间，传入之后会自动申请。
 *  left, right    要拼接的两个内容
*/
SN_PUBLIC(snErr_ctx) snString_Splicing SN_OPEN_API
SN_FUNC_OF((snObject **dst, snObject *left, snObject *right));

/**
 *  此函数：内容切片
 *  dst    可以不用申请内存空间，传入之后会自动申请。
 *  src    需要切片的内容
 *  start  切片开始处
*/
SN_PUBLIC(snErr_ctx) snString_Slice SN_OPEN_API
SN_FUNC_OF((snObject **dst, snObject *src, snSSize start, snSSize end));

/**
 *  此函数：内容输入
 *  buf     缓冲区
 *  size    缓冲区长度
*/
SN_PUBLIC(snErr_ctx) snString_Scanf SN_OPEN_API
SN_FUNC_OF((snByte *buf, snSize size));

/**
 *  此函数：内容倒转
 *  buf     缓冲区
 *  size    缓冲区长度
*/
SN_PUBLIC(snErr_ctx) snString_Reverse SN_OPEN_API
SN_FUNC_OF((snByte *data, snSize size));

/**
 *  此函数：求出数字的二进制值
 *  dst     结果缓冲区，不用手动申请内存空间
 *  src     数字
*/
SN_PUBLIC(snErr_ctx) snString_Binary SN_OPEN_API
SN_FUNC_OF((snByte **dst, snSize src));

/**
 *  此函数：判断两个内容是否相同
 *  src1     第一个内容
 *  src2     第二个内容
 *  size     内容长度
 *  
 *  相同则返回true
*/
SN_PUBLIC(snBool) snString_Compare SN_OPEN_API
SN_FUNC_OF((snByte *src1, snByte *src2, snSize size));

// 计算数字的长度
SN_PUBLIC(sn_u32) snString_numlen SN_OPEN_API
SN_FUNC_OF((snSize _n));

#endif // #ifndef __SN_STRING__
