#include <wmkc_misc.hpp>

wmkcVoid wmkcMisc::PRINT(const wmkcByte *data, wmkcSize len, wmkcSize num, wmkcBool newline, wmkcBool tableChar)
{
    for(wmkcSize x = 0; x < len; ++x) {
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

wmkcVoid wmkcMisc::PRINT_N(const wmkcByte *arr, wmkcSize size, wmkcBool newline)
{
    for(wmkcSize x = 0; x < size; ++x) {
        printf("%3d", x[arr]);
        if(!((x + 1) % 32)) {
            printf("\n");
        } else {
            printf(", ");
        }
    }
    if(newline) printf("\n");
}

wmkcVoid wmkcMisc::PRINT_BOX(const wmkcByte *box, wmkcSize size, wmkcSize num, wmkcBool newline)
{
    for(wmkcSize x = 0; x < size; ++x) {
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

wmkcVoid wmkcMisc::PRINT_RAW(const wmkcByte *buf, wmkcSize size, wmkcBool newline)
{
    for(wmkcSize x = 0; x < size; ++x) {
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

