#include <snHexdump.h>

SN_PRIVATE(snVoid) _hexdump
SN_FUNC_OF((snSize offset, snByte *buf, sn_u32 size))
{
    sn_u32 i;
    printf("%012"PRIx64" ", offset);
    for(i = 0; i < SN_HEXDUMP_SIZE; ++i) {
        SN_HEXDUMP_PRINT_HEX_SET;}
    for(i = 0; i < size; ++i) {
        SM_HEXDUMP_PRINT_PLAINTEXT;}
    printf("\n");
}

SN_PUBLIC(snErr_ctx) snHexdump SN_OPEN_API
SN_FUNC_OF((snFileString fn))
{
    snErr_ctx error;
    if(!fn) {
        snErr_return(error, snErr_ErrNULL, "snHexdump: fn is NULL.");
    }
    snFile *fp = snNull;
    snByte buf[SN_HEXDUMP_SIZE];
    snSize offset = 0;
    snSize nRead = 0;

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


