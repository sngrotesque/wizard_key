#include <wmkc_image.h>

/**
 * @brief 检查PNG图像的位深设置值是否正确
 * @authors SN-Grotesque
 * @note PNG图像位深值只接受[0, 2, 3, 4, 6]
 * @param x 位深值
 * @return True: 设置正确，否则错误
 */
WMKC_PRIVATE(wmkcBool) wmkcImage_png_check_bitDeep
WMKC_OF((wmkcByte x))
{
    // 1:  1 << 0
    // 4:  1 << 2
    // 8:  1 << 3
    // 16: 1 << 4
    // 64: 1 << 6
    return ((1 | 4 | 8 | 16 | 64) & (1 << x)) != 0;
}

/**
 * @brief 检查PNG图像的颜色类型设置值是否正确
 * @authors SN-Grotesque
 * @note PNG图像颜色类型值只接受[1, 2, 4, 8, 16]
 * @param x 颜色类型值
 * @return True: 设置正确，否则错误
 */
WMKC_PRIVATE(wmkcBool) wmkcImage_png_check_colorType
WMKC_OF((wmkcByte x))
{
    // 2:     1 << 1
    // 4:     1 << 2
    // 16:    1 << 4
    // 256:   1 << 8
    // 65536: 1 << 16
    return ((2 | 4 | 16 | 256 | 65536) & (1 << x)) != 0;
}

/**
 * @brief 检查PNG图像的压缩方法设置值是否正确
 * @authors SN-Grotesque
 * @note PNG图像压缩方法值只接受[0]
 * @param x 压缩方法值
 * @return True: 设置正确，否则错误
 */
WMKC_PRIVATE(wmkcBool) wmkcImage_png_check_compression
WMKC_OF((wmkcByte x))
{
    return x == 0;
}

/**
 * @brief 检查PNG图像的过滤器方法设置值是否正确
 * @authors SN-Grotesque
 * @note PNG图像过滤器方法值只接受[0]
 * @param x 过滤器方法值
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PRIVATE(wmkcBool) wmkcImage_png_check_filter
WMKC_OF((wmkcByte x))
{
    return x == 0;
}

/**
 * @brief 检查PNG图像的交错方法设置值是否正确
 * @authors SN-Grotesque
 * @note PNG图像交错方法值只接受[0, 1]
 * @param x 交错方法值
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PRIVATE(wmkcBool) wmkcImage_png_check_interlace
WMKC_OF((wmkcByte x))
{
    return ((1 | 2) & (1 << x)) != 0;
}

WMKC_PUBLIC(wmkcImage_rgb565) rgb888_to_rgb565 WMKC_OPEN_API
WMKC_OF((wmkcImage_rgb888 rgb))
{
    wmkcImage_rgb565 rgb565 = {.r = rgb.r >> 3, .g = rgb.g >> 2, .b = rgb.b >> 3};
    return rgb565;
}

WMKC_PUBLIC(wmkcErr_obj) wmkcImage_png_init WMKC_OPEN_API
WMKC_OF((wmkcImage_png_obj *block))
{
    wmkcErr_obj error;
    if(!block) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcImage_png_crc32: block is NULL.");
    }
    wmkcMem_zero(block, sizeof(wmkcImage_png_obj));
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcImage_png_crc32 WMKC_OPEN_API
WMKC_OF((wmkcImage_png_obj *block))
{
    wmkcErr_obj error;
    if(!block || !block->data) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcImage_png_crc32: "
            "block or block->data is NULL.");
    }
    wmkc_u32 block_total_size = sizeof(block->name) + block->size_n;
    wmkcByte *block_total = wmkcNull;

    if(!wmkcMem_new(wmkcByte *, block_total, block_total_size)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcImage_png_crc32: "
            "Failed to allocate memory for (*IHDR).");
    }

    memcpy(block_total, block->name, sizeof(block->name));
    memcpy(block_total + 4, block->data, block->size_n);

    block->crc_n = crc32(0, block_total, block_total_size);
    wmkcStruct_pack(">I", block->crc, block->crc_n);

    wmkcMem_free(block_total);
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcImage_png_IHDR WMKC_OPEN_API
WMKC_OF((wmkcImage_png_obj **IHDR,
    wmkc_u16 width,   wmkc_u16 height,
    wmkcByte bitDeep, wmkcByte colorType,
    wmkcByte compression, wmkcByte filter, wmkcByte interlace))
{
    wmkcErr_obj error;
    if(!IHDR || !width || !height) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcImage_png_IHDR: "
            "IHDR or width or height is NULL.");
    }
    if(!wmkcImage_png_check_bitDeep(bitDeep)) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcImage_png_IHDR: "
            "Incorrect bit deep, can only be  [0, 2, 3, 4, 6].");
    }
    if(!wmkcImage_png_check_colorType(colorType)) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcImage_png_IHDR: "
            "Incorrect color type, can only be [1, 2, 4, 8, 16].");
    }
    if(!wmkcImage_png_check_compression(compression)) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcImage_png_IHDR: "
            "Incorrect compression method, can only be [0].");
    }
    if(!wmkcImage_png_check_filter(filter)) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcImage_png_IHDR: "
            "Incorrect filter method, can only be [0].");
    }
    if(!wmkcImage_png_check_interlace(interlace)) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcImage_png_IHDR: "
            "Incorrect interlace method, can only be [0, 1].");
    }

    // 为IHDR对象申请内存空间和初始化
    if(!wmkcMem_new(wmkcImage_png_obj *, (*IHDR), sizeof(wmkcImage_png_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcImage_png_IHDR: "
            "Failed to allocate memory for (*IHDR).");
    }
    wmkcImage_png_init((*IHDR));

    // 为IHDR对象的块申请内存空间
    if(!wmkcMem_new(wmkcByte *, (*IHDR)->data, ((*IHDR)->size_n = 0x0d))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcImage_png_IHDR: "
            "Failed to allocate memory for (*IHDR)->data.");
    }
    wmkcMem_zero((*IHDR)->data, (*IHDR)->size_n);

    // 设置块内容
    wmkcStruct_pack(">I", (*IHDR)->size, (*IHDR)->size_n); // 块长度
    memcpy((*IHDR)->name, "IHDR", sizeof((*IHDR)->name));  // 块名称
    wmkcStruct_pack(">I", (*IHDR)->data,       width);  // 宽度
    wmkcStruct_pack(">I", (*IHDR)->data + 0x4, height); // 高度
    *((*IHDR)->data + 0x8) = colorType;   // 颜色类型
    *((*IHDR)->data + 0x9) = bitDeep;     // 位深度
    *((*IHDR)->data + 0xa) = compression; // 压缩方法
    *((*IHDR)->data + 0xb) = filter;      // 过滤方法
    *((*IHDR)->data + 0xc) = interlace;   // 交错方法

    wmkcImage_png_crc32((*IHDR)); // 获取CRC32校验值

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
