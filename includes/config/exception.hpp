#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_EXCEPTION
#define WMKC_CPP_EXCEPTION
#include <stdexcept>
#include <cerrno>

// 使用C++20标准，如果用户指定了20标准那就是支持20，所以不需要担心不兼容的情况。
#if __cplusplus >= 202002
#   include <format>
#else
#   if defined(WMKC_PLATFORM_WINOS) && defined(_MSC_VER)
#       pragma comment(lib, "msvcprt")
#   endif
#endif

// 这些错误代码是为了之后使用？但似乎并没有多大用。
#define wmkcErr_OK         0 // 表示一切正常，不需要注意任何错误。
#define wmkcErr_Err        1 // 一般的错误，用于表示32位的错误代码。
#define wmkcErr_ErrNULL    2 // 错误的空数据，表示不应该传入一个空的数据（包含指针）。
#define wmkcErr_ErrMemory  4 // 错误的内存，只能用作申请内存失败时。
#define wmkcErr_ErrSysFunc 8 // 错误的调用，表示调用系统函数后返回了错误代码。


namespace wmkc {
    class LIBWMKC_API Exception : public std::exception {
        public:
            std::string output_message;
            Exception(wS32 code, std::string function, std::string message);
            const char *what() const noexcept override;
    };
}

#endif /* WMKC_CPP_EXCEPTION */
#endif /* WMKC_SUPPORT */
