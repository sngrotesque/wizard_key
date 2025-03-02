#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukColor.hh>

namespace wuk {
    namespace misc {
        LIBWUK_API void print_hex(const wByte *data, wSize len, wSize num, bool newline,
                                bool tableChar);
        LIBWUK_API void print_number(const wByte *arr, wSize size, bool newline);
        LIBWUK_API void print_box(const wByte *box, wSize size, wSize num, bool newline);
        LIBWUK_API void print_pybytes(const wByte *buf, wSize size, bool newline);
    }
}

#endif /* WUK_SUPPORT */
