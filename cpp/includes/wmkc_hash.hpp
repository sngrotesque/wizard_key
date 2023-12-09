#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_HASH
#define WMKC_CPP_HASH
#include <wmkc_exception.hpp>
#include <wmkc_binascii.hpp>
#include <wmkc_memory.hpp>
#include <openssl/evp.h>

class wmkcHash {
    public: class base_hash {
        public: base_hash(const EVP_MD *_md);
        public: virtual void update(wmkcByte *content, wmkcSize size);
        public: virtual void update(std::string content);
    };

    private: EVP_MD_CTX *md_ctx;
    private: const EVP_MD *md;

    public: std::string digest;
    public: std::string hexdigest;

    public: class sha256 :public base_hash {
        sha256(const EVP_MD *md)
        : base_hash(md)
        {
            
        }
    };

};

#endif
#endif
