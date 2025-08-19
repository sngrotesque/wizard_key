#include <crypto/WukCommon.hh>

#include <iostream>

using namespace wuk::crypto;

int main()
{
    wuk::u32 value = 395810823;
    wuk::u32 number = 19;

    std::cout << rotl32(value, number) << std::endl;

    std::cout << rotr32(value, 32 - number) << std::endl;

    return 0;
}
