from socket import socket
import struct
import zlib

def readChunk(sockfd: socket):
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

def writeChunk(sockfd: socket, content :bytes):
    sockfd.send(struct.pack('>I', len(content)))
    sockfd.sendall(content)
    sockfd.send(struct.pack('>I', zlib.crc32(content)))

def keyExchange(sockfd :socket):
    pass


