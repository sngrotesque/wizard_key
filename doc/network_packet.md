# 网络传输包设计 (Big-endian)
> 以下“第x个包”都是指单个网络传输包，只不过是单个包中的不同部分。

### 包的第一个字节（标志位）

```c
typedef enum {
    IS_NONE    = 0x0  // ........ 一般情况下不会使用到此标志（除非测试）
    IS_FILE    = 0x1  // .......1 是否是文件
    IS_DATA    = 0x2  // ......1. 是否是一般数据
    IS_SEGMENT = 0x4  // .....1.. 是否分段传输
    IS_OVER    = 0x8  // ....1... 是否为最后一个传输块
    IS_XCRYPT  = 0x10 // ...1.... 是否已加密
    IS_MSG     = 0x20 // ..1..... 是否为纯消息
    IS_URGENT  = 0x40 // .1...... 是否是紧急包
    IS_NO_ERR  = 0x80 // 1....... 传输是否无错误（此位必须恒为1，否则弃包）
} PacketFlag;
```

### 包的元数据

```c
typedef struct {
    uint8_t PacketTime[8]; // 这个网络包发送时的时间戳，使用double类型的浮点数表示。
    uint8_t SessionID[8];  // 这个网络包的会话ID。
    uint8_t Sequence[4];   // 这个网络包的序号，用于纠正包顺序。
    uint8_t MateCRC32[4];  // 这个网络包元数据的CRC32校验值。
} PacketMate;
```

### 包的实际数据

```c
typedef struct {
    uint8_t *Content;  // 这个网络包实际包含的数据。
    uint8_t Length[4]; // 这个网络包包含的实际数据的长度。
    uint8_t CRC32[4];  // 这个网络包实际数据长度与实际包含的数据的CRC32校验码，请确保前两者数据的大小加起来不超过2^32-1字节长度。
} PacketData;
```

### 整体包构成如下

```c
typedef struct {
    PacketFlag flag; // 标志位

    PacketMate mate; // 元数据

    PacketData data; // 实际数据
} PacketStructure;
```

如需考虑加密和认证，请使用[OpenSSl](https://www.openssl.org/)库对数据进行加密传输。
请不要考虑在打包数据包时执行压缩，除非有非常好的点子，否则请让使用者直接传入压缩后的数据。

如果后续不考虑自己实现，那请参考[WebSockets](https://en.wikipedia.org/wiki/WebSocket)或者[MQTT](https://en.wikipedia.org/wiki/MQTT)协议。
