from binascii import a2b_hex as a2b
from typing import Union, List
import threading
import hashlib
import struct
import lzma
import sys
import os

BlockSize = 128

def getFSize(fn :str):
    stat = os.stat(fn)
    return stat.st_size

def fileSet(folder :str, compress :bool = True):
    _set = []
    _path_list = os.walk(folder).__next__()[2]
    for fn in _path_list:
        fData = fread(fn)
        if compress:
            _set.append(lzma.compress(fData))
        else:
            _set.append()
    
    return tuple(_set)

def padding(data :bytes, view :bool = False):
    offset = (BlockSize - len(data) % BlockSize)
    format_char = f'>{offset-1}x'
    _result = data + struct.pack(format_char) + offset.to_bytes(1, 'big')
    if view: print(f'>>>> offset: [Dec {offset}] | [Hex {offset:02x}].')
    return _result

def sizeof(_object :object):
    return sys.getsizeof(_object)

def fread(fn :str, mode :str = 'rb', encoding = None):
    with open(fn, mode, encoding = encoding) as f:
        return f.read()

def fwrite(fn :str, data :object, mode :str = 'wb', encoding = None):
    with open(fn, mode, encoding = encoding) as f:
        f.write(data)

def sha256(data :object):
    _ = hashlib.sha256()
    _.update(data)
    return _.hexdigest()

def sha512(data :object):
    _ = hashlib.sha512()
    _.update(data)
    return _.hexdigest()


