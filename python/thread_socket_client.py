import socket
import struct
import random
import json
import time
import os

fd = socket.socket()
fd.connect(('127.0.0.1', 8083))

packet = json.dumps({
    'time': round(time.time(), 3),
    'data': os.urandom(16).hex(),
    'uid': random.randint(0, 0xffff)
}).encode()
packet_len = struct.pack('>I', len(packet))

fd.send(packet_len)
fd.send(packet)

fd.close()

