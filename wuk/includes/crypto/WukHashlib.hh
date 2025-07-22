#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukBinascii.hh>
#include <WukMemory.hh>

#include <openssl/evp.h>
#include <openssl/err.h>

namespace wuk {
    namespace crypto {
        class LIBWUK_API Hashlib {
        private:
            EVP_MD_CTX *md_ctx = nullptr;

            void init_ctx(EVP_MD *type, ENGINE *impl = nullptr);

        public:
            Hashlib();
            ~Hashlib();

        };
    }
}

#endif
