#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukEndianness.hh>
#include <WukBuffer.hh>
#include <WukTime.hh>
#include <zlib.h>

namespace wuk::net {
    enum class MessageTypes : wU32 {
        NONE    = 0x00000000, // ........ ........ ........ ........ 无标志位（默认情况）
        FILE    = 0x00000001, // ........ ........ ........ .......1 文件
        DATA    = 0x00000002, // ........ ........ ........ ......1. 一般数据
        SEGMENT = 0x00000004, // ........ ........ ........ .....1.. 已分段
        OVER    = 0x00000008, // ........ ........ ........ ....1... 已是分段包的最后一个
        XCRYPT  = 0x00000010, // ........ ........ ........ ...1.... 已加密
        MESSAGE = 0x00000020, // ........ ........ ........ ..1..... 纯消息
        URGENT  = 0x00000040, // ........ ........ ........ .1...... 紧急包

        IMAGE   = 0x00000080, // ........ ........ ........ 1....... 图像且FILE，否则视为表情包
        VIDEO   = 0x00000100, // ........ ........ .......1 ........ 视频
        AUDIO   = 0x00000200, // ........ ........ ......1. ........ 音频
        ANYM    = 0x00000400, // ........ ........ .....1.. ........ 匿名包

        NOERR   = 0x80000000, // 1....... ........ ........ ........ 校验位，必须为1，否则弃包
    };
    using MsgT = MessageTypes;

    class WukPacket {
    private:
        wU32 proto_ver    = 0; // 协议版本号
        wU32 seg_id       = 0; // （分段）包id，默认为0
        wU32 msg_seq      = 0; // 序列号
        MsgT msg_type     = MsgT::NONE; // 消息类型

        wU64 msg_id       = 0; // 包id（不同于msg_seq，请勿混为一谈）
        wU64 msg_size     = 0; // 包所含数据的长度（不包含这些元数据）

        wU64 sender_id    = 0; // 发送方id
        wU64 recipient_id = 0; // 接收方id

        double time_stamp = 0; // UNIX时间戳

        wuk::Buffer   message; // 消息体

    public:
        WukPacket() = default;

        WukPacket(MsgT msg_type,
                  wU32 proto_ver = 0x01, wU32 seg_id = 0, wU32 msg_seq = 0, wU64 msg_id = 0,
                  wU64 sender_id = 0, wU64 recipient_id = 0);

    public:
        void set_proto_ver(wU32 value);
        void set_seg_id(wU32 value);
        void set_msg_seq(wU32 value);
        void set_msg_type(MsgT value);

        void set_msg_id(wU64 value);

        void set_sender_id(wU64 value);
        void set_recipient_id(wU64 value);

        void set_message(wuk::Buffer message);

        wuk::Buffer get_packet() noexcept;
    };
}
#endif
