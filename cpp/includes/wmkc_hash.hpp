#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_HASH
#define WMKC_CPP_HASH
#include <wmkc_exception.hpp>
#include <wmkc_binascii.hpp>
#include <wmkc_memory.hpp>
#include <openssl/evp.h>

class wmkcHash {
    private: EVP_MD_CTX *md_ctx;
    private: const EVP_MD *md;

    public: wmkcByte digest[64 + 1]; // SHA-512
    public: wmkcByte hexdigest[(64 << 1) + 1];
};

#endif
#endif
