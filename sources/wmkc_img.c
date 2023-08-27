#include <wmkc_img.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcImg_rgba_new WMKC_OPEN_API
WMKC_OF((wmkcImg_rgba_obj **rgb, wmkc_u32 width, wmkc_u32 height))
{
    wmkcErr_obj error;
    if(!rgb || !width || !height) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcImg_rgba_new",
            "rgb or width or height is NULL.");
    }

    if(!wmkcMem_new(wmkcImg_rgba_obj *, (*rgb), sizeof(wmkcImg_rgba_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcImg_rgba_new",
            "Failed to allocate memory for (*rgb).");
    }
    (*rgb)->width = width;
    (*rgb)->height = height;
    (*rgb)->size = (width * height);
    wmkcSize memory_size = sizeof(wmkcImg_rgba) * (*rgb)->size;

    if(!wmkcMem_new(wmkcImg_rgba *, (*rgb)->rgb, memory_size)) {
        wmkcMem_free((*rgb));
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcImg_rgba_new",
            "Failed to allocate memory for (*rgb)->rgb.");
    }
    wmkcMem_zero((*rgb)->rgb, memory_size);

    wmkcErr_return(error, wmkcErr_OK, "wmkcImg_rgba_new", "OK.");
}

WMKC_PUBLIC(wmkcVoid) wmkcImg_rgba_set_pixel WMKC_OPEN_API
WMKC_OF((wmkcImg_rgba_obj *rgb, wmkc_u32 pos_x, wmkc_u32 pos_y,
    wmkcByte r, wmkcByte g, wmkcByte b, wmkcByte a))
{
    (rgb->rgb + (pos_y * rgb->width + pos_x))->r = r;
    (rgb->rgb + (pos_y * rgb->width + pos_x))->g = g;
    (rgb->rgb + (pos_y * rgb->width + pos_x))->b = b;
    (rgb->rgb + (pos_y * rgb->width + pos_x))->a = a;
}

WMKC_PUBLIC(wmkcImg_rgb565) wmkcImg_rgba_to_rgb565 WMKC_OPEN_API
WMKC_OF((wmkcImg_rgba rgb))
{
    wmkcImg_rgb565 rgb565 = {rgb.r >> 3, rgb.g >> 2, rgb.b >> 3};
    return rgb565;
}

