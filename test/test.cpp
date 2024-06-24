#include <network/socket.hpp>
#include <crypto/fea.hpp>
#include <crypto/chacha20.hpp>

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

#include <crypto/fea.cpp>
#include <crypto/fea_ecb.cpp>
#include <crypto/fea_cbc.cpp>
#include <crypto/fea_ctr.cpp>
#include <crypto/fea_cfb.cpp>
#include <crypto/chacha20.cpp>

#include <base64.cpp>
#include <binascii.cpp>
#include <random.cpp>
#include <hexdump.cpp>
#include <padding.cpp>
#include <struct.cpp>
#include <misc.cpp>
#include <time.cpp>
// Source path: End

// Usage: python run.py test\test.cpp -O3 -Wall -lws2_32 -lssl -lcrypto -DWMKC_EXPORTS -Wno-sign-compare
namespace wmkc {
    namespace test {
        void derivedKey(string passwd, string salt, wByte *key, wByte *iv, wS32 key_len = 32, wS32 iv_len = 16)
        {
            wByte *content = new wByte[key_len + iv_len];

            PKCS5_PBKDF2_HMAC(passwd.c_str(), passwd.size(), (wByte *)salt.c_str(), salt.size(),
                            10000, EVP_sha256(), key_len + iv_len, content);

            memcpy(key, content, key_len);
            memcpy(iv, content + key_len, iv_len);

            delete[] content;
        }
    }
}

void Base64_decode_test()
{
    wmkc::Base64 base64;

    const char *buffer = "UwU//";
    // const char *buffer = "wu////";
    // const char *buffer = "ac=";
    wSize length = strlen(buffer);

    printf("buffer: %s\n", buffer);
    printf("length: %zd\n", length);

    try {
        wByte *result = base64.decode(buffer, length);
        printf("result: %s\n", result);
        printf("length: %zd\n", length);
        wmkc::misc::PRINT_PyBytes(result, length, 1);
        delete[] result;
    } catch (exception &e) {
        cout << e.what() << endl;
    }
}

void binascii_test()
{
    wmkc::Binascii binascii;

    string result;
    cout << (result = binascii.b2a_hex("GET / HTTP/1.1\r\nHost: passport.bilibili.com\r\nUser-Agent: android\r\n\r\n")) << endl;

    cout << binascii.a2b_hex(result) << endl;
}

int main(int argc, char **argv)
{
    Base64_decode_test();

    return 0;
}




