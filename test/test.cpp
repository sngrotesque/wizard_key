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
#include <crypto/chacha20.cpp>

#include <base64.cpp>
#include <random.cpp>
#include <hexdump.cpp>
#include <padding.cpp>
#include <struct.cpp>
#include <misc.cpp>
#include <time.cpp>
// Source path: End

using namespace wmkc::crypto;

// Usage: python run.py test\test.cpp -O3 -Wall -lws2_32 -lssl -lcrypto -DWMKC_EXPORTS -Wno-sign-compare
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

void base64_impl_test()
{
    wmkc::Base64 base64;

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
            "OC4wKSBHZWNrby8yMDEwMDEwMSBGaXJlZm94LzEyOC4wDQoNCg==sa1C"};

        cout << ">--------------- Encoding test ---------------<" << endl;
        cout << base64.encode(unencoded) << endl;
        cout << ">--------------- Decoding test ---------------<" << endl;
        cout << base64.decode(undecoded) << endl;

    } catch(std::exception &e) {
        cout << e.what() << endl;
    }
}

void base64_speed_test(wSize length)
{
    wmkc::Base64 base64;
    wmkc::Time timer;
    double start;
    double stop;

    wByte *unencoded = new (std::nothrow) wByte[length];
    char *undecoded = nullptr;

    start = timer.time();
    undecoded = base64.encode(unencoded, length);
    stop = timer.time();

    printf("encoding time: %.4lf\n", stop-start);
    delete[] unencoded;

    start = timer.time();
    unencoded = base64.pyDecode(undecoded, length);
    stop = timer.time();

    printf("decoding time: %.4lf\n", stop-start);
    delete[] unencoded;

    delete[] undecoded;
}

char *get_base64_string(const char *buffer, size_t length)
{
    std::vector<wByte> result_vector;

    for(size_t i = 0; i < length; ++i) {
        if(b64de_table[buffer[i]] != 255) {
            result_vector.push_back(buffer[i]);
        }
    }
    
    char *result_char = new (std::nothrow) char[result_vector.size()];
    memcpy(result_char, result_vector.data(), result_vector.size());

    return result_char;
}

int main(int argc, char **argv)
{
    std::vector<std::string> buffer{
        "c3##RhY2t...vdmV!?y~Zmxvdw==",
        "c3\nRhY2tvd\nmVyZmxvdw==",
        "c3Rh$$$$$$$$$$$$$$$$$$$$$Y2tvdmVy###############Zmxvdw=="};

    for(int i = 0; i < buffer.size(); ++i) {
        char *res = get_base64_string(buffer[i].c_str(), buffer[i].size());
        printf("%s\n", res);
        delete[] res;
    }

    return 0;
}




