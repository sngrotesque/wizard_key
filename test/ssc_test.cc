#include <crypto/WukSSC.hh>
#include <crypto/WukUtils.hh>
#include <WukBinascii.hh>
#include <WukBase64.hh>
#include <WukRandom.hh>
#include <WukBuffer.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <crypto/WukSSC.cc>
#include <crypto/WukUtils.cc>
#include <WukBinascii.cc>
#include <WukBase64.cc>
#include <WukRandom.cc>
#include <WukBuffer.cc>
#include <WukTime.cc>
#include <WukMisc.cc>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <unistd.h>

void test1(const wByte *key, const wByte *iv, wuk::crypto::Counter counter)
{
    wuk::crypto::SSC ssc(key, iv, counter);

    char test_plaintext[1024] = {
        // "GET /qrcode/getLoginUrl HTTP/1.1\r\n"
        // "Host: passport.bilibili.com\r\n"
        // "Accept: application/json; q=0.9, */*\r\n"
        // "Connection: keep-alive\r\n"
        // "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:129.0) Gecko/20100101 Firefox/129.0\r\n\r\n"

        // "0000000000000000000000000000000000000000000000000000000000000000"
        // "0000000000000000000000000000000000000000000000000000000000000000"
        // "0000000000000000000000000000000000000000000000000000000000000000"
        // "0000000000000000000000000000000000000000000000000000000000000000"

        // "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        // "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        // "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        // "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        // "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        // "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"

        "hello, world\n"
    };
    char *test_ciphertext[1024]{};

    wSize length = strlen(test_plaintext);

    memcpy(test_ciphertext, test_plaintext, length);

    ssc.xcrypt((wByte *)test_ciphertext, length);

    std::cout << "Plaintext:\t\t\t\t\t\t\t\tCiphertext:\n";
    wuk::misc::print_diff_hex((wByte *)test_plaintext, (wByte *)test_ciphertext, length, length, 16, true);
}

void keystream_chack(const wByte *key_left,  const wByte *iv_left,  wuk::crypto::Counter counter_left,
                    const wByte *key_right, const wByte *iv_right, wuk::crypto::Counter counter_right,
                    const wU32 rounds = 8)
{
    wuk::crypto::SSC ssc_left(key_left, iv_left, counter_left);
    wuk::crypto::SSC ssc_right(key_right, iv_right, counter_right);

    for (wU32 c = 0; c < rounds; ++c) {
        std::cout << "Key Stream:\n";

        wuk::misc::print_diff_hex(
            ssc_left.get_keystream(), ssc_right.get_keystream(),
            wuk::crypto::WUK_SSC_KSLEN, wuk::crypto::WUK_SSC_KSLEN,
            16, true
        );

        ssc_left.keystream_update();
        ssc_right.keystream_update();
    }
}

void speed_test(wSize length, const wByte *key, const wByte *iv, wuk::crypto::Counter counter)
{
    wByte *buffer = new (std::nothrow) wByte[length];

    if (!buffer) {
        std::cerr << "buffer malloc error.\n";
        return;
    }

    wuk::crypto::SSC ssc(key, iv, counter);
    wuk::Time timer;

    double start_time = timer.time();
    ssc.xcrypt(buffer, length);
    double stop_time = timer.time();

    printf("Performance testing, test data size: %zd MB, Time consumption: %.4lf second.\n",
            length / (1024 * 1024), (stop_time - start_time));

    delete[] buffer;
}

void encrypt_file(std::filesystem::path in_path, std::filesystem::path out_path,
                const wByte *key, const wByte *iv, wuk::crypto::Counter counter)
{
    std::fstream in_file(in_path, std::ios::binary | std::ios::in);
    std::fstream out_file(out_path, std::ios::binary | std::ios::out);

    if (!in_file.is_open() || !out_file.is_open()) {
        std::cerr << "failed to file open.\n";
        return;
    }
    wuk::crypto::SSC ssc(key, iv, counter);

    for (;;) {
        wByte buffer[4096]{};
        wSize length = in_file.read(reinterpret_cast<char *>(buffer), sizeof(buffer)).gcount();
        if (!length) {
            break;
        }

        ssc.xcrypt(buffer, length);

        out_file.write(reinterpret_cast<char *>(buffer), length);
    }
}

