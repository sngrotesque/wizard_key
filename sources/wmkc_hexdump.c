#include <wmkc_hexdump.h>

#define WMKC_HEXDUMP_SIZE 32
#define WMKC_HEXDUMP_PRINT_PLAINTEXT \
    

WMKC_PRIVATE(wmkcVoid) _hexdump
WMKC_OF((wmkcSize offset, wmkcByte *buf, wmkc_u32 size))
{
    wmkc_u32 i;
    printf("%012"PRIx64" ", offset);
    for(i = 0; i < WMKC_HEXDUMP_SIZE; ++i) {
        (i < size) ? (printf("%02x", *(buf + i))) : (printf("  "));
        ((i + 1) % size) ? (printf(" ")) : (printf(" "));
        if(i == (WMKC_HEXDUMP_SIZE / 2 - 1))
            printf(" ");
    }
    for(i = 0; i < size; ++i) {
        (*(buf + i) >= 0x20 && *(buf + i) < 0x7f) ? (printf("%c", *(buf + i))) : (printf("."));
    }
    printf("\n");
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcHexdump WMKC_OPEN_API
WMKC_OF((wmkcFileString fn))
{
    wmkcErr_ctx error;
    if(!fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcHexdump: fn is NULL.");
    }
    wmkcFile *fp = wmkcNull;
    wmkcByte buf[WMKC_HEXDUMP_SIZE];
    wmkcSize offset = 0;
    wmkcSize nRead = 0;

    if(!(fp = wmkcFile_fopen(fn, wmkcFile_text("rb")))) {
        wmkcErr_return(error, wmkcErr_FileOpen, "wmkcHexdump: File opening failed.");
    }

    while(!feof(fp)) {
        nRead = fread(buf, 1, WMKC_HEXDUMP_SIZE, fp);
        _hexdump(offset, buf, nRead);
        offset += WMKC_HEXDUMP_SIZE;
    }

    if(fclose(fp)) {
        wmkcErr_return(error, wmkcErr_FileClose, "wmkcHexdump: File closing failed.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

