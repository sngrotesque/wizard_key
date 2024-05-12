#include <crypto/snc.hpp>
#include <wmkc_time.hpp>

using namespace wmkcCrypto;

int main(int argc, char **argv)
{
    wByte *data = wmkcNull;
    wSize size = 16777216;
    wmkcTime time;
    wByte key[32];
    wByte iv[32];

    wmkcSNC snc(key, iv, SNC_keyMode::SNC_768);

    data = new wByte[size];

    double start = time.time();
    snc.encrypt(data, size, SNC_XcryptMode::CBC);
    double stop = time.time();

    std::cout << "time used: " << (stop - start) << std::endl;

    delete[] data;
    return 0;
}
