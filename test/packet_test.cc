#include <net/WukPacket.hh>
#include <net/WukPacket.pb.h>

#include <WukRandom.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <iostream>

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
    WukPacket packet;
    WukRandom random;
    WukTime timer;

    packet.set_type(MessageType::MESSAGE)
          .set_protocol(0x0101)
          .set_segment(0)
          .set_seq(random.rand())
          .set_timestamp(timer.time())
          .set_ids(random.rand(), random.rand())
          .set_message(std::string{"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"});
    std::string res = packet.serialize();

    cout << "Send packet buffer:\n";
    print_hex((wByte *)res.data(), res.length(), 16, true, true);

    // 读取序列化数据并显示
    cout << "Recv packet buffer:\n";
    WukPacket recv_packet;
    recv_packet.parse(res);
    std::string recv_message = recv_packet.get_message();
    cout << "\tMessage type:         " << static_cast<wU32>(recv_packet.get_type()) << "\n"
         << "\tMessage seq:          " << recv_packet.get_seq() << "\n"
         << "\tMessage seg id:       " << recv_packet.get_segment() << "\n"
         << "\tMessage proto ver:    " << recv_packet.get_protocol() << "\n"
         << "\tMessage id:           " << recv_packet.get_message_id() << "\n"
         << "\tMessage size:         " << recv_packet.get_message_size() << "\n"
         << "\tMessage sender id:    " << recv_packet.get_sender() << "\n"
         << "\tMessage recipient id: " << recv_packet.get_recipient() << "\n"
         << "\tMessage time stamp:   " << get_fixed(recv_packet.get_timestamp()) << "\n"
         << "\tMessage:              " << get_pybytes((wByte *)recv_message.data(), recv_message.length(), false) << endl;

    return 0;
}
