#include <config/wuk.hpp>

#if WUK_SUPPORT
#ifndef WUK_CPP_MISC
#define WUK_CPP_MISC
#include <config/color.hpp>

namespace wuk {
    namespace misc {
        // 为什么这些函数的名称是大写的？？？
        LIBWUK_API void print_hex(const wByte *data, wSize len, wSize num, bool newline,
                                bool tableChar);
        LIBWUK_API void print_number(const wByte *arr, wSize size, bool newline);
        LIBWUK_API void print_box(const wByte *box, wSize size, wSize num, bool newline);
        LIBWUK_API void print_pybytes(const wByte *buf, wSize size, bool newline);
    }
}

#endif /* WUK_CPP_MISC */
#endif /* WUK_SUPPORT */
