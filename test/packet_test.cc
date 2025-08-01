#include <net/WukPacket.hh>

#include <WukMisc.hh>

using namespace wuk::net;
using namespace wuk::misc;

int main()
{
    WukPacket packet(MsgT::MESSAGE, 1, 0, 0, 0, 0x4444444444444444, 0xaaaaaaaaaaaaaaaa);

    packet.set_message(wuk::Buffer{"hello, world.\nCrypto"});

    wuk::Buffer data = packet.get_packet();

    printf("Packet data:\n");
    print_hex(data.get_data(), data.get_length(), 16, true, true);

    return 0;
}
