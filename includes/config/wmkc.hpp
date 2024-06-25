#ifndef WMKC_CPP
#define WMKC_CPP

//  如果是Linux或Windows操作系统，那么支持。否则不支持。
#if defined(__linux)
#   if __BYTE_ORDER__
#       if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#           define WMKC_LE_ENDIAN true // 小端序
#       elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#           define WMKC_LE_ENDIAN false // 大端序
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
#   define WMKC_LE_ENDIAN true // 小端序，由于绝大多数（几乎所有）的Windows系统都是小端序，所以直接忽略大端序。
#else
#   define WMKC_SUPPORT false
#   warning "This library may not support the computer you are using."
#endif // #if defined(__linux)

#ifndef LIBWMKC_API
#  ifdef WMKC_EXPORTS
#    ifdef _WIN32
#      define LIBWMKC_API     __declspec(dllexport)
#    elif defined(__ELF__)
#      define LIBWMKC_API     __attribute__((visibility("protected")))
#    else
#      define LIBWMKC_API     __attribute__((visibility("default")))
#    endif
#  else
#    ifdef _WIN32
#      define LIBWMKC_API     __declspec(dllimport)
#    else
#      define LIBWMKC_API     __attribute__((visibility("default")))
#    endif
#  endif
#endif


#include <cstdarg>   // 标准参数库
#include <cstring>   // 标准字符串库
#include <cstdlib>   // 标准库
#include <cstdint>   // 标准数字类型库
#include <cstddef>   // 标准定义库
#include <cinttypes> // 用于在跨平台打印同样的数据类型

// 在Visual Studio中，C++已经集成了bool类型，不需要使用此头文件了。
#ifndef _MSC_VER
#   include <cstdbool>  // 标准布尔值库
#endif

#include <string>

// WMKC库类型定义
#ifndef WMKC_TYPE_DEFINED
#define WMKC_TYPE_DEFINED
typedef uint8_t  wByte;      // 字节类型
typedef ssize_t  wSSize;     // 长整数类型
typedef size_t   wSize;      // 长度类型
typedef int16_t  wS16, wI16; // 宽字节类型
typedef int32_t  wS32, wI32; // 整数类型
typedef uint16_t wU16;       // 无符号宽字节类型
typedef uint32_t wU32;       // 无符号整数类型
#endif /* WMKC_TYPE_DEFINED */

// 定义宏函数
#ifndef WMKC_MACRO_DEFINED
#define WMKC_MACRO_DEFINED
#define wmkc_toString(x) #x /* 将x转为字符串 */
#define wmkc_min(x, y) (((x) < (y)) ? (x) : (y))
#endif /* WMKC_MACRO_DEFINED */

#endif // #ifndef WMKC_CPP
