import threading
import socket
import struct
import json
import time

def create_new_thread_socket(fd :socket.socket, addr :tuple):
    packet_len = struct.unpack('>I', fd.recv(4))[0]
    packet = json.loads(fd.recv(packet_len).decode())
    client_addr = f'{addr[0]}:{addr[1]}'
    print(f'time: {round(time.time(), 3)}, addr: {client_addr}, packet: {packet}')
    fd.close()

def create_new_socket(host :str, port :int):
    fd = socket.socket()
    fd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    fd.bind((host, port))
    fd.listen(5)
    
    while True:
        c_fd, addr = fd.accept()
        th = threading.Thread(target = create_new_thread_socket, args = (c_fd, addr))
        th.start()

create_new_socket('0.0.0.0', 8083)


