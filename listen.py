from socket import *
import threading
import select
import re
import os

DEFAULT_LISTEN_ADDR = '0.0.0.0'
DEFAULT_LISTEN_PORT = 4772
DEFAULT_LISTEN_NUM = 30

def client_handler(sockfd :socket):
    try:
        connect_info = sockfd.recv(4096)
        target_host, target_port, protocol = \
            re.findall(b'CONNECT ([\w\d\.\-]+):(\d+) (HTTP/[\d.]+)', connect_info, re.S | re.I)[0]
        sockfd.send(f'{protocol} 200 Connection Established\r\n\r\n'.encode())
        target_sockfd = socket(AF_INET, SOCK_STREAM, 0)
        target_sockfd.connect((target_host, int(target_port)))
        while True:
            rlist, wlist, xlist = select.select([sockfd, target_sockfd], [], [])
            if sockfd in rlist:
                data = sockfd.recv(4096)
                if not data:
                    break
                target_sockfd.sendall(data)
            if target_sockfd in rlist:
                data = target_sockfd.recv(4096)
                if not data:
                    break
                sockfd.sendall(data)
    except (IndexError, ConnectionResetError) as e:
        print(f'抛出错误[Error]: {e}')
    finally:
        sockfd.close()

sockfd = socket(AF_INET, SOCK_STREAM, 0)
sockfd.bind((DEFAULT_LISTEN_ADDR, DEFAULT_LISTEN_PORT))
print(f'Bind Native: {DEFAULT_LISTEN_ADDR}:{DEFAULT_LISTEN_PORT}')
sockfd.listen(DEFAULT_LISTEN_NUM)
print(f'Listen number: {DEFAULT_LISTEN_NUM}')

while True:
    client_sockfd, addr = sockfd.accept()
    print(f'Client connected: {addr[0]}:{addr[1]}')
    th = threading.Thread(target=client_handler, args=(client_sockfd,))
    th.start()
