#include <crypto/WukChaCha20.hh>
#include <WukMisc.hh>
#include <WukTime.hh>

#include <crypto/WukChaCha20.cc>
#include <WukMisc.cc>
#include <WukTime.cc>

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>

const     wByte *chacha20_key   = (wByte *)"bbcdef0123456789abcdef0123456789";
const     wByte *chacha20_nonce = (wByte *)"abcdef012345";
constexpr wU32 chacha20_counter = 1535;

void chacha20_test()
{
    wuk::crypto::ChaCha20 chacha20(chacha20_key, chacha20_nonce, chacha20_counter);

    chacha20.init();

    char _plaintext[256] = {"Hello, world.\n"};
    wByte *plaintext = (wByte *)_plaintext;
    wSize length = strlen(_plaintext);

    std::cout << "Plaintext:\n";
    wuk::misc::print_hex(plaintext, length, 16, true, true);

    chacha20.xcrypt(plaintext, length);

    std::cout << "Ciphertext:\n";
    wuk::misc::print_hex(plaintext, length, 16, true, true);
}

void speed_test(size_t length)
{
    wByte *buffer = new (std::nothrow) wByte[length];
    if (!buffer) {
        throw std::runtime_error("buffer malloc error.");
    }
    wuk::crypto::ChaCha20 chacha20(chacha20_key, chacha20_nonce, chacha20_counter);
    wuk::Time timer;

    chacha20.init();

    std::cout << "Test algorithm encryption speed, data size: "
              << static_cast<wSize>(length / pow(1024, 2)) << " MB.\n";

    double start_time = timer.time();
    chacha20.xcrypt(buffer, length);
    double stop_time = timer.time();
    std::cout << "Time taken: " << std::fixed << std::setprecision(4)
                                << (stop_time - start_time)
                                << ".\n";

    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<wU32>(buffer[0]);

    delete[] buffer;
}

int main()
{
    // chacha20_test();
    speed_test(1024 * pow(1024, 2));

    return 0;
}
