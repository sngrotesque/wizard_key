import binascii
import hashlib
import snpy
import random

def hexStream2rawStream(data :str):
    return binascii.a2b_hex(''.join(data.split()))

def bytesStream2binaryStream(data :bytes):
    return ''.join([f'{bin(x)[2:]:>08s}' for x in data])

res = hexStream2rawStream('c1 ce a9 07 25 20 d0 d0 e9 a2 78 5b be 2d 64 46')
print(res)
