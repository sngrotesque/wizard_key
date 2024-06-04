from typing import Callable
import wtools
import socket
import time
import sys
import os

import wtools.utils

def fcipher_xcrypt():
    if len(sys.argv) < 5:
        exit(f'usage: python {sys.argv[0]} [Password] [input_path] [output_path] [e]ncrypt/[d]ncrypt')
    
    password = sys.argv[1]
    input_path = sys.argv[2]
    output_path = sys.argv[3]
    xcrypt_mode = sys.argv[4].lower()
    
    if input_path == output_path:
        print(f'input_path == output_path, are you sure?')
        choice = input('[y/N]').lower()
        if (choice == '') or (choice == 'n') or (choice == 'no'):
            exit('exit.')
        elif (choice == 'y') or (choice == 'yes'):
            print('continue.')
        else:
            exit('error input.')
    
    ctx = wtools.fcipher(password.encode())
    
    if xcrypt_mode == 'e':
        ctx.encrypt(input_path, output_path)
    elif xcrypt_mode == 'd':
        ctx.decrypt(input_path, output_path)
    else:
        exit(f'unknown xcrypt_mode.')

def listen(addr :str, port :int = 9971):
    fd = socket.socket()
    print(f'bind to: {addr}:{port}')
    fd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    fd.bind((addr, port))
    fd.listen(5)
    print('Waiting for thr client to connect...')
    cfd, caddr = fd.accept()
    
    print(f'Client connected: {caddr[0]}:{caddr[1]}')
    pkt = wtools.packet()
    res = pkt.recv(cfd)
    print(f'session id: {res[0]}, time: {res[1]:.4f}, seq: {res[2]}, data: {res[3]}.')
    
    pkt.send(cfd, b'done.')
    
    cfd.close()
    fd.close()

def client(addr :str, port :int = 9971, path :str = None):
    fd = socket.socket()
    fd.connect((addr, port))
    
    pkt = wtools.packet()

    if path:
        data = wtools.utils.fread(path)
        pkt.send(fd, data)
    else:
        pkt.send(fd, b'wtools.packet test!')

    print(pkt.recv(fd))
    
    fd.close()

def pixiv_test():
    pix = wtools.pixiv(38279179, cookies = 'E:/pixiv_cookie.txt', save_path = 'F:/Pitchers/Pixiv/NewTest', proxies = 'http://localhost:8081')
    res = pix.getTotalArtistList()
    print(res)

if __name__ == '__main__':
    client('192.168.0.103', path='w:/data.bin')
