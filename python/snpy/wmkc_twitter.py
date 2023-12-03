import struct
import random
import rsa
import re

class wmkcTwitter:
    def __init__(self):
        pass

    def _gen_rand_file_name(self, fn :str):
        pk, sk = rsa.newkeys(2048)
        random.seed(sk.d)
        _fn, _ftype = re.findall(r'(.*)\.([\w\.]+)$', fn, re.S)[0]
        _ascii = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_-.'
        _data = '_' + ''.join([random.choice(_ascii) for x in range(12)])
        return '.'.join([_fn+_data, _ftype])

    def encrypt(self, message :bytes):
        message = bytearray(message)
        for i in range(len(message)):
            message[i] = (message[i] + i) ^ 0xff
        message = [bytes(message[x:x+8]) for x in range(0, len(message), 8)]
        for x in range(len(message)):
            if len(message[x]) % 8:
                message[x] += (b'\x00' * (8 - (len(message[x]) % 8)))
            message[x] = struct.unpack('>Q', message[x])[0]
        return message
