import struct
import zlib
import sys
import os

def process(data :bytes):
    BLOCKLEN = 16

    def randPadding(data :bytes):
        # 填充数据至指定长度
        data += os.urandom(len(data))
        paddingSize = BLOCKLEN - (len(data) % BLOCKLEN)
        data += os.urandom((paddingSize - 1))
        data += paddingSize.to_bytes(1, 'big')
        return data

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
        return res[::-1]

    def split(data :bytes, splitNumber :int):
        data = [data[x:x+splitNumber] for x in range(0, len(data), splitNumber)]
        return b'\x00' + b'\x00'.join(data)

    data = randPadding(data)
    width, height = factor(len(data))[0]
    data += os.urandom(width * height * 4)
    data = split(data, width * 4)
    return (data, [width, height])

data = ''.encode()
data, (width, height) = process(data)
print(f'data: {data[:16]}')
print(f'size: {len(data)}')
print(f'width: {width}')
print(f'height: {height}\n')

pixel = zlib.compress(data)
width = struct.pack('>I', width)
height = struct.pack('>I', height)
IHDR_CRC = zlib.crc32(b'IHDR'+width+height+b'\x08\x06\x00\x00\x00')
IHDR_CRC = struct.pack('>I', IHDR_CRC)
IDAT_CRC = zlib.crc32(b'IDAT'+pixel)
IDAT_CRC = struct.pack('>I', IDAT_CRC)

print(f'pixel: {pixel.hex()}')
print(f'width: {width.hex()}')
print(f'height: {height.hex()}')
print(f'IHDR_CRC: {IHDR_CRC.hex()}')
print(f'IHDR_CRC: {IDAT_CRC.hex()}')

