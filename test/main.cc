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
        wuk::misc::print_hex(packet_buffer.get_data(), packet_buffer.get_size(), 16, 1, 0);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return;
    }
}

void wuk_socket_test()
{
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);

    try {
        wuk::net::Socket fd{AF_INET, SOCK_STREAM, IPPROTO_TCP};

        fd.setsockopt(SOL_SOCKET, SO_SNDTIMEO, {1});
        fd.setsockopt(SOL_SOCKET, SO_RCVTIMEO, {1});
        fd.setsockopt(SOL_SOCKET, SO_REUSEADDR, {1});

        fd.connect("passport.bilibili.com", 80);
        fd.send("GET / HTTP/1.1\r\nHost: passport.bilibili.com\r\nUser-Agent: android\r\n\r\n");

        std::cout << fd.recv(4096);

        fd.shutdown(wuk::net::SD_SW::BOTH);
        fd.close();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    WSACleanup();
}

void WukBuffer_test()
{
    wuk::Buffer buffer{4096};

    wByte temp[3072]{};
    memset(temp, 0xff, sizeof(temp));

    memcpy(buffer.get_data(), temp, sizeof(temp));

    wuk::misc::print_hex(buffer.get_data(), sizeof(temp), 32, 1, 0);
}

int main(int argc, char **argv)
{
    WukBuffer_test();

    return 0;
}
