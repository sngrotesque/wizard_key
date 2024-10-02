import subprocess
import time
import sys
import re
import os

output_folder  = '_compiled'
c_cpp_filename = ['c', 'cc', 'cpp', 'cxx']

def create_output_filename(in_path :str):
    if not os.path.exists(in_path):
        raise RuntimeError('传入了不存在的文件路径')

    full_filename = re.findall(r'.*[\\/]+([\w\d]+\.\w+)$', in_path, re.S)[0]

    extensions_filename = re.findall(r'.*\.(\w+)', full_filename, re.S)[0]
    if extensions_filename not in c_cpp_filename:
        raise RuntimeError(f'{in_path} is not a C or C++ code file.')

    match_statement = rf'\.[{"|".join(c_cpp_filename)}]+'

    output_fn = None
    if sys.platform == 'win32':
        output_fn = re.sub(match_statement, r'.exe', full_filename)
    else:
        output_fn = re.sub(match_statement, r'', full_filename)

    if extensions_filename == 'c':
        return output_fn, 'gcc'
    return output_fn, 'g++'

class compile:
    def __init__(self, source_file_path :str, user_args :list):
        if not os.path.exists(source_file_path):
            raise FileExistsError(f'\'{source_file_path}\' does not exists.')

        self.compile_args = []

        self.source_path = source_file_path
        self.output_path = None
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
        if not os.path.exists(output_folder):
            os.mkdir(output_folder)

        for arg in self.user_args:
            self.add_parameter(arg)
        args = self.merge_parameter()

        output_filename, compile_program = create_output_filename(self.source_path)

        self.output_path = os.path.join(output_folder, output_filename)
        build_command = f'{compile_program} {self.source_path} {args} -o {self.output_path}'

        print(f'程序编译开始，完整指令为：{COLOR}{build_command}{RESET}')
        compile_err_code = self.__run_command(build_command)
        if compile_err_code:
            exit(f'编译程序时出现了错误，返回的错误代码为：{compile_err_code}')

    def run(self):
        print(f'程序编译完成，程序路径为：{COLOR}{self.output_path}{RESET}')
        self.__run_command(self.output_path)

def main():
    if len(sys.argv) < 2:
        exit(f'Too few parameters: {COLOR}{sys.argv[0]}{RESET} {COLOR}[src_file, [parameter]]{RESET}')

    cmp = compile(sys.argv[1], sys.argv[2:])

    cmp.add_parameter('-I wuk/includes')
    cmp.add_parameter('-I wuk/sources')

    # 通用包含代码
    cmp.add_parameter('./wuk/sources/config/WukException.cc')
    cmp.add_parameter('./wuk/sources/WukMemory.cc')

    start = time.time()
    cmp.build()
    stop = time.time()

    print(f'构建所用时间：{stop-start:.2f}秒。')

    cmp.run()

if __name__ == '__main__':
    color_table = {
        'black':   '\x1b[90m',
        'red':     '\x1b[91m',
        'green':   '\x1b[92m',
        'yellow':  '\x1b[93m',
        'blue':    '\x1b[94m',
        'magenta': '\x1b[95m',
        'cyan':    '\x1b[96m',
        'white':   '\x1b[97m',
        'reset':   '\x1b[0m'
    }

    COLOR, RESET = '', color_table['reset']

    if '--color' in sys.argv:
        console_color_param = sys.argv.index('--color')
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

    main()
