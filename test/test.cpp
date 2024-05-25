#include <network/socket.hpp>
#include <crypto/fea.hpp>
#include <base64.hpp>
#include <random.hpp>
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

#include <base64.cpp>
#include <random.cpp>
#include <padding.cpp>
#include <struct.cpp>
#include <misc.cpp>
#include <time.cpp>
// Source path: End

// Usage: python run.py test\test.cpp -O3 -lws2_32 -lssl -lcrypto
namespace wmkc {
    namespace test {
        wVoid derivedKey(const string passwd, const string salt, wByte *key, wByte *iv, wS32 key_len = 32, wS32 iv_len = 16)
        {
            const wS32 length = key_len + iv_len;
            wByte *content = new wByte[length];

            PKCS5_PBKDF2_HMAC(passwd.c_str(), passwd.size(), (wByte *)salt.c_str(),
                                salt.size(), 10000, EVP_sha256(), length, content);

            memcpy(key, content, key_len);
            memcpy(iv, content + key_len, iv_len);

            delete[] content;
        }
    }
}

void test()
{
    const wByte *key = (wByte *)"00000000000000000000000000000000";
    const wByte *iv  = (wByte *)"abcdefghijklmopq";
    wmkc::crypto::Nonce_CTX nonce("abcdef");
    wmkc::crypto::FEA ctx(key, iv, nonce);

    char data[2048] = {"我是你爹，傻逼。\n"};
    wByte *buffer = (wByte *)data;
    size_t length = strlen(data);

    ctx.encrypt(buffer, length, wmkc::crypto::xcryptMode::CTR);

    wmkc::misc::PRINT_HEX(buffer, length, 32, 1, 0);
}

int main(int argc, char **argv)
{
    test();

    return 0;
}
