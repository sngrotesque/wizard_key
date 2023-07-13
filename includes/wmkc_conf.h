#ifndef WMKC
#define WMKC // Wizard Magic Key Cyber（网络巫师的魔法钥匙）

#include <stdio.h>    // 标准输入输出库
#include <locale.h>   // 时区库
#include <string.h>   // 标准字符串库
#include <stdlib.h>   // 标准库
#include <stdint.h>   // 标准数字类型库
#include <signal.h>   // 信号库
#include <stdbool.h>  // 标准布尔值库
#include <inttypes.h> // 用于在跨平台打印同样的数据类型

#if __cplusplus > 199711L
#define register
#endif

// 判断是否是64位系统，不是的话，不进行支持。
#if defined(_WIN64) || (__SIZEOF_SIZE_T__ == 8) || defined(__x86_64__) || defined(__LP64__)
//  如果是Linux或Windows操作系统，那么支持。否则不支持。
#if defined(__linux) || defined(__linux__)
#   define WMKC_PLATFORM_LINUX
#   define WMKC_SUPPORT true
#elif defined(_WIN32) || defined(_WIN64)
#   define WMKC_PLATFORM_WINOS
#   define WMKC_SUPPORT true
#else
#   define WMKC_SUPPORT false
#endif // #if defined(__linux) || defined(__linux__)

// 定义版本号
#define WMKC_VERSION     "v6.3.0-canary"
#define WMKC_VER_HEX      0x630
#define WMKC_VER_MAJOR    6
#define WMKC_VER_MINOR    3
#define WMKC_VER_REVISION 0

// 定义一些一般类型
#ifndef WMKC_TYPE_DEFINED
#define WMKC_TYPE_DEFINED
#define WMKC_OPEN_API             // 公共函数请加上此宏用于标识
#define WMKC_PRIVATE_INLINE(type) static inline type // 静态内联函数
#define WMKC_PRIVATE_CONST(type)  static const  type // 静态常量
#define WMKC_PRIVATE(type)        static type // 静态函数（私有函数）
#define WMKC_PUBLIC(type)         type        // 动态函数（公共函数）
#define WMKC_OF(args)             args        // 函数取消调用消耗

#define wmkcNull                  NULL        // 空指针
#define wmkcFast                  register    // 寄存器类型
typedef char                      wmkcChar;   // 字符类型
typedef uint8_t                   wmkcByte;   // 字节类型
typedef int64_t                   wmkcSSize;  // 长整数类型
typedef size_t                    wmkcSize;   // 长度类型
typedef void                      wmkcVoid;   // 空类型
typedef bool                      wmkcBool;   // 布尔类型
typedef int16_t                   wmkc_s16;   // 宽字节类型
typedef int32_t                   wmkc_s32;   // 整数类型
typedef uint16_t                  wmkc_u16;   // 无符号宽字节类型
typedef uint32_t                  wmkc_u32;   // 无符号整数类型
typedef const char *              wmkcCSTR;   // 固定字符串类型
#endif // #ifndef WMKC_TYPE_DEFINED

// 定义宏函数
/***将x转为字符串 ********/
#define wmkc_toString(x) #x

#else
#   define WMKC_SUPPORT false
#endif
#endif // #ifndef WMKC_CONF__
