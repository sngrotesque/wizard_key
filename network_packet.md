# 网络传输包设计 (Big-endian)
> 以下“第x个包”都是指单个网络传输包，只不过是单个包中的不同部分。

### 第一个字节
- **Flag**       [^f]: 1 Byte.

```py
class flag:
    IS_NONE    = 0x0  # ........ 一般情况下不会使用到此标志（除非测试）
    IS_FILE    = 0x1  # .......1 是否是文件
    IS_DATA    = 0x2  # ......1. 是否是一般数据
    IS_SEGMENT = 0x4  # .....1.. 是否分段传输
    IS_OVER    = 0x8  # ....1... 是否为最后一个传输块
    IS_XCRYPT  = 0x10 # ...1.... 是否已加密
    IS_MSG     = 0x20 # ..1..... 是否为纯消息
    IS_NO_ERR  = 0x80 # 1....... 传输是否无错误（此位必须恒为1，否则弃包）
```

### 第1个包
- **Sequence    (Meta info)**[^mi1]: 4 Bytes.
- **Session ID  (Meta info)**[^mi2]: 8 Bytes.
- **Packet Time (Meta info)**[^mi3]: 8 Bytes.
- **(Meta info) CRC32**      [^mic]: 4 Bytes.

### 第2个包与其自身的CRC32校验
- **Length (Content)**[^c1]: 4 Bytes.
- **Data   (Content)**[^c2]: $(Length)[^c1] Bytes.
- **(Content)    CRC**[^cc]: 4 Bytes.

如需考虑加密和认证，请使用[OpenSSl](https://www.openssl.org/)库对数据进行加密传输。
请不要考虑在打包数据包时执行压缩，除非有非常好的点子，否则请让使用者直接传入压缩后的数据。

如果后续不考虑自己实现，那请参考[WebSockets](https://en.wikipedia.org/wiki/WebSocket)或者[MQTT](https://en.wikipedia.org/wiki/MQTT)协议。

[^f]:   这个数据为包的标志位，用于告知对向方此包的类型，应单独发送和接收（不与包结合）。
[^mi1]: 这个网络包的序号，用于纠正包顺序。
[^mi2]: 这个网络包的会话ID。
[^mi3]: 这个网络包发送时的时间戳，使用double类型的浮点数表示，保留4位小数。
[^mic]: 这个网络包的元数据的CRC32校验值。
[^c1]: 这个网络包包含的实际数据的长度。
[^c2]: 这个网络包实际包含的数据。
[^cc]: 这个网络包实际数据长度与实际包含的数据的CRC32校验码，请确保前两者数据的大小加起来不超过2<sup>32</sup>-1字节长度。