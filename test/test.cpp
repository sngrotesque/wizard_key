#include <network/socket.hpp>
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

// Usage: python run.py test\test.cpp -O3 -Wall -lws2_32 -lssl -lcrypto -DWMKC_EXPORTS -Wno-sign-compare
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
    }
}

void fea_binascii_base64_test()
{
    wmkc::Binascii binascii;
    wmkc::Base64 base64;

    wmkc::crypto::FEA fea;
    wmkc::crypto::Counter counter;
    wmkc::crypto::xcryptMode mode{wmkc::crypto::xcryptMode::CFB};

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

    counter = wmkc::crypto::Counter{"hellowoeld.", 1};
    fea = wmkc::crypto::FEA{key, iv, counter};

    // 打印key和IV
    cout << "Key-Hex:    " << binascii.b2a_hex(string_key) << endl;
    cout << "IV-Hex:     " << binascii.b2a_hex(string_iv) << endl;
    cout << "Key-Base64: " << base64.encode(string_key) << endl;
    cout << "IV-Base64:  " << base64.encode(string_iv) << endl;
    // 打印计数器
    cout << "Counter:\n";
    wmkc::misc::PRINT_HEX(counter.counter, 16, 16, false, true);

    char _tmp[2048] = {"hello, world"};
    wByte *buffer = (wByte *)_tmp;
    wSize length = strlen(_tmp);

    cout << "Plaintext:\n";
    wmkc::misc::PRINT_HEX(buffer, length, 32, length%32, true);

    fea.encrypt(buffer, length, mode);

    cout << "Ciphertext:\n";
    wmkc::misc::PRINT_HEX(buffer, length, 32, length%32, true);
}

int main(int argc, char **argv)
{
    wmkc::Base64 base64;

    try {
        cout << base64.decode("XChVd1UpLw==") << endl;
    } catch (exception &e) {
        cout << e.what() << endl;
    }

    return 0;
}




