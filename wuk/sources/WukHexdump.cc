#include <WukHexdump.hh>

#include <fstream>

constexpr wuk::u32 WUK_HD_BL = 16; // HexDump Block Size

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
void _hexdump(wuk::ulong offset, wuk::byte *buf, wuk::u32 size)
{
    printf("%012zx | ", offset);
    for(wuk::u32 i = 0; i < WUK_HD_BL; ++i) {
        if (i < size) {
            printf("%02x", *(buf + i));
        } else {
            printf("  ");
        }

        printf(((i + 1) % size) ? " " : " | ");

        if(i == (WUK_HD_BL / 2 - 1)) {
            printf(" ");
        }
    }
    for(wuk::u32 i = 0; i < size; ++i) {
        (*(buf + i) >= 0x20 && *(buf + i) < 0x7f) ? \
            (printf("%c", *(buf + i))) : (printf("."));
    }
    printf("\n");
}

void wuk::hexdump(fs::path file_path)
{
    std::fstream f(file_path, std::ios::in | std::ios::binary);

    if(!f.is_open()) {
        throw wuk::Exception(wuk::Error::FNOTF, "wuk::hexdump",
            "Failed to file open.");
    }
    wuk::byte buffer[WUK_HD_BL]{};
    wuk::ulong offset = 0;
    wuk::ulong nRead = 0;

    for(;;) {
        nRead = f.read(reinterpret_cast<char *>(buffer), WUK_HD_BL).gcount();
        if (!nRead) {
            f.close();
            break;
        }
        _hexdump(offset, buffer, nRead);
        offset += nRead;
    }
}

void wuk::hexdump(wuk::byte *data, wuk::ulong length)
{
    wuk::ulong leftover = length % WUK_HD_BL;
    wuk::ulong total    = length / WUK_HD_BL;
    wuk::ulong offset   = 0;

    for(wuk::ulong i = 0; i < total; ++i, offset += WUK_HD_BL) {
        _hexdump(offset, data + offset, WUK_HD_BL);
    }
    if(leftover) {
        _hexdump(offset, data + offset, leftover);
    }
}
