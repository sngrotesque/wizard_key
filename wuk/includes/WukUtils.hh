#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>

#include <zlib.h>
#include <lzma.h>

namespace wuk {
    w_u32 wuk_zlib_crc32(w_u32 crc, w_byte *buffer, w_ulong length);
}

#endif
