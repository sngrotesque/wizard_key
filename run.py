from subprocess import call
from sys import platform, argv
from os import getlogin

def run(cmd :str):
    call(cmd, shell=True)

def run_code(program :str, parameters :list):
    userName = getlogin()

    parameters.append('-lssl')
    parameters.append('-lcrypto')
    parameters.append('-lm')

    if program == 'gcc':
        inPath = 'test/test.c'
    elif program == 'g++':
        inPath = 'test/test.cpp'

    if platform == 'win32':
        parameters.append('-I E:\\Projects\\Shark_Coast\\includes')
        parameters.append('-I E:\\Projects\\Shark_Coast\\source')
        parameters.append('-I E:\\OpenSSL\\include')
        parameters.append('-L E:\\OpenSSL\\lib')
        inPath = inPath.replace('/', '\\')
        outPath = 'out\\main.exe'

    if platform == 'linux':
        if '--server-args' in parameters:
            parameters.append(f'-I /root/code/Shark_Coast/includes')
            parameters.append(f'-I /root/code/Shark_Coast/source')
            parameters.remove('--server-args')
        else:
            parameters.append(f'-I /home/{userName}/code/Projects/Shark_Coast/includes')
            parameters.append(f'-I /home/{userName}/code/Projects/Shark_Coast/source')
        outPath = 'out/main'

    if '--print-args' in parameters:
        parameters.remove("--print-args")
        print(f'{program} {inPath} {" ".join(parameters)} -o {outPath}')

    text = f'{program} {inPath} {" ".join(parameters)} -o {outPath}'
    run(text)
    run(outPath)

if __name__ == '__main__':
    if len(argv) < 2:
        exit(f'python {argv[0]} [parameters]')

    mode = argv[1]
    para = argv[2:]

    if mode == 'c':
        program = 'gcc'
    elif mode == 'cpp':
        program = 'g++'
    else:
        exit(f'python {argv[0]} [c/cpp]')
    
    run_code(program, para)







