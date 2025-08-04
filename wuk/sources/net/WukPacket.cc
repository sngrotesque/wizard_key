#include <net/WukPacket.hh>

#include <zlib.h>

#define RETURN return *this

// PRIVATE: Function

bool wuk::net::WukPacket::validate() const
{
    MessageType m_type_flag = this->m_message.m_type();

    if (this->m_message.m_proto_ver() < 0x01) {
        return false;
    }

    if (this->m_message.m_content().size() != this->m_message.m_length()) {
        return false;
    }

    if ((this->m_message.m_sender() == 0) ||
        (this->m_message.m_recipient() == 0)) {
        return false;
    }

    if (this->m_message.m_timestamp() <= 0) {
        return false;
    }

    // 如果未设置分包包id却设置了SEGMENT位
    if (this->m_message.m_segment_id() == 0 &&
        (m_type_flag & MessageType::SEGMENT)) {
        return false;
    }
    // 如果设置了OVER位但未定义此为分段包
    if ( (m_type_flag & MessageType::OVER) &&
        !(m_type_flag & MessageType::SEGMENT)) {
        return false;
    }
    // 如果设置了IMAGE位但未定义此为文件包
    if ( (m_type_flag & MessageType::IMAGE) &&
        !(m_type_flag & MessageType::FILE)) {
        return false;
    }

    return true;
}

// PUBLIC: Setter

wuk::net::WukPacket &wuk::net::WukPacket::set_type(MessageType type)
{
    this->m_message.set_m_type(type);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::add_flag(MessageType flag)
{
    wU32 cur_flag = static_cast<int>(this->m_message.m_type());
    wU32 new_flag = static_cast<int>(flag);
    this->m_message.set_m_type(static_cast<MessageType>(cur_flag | new_flag));
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_sequence(wU32 seq)
{
    this->m_message.set_m_sequence(seq);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_segment_id(wU32 m_segment_id, bool is_last)
{
    this->m_message.set_m_segment_id(m_segment_id);

    if (m_segment_id) {
        this->add_flag(MessageType::SEGMENT);
    }

    if (is_last) {
        this->add_flag(MessageType::OVER);
    }

    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_proto_ver(wU32 version)
{
    if (version < 0x01) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::net::WukPacket::set_proto_ver",
            "Protocol version too low.");
    }
    this->m_message.set_m_proto_ver(version);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_ids(wU64 sender, wU64 recipient)
{
    this->m_message.set_m_sender(sender);
    this->m_message.set_m_recipient(recipient);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_sender(wU64 id)
{
    this->m_message.set_m_sender(id);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_recipient(wU64 id)
{
    this->m_message.set_m_recipient(id);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_timestamp(double time_val)
{
    if (time_val == 0) {
        time_val = timer.time();
    }
    this->m_message.set_m_timestamp(time_val);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_message_id(wU32 id)
{
    this->m_message.set_m_id(id);
    RETURN;
}

wuk::net::WukPacket &wuk::net::WukPacket::set_message(const void *buffer, wSize length)
{
    this->m_message.set_m_length(length);
    this->m_message.set_m_content(buffer, length);
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
    return this->m_message.m_type();
}

bool wuk::net::WukPacket::has_flag(MessageType flag) const
{
    return (this->get_type() & flag) == flag;
}

wU32 wuk::net::WukPacket::get_sequence() const
{
    return this->m_message.m_sequence();
}

wU32 wuk::net::WukPacket::get_segment_id() const
{
    return this->m_message.m_segment_id();
}

wU32 wuk::net::WukPacket::get_proto_ver() const
{
    return this->m_message.m_proto_ver();
}

wU64 wuk::net::WukPacket::get_sender() const
{
    return this->m_message.m_sender();
}

wU64 wuk::net::WukPacket::get_recipient() const
{
    return this->m_message.m_recipient();
}

double wuk::net::WukPacket::get_timestamp() const
{
    return this->m_message.m_timestamp();
}

wU32 wuk::net::WukPacket::get_message_id() const
{
    return this->m_message.m_id();
}

wSize wuk::net::WukPacket::get_message_size() const
{
    return this->m_message.m_content().length();
}

const std::string &wuk::net::WukPacket::get_message() const
{
    return this->m_message.m_content();
}

const wuk::Buffer wuk::net::WukPacket::get_message(int) const
{
    const std::string &s = this->m_message.m_content();
    const wByte *buffer = reinterpret_cast<const wByte *>(s.data());
    const wSize length = s.length();
    return wuk::Buffer(buffer, length);
}

// PUBLIC: Function

const std::string wuk::net::WukPacket::serialize()
{
    if (this->validate() == false) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::net::WukPacket::serialize",
            "Data member validation failed.");
    }

    // 防御性拷贝避免多线程竞争
    wuk::net::Message tmp = this->m_message;
    if (this->m_message.m_id() == 0) {
        std::string s = tmp.SerializeAsString();
        tmp.set_m_id(crc32(0, reinterpret_cast<wByte *>(s.data()), s.length()));
    }

    return this->m_message.SerializeAsString();
}

wuk::net::WukPacket &wuk::net::WukPacket::parse(const std::string &buffer)
{
    return this->parse_from(buffer.data(), buffer.length());
}

wuk::net::WukPacket &wuk::net::WukPacket::parse_from(const void *buffer, wSize length)
{
    this->m_message.Clear();

    if (!this->m_message.ParseFromArray(buffer, static_cast<int>(length))) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::net::WukPacket::parse_from",
            "Invalid binary data");
    }

    if (!this->validate()) {
        this->m_message.Clear();
        throw wuk::Exception(wuk::Error::ERR, "wuk::net::WukPacket::parse",
            "Parsed data validation failed");
    }

    return *this;
}
