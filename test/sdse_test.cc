#include <crypto/WukSDSE.hh>
#include <crypto/WukUtils.hh>
#include <WukBase64.hh>
// #include <WukRandom.hh>
#include <WukMisc.hh>
#include <WukTime.hh>

#include <crypto/WukSDSE.cc>
#include <crypto/WukUtils.cc>
#include <WukBuffer.cc>
#include <WukBase64.cc>
// #include <WukRandom.cc>
#include <WukMisc.cc>
#include <WukTime.cc>

#include <iostream>

void speed_test()
{
    wByte key[32]{};
    wByte iv[8]{};
    wByte nonce[12]{};
    wU32 count = 0xffffffff;

    wByte temp[64]{}; wByte *tp = temp;
    for (wU32 i = 0; i < 64; temp[i] = i, i++);
    // temp[0]++;

    memcpy(key,   tp, 32); tp += 32;
    memcpy(iv,    tp, 8);  tp += 8;
    memcpy(nonce, tp, 12);

    wuk::crypto::Counter counter(nonce, sizeof nonce, count);

    wuk::crypto::SDSE sdse(key, iv, counter);

    wSize length = 1024 * 1024 * 1024; // 1 GB
    wByte *buffer = new (std::nothrow) wByte[length];

    double start_time = wuk::Time::time();
    sdse.xcrypt(buffer, length);
    double end_time = wuk::Time::time();
    double elapsed_time = end_time - start_time;
    std::cout << "Encryption time: " << elapsed_time << " seconds\n";
    std::cout << "Speed: " << (length / (1024.0 * 1024.0)) / elapsed_time << " MB/s\n";

    delete[] buffer;
}

void test()
{
    wByte key[32]{
        0xef, 0x52, 0x0c, 0x4d, 0xbc, 0x07, 0xf3, 0x7c,
        0x96, 0xbf, 0xc2, 0xb7, 0x13, 0xba, 0x06, 0x32,
        0x6a, 0x30, 0xc7, 0x5e, 0x58, 0x79, 0x92, 0x28,
        0x9a, 0x2a, 0xf5, 0xfd, 0x63, 0xf8, 0x90, 0xd3};
    wByte iv[8]{
        0xbb, 0xe0, 0x6f, 0x1f, 0xd6, 0x16, 0x1e, 0x11};
    wByte nonce[12]{
        0x4e, 0x05, 0xa7, 0x90, 0x6e, 0xa5, 0x37, 0xaa,
        0xed, 0x39, 0xe2, 0x7f};
    wU32 count = 0x315bad93;

    wuk::crypto::Counter counter(nonce, sizeof nonce, count);
    wuk::crypto::SDSE sdse(key, iv, counter);

    // char _plaintext[] = {
    //     " This is a test string for SDSE encryption.\r\n"
    //     "It will be encrypted using the SDSE algorithm.\r\n"
    //     "The SDSE algorithm is a stream cipher that uses a keystream to encrypt data.\r\n\r\n"
    // };
    // wByte *buffer = reinterpret_cast<wByte *>(_plaintext);
    // wSize length = strlen(_plaintext);
    wByte buffer[256]{};
    wSize length = 256;
    for(int x=0;x<256;++x) {
        buffer[x] = x;
    }

    std::cout << "Original plaintext:\n";
    wuk::misc::print_hex(buffer, length, 32, true, true);

    sdse.xcrypt(buffer, length);

    std::cout << "Encrypted text:\n";
    wuk::misc::print_hex(buffer, length, 32, true, true);

    // std::cout << "Encrypted text (Rawstream):\n\t";
    // wuk::misc::print_pybytes(buffer, length, true);
}

int main()
{
    speed_test();
    // test();

    return 0;
}
