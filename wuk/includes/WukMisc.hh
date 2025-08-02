#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukColor.hh>
#include <cstdio>
#include <string>
#include <sstream>
#include <iomanip>

namespace wuk {
    namespace misc {
        LIBWUK_API void print_diff_hex(const wByte *data1, const wByte *data2,
                                    wSize len1, wSize len2,
                                    wU32 hex_per_line, bool indent);
        LIBWUK_API void print_hex(const wByte *data, wSize len, wSize num, bool newline,
                                bool indent);
        LIBWUK_API void print_number(const wByte *arr, wSize size, wU32 num, bool newline);
        LIBWUK_API void print_box(const wByte *box, wSize size, wSize num, bool newline);
        LIBWUK_API void print_pybytes(const wByte *buf, wSize size, bool newline);

        LIBWUK_API std::string get_pybytes(const wByte *buf, wSize size, bool newline);
    }
}

#endif
