from base64 import (b64encode, b64decode, b85encode, b85decode)
from binascii import a2b_hex as a2b
from random import choice

def addkey(Length: int) -> str:
    number_list = [x for x in range(1*10**3+100, 1*10**4-100)]
    key = []
    for x in range(Length):
        key.append(choice(number_list))
        key[x] = str(key[x])[::-1]
    key = hex(int("".join(key)[::-1])).replace('0x', '')
    if len(key) % 2 == 1:
        key = "c"+key
    key = list(b85encode(b64encode(a2b(key))))
    for x in range(len(key)):
        key[x] = key[x] ^ 0x21
        key[x] = hex(key[x]).replace('0x', '')
        if len(key[x]) == 1:
            key[x] = "0"+key[x]
    key = b85encode(a2b("".join(key))).decode()
    return key

def openkey(KeyData: str) -> list:
    key = list(b85decode(KeyData.encode()))
    for x in range(len(key)):
        key[x] = hex(key[x] ^ 0x21).replace('0x', '')
        if len(key[x]) == 1:
            key[x] = "0"+key[x]
    key = b64decode(b85decode(a2b("".join(key)))).hex()
    if key[0] == 'c':
        key = list(key)
        key.pop(0)
        key = "".join(key)
    key = str(int("0x"+key, 16))[::-1]
    Copy = []
    index = 0
    for x in range(len(key)//4):
        Copy.append(key[index:index+4])
        Copy[x] = int(Copy[x][::-1])
        index += 4
    return Copy

class TKTKSK:
    def __init__(self, data: bytes, key: str, move: int) -> bytes:
        self.Data = data
        self.Key = key
        self.Move = move

    def __repr__(self):
        return "Data length: {} byte.".format(len(self.Data))

    @property
    def encrypt(self):
        p = list(self.Data)
        k = openkey(self.Key)
        m = self.Move
        c = [1 for x in range(len(p))]

        index = 0
        for x in range(len(p)):
            try:
                c[x] = p[x] ^ k[index]
            except IndexError:
                index = 0
                c[x] = p[x] ^ k[index]
            c[x]=str(c[x])[::-1]
            index += 1
        del(p)
        c="".join(c)

        return c

    @property
    def decrypt(self):
        pass
