from .utils import (
    BlockSize, padding
)
from socket import (
    socket, SOL_SOCKET, SO_REUSEADDR,
    AF_INET, SOCK_STREAM, SOCK_DGRAM
)

def _socketRecv(_socket :socket, size :int = 4096):
    if _socket.type == 1: recv = _socket.recv
    elif _socket.type == 2: recv = _socket.recvfrom
    socketResults = recv(size)
    _socket.setblocking(0)
    try:
        while True:
            data = recv(size)
            if not data:
                break
            socketResults += data
    except BlockingIOError:
        pass
    _socket.setblocking(1)
    return socketResults

class snSocket:
    def __init__(self,
        target_host :str = None,
        target_port :int = None,
        listen_host :str = None,
        listen_port :int = None,
        ):
        self.target_host = target_host
        self.target_port = target_port
        self.listen_host = listen_host
        self.listen_port = listen_port
        
        self.socket_ipv4 :socket
        self.socket_addr :str

    def init(self, UDP :bool = False):
        if UDP: self.socket_ipv4 = socket(AF_INET, SOCK_DGRAM, 0)
        else: self.socket_ipv4 = socket(AF_INET, SOCK_STREAM, 0)

    def bind(self, listen_num :int = None):
        self.socket_ipv4.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
        self.socket_ipv4.bind((self.listen_host, self.listen_port))
        if self.socket_ipv4.type == 1:
            self.socket_ipv4.listen(listen_num)
            self.socket_ipv4, self.socket_addr = self.socket_ipv4.accept()

    def connect(self):
        self.socket_ipv4.connect((self.target_host, self.target_port))

    def sendMessage(self, message :bytes):
        if self.socket_ipv4.type == 1:
            self.socket_ipv4.sendall(message)
        elif self.socket_ipv4.type == 2:
            self.socket_ipv4.sendto(message, (self.target_host, self.target_port))

    def recvMessage(self):
        return _socketRecv(self.socket_ipv4)

    def SyncMessage(self):
        pass

    def close(self):
        self.socket_ipv4.close()





