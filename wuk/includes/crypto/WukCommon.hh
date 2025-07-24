#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukEndianness.hh>

static inline wU32 rotl32(const wU32 &x, const wU32 &n)
{
    return (x << n) | (x >> (32 - n));
}

static inline wU32 rotr32(const wU32 &x, const wU32 &n)
{
    return (x >> n) | (x << (32 - n));
}

static inline wByte rotl8(const wU32 &x, const wU32 &n)
{
    return (x << n) | (x >> (8 - n));
}

static inline wByte rotr8(const wU32 &x, const wU32 &n)
{
    return (x >> n) | (x << (8 - n));
}

// LOAD or PACK - Little endianness
static inline wU32 load32le(const wByte dst[4])
{
    wU32 w;
#   ifdef WUK_NATIVE_LE
    memcpy(&w, dst, sizeof w);
#   else
    w  = ((wU32)dst[0]);
    w |= ((wU32)dst[1] <<  8);
    w |= ((wU32)dst[2] << 16);
    w |= ((wU32)dst[3] << 24);
#   endif
    return w;
}

static inline void pack32le(wByte dst[4], wU32 w)
{
#   ifdef WUK_NATIVE_LE
    memcpy(dst, &w, sizeof w);
#   else
    dst[0] = (wByte) w; w >>= 8;
    dst[1] = (wByte) w; w >>= 8;
    dst[2] = (wByte) w; w >>= 8;
    dst[3] = (wByte) w;
#   endif
}

// LOAD or PACK - Big endianness
static inline wU32 load32be(const wByte dst[4])
{
#   ifndef WUK_NATIVE_LE
    memcpy(&w, src, sizeof w);
#   else
    return static_cast<wU32>(dst[0]) << 24 |
           static_cast<wU32>(dst[1]) << 16 |
           static_cast<wU32>(dst[2]) <<  8 |
           static_cast<wU32>(dst[3]);
#   endif
}

static inline wU32 load32be(const wByte &a, const wByte &b, const wByte &c, const wByte &d)
{
    return static_cast<wU32>(a) << 24 |
           static_cast<wU32>(b) << 16 |
           static_cast<wU32>(c) <<  8 |
           static_cast<wU32>(d);
}

static inline void pack32be(wByte dst[4], const wU32 &w)
{
#   ifndef WUK_NATIVE_LE
    memcpy(dst, &w, sizeof w);
#   else
    dst[0] = w >> 24;
    dst[1] = w >> 16;
    dst[2] = w >> 8;
    dst[3] = w;
#   endif
}

static inline void pack32be(wByte &a, wByte &b, wByte &c, wByte &d, const wU32 &w)
{
    a = w >> 24;
    b = w >> 16;
    c = w >> 8;
    d = w;
}

#endif
