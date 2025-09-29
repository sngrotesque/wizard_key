#include <common/includes/WukPacket.hh>

#include <zlib.h>

#define RETURN return *this

namespace wuk::im {
// PRIVATE: Function
    bool Packet::validate() const noexcept
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
    Packet &Packet::set_type(MessageType type) noexcept
    {
        this->m_message.set_m_type(type);
        RETURN;
    }

    Packet &Packet::add_flag(MessageType flag) noexcept
    {
        wuk::u32 cur_flag = static_cast<int>(this->m_message.m_type());
        wuk::u32 new_flag = static_cast<int>(flag);
        this->m_message.set_m_type(static_cast<MessageType>(cur_flag | new_flag));
        RETURN;
    }

    Packet &Packet::set_sequence(wuk::u32 seq) noexcept
    {
        this->m_message.set_m_sequence(seq);
        RETURN;
    }

    Packet &Packet::set_segment_id(wuk::u32 m_segment_id, bool is_last) noexcept
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

    Packet &Packet::set_proto_ver(wuk::u32 version)
    {
        if (version < 0x01) {
            throw wuk::Exception(wuk::Error::ERR,
                "wuk::net::Packet::set_proto_ver",
                "Protocol version too low.");
        }
        this->m_message.set_m_proto_ver(version);
        RETURN;
    }

    Packet &Packet::set_ids(wuk::u64 sender, wuk::u64 recipient) noexcept
    {
        this->m_message.set_m_sender(sender);
        this->m_message.set_m_recipient(recipient);
        RETURN;
    }

    Packet &Packet::set_sender(wuk::u64 id) noexcept
    {
        this->m_message.set_m_sender(id);
        RETURN;
    }

    Packet &Packet::set_recipient(wuk::u64 id) noexcept
    {
        this->m_message.set_m_recipient(id);
        RETURN;
    }

    Packet &Packet::set_timestamp(wuk::f64 time_val) noexcept
    {
        if (time_val == 0) {
            time_val = this->m_time.time<wuk::f64>();
        }
        this->m_message.set_m_timestamp(time_val);
        RETURN;
    }

    Packet &Packet::set_message_id(wuk::u32 id) noexcept
    {
        this->m_message.set_m_id(id);
        RETURN;
    }

    Packet &Packet::set_message(const void *buffer, wuk::ulong length) noexcept
    {
        this->m_message.set_m_length(length);
        this->m_message.set_m_content(buffer, length);
        RETURN;
    }

    Packet &Packet::set_message(const std::string &buffer) noexcept
    {
        return this->set_message(reinterpret_cast<const wuk::byte *>(buffer.data()),
                                buffer.length());
    }

    Packet &Packet::set_message(const wuk::Buffer &buffer) noexcept
    {
        return this->set_message(buffer.data(), buffer.size());
    }

    // PUBLIC: Getter

    MessageType Packet::get_type() const noexcept
    {
        return this->m_message.m_type();
    }

    bool Packet::has_flag(MessageType flag) const noexcept
    {
        return (this->get_type() & flag) == flag;
    }

    wuk::u32 Packet::get_sequence() const noexcept
    {
        return this->m_message.m_sequence();
    }

    wuk::u32 Packet::get_segment_id() const noexcept
    {
        return this->m_message.m_segment_id();
    }

    wuk::u32 Packet::get_proto_ver() const noexcept
    {
        return this->m_message.m_proto_ver();
    }

    wuk::u64 Packet::get_sender() const noexcept
    {
        return this->m_message.m_sender();
    }

    wuk::u64 Packet::get_recipient() const noexcept
    {
        return this->m_message.m_recipient();
    }

    wuk::f64 Packet::get_timestamp() const noexcept
    {
        return this->m_message.m_timestamp();
    }

    wuk::u32 Packet::get_message_id() const noexcept
    {
        return this->m_message.m_id();
    }

    wuk::ulong Packet::get_message_size() const noexcept
    {
        return this->m_message.m_content().length();
    }

    const std::string &Packet::get_message() const noexcept
    {
        return this->m_message.m_content();
    }

    const wuk::Buffer Packet::get_message(int) const noexcept
    {
        const std::string &s = this->m_message.m_content();
        const wuk::byte *buffer = reinterpret_cast<const wuk::byte *>(s.data());
        const wuk::ulong length = s.length();
        return wuk::Buffer(buffer, length);
    }

    // PUBLIC: Function

    const std::string Packet::serialize()
    {
        if (this->validate() == false) {
            throw wuk::Exception(wuk::Error::ERR,
                "wuk::net::Packet::serialize",
                "Data member validation failed.");
        }

        // 防御性拷贝避免多线程竞争
        Message tmp = this->m_message;
        if (this->m_message.m_id() == 0) {
            std::string s = tmp.SerializeAsString();
            tmp.set_m_id(crc32(0, reinterpret_cast<wuk::byte *>(s.data()), s.length()));
        }

        return this->m_message.SerializeAsString();
    }

    Packet &Packet::parse(const std::string &buffer)
    {
        return this->parse_from(buffer.data(), buffer.length());
    }

    Packet &Packet::parse_from(const void *buffer, wuk::ulong length)
    {
        this->m_message.Clear();

        if (!this->m_message.ParseFromArray(buffer, static_cast<int>(length))) {
            throw wuk::Exception(wuk::Error::ERR,
                "wuk::net::Packet::parse_from",
                "Invalid binary data");
        }

        if (!this->validate()) {
            this->m_message.Clear();
            throw wuk::Exception(wuk::Error::ERR,
                "wuk::net::Packet::parse",
                "Parsed data validation failed");
        }

        return *this;
    }
}
