// python make.py test\main.cc -DWUK_EXPORTS -lws2_32 -lz -lssl -lcrypto
#include <config/WukConfig.hh>

#include <network/WukSocket.cc>
#include <network/WukPacket.cc>

#include <crypto/WukFEA.cc>
#include <crypto/WukFEA_ECB.cc>
#include <crypto/WukFEA_CBC.cc>
#include <crypto/WukFEA_CTR.cc>
#include <crypto/WukFEA_CFB.cc>
#include <crypto/WukUtils.cc>

#include <WukBase64.cc>
#include <WukBinascii.cc>
#include <WukPadding.cc>
#include <WukBuffer.cc>
#include <WukRandom.cc>
#include <WukMisc.cc>
#include <WukTime.cc>

#include <iostream>
#include <iomanip>
#include <memory>

#include <openssl/evp.h>

using namespace std;

namespace CryptoCipher {
    enum class WukHash_Type {
        MD5, SHA1, SHA256, SHA384, SHA512
    };

    class LIBWUK_API FEA {
    private:
        wuk::crypto::FEA fea_ctx;
        wuk::crypto::Counter counter;
        w_u32 segment_size;

    private:
        wuk::Buffer get_digest(const char *data, wSize length, WukHash_Type type = WukHash_Type::SHA256)
        {
            wuk::Buffer digestBuffer;

            const EVP_MD *md = nullptr;
            switch(type) {
                case WukHash_Type::MD5:    md = EVP_md5();    break;
                case WukHash_Type::SHA1:   md = EVP_sha1();   break;
                case WukHash_Type::SHA256: md = EVP_sha256(); break;
                case WukHash_Type::SHA384: md = EVP_sha384(); break;
                case WukHash_Type::SHA512: md = EVP_sha512(); break;
            }

            EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
            EVP_DigestInit_ex(md_ctx, md, nullptr);
            EVP_DigestUpdate(md_ctx, data, length);
            EVP_DigestFinal_ex(md_ctx, digestBuffer.append_write(32), nullptr);
            EVP_MD_CTX_free(md_ctx);

            return digestBuffer;
        }

    public:
        FEA()
        : fea_ctx(), counter(), segment_size(128)
        {

        }

        void set_counter(wuk::crypto::Counter &counter)
        {
            this->counter = counter;
        }

        void set_segment_size(w_u32 segment_size)
        {
            this->segment_size = segment_size;
        }

        void new_ctx(const char *password)
        {
            wuk::Buffer salt = get_digest(password, strlen(password));

            wByte KeyWithIV[48];
            const wByte *key = KeyWithIV;
            const wByte *iv  = KeyWithIV + 32;

            PKCS5_PBKDF2_HMAC(password, strlen(password), salt.get_data(), salt.get_length(),
                11246, EVP_sha256(), sizeof(KeyWithIV), KeyWithIV);

            this->fea_ctx = wuk::crypto::FEA{key, iv, this->counter, this->segment_size};
        }

        void encrypt(const wByte *plaintext, wSize length, wuk::crypto::mode encrypt_mode)
        {
            wuk::Base64 base64;
            wuk::Binascii binascii;

            wByte *ciphertext = (wByte *)malloc(length);
            if(!ciphertext) {
                throw wuk::Exception(wuk::Error::ERR, "FEA_test::encrypt",
                    "failed to alloc for ciphertext.");
            }
            memcpy(ciphertext, plaintext, length);

            this->fea_ctx.encrypt(ciphertext, length, encrypt_mode);

            cout << "ciphertext[Raw]:    "; wuk::misc::print_pybytes(ciphertext, length, true);
            cout << "ciphertext[Hex]:    " << binascii.b2a_hex(string{reinterpret_cast<char *>(ciphertext), length}) << endl;
            cout << "ciphertext[Base64]: " << base64.encode(string{reinterpret_cast<char *>(ciphertext), length}) << endl;

            free(ciphertext);
        }
    };
}

void test()
{
    try {
        CryptoCipher::FEA fea;
        wuk::crypto::mode mode = wuk::crypto::mode::CFB;

        const char *password = "SM-SN-Grotesque-C++-Develop,";
        const char *_content = {"{'user':'sngrotesque', 'password': 'hello,world.'}"};
        const wByte *plaintext = reinterpret_cast<const wByte *>(_content);
        wSize plaintext_length = strlen(_content);

        wuk::crypto::Counter counter{"sngrotesque", 61};
        fea.set_counter(counter);
        fea.set_segment_size(24);
        fea.new_ctx(password);

        fea.encrypt(plaintext, plaintext_length, mode);

    } catch (wuk::Exception &e) {
        cout << e.what() << endl;
    } catch (std::exception &e) {
        cout << e.what() << endl;
    }
}

int main()
{
    test();

    return 0;
}
