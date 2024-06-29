#include <config/wuk.hpp>

#if WUK_SUPPORT
#ifndef WUK_CPP_HASH
#define WUK_CPP_HASH
#include <config/exception.hpp>
#include <binascii.hpp>
#include <memory.hpp>
#include <openssl/evp.h>

namespace wuk {
    class LIBWUK_API Hashlib {
        private:
            EVP_MD_CTX *md_ctx;
            const EVP_MD *md;

        public:
            wByte digest[64 + 1]; // SHA-512
            wByte hexdigest[(64 << 1) + 1];
    };
}

#endif
#endif
