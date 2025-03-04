#include <network/socket.hpp>
#include <network/ssl.hpp>
#include <crypto/fea.hpp>
#include <crypto/chacha20.hpp>
#include <crypto/utils.hpp>

#include <base64.hpp>
#include <binascii.hpp>
#include <random.hpp>
#include <hexdump.hpp>
#include <padding.hpp>
#include <struct.hpp>
#include <misc.hpp>
#include <time.hpp>

#include <iostream>
#include <fstream>
using namespace std;

#include <openssl/evp.h>

// Source path: Begin
#include <network/socket.cpp>
#include <network/exception.cpp>
#include <network/ssl.cpp>

#include <crypto/fea.cpp>
#include <crypto/fea_ecb.cpp>
#include <crypto/fea_cbc.cpp>
#include <crypto/fea_ctr.cpp>
#include <crypto/fea_cfb.cpp>
#include <crypto/chacha20.cpp>
#include <crypto/utils.cpp>

#include <base64.cpp>
#include <binascii.cpp>
#include <random.cpp>
#include <hexdump.cpp>
#include <padding.cpp>
#include <struct.cpp>
#include <misc.cpp>
#include <time.cpp>
// Source path: End

/*
* Windows Usage: python make.py test\test.cpp -O3 -Wall -lws2_32 -lssl -lcrypto -DWUK_EXPORTS -Wno-sign-compare --std=c++17
* Linux   Usage: python make.py test\test.cpp -O3 -Wall -lssl -lcrypto -DWUK_EXPORTS -Wno-sign-compare --std=c++17
*/
namespace wuk {
    namespace test {
        void derivedKey(string passwd, string salt, wByte *key, wByte *iv,
                        wS32 key_len = 32, wS32 iv_len = 16)
        {
            wByte *content = new wByte[key_len + iv_len];

            const wByte *salt_buffer = (wByte *)salt.data();
            wSize salt_length = salt.size();

            PKCS5_PBKDF2_HMAC(passwd.c_str(), passwd.size(), salt_buffer,
                salt_length, 10000, EVP_sha256(), key_len + iv_len, content);

            memcpy(key, content, key_len);
            memcpy(iv, content + key_len, iv_len);

            delete[] content;
        }

        void fea_binascii_base64_test()
        {
            wuk::Binascii binascii;
            wuk::Base64 base64;

            wuk::crypto::FEA fea;
            wuk::crypto::Counter counter;
            wuk::crypto::xcryptMode mode{wuk::crypto::xcryptMode::CTR};

            char password[32]{};
            char salt[32]{};
            wByte key[32]{};
            wByte iv[16]{};

            cout << "Please enter password: ";
            cin.getline(password, sizeof(password) - 1);
        
            cout << "Please enter salt: ";
            cin.getline(salt, sizeof(salt) - 1);
        
            wuk::test::derivedKey(password, salt, key, iv);
        
            string string_key{string{(char *)key, 32}};
            string string_iv{string{(char *)iv, 16}};
        
            counter = wuk::crypto::Counter{"hellowoeld.", 0};
            fea = wuk::crypto::FEA{key, iv, counter};
        
            // 打印key和IV
            cout << "Key-Hex:    " << binascii.b2a_hex(string_key) << endl;
            cout << "IV-Hex:     " << binascii.b2a_hex(string_iv) << endl;
            cout << "Key-Base64: " << base64.encode(string_key) << endl;
            cout << "IV-Base64:  " << base64.encode(string_iv) << endl;
            // 打印计数器
            cout << "Counter:\n";
            wuk::misc::print_hex(counter.counter, 16, 16, false, true);
        
            char _tmp[2048] = {"\\(UwU)/"};
            wByte *buffer = (wByte *)_tmp;
            wSize length = strlen(_tmp);
        
            cout << "Plaintext:\n";
            wuk::misc::print_hex(buffer, length, 32, length%32, true);
        
            fea.encrypt(buffer, length, mode);
        
            cout << "Ciphertext:\n";
            wuk::misc::print_hex(buffer, length, 32, length%32, true);
        }

