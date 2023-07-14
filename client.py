from snpy import *
import rsa
import os

net = wmkcNet(AF_INET, SOCK_STREAM, '127.0.0.1', 49281)
net.connect()


pk = wmkcCrypto_bytesToPubKey(net.readChunk())
aes_key, aes_iv = os.urandom(32), os.urandom(16)

net.writeChunk(rsa.encrypt(aes_key, pk))
net.writeChunk(rsa.encrypt(aes_iv, pk))


ctx = wmkcCrypto(key = aes_key, iv = aes_iv, mode = AES.MODE_CTR, nonce = b'jia13n0s')
content = ctx.aes_decrypt(net.readChunk())

print(wmkcCrypto_sha256(content))

net.close()
