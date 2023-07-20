import sys, os, re
import subprocess

class action:
    def __init__(self, inPath :str, param :list, program :str, printArgs :bool):
        self.in_path   = inPath
        self.out_path  = None
        self.param     = param
        self.program   = program
        self.printArgs = printArgs

    def call(self, command :str):
        status = subprocess.call(command, shell = True)
        if status:
            exit(f'错误的执行状态码：{status}')

    def init(self):
        if sys.platform == 'win32':
            os.environ['PATH'] = os.environ['PATH'] + ';' + os.path.join(
                'E:', 'Projects', '_Library_WMKC', 'includes', 'libpng')

        self.out_path = list(re.findall(r'(\w+)[/\\]+(\w+\.\w+)', self.in_path, re.S | re.I)[0])
        self.out_path[0] = os.path.join('misc', 'compiled')
        self.out_path[1] = re.sub(r'.c', r'.exe', self.out_path[1]) \
            if sys.platform=='win32' else re.sub(r'.c', r'', self.out_path[1])
        self.out_path = os.path.join(self.out_path[0], self.out_path[1])

    def run_code(self):
        self.param.append('-I includes')
        self.param.append('-I sources')
        self.param.append('-I includes/openssl/include/')

        self.param.append('-L includes/zlib')    # Referer: Zlib
        self.param.append('-L includes/openssl/') # Referer: OpenSSL
        self.param.append('-L includes/libpng')  # Referer: Libpng
        self.param.append('-L includes/cjson')   # Referer: cJSON
        self.param.append('-L includes/iconv')   # Referer: Iconv
        self.param.append('-L includes/libjpeg') # Referer: Libjpeg

        self.param.append('-lssl')    # OpenSSL
        self.param.append('-lcrypto') # OpenSSL
        self.param.append('-lz')      # Zlib
        self.param.append('-lm')      # Math
        self.param.append('-lpng16')  # Libpng
        self.param.append('-ljpeg')   # Libjpeg
        self.param.append('-lcjson')  # cJSON
        self.param.append('-liconv')  # Iconv

        if self.printArgs:
            print(f'{self.program} {self.in_path} {" ".join(self.param)} -o {self.out_path}')

        self.param = ' '.join(self.param)

        self.call(f'{self.program} {self.in_path} {self.param} -o {self.out_path}') # Compile Code
        self.call(self.out_path) # Run Program

if __name__ == '__main__':
    if len(sys.argv) < 3:
        exit(f'python {sys.argv[0]} [path] [c/cpp] [parameters]')

    path = sys.argv[1]
    para = sys.argv[3:]
    if sys.argv[2] == 'c':
        program = 'gcc'
    elif sys.argv[2] == 'cpp':
        program = 'g++'
    else:
        exit(f'python {sys.argv[0]} [path] [c/cpp]')

    printArgs = True if '--print-args' in para else False
    if '--print-args' in para:
        para.remove('--print-args')

    ctx = action(inPath = path, param = para, program = program, printArgs = printArgs)
    ctx.init()
    ctx.run_code()
