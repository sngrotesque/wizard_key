from Crypto.Cipher import AES
import hashlib
import rsa
import os

def GeneratePairPublicPrivatekeys(pk_save_path :str, sk_save_path :str, nbit :int = 2048):
    pk, sk = rsa.newkeys(nbit)
    with open(pk_save_path, 'wb') as f:
        f.write(pk.save_pkcs1())
    with open(sk_save_path, 'wb') as f:
        f.write(sk.save_pkcs1())

def UnableVerifySecurityFile(path :str):
    raise RuntimeError(f'File \'{path}\' has been tampered with. the security of this file '
        'cannot be verified.')

def sha256(content :bytes):
    return hashlib.sha256(content).digest()

def aes_ctr_xcrypt(content :bytes, key :bytes, nonce :bytes):
    return AES.new(mode = AES.MODE_CTR, key = key, nonce = nonce).encrypt(content)

class fc:
    def __init__(self, inPath :str, outPath :str, PubKeyPEM_Path :str, PriKeyPEM_Path :str, overwriteFile :bool = True):
        if os.stat(inPath).st_size > 2_147_483_648:
            raise RuntimeError(f'The \'{inPath}\' file exceeds 2GB in size, please split the file!')
        if inPath == outPath:
            raise RuntimeError(
                f'\'{inPath}\' and \'{outPath}\' are the same, please use different paths.')
        if not overwriteFile:
            if os.path.exists(outPath):
                raise RuntimeError(f'Path \'{outPath}\' already has a file with the same name, '
                    'and the parameter for this class is set to \'Do not overwrite the original file\'.')

        self.rsa_pk  :rsa.PublicKey  = None
        self.rsa_sk  :rsa.PrivateKey = None

        self.aes_key   :bytes = None
        self.aes_nonce :bytes = None
        self.aes_mode  :int   = AES.MODE_CTR

        self.inPath  = inPath
        self.outPath = outPath

        with open(PubKeyPEM_Path, 'r', encoding='UTF-8') as f:
            self.rsa_pk = rsa.PublicKey.load_pkcs1(f.read())
        with open(PriKeyPEM_Path, 'r', encoding='UTF-8') as f:
            self.rsa_sk = rsa.PrivateKey.load_pkcs1(f.read())

    def encrypt(self):
        self.aes_key = os.urandom(32)
        self.aes_nonce = os.urandom(8)
        with open(self.outPath, 'wb') as fw:
            with open(self.inPath, 'rb') as fr:
                plaintext = fr.read()
                plaintextDigest = sha256(plaintext)

                aes_key_ciphertext = rsa.encrypt(self.aes_key, self.rsa_pk)
                aes_nonce_ciphertext = rsa.encrypt(self.aes_nonce, self.rsa_pk)
                plaintextDigest_ciphertext = rsa.encrypt(plaintextDigest, self.rsa_pk)
                ciphertext = aes_ctr_xcrypt(plaintext, self.aes_key, self.aes_nonce)

                fw.write(aes_key_ciphertext)
                fw.write(aes_nonce_ciphertext)
                fw.write(plaintextDigest_ciphertext)
                fw.write(ciphertext)

    def decrypt(self):
        with open(self.outPath, 'wb') as fw:
            with open(self.inPath, 'rb') as fr:
                try:
                    self.aes_key = rsa.decrypt(fr.read(self.rsa_pk.n.bit_length() >> 3), self.rsa_sk)
                    self.aes_nonce = rsa.decrypt(fr.read(self.rsa_pk.n.bit_length() >> 3), self.rsa_sk)
                    plaintextDigest = rsa.decrypt(fr.read(self.rsa_pk.n.bit_length() >> 3), self.rsa_sk)
                except rsa.pkcs1.DecryptionError:
                    UnableVerifySecurityFile(self.inPath)
                plaintext = aes_ctr_xcrypt(fr.read(), self.aes_key, self.aes_nonce)
                tmp_digest = sha256(plaintext)
                if tmp_digest != plaintextDigest:
                    UnableVerifySecurityFile(self.inPath)
                fw.write(plaintext)

inPath  = 'i:/Pitchers/'
outPath = 'C:/Users/z7z-h/Desktop/fileCipher_test/W_plaintext.png'
ctx = fc(inPath, outPath, './RSA_PubKey.pem', './RSA_PriKey.pem')
ctx.decrypt()
