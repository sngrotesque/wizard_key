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

void Base64_decode_test()
{
    vector<string> bufferList{
        "\n\n==R0VUIC9xcmNvZGUvZ2V0TG9naW5VcmwgSFR\nUUC8xLjENCkhvc3Q6IHBhc3Nwb3J0"
        "LmJpbGliaWxpLmNvbQ0KQWNjZXB0OiAqLyo\nNCkNvbm5lY3Rpb246IGtlZXAtYWxp"
        "dmUNCkNvbnRlbnQtVHlwZTogYXBwbGljYXRpb24veC13d3ctZnJvbV91cmxlbmNv\n"
        "ZGVkDQpDb250ZW50LUxlbmd0aDogMjANClVzZXItQWdlbnQ6IE1v\nemlsbGEvNS4w"
        "IChYMTE7IExpbnV4IHg4Nl82NDsgcnY6MTI4LjApIEdlY2t\nvLzIwMTAwMTAxIEZp"
        "cmVmb3gvMTI4LjANCg0Ka2\n\n\nV5OjAxMjM0NTY3ODlhYmNkZWY="};
    wmkc::Base64 base64;

    base64.strict_mode = true;

    try {
        for(string &bufferString : bufferList) {
            string result = base64.decode(bufferString);
            cout << result << endl;
        }
    } catch (exception &e) {
        cout << e.what() << endl;
    }
}

int main(int argc, char **argv)
{
    Base64_decode_test();

    return 0;
}




