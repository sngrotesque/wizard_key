from Crypto.Protocol.KDF import PBKDF2
from Crypto.Cipher import ChaCha20
from Crypto.Hash import SHA256
from datetime import datetime
import os

RAND_bytes = os.urandom

def str_to_unix_timestamp(time_str :str, format :str = '%Y-%m-%d %H:%M:%S'):
    dt = datetime.strptime(time_str, format)  # 解析为datetime对象
    return int(dt.timestamp() * 1000)  # 乘以1000转为毫秒级

KEY_SIZE   = 32
SALT_SIZE  = 16
NONCE_SIZE = 12
ITERATIONS = 102401

def derive_key_and_nonce(password: str, salt: bytes) -> tuple[bytes, bytes]:
    derived = PBKDF2(password, salt,
                     dkLen = KEY_SIZE + NONCE_SIZE,
                     count = ITERATIONS,
                     hmac_hash_module = SHA256)
    return derived[:KEY_SIZE], derived[KEY_SIZE:]

def chacha20_encrypt(data :bytes, password :str):
    salt = RAND_bytes(SALT_SIZE)
    key, nonce = derive_key_and_nonce(password, salt)
    
    cipher = ChaCha20.new(key = key, nonce = nonce)

    ciphertext = salt + cipher.encrypt(data)

    return ciphertext

def chacha20_decrypt(data :bytes, password :str):
    salt = data[:SALT_SIZE]
    ciphertext = data[SALT_SIZE:]
    key, nonce = derive_key_and_nonce(password, salt)

    cipher = ChaCha20.new(key = key, nonce = nonce)

    plaintext = cipher.decrypt(ciphertext)

    return plaintext

def main():
    ...

if __name__ == '__main__':
    main()
