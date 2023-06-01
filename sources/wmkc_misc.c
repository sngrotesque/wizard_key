/**
 * @note 此库是一个杂项库，可以不添加WMKC_SUPPORT宏进行强行
 *       判断。也可以不添加注释。
*/
#include <wmkc_misc.h>

WMKC_PUBLIC(wmkcVoid) wmkcMisc_PRINT WMKC_OPEN_API
WMKC_OF((wmkcByte *data, wmkcSize len, wmkcSize num, wmkcBool newline, wmkcBool tableChar))
{
    for(wmkcSize x = 0; x < len; ++x) {
        if(tableChar && ((x) % num == 0)) printf("\t");

        if(!data[x]) {
            printf(WMKC_SET_COLOR(WMKC_FORE_COLOR_LIGHTRED, "%02x"), data[x]);
        } else if(!(data[x] ^ 0xff)) {
            printf(WMKC_SET_COLOR(WMKC_FORE_COLOR_LIGHTYELLOW, "%02x"), data[x]);
        } else {
            printf("%02x", data[x]);
        }

        if((x + 1) % num) printf(" ");
        else printf("\n");
    }
    if(newline) printf("\n");
}

WMKC_PUBLIC(wmkcVoid) wmkcMisc_PRINT_N WMKC_OPEN_API
WMKC_OF((wmkcByte *arr, wmkcSize size, wmkcBool newline))
{
    for(wmkcSize x = 0; x < size; ++x) {
        printf("%3d", x[arr]);
        if(!((x + 1) % 32)) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
    if(newline) printf("\n");
}

WMKC_PUBLIC(wmkcVoid) wmkcMisc_PRINT_BOX WMKC_OPEN_API
WMKC_OF((wmkcByte *box, wmkcSize size, wmkcSize num, wmkcBool newline))
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

WMKC_PUBLIC(wmkcVoid) wmkcMisc_PRINT_RAW WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size, wmkcBool newline))
{
    wmkcSize x;
    for(x = 0; x < size; ++x) {
        if(buf[x] && buf[x] < 0x20)
            printf("\n");
        if(buf[x] > 0x7e)
            continue;
        printf("%c", buf[x]);
    }
    if(newline)
        printf("\n");
}