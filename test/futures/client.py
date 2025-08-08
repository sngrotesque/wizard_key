import socket
import struct
import zlib

def send_data(fd :socket.socket, data :bytes):
    length = len(data)
    packet = struct.pack('<I', length) + data
    return fd.sendall(packet)

fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
fd.connect(('127.0.0.1', 48888))

while True:
    message = input('请输入内容：')
    if (message == 'exit'):
        break
    
    send_data(fd, message.encode())

fd.close()
