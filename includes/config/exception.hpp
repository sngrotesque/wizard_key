#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_EXCEPTION
#define WMKC_CPP_EXCEPTION
#include <stdexcept>
#include <cerrno>

#define wmkcErr_OK              0    // 表示一切正常，不需要注意任何错误。
#define wmkcErr_Err             -1   // 一般的错误，用于表示32位的错误代码。
#define wmkcErr_ErrNULL         -2   // 错误的空数据，表示不应该传入一个空的数据（包含指针）。
#define wmkcErr_ErrMemory       -3   // 错误的内存，只能用作申请内存失败时。
#define wmkcErr_ErrSysFunc      -6   // 错误的调用，表示调用系统函数后返回了错误代码。

/*
* VS Ver     MSVC Ver   _MSC_VER Value
* VS 2022    14.3       1930
* VS 2019    14.2       1920
* VS 2017    14.1       1910
* VS 2015    14.0       1900
* VS 2013    12.0       1800
* VS 2012    11.0       1700
* VS 2010    10.0       1600
* VS 2008    9.0        1500
* VS 2005    8.0        1400
* VS 2003    7.1        1310
* VS 2002    7.0        1300
* VC++6.0    6.0        1200
*/

// 为真的话使用自定义实现的to_string方法，否则使用标准库的实现
// 此项是为了防止某些版本的Visual Studio莫名报错，傻逼Microsoft。
#define WMKC_ERR_EXCEPTION_TO_STRING false

#if defined(WMKC_PLATFORM_WINOS) && defined(_MSC_VER)
#   if (WMKC_ERR_EXCEPTION_TO_STRING == false)
#      pragma comment(lib, "msvcprt")
#   else
namespace std {
    template <typename T>
    std::string to_string(T value);
};
#   endif
#endif

namespace wmkc {
    void exception(wS32 errCode, std::string funcName, std::string errMessage);
}

#endif /* WMKC_CPP_EXCEPTION */
#endif /* WMKC_SUPPORT */
