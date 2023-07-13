from Crypto.Random import get_random_bytes
from Crypto.Protocol.KDF import PBKDF2
from Crypto.Cipher import AES
import hashlib
import rsa

ECB = (1 << AES.MODE_ECB)
CCM = (1 << AES.MODE_CCM)
EAX = (1 << AES.MODE_EAX)
GCM = (1 << AES.MODE_GCM)
OCB = (1 << AES.MODE_OCB)

def wmkcCrypto_sha256(content :object, path :str = None):
    hash_object = hashlib.sha256()
    if path:
        with open(path, 'rb') as f:
            hash_object.update(f.read(409600))
    else:
        hash_object.update(content)
    return hash_object.hexdigest()

def wmkcCrypto_rsaNewkeys(nbits :int):
    return rsa.newkeys(nbits)

def wmkcCrypto_newSalt(saltLen :int = 32):
    return get_random_bytes(saltLen)

def wmkcCrypto_newKey(password :bytes, salt :bytes, keyLen :int = 32):
    return PBKDF2(password, salt, dkLen = keyLen)

class wmkcCrypto:
    def __init__(self,
            key    :bytes = None,
            iv     :bytes = None,
            nonce  :bytes = None,
            salt   :bytes = None,
            passwd :bytes = None,
            mode   :int   = AES.MODE_CBC,
            pubKey :rsa.PublicKey  = None,
            priKey :rsa.PrivateKey = None):
        self.key    = key
        self.iv     = iv
        self.nonce  = nonce
        self.salt   = salt
        self.passwd = passwd
        self.mode   = mode

        self.pubKey = pubKey
        self.priKey = priKey

    def aes_encrypt(self, content :bytes) -> bytes:
        if self.mode == AES.MODE_CTR:
            aes = AES.new(key = self.key, mode = self.mode, nonce = self.nonce)
        elif self.mode == AES.MODE_CBC:
            aes = AES.new(key = self.key, IV = self.iv, mode = self.mode)
        elif (1 << self.mode) & (ECB | CCM | EAX | GCM | OCB):
            aes = AES.new(key = self.key, mode = self.mode)

        return aes.encrypt(content)

    def aes_decrypt(self, content :bytes) -> bytes:
        ECB, CCM, EAX = (1 << AES.MODE_ECB), (1 << AES.MODE_CCM), (1 << AES.MODE_EAX)
        GCM, OCB      = (1 << AES.MODE_GCM), (1 << AES.MODE_OCB)

        if self.mode == AES.MODE_CTR:
            aes = AES.new(key = self.key, mode = self.mode, nonce = self.nonce)
        elif self.mode == AES.MODE_CBC:
            aes = AES.new(key = self.key, IV = self.iv, mode = self.mode)
        elif (1 << self.mode) & (ECB | CCM | EAX | GCM | OCB):
            aes = AES.new(key = self.key, mode = self.mode)

        return aes.decrypt(content)

    def rsa_encrypt(self, content :bytes):
        return rsa.encrypt(content, self.pubKey)

    def rsa_decrypt(self, content :bytes):
        return rsa.decrypt(content, self.priKey)
