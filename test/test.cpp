#include <network/socket.hpp>
#include <crypto/fea.hpp>
#include <crypto/chacha20.hpp>
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

#include <random.cpp>
#include <hexdump.cpp>
#include <padding.cpp>
#include <struct.cpp>
#include <misc.cpp>
#include <time.cpp>
// Source path: End

using namespace wmkc::crypto;

// Usage: python run.py test\test.cpp -O3 -Wall -lws2_32 -lssl -lcrypto -DWMKC_EXPORTS
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

#include <base64.hpp>
#include <base64.cpp>

void base64_test(std::string _buffer, wBool encoding)
{
    wmkc::Base64 base64;
    std::string result{};

    if(encoding) {
        result = base64.encode(_buffer);
    } else {
        result = base64.decode(_buffer);
    }

    printf("result: %s\n", result.c_str());
    printf("length: %llu\n", result.size());
}

int main(int argc, char **argv)
{
    try {
        std::string unencoded{
            "GET /qrcode/getLoginUrl HTTP/1.1\r\n"
            "Host: passport.bilibili.com\r\n"
            "Accept: */*\r\n"
            "Connection: close\r\n"
            "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:128.0) Gecko/20100101 Firefox/128.0\r\n\r\n"};
        std::string undecoded{
            "R0VUIC9xcmNvZGUvZ2V0TG9naW5VcmwgSFRUUC8xLjENCkhvc3Q6IHBhc3Nwb3J0"
            "LmJpbGliaWxpLmNvbQ0KQWNjZXB0OiAqLyoNCkNvbm5lY3Rpb246IGNsb3NlDQpV"
            "c2VyLUFnZW50OiBNb3ppbGxhLzUuMCAoWDExOyBMaW51eCB4ODZfNjQ7IHJ2OjEy"
            "OC4wKSBHZWNrby8yMDEwMDEwMSBGaXJlZm94LzEyOC4wDQoNCg=="};

        std::cout << ">--------------- Encoding test ---------------<" << std::endl;
        base64_test(unencoded, true);
        std::cout << ">--------------- Decoding test ---------------<" << std::endl;
        base64_test(undecoded, false);
    } catch(std::exception &e) {
        cout << e.what() << endl;
    }

    return 0;
}
