#include <snHexdump.h>

WMKC_PRIVATE(wmkcVoid) _hexdump
WMKC_OF((wmkcSize offset, wmkcByte *buf, wmkc_u32 size))
{
    wmkc_u32 i;
    printf("%012"PRIx64" ", offset);
    for(i = 0; i < SN_HEXDUMP_SIZE; ++i) {
        SN_HEXDUMP_PRINT_HEX_SET;}
    for(i = 0; i < size; ++i) {
        SM_HEXDUMP_PRINT_PLAINTEXT;}
    printf("\n");
}

WMKC_PUBLIC(snErr_ctx) snHexdump WMKC_OPEN_API
WMKC_OF((snFileString fn))
{
    snErr_ctx error;
    if(!fn) {
        snErr_return(error, snErr_ErrNULL, "snHexdump: fn is NULL.");
    }
    wmkcFile *fp = wmkcNull;
    wmkcByte buf[SN_HEXDUMP_SIZE];
    wmkcSize offset = 0;
    wmkcSize nRead = 0;

    if(!(fp = snFile_fopen(fn, snFile_text("rb")))) {
        snErr_return(error, snErr_FileOpen, "snHexdump: File opening failed.");
    }

    while(!feof(fp)) {
        nRead = fread(buf, 1, SN_HEXDUMP_SIZE, fp);
        _hexdump(offset, buf, nRead);
        offset += SN_HEXDUMP_SIZE;
    }

    if(fclose(fp)) {
        snErr_return(error, snErr_FileClose, "snHexdump: File closing failed.");
    }
    snErr_return(error, snErr_OK, "OK.");
}