#if defined(WMKC_IMG_CUSTOMIZED_PNG) && (WMKC_IMG_CUSTOMIZED_PNG == true)
WMKC_PUBLIC(wmkcErr_obj) wmkcImg_png_chunk_free WMKC_OPEN_API
WMKC_OF((wmkcImg_png_chunk_obj **chunk))
{
    wmkcErr_obj error;
    if(!chunk) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcImg_png_chunk_free", "chunk is NULL.");
    }
    if((*chunk)->data) {
        wmkcMem_free((*chunk)->data);
    }
    wmkcMem_free((*chunk));
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcImg_png_free WMKC_OPEN_API
WMKC_OF((wmkcImg_png_obj **chunk))
{
    wmkcErr_obj error;
    if(!chunk) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcImg_png_free", "chunk is NULL.");
    }
    if((*chunk)->data) {
        wmkcMem_free((*chunk)->data);
    }
    wmkcMem_free((*chunk));
    wmkcErr_return(error, wmkcErr_OK, "wmkcImg_png_free", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcImg_png_setCrc32 WMKC_OPEN_API
WMKC_OF((wmkcImg_png_obj *chunk))
{
    /*
    * 此函数用于计算出[块名称][块内容]的CRC值
    */
    wmkcErr_obj error;
    if(!chunk || !chunk->data) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcImg_png_setCrc32",
            "chunk or chunk->data is NULL.");
    }
    wmkc_u32 chunk_total_size = sizeof(chunk->name) + chunk->size_n;
    wmkcByte *chunk_total = wmkcNull;

    if(!wmkcMem_new(wmkcByte *, chunk_total, chunk_total_size)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcImg_png_setCrc32",
            "Failed to allocate memory for (*chunk).");
    }

    memcpy(chunk_total, chunk->name, sizeof(chunk->name));
    memcpy(chunk_total + 4, chunk->data, chunk->size_n);

    chunk->crc_n = crc32(0, chunk_total, chunk_total_size);
    wmkcStruct_pack(">I", chunk->crc, chunk->crc_n);

    wmkcMem_free(chunk_total);
    wmkcErr_return(error, wmkcErr_OK, "wmkcImg_png_setCrc32", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcImg_png_setIHDR WMKC_OPEN_API
WMKC_OF((wmkcImg_png_obj **chunk,
    wmkc_u32 width, wmkc_u32 height,
    wmkcImg_png_bitDepth  bitDeep,
    wmkcImg_png_colorType colorType,
    wmkcByte compression,
    wmkcByte filter,
    wmkcByte interlace))
{
    wmkcErr_obj error;
    if(!chunk || !width || !height) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcImg_png_setIHDR",
            "chunk or width or height is NULL.");
    }

    // 为chunk对象申请内存空间和初始化
    if(!wmkcMem_new(wmkcImg_png_obj *, (*chunk), sizeof(wmkcImg_png_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcImg_png_setIHDR",
            "Failed to allocate memory for (*chunk).");
    }
    wmkcMem_zero((*chunk), sizeof(wmkcImg_png_obj));

    // 为chunk对象的块申请内存空间
    if(!wmkcMem_new(wmkcByte *, (*chunk)->data, ((*chunk)->size_n = 0x0d))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcImg_png_setIHDR",
            "Failed to allocate memory for (*chunk)->data.");
    }
    wmkcMem_zero((*chunk)->data, (*chunk)->size_n);

    // 设置块内容
    wmkcStruct_pack(">I", (*chunk)->size, (*chunk)->size_n); // 块长度
    memcpy((*chunk)->name, "IHDR", sizeof((*chunk)->name));  // 块名称
    wmkcStruct_pack(">I", (*chunk)->data,       width);  // 宽度
    wmkcStruct_pack(">I", (*chunk)->data + 0x4, height); // 高度
    *((*chunk)->data + 0x8) = bitDeep;     // 位深度
    *((*chunk)->data + 0x9) = colorType;   // 颜色类型
    *((*chunk)->data + 0xa) = compression; // 压缩方法
    *((*chunk)->data + 0xb) = filter;      // 过滤方法
    *((*chunk)->data + 0xc) = interlace;   // 交错方法

    wmkcImg_png_setCrc32((*chunk)); // 获取CRC32校验值

    wmkcErr_return(error, wmkcErr_OK, "wmkcImg_png_setIHDR", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcImg_png_buildChunk WMKC_OPEN_API
WMKC_OF((wmkcImg_png_chunk_obj **chunk, wmkcImg_png_obj *png))
{
    wmkcErr_obj error;
    if(!chunk || !png) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcImg_png_buildChunk",
            "chunk or png is NULL.");
    }

    if(!wmkcMem_new(wmkcImg_png_chunk_obj *, (*chunk), sizeof(wmkcImg_png_chunk_obj))) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcImg_png_buildChunk",
            "Failed to allocate memory for (*chunk).");
    }

    // 计算单个块的完整长度
    // Size[4 Bytes] + Name[4 Bytes] + Data[... Bytes] + CRC[4 Bytes]
    (*chunk)->size = 4 + 4 + png->size_n + 4;
    if(!wmkcMem_new(wmkcByte *, (*chunk)->data, (*chunk)->size)) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcImg_png_buildChunk",
            "Failed to allocate memory for (*chunk)->data.");
    }

    memcpy((*chunk)->data,                   png->size, sizeof(png->size));
    memcpy((*chunk)->data + 4,               png->name, sizeof(png->name));
    memcpy((*chunk)->data + 8,               png->data, png->size_n);
    memcpy((*chunk)->data + 8 + png->size_n, png->crc,  sizeof(png->crc));

    wmkcErr_return(error, wmkcErr_OK, "wmkcImg_png_buildChunk", "OK.");
}
#endif /* WMKC_IMG_CUSTOMIZED_PNG */
