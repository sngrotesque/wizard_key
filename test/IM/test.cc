#include "packet.pb.h"
#include "packet.pb.cc"

#include <WukRandom.hh>
#include <WukMisc.hh>
#include <WukTime.hh>

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace wuk;
using namespace wuk::net;
using namespace wuk::misc;
using namespace std;

string get_fixed(double x)
{
    stringstream ss;
    ss << fixed << setprecision(4) << x;
    return ss.str();
}

int main()
{
    wuk::Random random;
    wuk::Time timer;

    WukPacket send_packet;
    WukPacket recv_packet;

    // 设置消息包数据
    const char message[] = {
        "GET / HTTP/1.1\r\n"
        "Host: exmple.com\r\n"
        "Accept: */*\r\n"
        "User-Agent: Android\r\n\r\n"
    };
    size_t message_length = sizeof(message) - 1;
    send_packet.set_msg_type(MessageType::DATA);
    send_packet.set_msg_seq(static_cast<wU32>(random.rand()));
    send_packet.set_seg_id(0);
    send_packet.set_proto_ver(0x01);
    send_packet.set_msg_id(static_cast<wU32>(random.rand()));
    send_packet.set_msg_size(message_length);
    send_packet.set_sender_id(random.rand());
    send_packet.set_recipient_id(random.rand());
    send_packet.set_time_stamp(timer.time());
    send_packet.set_message(message, message_length);
    // 序列化消息包数据并显示
    string a = send_packet.SerializeAsString();
    cout << "Send packet buffer:\n";
    print_hex((wByte *)a.data(), a.length(), 16, true, true);

    // 读取序列化数据并显示
    cout << "Recv packet buffer:\n";
    recv_packet.ParseFromString(a);
    string recv_message = recv_packet.message();
    cout << "\tMessage type:         " << static_cast<wU32>(recv_packet.msg_type()) << "\n"
         << "\tMessage seq:          " << recv_packet.msg_seq() << "\n"
         << "\tMessage seg id:       " << recv_packet.seg_id() << "\n"
         << "\tMessage proto ver:    " << recv_packet.proto_ver() << "\n"
         << "\tMessage id:           " << recv_packet.msg_id() << "\n"
         << "\tMessage size:         " << recv_packet.msg_size() << "\n"
         << "\tMessage sender id:    " << recv_packet.sender_id() << "\n"
         << "\tMessage recipient id: " << recv_packet.recipient_id() << "\n"
         << "\tMessage time stamp:   " << get_fixed(recv_packet.time_stamp()) << "\n"
         << "\tMessage:              " << get_pybytes((wByte *)recv_message.data(), recv_message.length(), false) << endl;

    return 0;
}
