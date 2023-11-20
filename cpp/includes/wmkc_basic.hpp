#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_BASIC
#define WMKC_CPP_BASIC

wmkcByte wmkcBasic_byteSwap(wmkcByte x);
wmkc_u16 wmkcBasic_wordSwap(wmkc_u16 x);
wmkc_u32 wmkcBasic_longSwap(wmkc_u32 x);

#endif /* WMKC_BASIC */
#endif /* WMKC_SUPPORT */
