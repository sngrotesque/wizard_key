#include <crypto/WukFEA.hh>
#include <crypto/WukSSC.hh>
#include <WukBuffer.hh>

#include <openssl/evp.h>

#include <iostream>
#include <iomanip>

enum class CipherType {
    FEA, SSC
};

class CryptoTest {
private:
    wuk::Buffer sha256(wuk::Buffer buffer)
    {
        wByte digest[32]{};

        const EVP_MD *md = EVP_sha256();
        EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();

        EVP_DigestInit_ex(md_ctx, md, nullptr);
        EVP_DigestUpdate(md_ctx, buffer.get_data(), buffer.get_length());
        EVP_DigestFinal_ex(md_ctx, digest, nullptr);
        EVP_MD_CTX_free(md_ctx);

        return wuk::Buffer(digest, sizeof(digest));
    }

    wuk::Buffer get_key(std::string password)
    {
        wuk::Buffer pass_salt = sha256(password);
        wuk::Buffer result;

        PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), pass_salt.get_data(), pass_salt.get_length(), 501001, EVP_sha256(), 128, result.append_write(128));

        return result;
    }
public:

};

int main()
{
    
}
