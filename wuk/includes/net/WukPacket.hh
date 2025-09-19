#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <net/WukError.hh>
#include <net/WukPacket.pb.h>
#include <WukBuffer.hh>
#include <WukTime.hh>

namespace wuk::net {
    class LIBWUK_API WukPacket {
    private:
        Message m_message;
        wuk::Time m_time;

    private:
        bool validate() const noexcept;

    public:
        WukPacket() = default;

    public: // Setter
        WukPacket &set_type(MessageType type) noexcept;
        WukPacket &add_flag(MessageType type) noexcept;

        WukPacket &set_sequence(wuk::u32 seq) noexcept;
        WukPacket &set_segment_id(wuk::u32 seg_id, bool is_last = false) noexcept;
        WukPacket &set_proto_ver(wuk::u32 version);

        WukPacket &set_ids(wuk::u64 sender, wuk::u64 recipient) noexcept;
        WukPacket &set_sender(wuk::u64 id) noexcept;
        WukPacket &set_recipient(wuk::u64 id) noexcept;

        WukPacket &set_timestamp(wuk::f64 time_val) noexcept;

        WukPacket &set_message_id(wuk::u32 id) noexcept;

        WukPacket &set_message(const void *buffer, wuk::ulong length) noexcept;
        WukPacket &set_message(const std::string &buffer) noexcept;
        WukPacket &set_message(const wuk::Buffer &buffer) noexcept;

    public: // Getter
        MessageType get_type() const noexcept;
        bool has_flag(MessageType flag) const noexcept;

        wuk::u32 get_sequence() const noexcept;
        wuk::u32 get_segment_id() const noexcept;
        wuk::u32 get_proto_ver() const noexcept;

        wuk::u64 get_sender() const noexcept;
        wuk::u64 get_recipient() const noexcept;

        wuk::f64 get_timestamp() const noexcept;

        wuk::u32 get_message_id() const noexcept;
        wuk::ulong get_message_size() const noexcept;

        const std::string &get_message() const noexcept;
        const wuk::Buffer get_message(int) const noexcept;

    public: // Function
        const std::string serialize();
        WukPacket &parse(const std::string &buffer);
        WukPacket &parse_from(const void *buffer, wuk::ulong length);
    };
}

#endif
