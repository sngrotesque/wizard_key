from .wmkc_net import *
import threading

wmkcVpn_default_port = 42761

class wmkcVpn:
    def run(self):
        pass

class wmkcVpn_client(wmkcVpn):
    def __init__(self,
                proxy_host  :str = '0.0.0.0',
                proxy_port  :int = 7466,
                server_host :str = None,
                server_port :int = wmkcVpn_default_port):
        self.proxy_host = proxy_host
        self.proxy_port = proxy_port
        self.server_host = server_host
        self.server_port = server_port

        self.ssl_sockfd = None
        self.sockfd = None

    def connect(self, content: bytes):
        pass

    def run(self):
        pass

class wmkcVpn_server(wmkcVpn):
    def __init__(self,
                listen_host :str = '0.0.0.0',
                listen_port :int = wmkcVpn_default_port,
                listen_num  :int = 32):
        self.listen_host = listen_host
        self.listen_port = listen_port
        self.listen_num  = listen_num

        self.sockfd = None

    def init(self):
        pass

    def accept(self):
        pass

    def run(self):
        pass


