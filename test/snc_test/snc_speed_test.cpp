#include <crypto/snc.hpp>
#include <wmkc_time.hpp>

using namespace wmkcCrypto;

int main(int argc, char **argv)
{
    wmkcByte *data = wmkcNull;
    wmkcSize size = 16777216;
    wmkcTime time;
    wmkcByte key[32];
    wmkcByte iv[32];

    wmkcSNC snc(key, iv, SNC_keyMode::SNC_768);

    data = new wmkcByte[size];

    double start = time.time();
    snc.encrypt(data, size, SNC_XcryptMode::CBC);
    double stop = time.time();

    std::cout << "time used: " << (stop - start) << std::endl;

    delete[] data;
    return 0;
}
