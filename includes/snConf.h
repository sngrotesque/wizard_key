/**
 *  我完全放弃了一些东西：
 *      float类型，默认采用double类型。
 *      signed char类型，默认使用char类型。
 *      uint64_t类型，默认使用size_t代替，因此不会提供32位系统支持。
*/
#ifndef __SN_CONF__
#define __SN_CONF__

/** 导入必要头文件 Begin ****************/
#include <stdio.h>    // 标准输入输出库
#include <string.h>   // 标准字符串库
#include <stdint.h>   // 标准数字类型库
#include <signal.h>   // 信号库
#include <stdbool.h>  // 标准布尔值库
#include <inttypes.h> // 用于在跨平台打印同样的数据类型
/** 导入必要头文件 End ****************/

/** 版本信息 Begin ****************/
#define SHARK_COAST_VERSION     "4.6.0"
#define SHARK_COAST_VERNUM       0x460
#define SHARK_COAST_VER_MAJOR    4
#define SHARK_COAST_VER_MINOR    6
#define SHARK_COAST_VER_REVISION 0
/** 版本信息 End ****************/

/** 错误代码 Begin ****************/
#ifndef __SN_ERROR__
#define __SN_ERROR__
// General
#define snErr_OK                  0x00000000         // 一切正常
#define snErr_Err64               0xffffffffffffffff // 一般错误：64位的错误
#define snErr_Err32               0xffffffff         // 一般错误：32位的错误
#define snErr_ErrInvalid          0xfffffffe         // 一般错误：错误的参数
#define snErr_ErrNullData         0xfffffffd         // 一般错误：如果返回此值就说明不应是空数据
#define snErr_ErrType             0xfffffffc         // 一般错误：错误的类型
#define snErr_ErrOutRange         0xfffffffb         // 一般错误：错误的范围，超过规定范围
#define snErr_ErrInvalidRange     0xfffffffa         // 一般错误：错误的范围，起始下标超过末尾下标
// Network
#define snErr_NetDomainResolv     0x00000001         // 网络相关错误：域名解析失败
#define snErr_NetSocket           0x00000002         // 网络相关错误：未能使用socket
#define snErr_NetConnect          0x00000005         // 网络相关错误：连接失败
#define snErr_NetBind             0x00000006         // 网络相关错误：绑定失败
#define snErr_NetListen           0x00000007         // 网络相关错误：监听失败
#define snErr_NetAccept           0x00000008         // 网络相关错误：接收连接失败
#define snErr_NetSockfdType       0x00000009         // 网络相关错误：错误的socket类型
#define snErr_NetSend             0x0000000a         // 网络相关错误：发送数据出现错误
#define snErr_NetRecv             0x0000000b         // 网络相关错误：接收数据出现错误
#define snErr_NetClose            0x0000000c         // 网络相关错误：关闭套接字时出现错误
#define snErr_NetWsaData          0x0000000d         // 网络相关错误：清理WSADATA数据出现错误
#define snErr_NetFamily           0x0000000e         // 网络相关错误：错误的网络家族
#define snErr_NetSetSockOpt       0x0000000f         // 网络相关错误：setsockopt函数错误
// File or Path
#define snErr_FileFolderPath      0x00000200         // 文件相关错误：错误的路径
#define snErr_FileNull            0x00000201         // 文件相关错误：空文件
#define snErr_FileClose           0x00000202         // 文件相关错误：文件关闭失败
// Memory
#define snErr_Memory              0x6d656f66         // 内存相关错误：Memory failure
#define snErr_MemNMSR             0x6e6d7372         // 内存相关错误：No memory space requested
#define snErr_MemRelease          0x776d7269         // 内存相关错误：Wrong memory release instruction
#endif // #ifndef __SN_ERROR__
/** 错误代码 End ****************/

/** 其他功能 Begin ****************/
// 是否启用其他功能
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
#   endif
#endif // #if defined(SN_ENABLE_FEATURES)
/** 其他功能 End ****************/

/** 类型定义 Begin ****************/
// 如果C++禁用了register关键词，那么启用它
#if __cplusplus > 199711L
#   define register
#endif

#define SN_PRIVATE_INLINE(type) static inline type // 静态内联函数
#define SN_PRIVATE_CONST(type)  static const  type // 静态常量
#define SN_PRIVATE(type)        static type        // 静态函数（私有函数）
#define SN_PUBLIC(type)         type               // 动态函数（公共函数）
#define SN_FUNC_OF(args)        args               // 函数取消调用消耗
#define SN_OPEN_API                                // 公共函数请加上此宏用于标识

#define SN_RELEASE_NO           0x00 // 不需要释放
#define SN_RELEASE_NORMAL       0x01 // 释放掉结构体中的指针
#define SN_RELEASE_FILE         0x02 // 释放文件指针
#define SN_RELEASE_SRC          0x09 // 释放掉结构体中的输入指针
#define SN_RELEASE_DST          0x10 // 释放掉结构体中的输出指针
//      SN_RELEASE_XX           0x03 !!! 不能定义 !!!
//      SN_RELEASE_XX           0x19 !!! 不能定义 !!!

#define snNull                  NULL      // 空指针
#define snFast                  register  // 寄存器类型

typedef const char *            snString; // 固定字符串类型

typedef char                    snChar;   // 字符类型
typedef uint8_t                 snByte;   // 字节类型

typedef double                  snFloat;  // 浮点数类型

typedef int64_t                 snSSize;  // 长整数类型
typedef size_t                  snSize;   // 长度类型
typedef int16_t                 sn_16;    // 宽字节类型
typedef int32_t                 sn_32;    // 整数类型
typedef uint16_t                sn_u16;   // 无符号宽字节类型
typedef uint32_t                sn_u32;   // 无符号整数类型

typedef void                    snVoid;   // 空类型
typedef bool                    snBool;   // 布尔类型
typedef FILE                    snFile;   // 文件类型
typedef uint32_t                snError;  // 错误类型
/** 类型定义 End ****************/

/** 函数定义 Begin ****************/
// 字节交换：0x91 -> 0x19
#define snByteSwap(x) ((((x) & 0xf) << 4) ^ ((x) >> 4))
// 宽字交换：0x91ba -> 0xab19
#define snWordSwap(x) ((snByteSwap(x & 0xff) << 8) ^ snByteSwap((x & 0xff00) >> 8))
// 长字交换：0x91ba4951 -> 0x1594ab19
#define snLongSwap(x) ((snWordSwap((x & 0xffff0000) >> 16)) ^ (snWordSwap(x & 0xffff) << 16))
// 内存空间申请
#define snMemory_new(type, x, size) (x = (type)malloc((size)))
// 内存空间释放
#define snMemory_release(x) free(x); x = snNull
// 内存内容初始化为0
#define snZeroObject(x, s) memset((x), 0x00, (s))
/** 函数定义 End ****************/

#endif // #ifndef __SN_CONF__

// #define SN_TEST_DEFINE(x, y) (x # y)  // 字符串与数字拼接（不接受指针）
// #define SN_TEST_DEFINE(x, y) (x ## y) // 将数字拼接（不接受指针）
// #define SN_TEST_DEFINE(x, y) (x y)    // 将字符串拼接（不接受指针）
