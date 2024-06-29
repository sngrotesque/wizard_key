#include <config/wuk.hpp>

#if WUK_SUPPORT
#ifndef WUK_CPP_CHACHA20
#define WUK_CPP_CHACHA20
#include <assert.h>

namespace wuk {
    namespace crypto {
        class LIBWUK_API ChaCha20 {
        private:
            typedef wByte _key256[32];
            typedef wByte _nonce96[12];
            wU32 state[16];
            _key256 key;
            _nonce96 nonce;
            wU32 counter;
        public:
            ChaCha20(wByte *key, wByte *nonce, wU32 counter = 0);
            void init();
            void xcrypt(wByte *buffer, wSize length);
        };
    }
}

#endif
#endif
