from Crypto.Protocol.KDF import PBKDF2
from Crypto.Hash import SHA512
from Crypto.Cipher import AES
import base64

def aes_encrypt(plaintext :bytes, key :bytes, iv :bytes, mode :int = AES.MODE_CFB, segment_size :int = 8):
    aes_ctx = AES.new(key = key, IV = iv, mode = mode, segment_size = segment_size)
    return base64.b64encode(aes_ctx.encrypt(plaintext))

def aes_decrypt(ciphertext :bytes, key :bytes, iv :bytes, mode :int = AES.MODE_CFB, segment_size :int = 8):
    aes_ctx = AES.new(key = key, IV = iv, mode = mode, segment_size = segment_size)
    return aes_ctx.decrypt(base64.b64decode(ciphertext))

def generate_key_with_iv(password :bytes, salt :bytes, ):
    data = PBKDF2(password = password, salt = salt, dkLen = (32 + 16), count = 5215, hmac_hash_module = SHA512)
    return data[:32], data[-16:]

def xcrypt_cookie(cookie_path :str, password :bytes, salt :bytes, encrypt :bool):
    if isinstance(password, str):
        password = password.encode('utf-8')
    if isinstance(salt, str):
        salt = salt.encode('utf-8')

    with open(cookie_path, 'rb') as f:
        cookie = f.read()

    key, iv = generate_key_with_iv(password, salt)

    if encrypt:
        cookie = aes_encrypt(cookie, key, iv)
    else:
        cookie = aes_decrypt(cookie, key, iv)

    with open(cookie_path, 'wb') as f:
        f.write(cookie)

xcrypt_cookie('./pixiv_cookie.txt', '', '', 0)
