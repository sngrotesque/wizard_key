from Crypto.Util.Padding import pad
from typing import Union, List
from Crypto.Cipher import AES
from hashlib import sha256
from socket import *
import struct
import rsa
import os

DEFAULT_TIMEOUT = 7.0

class chunk_obj:
    def __init__(self):
        self.chunk = b''
        self.size = None
        self.data = None
        self.hash = None

def fread(fn :str, mode :str = 'rb', encoding = None):
    with open(fn, mode, encoding = encoding) as f:
        return f.read()

def fwrite(fn :str, data :object, mode :str = 'wb', encoding = None):
    with open(fn, mode, encoding = encoding) as f:
        f.write(data)

def buildChunk(data :bytes) -> chunk_obj:
    chunk       = chunk_obj()
    chunk.size  = struct.pack('>I', len(data))
    chunk.data  = data
    chunk.hash  = sha256(data).digest()
    chunk.chunk = chunk.size + chunk.data + chunk.hash
    return chunk

def parseChunk(sockfd :socket) -> chunk_obj:
    chunk       = chunk_obj()
    chunk.size  = struct.unpack('>I', sockfd.recv(4))[0]
    chunk.data  = sockfd.recv(chunk.size)
    chunk.hash  = sockfd.recv(32)
    chunk.chunk = struct.pack('>I', chunk.size) + chunk.data + chunk.hash
    return chunk

def keyExchange(sockfd :socket):
    rsa_pk = rsa.PublicKey.load_pkcs1(parseChunk(sockfd).data)
    aes_key = os.urandom(32)
    aes_iv = os.urandom(16)
    aes_key_c = rsa.encrypt(aes_key, rsa_pk)
    aes_iv_c = rsa.encrypt(aes_iv, rsa_pk)
    sockfd.sendall(buildChunk(aes_key_c).chunk)
    sockfd.sendall(buildChunk(aes_iv_c).chunk)
    return aes_key, aes_iv

def aes256_encrypt(data :bytes, key :bytes, iv :bytes):
    aes = AES.new(key = key, IV = iv, mode = AES.MODE_CBC)
    return aes.encrypt(data)

def aes256_decrypt(data :bytes, key :bytes, iv :bytes):
    aes = AES.new(key = key, IV = iv, mode = AES.MODE_CBC)
    return aes.decrypt(data)

def client(target_host :str, target_port :int, content :bytes):
    sockfd = socket(AF_INET, SOCK_STREAM, 0)
    sockfd.settimeout(DEFAULT_TIMEOUT)
    sockfd.connect((target_host, target_port))

    aes_key, aes_iv = keyExchange(sockfd)
    print(f'aes_key: {aes_key.hex()}')
    print(f'aes_iv:  {aes_iv.hex()}')

    result = aes256_encrypt(pad(content, AES.block_size), aes_key, aes_iv)
    sockfd.sendall(buildChunk(result).chunk)

    sockfd.close()

host = '127.0.0.1'
port = 49281
data = ('0'*65536).encode()

client(host, port, data)









