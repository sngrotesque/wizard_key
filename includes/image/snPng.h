#ifndef __SN_IMAGE_PNG__
#define __SN_IMAGE_PNG__
#include <zlib/zlib.h>
#include <snConf.h>
#include <snNum.h>

#define PNG_HEADER "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"
#define PNG_IEND "\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82"

typedef struct snPng_block {
    wmkcByte size[4];
    wmkcSize dataSize;
    wmkcByte *data;
    wmkcByte crc[4];
} snPng_block;

WMKC_PUBLIC(snErr_ctx) snPng_new WMKC_OPEN_API
WMKC_OF((snPng_block **obj));

WMKC_PUBLIC(snErr_ctx) snPng_free WMKC_OPEN_API
WMKC_OF((snPng_block **obj));


#endif
