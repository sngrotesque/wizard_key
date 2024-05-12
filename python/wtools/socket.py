import socket

def tcp_socket(family :int = socket.AF_INET):
    return socket.socket(family, socket.SOCK_STREAM, socket.IPPROTO_TCP)

def udp_socket(family :int = socket.AF_INET):
    return socket.socket(family, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
