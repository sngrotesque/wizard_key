from snpy import *
import rsa

net = wmkcNet(AF_INET, SOCK_STREAM, '0.0.0.0', 49281)
net.bind()
net.listen(5)
net.sockfd = net.accept()

pk, sk = wmkcCrypto_rsaNewkeys(2048)
net.writeChunk(wmkcCrypto_pubKeyToBytes(pk, 2048))

aes_key = rsa.decrypt(net.readChunk(), sk)
aes_iv = rsa.decrypt(net.readChunk(), sk)

content = wmkcFile_fread('p:/FruityLoopsStudioBackground.html')
ctx = wmkcCrypto(key = aes_key, iv = aes_iv, mode = AES.MODE_CTR, nonce = b'jia13n0s')
net.writeChunk(ctx.aes_encrypt(content))

print(wmkcCrypto_sha256(content))

net.close()
