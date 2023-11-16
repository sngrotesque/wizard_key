from socket import *
import struct
import zlib
import ssl

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

class wmkcNet:
    def __init__(self, host: str, port :int, sockfd_family :int = AF_INET, sockfd_type: int = SOCK_STREAM):
        self.sockfd = socket(sockfd_family, sockfd_type, 0)
        self.host = host
        self.port = port

        self.client_info = None

    def ssl_context(self):
        self.sockfd = ssl.SSLContext().wrap_socket(self.sockfd, server_hostname = self.host)

    def timeout(self, _time_out :float):
        self.sockfd.settimeout(_time_out)

    def bind(self):
        self.sockfd.bind((self.host, self.port))

    def listen(self, __backlog: int):
        self.sockfd.listen(__backlog)

    def accept(self) -> socket:
        tmp_sockfd, self.client_info = self.sockfd.accept()
        return tmp_sockfd

    def connect(self):
        self.sockfd.connect((self.host, self.port))

    def close(self):
        self.sockfd.close()

