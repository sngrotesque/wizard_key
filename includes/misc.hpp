#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_MISC
#define WMKC_CPP_MISC
#include <config/color.hpp>

namespace wmkc {
    namespace misc {
        // 为什么这些函数的名称是大写的？？？
        LIBWMKC_API void PRINT_HEX(const wByte *data, wSize len, wSize num, bool newline,
                                bool tableChar);
        LIBWMKC_API void PRINT_N(const wByte *arr, wSize size, bool newline);
        LIBWMKC_API void PRINT_BOX(const wByte *box, wSize size, wSize num, bool newline);
        LIBWMKC_API void PRINT_PyBytes(const wByte *buf, wSize size, bool newline);
    }
}

#endif /* WMKC_CPP_MISC */
#endif /* WMKC_SUPPORT */
