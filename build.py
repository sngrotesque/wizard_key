'''
！！！注意！！！此构建脚本只适用于 GNUC / MingW64环境 。

仅用作展示如何将此库构建为一个可用的动态库，后续此库将基于CMake实现构建系统。
'''
import subprocess
import shutil
import sys
import os

name = 'wuk'
builds_dir = 'build'
library_dir = 'lib'

# 编译选项
compiler_args = ' '.join([
    '-I wuk/includes',
    '-O2',

    '--std=c++17',
    '-DWUK_EXPORTS',
    '-march=native',

    '-Wcast-align',
    '-Wpedantic',
    '-Wformat',
    '-Wextra',
    '-Werror',
    '-Wall',

    '-Wno-unused-variable',
    '-Wno-unused-function',
    '-Wno-unused-parameter',
    '-Wno-unused-but-set-variable',
])

# 外部库依赖项
libs = ' '.join([
    '-lssl',
    '-lcrypto',
    '-lprotobuf',
    '-lws2_32',
    '-lpq',
    '-lz',
    '-lbcrypt',
])

sources = [
    'wuk/sources/crypto/WukChaCha20.cc',
    'wuk/sources/crypto/WukOP4_sse4.cc',
    'wuk/sources/im/WukIM.cc',
    'wuk/sources/im/WukPsql.cc',
    'wuk/sources/net/WukPacket.cc',
    'wuk/sources/net/WukPacket.pb.cc',
    'wuk/sources/net/WukSocket.cc',
    'wuk/sources/net/WukSocketEx.cc',
    'wuk/sources/utils/bytes.cc',
    'wuk/sources/WukBase64.cc',
    'wuk/sources/WukBinascii.cc',
    'wuk/sources/WukBuffer.cc',
    'wuk/sources/WukHexdump.cc',
    'wuk/sources/WukMisc.cc',
    'wuk/sources/WukPadding.cc',
    'wuk/sources/WukRandom.cc'
]

if not os.path.exists(builds_dir):
    os.mkdir(builds_dir)

if not os.path.exists(library_dir):
    os.mkdir(library_dir)

objects = [
    os.path.join(builds_dir, os.path.basename(item).replace('.cc', '.o'))
    for item in sources
]

for i in range(len(sources)):
    subprocess.call(f'g++ -c -fPIC {compiler_args} {sources[i]} -o {objects[i]}', shell=True)

lib_path = os.path.join(library_dir, 'lib' + name + ('.dll' if sys.platform == 'win32' else '.so'))
objects = ' '.join(objects)

subprocess.call(f'g++ -shared {objects} {libs} -o {lib_path}', shell=True)

shutil.rmtree(builds_dir)
