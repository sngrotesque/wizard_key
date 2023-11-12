import subprocess
import time
import os

path_join = os.path.join
path_exists = os.path.exists
output_folder = '_compiled'

def run(cmd :str):
    subprocess.call(cmd, shell=True)

def main():
    inFile = path_join('test', 'main.cpp')
    outFile = path_join(output_folder, 'main.exe')
    args = ' '.join([
        '-I .',
        '-I c',
        '-I c/includes',
        '-I cpp',
        '-I cpp/includes/',
        '-lws2_32',
        '-O3'
    ])
    sources_path = ' '.join([
        'c/sources/crypto/snc.c',
        'c/sources/wmkc_memory.c',
        'c/sources/wmkc_misc.c',
        'c/sources/wmkc_base64.c',
        'cpp/sources/wmkc_exception.cpp',
        'cpp/sources/crypto/snc.cpp',
        'cpp/sources/wmkc_base64.cpp',
        'cpp/sources/network/wmkc_net.cpp'
    ])

    compile_command = f'g++ {inFile} {sources_path} {args} -o {outFile}'
    run(compile_command)
    run(outFile)

if __name__ == '__main__':
    if not path_exists(output_folder):
        os.mkdir(output_folder)
    main()
