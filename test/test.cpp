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
* Windows Usage: python run.py test\test.cpp -O3 -Wall -lws2_32 -lssl -lcrypto -DWMKC_EXPORTS -Wno-sign-compare --std=c++17
* Linux   Usage: python run.py test\test.cpp -O3 -Wall -lssl -lcrypto -DWMKC_EXPORTS -Wno-sign-compare --std=c++17
*/
namespace wmkc {
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
            wmkc::Binascii binascii;
            wmkc::Base64 base64;

            wmkc::crypto::FEA fea;
            wmkc::crypto::Counter counter;
            wmkc::crypto::xcryptMode mode{wmkc::crypto::xcryptMode::CTR};

            char password[32]{};
            char salt[32]{};
            wByte key[32]{};
            wByte iv[16]{};

            cout << "Please enter password: ";
            cin.getline(password, sizeof(password) - 1);
        
            cout << "Please enter salt: ";
            cin.getline(salt, sizeof(salt) - 1);
        
            wmkc::test::derivedKey(password, salt, key, iv);
        
            string string_key{string{(char *)key, 32}};
            string string_iv{string{(char *)iv, 16}};
        
            counter = wmkc::crypto::Counter{"hellowoeld.", 0};
            fea = wmkc::crypto::FEA{key, iv, counter};
        
            // 打印key和IV
            cout << "Key-Hex:    " << binascii.b2a_hex(string_key) << endl;
            cout << "IV-Hex:     " << binascii.b2a_hex(string_iv) << endl;
            cout << "Key-Base64: " << base64.encode(string_key) << endl;
            cout << "IV-Base64:  " << base64.encode(string_iv) << endl;
            // 打印计数器
            cout << "Counter:\n";
            wmkc::misc::print_hex(counter.counter, 16, 16, false, true);
        
            char _tmp[2048] = {"\\(UwU)/"};
            wByte *buffer = (wByte *)_tmp;
            wSize length = strlen(_tmp);
        
            cout << "Plaintext:\n";
            wmkc::misc::print_hex(buffer, length, 32, length%32, true);
        
            fea.encrypt(buffer, length, mode);
        
            cout << "Ciphertext:\n";
            wmkc::misc::print_hex(buffer, length, 32, length%32, true);
        }

        void test()
        {
#           ifdef WMKC_PLATFORM_WINOS
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
                wmkc::net::SSL_Context ssl_ctx{TLS_method()};
                wmkc::net::SSL_Socket ssl_fd = ssl_ctx.wrap_socket(
                    wmkc::net::Socket{AF_INET, SOCK_STREAM, IPPROTO_TCP}, remote_addr);

                ssl_fd.connect(remote_addr, remote_port);

                ssl_fd.send(request_header);
                cout << ssl_fd.recv(4096) << endl;

                ssl_ctx.destroy();
            } catch (exception &e) {
                cout << e.what() << endl;
            }

#           ifdef WMKC_PLATFORM_WINOS
            WSACleanup();
#           endif
        }

        void struct_test()
        {
            wmkc::Struct Struct;
            string res = Struct.pack(">IIId", 1, 2, 3, 3.0);

            wByte *result = (wByte *)res.data();
            wSize length = res.size();

            wmkc::misc::print_hex(result, length, 32, 1, 0);
        }
    }
}

int main(int argc, char **argv)
{
    // wmkc::test::struct_test();

    const char    *p1 = "12345";
    const uint8_t *p2 = static_cast<const uint8_t *>(static_cast<const void *>(p1));

    const char    *p3 = "12345";
    const uint8_t *p4 = reinterpret_cast<const uint8_t *>(p3);

    const char    *p5 = "12345";
    const uint8_t *p6 = (const uint8_t *)p5;

    printf("p1 ptr addr: %p\n", p1);
    printf("p2 ptr addr: %p\n", p2);

    printf("p3 ptr addr: %p\n", p3);
    printf("p4 ptr addr: %p\n", p4);

    printf("p5 ptr addr: %p\n", p5);
    printf("p6 ptr addr: %p\n", p6);

    return 0;
}
