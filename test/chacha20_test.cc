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

void keystream_test(wU32 count)
{
    const wByte *cc20kl = (wByte *)"abcdef0123456789abcdef0123456789";
    const wByte *cc20nl = (wByte *)"abcdef012345";
    const wU32   cc20cl = 1535;

    const wByte *cc20kr = (wByte *)"bbcdef0123456789abcdef0123456789";
    const wByte *cc20nr = (wByte *)"abcdef012345";
    const wU32   cc20cr = 1535;

    wuk::crypto::ChaCha20 cc20l(cc20kl, cc20nl, cc20cl);
    wuk::crypto::ChaCha20 cc20r(cc20kr, cc20nr, cc20cr);

    cc20l.init();
    cc20r.init();

    wByte data[4]{};
    for (wU32 c = 0; c < count; ++c) {
        std::cout << "Keystream test, count: " << c + 1 << ".\n";
        wuk::misc::print_diff_hex((uint8_t *)cc20l.state, (uint8_t *)cc20r.state,
                                  sizeof(cc20l.state), sizeof(cc20r.state), 16, true);

        cc20r.xcrypt(data, sizeof data);
        cc20l.xcrypt(data, sizeof data);
    }
}

int main()
{
    // chacha20_test();
    // speed_test(1024 * pow(1024, 2));
    keystream_test(4);

    return 0;
}
