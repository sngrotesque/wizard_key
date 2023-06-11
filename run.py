from os.path import exists
from os import mkdir
from subprocess import call
from sys import platform, argv

OUT_PATH = "snCompiled"
if platform == 'win32':
    PATH_SYMBOL = '\\'
    PROGRAM_NAME = 'main.exe'
else:
    PATH_SYMBOL = '/'
    PROGRAM_NAME = 'main'

def run(cmd :str):
    call(cmd, shell=True)

def run_code(program :str, parameters :list):
    parameters.append('-I includes')
    parameters.append('-I sources')

    parameters.append('-lssl')    # OpenSSL
    parameters.append('-lcrypto') # OpenSSL
    parameters.append('-lz')      # Zlib
    parameters.append('-lm')      # Math
    parameters.append('-lpng16')  # Libpng
    parameters.append('-lcjson')  # cJSON

    parameters.append('-L includes/zlib')    # Referer: Zlib
    parameters.append('-L includes/openssl') # Referer: OpenSSL
    parameters.append('-L includes/libpng')  # Referer: Libpng
    parameters.append('-L includes/cjson')   # Referer: cJSON

    # ICU4C
    parameters.append('-I includes/unicode_winos/include')
    parameters.append('-L includes/unicode_winos/lib64')
    parameters.append('-licuio')
    parameters.append('-licuuc')

    if program == 'gcc':
        # C Programming Language
        inPath = f'test{PATH_SYMBOL}test.c'
        # inPath = f'test{PATH_SYMBOL}WizChat.c'
    elif program == 'g++':
        # C++ Programming Language
        inPath = f'test{PATH_SYMBOL}test.cpp'

    outPath = f'{OUT_PATH}{PATH_SYMBOL}{PROGRAM_NAME}' # Output path

    # View overall command parameters
    if '--print-args' in parameters:
        parameters.remove("--print-args")
        print(f'{program} {inPath} {" ".join(parameters)} -o {outPath}')

    text = f'{program} {inPath} {" ".join(parameters)} -o {outPath}'
    run(text)     # Compile Code
    run(outPath)  # Run Program

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
    
    if not exists(OUT_PATH):
        mkdir(OUT_PATH)
    
    run_code(program, para)
