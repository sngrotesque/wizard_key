'''
1. packet seq:    4 Bytes.
2. packet length: 4 Bytes.
3. packet crc32:  4 Bytes (seq + length).
4. packet data:   [packet length] Bytes.
5. packet sha256: 32 Bytes.
6. packet end:    8 Bytes. [53 4e 45 02 4e 01 44 00]
'''
from .crypto import w_crypto
from .utils import w_utils
import socket
import struct
import zlib

def pack_uint32(n :int):
    return struct.pack('!I', n)

class w_packet:
    def send(self, fd :socket.socket, seq :int, data :bytes):
        packet_data = w_utils().lzma2_compress(data)
        packet_seq = pack_uint32(seq)
        packet_len = pack_uint32(len(packet_data))
        packet_crc = pack_uint32(zlib.crc32(packet_data))
        packet = packet_seq + packet_len + packet_crc + packet_data
        packet += w_crypto().hash_sum('sha256', data = packet)[0]
        packet += b'\x53\x4e\x45\x02\x4e\x01\x44\x00'
        # fd.sendall(fd)
        print(packet)

