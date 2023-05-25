#ifndef WMKC_IMAGE_PNG
#define WMKC_IMAGE_PNG
#include <zlib/zlib.h>
#include <wmkc_conf.h>
#include <wmkc_struct.h>

#define PNG_HEADER "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"
#define PNG_IEND "\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82"

typedef struct wmkcPng_block {
    wmkcByte size[4];
    wmkcSize dataSize;
    wmkcByte *data;
    wmkcByte crc[4];
} wmkcPng_block;

WMKC_PUBLIC(wmkcErr_ctx) wmkcPng_new WMKC_OPEN_API
WMKC_OF((wmkcPng_block **obj));

WMKC_PUBLIC(wmkcErr_ctx) wmkcPng_free WMKC_OPEN_API
WMKC_OF((wmkcPng_block **obj));


#endif
