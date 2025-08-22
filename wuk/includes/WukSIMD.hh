#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#   include <immintrin.h>

namespace wuk::simd {
    enum class simd_size { epi8, epi16, epi32, epi64 };
}

namespace wuk::simd::sse {
    using s128 = __m128i;

    inline s128 load128(const void *x)
    {
        return _mm_load_si128(reinterpret_cast<const s128 *>(x));
    }

    inline void store128(void *x, s128 y)
    {
        _mm_store_si128(reinterpret_cast<s128 *>(x), y);
    }

    inline s128 loadu128(const void *x)
    {
        return _mm_loadu_si128(reinterpret_cast<const s128 *>(x));
    }

    inline void storeu128(void *x, s128 y)
    {
        _mm_storeu_si128(reinterpret_cast<s128 *>(x), y);
    }

    inline s128 and128(s128 a, s128 b)
    {
        return _mm_and_si128(a, b);
    }

    inline s128 or128(s128 a, s128 b)
    {
        return _mm_or_si128(a, b);
    }

    inline s128 xor128(s128 a, s128 b)
    {
        return _mm_xor_si128(a, b);
    }

    template <simd_size size>
    s128 add(s128 a, s128 b);

    template<> inline s128 add<simd_size::epi8>(s128 a, s128 b)
    {
        return _mm_add_epi8(a, b);
    }

    template<> inline s128 add<simd_size::epi16>(s128 a, s128 b)
    {
        return _mm_add_epi16(a, b);
    }

    template<> inline s128 add<simd_size::epi32>(s128 a, s128 b)
    {
        return _mm_add_epi32(a, b);
    }

    template<> inline s128 add<simd_size::epi64>(s128 a, s128 b)
    {
        return _mm_add_epi64(a, b);
    }

    template <simd_size size>
    s128 sub(s128 a, s128 b);

    template<> inline s128 sub<simd_size::epi8>(s128 a, s128 b)
    {
        return _mm_sub_epi8(a, b);
    }

    template<> inline s128 sub<simd_size::epi16>(s128 a, s128 b)
    {
        return _mm_sub_epi16(a, b);
    }

    template<> inline s128 sub<simd_size::epi32>(s128 a, s128 b)
    {
        return _mm_sub_epi32(a, b);
    }

    template<> inline s128 sub<simd_size::epi64>(s128 a, s128 b)
    {
        return _mm_sub_epi64(a, b);
    }

    template <simd_size size>
    s128 shl(s128 x, wU32 n);

    template<> inline s128 shl<simd_size::epi16>(s128 x, wU32 n)
    {
        return _mm_slli_epi16(x, n);
    }

    template<> inline s128 shl<simd_size::epi32>(s128 x, wU32 n)
    {
        return _mm_slli_epi32(x, n);
    }

    template<> inline s128 shl<simd_size::epi64>(s128 x, wU32 n)
    {
        return _mm_slli_epi64(x, n);
    }

    template <simd_size size>
    s128 shr(s128 x, wU32 n);

    template<> inline s128 shr<simd_size::epi16>(s128 x, wU32 n)
    {
        return _mm_srli_epi16(x, n);
    }

    template<> inline s128 shr<simd_size::epi32>(s128 x, wU32 n)
    {
        return _mm_srli_epi32(x, n);
    }

    template<> inline s128 shr<simd_size::epi64>(s128 x, wU32 n)
    {
        return _mm_srli_epi64(x, n);
    }

    template <simd_size size>
    s128 rotl(s128 x, wU32 n);

    template<> inline s128 rotl<simd_size::epi16>(s128 x, wU32 n)
    {
        return or128(
            shl<simd_size::epi16>(x, n),
            shr<simd_size::epi16>(x, 16 - n)
        );
    }

    template<> inline s128 rotl<simd_size::epi32>(s128 x, wU32 n)
    {
        return or128(
            shl<simd_size::epi32>(x, n),
            shr<simd_size::epi32>(x, 32 - n)
        );
    }

    template<> inline s128 rotl<simd_size::epi64>(s128 x, wU32 n)
    {
        return or128(
            shl<simd_size::epi64>(x, n),
            shr<simd_size::epi64>(x, 64 - n)
        );
    }

    template <simd_size size>
    s128 rotr(s128 x, wU32 n);

    template<> inline s128 rotr<simd_size::epi16>(s128 x, wU32 n)
    {
        return or128(
            shr<simd_size::epi16>(x, n),
            shl<simd_size::epi16>(x, 16 - n)
        );
    }

    template<> inline s128 rotr<simd_size::epi32>(s128 x, wU32 n)
    {
        return or128(
            shr<simd_size::epi32>(x, n),
            shl<simd_size::epi32>(x, 32 - n)
        );
    }

