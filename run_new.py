import subprocess
import time
import sys
import re
import os

path_join = os.path.join
path_exists = os.path.exists
output_folder = '_compiled'

RED = '\x1b[91m'
CYAN = '\x1b[96m'
RESET = '\x1b[0m'

def create_output_name(input_path :str):
    tmp = re.findall(r'(?:.*[/\\])?(.*)$', input_path, re.S)[0]
    if ('.c' not in tmp) or ('.cpp' not in tmp):
        raise TypeError(f'\'{tmp}\' is not a C or C++ code file.')
    if sys.platform == 'win32':
        return re.sub(r'\.[cpp|c]+', r'.exe', tmp)
    return re.sub(r'\.[cpp|c]', r'', tmp)

class compile:
    def __init__(self, source_file_path :str):
        if not path_exists(source_file_path):
            raise FileExistsError(f'\'{source_file_path}\' does not exists.')

        self.compile_args = ['-O3']
        self.source_file_path = source_file_path

    def __run_command(self, cmd :str):
        return subprocess.call(cmd, shell=True)

    # 合并GCC参数
    def merge_parameter(self):
        return ' '.join(self.compile_args)

    # 添加GCC参数
    def add_parameter(self, args :str):
        self.compile_args.append(args)

def main():
    if len(sys.argv) < 2:
        exit(f'Too few parameters: {RED}{sys.argv[0]}{RESET} {CYAN}[src_file, [parameter]]{RESET}')

    source_file_path = sys.argv[1]
    source_args      = sys.argv[2:]
    output_file_path = create_output_name(source_file_path)

    print(f'source_file_path: {source_file_path}')
    print(f'output_file_path: {output_file_path}')
    print(f'source_args: {source_args}')

    # cmp = compile(source_file_path)
    # cmp.add_parameter('-I c')

if __name__ == '__main__':
    main()
