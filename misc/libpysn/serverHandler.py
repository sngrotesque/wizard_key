from snchat.crypto import snCrypto
from snchat._snSocket import baseChatClass
from socket import *

pk, sk = snCrypto(rsa_pk_fn='rsa_public.key', rsa_sk_fn='rsa_private.key').rsa_load_keys
client_pk = snCrypto(rsa_pk=pk).rsa_save_key[0]

server = baseChatClass(listen_host='0.0.0.0', listen_port=48894)
server.init()
server.bind(5)

server.sendMessage(client_pk)
aes_key_crypto = server.recvMessage()
aes_key = snCrypto(rsa_sk=sk).rsa_decrypt(aes_key_crypto, base64=1)

ctx = snCrypto(aes_key = aes_key)
text = 'hello, world.\n'.encode()
server.sendMessage(ctx.aes256_encrypt(text, base64=1))








server.close()
