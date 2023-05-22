from subprocess import call
from os.path import exists
from os import removedirs
from sys import platform
from os import listdir
from os import remove
from os import mkdir
import re

def action(cmd :str):
    call(cmd, shell=True)

objectTempOutput = 'compiled'
library_name = 'libshark.lib'

if platform == 'win32':
    zlib_name = 'zdll'
else:
    zlib_name = 'z'

compile_temp_path = []

source_file_list = [
    'sources/crypto/snc.c',
    'sources/crypto/snCrypto.c',
    'sources/image/snPng.c',
    'sources/network/snChat.c',
    'sources/network/snNet.c',
    'sources/network/snTransfer.c',
    'sources/snBase64.c',
    'sources/snBinascii.c',
    'sources/snDataString.c',
    'sources/snFile.c',
    'sources/snHash.c',
    'sources/snHexdump.c',
    'sources/snKey.c',
    'sources/snMath.c',
    'sources/snMisc.c',
    'sources/snNum.c',
    'sources/snObject.c',
    'sources/snPadding.c',
    'sources/snRand.c',
    'sources/snSort.c',
    'sources/snTime.c'
]

if not exists(objectTempOutput):
    print(f'{objectTempOutput}目录不存在，将创建。')
    mkdir(objectTempOutput)
if not exists(f'{objectTempOutput}/temp'):
    print(f'{objectTempOutput}/temp目录不存在，将创建。')
    mkdir(f'{objectTempOutput}/temp')

print('开始生成临时对象文件...')
for src in source_file_list:
    tempFilePath = re.findall(
        r'(?:\w+/)?'
        r'\w+/(\w+).c',
        src, re.S)[0]
    tempFilePath = f'{objectTempOutput}/temp/{tempFilePath}.o'
    compile_temp_path.append(tempFilePath)

    if not exists(tempFilePath):
        command = (
            f'gcc -I includes -L includes/openssl -L includes/zlib '
            f'-lssl -lcrypto -l{zlib_name} -lm -lws2_32 '
            f'-static --std=c18 -Ofast -Wno-pragmas -Wall '
            f'-c {src} -o {tempFilePath}'
        )
        # print(f'生成临时对象文件命令：{command}')
        action(command)

print('开始将临时对象文件组合成库文件...')
compile_path = ' '.join(compile_temp_path)
command = f'ar rcs {library_name} {compile_path}'
action(command)

print(f'库文件生成完毕，删除{objectTempOutput}目录与其中的内容。')
for i in compile_temp_path:
    remove(i)
try:
    removedirs(f'{objectTempOutput}')
except:
    pass

'''
gcc test\shark_coast.c -Wall -I includes -L. -L includes/openssl -lshark -lssl -lcrypto \
    -o shark_coast.exe && .\shark_coast.exe
'''
