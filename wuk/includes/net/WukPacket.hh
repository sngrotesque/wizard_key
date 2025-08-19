#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
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

        WukPacket &set_sequence(wU32 seq);
        WukPacket &set_segment_id(wU32 seg_id, bool is_last = false);
        WukPacket &set_proto_ver(wU32 version);

        WukPacket &set_ids(wU64 sender, wU64 recipient);
        WukPacket &set_sender(wU64 id);
        WukPacket &set_recipient(wU64 id);

        WukPacket &set_timestamp(double time_val);

        WukPacket &set_message_id(wU32 id);

        WukPacket &set_message(const void *buffer, wSize length);
        WukPacket &set_message(const std::string &buffer);
        WukPacket &set_message(const wuk::Buffer &buffer);

    public: // Getter
        MessageType get_type() const;
        bool has_flag(MessageType flag) const;

        wU32 get_sequence() const;
        wU32 get_segment_id() const;
        wU32 get_proto_ver() const;

        wU64 get_sender() const;
        wU64 get_recipient() const;

        double get_timestamp() const;

        wU32 get_message_id() const;
        wSize get_message_size() const;

        const std::string &get_message() const;
        const wuk::Buffer get_message(int) const;

    public: // Function
        const std::string serialize();
        WukPacket &parse(const std::string &buffer);
        WukPacket &parse_from(const void *buffer, wSize length);
    };
}

#endif
