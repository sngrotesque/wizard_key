import subprocess
import logging
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

    output_fn = re.sub(match_statement, r'.exe', full_filename) \
        if sys.platform == 'win32' else re.sub(match_statement, r'', full_filename)

    return output_fn, 'gcc' if extensions_filename == 'c' else 'g++'

class compile:
    def __init__(self, source_file_path :str, user_args :list):
        if not os.path.exists(source_file_path):
            raise FileExistsError(f'\'{source_file_path}\' does not exists.')

        self.compile_args = []

        self.source_path = source_file_path
        self.output_path = None
        self.user_args = user_args

    def __run_command(self, cmd :str):
        # return subprocess.call(cmd, shell=True)
        try:
            result = subprocess.run(cmd, shell = True, check = True)
            return result.returncode
        except subprocess.CalledProcessError as e:
            logging.error(f'Command \'{cmd}\' failed with return code {e.returncode}')
            sys.exit(e.returncode)

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

        logging.info(f'程序编译开始，完整指令为：{COLOR}{build_command}{RESET}')
        compile_err_code = self.__run_command(build_command)
        if compile_err_code:
            sys.exit(f'编译程序时出现了错误，返回的错误代码为：{compile_err_code}')

    def run(self):
        logging.info(f'程序编译完成，程序路径为：{COLOR}{self.output_path}{RESET}')
        self.__run_command(self.output_path)

def main():
    logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

    if len(sys.argv) < 2:
        sys.exit(f'Too few parameters: {COLOR}{sys.argv[0]}{RESET} {COLOR}[src_file, [parameter]]{RESET}')

    comp = compile(sys.argv[1], sys.argv[2:])

    compile_params = (
        '-I wuk/includes',
        '-I wuk/sources',

        # 编译选项
        '-O3',                # 优化级别：3（最高级别）
        '-Wsuggest-override', # 建议使用override关键词来确保派生类中的虚函数覆盖基类的虚函数。
        # '-Wnon-virtual-dtor', # 警告在有多态基类的情况下，如果析构函数不是虚函数。
        '-Wredundant-decls',  # 警告冗余声明。
        # '-Wold-style-cast',   # 警告使用C风格的强制类型转换。
        # '-Wuseless-cast',     # 警告无效或无用的强制类型转换。
        # '-Wfloat-equal',      # 警告比较浮点数是否相等。
        '-Wcast-align',       # 警告由于类型转换而导致的潜在内存对齐问题。
        '-Wpedantic',         # 启用对C++标准严格语法和语义的警告，对一些非标准的扩展和行为给出警告。
        # '-Wshadow',           # 警告变量被隐藏（如在内层作用域中定义了同名变量）。
        '-Wformat',           # 警告与printf和scanf格式字符串相关的问题。
        # '-Wundef',            # 警告未定义的宏。
        '-Wextra',            # 用于-Wall未显示的额外的警告，帮助发现代码中可能存在的问题。
        '-Werror',            # 将所有警告视为错误，这样可以确保你的代码在没有警告的情况下编译。
        '-Wall',              # 显示绝大多数错误和警告。
        
        # 暂时性编译选项
        '-Wno-unused-variable',  # 忽略未使用错误
        '-Wno-unused-parameter', # 忽略未使用参数
    )

    for param in compile_params:
        comp.add_parameter(param)

    start = time.time()
    comp.build()
    stop = time.time()

    logging.info(f'构建所用时间：{stop-start:.2f}秒。')

    comp.run()

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