    template<> inline s128 rotr<simd_size::epi64>(s128 x, wU32 n)
    {
        return or128(
            shr<simd_size::epi64>(x, n),
            shl<simd_size::epi64>(x, 64 - n)
        );
    }

    template <simd_size size>
    s128 mullo(s128 a, s128 b);

    template<> inline s128 mullo<simd_size::epi16>(s128 a, s128 b)
    {
        return _mm_mullo_epi16(a, b);
    }

    template<> inline s128 mullo<simd_size::epi32>(s128 a, s128 b)
    {
        return _mm_mullo_epi32(a, b);
    }

    template<> inline s128 mullo<simd_size::epi64>(s128 a, s128 b)
    {
        return _mm_mullo_epi64(a, b);
    }

    template <simd_size size>
    s128 mul(s128 a, s128 b);

    template<> inline s128 mul<simd_size::epi32>(s128 a, s128 b)
    {
        return _mm_mul_epi32(a, b);
    }

    template <simd_size> struct simd_tag {};
    using epi16_tag = simd_tag<simd_size::epi16>;
    using epi32_tag = simd_tag<simd_size::epi32>;
    using epi64_tag = simd_tag<simd_size::epi64>;

    template <int idx>
    inline wU16 extract(s128 x, epi16_tag) {
        static_assert(idx >= 0 && idx < 8, "Index for epi16 must be in [0, 7]");
        return _mm_extract_epi16(x, idx);
    }

    template <int idx>
    inline wU32 extract(s128 x, epi32_tag) {
        static_assert(idx >= 0 && idx < 4, "Index for epi32 must be in [0, 3]");
#       ifdef __SSE4_1__
        return _mm_extract_epi32(x, idx);
#       else
        alignas(16) wU32 tmp[4];
        sse::store128(tmp, x);
        return tmp[idx];
#       endif
    }

    template <int idx>
    inline wU64 extract(s128 x, epi64_tag) {
        static_assert(idx >= 0 && idx < 2, "Index for epi64 must be in [0, 1]");
#       if defined(__SSE4_1__)
        return _mm_extract_epi64(x, idx);
#       else
        alignas(16) wU64 tmp[2];
        sse::store128(tmp, x);
        return tmp[idx];
#       endif
    }

    template <simd_size size, int idx>
    inline auto extract(s128 x) {
        return extract<idx>(x, simd_tag<size>{});
    }
}

namespace wuk::simd::avx {
    using s256 = __m256i;

    inline s256 load256(const void *x)
    {
        return _mm256_load_si256(reinterpret_cast<const s256 *>(x));
    }

    inline void store256(void *x, s256 y)
    {
        _mm256_store_si256(reinterpret_cast<s256 *>(x), y);
    }

    inline s256 loadu256(const void *x)
    {
        return _mm256_loadu_si256(reinterpret_cast<const s256 *>(x));
    }

    inline void storeu256(void *x, s256 y)
    {
        _mm256_storeu_si256(reinterpret_cast<s256 *>(x), y);
    }

    inline s256 and256(s256 a, s256 b)
    {
        return _mm256_and_si256(a, b);
    }

    inline s256 or256(s256 a, s256 b)
    {
        return _mm256_or_si256(a, b);
    }

    inline s256 xor256(s256 a, s256 b)
    {
        return _mm256_xor_si256(a, b);
    }

    template <simd_size size>
    s256 add(s256 a, s256 b);

    template<> inline s256 add<simd_size::epi8>(s256 a, s256 b)
    {
        return _mm256_add_epi8(a, b);
    }

    template<> inline s256 add<simd_size::epi16>(s256 a, s256 b)
    {
        return _mm256_add_epi16(a, b);
    }

    template<> inline s256 add<simd_size::epi32>(s256 a, s256 b)
    {
        return _mm256_add_epi32(a, b);
    }

    template<> inline s256 add<simd_size::epi64>(s256 a, s256 b)
    {
        return _mm256_add_epi64(a, b);
    }

    template <simd_size size>
    s256 sub(s256 a, s256 b);

    template<> inline s256 sub<simd_size::epi8>(s256 a, s256 b)
    {
        return _mm256_sub_epi8(a, b);
    }

    template<> inline s256 sub<simd_size::epi16>(s256 a, s256 b)
    {
        return _mm256_sub_epi16(a, b);
    }

    template<> inline s256 sub<simd_size::epi32>(s256 a, s256 b)
    {
        return _mm256_sub_epi32(a, b);
    }

    template<> inline s256 sub<simd_size::epi64>(s256 a, s256 b)
    {
        return _mm256_sub_epi64(a, b);
    }

    template <simd_size size>
    s256 shl(s256 x, wU32 n);

