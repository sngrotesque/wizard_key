from Crypto.Cipher import AES
from PIL import Image
import struct
import zlib
import json
import os

def fread(fn :str):
    with open(fn, 'rb') as f:
        return f.read()

def fwrite(fn :str, data :bytes):
    with open(fn, 'wb') as f:
        f.write(data)

def checkImage(path :str):
    # 检测PNG图像是否正确
    try:
        img = Image.open(path)
        img.verify()
        print(f'Pillow   check result: OK!')
    except:
        print(f'Pillow   check result: ERROR!')
    pngcheck = os.popen(f'H:\pngcheck\pngcheck.exe {path}').readline()
    print(f'pngcheck check result: {pngcheck}')

def factor(n :int):
    # 求n的所有因式
    res, temp = [], []
    for x in range(1, int(n ** 0.5) + 1):
        if not n % x:
            temp.append(x)
            if x != n // x:
                temp.append(n // x)
    for i in range(len(temp)):
        for j in range(i + 1):
            if temp[i] * temp[j] == n:
                res.append((temp[i], temp[j]))
    return res[::-1]

def randPadding(BLOCKLEN :int, data :bytes):
    # 填充数据至指定长度
    paddingSize = BLOCKLEN - (len(data) % BLOCKLEN)
    data += os.urandom((paddingSize - 1))
    data += paddingSize.to_bytes(1, 'big')
    return data

def split(data :bytes, splitNumber :int):
    # 将传入的数据按照splitNumber指定的长度进行分组，并在每个分组前添加一个00
    data = [data[x:x+splitNumber] for x in range(0, len(data), splitNumber)]
    return b'\x00' + b'\x00'.join(data)

def setUint32(_n :int):
    return struct.pack('>I', _n)

def setCRC32(_data :bytes):
    return setUint32(zlib.crc32(_data))

def aes256_encrypt(data :bytes, key :bytes, iv :bytes):
    aes = AES.new(key = key, IV = iv, mode = AES.MODE_CBC)
    return aes.encrypt(data)

def aes256_decrypt(data :bytes, key :bytes, iv :bytes):
    aes = AES.new(key = key, IV = iv, mode = AES.MODE_CBC)
    return aes.decrypt(data)

class png:
    def __init__(self, data :bytes, encrypted :bool = False, note :str = None):
        self.AES_KEY = b'\t\xe7\xb1\x97n\xacw\xb1\x83[0+9r\xd4\x1d\xcaC\x8c\x05\xab?\xd5{\xbfs\xc5\xb5]\x0cL\xa9'
        self.AES_IV = b'Rt\xec\x1c\\>\xe3\xb23\x9d+\xcee\xf8-Q'
        self.BLOCKLEN = 16
        self.ENCRYPTED = encrypted

        self.data = data
        self.note = note

        self.pixel = None
        self.width = None
        self.height = None

    def genPixel(self):
        data = randPadding(self.BLOCKLEN, self.data)
        if self.ENCRYPTED:
            data = aes256_encrypt(data, self.AES_KEY, self.AES_IV)
        self.width, self.height = factor(len(data))[0]
        paddingSize = (self.width * self.height * 4) - 4 - len(data)
        data = data + os.urandom(paddingSize) + setUint32(paddingSize)
        return split(data, self.width * 4)

    def generate(self):
        self.pixel = self.genPixel()
        HEADER = b'\x89\x50\x4e\x47\x0d\x0a\x1a\x0a'
        IHDR = b'IHDR' + setUint32(self.width) + setUint32(self.height) + b'\x08\x06\x00\x00\x00'
        IHDR = setUint32(len(IHDR) - 4) + IHDR + setCRC32(IHDR)
        sRGB = b'\x00\x00\x00\x01\x73\x52\x47\x42\x00\xAE\xCE\x1C\xE9'
        iTXt = b''
        if self.note:
            iTXt = b'iTXttext "' + self.note.encode() + b'"'
            iTXt = setUint32(len(iTXt) - 4) + iTXt + setCRC32(iTXt)
        IDAT = b'IDAT' + zlib.compress(self.pixel)
        IDAT = setUint32(len(IDAT) - 4) + IDAT + setCRC32(IDAT)
        END = b'\x00\x00\x00\x00\x49\x45\x4e\x44\xae\x42\x60\x82'
        return HEADER + IHDR + sRGB + iTXt + IDAT + END


