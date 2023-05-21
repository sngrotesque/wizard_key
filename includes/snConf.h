/**
 *  我完全放弃了一些东西：
 *      float类型，默认采用double类型。
 *      signed char类型，默认使用char类型。
 *      uint64_t类型，默认使用size_t代替，因此不会提供32位系统支持。
 * 
 *  下一步更新为：
 *      添加snError类型的结构体，参考Python源代码实现。
 *      包含错误代码和错误信息的文本，使用宏的方式定义。
*/
#ifndef __SN_CONF__
#define __SN_CONF__

#include <stdio.h>    // 标准输入输出库
#include <string.h>   // 标准字符串库
#include <stdint.h>   // 标准数字类型库
#include <signal.h>   // 信号库
#include <stdbool.h>  // 标准布尔值库
#include <inttypes.h> // 用于在跨平台打印同样的数据类型

#if __cplusplus > 199711L
#define register
#endif

// 判断是否是64位系统及以上（如果有的话）
#if defined(__SIZEOF_SIZE_T__) && (__SIZEOF_SIZE_T__ >= 8)
#define SHARK_COAST_VERSION     "v4.8.0"
#define SHARK_COAST_VERNUM       0x480
#define SHARK_COAST_VER_MAJOR    4
#define SHARK_COAST_VER_MINOR    8
#define SHARK_COAST_VER_REVISION 0
#define SHARK_COAST_SUPPORT      true

#define SN_PRIVATE_INLINE(type) static inline type // 静态内联函数
#define SN_PRIVATE_CONST(type)  static const  type // 静态常量
#define SN_PRIVATE(type)        static type        // 静态函数（私有函数）
#define SN_PUBLIC(type)         type               // 动态函数（公共函数）
#define SN_FUNC_OF(args)        args               // 函数取消调用消耗
#define SN_OPEN_API                                // 公共函数请加上此宏用于标识

//      字节交换：    0x91 -> 0x19
#define snSwapByte(x) ((((x) & 0xf) << 4) ^ ((x) >> 4))
//      宽字交换：    0x91ba -> 0xab19
#define snSwapWord(x) ((snSwapByte((x) & 0xff) << 8) ^ snSwapByte((x) >> 8))
//      长字交换：    0x91ba4951 -> 0x1594ab19
#define snSwapLong(x) ((snSwapWord(x & 0xffff) << 16) ^ (snSwapWord(x >> 16)))
//      内存空间申请
#define snMemoryNew(type, x, size) (x = (type)malloc((size)))
//      内存空间释放
#define snMemoryFree(x) free(x); x = snNull
//      内存内容初始化为0
#define snZeroObject(x, s) memset((x), 0x00, (s))

#ifndef _SHARK_COAST_DEFINED
#define _SHARK_COAST_DEFINED
#define snNull      NULL      // 空指针
#define snFast      register  // 寄存器类型
typedef const char *snString; // 固定字符串类型
typedef char        snChar;   // 字符类型
typedef uint8_t     snByte;   // 字节类型
typedef double      snFloat;  // 浮点数类型
typedef int64_t     snSSize;  // 长整数类型
typedef size_t      snSize;   // 长度类型
typedef int16_t     sn_16;    // 宽字节类型
typedef int32_t     sn_32;    // 整数类型
typedef uint16_t    sn_u16;   // 无符号宽字节类型
typedef uint32_t    sn_u32;   // 无符号整数类型
typedef void        snVoid;   // 空类型
typedef bool        snBool;   // 布尔类型
typedef FILE        snFile;   // 文件类型
typedef size_t      snError;  // 错误代码类型（将snErr_ctx对象替换snError后删掉此类型）
#endif // #ifndef _SHARK_COAST_DEFINED

#ifndef SN_ERROR_CODE
#   define SN_ERROR_CODE
#   define snErr_OK                0ULL  // 一切正常
#   define snErr_Err64            -1ULL  // 一般：64位的错误
#   define snErr_Err32            -1U    // 一般：32位的错误
#   define snErr_ErrInvalid       -2ULL  // 一般：错误的参数
#   define snErr_ErrNullData      -3ULL  // 一般：不应是空数据
#   define snErr_ErrType          -4ULL  // 一般：错误的类型
#   define snErr_ErrOutRange      -5ULL  // 一般：超过规定范围
#   define snErr_ErrInvalidRange  -6ULL  // 一般：起始范围超过末尾范围
#   define snErr_ErrMemory        -7ULL  // 一般：内存错误（Memory failure）
#   define snErr_NetDomainResolv   1ULL  // 网络：域名解析失败
#   define snErr_NetSocket         2ULL  // 网络：未能使用socket
#   define snErr_NetConnect        5ULL  // 网络：连接失败
#   define snErr_NetBind           6ULL  // 网络：绑定失败
#   define snErr_NetListen         7ULL  // 网络：监听失败
#   define snErr_NetAccept         8ULL  // 网络：接收连接失败
#   define snErr_NetSockfdType     9ULL  // 网络：错误的socket类型
#   define snErr_NetSend           10ULL // 网络：发送数据出现错误
#   define snErr_NetRecv           11ULL // 网络：接收数据出现错误
#   define snErr_NetClose          12ULL // 网络：关闭套接字时出现错误
#   define snErr_NetWsaData        13ULL // 网络：清理WSADATA数据出现错误
#   define snErr_NetFamily         14ULL // 网络：错误的网络家族
#   define snErr_NetSetSockOpt     15ULL // 网络：setsockopt函数错误
#   define snErr_FileFolderPath    32ULL // 文件：错误的路径
#   define snErr_FileNull          33ULL // 文件：空文件
#   define snErr_FileClose         34ULL // 文件：文件关闭失败
//  初始化snErr对象
#   define snErr_new(ctx) \
    if(!snMemoryNew(snErr_ctx *, ctx, sizeof(snErr_ctx))) \
        return snNull

