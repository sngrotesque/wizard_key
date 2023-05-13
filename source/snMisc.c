#include <snMisc.h>

SN_PUBLIC(snVoid) snMisc_PRINT SN_OPEN_API
SN_FUNC_OF((snByte *data, snSize len, snSize num, snBool newline, snBool tableChar))
{
    for(snSize x = 0; x < len; ++x) {
        if(tableChar && x == 0)
            printf("\t");
        switch(data[x]) {
            case 0:
                printf("%s%02x%s", SN_CF_LIGHTRED, data[x], SN_CA_RESET);
                break;
            case 0xff:
                printf("%s%02x%s", SN_CF_LIGHTYELLOW, data[x], SN_CA_RESET);
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

SN_PUBLIC(snVoid) snMisc_PRINT_N SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size, snBool newline))
{
    for(snSize x = 0; x < size; ++x) {
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

SN_PUBLIC(snVoid) snMisc_PRINT_BOX SN_OPEN_API
SN_FUNC_OF((snByte *box, snSize size, snSize num, snBool newline))
{
    for(snSize x = 0; x < size; ++x) {
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

SN_PUBLIC(snVoid) snMisc_PRINT_RAW SN_OPEN_API
SN_FUNC_OF((snByte *buf, snSize size, snBool newline))
{
    snSize x;
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