// python make.py test\main.cc -O3 -Wall -DWUK_EXPORTS -lws2_32 -lz
#include <config/WukConfig.hh>

#include <network/WukException.cc>
#include <network/WukSocket.cc>
#include <network/WukPacket.cc>

#include <WukPadding.cc>
#include <WukBuffer.cc>
#include <WukRandom.cc>
#include <WukMisc.cc>
#include <WukTime.cc>

#include <iostream>
#include <iomanip>
#include <memory>

using namespace std;

void WukBuffer_test()
{
    wuk::Buffer buffer;

    // wuk::misc::print_hex(buffer.get_data(), buffer.get_length(), 32, 1, 0);
    // wuk::misc::print_pybytes(buffer.get_data(), buffer.get_length(), 1);
}

int main(int argc, char **argv)
{
    WukBuffer_test();

    return 0;
}
