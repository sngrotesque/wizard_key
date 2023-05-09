'''
RSA算法流程文字描述如下：
    1. 任意客户端对服务器发起请求，服务器首先发回复自己的公钥到客户端（公钥明文传输）。
    2. 客户端使用随机数算法，生成一个密钥S，使用收到的公钥进行 加密，生成C，把C发送到服务器。
    3. 服务器收到C，使用公钥对应的私钥进行解密，得到S。
    4. 上述交换步骤后，客户端和服务器都得到了S，S为密钥（预主密钥）。

DHE算法流程文字描述如下：
    1. 客户端计算一个随机值Xa，使用Xa作为指数，即计算Pa = q^Xa mod p，其中q和p是全世界公认的一对值。客户端把Pa发送至服务器，Xa作为自己私钥，仅且自己知道。
    2. 服务器和客户端计算流程一样，生成一个随机值Xb，使用Xb作为指数，计算Pb = q^Xb mod p，将结果Pb发送至客户端，Xb仅自己保存。
    3. 客户端收到Pb后计算Sa = Pb ^Xa mod p；服务器收到Pa后计算Sb = Pa^Xb mod p
    4. 算法保证了Sa = Sb = S，故密钥交换成功，S为密钥（预主密钥）。

ECDHE算法流程文字描述如下：
    1. 客户端随机生成随机值Ra，计算Pa(x, y) = Ra * Q(x, y)，Q(x, y)为全世界公认的某个椭圆曲线算法的基点。将Pa(x, y)发送至服务器。
    2. 服务器随机生成随机值Rb，计算Pb(x,y) - Rb * Q(x, y)。将Pb(x, y)发送至客户端。
    3. 客户端计算Sa(x, y) = Ra * Pb(x, y)；服务器计算Sb(x, y) = Rb *Pa(x, y)
    4. 算法保证了Sa = Sb = S，提取其中的S的x向量作为密钥（预主密钥）。
'''

from base64 import b64encode, b64decode
from .utils import fread, fwrite
from Crypto.Cipher import AES
from os import urandom
import rsa as RSA

class snCrypto:
    def __init__(self,
            aes_key :bytes = None,
            rsa_pk :RSA.PublicKey = None,
            rsa_pk_fn :str = None,
            rsa_sk :RSA.PrivateKey = None,
            rsa_sk_fn :str = None,
        ):
        self.AES_IV = b'\xcd\x8a\x9a\x1e\xe7\x07j\xac\n\xfb\xb9&p\x9f\x89\xd7'
        self.BlockSize = 32
        
        self.aes_key = aes_key
        self.rsa_pk = rsa_pk
        self.rsa_pk_fn = rsa_pk_fn
        self.rsa_sk = rsa_sk
        self.rsa_sk_fn = rsa_sk_fn

    @property
    def aes256_createKey(self):
        return urandom(self.BlockSize)

    @property
    def rsa_createKey(self):
        publicKey, privateKey = RSA.newkeys((self.BlockSize + 11) * 8)
        return publicKey, privateKey

    @property
    def rsa_load_key(self):
        pk, sk = None, None
        try:
            pk = RSA.PublicKey.load_pkcs1(self.rsa_pk)
            sk = RSA.PrivateKey.load_pkcs1(self.rsa_sk)
        except:
            pass
        return pk, sk

    @property
    def rsa_save_key(self):
        pk_pkcs1, sk_pkcs1 = None, None
        try:
            pk_pkcs1 = RSA.PublicKey.save_pkcs1(self.rsa_pk)
            sk_pkcs1 = RSA.PrivateKey.save_pkcs1(self.rsa_sk)
        except:
            pass
        return pk_pkcs1, sk_pkcs1

    @property
    def rsa_load_keys(self):
        pk_file = fread(self.rsa_pk_fn)
        sk_file = fread(self.rsa_sk_fn)
        pk = RSA.PublicKey.load_pkcs1(pk_file)
        sk = RSA.PrivateKey.load_pkcs1(sk_file)
        return pk, sk

    @property
    def rsa_save_keys(self):
        pk_pkcs1 = RSA.PublicKey.save_pkcs1(self.rsa_pk)
        sk_pkcs1 = RSA.PrivateKey.save_pkcs1(self.rsa_sk)
        fwrite(self.rsa_pk_fn, pk_pkcs1)
        fwrite(self.rsa_sk_fn, sk_pkcs1)

    def aes256_encrypt(self, message :bytes, base64 :bool = False):
        AES_CTX = AES.new(key=self.aes_key, iv=self.AES_IV, mode=AES.MODE_CFB)
        if base64: return b64encode(AES_CTX.encrypt(message))
        return AES_CTX.encrypt(message)

    def aes256_decrypt(self, ciphertext :bytes, base64 :bool = False):
        AES_CTX = AES.new(key=self.aes_key, iv=self.AES_IV, mode=AES.MODE_CFB)
        if base64: return AES_CTX.decrypt(b64decode(ciphertext))
        return AES_CTX.decrypt(ciphertext)

    def rsa_encrypt(self, message :bytes, base64 :bool = False):
        if base64: return b64encode(RSA.encrypt(message, self.rsa_pk))
        return RSA.encrypt(message, self.rsa_pk)

    def rsa_decrypt(self, ciphertext :bytes, base64 :bool = False):
        if base64: return RSA.decrypt(b64decode(ciphertext), self.rsa_sk)
        return RSA.decrypt(ciphertext, self.rsa_sk)





