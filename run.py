from os import mkdir, system, environ
from sys import platform, argv
from subprocess import call
from os.path import exists, join
import re

# class run_program:
#     def __init__(self, inPath: str, param :list = None):
#         self.inPath = inPath
#         self.outPath = None

#         self.param = param

if platform == 'win32':
    environ['PATH'] = environ['PATH'] + ';' + join(
        'E:', 'Projects', '_Library_WMKC', 'includes', 'libpng')

def run(cmd :str):
    call(cmd, shell=True)

def run_code(inPath :str, program :str, parameters :list, outPath :str):
    parameters.append('-I includes')
    parameters.append('-I sources')
    parameters.append('-I includes/openssl/include/')

    parameters.append('-L includes/zlib')    # Referer: Zlib
    parameters.append('-L includes/openssl/') # Referer: OpenSSL
    parameters.append('-L includes/libpng')  # Referer: Libpng
    parameters.append('-L includes/cjson')   # Referer: cJSON
    parameters.append('-L includes/iconv')   # Referer: Iconv
    parameters.append('-L includes/libjpeg') # Referer: Libjpeg

    parameters.append('-lssl')    # OpenSSL
    parameters.append('-lcrypto') # OpenSSL
    parameters.append('-lz')      # Zlib
    parameters.append('-lm')      # Math
    parameters.append('-lpng16')  # Libpng
    parameters.append('-ljpeg')   # Libjpeg
    parameters.append('-lcjson')  # cJSON
    parameters.append('-liconv')  # Iconv

    # View overall command parameters
    if '--print-args' in parameters:
        parameters.remove("--print-args")
        print(f'{program} {inPath} {" ".join(parameters)} -o {outPath}')

    parameters = ' '.join(parameters)

    text = f'{program} {inPath} {parameters} -o {outPath}'
    run(text)     # Compile Code
    run(outPath)  # Run Program

if __name__ == '__main__':
    if len(argv) < 3:
        exit(f'python {argv[0]} [path] [c/cpp] [parameters]')

    path = argv[1]
    mode = argv[2]
    para = argv[3:]

    if    mode == 'c':   program = 'gcc'
    elif  mode == 'cpp': program = 'g++'
    else: exit(f'python {argv[0]} [path] [c/cpp]')
    
    outPath = list(re.findall(r'(\w+)[/\\]+(\w+\.\w+)', path, re.S | re.I)[0])
    outPath[0] = join('misc', 'compiled')
    outPath[1] = re.sub(r'.c', r'.exe', outPath[1]) if platform=='win32' else re.sub(r'.c', r'', outPath[1])
    outPath = join(outPath[0], outPath[1])
    
    run_code(path, program, para, outPath)
