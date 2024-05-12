import gzip
import lzma
import zlib

# Compress
def gzip_comp(data :bytes, level :int = 9):
    return gzip.compress(data, compresslevel = level)

def lzma2_comp(data :bytes, level :int = None, format = lzma.FORMAT_RAW, filters = [{"id": lzma.FILTER_LZMA2}]):
    return lzma.compress(data, preset = level, format = format, filters = filters)

def zlib_comp(data :bytes, level :int = -1):
    return zlib.compress(data, level = level)

# Decompress
def gzip_decomp(data :bytes):
    return gzip.decompress(data)

def lzma2_decomp(data :bytes, format = lzma.FORMAT_RAW, filters = [{"id": lzma.FILTER_LZMA2}]):
    return lzma.decompress(data, format = format, filters = filters)

def zlib_decomp(data :bytes):
    return zlib.decompress(data)
