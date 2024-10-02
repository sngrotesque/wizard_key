#include <config/WukConfig.hh>

#include <network/WukException.cc>
#include <network/WukSocket.cc>
#include <network/WukPacket.cc>

#include <WukMisc.cc>

#include <iostream>

int main(int argc, char **argv)
{
    wuk::net::PacketEndianness endian;

    wByte array[4]{};
    uint32_t number{123456};

    endian.write_bytearray(array, sizeof(array), number);

    wuk::misc::print_hex(array, sizeof(array), 8, 1, 0);

    uint32_t result = endian.read_bytearray<uint32_t>(array);

    std::cout << "result: " << result << std::endl;

    return 0;
}
