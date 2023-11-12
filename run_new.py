import subprocess
import time
import sys
import re
import os

path_join = os.path.join
path_exists = os.path.exists
output_folder = '_compiled'
matchingStatement = r'\.[cpp]+'

RED   = '\x1b[91m'
CYAN  = '\x1b[96m'
RESET = '\x1b[0m'

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

        self.compile_args = ['-O3']
        self.compile_program = None

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

        print(f'程序编译开始，完整指令为：{CYAN}{build_command}{RESET}')
        self.__run_command(build_command)

    def run(self):
        print(f'程序编译完成，程序路径为：{CYAN}{self.output_file_path}{RESET}')
        self.__run_command(self.output_file_path)

def main():
    if len(sys.argv) < 2:
        exit(f'Too few parameters: {RED}{sys.argv[0]}{RESET} {CYAN}[src_file, [parameter]]{RESET}')

    cmp = compile(sys.argv[1], sys.argv[2:])

    cmp.add_parameter('-I .')
    cmp.add_parameter('-I c')
    cmp.add_parameter('-I cpp')
    cmp.add_parameter('-I c/includes')
    cmp.add_parameter('-I c/sources')
    cmp.add_parameter('-I cpp/includes')
    cmp.add_parameter('-I cpp/sources')

    cmp.add_parameter('c/sources/crypto/snc.c')
    cmp.add_parameter('c/sources/wmkc_base64.c')
    cmp.add_parameter('c/sources/wmkc_memory.c')
    cmp.add_parameter('cpp/sources/crypto/snc.cpp')
    cmp.add_parameter('cpp/sources/network/wmkc_net.cpp')
    cmp.add_parameter('cpp/sources/wmkc_exception.cpp')
    cmp.add_parameter('cpp/sources/wmkc_base64.cpp')

    cmp.build()
    cmp.run()

if __name__ == '__main__':
    main()
