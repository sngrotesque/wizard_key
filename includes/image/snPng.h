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

SN_PRIVATE(snVoid) snPng_init_ctx(snPng_ctx *ctx)
{
    memcpy(ctx->header, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 8);
    memcpy(ctx->end, "\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82", 12);
}

SN_PRIVATE(snVoid) snPng_init_block(snPng_block *block, snByte *type, snByte *data, snSize size)
{
    
}

SN_PRIVATE(snByte *) snPng_set_pixel(sn_u32 width, sn_u32 height, snByte bitDeep, snByte colorType)
{
    static snByte width_uint8[4];
    static snByte height_uint8[4];
    static snByte content[10];
    snByte *p = content;

    snNum_pack('>', 4, &width, width_uint8);
    snNum_pack('>', 4, &height, height_uint8);

    memcpy(p, width_uint8, 4);
    memcpy(p + 4, height_uint8, 4);
    *(p + 8) = bitDeep;
    *(p + 9) = colorType;

    return p;
}

