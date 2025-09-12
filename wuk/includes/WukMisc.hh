#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <utils/color.hh>
#include <string>

namespace wuk::misc {
    LIBWUK_API void print_diff_hex(const wuk::byte *data1, const wuk::byte *data2,
                                         wuk::ulong len1, wuk::ulong len2,
                                         wuk::u32 hex_per_line, bool indent);
    LIBWUK_API void print_hex(const wuk::byte *data, wuk::ulong len, wuk::ulong hex_per_line, bool newline,
                            bool indent);
    LIBWUK_API void print_number(const wuk::byte *arr, wuk::ulong size, wuk::u32 num, bool newline);
    LIBWUK_API void print_box(const wuk::byte *box, wuk::ulong size, wuk::ulong num, bool newline);
    LIBWUK_API void print_pybytes(const wuk::byte *buf, wuk::ulong size, bool newline);

    LIBWUK_API std::string get_pybytes(const wuk::byte *buf, wuk::ulong size, bool newline);
    LIBWUK_API std::string log_utf8(const std::string &message);
}

#endif
