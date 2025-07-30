#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukEndianness.hh>

#ifdef LIBSODIUM_SUPPORT
#   include <sodium.h>
#endif

namespace wuk {
    namespace crypto {
        inline wU32 rotl32(const wU32 &x, const wU32 &n)
        {
            return (x << n) | (x >> (32 - n));
        }

        inline wU32 rotr32(const wU32 &x, const wU32 &n)
        {
            return (x >> n) | (x << (32 - n));
        }

        inline wByte rotl8(const wU32 &x, const wU32 &n)
        {
            return (x << n) | (x >> (8 - n));
        }

        inline wByte rotr8(const wU32 &x, const wU32 &n)
        {
            return (x >> n) | (x << (8 - n));
        }

        // LOAD or PACK - Little endianness
        inline wU32 load32le(const wByte dst[4])
        {
            wU32 w;
#           ifdef WUK_NATIVE_LE
            memcpy(&w, dst, sizeof w);
#           else
            w  = ((wU32)dst[0]);
            w |= ((wU32)dst[1] <<  8);
            w |= ((wU32)dst[2] << 16);
            w |= ((wU32)dst[3] << 24);
#           endif
            return w;
        }

        inline void pack32le(wByte dst[4], wU32 w)
        {
#           ifdef WUK_NATIVE_LE
            memcpy(dst, &w, sizeof w);
#           else
            dst[0] = (wByte) w; w >>= 8;
            dst[1] = (wByte) w; w >>= 8;
            dst[2] = (wByte) w; w >>= 8;
            dst[3] = (wByte) w;
#           endif
        }

        // LOAD or PACK - Big endianness
        inline wU32 load32be(const wByte dst[4])
        {
            wU32 w;
#           ifndef WUK_NATIVE_LE
            memcpy(&w, src, sizeof w);
#           else
            w  = ((wU32)dst[3]);
            w |= ((wU32)dst[2] <<  8);
            w |= ((wU32)dst[1] << 16);
            w |= ((wU32)dst[0] << 24);
#           endif
            return w;
        }

        inline void pack32be(wByte dst[4], const wU32 &w)
        {
#           ifndef WUK_NATIVE_LE
            memcpy(dst, &w, sizeof w);
#           else
            dst[0] = w >> 24;
            dst[1] = w >> 16;
            dst[2] = w >> 8;
            dst[3] = w;
#           endif
        }
    }
}

#endif
