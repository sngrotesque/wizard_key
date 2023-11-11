import struct
import zlib

def fread(fn :str):
    with open(fn, 'rb') as f:
        return f.read()

def fwrite(fn :str, data :bytes):
    with open(fn, 'wb') as f:
        f.write(data)

def split(data :bytes, splitNumber :int):
    # 将传入的数据按照splitNumber指定的长度进行分组，并在每个分组前添加一个00
    data = [data[x:x+splitNumber] for x in range(0, len(data), splitNumber)]
    return b'\x00' + b'\x00'.join(data)

def setUint32(_n :int):
    return struct.pack('>I', _n)

def setCRC32(_data :bytes):
    return setUint32(zlib.crc32(_data))

def generate(width :int, height :int):
    HEAD = b'\x89\x50\x4e\x47\x0d\x0a\x1a\x0a'
    IHDR = b'IHDR' + setUint32(width) + setUint32(height) + b'\x08\x06\x00\x00\x00'
    IHDR = setUint32(len(IHDR) - 4) + IHDR + setCRC32(IHDR)
    sRGB = b'\x00\x00\x00\x01\x73\x52\x47\x42\x00\xAE\xCE\x1C\xE9'
    pixel = zlib.compress(split(b'\x00\x00\x00\x00' * (width * height), width * 4))
    IDAT = b'IDAT' + pixel
    IDAT = setUint32(len(IDAT) - 4) + IDAT + setCRC32(IDAT)
    IEND = b'\x00\x00\x00\x00\x49\x45\x4e\x44\xae\x42\x60\x82'
    PNG = HEAD + IHDR + sRGB + IDAT + IEND
    return PNG

fwrite('p:/transparent.png', generate(3840, 2160))
