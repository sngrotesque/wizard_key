#include <config/WukConfig.hh>

#include <network/WukException.cc>
#include <network/WukSocket.cc>
#include <network/WukPacket.cc>

#include <WukMisc.cc>
#include <WukTime.cc>

#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
    wuk::net::Packet packet_packet{};
    wuk::Time timer{};

    wByte *packet{};
    wSize packet_length{};

    packet_packet.flag.set_flag(
        wuk::net::PACKET_IS_FILE |
        wuk::net::PACKET_IS_OVER |
        wuk::net::PACKET_IS_XCRTPT
    );

    // 构建元数据
    packet_packet.mate.write_mate_time(timer.time());
    packet_packet.mate.write_mate_session_id(718274593);
    packet_packet.mate.write_mate_sequence(0);
    packet_packet.mate.write_mate_crc();

    // 构建包数据
    packet_packet.data.write_data_data("hello, world.\n");
    packet_packet.data.write_data_crc();

    packet_length = 1 + 24 + packet_packet.data.read_data_length() + 4;
    packet = (wByte *)malloc(packet_length);
    wuk::memory_zero(packet, packet_length);

    *packet = static_cast<wByte>(packet_packet.flag.packet_flag_value);
    memcpy(packet, &packet_packet.flag.packet_flag_value, 1);
    // memcpy(packet + 1, )

    wuk::misc::print_hex(packet, packet_length, 16, 1, 0);

    free(packet);
    packet = nullptr;

    return 0;
}
