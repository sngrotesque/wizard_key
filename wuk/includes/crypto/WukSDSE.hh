#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <WukMemory.hh>
#include <config/WukException.hh>
#include <config/WukEndianness.hh>

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
            SDSE(const wByte key[32], const wByte nonce[20], wU32 counter = 0);
            ~SDSE();

        public:
            void init(const wByte key[32], const wByte nonce[20], wU32 counter = 0);
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
