from Crypto.Cipher import AES

key = b'1234567890abcdef1234567890abcdef'
iv = b'1234567890abcdef'

def aes_cfb_encrypt(content, key, iv):
    aes = AES.new(key = key, IV = iv, mode = AES.MODE_CFB, segment_size = 64)
    return aes.encrypt(content)

data = b'........orld'
data = aes_cfb_encrypt(data, key, iv)
data = aes_cfb_encrypt(data, key, iv)
print(data)
