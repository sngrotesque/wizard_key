#include <WukMisc.hh>

#include <cstdio>
#include <sstream>
#include <iomanip>
#ifdef WUK_PLATFORM_WINOS
#   include <windows.h>
#endif

using namespace wuk::color;

void print_diff_hex_byte(wuk::byte byte) {
#   ifndef WUK_NOT_COLOR
    if (byte == 0x00) {
        printf("%s""%02x ""%s", fore::lightRed, byte, all::reset);
    } else if (byte == 0xff) {
        printf("%s""%02x ""%s", fore::lightYellow, byte, all::reset);
    } else {
        printf("%02x ", byte);
    }
#   else
    printf("%02x ", byte);
#   endif
}

void print_diff_hex_line(const wuk::byte *data, wuk::ulong len, wuk::ulong start, wuk::ulong hex_per_line) {
    for (wuk::ulong j = 0; j < hex_per_line; ++j) {
        if ((start + j) < len) {
            print_diff_hex_byte(data[start + j]);
        } else {
            printf("   "); // 三个空格对齐
        }
    }
}

namespace wuk::misc {
    void print_diff_hex(const wuk::byte *data1, const wuk::byte *data2,
                        wuk::ulong len1, wuk::ulong len2,
                        wuk::u32 hex_per_line, bool indent)
    {
        wuk::ulong max_len = len1 > len2 ? len1 : len2; // 取两个数据的最大长度

        for (wuk::ulong i = 0; i < max_len; i += hex_per_line) {
            if (indent) printf("\t");

            print_diff_hex_line(data1, len1, i, hex_per_line);
            printf("\t\t");
            print_diff_hex_line(data2, len2, i, hex_per_line);

            printf("\n");
        }
    }

    void print_hex(const wuk::byte *data, wuk::ulong len, wuk::ulong num, bool newline,
                            bool indent)
    {
        for(wuk::ulong i = 0; i < len; ++i) {
            if(indent && ((i) % num == 0)) {
                printf("\t");
            }

    #       ifndef WUK_NOT_COLOR
            if(data[i] == 0x00) {
                printf("%s""%02x""%s", fore::lightRed, data[i], all::reset);
            } else if(data[i] == 0xff) {
                printf("%s""%02x""%s", fore::lightYellow, data[i], all::reset);
            } else {
                printf("%02x", data[i]);
            }
    #       else
            printf("%02x", data[i]);
    #       endif

            printf(((i + 1) % num) ? " " : "\n");
        }
        if(newline) printf("\n");
    }

    void print_number(const wuk::byte *arr, wuk::ulong size, wuk::u32 num, bool newline)
    {
        for (wuk::ulong i = 0; i < size; ++i) {
            int len = printf("%d", arr[i]);

            if ((i + 1) != size)
                printf(",");

            for (int pad = 4 - len; pad > 0; --pad)
                putchar(' ');

            // 每 num 个元素换行
            if ((i + 1) % num == 0)
                printf("\n");
        }

        if (size % num != 0)
            printf("\n");

        if (newline)
            printf("\n");
    }

    void print_box(const wuk::byte *box, wuk::ulong size, wuk::ulong num, bool newline)
    {
        for(wuk::ulong i = 0; i < size; ++i) {
            printf("0x%02x", box[i]);
            printf(((i + 1) != size) ? (((i + 1) % num == 0) ? (",\n") : (", ")) : ("\n"));
        }
        if (newline)
            printf("\n");
    }

    void print_pybytes(const wuk::byte *buf, wuk::ulong size, bool newline)
    {
        for(wuk::ulong i = 0; i < size; ++i) {
            if (buf[i] < 0x20) {
                switch (buf[i]) {
                case 0x0a:
                    printf("\\n"); break;
                case 0x09:
                    printf("\\t"); break;
                case 0x0d:
                    printf("\\r"); break;
                default:
                    printf("\\x%02x", buf[i]);
                    break;
                }
            } else if (buf[i] >= 0x20 && buf[i] < 0x7f) {
                if (buf[i] == 0x5c) {
                    printf("\\\\");
                } else {
                    printf("%c", buf[i]);
                }
            } else {
                printf("\\x%02x", buf[i]);
            }
        }
        if(newline) {
            printf("\n");
        }
    }

    std::string get_pybytes(const wuk::byte *buf, wuk::ulong size, bool newline)
    {
        std::stringstream ss;

        for(wuk::ulong i = 0; i < size; ++i) {
            if (buf[i] < 0x20) {
                switch (buf[i]) {
                    case 0x0a: ss << "\\n"; break;
                    case 0x09: ss << "\\t"; break;
                    case 0x0d: ss << "\\r"; break;
                    default:   ss << "\\x"
                                << std::hex
                                << std::setw(2)
                                << std::setfill('0')
                                << static_cast<int>(buf[i]);
                                break;
                }
            } else if (buf[i] >= 0x20 && buf[i] < 0x7f) {
                if (buf[i] == 0x5c) {
                    ss << "\\\\";
                } else {
                    ss << buf[i];
                }
            } else {
                ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buf[i]);
            }
        }
        if(newline) {
            ss << "\n";
        }
        return ss.str();
    }
}
