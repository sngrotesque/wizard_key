#include <wmkc_basic.hpp>

wmkcByte wmkcBasic_byteSwap(wmkcByte x)
{
    return ((x & 0xf) << 4) ^ (x >> 4);
}

wmkc_u16 wmkcBasic_wordSwap(wmkc_u16 x)
{
    return ((x & 0xf) << 12) | ((x & 0xf0) << 4) | ((x & 0xf00) >> 4) | (x >> 12);
}

wmkc_u32 wmkcBasic_longSwap(wmkc_u32 x)
{
    return \
        ((x & 0xf) << 28) | ((x & 0xf0) << 20) | ((x & 0xf00) << 12) | ((x & 0xf000) << 4) |
        ((x >> 4) & 0xf000) | ((x >> 12) & 0xf00) | ((x >> 20) & 0xf0) | (x >> 28);
}
