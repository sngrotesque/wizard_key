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
    plaintext = b"hello world!"
    ciphertext = cipher.encrypt(plaintext) # encrypt the plaintext
    print(ciphertext)
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



aes256ctr()

