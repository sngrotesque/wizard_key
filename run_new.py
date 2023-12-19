import subprocess
import time
import sys
import re
import os

path_join = os.path.join
path_exists = os.path.exists
output_folder = '_compiled'
matchingStatement = r'\.[cpp]+'
output_color = True

color_table = {
    'black':   '\x1b[90m',
    'red':     '\x1b[91m',
    'green':   '\x1b[92m',
    'yellow':  '\x1b[93m',
    'blue':    '\x1b[94m',
    'magenta': '\x1b[95m',
    'cyan':    '\x1b[96m',
    'white':   '\x1b[97m'
}

if '--console-color' in sys.argv:
    console_color_param = sys.argv.index('--console-color')
    console_color_use_default = False
    try:
        if sys.argv[console_color_param + 1].lower() not in color_table:
            console_color_use_default = True
    except IndexError:
        console_color_use_default = True

    if console_color_use_default:
        COLOR = color_table['cyan']
    else:
        COLOR = color_table[sys.argv[console_color_param + 1].lower()]
        sys.argv.pop(console_color_param)

    sys.argv.pop(console_color_param)

    RED   = '\x1b[91m'
    RESET = '\x1b[0m'
else:
    RED, COLOR, RESET = ('', '', '')

def create_output_name(input_path :str):
    tmp = re.findall(r'(?:.*[/\\])?(.*)$', input_path, re.S)[0]
    if ('.c' not in tmp) and ('.cpp' not in tmp):
        raise TypeError(f'\'{tmp}\' is not a C or C++ code file.')
    if sys.platform == 'win32':
        return re.sub(matchingStatement, r'.exe', tmp)
    return re.sub(matchingStatement, r'', tmp)

def get_code_file_type(input_path :str):
    tmp = re.findall(rf'(?:.*[/\\])?.*({matchingStatement})$', input_path, re.S)[0]
    if tmp == '.c':
        return 'gcc'
    elif (tmp == '.cpp') or (tmp == '.cc'):
        return 'g++'

class compile:
    def __init__(self, source_file_path :str, user_args :list):
        if not path_exists(source_file_path):
            raise FileExistsError(f'\'{source_file_path}\' does not exists.')

        self.compile_args = [
            # '-O3'
        ]
        self.compile_program = None
        self.compile_err_code = 0

        self.source_file_path = source_file_path
        self.output_file_path = None
        self.user_args = user_args

    def __run_command(self, cmd :str):
        return subprocess.call(cmd, shell=True)

    # 合并GCC参数
    def merge_parameter(self):
        return ' '.join(self.compile_args)

    # 添加GCC参数
    def add_parameter(self, args :str):
        self.compile_args.append(args)

    def build(self):
        if not path_exists(output_folder):
            os.mkdir(output_folder)

        for arg in self.user_args:
            self.add_parameter(arg)
        args = self.merge_parameter()

        self.output_file_path = path_join(output_folder, create_output_name(self.source_file_path))
        self.compile_program = get_code_file_type(self.source_file_path)
        build_command = f'{self.compile_program} {self.source_file_path} {args} -o {self.output_file_path}'

        print(f'程序编译开始，完整指令为：{COLOR}{build_command}{RESET}')
        self.compile_err_code = self.__run_command(build_command)

    def run(self):
        if self.compile_err_code:
            exit(f'编译程序时出现了错误，返回的错误代码为：{self.compile_err_code}')
        print(f'程序编译完成，程序路径为：{COLOR}{self.output_file_path}{RESET}')
        self.__run_command(self.output_file_path)

def main():
    if len(sys.argv) < 2:
        exit(f'Too few parameters: {RED}{sys.argv[0]}{RESET} {COLOR}[src_file, [parameter]]{RESET}')

    cmp = compile(sys.argv[1], sys.argv[2:])

    # cmp.add_parameter('-I .')
    # cmp.add_parameter('-I c')
    # cmp.add_parameter('-I cpp')
    # cmp.add_parameter('-I c/includes')
    # cmp.add_parameter('-I c/sources')
    cmp.add_parameter('-I cpp/includes')
    cmp.add_parameter('-I cpp/sources')

    # cmp.add_parameter('library/qrcode/qrcode.cpp')

    # cmp.add_parameter('cpp/sources/network/wmkc_net_exception.cpp')
    # cmp.add_parameter('cpp/sources/network/wmkc_ssl.cpp')
    # cmp.add_parameter('cpp/sources/network/wmkc_net.cpp')
    # cmp.add_parameter('cpp/sources/network/wmkc_dns.cpp')

    # cmp.add_parameter('cpp/sources/crypto/crypto.cpp')
    # cmp.add_parameter('cpp/sources/crypto/snc.cpp')
    # cmp.add_parameter('cpp/sources/crypto/fea.cpp')

    cmp.add_parameter('cpp/sources/wmkc_exception.cpp')
    cmp.add_parameter('cpp/sources/wmkc_memory.cpp')
    # cmp.add_parameter('cpp/sources/wmkc_random.cpp')
    cmp.add_parameter('cpp/sources/wmkc_struct.cpp')
    # cmp.add_parameter('cpp/sources/wmkc_basic.cpp')
    cmp.add_parameter('cpp/sources/wmkc_misc.cpp')
    # cmp.add_parameter('cpp/sources/wmkc_time.cpp')

    # cmp.add_parameter('c/sources/crypto/snc.c')
    # cmp.add_parameter('c/sources/wmkc_random.c')
    # cmp.add_parameter('c/sources/wmkc_basic.c')
    # cmp.add_parameter('c/sources/wmkc_memory.c')
    # cmp.add_parameter('c/sources/wmkc_misc.c')

    cmp.build()
    cmp.run()

if __name__ == '__main__':
    # python run_new.py test\main.cpp -O3 -lws2_32 -lssl -lcrypto -lcjson -Wall -Wno-sign-compare
    main()
