#ifndef __SN_IMAGE_PNG__
#define __SN_IMAGE_PNG__
#include <zlib/zlib.h>
#include <snConf.h>
#include <snNum.h>

#define PNG_HEADER "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"
#define PNG_IEND "\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82"

typedef struct {
    snByte size[4];
    snByte *data;
    snSize dataSize;
    snByte crc[4];
} snPng_ctx;

#endif
