#include <config/WukConfig.hh>

#include <iostream>

#include <WukBase64.cc>
#include <WukBinascii.cc>
#include <WukRandom.cc>
#include <WukTime.cc>

int main(int argc, char **argv)
{
    wuk::Base64 wuk_base64;
    wuk::Binascii wuk_binascii;
    wuk::Random wuk_random;
    wuk::Time wuk_time;

    std::cout << "hello, world" << std::endl;

    std::cout << wuk_base64.encode("hello, world") << std::endl;
    std::cout << wuk_binascii.b2a_hex("hello, world") << std::endl;
    std::cout << wuk_random.randint(0, 0xffff) << std::endl;
    std::cout << wuk_time.time() << std::endl;

    return 0;
}
