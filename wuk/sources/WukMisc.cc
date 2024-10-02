#include <WukMisc.hh>

void wuk::misc::print_hex(const wByte *data, wSize len, wSize num, bool newline,
                                                                bool tableChar)
{
    using namespace wuk::color;

    for(wSize i = 0; i < len; ++i) {
        if(tableChar && ((i) % num == 0)) {
            printf("\t");
        }

        if(!data[i]) {
            printf("%s""%02x""%s", fore::lightRed, data[i], all::reset);
        } else if(!(data[i] ^ 0xff)) {
            printf("%s""%02x""%s", fore::lightYellow, data[i], all::reset);
        } else {
            printf("%02x", data[i]);
        }

        if((i + 1) % num) {
            printf(" ");
        } else {
            printf("\n");
        }
    }
    if(newline) printf("\n");
}

void wuk::misc::print_number(const wByte *arr, wSize size, bool newline)
{
    for(wSize i = 0; i < size; ++i) {
        printf("%3d", i[arr]);
        if(!((i + 1) % 32)) {
            printf("\n");
        } else {
            printf(", ");
        }
    }
    if(newline) printf("\n");
}

void wuk::misc::print_box(const wByte *box, wSize size, wSize num, bool newline)
{
    for(wSize i = 0; i < size; ++i) {
        printf("0x%02x", box[i]);
        if((i + 1) != size) {
            if((i + 1) % num == 0) {
                printf(",\n");
            } else {
                printf(", ");
            }
        } else {
            printf("\n");
        }
    }
    if (newline)
        printf("\n");
}

// 实现了一个，Python的Bytes的打印？？？
// yes
void wuk::misc::print_pybytes(const wByte *buf, wSize size, bool newline)
{
    for(wSize i = 0; i < size; ++i) {
        if(buf[i] >= 0x20 && buf[i] < 0x7f) {
            printf("%c", buf[i]);
        } else if(buf[i] < 0x20) {
            if(buf[i] == 0x0a) {
                printf("\\n");
            } else if(buf[i] == 0x09) {
                printf("\\t");
            } else if(buf[i] == 0x0d) {
                printf("\\r");
            } else {
                printf("\\x%02x", buf[i]);
            }
        } else if(buf[i] > 0x7e) {
            printf("\\x%02x", buf[i]);
        } else {
            printf("=");
        }
    }
    if(newline) {
        printf("\n");
    }
}

