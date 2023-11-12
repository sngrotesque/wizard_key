import subprocess
import time
import os

path_join = os.path.join

def run(cmd :str):
    subprocess.call(cmd, shell=True)

inFile = path_join('test', 'main.cpp')
outFile = path_join('compiled', 'main.exe')
args = ' '.join([
    '-I .',
    '-I c',
    '-I c/includes',
    '-I cpp',
    '-I cpp/includes/',
    '-O3'
])
sources_path = ' '.join([
    'c/sources/crypto/snc.c',
    'c/sources/wmkc_memory.c',
    'c/sources/wmkc_misc.c',
    'c/sources/wmkc_base64.c',
    'cpp/sources/wmkc_exception.cpp',
    'cpp/sources/crypto/snc.cpp',
    'cpp/sources/wmkc_base64.cpp'
])

compile_command = f'g++ {inFile} {sources_path} {args} -o {outFile}'
run(compile_command)
run(outFile)
