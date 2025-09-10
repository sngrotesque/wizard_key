/**
 * @file WukOP4Easy.hh
 * @author SN-Grotesque
 * @brief 此为OP4加密算法的超简易实现
 * @note 因其不存在密钥并且削弱了轮数，你不应该将其视为一个加密算法，而
 *       应该将它视为一个特殊的编码算法（类似于摩斯密码或凯撒密码）。
 *       因为凯撒密码和摩斯密码的特点就是一旦算法泄漏所有秘密性也
 *       将不复存在，这在现代密码学中是不被允许的。
 */
#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <atomic>

/* 此算法不具备任何意义上的安全性，所有因使用它而导致的安全问题由使用者自己承担。 */
namespace wuk::crypto {
    constexpr wuk::u32 OP4_BL  = 16; // Block length
    constexpr wuk::u32 OP4_NR  = 4;  // Number of rounds

    class LIBWUK_API OP4Easy {
    public:
        OP4Easy() = default;

    public:
        bool crypto_stream(wuk::byte *out, const wuk::byte *in, wuk::ulong length);
    };
}

#endif
