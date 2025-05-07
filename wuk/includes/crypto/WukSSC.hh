#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <WukMemory.hh>
#include <config/WukException.hh>
#include <crypto/WukUtils.hh>

namespace wuk {
    namespace crypto {
        constexpr wU32 WUK_SSC_KSLEN    = 64; // key stream length
        constexpr wU32 WUK_SSC_KEYLEN   = 32; // key length
        constexpr wU32 WUK_SSC_IVLEN    = 8;  // Initialization vector length

        // SN Stream Cipher
        class LIBWUK_API SSC {
        private:
            wByte keystream[WUK_SSC_KSLEN]{}; // 密钥流
            wU32 *state = reinterpret_cast<wU32 *>(keystream); // 密钥流状态

        public: // temporary public
            inline void keystream_update() noexcept;          // 密钥流更新

        public:
            SSC() = default;
            SSC(const wByte *key, const wByte *iv, const wuk::crypto::Counter counter);

        public:
            void keystream_init(const wByte *key, const wByte *iv, const wuk::crypto::Counter counter);
            void xcrypt(wByte *buffer, wSize length);
            const wByte *get_keystream() const;
        };
    }
}

#endif
