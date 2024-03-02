from .utils import w_utils
from Crypto.Protocol.KDF import PBKDF2
from Crypto.Cipher import AES
import hashlib

class w_crypto:
    def hash_sum(self, hash_name :str, **kwargs):
        obj = hashlib.new(hash_name)
        if kwargs.get('path'):
            path = kwargs.get('path')
            with open(path, 'rb') as f:
                obj.update(f.read())
        elif kwargs.get('data'):
            obj.update(kwargs.get('data'))
        return obj.digest(), obj.hexdigest()

    def aes_ctr_xcrypt(self, data :bytes, key :bytes, nonce :bytes):
        return AES.new(key, AES.MODE_CTR, nonce = nonce).encrypt(data)

    def aes_cbc_encrypt(self, data :bytes, key :bytes, iv :bytes):
        return AES.new(key, AES.MODE_CBC, IV = iv).encrypt(data)

    def aes_cbc_decrypt(self, data :bytes, key :bytes, iv :bytes):
        return AES.new(key, AES.MODE_CBC, IV = iv).decrypt(data)

    def aes_cfb_encrypt(self, data :bytes, key :bytes, iv :bytes, segment_size :int):
        return AES.new(key, AES.MODE_CFB, IV = iv, segment_size = segment_size).encrypt(data)

    def aes_cfb_decrypt(self, data :bytes, key :bytes, iv :bytes, segment_size :int):
        return AES.new(key, AES.MODE_CFB, IV = iv, segment_size = segment_size).decrypt(data)



