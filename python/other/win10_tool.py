from typing import Union
import subprocess
import hashlib
import logging
import zipfile
import random
import shutil
import socket
import stat
import sys
import os
import re

def runPopen(cmd :str):
    p = subprocess.Popen(cmd, shell = True, stdout = subprocess.PIPE)
    return p.stdout.read().decode()

def fread(path :str, mode :str, encoding :str = None):
    with open(path, mode, encoding = encoding) as f:
        return f.read()

def fwrite(path :str, content :Union[str, bytes], mode :str, encoding :str = None):
    with open(path, mode, encoding = encoding) as f:
        return f.write(content)

def sha256(path :str):
    if os.stat(path).st_size < ((2 ** 32) >> 1):
        with open(path, 'rb') as f:
            return hashlib.sha256(f.read()).hexdigest()
    return runPopen(f'sha256sum {path}')[:64]

def compareHash(path1, path2, showHash :bool = False):
    if showHash:
        hash1 = sha256(path1)
        hash2 = sha256(path2)
        print(f'\'{path1}\': \'{hash1}\'')
        print(f'\'{path2}\': \'{hash2}\'')
        return hash1 == hash2
    return sha256(path1) == sha256(path2)

def recordDiffByteIndices(path1, path2):
    with open(path1, 'rb') as f:
        data1 = f.read()
    with open(path2, 'rb') as f:
        data2 = f.read()
    if len(data1) != len(data2):
        print('数据长度不同，没必要比较')
        return []
    index_list = []
    for i in range(len(data1)):
        if data1[i] != data2[i]:
            index_list.append(i)
    return index_list

class copyDirectories:
    def __init__(self, srcPath :str, dstPath :str):
        self.srcPath = srcPath.strip('/')
        self.dstPath = dstPath.strip('/')
        
        self.IS_FILE  = 2
        self.IS_DIR   = 1
        self.IS_OTHER = 0

    def fileCopy(self, srcPath :str, dstPath :str):
        return shutil.copy(srcPath, dstPath)

    def what_is_path_type(self, path :str):
        mode = os.stat(path).st_mode
        if stat.S_ISDIR(mode):
            return self.IS_DIR
        elif stat.S_ISREG(mode):
            return self.IS_FILE
        else:
            return self.IS_OTHER

    def use_shutil(self, start_index :int = 0, log_file :str = 'shutil_copy.log'):
        pathList = os.listdir(self.srcPath)
        pathListLen = len(pathList)

        for i in range(start_index, pathListLen):
            src_path_preview = f'{self.srcPath}/{pathList[i]}'
            dst_path_preview = f'{self.dstPath}/{pathList[i]}'
            path_type = self.what_is_path_type(src_path_preview)

            print(f'[{i:>5d}/{pathListLen}]-[{path_type}]: {src_path_preview[:48]:<48s} {dst_path_preview[:64]}')

            try:
                if path_type == self.IS_DIR:
                    shutil.copytree(src_path_preview, dst_path_preview)
                elif path_type == self.IS_FILE:
                    shutil.copy(src_path_preview, dst_path_preview)
                else:
                    message = f'Strange file type: \'{src_path_preview}\''
                    print(message)
                    fwrite(log_file, f'{message}\n', 'a+')
            except Exception as message:
                print(message)
                fwrite(log_file, f'{message}\n', 'a+')

ctx = copyDirectories('G:/Windows/System32', 'D:/Windows/System32')
ctx.use_shutil(start_index=1181)
