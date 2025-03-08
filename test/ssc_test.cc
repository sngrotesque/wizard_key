#include <crypto/WukSSC.hh>
#include <crypto/WukUtils.hh>
#include <WukBinascii.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <crypto/WukSSC.cc>
#include <crypto/WukUtils.cc>
#include <WukBinascii.cc>
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

int main()
{
    // const wByte *key_left              = (const wByte *)"abcdef0123456789abcdef0123456789";
    // const wByte *iv_left               = (const wByte *)"abcdef0123456789";
    // wuk::crypto::Counter counter_left  = {"sngrotesque", 776};
    // const wByte *key_right             = (const wByte *)"abcdef0123456789abcdef0123456789";
    // const wByte *iv_right              = (const wByte *)"abcdef0123456789";
    // wuk::crypto::Counter counter_right = {"sngrotesquE", 776};

    char null_array[32]{};
    const wByte *key_left = (const wByte *)null_array;
    const wByte *iv_left = (const wByte *)null_array;
    wuk::crypto::Counter counter_left = {{null_array, 12}, 0x1000};
    const wByte *key_right = (const wByte *)null_array;
    const wByte *iv_right = (const wByte *)null_array;
    wuk::crypto::Counter counter_right = {{null_array, 12}, 0x1001};

    const wByte *root_key = (const wByte *)"sngrotesque0123456789abcdef.-+=~";
    const wByte *root_iv = (const wByte *)"abcdef0123456789";
    wuk::crypto::Counter root_counter("helloworld", 1);

    test1(root_key, root_iv, root_counter);
    keystream_chack(key_left, iv_left, counter_left, key_right, iv_right, counter_right, 3);
    speed_test(1024 * 1024 * 1024, root_key, root_iv, root_counter);
    // encrypt_file("", "", root_key, root_iv, root_counter);

    return 0;
}
