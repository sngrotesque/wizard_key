from typing import Callable
import wtools
import time
import sys
import os

def attack_fcipher(ciphertext_path :str):
    with open(ciphertext_path, 'rb') as f:
        encrypted_content = f.read()

    salt_digest       = encrypted_content[:32]
    salt              = encrypted_content[32:48]
    ciphertext_digest = encrypted_content[48:80]
    ciphertext        = encrypted_content[80:-32]
    ciphertextDigest_width_password_digest = encrypted_content[-32:]

    ciphertext        = os.urandom(256)
    ciphertext_digest = wtools.get_digest(ciphertext)

    with open(ciphertext_path, 'wb') as f:
        f.write(wtools.fcipher.join_bytes(salt_digest, salt, ciphertext_digest, ciphertext,
                ciphertextDigest_width_password_digest))

def fcipher_xcrypt():
    if len(sys.argv) < 5:
        exit(f'usage: python {sys.argv[0]} [Password] [input_path] [output_path] [e]ncrypt/[d]ncrypt')
    
    password = sys.argv[1]
    input_path = sys.argv[2]
    output_path = sys.argv[3]
    xcrypt_mode = sys.argv[4].lower()
    
    if input_path == output_path:
        print(f'input_path == output_path, are you sure?')
        choice = input('[y/N]').lower()
        if (choice == '') or (choice == 'n') or (choice == 'no'):
            exit('exit.')
        elif (choice == 'y') or (choice == 'yes'):
            print('continue.')
        else:
            exit('error input.')
    
    ctx = wtools.fcipher(password.encode())
    
    if xcrypt_mode == 'e':
        ctx.encrypt(input_path, output_path)
    elif xcrypt_mode == 'd':
        ctx.decrypt(input_path, output_path)
    else:
        exit(f'unknown xcrypt_mode.')

from Crypto.Cipher import AES
from Crypto.Util import Counter

def aes_speed_test(key: bytes, iv: bytes):
    buffer = bytes(256 * 1024**2)
    ctr = Counter.new(AES.block_size - (len(iv) * 8), prefix=iv, initial_value=0)
    aes = AES.new(key, AES.MODE_CTR, counter=ctr)

    timer_result = 0
    for count in range(10):
        print(f'Count: {count + 1}')
        timer_start = time.time()
        encrypted_content = aes.encrypt(buffer)
        timer_stop = time.time()
        timer_result += (timer_stop - timer_start)

    print(encrypted_content[0])
    print(f'10 time used: {timer_result/10:.4f}')

if __name__ == '__main__':
    aes_speed_test(b'00000000000000000000000000000000', b'00000')

