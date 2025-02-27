#include <crypto/WukSSC.hh>
#include <crypto/WukUtils.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <crypto/WukSSC.cc>
#include <crypto/WukUtils.cc>
#include <WukTime.cc>
#include <WukMisc.cc>

#include <iostream>
#include <iomanip>
#include <chrono>

void print_hex_data(const wByte *data1, const wByte *data2, wSize len1, wSize len2, wU32 hex_per_line, bool indent)
{
    wSize max_len = len1 > len2 ? len1 : len2; // 取两个数据的最大长度
    wSize i = 0;
    wU32 j;

    while (i < max_len) {
        // 缩进
        if (indent) {
            printf("\t");
        }

        // 打印第一个数据的十六进制
        for (j = 0; (j < hex_per_line) && ((i + j) < len1); ++j) {
            if (data1[i + j] == 0) {
                printf("\x1b[91m%02x \x1b[0m", data1[i + j]);
            } else if (data1[i + j] == 255) {
                printf("\x1b[93m%02x \x1b[0m", data1[i + j]);
            } else {
                printf("%02x ", data1[i + j]);
            }
        }
        // 补齐第一个数据的空格
        for (j = (len1 - i); j < hex_per_line; ++j) {
            printf("   "); // 三个空格对齐
        }

        // 打印分隔符
        printf("\t\t");

        // 打印第二个数据的十六进制
        for (j = 0; (j < hex_per_line) && ((i + j) < len2); ++j) {
            if (data2[i + j] == 0) {
                printf("\x1b[91m%02x \x1b[0m", data2[i + j]);
            } else if (data2[i + j] == 255) {
                printf("\x1b[93m%02x \x1b[0m", data2[i + j]);
            } else {
                printf("%02x ", data2[i + j]);
            }
        }
        // 补齐第二个数据的空格
        for (j = (len2 - i); j < hex_per_line; ++j) {
            printf("   "); // 三个空格对齐
        }

        // 换行
        printf("\n");

        // 更新索引
        i += hex_per_line;
    }
}

void test1()
{
    const wByte *key = (const wByte *)"bbcdef0123456789abcdef0123456789";
    const wByte *iv  = (const wByte *)"bbcdef0123456789";
    wuk::crypto::Counter counter("sngrotesque", 1);

    wuk::crypto::SSC ssc(key, iv, counter);

    char test_plaintext[256] = {
        "gET /qrcode/getLoginUrl HTTP/1.1\r\n"
        "Host: passport.bilibili.com\r\n"
        "Accept: application/json; q=0.9, */*\r\n"
        "Connection: keep-alive\r\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:129.0) Gecko/20100101 Firefox/129.0\r\n\r\n"
    };
    wByte *buffer = (wByte *)test_plaintext;
    wSize length = strlen(test_plaintext);

    std::cout << "Plaintext:\n";
    wuk::misc::print_hex(buffer, length, 32, true, true);

    ssc.xcrypt(buffer, length);

    std::cout << "Ciphertext:\n";
    wuk::misc::print_hex(buffer, length, 32, true, true);
}

void test2()
{
    const wByte *key_left = (const wByte *)"abcdef0123456789abcdef0123456789";
    const wByte *iv_left  = (const wByte *)"abcdef0123456789";

    const wByte *key_right = (const wByte *)"bbcdef0123456789abcdef0123456789";
    const wByte *iv_right  = (const wByte *)"abcdef0123456789";

    wuk::crypto::Counter counter("sngrotesque", 21902002);

    wuk::crypto::SSC ssc_left(key_left, iv_left, counter);
    wuk::crypto::SSC ssc_right(key_right, iv_right, counter);

    for (wU32 c = 0; c < 4; ++c) {
        std::cout << "Key Stream:\n";

        print_hex_data(
            ssc_left.get_keystream(), ssc_right.get_keystream(),
            wuk::crypto::WUK_SSC_KSLEN, wuk::crypto::WUK_SSC_KSLEN,
            16, true
        );

        ssc_left.keystream_update();
        ssc_right.keystream_update();
    }
}

void speed_test(wSize length)
{
    wByte *buffer = new (std::nothrow) wByte[length];

    if (!buffer) {
        std::cerr << "buffer malloc error.\n";
        return;
    }

    const wByte *key = (const wByte *)"bbcdef0123456789abcdef0123456789";
    const wByte *iv  = (const wByte *)"abcdef0123456789";
    wuk::crypto::Counter counter("sngrotesque", 1);

    wuk::crypto::SSC ssc(key, iv, counter);
    wuk::Time timer;

    double start_time = timer.time();
    ssc.xcrypt(buffer, length);
    double stop_time = timer.time();

    std::cout << std::fixed << std::setprecision(4) << "Time taken: " << (stop_time - start_time) << "\n";
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[0]) << "\n";

    delete[] buffer;
}

int main()
{
    // speed_test(1024 * 1024 * 1024);

    test1();
    // test2();

    return 0;
}
