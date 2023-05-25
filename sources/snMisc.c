#include <snMisc.h>

WMKC_PUBLIC(wmkcVoid) snMisc_PRINT WMKC_OPEN_API
WMKC_OF((wmkcByte *data, wmkcSize len, wmkcSize num, wmkcBool newline, wmkcBool tableChar))
{
    for(wmkcSize x = 0; x < len; ++x) {
        if(tableChar && x == 0)
            printf("\t");
        switch(data[x]) {
            case 0:
                printf("%s%02x%s", SN_FORE_COLOR_LIGHTRED, data[x], SN_ALL_COLOR_RESET);
                break;
            case 0xff:
                printf("%s%02x%s", SN_FORE_COLOR_LIGHTYELLOW, data[x], SN_ALL_COLOR_RESET);
                break;
            default:
                printf("%02x", data[x]);
                break;
        }
        switch((x + 1) % num) {
            case 0:
                printf("\n");
                if(tableChar)
                    printf("\t");
                break;
            default:
                printf(" ");
                break;
        }
    }
    if(newline)
        printf("\n");
}

WMKC_PUBLIC(wmkcVoid) snMisc_PRINT_N WMKC_OPEN_API
WMKC_OF((wmkc_s32 *arr, wmkcSize size, wmkcBool newline))
{
    for(wmkcSize x = 0; x < size; ++x) {
        printf("%3d", x[arr]);
        if(!((x+1) % 32)) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
    if (newline)
        printf("\n");
}

WMKC_PUBLIC(wmkcVoid) snMisc_PRINT_BOX WMKC_OPEN_API
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

WMKC_PUBLIC(wmkcVoid) snMisc_PRINT_RAW WMKC_OPEN_API
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