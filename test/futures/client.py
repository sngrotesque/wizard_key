import socket
import struct
import time
import zlib

'''
这作为一个简易版的早期实现，不应该考虑太复杂的东西。
一切以最小化可使用为基础。
'''

'''
包的构成（小端序）：
1. 4 Bytes: 包长度（仅包含第四项数据流）
2. 4 Bytes: 用户ID
3. 8 Bytes: 时间戳
4. x Bytes: 数据流
5. 4 Bytes: CRC32（检验以上所有值）
'''

class ClientSocket:
    def __init__(self, uid :int):
        self.fd = socket.socket()
        self.uid = struct.pack('@I', uid)

    def __del__(self):
        self.fd.close()

    def send_packet(self, data :bytes):
        data_size = len(data)
        data_time = time.time()

        packet_base = struct.pack(f'@I4sd{data_size}s',
                                data_size,
                                self.uid,
                                data_time,
                                data)
        packet_crc32 = zlib.crc32(packet_base)
        packet_final = packet_base + packet_crc32

        self.fd.sendall(packet_final)

    def recv_packet(self):
        packet_size = self.fd.recv(4)
        packet_uid = self.fd.recv(4)
        packet_time = self.fd.recv(8)

        current_time = time.time()
        size, uid, timestamp = struct.unpack('@IId', packet_size, packet_uid, packet_time)
        if not size:
            raise ValueError('The data length should not be 0.')
        if not uid:
            raise ValueError('UID cannot be 0.')
        if (timestamp > current_time) or (timestamp < (current_time - 300)):
            raise ValueError('The packet has the wrong time.')

        tmp_size = size
        data = b''
        while tmp_size:
            tmp = self.fd.recv(min(2048, tmp_size))
            if not tmp:
                break
            data += tmp
            tmp_size -= len(tmp)

        packet_base = packet_size + packet_uid + packet_time + data

        packet_crc32 = self.fd.recv(4)
        
        if packet_crc32 != struct.pack('@I', zlib.crc32(packet_base)):
            raise ValueError('The packet has been tampered with.')

        return uid, timestamp, data

# 这是测试使用的，不需要验证数据安全性
class TestClientSocket:
    def __init__(self):
        self.fd = socket.socket()

    def __del__(self):
        self.fd.close()

    def connect(self, raddr :str, rport :int):
        self.fd.connect((raddr, rport))

    def send(self, data :bytes):
        self.fd.sendall(struct.pack('@I', len(data)))
        self.fd.sendall(data)

    def recv(self):
        size = struct.unpack('@I', self.fd.recv(4))[0]

        tmp_size = size
        data = b''
        while tmp_size:
            tmp = self.fd.recv(min(2048, tmp_size))
            if not tmp:
                break
            data += tmp
            tmp_size -= len(tmp)

        return data

fd = TestClientSocket()
fd.connect('127.0.0.1', 48888)
while True:
    message = input('Please enter: ')
    if message == 'exit':
        break
    fd.send(message.encode())

