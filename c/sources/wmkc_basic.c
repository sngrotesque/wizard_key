#include <wmkc_basic.h>

WMKC_PUBLIC(wmkcByte) wmkcBasic_byteSwap WMKC_OPEN_API
WMKC_OF((wmkcByte x))
{
    return ((x & 0xf) << 4) ^ (x >> 4);
}

WMKC_PUBLIC(wmkc_u16) wmkcBasic_wordSwap WMKC_OPEN_API
WMKC_OF((wmkc_u16 x))
{
    return ((x & 0xf) << 12) | ((x & 0xf0) << 4) | ((x & 0xf00) >> 4) | (x >> 12);
}

WMKC_PUBLIC(wmkc_u32) wmkcBasic_longSwap WMKC_OPEN_API
WMKC_OF((wmkc_u32 x))
{
    return \
        ((x & 0xf) << 28) | ((x & 0xf0) << 20) | ((x & 0xf00) << 12) | ((x & 0xf000) << 4) |
        ((x >> 4) & 0xf000) | ((x >> 12) & 0xf00) | ((x >> 20) & 0xf0) | (x >> 28);
}

