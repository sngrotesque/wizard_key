from Crypto.Util.Padding import pad
from Crypto.Cipher import AES
from hashlib import sha256
from socket import *
import struct
import rsa
import os

DEFAULT_TIMEOUT = 7.0
DEFAULT_RSA_KEY_BIT = 512

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
    rsa_pk, rsa_sk = rsa.newkeys(DEFAULT_RSA_KEY_BIT)
    sockfd.sendall(buildChunk(rsa_pk.save_pkcs1()).chunk)
    aes_key = rsa.decrypt(parseChunk(sockfd).data, rsa_sk)
    aes_iv = rsa.decrypt(parseChunk(sockfd).data, rsa_sk)
    return aes_key, aes_iv

def aes256_encrypt(data :bytes, key :bytes, iv :bytes):
    aes = AES.new(key = key, IV = iv, mode = AES.MODE_CBC)
    return aes.encrypt(data)

def aes256_decrypt(data :bytes, key :bytes, iv :bytes):
    aes = AES.new(key = key, IV = iv, mode = AES.MODE_CBC)
    return aes.decrypt(data)

def listen(target_host :str, target_port :int):
    sockfd = socket(AF_INET, SOCK_STREAM, 0)
    sockfd.settimeout(DEFAULT_TIMEOUT)
    sockfd.bind((target_host, target_port))
    sockfd.listen(4)
    sockfd, addr = sockfd.accept()
    print(f'client addr: {addr[0]}:{addr[1]}')

    aes_key, aes_iv = keyExchange(sockfd)
    print(f'aes_key: {aes_key.hex()}')
    print(f'aes_iv:  {aes_iv.hex()}')

    result = aes256_decrypt(parseChunk(sockfd).data, aes_key, aes_iv)
    print(result, len(result))

    sockfd.close()

host = '0.0.0.0'
port = 49281
listen(host, port)
