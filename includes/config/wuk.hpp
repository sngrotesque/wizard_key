#ifndef WUK_CPP
#define WUK_CPP

#include <cstdlib>   // 标准库
#include <cstdarg>   // 标准参数库
#include <cstring>   // 标准字符串库
#include <cstdint>   // 标准数字类型库
#include <cinttypes> // 用于在跨平台打印同样的数据类型

#include <string>

/**
 * 在Visual Studio中，C++已经集成了bool类型，不需要使用此头文件了。
 * https://learn.microsoft.com/zh-cn/cpp/cpp/bool-cpp?view=msvc-170
 */
#ifndef _MSC_VER
#   include <cstdbool>  // 标准布尔值库
#endif

/**
 * 平台判断，后续可能继续改进。
 * 
 * Microsoft官方只定义了这两个宏用于区分当前是否为Windows平台，不要添加其他宏来判断，因为那是无意义的。
 * https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170
 * 
 * 关于其他平台（信息未证实，请谨慎采信，信息真实度未知）
 * https://www.cnblogs.com/foohack/p/5013272.html
 * 
 * 关于Linux平台（主要为GNU环境，可能不包括Clang）
 * https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
 */
#if defined(_WIN32) || defined(_WIN64) // Microsoft Windows
#   define WUK_PLATFORM_WINOS
#   define WUK_SUPPORT true
#   define WUK_LE_ENDIAN true // 小端序，由于绝大多数（几乎所有）的Windows系统都是小端序，所以直接忽略大端序。
#elif defined(__linux) || defined(__gnu_linux__) || defined(__linux__) // Linux
#   define WUK_PLATFORM_LINUX
#   define WUK_SUPPORT true
#   if __BYTE_ORDER__
#       if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#           define WUK_LE_ENDIAN true // 小端序
#       elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#           define WUK_LE_ENDIAN false // 大端序
#       else
#           error "Endian judgment error."
#       endif
#   else
#       error "Endian judgment error."
#   endif
#elif defined(__ANDROID__) // Android
#   define WUK_PLATFORM_ANDROID
#   define WUK_SUPPORT false
#elif defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || \
    defined(__MACH__) // Mac OS
#   define WUK_PLATFORM_MACOS
#   define WUK_SUPPORT false
#endif

/**
 * https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html
 * https://dev.to/tenry/predefined-c-c-macros-43id
 * Visual Studio       _MSC_VER
 * gcc                 __GNUC__
 * clang               __clang__
 * llvm                __llvm__
 * MinGW 32            __MINGW32__
 * MinGW-w64 32bit     __MINGW32__
 * MinGW-w64 64bit     __MINGW64__
 * 
 * 对于Clang
 * https://releases.llvm.org/11.0.0/tools/clang/docs/UsersManual.html
 * https://clang.llvm.org/docs/ClangCommandLineReference.html
 * https://www.bookstack.cn/read/clang-llvm/get_started.md
 */
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
#elif defined(__GNUC__)
#else
#   ifndef WUK_NO_COMPILER_WARNING
#       warning "This library is not using this compiler for testing, please be careful."
#   endif
#endif

// 检查是否被支持
#if WUK_SUPPORT == false
#   warning "This library may not support the computer you are using."
#endif

// 检查是否存在导出宏
#ifndef LIBWUK_API
#  ifdef WUK_EXPORTS
#    ifdef WUK_PLATFORM_WINOS
#      define LIBWUK_API     __declspec(dllexport)
#    elif defined(__ELF__) || defined(WUK_PLATFORM_LINUX)
#      define LIBWUK_API     __attribute__((visibility("protected")))
#    else
#      define LIBWUK_API     __attribute__((visibility("default")))
#    endif
#  else
#    ifdef WUK_PLATFORM_WINOS
#      define LIBWUK_API     __declspec(dllimport)
#    else
#      define LIBWUK_API     __attribute__((visibility("default")))
#    endif
#  endif
#endif

// WUK库类型定义
#ifndef WUK_TYPE_DEFINED
#define WUK_TYPE_DEFINED
typedef uint8_t  wByte,  w_byte;  // 字节类型

typedef int16_t  wS16,   w_s16;   // 16位带符号整数
typedef int16_t  wI16,   w_i16;   // 16位带符号整数
typedef uint16_t wU16,   w_u16;   // 16位无符号整数

typedef int32_t  wS32,   w_s32;   // 32位带符号整数
typedef int32_t  wI32,   w_i32;   // 32位带符号整数
typedef uint32_t wU32,   w_u32;   // 32位无符号整数

typedef int64_t  wI64,   w_i64;   // 64位带符号整数
typedef int64_t  wS64,   w_s64;   // 64位带符号整数
typedef uint64_t wU64,   w_u64;   // 64位无符号整数

typedef ssize_t  wSSize, w_long;  // 带符号长整数
typedef size_t   wSize,  w_ulong; // 无符号长整数
#endif /* WUK_TYPE_DEFINED */

// 定义宏函数
#ifndef WUK_MACRO_DEFINED
#define WUK_MACRO_DEFINED
#define wuk_toString(x) #x /* 将x转为字符串 */
#define wuk_min(x, y) (((x) < (y)) ? (x) : (y))
#endif /* WUK_MACRO_DEFINED */

#endif // #ifndef WUK_CPP
