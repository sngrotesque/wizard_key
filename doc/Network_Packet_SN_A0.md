# 网络传输包设计

0. 请使用Protobuf构建。
1. [Protobuf官网](https://protobuf.dev/)。
2. [Protobuf下载](https://github.com/protocolbuffers/protobuf/releases)。

### 构成

```proto
using u8  = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i32 = int32_t; // 为兼容protobuf
using i64 = int64_t;
using f32 = float;
using f64 = double;

namespace wuk::net {
    enum class MessageTypes : i32 {
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
    };
    using MsgT = MessageTypes;

    struct Message {
        MsgT msg_type;     // 消息类型
        u32  msg_seq;      // 序列号
        u32  seg_id;       // （分段）包id
        u32  proto_ver;    // 协议版本号

        u64  sender_id;    // 发送方id
        u64  recipient_id; // 接收方id

        f64  time_stamp;   // UNIX时间戳

        u32  msg_size;     // 包所含数据的长度（不包含这些元数据）
        u32  msg_id;       // 包id（不同于msg_seq，请勿混为一谈）

        u8  *message;      // 消息体
    };
}
```

### 详解

请查看`wuk\WukPacket.proto`。
