import zlib
import struct
import os

class png:
    def __init__(self, width :int, height :int):
        self.head        = b'\x89\x50\x4E\x47\x0D\x0A\x1A\x0A'
        
        self.width       = width
        self.height      = height
        
        self.bit_depth   = 8
        self.colorType   = 2
        self.compression = 0
        self.filter      = 0
        self.scan        = 0
    
    def build_IHDR_chunk(self):
        IHDR_chunk = struct.pack('>I4sIIBBBBB',
            13,             # length
            b'IHDR',        # name
            self.width,     # image width
            self.height,    # image height
            self.bit_depth, # image bit depth
            self.colorType, # image color type
            self.compression,
            self.filter,
            self.scan)
        IHDR_crc32 = struct.pack('>I', zlib.crc32(IHDR_chunk[4:]))
        return IHDR_chunk + IHDR_crc32

    def build_IDAT_chunk(self, content :bytes):
        def split(data :bytes, splitNumber :int):
            # 将传入的数据按照splitNumber指定的长度进行分组，并在每个分组前添加一个00
            data = [data[x:x+splitNumber] for x in range(0, len(data), splitNumber)]
            return b'\x00' + b'\x00'.join(data)

        compressed = zlib.compress(split(content, self.width * 3))
        IDAT_chunk = struct.pack(f'>I4s{len(compressed)}s',
            len(compressed), # length
            b'IDAT',         # name
            compressed,      # data
            )
        IDAT_crc32 = struct.pack('>I', zlib.crc32(IDAT_chunk[4:]))
        return IDAT_chunk + IDAT_crc32

    def build_IEND_chunk(self):
        IEND_chunk = struct.pack('>I4s', 0, b'IEND')
        IEND_crc32 = struct.pack('>I', zlib.crc32(IEND_chunk[4:]))
        return IEND_chunk + IEND_crc32
