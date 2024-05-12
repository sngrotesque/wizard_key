from socket import *
import struct
import zlib

def wmkcNet_readChunk(sockfd :socket):
    content_size = struct.unpack('>I', sockfd.recv(4))[0]
    content = b''
    while content_size:
        temp = sockfd.recv(min(content_size, 4096))
        content += temp
        content_size -= len(temp)
    crc32 = struct.unpack('>I', sockfd.recv(4))[0]
    if crc32 == zlib.crc32(content):
        return content
    return None

def wmkcNet_writeChunk(sockfd :socket, content :bytes):
    sockfd.send(struct.pack('>I', len(content)))
    sockfd.sendall(content)
    sockfd.send(struct.pack('>I', zlib.crc32(content)))
