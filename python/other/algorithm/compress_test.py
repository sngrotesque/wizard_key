from faker import Faker
import gzip
import lz4.frame
import lzo
import lzma
import time
import zlib

class compression_test:
    def __init__(self, data :bytes):
        self.CYAN = '\x1b[96m'
        self.RST = '\x1b[0m'
        self.GOLD = '\x1b[93m'
        self.data = data
        print(f'The length of data used for testing: {self.CYAN}{len(self.data)}{self.RST} Bytes.')

    def print_results(self, name :str, time_used :float, length :int):
        print(f'[+] {self.GOLD}{name}{self.RST} time used: {self.CYAN}{time_used:.4f}{self.RST}.')
        print(f'[+] {self.GOLD}{name}{self.RST} comparison of compressed data length: '
            f'[{self.CYAN}{length}{self.RST}|{self.CYAN}{len(self.data)}{self.RST}]. '
            f'Ratio of compressed data to raw data: {(length/len(self.data))*100:.2f}%.')

    def lzma_test(self, format = lzma.FORMAT_RAW, filters = [{"id": lzma.FILTER_LZMA2}]):
        ctx = lzma.LZMACompressor(format = format, filters = filters)
        start = time.time()
        result = ctx.compress(self.data)
        result += ctx.flush()
        stop = time.time()
        self.print_results('lzma', stop-start, len(result))

    def zlib_test(self):
        start = time.time()
        result = zlib.compress(self.data)
        stop = time.time()
        self.print_results('zlib', stop-start, len(result))

    def lzo_test(self):
        start = time.time()
        result = lzo.compress(self.data)
        stop = time.time()
        self.print_results('lzo', stop-start, len(result))

    def lz4_test(self):
        start = time.time()
        result = lz4.frame.compress(self.data)
        stop = time.time()
        self.print_results('lz4', stop-start, len(result))

    def gzip_test(self):
        start = time.time()
        result = gzip.compress(self.data)
        stop = time.time()
        self.print_results('gzip', stop-start, len(result))

data = b''

for x in range(256):
    data += Faker('zh_CN').text().encode()

c = compression_test(data)
c.lzma_test()
c.zlib_test()
c.gzip_test()
c.lzo_test()
c.lz4_test()
