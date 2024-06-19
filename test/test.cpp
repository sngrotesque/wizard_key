#include <network/socket.hpp>
#include <crypto/fea.hpp>
#include <crypto/chacha20.hpp>
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
#include "crypto/chacha20.cpp"

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
        void derivedKey(const string passwd, const string salt, wByte *key, wByte *iv, wS32 key_len = 32, wS32 iv_len = 16)
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

void speed_test()
{
    wSize length = 268435456;
    wByte *buffer = new wByte[length];

    FEA fea((wByte *)"0123456789abcdef0123456789abcdef", (wByte *)"0123456789abcdef");

    double start = wmkc::Time().time();
    fea.encrypt(buffer, length, xcryptMode::CTR);
    double stop = wmkc::Time().time();

    printf("%.4lf\n", stop-start);
    delete[] buffer;
}

void chacha20_test()
{
    wByte *key = (wByte *)"ahu1t9yasf97t1935guashiudas15./-";
    wByte *nonce = (wByte *)"j0f18y9GH2kd";
    uint32_t count = 1;

    ChaCha20 ctx(key, nonce, count);
    ctx.init();

    size_t length = 256 * (1024 * 1024);
    printf("Create data (length: %llu bytes.)\n", length);
    uint8_t *buffer = new wByte[length];

    wmkc::Time timer;
    double start;
    double stop;
    printf("start time\n");
    start = timer.time();
    ctx.xcrypt(buffer, length);
    stop = timer.time();
    printf("stop time\n");

    printf("time used: %.4lf\n", stop-start);
    printf("buffer[0]: %02x\n", buffer[0]);

    delete[] buffer;
}

void base64_test()
{
    wmkc::Time timer;
    wmkc::Base64 base64;
    
    string src{
        "R0VUIC9xcmNvZGUvZ2V0TG9naW5VcmwgSFRUUC8xLjENCkhvc3Q6IHBhc3Nwb3J0"
        "LmJpbGliaWxpLmNvbQ0KQWNjZXB0OiBhcHBsaWNhdGlvbi9qc29uDQpBY2NlcHQt"
        "TGFuZ3VhZ2U6IHpoLUNOLHpoO3E9MC45LGVuO3E9MC44LGVuLUdCO3E9MC43LGVu"
        "LVVTO3E9MC42DQpVc2VyLUFnZW50OiBNb3ppbGxhLzUuMCAoV2luZG93cyBOVCAx"
        "MC4wOyBXaW42NDsgeDY0KSBBcHBsZVdlYktpdC81MzcuMzYgKEtIVE1MLCBsaWtl"
        "IEdlY2tvKSBDaHJvbWUvMTI1LjAuMC4wIFNhZmFyaS81MzcuMzYgRWRnLzEyNS4w"
        "LjAuMA0KDQo="};
    string dst{base64.decode(src)};

    cout << dst << endl;

}

int main(int argc, char **argv)
{
    base64_test();

    return 0;
}
