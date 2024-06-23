#include <misc.hpp>

void wmkc::misc::PRINT_HEX(const wByte *data, wSize len, wSize num, wBool newline,
                                                                wBool tableChar)
{
    for(wSize x = 0; x < len; ++x) {
        if(tableChar && ((x) % num == 0)) {
            printf("\t");
        }

        if(!data[x]) {
            printf(WMKC_COLOR_SET(WMKC_COLOR_FORE_LIGHTRED, "%02x"), data[x]);
        } else if(!(data[x] ^ 0xff)) {
            printf(WMKC_COLOR_SET(WMKC_COLOR_FORE_LIGHTYELLOW, "%02x"), data[x]);
        } else {
            printf("%02x", data[x]);
        }

        if((x + 1) % num) {
            printf(" ");
        } else {
            printf("\n");
        }
    }
    if(newline) printf("\n");
}

void wmkc::misc::PRINT_N(const wByte *arr, wSize size, wBool newline)
{
    for(wSize x = 0; x < size; ++x) {
        printf("%3d", x[arr]);
        if(!((x + 1) % 32)) {
            printf("\n");
        } else {
            printf(", ");
        }
    }
    if(newline) printf("\n");
}

void wmkc::misc::PRINT_BOX(const wByte *box, wSize size, wSize num, wBool newline)
{
    for(wSize x = 0; x < size; ++x) {
        printf("0x%02x", box[x]);
        if((x + 1) != size) {
            if((x + 1) % num == 0) {
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

void wmkc::misc::PRINT_PyBytes(const wByte *buf, wSize size, wBool newline)
{
    for(wSize x = 0; x < size; ++x) {
        if(buf[x] >= 0x20 && buf[x] < 0x7f) {
            printf("%c", buf[x]);
        } else if(buf[x] < 0x20) {
            if(buf[x] == 0x0a) {
                printf("\\n");
            } else if(buf[x] == 0x09) {
                printf("\\t");
            } else if(buf[x] == 0x0d) {
                printf("\\r");
            } else {
                printf("\\x%02x", buf[x]);
            }
        } else if(buf[x] > 0x7e) {
            printf("\\x%02x", buf[x]);
        } else {
            printf("=");
        }
    }
    if(newline) {
        printf("\n");
    }
}

