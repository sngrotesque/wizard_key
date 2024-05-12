''' Markdown
# 网络传输包设计 (Big-endian)

1. **proto   ver** [^1]: 4 Bytes.
3. **session id**  [^2]: 4 Bytes.
2. **packet  seq** [^3]: 4 Bytes.
4. **packet  size**[^4]: 4 Bytes.
5. **packet  time**[^5]: 8 Bytes.
6. **packet  data**[^6]: [packet length] Bytes.
7. **packet  crc** [^7]: 4 Bytes.

如需考虑加密和认证，请使用OpenSSl库对数据进行加密传输。
请不要考虑在打包数据包时执行压缩，除非有非常好的点子，否则请让使用者直接传入压缩后的数据。

[^1]: 这个网络包所使用的协议版本。
[^2]: 这个网络包的会话ID。
[^3]: 这个网络包的序号，用于纠正包顺序。
[^4]: 这个网络包包含的数据的长度。
[^5]: 这个网络包发送时的时间戳，使用double类型的浮点数表示，保留4位小数。
[^6]: 这个网络包实际包含的数据。
[^7]: 这个网络包CRC32校验码，包含前面的所有数据，所以请确保前面所有数据的大小不超过2<sup>32</sup>-1字节长度。
'''

'''
目前对于这些实现，请考虑WebSockets或者MQTT协议。
'''
from .crypto import zlib_crc32

import socket
import struct
import time

class packet:
    def __init__(self, proto_ver :int = 0x00000000, session_id :int = 0x00000000):
        self.proto_ver = proto_ver
        self.session_id = session_id

    def __recv(self, fd :socket.socket, length :int):
        data = b''
        while length:
            temp   =  fd.recv(min(length, 4096))
            data   += temp
            length -= len(temp)
        return data

    def send_pack(self, fd :socket.socket, data :bytes, seq :int = 1):
        data_len = len(data)

        packet = struct.pack(f'!IIIId{data_len}s',
            self.proto_ver,   # protocol version
            self.session_id,  # session id
            seq,              # sequence
            data_len,         # the length of compressed data
            time.time(),      # current timestamp
            data)             # compressed data
        packet += struct.pack('!I', zlib_crc32(packet))

        fd.sendall(packet)

    def recv_pack(self, fd :socket.socket):
        proto_ver   = struct.unpack('!I', fd.recv(4))[0]
        session_id  = struct.unpack('!I', fd.recv(4))[0]
        seq         = struct.unpack('!I', fd.recv(4))[0]
        size        = struct.unpack('!I', fd.recv(4))[0]
        timer       = struct.unpack('!d', fd.recv(8))[0]
        data        = self.__recv(fd, size)
        crc         = struct.unpack('!I', fd.recv(4))[0]

        tmp = struct.pack(f'!IIIId{size}s',
            proto_ver,        # protocol version
            session_id,       # session id
            seq,              # sequence
            size,             # the length of data
            timer,            # current timestamp
            data)             # data

        if zlib_crc32(tmp) != crc:
            raise RuntimeError(f'The obtained package has a different CRC than the original package.')

        return proto_ver, session_id, seq, size, timer, data, crc
