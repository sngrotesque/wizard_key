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
 * @param buf 这是一个指针，指向需要打印的内容的地址。
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

/**
 * @brief 以十六进制打印文件完整内容
 * @authors SN-Grotesque
 * 
 * 以十六进制打印文件的完整内容，每一行的长度为WMKC_HEXDUMP_BLOCKLEN。
 * 
 * @note 无
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcHexdump WMKC_OPEN_API
WMKC_OF((wmkcCSTR fn))
{
    wmkcErr_obj error;
    if(!fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcHexdump: fn is NULL.");
    }
    FILE *fp = wmkcNull;
    wmkcByte buf[WMKC_HEXDUMP_BLOCKLEN];
    wmkcSize offset = 0;
    wmkcSize nRead = 0;

    if(!(fp = wmkcFile_fopen(fn, "rb"))) {
        wmkcErr_return(error, wmkcErr_FileOpen, "wmkcHexdump: File opening failed.");
    }

    while(!feof(fp)) {
        nRead = fread(buf, 1, WMKC_HEXDUMP_BLOCKLEN, fp);
        _hexdump(offset, buf, nRead);
        offset += WMKC_HEXDUMP_BLOCKLEN;
    }

    if(fclose(fp)) {
        wmkcErr_return(error, wmkcErr_FileClose, "wmkcHexdump: File closing failed.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

