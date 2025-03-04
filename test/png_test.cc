#include <config/WukConfig.hh>
#include <WukBuffer.hh>
#include <WukMisc.hh>
#include <zlib.h>

#include <iostream>
#include <filesystem>
#include <fstream>

#include <WukBuffer.cc>
#include <WukMisc.cc>

using namespace std;

constexpr wByte png_head_bytes[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

void print_buffer_use(wuk::Buffer &buffer)
{
    printf("buffer length: %zd\n", buffer.get_length());
    printf("buffer size:   %zd\n", buffer.get_size());
    printf("==================\n");
}

wuk::Buffer get_chunk(string name, wuk::Buffer data)
{
    wuk::Buffer result{4 + 4 + data.get_length() + 4};

    result.append_number(static_cast<wU32>(data.get_length()));
    result.append(name);
    result += data;

    const wByte *p = result.get_data() + 4;
    wU32 crc_val = crc32(0, p, result.get_length() - 4);

    result.append_number(crc_val);
    result.shrink_to_fit();

    return result;
}

wuk::Buffer get_ihdr_chunk(wU32 width, wU32 height, wByte bit_depth, wByte color_type)
{
    wuk::Buffer result{4 + 4 + 4 + 4 + 1 + 1 + 1 + 1 + 1 + 4};

    // 长度
    result.append_number(0x0d);
    // 名称
    result.append("IHDR");
    // 宽度
    result.append_number(width);
    // 高度
    result.append_number(height);
    // 位深
    result.append_number(bit_depth);
    // 颜色类型
    result.append_number(color_type);
    // 压缩方法
    result.append_number(static_cast<wByte>(0));
    // 过滤器方法
    result.append_number(static_cast<wByte>(0));
    // 扫描方法
    result.append_number(static_cast<wByte>(0));

    const wByte *p = result.get_data() + 4;
    wU32 crc_val = crc32(0, p, result.get_length() - 4);

    result.append_number(crc_val);
    result.shrink_to_fit();

    return result;
}

void test()
{
    const wByte *_pixel = (wByte *)"\x00\x00\x00\x00\xff\xff\xff\x00\xff\x00\xff\xff\x00\x00";
    wU32 _pixel_length = 14;

    wU32 pixel_length = compressBound(_pixel_length);
    wByte *pixel = wuk::m_alloc<wByte *>(pixel_length);

    int err = compress2(pixel, reinterpret_cast<uLongf *>(&pixel_length),
                        _pixel, _pixel_length, 9);
    if (err != Z_OK) {
        throw wuk::Exception(err, "main", zError(err));
    }

    wuk::Buffer ihdr = get_ihdr_chunk(2, 2, 8, 2);
    wuk::Buffer idat = get_chunk("IDAT", {pixel, pixel_length});
    wuk::Buffer iend = get_chunk("IEND", {});

    wuk::Buffer png{png_head_bytes, sizeof(png_head_bytes)};
    png += ihdr;
    png += idat;
    png += iend;

    wuk::misc::print_hex(png.get_data(), png.get_length(), 16, 1, 0);
    
    fstream f{filesystem::path{"F:/test.png"}, ios::binary | ios::out};
    f.write(png.get_cstr(), png.get_length());
    f.close();
}

int main()
{
    try {
        test();
    } catch (wuk::Exception &e) {
        cout << e.what() << endl;
    }

    return 0;
}
