#ifndef WMKC_CPP
#define WMKC_CPP

// // 判断是否是64位系统，不是的话，不进行支持。
// #if defined(_WIN64) || (__SIZEOF_SIZE_T__ == 8) || defined(__x86_64__) || defined(__LP64__)
// // ...
// #else
// #   define WMKC_SUPPORT false
// #   warning "This library may not support the computer you are using."
// #endif // #if defined(_WIN64) || (__SIZEOF_SIZE_T__ == 8) || defined(__x86_64__) || defined(__LP64__)

#include <cstdarg>   // 标准参数库
#include <cstring>   // 标准字符串库
#include <cstdlib>   // 标准库
#include <cstdint>   // 标准数字类型库
#include <cstdbool>  // 标准布尔值库
#include <cinttypes> // 用于在跨平台打印同样的数据类型

#include <string>

//  如果是Linux或Windows操作系统，那么支持。否则不支持。
#if defined(__linux)
#   if __BYTE_ORDER__
#       if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#           define WMKC_LE_DIAN true // 小端序
#       elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#           define WMKC_LE_DIAN false // 大端序
#       else
#           error "Endian judgment error."
#       endif
#   else
#       error "Endian judgment error."
#   endif
#   define WMKC_PLATFORM_LINUX
#   define WMKC_SUPPORT true
#elif defined(_WIN32) || defined(_WIN64)
#   define WMKC_PLATFORM_WINOS
#   define WMKC_SUPPORT true
#   define WMKC_LE_DIAN true // 小端序
#else
#   define WMKC_SUPPORT false
#   warning "This library may not support the computer you are using."
#endif // #if defined(__linux)

// WMKC库类型定义
#ifndef WMKC_TYPE_DEFINED
#define WMKC_TYPE_DEFINED
typedef char                      wChar;   // 字符类型
typedef uint8_t                   wByte;   // 字节类型
typedef int64_t                   wSSize;  // 长整数类型
typedef size_t                    wSize;   // 长度类型
typedef void                      wVoid;   // 空类型
typedef bool                      wBool;   // 布尔类型
typedef int16_t                   wS16;    // 宽字节类型
typedef int16_t                   wI16;
typedef int32_t                   wS32;    // 整数类型
typedef int32_t                   wI32;
typedef uint16_t                  wU16;    // 无符号宽字节类型
typedef uint32_t                  wU32;    // 无符号整数类型
#endif /* WMKC_TYPE_DEFINED */

// 定义宏函数
#ifndef WMKC_MACRO_DEFINED
#define WMKC_MACRO_DEFINED
#define wmkc_toString(x) #x /* 将x转为字符串 */
#define wmkc_min(x, y) (((x) < (y)) ? (x) : (y))
#endif /* WMKC_MACRO_DEFINED */

#endif // #ifndef WMKC_CPP
