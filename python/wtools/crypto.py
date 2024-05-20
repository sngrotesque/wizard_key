from Crypto.Protocol.KDF import PBKDF2
from Crypto.Cipher import AES
from typing import Union
import hashlib
import os

# 如果args为字符串类型，那么会被作为路径使用，如果为字节串类型，那么会被当做数据使用。否则抛出运行时异常。
def get_digest(args :Union[str, bytes, bytearray], method = None):
    if not method:
        method = hashlib.sha256()
    block_size = 512 * 1024**2
    sha256_ctx = method
    if isinstance(args, str):
        with open(args, 'rb') as f:
            while True:
                data = f.read(block_size)
                if not data:
                    break
                sha256_ctx.update(data)
    elif isinstance(args, (bytes, bytearray)):
        for i in range(0, len(args), block_size):
            sha256_ctx.update(args[i:i + block_size])
    else:
        raise RuntimeError(f'unknown args: {args}.')
    return sha256_ctx.digest()

def generate_keyWithIV(password :bytes, salt :bytes, key_len :int = 32, iv_len :int = 16):
    content = PBKDF2(password, salt, (key_len + iv_len), 7881)
    return content[:key_len], content[-iv_len:]

def aes_encrypt(data :bytes, key :bytes, mode :int, iv :bytes = ..., nonce :bytes = ..., counter :dict = ..., segment_size :int = ...):
    aes = AES.new(key, mode, IV = iv, nonce = nonce, counter = counter, segment_size = segment_size)
    return aes.encrypt(data)

def aes_decrypt(data :bytes, key :bytes, mode :int, iv :bytes = ..., nonce :bytes = ..., counter :dict = ..., segment_size :int = ...):
    aes = AES.new(key, mode, IV = iv, nonce = nonce, counter = counter, segment_size = segment_size)
    return aes.decrypt(data)

class fcipher:
    def __init__(self, password :bytes, salt :bytes = None):
        self.password = password
        self.salt   = os.urandom(16) if not salt else salt
        if len(self.salt) != 16:
            raise ValueError('The length of salt must be 16 bytes.')

    def join_bytes(self, *args :bytes):
        return bytes().join(args)

    def new_aes_ctx(self):
        key, iv = generate_keyWithIV(self.password, self.salt, 32, 16)
        return AES.new(key, mode = AES.MODE_CFB, IV = iv, segment_size = 112)

    '''
    实现解密过程的时候请注意：
        请一定要实现具体判断（也就是说这里存在三种情况，1是数据被篡改，2是密码错误，3是密码错误的同时数据还被篡改了）。
        一定要在解密时抛出异常告知解密者是这三种情况的哪一种。
    '''
    def encrypt(self, input_path :str, output_path :str):
        aes = self.new_aes_ctx()
        
        # 读取明文并计算摘要后将摘要拼接在明文后面
        with open(input_path, 'rb') as f:
            plaintext = f.read()
            plaintext_digest = get_digest(plaintext)
            plaintext_final = self.join_bytes(plaintext_digest, plaintext)

        # 得到密文
        ciphertext = aes.encrypt(plaintext_final)
        # 获取密文摘要
        ciphertext_digest = get_digest(ciphertext)
        # 将密文摘要与用户密码拼接
        ciphertextDigest_width_password = self.join_bytes(ciphertext_digest, self.password)
        # 计算出密文摘要与用户密码拼接后的数据的摘要
        ciphertextDigest_width_password_digest = get_digest(ciphertextDigest_width_password)

        # 将盐与密码组合的摘要，盐，密文摘要，密文，密文摘要与密码的摘要拼接。
        with open(output_path, 'wb') as f:
            f.write(self.join_bytes(get_digest(self.join_bytes(self.salt, self.password)),
                self.salt, ciphertext_digest, ciphertext, ciphertextDigest_width_password_digest))

    def decrypt(self, input_path: str, output_path: str):
        with open(input_path, 'rb') as f:
            encrypted_content = f.read()

        salt_password_digest = encrypted_content[:32]
        salt                 = encrypted_content[32:48]
        ciphertext_digest    = encrypted_content[48:80]
        ciphertext           = encrypted_content[80:-32]
        ciphertextDigest_width_password_digest = encrypted_content[-32:]

        # 验证密码是否正确
        if get_digest(self.join_bytes(ciphertext_digest, self.password))\
            != ciphertextDigest_width_password_digest:
            raise ValueError('密码错误或数据被篡改，无法解密数据。')

        # 验证盐是否正确
        if get_digest(self.join_bytes(salt, self.password)) != salt_password_digest:
            raise ValueError('盐验证失败，数据已被篡改。')

        # 验证密文摘要
        if get_digest(ciphertext) != ciphertext_digest:
            raise ValueError('密文验证失败，文件已被篡改。')

        # 使用new_aes_ctx方法创建AES上下文
        self.salt = salt  # 更新实例变量salt
        aes = self.new_aes_ctx()

        # 解密
        decrypted_content = aes.decrypt(ciphertext)

        # 提取原始数据和摘要
        plaintext_digest = decrypted_content[:32]
        plaintext = decrypted_content[32:]

        # 验证数据完整性
        if get_digest(plaintext) != plaintext_digest:
            raise ValueError('数据验证失败，文件已被篡改。')

        # 写入解密后的数据
        with open(output_path, 'wb') as f:
            f.write(plaintext)