        void test()
        {
#           ifdef WUK_PLATFORM_WINOS
            WSADATA ws;
            WSAStartup(MAKEWORD(2,2), &ws);
#           endif

            string remote_addr{"www.bilibili.com"};
            wU16 remote_port{443};
            string request_header{
                "GET / HTTP/1.1\r\n"
                "Host: www.bilibili.com\r\n"
                "Accept: */*\r\n"
                "Connection: close\r\n"
                "User-Agent: android\r\n\r\n"
            };

            try {
                wuk::net::SSL_Context ssl_ctx{TLS_method()};
                wuk::net::SSL_Socket ssl_fd = ssl_ctx.wrap_socket(
                    wuk::net::Socket{AF_INET, SOCK_STREAM, IPPROTO_TCP}, remote_addr);

                ssl_fd.connect(remote_addr, remote_port);

                ssl_fd.send(request_header);
                cout << ssl_fd.recv(4096) << endl;

                ssl_ctx.destroy();
            } catch (exception &e) {
                cout << e.what() << endl;
            }

#           ifdef WUK_PLATFORM_WINOS
            WSACleanup();
#           endif
        }
    }
}

class fcipher {
private:
    wuk::Random random;
    std::string password;
    wByte salt[16];

    wByte fea_key[32];
    wByte fea_iv[16];
public:
    fcipher(std::string password)
    : random(), password(password), salt()
    {

    }

    template <typename T>
    void encrypt(T in_path, T out_path)
    {
        random.urandom(this->salt, sizeof(this->salt));
        wuk::test::derivedKey(this->password, reinterpret_cast<const char *>(salt),
                            this->fea_key, this->fea_iv);
        wuk::crypto::FEA fea(this->fea_key, this->fea_iv, {}, 112);

        fstream f_in(in_path,   ios::in  | ios::binary | ios::ate);
        fstream f_out(out_path, ios::out | ios::binary);
        if(!f_in.is_open() || !f_out.is_open()) {
            throw wuk::Exception(wukErr_Err, "fcipher::encrypt",
                "Failed to file open.");
        }

        wSize length = f_in.tellg();
        wByte *buffer = new (std::nothrow) wByte[length];
        if(!buffer) {
            throw wuk::Exception(wukErr_ErrMemory, "fcipher::encrypt",
                "Failed to allocate memory for buffer.");
        }
        f_in.seekg(0);
        f_in.read(reinterpret_cast<char *>(buffer), length);
        f_in.close();

        fea.encrypt(buffer, length, wuk::crypto::xcryptMode::CFB);

        f_out.write(reinterpret_cast<char *>(buffer), length);
        f_out.close();
    }
};

enum class HashDigest_Method {
    MD5, SHA1, SHA224, SHA256, SHA384, SHA512
};

string get_hash_hexdigest(const wchar_t *file_path, HashDigest_Method method = HashDigest_Method::SHA256)
{
    fstream f_obj{file_path, ios::in | ios::binary};

    wuk::Binascii binascii;
    wByte digest[64]{};
    w_u32 digest_size;

    char buffer[4096]{};
    size_t length;

    if(!f_obj.is_open()) {
        throw wuk::Exception(wukErr_Err, "get_hash_hexdigest", "failed to file open.");
    }

    const EVP_MD *md = nullptr;
    switch(method) {
    case HashDigest_Method::MD5:
        md = EVP_md5();
        digest_size = 16;
        break;
    case HashDigest_Method::SHA1:
        md = EVP_sha1();
        digest_size = 20;
        break;
    case HashDigest_Method::SHA224:
        md = EVP_sha224();
        digest_size = 28;
        break;
    case HashDigest_Method::SHA256:
        md = EVP_sha256();
        digest_size = 32;
        break;
    case HashDigest_Method::SHA384:
        md = EVP_sha384();
        digest_size = 48;
        break;
    case HashDigest_Method::SHA512:
        md = EVP_sha512();
        digest_size = 64;
        break;
    default:
        throw wuk::Exception(wukErr_Err, "get_hash_hexdigest", "Wrong hash method.");
    }
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();

    EVP_DigestInit_ex(md_ctx, md, nullptr);
    for(;;) {
        length = f_obj.read(buffer, sizeof(buffer)).gcount();
        if(!length) {
            break;
        }
        EVP_DigestUpdate(md_ctx, buffer, length);
    }
    EVP_DigestFinal_ex(md_ctx, digest, nullptr);

    EVP_MD_CTX_free(md_ctx);
    f_obj.close();

    return binascii.b2a_hex(string{reinterpret_cast<char *>(digest), digest_size});;
}

int main(int argc, char **argv)
{
    string hexdigest = get_hash_hexdigest(
        L"F:/Pitchers/Pixiv/14698535/2021_06_23_09_47_30_90749965_p0.jpg",
        HashDigest_Method::SHA256);
    
    cout << "hexdigest: " << hexdigest << endl;

    return 0;
}