    template<> inline s256 shl<simd_size::epi16>(s256 x, wU32 n)
    {
        return _mm256_slli_epi16(x, n);
    }

    template<> inline s256 shl<simd_size::epi32>(s256 x, wU32 n)
    {
        return _mm256_slli_epi32(x, n);
    }

    template<> inline s256 shl<simd_size::epi64>(s256 x, wU32 n)
    {
        return _mm256_slli_epi64(x, n);
    }

    template <simd_size size>
    s256 shr(s256 x, wU32 n);

    template<> inline s256 shr<simd_size::epi16>(s256 x, wU32 n)
    {
        return _mm256_srli_epi16(x, n);
    }

    template<> inline s256 shr<simd_size::epi32>(s256 x, wU32 n)
    {
        return _mm256_srli_epi32(x, n);
    }

    template<> inline s256 shr<simd_size::epi64>(s256 x, wU32 n)
    {
        return _mm256_srli_epi64(x, n);
    }

    template <simd_size size>
    s256 rotl(s256 x, wU32 n);

    template<> inline s256 rotl<simd_size::epi16>(s256 x, wU32 n)
    {
        return or256(
            shl<simd_size::epi16>(x, n),
            shr<simd_size::epi16>(x, 16 - n)
        );
    }

    template<> inline s256 rotl<simd_size::epi32>(s256 x, wU32 n)
    {
        return or256(
            shl<simd_size::epi32>(x, n),
            shr<simd_size::epi32>(x, 32 - n)
        );
    }

    template<> inline s256 rotl<simd_size::epi64>(s256 x, wU32 n)
    {
        return or256(
            shl<simd_size::epi64>(x, n),
            shr<simd_size::epi64>(x, 64 - n)
        );
    }

    template <simd_size size>
    s256 rotr(s256 x, wU32 n);

    template<> inline s256 rotr<simd_size::epi16>(s256 x, wU32 n)
    {
        return or256(
            shr<simd_size::epi16>(x, n),
            shl<simd_size::epi16>(x, 16 - n)
        );
    }

    template<> inline s256 rotr<simd_size::epi32>(s256 x, wU32 n)
    {
        return or256(
            shr<simd_size::epi32>(x, n),
            shl<simd_size::epi32>(x, 32 - n)
        );
    }

    template<> inline s256 rotr<simd_size::epi64>(s256 x, wU32 n)
    {
        return or256(
            shr<simd_size::epi64>(x, n),
            shl<simd_size::epi64>(x, 64 - n)
        );
    }

    template <simd_size size>
    s256 mullo(s256 a, s256 b);

    template<> inline s256 mullo<simd_size::epi16>(s256 a, s256 b)
    {
        return _mm256_mullo_epi16(a, b);
    }

    template<> inline s256 mullo<simd_size::epi32>(s256 a, s256 b)
    {
        return _mm256_mullo_epi32(a, b);
    }

    template<> inline s256 mullo<simd_size::epi64>(s256 a, s256 b)
    {
        return _mm256_mullo_epi64(a, b);
    }

    template <simd_size size>
    s256 mul(s256 a, s256 b);

    template<> inline s256 mul<simd_size::epi32>(s256 a, s256 b)
    {
        return _mm256_mul_epi32(a, b);
    }

    template <simd_size> struct simd_tag {};
    using epi16_tag = simd_tag<simd_size::epi16>;
    using epi32_tag = simd_tag<simd_size::epi32>;
    using epi64_tag = simd_tag<simd_size::epi64>;

    template <int idx>
    inline wU16 extract(s256 x, epi16_tag) {
        static_assert(idx >= 0 && idx < 8, "Index for epi16 must be in [0, 7]");
        return _mm256_extract_epi16(x, idx);
    }

    template <int idx>
    inline wU32 extract(s256 x, epi32_tag) {
        static_assert(idx >= 0 && idx < 4, "Index for epi32 must be in [0, 3]");
#       if defined(__SSE4_1__)
        return _mm256_extract_epi32(x, idx);
#       else
        alignas(16) wU32 tmp[4];
        store256(tmp, x);
        return tmp[idx];
#       endif
    }

    template <int idx>
    inline wU64 extract(s256 x, epi64_tag) {
        static_assert(idx >= 0 && idx < 2, "Index for epi64 must be in [0, 1]");
#       if defined(__SSE4_1__)
        return _mm256_extract_epi64(x, idx);
#       else
        alignas(16) wU64 tmp[2];
        store256(tmp, x);
        return tmp[idx];
#       endif
    }

    template <simd_size size, int idx>
    inline auto extract(s256 x) {
        return extract<idx>(x, simd_tag<size>{});
    }
}

#endif
