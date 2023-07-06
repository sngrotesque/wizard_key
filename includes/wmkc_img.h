#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_IMG
#define WMKC_IMG
#include <wmkc_error.h>
#include <wmkc_memory.h>
#include <wmkc_struct.h>
#include <zlib/zlib.h>

typedef struct {
    wmkcByte r;
    wmkcByte g;
    wmkcByte b;
} wmkcImage_rgb888_obj;

typedef struct {
    wmkcByte r :5;
    wmkcByte g :6;
    wmkcByte b :5;
} wmkcImage_rgb565_obj;

#ifndef WMKC_IMG_PNG
#define WMKC_IMG_PNG
// PNG图像结构文件头
#define WMKC_IMG_PNG_HEAD "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"
// PNG图像结构文件尾
#define WMKC_IMG_PNG_IEND "\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82"

// PNG图像的颜色类型
typedef enum {
    // 灰度图像，每个像素只有一个灰度值。
    wmkcImg_png_colorType_gray      = 0,
    // 真彩色图像，每个像素有RGB三个分量。
    wmkcImg_png_colorType_rgb       = 2,
    // 索引彩色图像。每个像素颜色由调色板中的一个索引值确定。
    wmkcImg_png_colorType_palette   = 3,
    // 带A通道的灰度图像。每个像素有一个灰度值和一个A值。
    wmkcImg_png_colorType_gratAlpha = 4,
    // 带A通道的真彩色图像。每个像素有RGB三个分量和一个A值。
    wmkcImg_png_colorType_rgba      = 6
} wmkcImg_png_colorType;

// PNG图像的位深度
typedef enum {
    // 每个颜色分量或样本占用1位。
    wmkcImg_png_bitDeep_1  = 1,
    // 每个颜色分量或样本占用2位。
    wmkcImg_png_bitDeep_2  = 2,
    // 每个颜色分量或样本占用4位。
    wmkcImg_png_bitDeep_4  = 4,
    // 每个颜色分量或样本占用8位。
    wmkcImg_png_bitDeep_8  = 8,
    // 每个颜色分量或样本占用16位。
    wmkcImg_png_bitDeep_16 = 16
} wmkcImg_png_bitDeep;

// PNG图像块
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
} wmkcImg_png_obj;

// PNG图像块（构建后）
typedef struct {
    /*
    * 这个指针是单个块的完整内容
    * 包含以下：
    * [4   Bytes] 块长度
    * [4   Bytes] 块名称
    * [... Bytes] 块内容
    * [4   Bytes] 块CRC校验值
    */
    wmkcByte *data;
    // 这个长度是单个完整块的长度
    wmkc_u32 size;
} wmkcImg_png_chunk_obj;

/**
 * @brief rgb888转换rgb565函数
 * @authors SN-Grotesque
 * @note 无
 * @param rgb RGB888结构体
 * @return wmkcImage_rgb565类型
 */
WMKC_PUBLIC(wmkcImage_rgb565_obj) wmkcImage_to_rgb565 WMKC_OPEN_API
WMKC_OF((wmkcImage_rgb888_obj rgb));

/**
 * @brief 计算PNG块数据的CRC32校验值
 * @authors SN-Grotesque
 * @note 无
 * @param chunk 指针，指向wmkcImg_png对象的地址
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcImg_png_setCrc32 WMKC_OPEN_API
WMKC_OF((wmkcImg_png_obj *chunk));

/**
 * @brief 生成IHDR块函数
 * @authors SN-Grotesque
 * @note 无
 * @param chunk 指针，指向wmkcImg_png对象指针的地址
 * @param width 图像宽度[4 Bytes]
 * @param height 图像高度[4 Bytes]
 * @param bitDeep 图像的位深[1, 2, 4, 8, 16]
 * @param colorType 图像的颜色类型[0, 2, 3, 4, 6]
 * @param compression 图像的压缩方法[0]
 * @param filter 图像的过滤器方法[0]
 * @param interlace 图像的交错方法[0, 1]
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcImg_png_setIHDR WMKC_OPEN_API
WMKC_OF((wmkcImg_png_obj **chunk,
    wmkc_u32 width, wmkc_u32 height,
    wmkcImg_png_bitDeep   bitDeep,
    wmkcImg_png_colorType colorType,
    wmkcByte compression,
    wmkcByte filter,
    wmkcByte interlace));

/**
 * @brief 构建PNG单个完整块的函数
 * @authors SN-Grotesque
 * @note 无
 * @param chunk 指针，指向wmkcImg_png_chunk对象指针的地址
 * @param png 指针，指向wmkcImg_png对象的地址
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcImg_png_buildChunk WMKC_OPEN_API
WMKC_OF((wmkcImg_png_chunk_obj **chunk, wmkcImg_png_obj *png));

#endif /* WMKC_IMG_PNG */

#endif /* WMKC_IMG */
#endif /* WMKC_SUPPORT */
