from Crypto.Util.Padding import pad, unpad
from typing import Union
import struct
import zlib

def build(data :bytes, width :int = 0, height :int = 0, colorType :int = 8, bitDepth :int = 0):
    def split_data(data :bytes, width :int):
        result = b''
        for i in range(0, len(data), width):
            tmp = data[i:i+width]
            if len(tmp) < width:
                tmp = pad(tmp, width)
            result += (b'\x00' + tmp)
        return result

    def build_chunk(name :Union[str, bytes], data :bytes):
        if isinstance(name, str):
            name = name.encode()
        size = len(data)
        chunk = struct.pack(f'!I4s{size}sI', size, name, data, zlib.crc32(name + data))
        return chunk

    def factor(n :int):
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
        return res[::-1][0]

    if not width or not height:
        width, height = factor(len(data))

    print(f'image info, width: {width}, height: {height}')

    head = b'\x89\x50\x4E\x47\x0D\x0A\x1A\x0A'
    ihdr = build_chunk('IHDR', struct.pack('!IIBBBBB', width, height, colorType, bitDepth, 0, 0, 0))
    srgb = b'\x00\x00\x00\x01\x73\x52\x47\x42\x00\xAE\xCE\x1C\xE9'
    idat = build_chunk('IDAT', zlib.compress(split_data(data, width)))
    iend = b'\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82'

    return head + ihdr + srgb + idat + iend

# data = 'fuck you'.encode()
with open('C:/Users/sn/Desktop/收纳/小南娘.txt', 'rb') as f:
    data = f.read()

png = build(data)

print(png)

with open('hidden_test.png', 'wb') as f:
    f.write(png)
