import binascii
import rsa

import time

def newkeys(pk :rsa.PublicKey = None, sk :rsa.PrivateKey = None, bit_length :int = 2048):
    pk, sk = rsa.newkeys(bit_length)
    n = binascii.a2b_hex(hex(pk.n)[2:].zfill((bit_length // 8) << 1))
    d = binascii.a2b_hex(hex(sk.d)[2:].zfill((bit_length // 8) << 1))
    return {'n': n, 'd': d}

def test():
    s = time.time()
    res = newkeys()
    print(f'用时：{time.time() - s:.2f}')
    for x in res:
        print(f'{x}: {res[x]}')

if __name__ == '__main__':
    test()
