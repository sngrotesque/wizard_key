from snCrypto import snCrypto
from snSocket import baseChatClass
from socket import *

client = baseChatClass(target_host='sngrotesque.com', target_port=48894)
client.init()
client.connect()

pk_pkcs1 = client.recvMessage()
pk = snCrypto(rsa_pk = pk_pkcs1).rsa_load_key[0]
aes_key = snCrypto().aes256_createKey

aes_key_crypto = snCrypto(rsa_pk = pk).rsa_encrypt(aes_key, base64=True)
client.sendMessage(aes_key_crypto)

ctx = snCrypto(aes_key = aes_key)
result = ctx.aes256_decrypt(client.recvMessage(), base64=1)

print(result)




client.close()
