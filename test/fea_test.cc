#include <crypto/WukFEA.hh>
#include <crypto/WukUtils.hh>
#include <WukBuffer.hh>
#include <WukBase64.hh>
#include <WukPadding.hh>
#include <WukMisc.hh>
#include <WukTime.hh>

#include <crypto/WukFEA.cc>
#include <crypto/WukFEA_ECB.cc>
#include <crypto/WukFEA_CBC.cc>
#include <crypto/WukFEA_CTR.cc>
#include <crypto/WukFEA_CFB.cc>
#include <crypto/WukUtils.cc>
#include <WukBuffer.cc>
#include <WukBase64.cc>
#include <WukPadding.cc>
#include <WukMisc.cc>
#include <WukTime.cc>

#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>

#include <WukRandom.hh>
#include <WukRandom.cc>

#include <openssl/evp.h>

class FCipher {
private:
    wuk::crypto::mode x_mode = wuk::crypto::mode::CTR;
    
    wuk::crypto::FEA build_new_ctx(wuk::Buffer password, wuk::Buffer nonce, wuk::Buffer salt, wSize start_val)
    {
        wuk::crypto::Counter counter{nonce.get_data(), static_cast<wU32>(nonce.get_length()), start_val};
        wByte keyWithIv[32 + 16]{};

        int err = PKCS5_PBKDF2_HMAC(password.get_cstr(), password.get_length(), salt.get_data(), salt.get_length(),
                                    14117, EVP_sha256(), sizeof(keyWithIv), keyWithIv);
        if(err != 1) {
            throw wuk::Exception(wuk::Error::ERR, "FCipher::build_new_ctx", "call PKCS5_PBKDF2_HMAC error.");
        }

        return wuk::crypto::FEA{keyWithIv, keyWithIv + 32, counter};
    }

public:
    FCipher() = default;

    void encrypt_file(std::filesystem::path src, std::filesystem::path dst, wuk::Buffer password)
    {
        std::ifstream file_src{src, std::ios::binary};
        std::ofstream file_dst{dst, std::ios::binary};
        wuk::Random random;

        wuk::Buffer nonce{12};
        wuk::Buffer salt{16};
        wuk::Buffer start_val;
        wSize s_val{0};

        if (!file_src.is_open()) {
            throw wuk::Exception(wuk::Error::ERR, "FCipher::encrypt_file",
                                "Failed to open source file.");
        }
        if(!file_dst.is_open()) {
            throw wuk::Exception(wuk::Error::ERR, "FCipher::encrypt_file",
                                "Failed to open dst file.");
        }

        random.urandom(nonce.append_write(12), nonce.get_size());
        random.urandom(salt.append_write(16), salt.get_size());
        s_val = random.randint(0, ~0);
        start_val.append_number(s_val);

        wuk::crypto::FEA fea_ctx = this->build_new_ctx(password, nonce, salt, s_val);

        file_dst.write(start_val.get_cstr(), start_val.get_length());
        file_dst.write(salt.get_cstr(), salt.get_length());
        file_dst.write(nonce.get_cstr(), nonce.get_length());

        wByte buffer[4096]{};
        std::streamsize length{0};
        char *buffer_ptr = reinterpret_cast<char *>(buffer);
        for (;;) {
            file_src.read(buffer_ptr, sizeof(buffer));
            length = file_src.gcount();

            if (!length) {
                break;
            }

            fea_ctx.encrypt(buffer, length, this->x_mode);
            file_dst.write(buffer_ptr, length);
        }
    }

    void decrypt_file(std::filesystem::path src, std::filesystem::path dst, wuk::Buffer password)
    {
        std::ifstream file_src{src, std::ios::binary};
        std::ofstream file_dst{dst, std::ios::binary};

        if(!file_src.is_open()) {
            throw wuk::Exception(wuk::Error::ERR, "FCipher::encrypt_file", "Failed to open source file.");
        }
        if(!file_dst.is_open()) {
            throw wuk::Exception(wuk::Error::ERR, "FCipher::encrypt_file", "Failed to open dst file.");
        }

        wuk::Buffer nonce{12};
        wuk::Buffer salt{16};
        char start_val[8]{};
        wSize s_val{0};

        file_src.read(start_val, sizeof(start_val));
        
        for (wU32 i = 0; i < (sizeof(start_val) >> 1); ++i) {
            wByte swap = start_val[i];
            start_val[i] = start_val[sizeof(start_val) - 1 - i];
            start_val[sizeof(start_val) - 1 - i] = swap;
        }
        memcpy(&s_val, start_val, sizeof(start_val));

        file_src.read(reinterpret_cast<char *>(salt.append_write(salt.get_size())), salt.get_size());
        file_src.read(reinterpret_cast<char *>(nonce.append_write(nonce.get_size())), nonce.get_size());

        wuk::crypto::FEA fea_ctx = this->build_new_ctx(password, nonce, salt, s_val);

        wByte buffer[4096]{};
        std::streamsize length{0};
        char *buffer_ptr = reinterpret_cast<char *>(buffer);
        for (;;) {
            file_src.read(buffer_ptr, sizeof(buffer));
            length = file_src.gcount();

            if (!length) {
                break;
            }

            fea_ctx.decrypt(buffer, length, this->x_mode);
            file_dst.write(buffer_ptr, length);
        }
    }
};

