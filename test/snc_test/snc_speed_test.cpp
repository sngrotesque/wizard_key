#include <crypto/snc.hpp>
#include <wmkc_random.hpp>
#include <wmkc_time.hpp>

int main(int argc, char **argv)
{
    wmkcByte key[32];
    wmkcByte iv[32];

    wmkcCrypto::wmkcSNC snc(key, iv, wmkcCrypto::SNC_keyMode::SNC_256);
    wmkcTime time;
    wmkcByte *data = wmkcNull;
    wmkcSize size = 16777216;

    data = new wmkcByte[size];

    double start = time.time();
    snc.encrypt(data, size, wmkcCrypto::SNC_XcryptMode::CTR);
    double stop = time.time();

    std::cout << "time used: " << (stop - start) << std::endl;

    delete[] data;
    return 0;
}
