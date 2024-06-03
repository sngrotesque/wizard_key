# 对于包的设计，请看network_packet.md文件。
from typing import Tuple
import socket
import struct
import time
import zlib

class packet_flag:
    IS_NONE    = 0x0  # ........ 一般情况下不会使用到此标志（除非测试）
    IS_FILE    = 0x1  # .......1 是否是文件
    IS_DATA    = 0x2  # ......1. 是否是一般数据
    IS_SEGMENT = 0x4  # .....1.. 是否分段传输
    IS_OVER    = 0x8  # ....1... 是否为最后一个传输块
    IS_XCRYPT  = 0x10 # ...1.... 是否已加密
    IS_MSG     = 0x20 # ..1..... 是否为纯消息
    IS_NO_ERR  = 0x80 # 1....... 传输是否无错误（此位必须恒为1，否则弃包）

class packet:
    def __init__(self, session_id :int = 0):
        self.session_id = session_id

    def __recv_all(self, fd :socket.socket, length :int):
        data = b''
        while length:
            temp   =  fd.recv(min(length, 4096))
            data   += temp
            length -= len(temp)
        return data

    def join_bytes(self, *args :bytes):
        return bytes().join(args)

    def send(self, fd :socket.socket, data :bytes, sequence :int = 1):
        meta = struct.pack('!QdI', self.session_id, time.time(), sequence)
        meta_crc = struct.pack('!I', zlib.crc32(meta))

        length = len(data)
        content = struct.pack(f'!I{length}s', length, data)
        content_crc = struct.pack('!I', zlib.crc32(content))

        packetAll = self.join_bytes(meta, meta_crc, content, content_crc)
        fd.sendall(packetAll)

    def recv(self, fd :socket.socket) -> Tuple[int, float, int, bytes]:
        tmp_meta = fd.recv(8 + 8 + 4)
        tmp_meta_crc = fd.recv(4)
        
        if zlib.crc32(tmp_meta) != struct.unpack('!I', tmp_meta_crc)[0]:
            raise ValueError('The CRC32 validation of metadata failed.')
        
        session_id, timer, sequence = struct.unpack('!QdI', tmp_meta)

        length = struct.unpack('!I', fd.recv(4))[0]
        content = self.__recv_all(fd, length)
        content_crc = fd.recv(4)

        if zlib.crc32(self.join_bytes(content, length)) != \
                                struct.unpack('!I', content_crc)[0]:
            raise ValueError('The CRC32 verification of data content failed.')

        return session_id, timer, sequence, content
