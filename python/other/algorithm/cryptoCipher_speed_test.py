from Crypto.Util.Padding import pad
from Crypto.Cipher import ChaCha20, AES, DES3, DES
from Crypto.Random import get_random_bytes
from typing import Callable
import time

random_data = {
    '32_bit' : get_random_bytes(4),
    '64_bit' : get_random_bytes(8),
    '96_bit' : get_random_bytes(12),
    '128_bit': get_random_bytes(16),
    '192_bit': get_random_bytes(24),
    '256_bit': get_random_bytes(32)
}

def aes_test(p :bytes):
    aes_ctx = AES.new(key = random_data['256_bit'], nonce = random_data['64_bit'], mode = AES.MODE_CTR)
    return aes_ctx.encrypt(p)

def chacha20_test(p :bytes):
    chacha20_ctx = ChaCha20.new(key = random_data['256_bit'], nonce = random_data['192_bit'])
    return chacha20_ctx.encrypt(p)

def des3_test(p :bytes):
    des3_ctx = DES3.new(key = random_data['192_bit'], mode = DES3.MODE_CTR, nonce = random_data['32_bit'])
    return des3_ctx.encrypt(p)

def des_test(p :bytes):
    des_ctx = DES.new(key = random_data['64_bit'], mode = DES3.MODE_CTR, nonce = random_data['32_bit'])
    return des_ctx.encrypt(p)

def speed_test(func :Callable, p_length :int = 16):
    p = get_random_bytes(p_length * 1024**2)

    print('Start timing.')
    start_time = time.time()

    res = func(p)

    stop_time = time.time()
    print('End timing.')

    result_time = stop_time - start_time

    print(f'Test data length: {len(p)}bytes, {len(p) / 1024**2:.2f}mb.')
    print(f'time used: {result_time:.4f}')
    print(f'result[0]: {res[0]:02x}')

speed_test(aes_test, 16)

