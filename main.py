from snpy import *
import struct
import time
import rsa

key = b'0123456789abcdef0123456789abcdef'
iv = b'0123456789abcdef'
nonce = b'[a8371_]'

content = bytes([
    8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2
])

ctx = wmkcCrypto(key = key, iv = iv, nonce = nonce, mode = AES.MODE_CTR, segment_size=128)
res = ctx.aes_encrypt(content)

# print(res)
print(wmkcMisc_PRINT_BOX(res))
