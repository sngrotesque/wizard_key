#include <crypto/WukSDSE.hh>
#include <crypto/WukUtils.hh>
#include <WukBinascii.hh>
#include <WukBuffer.hh>
#include <WukMisc.hh>
#include <WukTime.hh>

#include <iostream>
#include <openssl/evp.h>

void speed_test(wSize length)
{
    wByte key_or_iv_nonce[32]{};

    wuk::crypto::SDSE sdse(key_or_iv_nonce, key_or_iv_nonce);

    wByte *buffer = wuk::m_alloc<wByte *>(length);
    if (!buffer) {
        std::cerr << "Memory allocation failed.\n";
        return;
    }

    double start_time = wuk::Time::time();
    sdse.xcrypt(buffer, length);
    double end_time = wuk::Time::time();
    double elapsed_time = end_time - start_time;
    std::cout << "Encryption time: " << elapsed_time << " seconds" << std::endl;
    std::cout << "Speed: " << (length / (1024.0 * 1024.0)) / elapsed_time << " MB/s" << std::endl;

    wuk::m_free(buffer);
}

wuk::Buffer sha256(const wByte *data, size_t len) {
    wuk::Binascii binascii;
    wuk::Buffer digest(32);

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();  // 创建上下文
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, data, len);
    EVP_DigestFinal_ex(mdctx, digest.append_write(32), nullptr);
    EVP_MD_CTX_free(mdctx);

    return binascii.b2a_hex(digest);
}

void test()
{
    // constexpr wByte key[32]{
    //     0xef, 0x52, 0x0c, 0x4d, 0xbc, 0x07, 0xf3, 0x7c,
    //     0x96, 0xbf, 0xc2, 0xb7, 0x13, 0xba, 0x06, 0x32,
    //     0x6a, 0x30, 0xc7, 0x5e, 0x58, 0x79, 0x92, 0x28,
    //     0x9a, 0x2a, 0xf5, 0xfd, 0x63, 0xf8, 0x90, 0xd3};
    // constexpr wByte nonce[20]{
    //     0xbb, 0xe0, 0x6f, 0x1f, 0xd6, 0x16, 0x1e, 0x11,
    //     0x4e, 0x05, 0xa7, 0x90, 0x6e, 0xa5, 0x37, 0xaa,
    //     0xed, 0x39, 0xe2, 0x7f};
    wByte key[32]{};
    wByte nonce[20]{};

    // for(int i=0;i<32;++i) {key[i]=1;}
    // for(int i=0;i<20;++i) {nonce[i]=1;}

    wuk::crypto::SDSE sdse(key, nonce, 0);

    std::cout << "Initial key stream:\n";
    wuk::misc::print_hex(sdse.get_keystream(), 64, 32, false, true);

    char _plaintext[] = {
        "This is a test string for SDSE encryption."
    };
    wByte *buffer = reinterpret_cast<wByte *>(_plaintext);
    wSize length = strlen(_plaintext);

    std::cout << "Original plaintext:\n";
    wuk::misc::print_hex(buffer, length, 32, true, true);

    // wByte copy[sizeof(_plaintext) + 1]{0};
    // memcpy(copy, buffer, length);

    sdse.xcrypt(buffer, length);

    std::cout << "Encrypted text:\n";
    wuk::misc::print_hex(buffer, length, 32, true, true);

    std::cout << "Ciphertext digest: " << sha256(buffer, length).get_cstr() << std::endl;

    // std::cout << "Original plaintext (Rawstream):\n\t";
    // wuk::misc::print_pybytes(copy, length, true);

    // std::cout << "Encrypted text (Rawstream):\n\t";
    // wuk::misc::print_pybytes(buffer, length, true);
}

int main()
{
    // speed_test(1024ULL*1024*1024);
    test();

    return 0;
}
