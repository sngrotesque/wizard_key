import threading
import random
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
        self.uid = uid

    def __del__(self):
        self.fd.close()

    def connect(self, raddr :str, rport :int):
        self.fd.connect((raddr, rport))

    def send(self, data :bytes):
        data_size = len(data)
        data_time = time.time()

        packet_base = struct.pack(
            f'@IId{data_size}s',
            data_size,
            self.uid,
            data_time,
            data
        )
        packet_crc32 = struct.pack('@I', zlib.crc32(packet_base))
        packet_final = packet_base + packet_crc32

        self.fd.sendall(packet_final)

    def recv(self):
        packet_size = self.fd.recv(4)
        packet_uid = self.fd.recv(4)
        packet_time = self.fd.recv(8)

        current_time = time.time()
        size, uid, timestamp = struct.unpack('@IId', packet_size, packet_uid, packet_time)
        if not size:
            raise ValueError('数据长度不能为0。')
        if not uid:
            raise ValueError('UID不能为0。')
        if (timestamp > current_time) or (timestamp < (current_time - 300)):
            raise ValueError('错误的网络包时间戳。')

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
            raise ValueError('数据包已被篡改。')

        return uid, timestamp, data

def main():
    server = ('47.79.146.143', 48888)
    charset = (
        '0123456789'
        'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        'abcdefghijklmnopqrstuvwxyz'
    )
    threads = 4
    count = (10, 50)

    def test():
        uid = random.randint(0, 0xffffffff)
        fd = ClientSocket(uid)
        fd.connect(*server)
        for _ in range(random.randint(*count)):
            message = ''.join(
                random.sample(charset, random.randint(4, 50))
            )
            fd.send(message.encode())
            time.sleep(random.uniform(0.001, 0.5))

    ths = [threading.Thread(target = test) for _ in range(threads)]

    for th in ths:
        th.start()
    for th in ths:
        th.join()

if __name__ == '__main__':
    main()
