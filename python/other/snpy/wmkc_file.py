from .wmkc_crypto import wmkcCrypto_hashlib
from os import walk, remove, stat
from os.path import exists, join
from typing import Union
import hashlib

def wmkcFile_fread(path :str, mode :str = 'rb', encoding = None) -> Union[str, bytes]:
    with open(path, mode, encoding = encoding) as f:
        return f.read()

def wmkcFile_fwrite(path :str, content :bytes, mode :str = 'wb', encoding = None):
    with open(path, mode, encoding = encoding) as f:
        return f.write(content)

def wmkcFile_deleteDuplicate(target_path :str):
    pathList = []
    print('正在统计整体文件信息...')
    for dirpath, _, filenames in walk(target_path):
        for filename in filenames:
            _fn = join(dirpath, filename)
            pathList.append([_fn, wmkcCrypto_hashlib(hashlib.sha256(), path = _fn), stat(_fn).st_size])

    for x in range(len(pathList)):
        print(f'第{x+1}个循环。')
        for y in range(x + 1, len(pathList)):
            if (not exists(pathList[x][0])) or (not exists(pathList[y][0])):
                continue
            if pathList[x][2] == pathList[y][2]:
                if pathList[x][1] == pathList[y][1]:
                    print(f'删除 -> {pathList[x][0]}')
                    remove(pathList[x][0])

