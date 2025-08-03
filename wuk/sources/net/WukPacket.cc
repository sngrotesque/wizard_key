#include <net/WukPacket.hh>

#define RETURN return *this

// PRIVATE: Function

bool wuk::net::WukPacket::validate() const
{
    if (this->_message.proto_ver() < 0x01) {
        return false;
    }
    if (this->_message.seg_id() == 0 && (this->_message.msg_type() & SEGMENT)) {
        return false;
    }

    if (this->_message.message().size() != this->_message.msg_size()) {
        return false;
    }

    if ((this->_message.sender_id() == 0) ||
        (this->_message.recipient_id() == 0)) {
        return false;
    }

    return true;
}

// PUBLIC: Setter

wuk::net::WukPacket &wuk::net::WukPacket::set_type(MessageType type)
{
    this->_message.set_msg_type(type);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::add_flag(MessageType flag)
{
    wU32 cur_flag = static_cast<int>(this->_message.msg_type());
    wU32 new_flag = static_cast<int>(flag);
    this->_message.set_msg_type(static_cast<MessageType>(cur_flag | new_flag));
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_seq(wU32 seq)
{
    this->_message.set_msg_seq(seq);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_segment(wU32 seg_id, bool is_last)
{
    this->_message.set_seg_id(seg_id);

    if (seg_id) {
        this->add_flag(MessageType::SEGMENT);
    }

    if (is_last) {
        this->add_flag(MessageType::OVER);
    }

    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_protocol(wU32 version)
{
    if (version < 0x01) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::net::WukPacket::set_protocol",
            "Protocol version too low.");
    }
    this->_message.set_proto_ver(version);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_ids(wU64 sender, wU64 recipient)
{
    this->_message.set_sender_id(sender);
    this->_message.set_recipient_id(recipient);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_timestamp(double time_val)
{
    if (time_val == 0) {
        time_val = timer.time();
    }
    this->_message.set_time_stamp(time_val);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_message_id(wU32 id)
{
    this->_message.set_msg_id(id);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_message(const void *buffer, wSize length)
{
    this->_message.set_msg_size(length);
    this->_message.set_message(buffer, length);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_message(const std::string &buffer)
{
    return this->set_message(reinterpret_cast<const wByte *>(buffer.data()),
                            buffer.length());
}

wuk::net::WukPacket &wuk::net::WukPacket::set_message(const wuk::Buffer &buffer)
{
    return this->set_message(buffer.get_data(), buffer.get_length());
}

// PUBLIC: Getter

wuk::net::MessageType wuk::net::WukPacket::get_type() const
{
    return this->_message.msg_type();
}

bool wuk::net::WukPacket::has_flag(MessageType flag) const
{
    return (this->get_type() & flag) == flag;
}

wU32 wuk::net::WukPacket::get_seq() const
{
    return this->_message.msg_seq();
}

wU32 wuk::net::WukPacket::get_segment() const
{
    return this->_message.seg_id();
}

wU32 wuk::net::WukPacket::get_protocol() const
{
    return this->_message.proto_ver();
}

wU64 wuk::net::WukPacket::get_sender() const
{
    return this->_message.sender_id();
}

wU64 wuk::net::WukPacket::get_recipient() const
{
    return this->_message.recipient_id();
}

double wuk::net::WukPacket::get_timestamp() const
{
    return this->_message.time_stamp();
}

wU32 wuk::net::WukPacket::get_message_id() const
{
    return this->_message.msg_id();
}

wSize wuk::net::WukPacket::get_message_size() const
{
    return this->_message.message().length();
}

const std::string &wuk::net::WukPacket::get_message() const
{
    return this->_message.message();
}

const wuk::Buffer wuk::net::WukPacket::get_message(int) const
{
    return wuk::Buffer(reinterpret_cast<const wByte *>(this->_message.message().data()),
                    this->_message.message().length());
}

// PUBLIC: Function

const std::string wuk::net::WukPacket::serialize()
{
    if (this->validate() == false) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::net::WukPacket::serialize",
            "Data member validation failed.");
    }

    if (this->_message.msg_id() == 0) {
        std::string s = this->_message.SerializeAsString();
        wU32 crc_val = crc32(0, reinterpret_cast<wByte *>(s.data()), s.length());
        this->_message.set_msg_id(crc_val);
    }
    return this->_message.SerializeAsString();
}

wuk::net::WukPacket &wuk::net::WukPacket::parse(const std::string &buffer)
{
    return this->parse_from(buffer.data(), buffer.length());
}

wuk::net::WukPacket &wuk::net::WukPacket::parse_from(const void *buffer, wSize length)
{
    if (!this->_message.ParseFromArray(buffer, static_cast<int>(length))) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::net::WukPacket::parse_from",
            "Invalid binary data");
    }

    if (!this->validate()) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::net::WukPacket::parse",
            "Parsed data validation failed");
    }

    return *this;
}
