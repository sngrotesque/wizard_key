#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <net/WukPacket.pb.h>
#include <WukBuffer.hh>
#include <WukTime.hh>

namespace wuk::net {
    class LIBWUK_API WukPacket {
    private:
        Message m_message;
        wuk::Time timer;

    private:
        bool validate() const;

    public:
        WukPacket() = default;

    public: // Setter
        WukPacket &set_type(MessageType type);
        WukPacket &add_flag(MessageType type);

        WukPacket &set_sequence(wuk::u32 seq);
        WukPacket &set_segment_id(wuk::u32 seg_id, bool is_last = false);
        WukPacket &set_proto_ver(wuk::u32 version);

        WukPacket &set_ids(wuk::u64 sender, wuk::u64 recipient);
        WukPacket &set_sender(wuk::u64 id);
        WukPacket &set_recipient(wuk::u64 id);

        WukPacket &set_timestamp(wuk::f64 time_val);

        WukPacket &set_message_id(wuk::u32 id);

        WukPacket &set_message(const void *buffer, wuk::ulong length);
        WukPacket &set_message(const std::string &buffer);
        WukPacket &set_message(const wuk::Buffer &buffer);

    public: // Getter
        MessageType get_type() const;
        bool has_flag(MessageType flag) const;

        wuk::u32 get_sequence() const;
        wuk::u32 get_segment_id() const;
        wuk::u32 get_proto_ver() const;

        wuk::u64 get_sender() const;
        wuk::u64 get_recipient() const;

        wuk::f64 get_timestamp() const;

        wuk::u32 get_message_id() const;
        wuk::ulong get_message_size() const;

        const std::string &get_message() const;
        const wuk::Buffer get_message(int) const;

    public: // Function
        const std::string serialize();
        WukPacket &parse(const std::string &buffer);
        WukPacket &parse_from(const void *buffer, wuk::ulong length);
    };
}

#endif
