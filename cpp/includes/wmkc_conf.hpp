#ifndef WMKC_CPP
#define WMKC_CPP

#include <iostream>
#include <stdexcept>

#include <cstring>   // 标准字符串库
#include <cstdlib>   // 标准库
#include <cstdint>   // 标准数字类型库
#include <csignal>   // 信号库
#include <cstdbool>  // 标准布尔值库
#include <cinttypes> // 用于在跨平台打印同样的数据类型

// 判断是否是64位系统，不是的话，不进行支持。
#if defined(_WIN64) || (__SIZEOF_SIZE_T__ == 8) || defined(__x86_64__) || defined(__LP64__)
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

#ifndef WMKC_TYPE_DEFINED
#define WMKC_TYPE_DEFINED
#define wmkcNull                  nullptr     // 空指针
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
#ifndef WMKC_MACRO_DEFINED
#define WMKC_MACRO_DEFINED
#define wmkc_toString(x) #x
#define wmkc_min(x, y) (((x) < (y)) ? (x) : (y))
#endif

#else
#   define WMKC_SUPPORT false
#   warning "This library may not support the computer you are using."
#endif // #if defined(_WIN64) || (__SIZEOF_SIZE_T__ == 8) || defined(__x86_64__) || defined(__LP64__)

#endif // #ifndef WMKC_CPP
