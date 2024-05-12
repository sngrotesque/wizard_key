from typing import List
import threading
import socket
import struct
import time
import zlib
import os

class W_CRC_ERROR(Exception): ...

class w_kcp:
    def __init__(self, laddr :str, lport :int, listen_number :int = 15, timeout :float = 60):
        self.fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
        self.fd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.fd.settimeout(timeout)
        self.fd.bind((laddr, lport))
        self.fd.listen(listen_number)

        self.client_fd_list :List[socket.socket] = []

    # 内部接收函数
    def __recv_message(self, current_fd :socket.socket, length :int):
        data = b''
        while length:
            temp   =  current_fd.recv(min(length, 2048))
            data   += temp
            length -= len(temp)
        return data

    # 发送函数
    def send(self, current_fd :socket.socket, seq :int, data :bytes):
        print('发送函数开始执行...')
        length = len(data)
        crc = zlib.crc32(data)
        packet = struct.pack(f'!II{length}sI', seq, length, data, crc)

        current_fd.sendall(packet)

    # 接收函数
    def recv(self, current_fd :socket.socket):
        print('接收函数开始执行...')
        seq, length = struct.unpack('!II', current_fd.recv(8))

        data = self.__recv_message(current_fd, length)

        if struct.unpack('!I', current_fd.recv(4))[0] != zlib.crc32(data):
            raise W_CRC_ERROR('The received data is different from the original data.')

        return seq, data

    # 客户端处理函数，后续请将接收和发送分别作为两个线程来执行。
    def client_handler(self, current_fd :socket.socket):
        while True:
            try:
                data = self.recv(current_fd)
                print(f'client message: {data}')

                # 给所有客户端发送心跳包
                for client_fd in self.client_fd_list:
                    self.send(client_fd, 0xffffffff, b'SN\x00\x00')

                # 后续请添加用于给所有客户端同步消息的机制
                # ...

            except W_CRC_ERROR:
                current_fd.close()
                self.client_fd_list.remove(current_fd)

    # 服务端启动
    def server(self):
        while True:
            client_fd, addr = self.fd.accept()
            print(f'client connected: {addr}')
            self.client_fd_list.append(client_fd)

            client_thread = threading.Thread(target = self.client_handler, args = (client_fd, ))
            client_thread.start()
