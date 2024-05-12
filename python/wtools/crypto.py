from Crypto.Cipher import AES
import hashlib
import zlib

def aes_encrypt(data :bytes, key :bytes, mode :int, iv :bytes = ..., nonce :bytes = ..., counter :dict = ..., segment_size :int = ...):
    aes = AES.new(key, mode, IV = iv, nonce = nonce, counter = counter, segment_size = segment_size)
    return aes.encrypt(data)

def aes_decrypt(data :bytes, key :bytes, mode :int, iv :bytes = ..., nonce :bytes = ..., counter :dict = ..., segment_size :int = ...):
    aes = AES.new(key, mode, IV = iv, nonce = nonce, counter = counter, segment_size = segment_size)
    return aes.decrypt(data)

def hash_sum(name :str, data :bytes):
    ctx = hashlib.new(name)
    ctx.update(data)
    return ctx.digest(), ctx.hexdigest()

def zlib_crc32(data :bytes):
    return zlib.crc32(data)
