#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <core/WukLIBSSL.hh>
#include <WukBuffer.hh>

namespace wuk::crypto {
    enum class HashlibType {
        MD5, SHA_1,

        SHA_224,  SHA_256,  SHA_384,  SHA_512,
        SHA3_224, SHA3_256, SHA3_384, SHA3_512
    };

    template <HashlibType T> const EVP_MD* get_EVP_md();

    template<> const EVP_MD* get_EVP_md<HashlibType::MD5>    () { return EVP_md5();    }
    template<> const EVP_MD* get_EVP_md<HashlibType::SHA_1>  () { return EVP_sha1();   }

    template<> const EVP_MD* get_EVP_md<HashlibType::SHA_224>() { return EVP_sha224(); }
    template<> const EVP_MD* get_EVP_md<HashlibType::SHA_256>() { return EVP_sha256(); }
    template<> const EVP_MD* get_EVP_md<HashlibType::SHA_384>() { return EVP_sha384(); }
    template<> const EVP_MD* get_EVP_md<HashlibType::SHA_512>() { return EVP_sha512(); }

    template<> const EVP_MD* get_EVP_md<HashlibType::SHA3_224>() { return EVP_sha3_224(); }
    template<> const EVP_MD* get_EVP_md<HashlibType::SHA3_256>() { return EVP_sha3_256(); }
    template<> const EVP_MD* get_EVP_md<HashlibType::SHA3_384>() { return EVP_sha3_384(); }
    template<> const EVP_MD* get_EVP_md<HashlibType::SHA3_512>() { return EVP_sha3_512(); }

    template <HashlibType T>
    class LIBWUK_API Hashlib {
    private:
        EVP_MD_CTX  *ctx = nullptr;
        const EVP_MD *md = get_EVP_md<T>();

    private:
        void init_ctx()
        {
            this->ctx = EVP_MD_CTX_new();
        }

        void init_md()
        {
            this->md = get_EVP_md<T>();
        }

        void free_ctx()
        {
            EVP_MD_CTX_free(this->ctx);
        }

    public:
        Hashlib()
        {
            static_assert(!(T == HashlibType::MD5 || T == HashlibType::SHA_1),
                "Insecure hash algorithm (MD5/SHA-1), use SHA-2 or SHA-3 instead");
            this->init_ctx();
            this->init_md();

            EVP_DigestInit_ex(this->ctx, this->md, nullptr);
        }

        ~Hashlib()
        {
            this->free_ctx();
        }

        void update(const wuk::byte *buffer, wuk::ulong length)
        {
            EVP_DigestUpdate(this->ctx, buffer, length);
        }

        const wuk::Buffer digest() const noexcept
        {
            wuk::u32 size = EVP_MD_size(this->md);
            wuk::Buffer result{size};

            EVP_DigestFinal_ex(this->ctx, result.append_write(size), nullptr);

            return result;
        }

        const std::string hexdigest() const noexcept
        {
            return this->digest().to_hex();
        }

        HashlibType what_type() const noexcept
        {
            return T;
        }
    };
}

#endif
