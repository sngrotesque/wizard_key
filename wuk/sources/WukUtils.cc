#include <WukUtils.hh>

w_u32 wuk::wuk_zlib_crc32(w_u32 crc, w_byte *buffer, w_ulong length)
{
    return crc32(crc, buffer, length);
}



