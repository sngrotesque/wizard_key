from Crypto.Random import get_random_bytes
from Crypto.Util.Padding import pad
from Crypto.Protocol.KDF import PBKDF2
from Crypto.Cipher import AES
from typing import Callable, Union
import binascii
import hashlib
import base64
import struct
import rsa

ECB = (1 << AES.MODE_ECB)
CBC = (1 << AES.MODE_CBC)
CTR = (1 << AES.MODE_CTR)
CFB = (1 << AES.MODE_CFB)
OFB = (1 << AES.MODE_OFB)

CCM = (1 << AES.MODE_CCM)
EAX = (1 << AES.MODE_EAX)
GCM = (1 << AES.MODE_GCM)
OCB = (1 << AES.MODE_OCB)

def wmkcCrypto_hashlib(method :Callable, data :bytes = None, path :str = None) -> Union[str, None]:
    if data:
        method.update(data)
    elif path:
        with open(path, 'rb') as f:
            while True:
                tmp = f.read(16777216)
                if not tmp:
                    break
                method.update(tmp)
    else:
        return None

    return method.hexdigest()

def wmkcCrypto_rsaNewkeys(nbits :int):
    return rsa.newkeys(nbits)

def wmkcCrypto_newSalt(saltLen :int = 32):
    return get_random_bytes(saltLen)

def wmkcCrypto_newKey(password :bytes, salt :bytes, keyLen :int = 32):
    return PBKDF2(password, salt, dkLen = keyLen)

def wmkcCrypto_padding(content :bytes, blockSize :int = 16, style = 'pkcs7'):
    return pad(content, blockSize, style = style)

def wmkcCrypto_randomPadding(content :bytes, blockSize :int = AES.block_size):
    offset = blockSize - len(content) % blockSize
    return content + get_random_bytes(offset - 1) + offset.to_bytes(1, 'big')

def wmkcCrypto_pubKeyToBytes(pk :rsa.PublicKey, bits :int):
    pubKey_N_len = struct.pack('>I', bits >> 3) # bits / 2^3 = bits / 8
    pubKey_N_bytes = binascii.a2b_hex(f'{pk.n:0{int(pubKey_N_len.hex(), 16)<<1}x}')
    pubKey_E_len = struct.pack('>I', 4)
    pubKey_E_bytes = struct.pack('>I', pk.e)
    return pubKey_N_len + pubKey_N_bytes + pubKey_E_len + pubKey_E_bytes

def wmkcCrypto_bytesToPubKey(pk_bytes :bytes):
    ptr = 0
    pubKey_N_len = struct.unpack('>I', pk_bytes[ptr:ptr+4])[0]; ptr += 4
    pubKey_N_bytes = pk_bytes[ptr:ptr+pubKey_N_len]; ptr += pubKey_N_len
    pubKey_E_len = struct.unpack('>I', pk_bytes[ptr:ptr+4])[0]; ptr += 4
    pubKey_E_bytes = struct.unpack('>I', pk_bytes[ptr:ptr+pubKey_E_len])[0]
    return rsa.PublicKey(n = int(pubKey_N_bytes.hex(), 16), e = pubKey_E_bytes)

def wmkcCrypto_base64_encode(content :bytes):
    return base64.b64encode(content)

def wmkcCrypto_base64_decode(content :bytes):
    return base64.b64decode(content)

class wmkcCrypto:
    def __init__(self,
            key    :bytes = None,
            iv     :bytes = None,
            nonce  :bytes = None,
            mode   :int   = AES.MODE_CBC,
            pubKey :rsa.PublicKey  = None,
            priKey :rsa.PrivateKey = None,
            segment_size      :int = 8):
        self.key    = key
        self.iv     = iv
        self.nonce  = nonce
        self.mode   = mode
        self.segment_size = segment_size

        self.pubKey = pubKey
        self.priKey = priKey

    def aes_encrypt(self, content :bytes) -> bytes:
        if (1 << self.mode) & CTR:
            aes = AES.new(key = self.key, mode = self.mode, nonce = self.nonce)
        elif (1 << self.mode) & CFB:
            aes = AES.new(key = self.key, IV = self.iv, mode = self.mode, segment_size = self.segment_size)
        elif (1 << self.mode) & (CBC | OFB):
            aes = AES.new(key = self.key, IV = self.iv, mode = self.mode)
        elif (1 << self.mode) & (ECB | CCM | EAX | GCM | OCB):
            aes = AES.new(key = self.key, mode = self.mode)

        return aes.encrypt(content)

    def aes_decrypt(self, content :bytes) -> bytes:
        if (1 << self.mode) & CTR:
            aes = AES.new(key = self.key, mode = self.mode, nonce = self.nonce)
        elif (1 << self.mode) & CFB:
            aes = AES.new(key = self.key, IV = self.iv, mode = self.mode, segment_size = self.segment_size)
        elif (1 << self.mode) & (CBC | OFB):
            aes = AES.new(key = self.key, IV = self.iv, mode = self.mode)
        elif (1 << self.mode) & (ECB | CCM | EAX | GCM | OCB):
            aes = AES.new(key = self.key, mode = self.mode)

        return aes.decrypt(content)

    def rsa_encrypt(self, content :bytes):
        return rsa.encrypt(content, self.pubKey)

    def rsa_decrypt(self, content :bytes):
        return rsa.decrypt(content, self.priKey)
