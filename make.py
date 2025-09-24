import os
import re
import sys
import time
import logging
import subprocess

# === 配置区 ===
# OUTPUT_DIR = '$HOME/exc'
OUTPUT_DIR = '/data/data/com.termux/files/home'
C_CPP_EXTENSIONS = ['c', 'cc', 'cpp', 'cxx']
COLOR_TABLE = {
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

# === 工具函数 ===
def extract_output_filename(path: str) -> tuple[str, str]:
    if not os.path.exists(path):
        raise FileNotFoundError(f"文件不存在：{path}")

    filename = os.path.basename(path)
    ext = filename.split('.')[-1]

    if ext not in C_CPP_EXTENSIONS:
        raise ValueError(f"{path} 不是 C/C++ 源文件")

    output_name = re.sub(rf'\.({"|".join(C_CPP_EXTENSIONS)})$', '.exe' if sys.platform == 'win32' else '', filename)
    compiler = 'gcc' if ext == 'c' else 'g++'
    return output_name, compiler


def run_command(cmd: str, verbose: bool = True) -> int:
    try:
        return subprocess.run(cmd, shell=True, check=True).returncode
    except subprocess.CalledProcessError as e:
        if verbose:
            logging.error(f"命令执行失败：{cmd}（返回码 {e.returncode}）")
        sys.exit(e.returncode)


def process_color_arg(arg: str) -> str:
    if arg not in sys.argv:
        return ''
    index = sys.argv.index(arg)
    try:
        color = sys.argv[index + 1].lower()
        color_code = COLOR_TABLE.get(color, COLOR_TABLE['cyan'])
        sys.argv.pop(index + 1)
    except IndexError:
        color_code = COLOR_TABLE['cyan']
    sys.argv.pop(index)
    return color_code

# === 编译器类 ===
class Compiler:
    def __init__(self, source: str, user_args: list[str], verbose: bool = True):
        if not os.path.exists(source):
            raise FileNotFoundError(f"源文件不存在：{source}")
        self.source = source
        self.user_args = user_args
        self.verbose = verbose
        self.output_path = None
        self.args = []

    def add_args(self, *args: str):
        self.args.extend(args)

    def build(self):
        os.makedirs(OUTPUT_DIR, exist_ok=True)

        output_name, compiler = extract_output_filename(self.source)
        self.output_path = os.path.join(OUTPUT_DIR, output_name)

        cmd = f"{compiler} {self.source} {' '.join(self.args + self.user_args)} -o {self.output_path}"
        if self.verbose:
            logging.info(f"编译命令：{COLOR}{cmd}{RESET}")
        run_command(cmd, self.verbose)

    def run(self):
        if self.verbose:
            logging.info(f"运行程序：{COLOR}{self.output_path}{RESET}")
        run_command(self.output_path, self.verbose)

# === 主函数 ===
def main(verbose: bool = True):
    logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

    if len(sys.argv) < 2:
        sys.exit(f"参数不足：{COLOR}{sys.argv[0]} [src_file] [options]{RESET}")

    compiler = Compiler(sys.argv[1], sys.argv[2:], verbose)

    # 源文件
    compiler.add_args(
        # 'wuk/sources/crypto/WukOP4.cc',
        # 'wuk/sources/crypto/WukOP4_sse4.cc',
        # 'wuk/sources/crypto/WukChaCha20.cc',
        'wuk/sources/im/WukIM.cc',
        'wuk/sources/db/WukPsql.cc',
        # 'wuk/sources/net/WukPacket.pb.cc',
        # 'wuk/sources/net/WukPacket.cc',
        # 'wuk/sources/net/WukSocket.cc',
        # 'wuk/sources/net/WukSocketEx.cc',
        'wuk/sources/utils/bytes.cc',
        # 'wuk/sources/WukBase64.cc',
        # 'wuk/sources/WukBinascii.cc',
        'wuk/sources/WukBuffer.cc',
        # 'wuk/sources/WukHexdump.cc',
        'wuk/sources/WukMisc.cc',
        # 'wuk/sources/WukPadding.cc',
    )

    # 编译参数
    compiler.add_args(
        '-I wuk/includes',
        # '-I wuk/sources',
        '-O2',
        # '-Wcast-align',
        # '-Wpedantic',
        # '-Wformat',
        # '-Wextra',
        '-Werror',
        '-Wall',

        # '-Wno-unused-variable',
        '-Wno-unused-function',
        # '-Wno-unused-parameter',
        # '-Wno-unused-but-set-variable',

        '--std=c++17',
        '-DWUK_EXPORTS',
        '-march=native',
    )

    # 库链接
    compiler.add_args(
        '-lfmt',
    )

    start = time.time()
    compiler.build()
    logging.info(f"编译耗时：{time.time() - start:.2f} 秒")
    compiler.run()

# === 入口点 ===
if __name__ == '__main__':
    COLOR = process_color_arg('--color')
    RESET = COLOR_TABLE['reset']
    main()
