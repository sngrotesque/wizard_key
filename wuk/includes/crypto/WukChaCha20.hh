#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <assert.h>

namespace wuk {
    namespace crypto {
        class LIBWUK_API ChaCha20 {
        private:
            wByte key[32];
            wByte nonce[12];

        public:
            wU32 state[16];
            wU32 counter;
        public:
            ChaCha20(const wByte *key, const wByte *nonce, wU32 counter = 0);
            void init();
            void xcrypt(wByte *buffer, wSize length);
        };
    }
}

#endif
