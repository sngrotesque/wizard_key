#include <zlib/zlib.h>
#include <snConf.h>
#include <snNum.h>

typedef struct {
    snByte size[4]; // 当前块长度
    snByte type[4]; // 当前块类型，比如IHDR
    snByte *data;   // 当前块的数据
    snByte crc[4];  // 当前块的CRC校验值
    sn_u32 crcNum;  // 用于结构体对齐且用于数字型CRC校验值
} snPng_block;

typedef struct {
    snByte header[8]; // 文件头
    snPng_block *block; // 文件中的块的内容，不要直接使用，用于指向被定义的snPng_block类型变量
    snByte end[12]; // 文件尾
} snPng_ctx;

SN_PUBLIC(snVoid) snPng_init_ctx SN_OPEN_API
SN_FUNC_OF((snPng_ctx *ctx))
{
    memcpy(ctx->header, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 8);
    memcpy(ctx->end, "\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82", 12);
}

SN_PUBLIC(snVoid) snPng_write_block SN_OPEN_API
SN_FUNC_OF((snPng_block *block))
{

}

SN_PUBLIC(snVoid) snPng_set_IHDR SN_OPEN_API
SN_FUNC_OF((snPng_block *block, sn_u32 width, sn_u32 height, snByte bitDeep, snByte colorType))
{
    static snByte width_buffer[4];
    static snByte height_buffer[4];
    static snByte swap;

    memcpy(block->size, "\x00\x00\x00\x0d", 4);
    memcpy(block->type, "IHDR", 4);

    memcpy(width_buffer, &width, 4);
    memcpy(height_buffer, &height, 4);

    if(snNum_PlatformEnd()) {
        swap = width_buffer[0];
        width_buffer[0] = width_buffer[3];
        width_buffer[3] = swap;
        swap = width_buffer[1];
        width_buffer[1] = width_buffer[2];
        width_buffer[2] = swap;

        swap = height_buffer[0];
        height_buffer[0] = height_buffer[3];
        height_buffer[3] = swap;
        swap = height_buffer[1];
        height_buffer[1] = height_buffer[2];
        height_buffer[2] = swap;
    }

    memcpy(block->data, width_buffer, 4);
    memcpy(block->data + 4, height_buffer, 4);
    memcpy(block->data + 8, &bitDeep, 1);
    memcpy(block->data + 9, &colorType, 1);
    memcpy(block->data + 10, "\x00\x00\x00", 3);
}

