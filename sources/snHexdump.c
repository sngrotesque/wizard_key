#include <snHexdump.h>

SN_PRIVATE(snVoid) _hexdump
SN_FUNC_OF((snSize offset, snByte *buf, sn_u32 size))
{
    static sn_u32 i;
    printf("%012"PRIx64" ", offset);
    for(i = 0; i < SN_HEXDUMP_SIZE; ++i) {
        SN_HEXDUMP_PRINT_HEX_SET;}
    for(i = 0; i < size; ++i) {
        SM_HEXDUMP_PRINT_PLAINTEXT;}
    printf("\n");
}

SN_PUBLIC(snVoid) snHexdump SN_OPEN_API
SN_FUNC_OF((snString fn))
{
    static snFile *fp = snNull;
    static snByte buf[SN_HEXDUMP_SIZE];
    static snSize offset = 0;
    static snSize nRead = 0;

    fp = fopen(fn, "rb");

    while(!feof(fp)) {
        nRead = fread(buf, 1, SN_HEXDUMP_SIZE, fp);
        _hexdump(offset, buf, nRead);
        offset += SN_HEXDUMP_SIZE;
    }

    fclose(fp);
}


