# 网络传输包设计
> 综合了之前的包的设计以及一些意见和实战之后得到的经验，重新设计网络包

### 包的构成

```cpp
class Packet {
private:
    uint32_t p_protocol_version; // 协议版本号
    uint32_t p_message_type;     // 消息类型
    uint32_t p_sequence;         // 顺序号
    uint32_t p_segment_id;       // 分段包的id

    double   p_time_stamp;       // UNIX时间戳
    uint64_t p_message_id;       // 唯一标识符
    uint64_t p_message_size;     // 消息实际内容的长度

    uint64_t p_sender_id;        // 发送方ID
    uint64_t p_recipient_id;     // 接收方ID

public:
    // 消息头
    uint8_t protocol_version[4];
    uint8_t message_type[4];
    uint8_t sequence[4];
    uint8_t segment_id[4];

    uint8_t time_stamp[8];
    uint8_t message_id[8];
    uint8_t message_size[8];

    uint8_t sender_id[8];
    uint8_t recipient_id[8];

    // 消息体
    uint8_t *message;          // 消息的实际内容

    // 附加信息
    uint8_t sha256_digest[32]; // SHA-256哈希对整个消息包的摘要，用于完整性校验
};

```

### 内部标准

> Standard number: **SN-A0**

```c
typedef enum {
    IS_NONE    = 0x00000000; // ................................ 无标志位
    IS_FILE    = 0x00000001; // ...............................1 文件
    IS_DATA    = 0x00000002; // ..............................1. 一般数据
    IS_SEGMENT = 0x00000004; // .............................1.. 已分段
    IS_OVER    = 0x00000008; // ............................1... 已是分段包的最后一个
    IS_XCRYPT  = 0x00000010; // ...........................1.... 已加密
    IS_MESSAGE = 0x00000020; // ..........................1..... 纯消息
    IS_URGENT  = 0x00000040; // .........................1...... 紧急包

    IS_IMAGE   = 0x00000080; // ........................1....... 图像且IS_FILE，否则视为表情包
    IS_VIDEO   = 0x00000100; // .......................1........ 视频
    IS_AUDIO   = 0x00000200; // ......................1......... 音频
    IS_ANYM    = 0x00000400; // .....................1.......... 匿名包

    IS_NOERR   = 0x80000000; // 1............................... 校验位，必须为1，否则弃包
} MessageTypes;
```

**SHA-256**的数据输入顺序为：
```txt
1)  protocol_version
2)  message_type
3)  sequence
4)  segment_id
5)  time_stamp
6)  message_id
7)  message_size
8)  sender_id
9)  recipient_id
10) message
```

详解：

1.  `sender_id`即使在`message_type`为`IS_ANYM`的情况下，也必须是准确的用户ID，不能匿名。
2.  `recipient_id`的第一个二进制位为1时，代表群组聊天，为0时代表一对一的私聊。
3.  `time_stamp`必须是UNIX时间戳，且格式必须是浮点数，使用时至少保留4位小数，如`1728856869.7596433`。




