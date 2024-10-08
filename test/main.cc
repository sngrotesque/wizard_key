#include <config/WukConfig.hh>

#include <network/WukException.cc>
#include <network/WukSocket.cc>
#include <network/WukPacket.cc>

#include <WukBuffer.cc>
#include <WukMisc.cc>
#include <WukTime.cc>

#include <iostream>
#include <iomanip>

void free_ptr(wByte **p)
{
    free(*p);
    *p = nullptr;
}

void build_packet_test()
{
    wuk::net::Packet packet_packet{};
    wuk::Time timer{};

    packet_packet.flag.set_flag(
        wuk::net::PACKET_IS_FILE |
        wuk::net::PACKET_IS_OVER |
        wuk::net::PACKET_IS_XCRTPT
    );

    // 构建元数据
    packet_packet.mate.write_mate_time(timer.time());
    packet_packet.mate.write_mate_session_id(718274593);
    packet_packet.mate.write_mate_sequence(0xeeeeeeee);
    packet_packet.mate.write_mate_crc();

    // 构建包数据
    packet_packet.data.write_data_data("hello, world.\n");
    packet_packet.data.write_data_crc();

    try {
        wuk::Buffer packet_buffer = packet_packet.build_packet_data();
        wuk::misc::print_hex(packet_buffer.data, packet_buffer.size, 16, 1, 0);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return;
    }
}

/*
19 41 d9 c1 5a 4a dd c9 3f 00 00 00 00 2a d0 00
21 ee ee ee ee 41 d2 c5 6b 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00
*/
int main(int argc, char **argv)
{
    build_packet_test();

    return 0;
}
