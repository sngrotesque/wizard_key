// Referer: https://github.com/marcizhu/ChaCha20
#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_CHACHA20
#define WMKC_CPP_CHACHA20
#include <assert.h>

namespace wmkc {
    namespace crypto {
        class ChaCha20 {
        private:
        typedef wByte _key256[32];
        typedef wByte _nonce96[12];
            wU32 state[16];
            _key256 key;
            _nonce96 nonce;
            wU32 counter;
        public:
            ChaCha20(wByte *key, wByte *nonce, wU32 counter = 0);
            wVoid init();
            wVoid xcrypt(wByte *buffer, wSize length);
        };
    }
}

#endif
#endif
