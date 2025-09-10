// 使用此文件之前，请先查看`crypto/WukOP4Easy.hh`中的说明
// 否则由此导致的任何安全问题，由你自己承担。
#include <crypto/WukOP4Easy.hh>
#include <WukBase64.hh>
#include <WukMisc.hh>

#include <iostream>

int main()
{
    wuk::crypto::OP4Easy op4;

    const char content[] = {
        // "gET / HTTP/1.1\r\n"
        // "Accept: */*\r\n"
        // "Connection: close\r\n"
        // "User-Agent: Android\r\n\r\n"
        "\x00\x01\x02\x03"
        "\x04\x05\x06\x07"
        "\x08\x09\x0a\x0b"
        "\x0c\x0d\x0e\x0f"
    };
    constexpr wuk::ulong length = sizeof(content) - 1;
    wuk::byte ciphertext[length]{0};

    const wuk::byte *plaintext = reinterpret_cast<const wuk::byte *>(content);

    op4.crypto_stream(ciphertext, plaintext, length);

    std::cout << "Plaintext:\t\t\t\t\t\t\tCiphertext:\n";
    wuk::misc::print_diff_hex(plaintext, ciphertext, length, length, 16, true);

    std::cout << "Plaintext(Base64):  " << wuk::base64::encode({plaintext, length}).to_str() << std::endl;
    std::cout << "ciphertext(Base64): " << wuk::base64::encode({ciphertext, length}).to_str() << std::endl;

    return 0;
}