wuk::crypto::FEA build_new_ctx(wuk::Buffer password, wuk::Buffer salt)
{
    wByte keyWithIv[32 + 16]{};

    int err = PKCS5_PBKDF2_HMAC(password.get_cstr(), password.get_length(), salt.get_data(), salt.get_length(),
                                14117, EVP_sha256(), sizeof(keyWithIv), keyWithIv);
    if(err != 1) {
        throw wuk::Exception(wuk::Error::ERR, "FCipher::build_new_ctx", "call PKCS5_PBKDF2_HMAC error.");
    }

    return wuk::crypto::FEA{keyWithIv, keyWithIv + 32};
}

void roundKey_test()
{
    wByte key[32]{};
    wByte iv[16]{};

    wuk::crypto::FEA fea(key, iv);

    wuk::misc::print_hex(fea.get_round_key(),
                        wuk::crypto::WUK_FEA_KEYLEN * wuk::crypto::WUK_FEA_NR,
                        wuk::crypto::WUK_FEA_KEYLEN, true, false);
}

void speed_test(wSize length)
{
    wByte key[32]{}, iv[16]{};
    wuk::crypto::FEA fea(key, iv);
    wuk::Time timer;
    
    wByte *content = wuk::m_alloc<wByte *>(length);

    auto start_time = timer.time();
    fea.encrypt(content, length, wuk::crypto::mode::CTR);
    auto stop_time = timer.time();

    printf("ciphertext[0]: %02x\n", content[0]);

    printf("Timer: %.4lf\n", (stop_time-start_time));

    wuk::m_free(content);
}

void test1()
{
    char _content[] = {
        "GET /qrcode/getLoginUrl HTTP/1.1\r\n"
        "Host: sngrotesque.com\r\n"
        "User-Agent: Android\r\n\r\n"
    };
    wByte *plaintext = reinterpret_cast<wByte *>(_content);
    wSize length = strlen(_content);

    wByte key[32] = {
        0x89, 0x66, 0x8e, 0xa6, 0x91, 0x74, 0x06, 0xd8,
        0x9e, 0x4e, 0x8e, 0x7a, 0x6e, 0x76, 0xc5, 0x89,
        0x1d, 0xe6, 0x22, 0x43, 0x25, 0x71, 0xbf, 0x8e,
        0xe3, 0x6a, 0x91, 0x95, 0xcd, 0x90, 0xe6, 0xc0
    };
    wByte iv[16] = {
        0xf3, 0x05, 0x0a, 0x5a, 0x1d, 0x21, 0x2e, 0xe2,
        0x70, 0x8a, 0x32, 0xc4, 0x34, 0xa2, 0xa0, 0xa5
    };
    wuk::crypto::Counter counter("sngrotesque", 2048);
    wuk::crypto::FEA fea(key, iv, counter);

    fea.encrypt(plaintext, length, wuk::crypto::mode::CTR);

    wuk::misc::print_hex(plaintext, length, 16, true, false);
}

void test2()
{
    wByte salt[16] = {
        0x72, 0xea, 0x8e, 0x2f, 0xaf, 0xdc, 0xf8, 0x50,
        0x25, 0xf6, 0x6b, 0x35, 0x2e, 0x33, 0x4a, 0x9f
    };

    wuk::crypto::FEA fea = build_new_ctx({"SN-Grotesque"}, {salt, sizeof(salt)});
    wuk::Base64 base64;

    char _[] = {
        "我爱你啊！"
    };
    wByte *plaintext = reinterpret_cast<wByte *>(_);
    wSize length = strlen(_);

    fea.encrypt(plaintext, length, wuk::crypto::mode::CFB);

    wuk::misc::print_hex(plaintext, length, 16, true, false);

    char *ciphertext = base64.encode(plaintext, length);
    std::cout << ciphertext << std::endl;
    wuk::m_free(ciphertext);
}

// python make.py test\fea_test.cc -DWUK_EXPORTS -lbcrypt -lssl -lcrypto

int main()
{
    try {
        // speed_test(1024 * 1024 * 1024);
        test1();
    } catch (wuk::Exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
