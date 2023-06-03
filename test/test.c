// #include <network/wmkc_net.h>
// #include <image/wmkc_png.h>
// #include <crypto/snc.h>
// #include <wmkc_binascii.h>
// #include <wmkc_hexdump.h>
// #include <wmkc_object.h>
// #include <wmkc_random.h>
// #include <wmkc_struct.h>
// #include <wmkc_thread.h>
// #include <wmkc_base64.h>
// #include <wmkc_dict.h>
// #include <wmkc_file.h>
// #include <wmkc_misc.h>
// #include <wmkc_math.h>
// #include <wmkc_time.h>
// #include <wmkc_hash.h>
// #include <wmkc_pad.h>
// #include <wmkc_key.h>

// #include <network/wmkc_net.c>
#include <image/wmkc_png.c>
#include <crypto/snc.c>
#include <wmkc_binascii.c>
#include <wmkc_hexdump.c>
#include <wmkc_object.c>
#include <wmkc_random.c>
#include <wmkc_struct.c>
#include <wmkc_thread.c>
#include <wmkc_base64.c>
#include <wmkc_dict.c>
#include <wmkc_file.c>
#include <wmkc_misc.c>
#include <wmkc_math.c>
#include <wmkc_time.c>
#include <wmkc_hash.c>
#include <wmkc_pad.c>
#include <wmkc_key.c>

#include <libpng/include/png.h>

#define CIPHER_TEST false

#if defined(WMKC_SNC) && (CIPHER_TEST)
static wmkcByte testKey[96] = {
    0x67, 0x65, 0x5b, 0x7b, 0x33, 0x78, 0x74, 0x6e, 0x49, 0x62, 0x6d, 0x3b, 0x7a, 0x77, 0x5d, 0x53,
    0x71, 0x52, 0x4a, 0x30, 0x3e, 0x2e, 0x63, 0x24, 0x67, 0x3b, 0x6e, 0x61, 0x6b, 0x3a, 0x21, 0x73,
    0x30, 0x66, 0x6c, 0x23, 0x74, 0x76, 0x45, 0x5d, 0x21, 0x3d, 0x50, 0x79, 0x60, 0x6c, 0x5d, 0x75,
    0x70, 0x6a, 0x68, 0x66, 0x52, 0x6e, 0x55, 0x4c, 0x7c, 0x51, 0x4e, 0x28, 0x31, 0x72, 0x7a, 0x40,
    0x49, 0x4e, 0x3b, 0x36, 0x6b, 0x69, 0x54, 0x37, 0x36, 0x70, 0x30, 0x3e, 0x3d, 0x60, 0x73, 0x42,
    0x5f, 0x61, 0x6f, 0x79, 0x7a, 0x36, 0x5e, 0x6d, 0x35, 0x40, 0x5f, 0x6d, 0x63, 0x57, 0x49, 0x31};
static wmkcByte testIv[32] = {
    0x7b, 0x75, 0x76, 0x37, 0x24, 0x59, 0x72, 0x29, 0x6e, 0x40, 0x5f, 0x65, 0x2d, 0x55, 0x75, 0x68,
    0x3d, 0x41, 0x78, 0x35, 0x48, 0x50, 0x7d, 0x73, 0x60, 0x4e, 0x33, 0x6f, 0x23, 0x47, 0x4c, 0x36};
#endif

void test()
{
    WCHAR *path = L"p:/Pixiv/7599643/2021_10_21_03_02_21_93580317_p0.jpg";
    SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, path, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
}

void png_test() {
    // 打开图像文件
    FILE *fp = fopen("p:/-3e7f44b459fa15c2.png", "rb");

    // 创建读取结构
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, wmkcNull, wmkcNull, wmkcNull);
    png_infop info_ptr = png_create_info_struct(png_ptr); // 创建信息结构
    png_init_io(png_ptr, fp);         // 初始化 I/O
    png_read_info(png_ptr, info_ptr); // 读取图像信息

    // 获取图像宽度和高度
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);

    // 打印图像宽度和高度
    printf("Image width:  %u\n", width);
    printf("Image height: %u\n", height);

    // 清理资源
    png_destroy_read_struct(&png_ptr, &info_ptr, wmkcNull);
    fclose(fp);
}

int main(int argc, char **argv)
{
    // test();
    png_test();

    return 0;
}
