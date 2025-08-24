#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukEndianness.hh>

#ifdef LIBSODIUM_SUPPORT
#   include <sodium.h>
#endif

namespace wuk::crypto {
    inline wuk::u32 rotl32(const wuk::u32 &x, const wuk::u32 &n)
    {
        return (x << n) | (x >> (32 - n));
    }

    inline wuk::u32 rotr32(const wuk::u32 &x, const wuk::u32 &n)
    {
        return (x >> n) | (x << (32 - n));
    }

    inline wuk::byte rotl8(const wuk::u32 &x, const wuk::u32 &n)
    {
        return (x << n) | (x >> (8 - n));
    }

    inline wuk::byte rotr8(const wuk::u32 &x, const wuk::u32 &n)
    {
        return (x >> n) | (x << (8 - n));
    }

    // LOAD or PACK - Little endianness
    inline wuk::u32 load32le(const wuk::byte dst[4])
    {
        wuk::u32 w;
#       ifdef WUK_NATIVE_LE
        memcpy(&w, dst, sizeof w);
#       else
        w  = ((wuk::u32)dst[0]);
        w |= ((wuk::u32)dst[1] <<  8);
        w |= ((wuk::u32)dst[2] << 16);
        w |= ((wuk::u32)dst[3] << 24);
#       endif
        return w;
    }

    inline void pack32le(wuk::byte dst[4], wuk::u32 w)
    {
#       ifdef WUK_NATIVE_LE
        memcpy(dst, &w, sizeof w);
#       else
        dst[0] = (wuk::byte) w; w >>= 8;
        dst[1] = (wuk::byte) w; w >>= 8;
        dst[2] = (wuk::byte) w; w >>= 8;
        dst[3] = (wuk::byte) w;
#       endif
    }

    // LOAD or PACK - Big endianness
    inline wuk::u32 load32be(const wuk::byte dst[4])
    {
        wuk::u32 w;
#       ifndef WUK_NATIVE_LE
        memcpy(&w, src, sizeof w);
#       else
        w  = ((wuk::u32)dst[3]);
        w |= ((wuk::u32)dst[2] <<  8);
        w |= ((wuk::u32)dst[1] << 16);
        w |= ((wuk::u32)dst[0] << 24);
#       endif
        return w;
    }

    inline void pack32be(wuk::byte dst[4], const wuk::u32 &w)
    {
#       ifndef WUK_NATIVE_LE
        memcpy(dst, &w, sizeof w);
#       else
        dst[0] = w >> 24;
        dst[1] = w >> 16;
        dst[2] = w >> 8;
        dst[3] = w;
#       endif
    }
}

#endif
