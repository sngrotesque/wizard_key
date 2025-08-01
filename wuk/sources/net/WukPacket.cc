#include <net/WukPacket.hh>

// PUBLIC
wuk::net::WukPacket::WukPacket(MsgT msg_type,
                  wU32 proto_ver, wU32 seg_id, wU32 msg_seq, wU64 msg_id,
                  wU64 sender_id, wU64 recipient_id)
{
    this->proto_ver = proto_ver;
    this->seg_id = seg_id;
    this->msg_seq = msg_seq;
    this->msg_type = msg_type;

    this->msg_id = msg_id;

    this->sender_id = sender_id;
    this->recipient_id = recipient_id;
}

void wuk::net::WukPacket::set_proto_ver(wU32 value)
{
    this->proto_ver = value;
}

void wuk::net::WukPacket::set_seg_id(wU32 value)
{
    this->seg_id = value;
}

void wuk::net::WukPacket::set_msg_seq(wU32 value)
{
    this->msg_seq = value;
}

void wuk::net::WukPacket::set_msg_type(MsgT value)
{
    this->msg_type = value;
}

void wuk::net::WukPacket::set_msg_id(wU64 value)
{
    this->msg_id = value;
}

void wuk::net::WukPacket::set_sender_id(wU64 value)
{
    this->sender_id = value;
}

void wuk::net::WukPacket::set_recipient_id(wU64 value)
{
    this->recipient_id = value;
}

void wuk::net::WukPacket::set_message(wuk::Buffer message)
{
    this->message = message;
}

wuk::Buffer wuk::net::WukPacket::get_packet() noexcept
{
    wuk::Time timer;
    wuk::Buffer verify_buffer;
    wU32 crc_val{0};

    // 保持最新数据更新
    this->time_stamp = timer.time();
    this->msg_size = this->message.get_length();

    // 添加元数据
    verify_buffer.append_number(this->proto_ver);
    verify_buffer.append_number(this->seg_id);
    verify_buffer.append_number(this->msg_seq);
    verify_buffer.append_number(static_cast<wU32>(this->msg_type));

    verify_buffer.append_number(this->msg_id);
    verify_buffer.append_number(this->msg_size);

    verify_buffer.append_number(this->sender_id);
    verify_buffer.append_number(this->recipient_id);

    verify_buffer.append_number(this->time_stamp);

    // 添加消息体
    verify_buffer.append(this->message.get_data(),
                        this->message.get_length());

    // 计算出CRC校验值
    crc_val = crc32(0,  verify_buffer.get_data(),
                        verify_buffer.get_length());
    // 添加CRC32校验值
    verify_buffer.append_number(crc_val);

    return verify_buffer;
}
