#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <common/includes/WukPacket.pb.hh>

#include <WukBuffer.hh>
#include <WukTime.hh>

namespace wuk::im {
    class LIBWUK_API Packet {
    private:
        Message m_message;
        wuk::Time m_time;

    private:
        bool validate() const noexcept;

    public:
        Packet() = default;
        ~Packet() = default;

    public:
        Packet(const Packet &other) = default;
        Packet(Packet &&other) = default;
        Packet &operator=(const Packet &other) = default;
        Packet &operator=(Packet &&other) = default;

    public: // Setter
        Packet &set_type(MessageType type) noexcept;
        Packet &add_flag(MessageType type) noexcept;

        Packet &set_sequence(wuk::u32 seq) noexcept;
        Packet &set_segment_id(wuk::u32 seg_id, bool is_last = false) noexcept;
        Packet &set_proto_ver(wuk::u32 version);

        Packet &set_ids(wuk::u64 sender, wuk::u64 recipient) noexcept;
        Packet &set_sender(wuk::u64 id) noexcept;
        Packet &set_recipient(wuk::u64 id) noexcept;

        Packet &set_timestamp(wuk::f64 time_val) noexcept;

        Packet &set_message_id(wuk::u32 id) noexcept;

        Packet &set_message(const void *buffer, wuk::ulong length) noexcept;
        Packet &set_message(const std::string &buffer) noexcept;
        Packet &set_message(const wuk::Buffer &buffer) noexcept;

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
        Packet &parse(const std::string &buffer);
        Packet &parse_from(const void *buffer, wuk::ulong length);
    };
}

#endif
