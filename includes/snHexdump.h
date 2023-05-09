#ifndef __SN_HEXDUMP__
#define __SN_HEXDUMP__

#include <snConf.h>

#define SN_HEXDUMP_SIZE 32
#define SN_HEXDUMP_PRINT_HEX_SET \
    (i < size)?(printf("%02x", *(buf + i))):(printf("  ")); \
    ((i + 1) % size)?(printf(" ")):(printf(" ")); \
    if(i == (SN_HEXDUMP_SIZE / 2 - 1)) \
        printf(" ")
#define SM_HEXDUMP_PRINT_PLAINTEXT \
    (*(buf + i) >= 0x20 && *(buf + i) < 0x7f)?(printf("%c", *(buf + i))):(printf("."))

SN_PUBLIC(snVoid) snHexdump SN_OPEN_API
SN_FUNC_OF((const snChar *fn));

#endif
