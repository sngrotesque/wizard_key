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
#include <lzma.h>

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

// Usage: python run.py test\test.cpp -O3 -lws2_32 -lssl -lcrypto -llzma

static const wByte *fea_key = (const wByte *)"helloworld.1234567890-=asfgjkzxc";
static const wByte *fea_iv  = (const wByte *)"abcdef0123456789";

namespace wmkc {
    namespace test {
        wVoid derivedKey(const string passwd, const string salt, wByte *key, wByte *iv, wS32 key_len = 32, wS32 iv_len = 16) {
            OpenSSL_add_all_algorithms();
            if (PKCS5_PBKDF2_HMAC(passwd.c_str(), passwd.size(), (wByte *)salt.c_str(),
                                salt.size(), 10000, EVP_sha256(), key_len, key) != 1) {
                wmkc::exception(wmkcErr_Err, "wmkc::test::generate_key_iv",
                                "Failed to generate key and IV.");
            }
            memcpy(iv, key + key_len - iv_len, iv_len);
        }

        wByte *lzma2_compress_data(const wByte *src, const wSize src_size, wSize &dst_size)
        {
            wByte *out_stream = nullptr;
            wSize out_stream_len;

            lzma_stream strm = LZMA_STREAM_INIT;
            wS32 ret;

            if((ret = lzma_easy_encoder(&strm, 5, LZMA_CHECK_CRC64)) != LZMA_OK) {
                wmkc::exception(ret, "lzma2_compress_data", "lzma_easy_encoder error.");
            }

            out_stream_len = src_size + src_size / 10 + 128;
            out_stream = new wByte[out_stream_len];
            if(!out_stream) {
                lzma_end(&strm);
                delete[] out_stream;
                wmkc::exception(wmkcErr_ErrMemory, "lzma2_compress_data",
                                "Failed to allocate memory of out_stream.");
            }

            strm.next_in = src;
            strm.avail_in = src_size;
            strm.next_out = out_stream;
            strm.avail_out = out_stream_len;

            ret = lzma_code(&strm, LZMA_FINISH);
            if(ret != LZMA_STREAM_END) {
                lzma_end(&strm);
                delete[] out_stream;
                wmkc::exception(ret, "lzma2_compress_data", "lzma_code error.");
            }

            dst_size = strm.total_out;
            lzma_end(&strm);

            return out_stream;
        }
    }
}

int main(int argc, char **argv)
{
    wmkc::structure Struct;
    std::string res = Struct.pack(
        "!HHHHHHBHH",
        0x04d2,
        0x0100,
        0x0001,
        0x0000,
        0x0000,
        0x0000,
        0x00,
        0x0001,
        0x0001);
    const wByte *buffer = (const wByte *)res.c_str();
    const wSize length = res.size();

    wmkc::misc::PRINT(buffer, length, 32, 1, 0);

    return 0;
}
