#pragma once
#include <cstdlib>   // 标准库
#include <cstring>   // 标准字符串库
#include <cstdint>   // 标准数字类型库

/**
 * 在Visual Studio（以及C++17标准）中，C++已经集成了bool类型，不需要使用此头文件了。
 * https://learn.microsoft.com/zh-cn/cpp/cpp/bool-cpp?view=msvc-170
 */
#ifdef INCLUDE_STDBOOL
#   include <cstdbool>  // 标准布尔值库
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
#if defined(_MSC_VER) || defined(_MSVC_LANG)
#   define WUK_COMPILER_MSVC
#elif defined(__MINGW32__) || defined(__MINGW64__)
#   define WUK_COMPILER_MINGW
#elif defined(__GNUC__)
#   define WUK_COMPILER_GCC
#elif defined(__clang__)
#   define WUK_COMPILER_CLANG
#   ifndef WUK_NO_COMPILER_ERROR
#       warning "This library is using Clang compiler for testing, please be careful."
#   endif
#else
#   define WUK_COMPILER_UNKNOWN
#   ifndef WUK_NO_COMPILER_ERROR
#       error "Unknown compiler, unsure if this library supports it, please be careful."
#   endif
#endif

// 判断编译时是否使用C++20标准
// ===============================================
// C++ Standard Version Detection
// ===============================================

// MSVC uses _MSVC_LANG instead of __cplusplus (unless /Zc:__cplusplus is enabled)
#ifdef _MSVC_LANG
#   define WUK_CPP_STD _MSVC_LANG
#else
#   define WUK_CPP_STD __cplusplus
#endif

// C++17 baseline (WUK requires C++17 or higher)
#if WUK_CPP_STD >= 201703L
#   define WUK_STD_CPP_17
#else
#   error "WUK requires C++17 or later. Please use /std:c++17 or -std=c++17."
#endif

// Optional: C++20 feature macros
#if WUK_CPP_STD >= 202002L
#   define WUK_STD_CPP_20
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
#elif defined(__linux) || defined(__gnu_linux__) || defined(__linux__) // Linux
#   define WUK_PLATFORM_LINUX
#   if defined(__ANDROID__) // Android，此处主要用于支持安卓终端模拟（如Termux）用户。
#       define WUK_PLATFORM_ANDROID
#   endif
#   define WUK_SUPPORT true
#elif defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || \
    defined(__MACH__) // Mac OS
#   define WUK_PLATFORM_MACOS
#   define WUK_SUPPORT false
#endif

// 检查是否被支持
#if defined(WUK_SUPPORT) && (WUK_SUPPORT != true)
#   error "This library may not support the computer you are using."
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

#ifdef WUK_COMPILER_MSVC
#   include <BaseTsd.h>
using ssize_t = SSIZE_T;
#endif

// 旧版类型声明
using wByte  = uint8_t;

using wU16   = uint16_t;
using wU32   = uint32_t;
using wU64   = uint64_t;

using wI16   = int16_t;
using wI32   = int32_t;
using wI64   = int64_t;

using wSize  = size_t;
using wSSize = ssize_t;

namespace wuk {
// 新版类型声明
    using byte  = uint8_t;

    using u8    = uint8_t;
    using u16   = uint16_t;
    using u32   = uint32_t;
    using u64   = uint64_t;

    using i8    = int8_t;
    using i16   = int16_t;
    using i32   = int32_t;
    using i64   = int64_t;

    using ulong = size_t;
    using ilong = ssize_t;

    using f32   = float;
    using f64   = double;

// 定义通用函数
    template <typename T>
    inline const T &min(const T &x, const T &y)
    {
        return (x < y) ? x : y;
    }

    template <typename T>
    inline const T &max(const T &x, const T &y)
    {
        return (x > y) ? x : y;
    }
}
