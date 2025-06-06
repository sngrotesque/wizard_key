#include <WukMisc.hh>

void print_diff_hex_byte(wByte byte) {
    if (byte == 0) {
        printf("%s""%02x ""%s", wuk::color::fore::lightRed, byte, wuk::color::all::reset);
    } else if (byte == 255) {
        printf("%s""%02x ""%s", wuk::color::fore::lightYellow, byte, wuk::color::all::reset);
    } else {
        printf("%02x ", byte);
    }
}

void print_diff_hex_line(const wByte *data, wSize len, wSize start, wSize hex_per_line) {
    for (wSize j = 0; j < hex_per_line; ++j) {
        if ((start + j) < len) {
            print_diff_hex_byte(data[start + j]);
        } else {
            printf("   "); // 三个空格对齐
        }
    }
}

void wuk::misc::print_diff_hex(const wByte *data1, const wByte *data2,
                    wSize len1, wSize len2,
                    wU32 hex_per_line, bool indent)
{
    wSize max_len = len1 > len2 ? len1 : len2; // 取两个数据的最大长度

    for (wSize i = 0; i < max_len; i += hex_per_line) {
        if (indent) printf("\t");

        print_diff_hex_line(data1, len1, i, hex_per_line);
        printf("\t\t");
        print_diff_hex_line(data2, len2, i, hex_per_line);

        printf("\n");
    }
}

void wuk::misc::print_hex(const wByte *data, wSize len, wSize num, bool newline,
                        bool indent)
{
    using namespace wuk::color;

    for(wSize i = 0; i < len; ++i) {
        if(indent && ((i) % num == 0)) {
            printf("\t");
        }

        if(!data[i]) {
            printf("%s""%02x""%s", fore::lightRed, data[i], all::reset);
        } else if(!(data[i] ^ 0xff)) {
            printf("%s""%02x""%s", fore::lightYellow, data[i], all::reset);
        } else {
            printf("%02x", data[i]);
        }

        printf(((i + 1) % num) ? " " : "\n");
    }
    if(newline) printf("\n");
}

void wuk::misc::print_number(const wByte *arr, wSize size, bool newline)
{
    for(wSize i = 0; i < size; ++i) {
        printf("%3d", i[arr]);
        printf((!((i + 1) % 32)) ? "\n" : ", ");
    }
    if(newline) printf("\n");
}

void wuk::misc::print_box(const wByte *box, wSize size, wSize num, bool newline)
{
    for(wSize i = 0; i < size; ++i) {
        printf("0x%02x", box[i]);
        printf(((i + 1) != size) ? (((i + 1) % num == 0) ? (",\n") : (", ")) : ("\n"));
    }
    if (newline)
        printf("\n");
}

void wuk::misc::print_pybytes(const wByte *buf, wSize size, bool newline)
{
    for(wSize i = 0; i < size; ++i) {
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

