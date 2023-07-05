#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_IMAGE
#define WMKC_IMAGE
#include <wmkc_error.h>
#include <wmkc_memory.h>
#include <wmkc_struct.h>
#include <zlib/zlib.h>

typedef struct {
    wmkcByte r;
    wmkcByte g;
    wmkcByte b;
} wmkcImage_rgb888;

typedef struct {
    wmkcByte r :5;
    wmkcByte g :6;
    wmkcByte b :5;
} wmkcImage_rgb565;

#ifndef WMKC_IMAGE_PNG
#define WMKC_IMAGE_PNG
// PNG图像结构文件头
#define WMKC_IMAGE_PNG_HEAD "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"
// PNG图像结构文件尾
#define WMKC_IMAGE_PNG_IEND "\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82"

typedef struct {
    /* * PNG图像结构块的构成（IDAT为例）
    * 00 00 FF A5 [块长度 :4字节(大端序)]
    * 49 44 41 54 [块名称IDAT :4字节]
    * .. .. .. .. [块内容 :可变字节，此处为65445字节]
    * A5 CE 02 68 [块校验 :4字节]
    * */
    wmkcByte size[4]; // 结构块的长度(大端序)
    wmkc_u32 size_n;  // 结构块的长度
    wmkcByte name[4]; // 结构块名称
    wmkcByte *data;   // 结构块的内容(包含块名称)
    wmkcByte crc[4];  // 结构块的CRC校验值(大端序)
    wmkc_u32 crc_n;   // 结构块的CRC校验值
} wmkcImage_png_obj;

/**
 * @brief rgb888转换rgb565函数
 * @authors SN-Grotesque
 * @note 无
 * @param rgb RGB888结构体
 * @return wmkcImage_rgb565类型
 */
WMKC_PUBLIC(wmkcImage_rgb565) rgb888_to_rgb565 WMKC_OPEN_API
WMKC_OF((wmkcImage_rgb888 rgb));

/**
 * @brief 初始化wmkcImage_png对象
 * @authors SN-Grotesque
 * @note 这个函数是将对象中的成员初始化为0
 * @param block 指针，指向wmkcImage_png对象的地址
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcImage_png_init WMKC_OPEN_API
WMKC_OF((wmkcImage_png_obj *block));

/**
 * @brief 计算PNG块数据的CRC32校验值
 * @authors SN-Grotesque
 * @note 无
 * @param block 指针，指向wmkcImage_png对象的地址
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcImage_png_crc32 WMKC_OPEN_API
WMKC_OF((wmkcImage_png_obj *block));

/**
 * @brief 生成IHDR块函数
 * @authors SN-Grotesque
 * @note 无
 * @param IHDR 指针，指向wmkcImage_png对象指针的地址
 * @param width 图像宽度
 * @param height 图像高度
 * @param colorType 图像的颜色类型
 * @param bitDeep 图像的位深
 * @param compression 图像的压缩方法
 * @param filter 图像的过滤器方法
 * @param interlace 图像的交错方法
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcImage_png_IHDR WMKC_OPEN_API
WMKC_OF((wmkcImage_png_obj **IHDR,
    wmkc_u16 width,   wmkc_u16 height,
    wmkcByte bitDeep, wmkcByte colorType,
    wmkcByte compression, wmkcByte filter, wmkcByte interlace));

#endif /* WMKC_IMAGE_PNG */

#endif /* WMKC_IMAGE */
#endif /* WMKC_SUPPORT */
