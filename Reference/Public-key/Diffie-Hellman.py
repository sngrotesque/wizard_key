'''Diffie-Hellman算法简单实现

简介：
    一种确保共享KEY安全穿越不安全网络的方法，它是一种密钥交换算法。
    Whitefield与Martin Hellman在1976年提出了一个奇妙的密钥交换协议，称为Diffie-Hellman密钥交换协议/算法。
    这个机制的巧妙在于需要安全通信的双方可以用这个方法确定对称密钥，然后可以用这个密钥进行加密和解密。
    但是注意，这个密钥交换协议/算法只能用于密钥的交换，而不能进行消息的加密和解密。
    双方确定要用的密钥后，要使用其他对称密钥操作加密算法实现加密和解密消息。
'''

# 有两个公开的数（其中必须有一个是质数）
PublicPrime = 197
PublicGenerator = 137

# 有两个各自保存的数（其中必须有一个是质数）
SN_NumberKeptSecret = 108967
LM_NumberKeptSecret = 135433

# 公开公式 + 公开数据
def publicFormula(privateNum :int):
    return (PublicGenerator**privateNum) % PublicPrime

# 私有公式 + 私有数据
def privateFormula(privateNum :int, publicKey :int):
    return (publicKey**privateNum) % PublicPrime

# 通过公开公式求出公钥
SN_PublicKey = publicFormula(SN_NumberKeptSecret)
LM_PublicKey = publicFormula(LM_NumberKeptSecret)

# 通过对方求出来公钥与自己的秘密数字运算求出私钥
SN_PrivateKey = privateFormula(SN_NumberKeptSecret, LM_PublicKey)
LM_PrivateKey = privateFormula(LM_NumberKeptSecret, SN_PublicKey)

print(SN_PrivateKey)
print(LM_PrivateKey)
