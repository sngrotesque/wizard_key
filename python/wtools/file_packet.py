from . import fwrite, fread
from .crypto import zlib_crc32
import struct

'''
File header:      4 Bytes [0A 53 4E 50]

Block:
    File name length: 2 Bytes
    File name:        [FileNameLength] Bytes (UTF-8)
    Data length:      4 Bytes
    Data:             [DataLength] Bytes
    CRC32:            4 Bytes, Include[FileNameLength, FileName, DataLength, Data]

    split:            [01 11 00 10]
'''


