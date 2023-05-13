from subprocess import call
from sys import platform, argv

OUT_PATH = "#output"
if platform == 'win32':
    PATH_SYMBOL = '\\'
    PROGRAM_NAME = 'main.exe'
else:
    PATH_SYMBOL = '/'
    PROGRAM_NAME = 'main'

def run(cmd :str):
    call(cmd, shell=True)

def run_code(program :str, parameters :list):
    parameters.append('-lssl')
    parameters.append('-lcrypto')
    parameters.append('-lzdll')
    parameters.append('-lm')

    parameters.append('-I includes')
    parameters.append('-I source')

    parameters.append(f'-L includes{PATH_SYMBOL}openssl')
    parameters.append(f'-L includes{PATH_SYMBOL}zlib')

    if program == 'gcc':
        inPath = f'test{PATH_SYMBOL}test.c'
    elif program == 'g++':
        inPath = f'test{PATH_SYMBOL}test.cpp'

    outPath = f'{OUT_PATH}{PATH_SYMBOL}{PROGRAM_NAME}'

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