//  赋值snErr对象并Return
#   define snErr_return(ctx, _code, _message) \
    ctx->message = _message; ctx->code = _code; \
    return ctx

//  释放snErr对象
#   define snErr_free(ctx) snMemoryFree(ctx)

typedef struct {
    snString message;
    snError code;
} snErr_ctx;
#endif // #ifndef SN_ERROR_CODE

#define SN_ENABLE_FEATURES
#if defined(SN_ENABLE_FEATURES)
#   define __SN_MEMORY
#   define __SN_ISO646
#   define __SN_LOCALE
#   define __SN_COLOR
#   if defined(__SN_MEMORY)
#       include <stdlib.h>
#       if !defined(__linux)
#           include <intrin.h>
#       endif
#       include <malloc.h>
#   endif
#   if defined(__SN_LOCALE)
#       include <locale.h>
#   endif
#   if defined(__SN_ISO646)
#       include <iso646.h>
#   endif
#   if defined(__SN_COLOR)
#       define SN_CA_RESET        "\x1b[0m"  // 重置所有颜色
#       define SN_CF_BLACK        "\x1b[30m" // 黑色（前景）
#       define SN_CF_RED          "\x1b[31m" // 红色（前景）
#       define SN_CF_GREEN        "\x1b[32m" // 绿色（前景）
#       define SN_CF_YELLOW       "\x1b[33m" // 黄色（前景）
#       define SN_CF_BLUE         "\x1b[34m" // 蓝色（前景）
#       define SN_CF_MAGENTA      "\x1b[35m" // 紫色（前景）
#       define SN_CF_CYAN         "\x1b[36m" // 青色（前景）
#       define SN_CF_WHITE        "\x1b[37m" // 白色（前景）
#       define SN_CF_LIGHTBLACK   "\x1b[90m" // 亮黑色（前景）
#       define SN_CF_LIGHTRED     "\x1b[91m" // 亮红色（前景）
#       define SN_CF_LIGHTGREEN   "\x1b[92m" // 亮绿色（前景）
#       define SN_CF_LIGHTYELLOW  "\x1b[93m" // 亮黄色（前景）
#       define SN_CF_LIGHTBLUE    "\x1b[94m" // 亮蓝色（前景）
#       define SN_CF_LIGHTMAGENTA "\x1b[95m" // 亮紫色（前景）
#       define SN_CF_LIGHTCYAN    "\x1b[96m" // 亮青色（前景）
#       define SN_CF_LIGHTWHITE   "\x1b[97m" // 亮白色（前景）
#       define SN_CB_RED          "\x1b[41m" // 红色（背景）
#       define SN_CB_GREEN        "\x1b[42m" // 绿色（背景）
#       define SN_CB_YELLOW       "\x1b[43m" // 黄色（背景）
#       define SN_CB_BLUE         "\x1b[44m" // 蓝色（背景）
#       define SN_CB_MAGENTA      "\x1b[45m" // 紫色（背景）
#       define SN_CB_CYAN         "\x1b[46m" // 青色（背景）
#       define SN_CB_WHITE        "\x1b[47m" // 白色（背景）
#       define SN_CB_LIGHTBLACK   "\x1b[100m" // 亮黑色（背景）
#       define SN_CB_LIGHTRED     "\x1b[101m" // 亮红色（背景）
#       define SN_CB_LIGHTGREEN   "\x1b[102m" // 亮绿色（背景）
#       define SN_CB_LIGHTYELLOW  "\x1b[103m" // 亮黄色（背景）
#       define SN_CB_LIGHTBLUE    "\x1b[104m" // 亮蓝色（背景）
#       define SN_CB_LIGHTMAGENTA "\x1b[105m" // 亮紫色（背景）
#       define SN_CB_LIGHTCYAN    "\x1b[106m" // 亮青色（背景）
#       define SN_CB_LIGHTWHITE   "\x1b[107m" // 亮白色（背景）
#   endif // #if defined(__SN_COLOR)
#endif // #if defined(SN_ENABLE_FEATURES)

#else // #if defined(__SIZEOF_SIZE_T__) && (__SIZEOF_SIZE_T__ >= 8)
#define SHARK_COAST_SUPPORT      false
#endif // #if defined(__SIZEOF_SIZE_T__) && (__SIZEOF_SIZE_T__ >= 8)

#endif // #ifndef __SN_CONF__


// #define SN_TEST_DEFINE(x, y) (x # y)  // 拼接（不接受指针）
// #define SN_TEST_DEFINE(x, y) (x ## y) // 拼接（不接受指针）
// #define SN_TEST_DEFINE(x, y) (x y)    // 将字符串拼接（不接受指针）
