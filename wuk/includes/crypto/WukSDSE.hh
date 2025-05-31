#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <WukMemory.hh>
#include <config/WukException.hh>
#include <config/WukEndianness.hh>
#include <crypto/WukUtils.hh>

namespace wuk {
    namespace crypto {
        constexpr wU32 WUK_SDSE_KSLEN  = 64; // Key stream length
        constexpr wU32 WUK_SDSE_KEYLEN = 32; // Key length
        constexpr wU32 WUK_SDSE_IVLEN  = 8;  // Initialization vector length

        // SN Data Stream Encryption
        class LIBWUK_API SDSE {
        private:
            wU32 state[16]{0}; // Key stream state

        public:
            SDSE() = default;
            SDSE(const wByte *key, const wByte *iv, const wuk::crypto::Counter counter);
            ~SDSE();

        public:
            void keystream_init(const wByte *key, const wByte *iv, const wuk::crypto::Counter counter);
            void xcrypt(wByte *buffer, wSize length);

            const uint32_t *get_state() const noexcept {
                return this->state;
            }

            const wByte *get_keystream() const noexcept {
                return reinterpret_cast<const wByte *>(this->state);
            }

            void clean() noexcept {
                wuk::memory_secure(this->state, sizeof(this->state));
            }
        };
    }
}

#endif
