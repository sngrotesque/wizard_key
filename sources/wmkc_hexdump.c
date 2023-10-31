#include <wmkc_hexdump.h>

/**
 * @brief 以十六进制单行打印一段二进制数据
 * @authors SN-Grotesque
 * 
 * 以十六进制单行打印一段二进制数据，每一行的
 * 长度为WMKC_HEXDUMP_BLOCKLEN。
 * 
 * @note 此函数为wmkcHexdump函数的私有函数
 * @param offset 这是一个偏移量，代表当前文件指针的位置。
 * @param buf 指针，指向需要打印的内容的地址。
 * @param size 这是一个长度，为buf指针指向的内容的长度（一般不超过32字节）。
 * @return 无
*/
WMKC_PRIVATE(wmkcVoid) _hexdump
WMKC_OF((wmkcSize offset, wmkcByte *buf, wmkc_u32 size))
{
    wmkc_u32 i;
    printf("%012"PRIx64" ", offset);
    for(i = 0; i < WMKC_HEXDUMP_BLOCKLEN; ++i) {
        (i < size) ? (printf("%02x", *(buf + i))) : (printf("  "));
        ((i + 1) % size) ? (printf(" ")) : (printf(" "));
        if(i == (WMKC_HEXDUMP_BLOCKLEN / 2 - 1))
            printf(" ");
    }
    for(i = 0; i < size; ++i) {
        (*(buf + i) >= 0x20 && *(buf + i) < 0x7f) ? (printf("%c", *(buf + i))) : (printf("."));
    }
    printf("\n");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcHexdump WMKC_OPEN_API
WMKC_OF((wmkcCSTR fn))
{
    wmkcErr_obj error;
    if(!fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcHexdump", "fn is NULL.");
    }
    wmkcFile_obj *file = wmkcNull;
    wmkcByte buf[WMKC_HEXDUMP_BLOCKLEN];
    wmkcSize offset = 0;
    wmkcSize nRead = 0;

    if((error = wmkcFile_open(&file, fn, "rb")).code)
        return error;

    while(!feof(file->fp)) {
        nRead = fread(buf, 1, WMKC_HEXDUMP_BLOCKLEN, file->fp);
        _hexdump(offset, buf, nRead);
        offset += WMKC_HEXDUMP_BLOCKLEN;
    }

    if((error = wmkcFile_close(&file)).code)
        return error;

    wmkcErr_return(error, wmkcErr_OK, "wmkcHexdump", "OK.");
}

