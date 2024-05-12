from Crypto.Cipher import AES
from typing import Union
import hashlib
import struct
import base64
import json
import os

coreKey = b'\x68\x7a\x48\x52\x41\x6d\x73\x6f\x35\x6b\x49\x6e\x62\x61\x78\x57'
metaKey = b'\x23\x31\x34\x6c\x6a\x6b\x5f\x21\x5c\x5d\x26\x30\x55\x3c\x27\x28'
ncmHead = b'CTENFDAM'

def checkFileType(fileHeaders :bytes):
    if fileHeaders != ncmHead:
        exit(f'>>>> 错误的文件类型！')

def DelFilledData(data :Union[bytes, bytearray]):
    return data[0:-data[-1]]

def fread(fn :str, mode :str = 'rb', encoding = None):
    with open(fn, mode, encoding = encoding) as f:
        return f.read()

def fwrite(fn :str, data :object, mode :str = 'wb', encoding = None):
    with open(fn, mode, encoding = encoding) as f:
        f.write(data)

def CreateKeyBox(keySize :int, ncm_key: Union[bytes, bytearray]):
    key_box = bytearray(range(256))
    byte = lastByte = keyOffset = 0
    for x in range(256):
        if keyOffset >= keySize:
            keyOffset = 0
        swap = key_box[x]
        byte = (swap + lastByte + ncm_key[keyOffset]) & 0xff
        key_box[x] = key_box[byte]
        key_box[byte] = swap
        lastByte = byte
        keyOffset += 1
    return key_box

def sha256(data :object):
    _hash = hashlib.sha256()
    _hash.update(data)
    return _hash.hexdigest()

class NcmDump:
    def __init__(self,
            in_file       :str = None, # 可以是文件也可以是文件夹
            out_file      :str = None,
            out_folder    :str = None,
            SaveSongCover :Union[str, bool] = False,
            SaveCRC       :Union[str, bool] = False
        ):
        self.in_file       = in_file
        self.out_file      = out_file
        self.out_folder    = out_folder
        self.SaveSongCover = SaveSongCover
        self.SaveCRC       = SaveCRC
        
        if not os.path.exists(self.in_file):
            exit('>>>> 输入路径不存在！')
        if out_file == out_folder == False:
            exit('>>>> 请添加输出路径！')
    
    @property
    def dump(self):
        inFile_fp = open(self.in_file, 'rb') # 打开文件
        fileHeaders = inFile_fp.read(8) # 读取并移动8字节文件指针
        checkFileType(fileHeaders) # 检查文件头是否是NCM类型
        inFile_fp.seek(2,1) # 移动2字节文件指针
    
        keySize = struct.unpack('<I', inFile_fp.read(4))[0] # 获取密码长度
        ncm_key = inFile_fp.read(keySize)  # 获取密钥
        ncm_key = bytearray([x ^ 0x64 for x in ncm_key]) # 重制密钥
    
        AES_CTX = AES.new(coreKey, AES.MODE_ECB) # 初始化AES对象
        ncm_key = AES_CTX.decrypt(bytes(ncm_key)).strip()[17:] # 解密出密钥
        ncm_key = bytearray(ncm_key) # 转为可变字节对象

        keySize = len(ncm_key) # * 处理一些事情 * #
        key_box = bytearray(range(256))
        byte = lastByte = keyOffset = 0
        for x in range(256):
            if keyOffset >= keySize:
                keyOffset = 0
            swap = key_box[x]
            byte = (swap + lastByte + ncm_key[keyOffset]) & 0xff
            key_box[x] = key_box[byte]
            key_box[byte] = swap
            lastByte = byte
            keyOffset += 1

        metaSize = struct.unpack('<I', inFile_fp.read(4))[0] # 获取meta长度
        meta_data = bytearray([x^0x63 for x in inFile_fp.read(metaSize)]) # 重制meta
        meta_data = base64.b64decode(meta_data[22:]) # 去掉多余部分并解码

        AES_CTX = AES.new(metaKey, AES.MODE_ECB) # 初始化AES对象
        meta_data = bytearray(AES_CTX.decrypt(bytes(meta_data))) # 解密出meta_data
        meta_data = DelFilledData(meta_data)[6:] # 去掉多余部分
        meta_data = json.loads(meta_data) # 转为json对象

        CyclicRedundancyCheck = struct.unpack('<I', inFile_fp.read(4))[0] # 获取CRC校验码
        inFile_fp.seek(5, 1) # 移动5字节文件指针
        image_data = inFile_fp.read(struct.unpack('<I', inFile_fp.read(4))[0]) # 获取歌曲封面

        file_name = inFile_fp.name.split("/")[-1].split(".ncm")[0] + '.' + meta_data['format']
        message_fp = open(os.path.join(os.path.split(self.in_file)[0], file_name), 'wb')

        while True:
            chunk = bytearray(inFile_fp.read(0x8000))
            chunkSize = len(chunk)

            if not chunk:
                break
                
            for x in range(1, chunkSize+1):
                j = x & 0xff
                chunk[x-1] ^= key_box[(key_box[j] + key_box[(key_box[j] + j) & 0xff]) & 0xff]

            message_fp.write(chunk)

        message_fp.close()
        inFile_fp.close()







