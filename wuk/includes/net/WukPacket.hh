#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukBuffer.hh>
#include <WukTime.hh>
#include <zlib.h>

#include <net/WukPacket.pb.h>

namespace wuk::net {
    class LIBWUK_API WukPacket {
    private:
    public:
        Message _message;
        wuk::WukTime timer;

    private:
        bool validate() const;

    public:
        WukPacket() = default;

    public:
        WukPacket &set_type(MessageType type);
        WukPacket &add_flag(MessageType type);

        WukPacket &set_seq(wU32 seq);
        WukPacket &set_segment(wU32 seg_id, bool is_last = false);
        WukPacket &set_protocol(wU32 version);

        WukPacket &set_ids(wU64 sender, wU64 recipient);

        WukPacket &set_timestamp(double time_val);

        // 如果未设置将在构建时自动使用已有数据得到crc32
        WukPacket &set_message_id(wU32 id);

        WukPacket &set_message(const void *buffer, wSize length);
        WukPacket &set_message(const std::string &buffer);
        WukPacket &set_message(const wuk::Buffer &buffer);
    
    public:
        const std::string serialize();
        Message &parse(const std::string &buffer);
    };
}
#endif
