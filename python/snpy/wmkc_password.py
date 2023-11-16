from .wmkc_crypto import *
from .wmkc_file import *
import json

class wmkcCrypto_password:
    def __init__(self, in_path :str, out_path :str):
        self.in_path = in_path
        self.out_path = out_path

        self.aes_key = None
        self.aes_iv  = None
        self.content = None

        self.ctx = None

    def encrypt(self, content: bytes):
        tmp = wmkcCrypto_randomPadding(content, blockSize=24)
        tmp = self.ctx.aes_encrypt(tmp[::-1])
        tmp = wmkcCrypto_base64_encode(tmp).decode()
        return tmp

    def decrypt(self, content :bytes):
        tmp = wmkcCrypto_base64_decode(content)
        tmp = self.ctx.aes_decrypt(tmp)[::-1]
        return tmp[:-tmp[-1]].decode()

    def load(self):
        self.content = json.loads(wmkcFile_fread(self.in_path, mode = 'r'))
        self.aes_key = wmkcCrypto_base64_decode(self.content['secret']['00']['key']['base64'])
        self.aes_iv  = wmkcCrypto_base64_decode(self.content['secret']['00']['iv']['base64'])
        self.ctx     = wmkcCrypto(key = self.aes_key, iv = self.aes_iv, mode = AES.MODE_CFB)

    def processJSON(self, enc :bool = True):
        for i in range(len(self.content) - 1):
            for account_index in range(len(self.content[f'{i:04x}']['accounts'])):
                username, password = self.content[f'{i:04x}']['accounts'][account_index]
                if enc:
                    username = self.encrypt(username.encode())
                    password = self.encrypt(password.encode())
                else:
                    username = self.decrypt(username.encode())
                    password = self.decrypt(password.encode())
                self.content[f'{i:04x}']['accounts'][account_index][0] = username
                self.content[f'{i:04x}']['accounts'][account_index][1] = password

    def write(self):
        wmkcFile_fwrite(self.out_path, json.dumps(self.content, indent=None), mode = 'w')
