from typing import Callable
import wtools
import random
import socket
import struct
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

def create_trueRandom_number(min :int, max :int):
    array_size = random.choice([1, 2, 4, 8])
    random_bytearray = os.urandom(array_size)

    random_number = -1
    if array_size == 1:
        random_number = struct.unpack('>B', random_bytearray)[0]
    elif array_size == 2:
        random_number = struct.unpack('>H', random_bytearray)[0]
    elif array_size == 4:
        random_number = struct.unpack('>I', random_bytearray)[0]
    elif array_size == 8:
        random_number = struct.unpack('>Q', random_bytearray)[0]

    return random_number % (max - min + 1) + min

def client(path :str):
    fd = socket.socket()
    fd.connect(('127.0.0.1', 80))
    data = wtools.utils.fread(path)

    fd.send(struct.pack('<I', len(data)))
    fd.sendall(data)
    print(fd.recv(5))

    fd.close()

data = os.urandom(int(256.71*1024**2))
wtools.utils.fwrite('test_file', data = data)

client('test_file')