void some_encryption_test()
{
    // wuk::Random random;
    // wuk::Buffer key{random.urandom(32)};
    // wuk::Buffer iv{random.urandom(16)};
    // wuk::crypto::Counter counter{random.urandom(12), random.randint(1, 16777216)};

    // wuk::misc::print_box(key.get_data(), 32, 16, true);
    // wuk::misc::print_box(iv.get_data(), 16, 16, true);
    // wuk::misc::print_box(counter.get(), 16, 16, true);

    wByte key[32] = {
        0xe4, 0xdb, 0x7e, 0x14, 0x89, 0xbc, 0x0c, 0x94, 0xdc, 0xd3, 0xb9, 0xc8, 0x81, 0x46, 0xcb, 0xdf,
        0xd3, 0x84, 0x72, 0x98, 0xe8, 0xc0, 0xce, 0xd2, 0x03, 0xb6, 0xf5, 0x24, 0xf4, 0x22, 0x96, 0xf6
    };
    wByte    iv[16] = {0x4d, 0xaf, 0xbc, 0x86, 0xd9, 0x71, 0x60, 0xe3};
    wByte nonce[12] = {0xa3, 0xcb, 0x72, 0x41, 0xf3, 0x90, 0x92, 0xbe, 0x0e, 0xaf, 0xce, 0x01};
    wuk::crypto::Counter counter(nonce, sizeof(nonce), 0x02);
    wuk::crypto::SSC ssc(key, iv, counter);
    wuk::Base64 base64;

    wByte *result = nullptr;
    char __content[] = {
        "煮波，你怎么了"

        // "RStqI6gSBrz3jSKX"
    };
    wByte *buffer = reinterpret_cast<wByte *>(__content);
    wSize length = strlen(__content);

    if (1) {
        ssc.xcrypt(buffer, length);
        std::cout << "result(Hex):" << std::endl;
        wuk::misc::print_hex(buffer, length, 32, true, true);
        result = (wByte *)base64.encode(buffer, length);
        std::cout << "result(Base64):\n\t" << result << std::endl;
    } else {
        result = base64.decode((char *)buffer, length);
        ssc.xcrypt(result, length);
        std::cout << "result(Hex):" << std::endl;
        wuk::misc::print_hex(result, length, 32, true, true);
        std::cout << "result(plaintext):\n\t" << result << std::endl;
    }
}

int main()
{
    // const wByte *key_left              = (const wByte *)"abcdef0123456789abcdef0123456789";
    // const wByte *iv_left               = (const wByte *)"abcdef0123456789";
    // wuk::crypto::Counter counter_left  = {"sngrotesque", 776};
    // const wByte *key_right             = (const wByte *)"abcdef0123456789abcdef0123456789";
    // const wByte *iv_right              = (const wByte *)"abcdef0123456789";
    // wuk::crypto::Counter counter_right = {"sngrotesquE", 776};

    const wByte key_left[32]{1};
    const wByte iv_left[8]{0};
    wuk::crypto::Counter counter_left = {"sngrotesque", 0x1001};
    const wByte key_right[32]{73};
    const wByte iv_right[8]{10};
    wuk::crypto::Counter counter_right = {"Sngrotesque", 0x1000};

    // const wByte *root_key = (const wByte *)"sngrotesque0123456789abcdef.-+=~";
    // const wByte *root_iv = (const wByte *)"abcdef0123456789";
    // wuk::crypto::Counter root_counter("helloworld", 1);

    try {
        // test1(root_key, root_iv, root_counter);
        keystream_chack(key_left, iv_left, counter_left, key_right, iv_right, counter_right, 7);
        // speed_test(1024 * 1024 * 1024, root_key, root_iv, root_counter);
        // encrypt_file("F:/Pitchers/sn-cosplay/image.bin", "F:/Pitchers/sn-cosplay/image.encrypted.bin", root_key, root_iv, root_counter);
        // some_encryption_test();
    } catch (wuk::Exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
