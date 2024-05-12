from fastgm import SM2
from gmssl import sm2

sk, pk = SM2.generate_key()

print(f'公钥：{pk}')
print(f'私钥：{sk}')

sm2_obj = sm2.CryptSM2(sk, pk)

cipher = sm2_obj.encrypt(b'hello,world')
print(cipher)
plain = sm2_obj.decrypt(cipher)
print(plain)
