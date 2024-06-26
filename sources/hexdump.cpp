#include <hexdump.hpp>

#ifndef WMKC_HD_BL
#define WMKC_HD_BL 16
#endif

/**
 * @brief 以十六进制单行打印一段二进制数据
 * @authors SN-Grotesque
 * 
 * 以十六进制单行打印一段二进制数据，每一行的
 * 长度为WMKC_HD_BL。
 * 
 * @param offset 这是一个偏移量，代表当前文件指针的位置。
 * @param buf 指针，指向需要打印的内容的地址。
 * @param size 这是一个长度，为buf指针指向的内容的长度（一般不超过32字节）。
 * @return 无
*/
void _hexdump(wSize offset, wByte *buf, wU32 size)
{
    wU32 i;
    printf("%012" PRIx64 " | ", offset);
    for(i = 0; i < WMKC_HD_BL; ++i) {
        (i < size) ? (printf("%02x", *(buf + i))) : (printf("  "));
        ((i + 1) % size) ? (printf(" ")) : (printf(" | "));
        if(i == (WMKC_HD_BL / 2 - 1))
            printf(" ");
    }
    for(i = 0; i < size; ++i) {
        (*(buf + i) >= 0x20 && *(buf + i) < 0x7f) ? \
            (printf("%c", *(buf + i))) : (printf("."));
    }
    printf("\n");
}

template <typename T>
void wmkc::hexdump(T file_path)
{
    std::fstream f(file_path, std::ios::in | std::ios::binary);

    if(!f.is_open()) {
        throw wmkc::Exception(wmkcErr_Err, "wmkc::hexdump", "Failed to file open.");
    }
    wByte buffer[WMKC_HD_BL];
    wSize offset = 0;
    wSize nRead = 0;

    while(f.peek() != EOF) {
        nRead = f.read(reinterpret_cast<char *>(buffer), WMKC_HD_BL).gcount();
        _hexdump(offset, buffer, nRead);
        offset += nRead;
    }

    f.close();
}

void wmkc::hexdump(wByte *data, wSize length)
{
    wSize leftover = length % WMKC_HD_BL;
    wSize total    = length / WMKC_HD_BL;
    wSize offset   = 0;

    for(wSize i = 0; i < total; ++i, offset += WMKC_HD_BL) {
        _hexdump(offset, data + offset, WMKC_HD_BL);
    }
    if(leftover) {
        _hexdump(offset, data + offset, leftover);
    }
}
