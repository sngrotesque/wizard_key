#include <network/socket.hpp>
#include <crypto/fea.hpp>
#include <base64.hpp>
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

#include <base64.cpp>
#include <random.cpp>
#include <hexdump.cpp>
#include <padding.cpp>
#include <struct.cpp>
#include <misc.cpp>
#include <time.cpp>
// Source path: End

using namespace wmkc::crypto;

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
    wmkc::crypto::xcryptMode mode;
    wmkc::crypto::FEA fea;
    wmkc::Random random;
    wByte key[32], iv[16];

    string password = "sngrotesque-ROOT0.";
    string nonce    = "/~n10*DF)4=^";
    string salt     = "\x71\x67\x39\x33\x2e\x50\x56\x15\xec\xcf\xf1\xb9\xfd\xb9\xc4\xb6";

    wmkc::test::derivedKey(password, salt, key, iv);
    fea = wmkc::crypto::FEA(key, iv, nonce);
    mode = wmkc::crypto::xcryptMode::CTR;

    char data[2048] = {"我草你妈，傻逼队友，你打你妈呢！日你妈！能不能让我赢一把啊！！？？？？？？"};
    wByte *buffer = (wByte *)data;
    wSize length = strlen(data);

    // wmkc::pad(buffer, length, WMKC_FEA_BL, false);

    printf("Plaintext:\n"); wmkc::misc::PRINT_HEX(buffer, length, 32, length%32, true);

    if(mode == wmkc::crypto::xcryptMode::ECB) {
        for(wU32 i = 0; i < length; i += WMKC_FEA_BL) {
            fea.encrypt(buffer + i, WMKC_FEA_BL, mode);
        }
    } else {
        fea.encrypt(buffer, length, mode);
    }

    printf("Ciphertext:\n"); wmkc::misc::PRINT_HEX(buffer, length, 32, length%32, true);
}

int main(int argc, char **argv)
{
    test();

    return 0;
}
