# from time import time, ctime, strftime, localtime, sleep
from base64 import b64encode as b64en, b64decode as b64de
from random import choice, sample
try:
    from Crypto.Cipher import AES
except:
    AES = 0
from os import system
system("cls")

def add_key(_r :set, _n :int, _range :int):
    MMMAX = len(hex(_r[1]).replace("0x",""))
    arr = [f"%0{MMMAX}X"%x for x in range(_r[0], _r[1] + 1)]
    for x in range(_n):
        print(f"0x{choice(arr)}, ", end="")
        if (x+1) % _range == 0:
            print()
    print()

if AES != 0:
    def aes256(p, k, iv = None):
        aes = AES.new(k, AES.MODE_CTR, iv)
        return aes.encrypt(p)

def main():
    add_key((0x00, 0xff), 256, 8)

if __name__ == "__main__":
    main()



























