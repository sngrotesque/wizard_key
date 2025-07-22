#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#   include <immintrin.h>

namespace wuk {
    namespace simd {
        enum class simd_size { epi8, epi16, epi32, epi64 };

        namespace sse {
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
#               ifdef __SSE4_1__
                return _mm_extract_epi32(x, idx);
#               else
                alignas(16) wU32 tmp[4];
                sse::store128(tmp, x);
                return tmp[idx];
#               endif
            }

            template <int idx>
            inline wU64 extract(s128 x, epi64_tag) {
                static_assert(idx >= 0 && idx < 2, "Index for epi64 must be in [0, 1]");
#               if defined(__SSE4_1__)
                return _mm_extract_epi64(x, idx);
#               else
                alignas(16) wU64 tmp[2];
                sse::store128(tmp, x);
                return tmp[idx];
#               endif
            }

            template <simd_size size, int idx>
            inline auto extract(s128 x) {
                return extract<idx>(x, simd_tag<size>{});
            }
        }

        namespace avx {
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
#               if defined(__SSE4_1__)
                return _mm256_extract_epi32(x, idx);
#               else
                alignas(16) wU32 tmp[4];
                store256(tmp, x);
                return tmp[idx];
#               endif
            }

            template <int idx>
            inline wU64 extract(s256 x, epi64_tag) {
                static_assert(idx >= 0 && idx < 2, "Index for epi64 must be in [0, 1]");
#               if defined(__SSE4_1__)
                return _mm256_extract_epi64(x, idx);
#               else
                alignas(16) wU64 tmp[2];
                store256(tmp, x);
                return tmp[idx];
#               endif
            }

            template <simd_size size, int idx>
            inline auto extract(s256 x) {
                return extract<idx>(x, simd_tag<size>{});
            }
        }
    }
}

#   if 0
#   if defined(WUK_PLATFORM_WINOS)
#       include <intrin.h>
#   elif defined(WUK_PLATFORM_LINUX)
#       include <cpuid.h>
#   endif

// 定义CPU特性位标志（CPUID规范）
constexpr wU32 WUK_BIT_SSE2    = (1 << 26);
constexpr wU32 WUK_BIT_SSSE3   = (1 << 9);
constexpr wU32 WUK_BIT_AVX2    = (1 << 5);
constexpr wU32 WUK_BIT_OSXSAVE = (1 << 27); // 操作系统需支持XSAVE指令

static inline bool cpu_supports(wU32 feature_bit) {
    wU32 eax{0}, ebx{0}, ecx{0}, edx{0};

    // 获取CPU特性（CPUID leaf 1和leaf 7）
#   if defined(WUK_PLATFORM_WINOS)
    int regs[4];
    __cpuid(regs, 1);          // Leaf 1: 基础特性（SSE2/SSSE3）
    eax = regs[0]; ebx = regs[1]; ecx = regs[2]; edx = regs[3];
    
    if (feature_bit == WUK_BIT_AVX2) {
        __cpuidex(regs, 7, 0);  // Leaf 7: AVX2等扩展特性
        ebx = regs[1];
    }
#   elif defined(WUK_PLATFORM_LINUX)
    __get_cpuid(1, &eax, &ebx, &ecx, &edx); // Leaf 1
    if (feature_bit == WUK_BIT_AVX2) {
        __get_cpuid_count(7, 0, &eax, &ebx, &ecx, &edx); // Leaf 7
    }
#   elif defined(WUK_PLATFORM_MACOS)
    // // macOS通过sysctl检测
    // const char *feature_name = nullptr;
    // switch (feature_bit) {
    //     case WUK_BIT_SSE2:  feature_name = "hw.optional.sse2";             break;
    //     case WUK_BIT_SSSE3: feature_name = "hw.optional.supplementalsse3"; break;
    //     case WUK_BIT_AVX2:  feature_name = "hw.optional.avx2_0";           break;
    // }

    // if (feature_name) {
    //     int flag = 0;
    //     size_t size = sizeof(flag);
    //     return sysctlbyname(feature_name, &flag, &size, nullptr, 0) == 0 && flag;
    // }
    // return false;
#   endif
    // 检查特性位（SSE2/SSSE3在edx/ecx，AVX2在ebx）
    if (feature_bit == WUK_BIT_SSE2)  return (edx & WUK_BIT_SSE2);
    if (feature_bit == WUK_BIT_SSSE3) return (ecx & WUK_BIT_SSSE3);
    if (feature_bit == WUK_BIT_AVX2)  return (ebx & WUK_BIT_AVX2) && (ecx & WUK_BIT_OSXSAVE);
    return false;
}
#   endif

#endif
