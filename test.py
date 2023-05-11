from Crypto.Random import get_random_bytes
from urllib.parse import unquote, quote
from Crypto.Protocol.KDF import PBKDF2
from typing import Union, List
from Crypto.Cipher import AES
from socket import *
import requests
import binascii
import hashlib
import random
import base64
import struct
import lzma
import time
import rsa
import cv2
import ssl
import os
import re

def fread(fn :str, mode :str = 'rb', encoding = None):
    with open(fn, mode, encoding = encoding) as f:
        return f.read()

def fwrite(fn :str, data :object, mode :str = 'wb', encoding = None):
    with open(fn, mode, encoding = encoding) as f:
        f.write(data)

def sha256(data :object):
    _ = hashlib.sha256(data).hexdigest()

def dump(arr :Union[List[int], bytes, bytearray]):
    size = len(arr)
    text = ''
    for x in range(size):
        text += f'0x{arr[x]:02x}'
        if (x + 1) != size:
            if not (x + 1) % 16:
                text += ',\n'
            else:
                text += ', '
    return text

def aes256ctr():
    # Encryption
    password = b"password"
    salt = b'#o\x99Jb\x17\xbal\xa0\xe9\x83B\x15\xbb\x16\xc3'
    # salt = get_random_bytes(16) # random salt for PBKDF2
    key = PBKDF2(password, salt, dkLen=32) # derive a 32-byte key from password and salt
    nonce = b' \xac\xee$c\xb3\x9c\xc9'
    # nonce = get_random_bytes(8) # random nonce for CTR mode
    cipher = AES.new(key, AES.MODE_CTR, nonce=nonce) # create a new AES cipher object
    plaintext = b"Hello world!"
    ciphertext = cipher.encrypt(plaintext) # encrypt the plaintext
    # Decryption
    cipher = AES.new(key, AES.MODE_CTR, nonce=nonce) # create a new AES cipher object with the same key and nonce
    decrypted = cipher.decrypt(ciphertext) # decrypt the ciphertext
    assert decrypted == plaintext # check if decryption is correct

class sboxRsbox:
    def __init__(self):
        self.sbox  = [x for x in range(256)]
        self.rsbox = [x for x in range(256)]

    @property
    def generateSbox(self):
        for x in range(16):
            random.shuffle(self.sbox)

    @property
    def generateRsbox(self):
        for x in range(256):
            self.rsbox[self.sbox[x]] = x

class twitter:
    def __init__(self):
        pass

    def _gen_rand_file_name(self, fn :str):
        pk, sk = rsa.newkeys(2048)
        random.seed(sk.d)
        _fn, _ftype = re.findall(r'(.*)\.([\w\.]+)$', fn, re.S)[0]
        _ascii = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_-.'
        _data = '_' + ''.join([random.choice(_ascii) for x in range(12)])
        return '.'.join([_fn+_data, _ftype])

    def encrypt(self, message :bytes):
        message = bytearray(message)
        for i in range(len(message)):
            message[i] = (message[i] + i) ^ 0xff
        message = [bytes(message[x:x+8]) for x in range(0, len(message), 8)]
        for x in range(len(message)):
            if len(message[x]) % 8:
                message[x] += (b'\x00' * (8 - (len(message[x]) % 8)))
            message[x] = struct.unpack('>Q', message[x])[0]
        return message

class music:
    def __init__(self, section :List[int]):
        self.section_1 = [0 for _ in range(section[0])]
        self.section_2 = [0 for _ in range(section[1])]
        self.section_3 = [0 for _ in range(section[2])]
        self.section_4 = [0 for _ in range(section[3])]
        self.section = section
        
        self.melody = [
            'C', 'D', 'E', 'F', 'G', 'A', 'B',
            # 'C#', 'D#', 'F#', 'G#', 'A#'
        ]

    def generate(self):
        for x in range(self.section[0]):
            self.section_1[x] = random.choice(self.melody)
        for x in range(self.section[1]):
            self.section_2[x] = random.choice(self.melody)
        for x in range(self.section[2]):
            self.section_3[x] = random.choice(self.melody)
        for x in range(self.section[3]):
            self.section_4[x] = random.choice(self.melody)
        result = [
            self.section_1, self.section_2, self.section_3, self.section_4
        ]
        return result

def aes256_encrypt(data :bytes, key :bytes, iv :bytes) -> bytes:
    aes = AES.new(key = key, IV = iv, mode = AES.MODE_CBC)
    return aes.encrypt(data)

def aes256_decrypt(data :bytes, key :bytes, iv :bytes) -> bytes:
    aes = AES.new(key = key, IV = iv, mode = AES.MODE_CBC)
    return aes.decrypt(data)

SN_NET_BLOCK = 4096

def snNet_listen():
    host = '0.0.0.0'
    port = 49281

    s = socket(AF_INET, SOCK_STREAM, 0)
    s.settimeout(300)
    s.bind((host, port))
    s.listen(5)
    print('等待连接...')
    s,addr = s.accept()

    print(f'客户端已连接：{addr}')

    fileSize = struct.unpack("<Q", s.recv(8))[0]
    fileHash = s.recv(32).hex()
    print(f'文件长度：{fileSize}')
    print(f'文件哈希：{fileHash}')

    StartTime = time.time()
    data = buf = b''
    while fileSize:
        buf = s.recv(SN_NET_BLOCK)
        data += buf
        fileSize -= len(buf)
    StopTime = time.time()

    speed = (len(data) / (StopTime - StartTime))
    print('传输速度为：', end='')
    if speed > 1048576:
        print(f'{speed / 1024**2:.2f} MB/秒。')
    elif speed > 1024 and speed < 1048576:
        print(f'{speed / 1024:.2f} KB/秒。')

    dataSize = len(data)
    dataHash = hashlib.sha256(data).hexdigest()
    print(f'接收完成。')
    print(f'数据长度：{dataSize}')
    print(f'数据哈希：{dataHash}')

    s.send(b'done')
    s.close()


snNet_listen()


