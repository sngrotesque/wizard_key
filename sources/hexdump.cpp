#include <hexdump.hpp>

#ifndef WUK_HD_BL
#define WUK_HD_BL 16
#endif

/**
 * @brief 以十六进制单行打印一段二进制数据
 * @authors SN-Grotesque
 * 
 * 以十六进制单行打印一段二进制数据，每一行的
 * 长度为WUK_HD_BL。
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
    for(i = 0; i < WUK_HD_BL; ++i) {
        (i < size) ? (printf("%02x", *(buf + i))) : (printf("  "));
        ((i + 1) % size) ? (printf(" ")) : (printf(" | "));
        if(i == (WUK_HD_BL / 2 - 1))
            printf(" ");
    }
    for(i = 0; i < size; ++i) {
        (*(buf + i) >= 0x20 && *(buf + i) < 0x7f) ? \
            (printf("%c", *(buf + i))) : (printf("."));
    }
    printf("\n");
}

template <typename T>
void wuk::hexdump(T file_path)
{
    std::fstream f(file_path, std::ios::in | std::ios::binary);

    if(!f.is_open()) {
        throw wuk::Exception(wukErr_Err, "wuk::hexdump", "Failed to file open.");
    }
    wByte buffer[WUK_HD_BL];
    wSize offset = 0;
    wSize nRead = 0;

    while(f.peek() != EOF) {
        nRead = f.read(reinterpret_cast<char *>(buffer), WUK_HD_BL).gcount();
        _hexdump(offset, buffer, nRead);
        offset += nRead;
    }

    f.close();
}

void wuk::hexdump(wByte *data, wSize length)
{
    wSize leftover = length % WUK_HD_BL;
    wSize total    = length / WUK_HD_BL;
    wSize offset   = 0;

    for(wSize i = 0; i < total; ++i, offset += WUK_HD_BL) {
        _hexdump(offset, data + offset, WUK_HD_BL);
    }
    if(leftover) {
        _hexdump(offset, data + offset, leftover);
    }
}
