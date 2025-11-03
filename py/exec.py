from pathlib import Path
import subprocess
import logging
import time
import sys
import os

# === 配置区 ===
OUTPUT_DIR = '_compiled'
# OUTPUT_DIR = '/data/data/com.termux/files/home'

# === 工具函数 ===
def pack_path(path :str):
    return os.path.join(*Path(path).parts)

def extract_output_filename(path: str) -> tuple[str, str]:
    filename = os.path.basename(path).split('.')[0]
    return f'{filename}.exe' if sys.platform == 'win32' else filename

# === 编译器类 ===
class Compiler:
    def __init__(self, user_args: list[str], verbose: bool = True):
        self.user_args = user_args
        self.verbose   = verbose

        self.preset_args = []

    def command(self, cmd: str) -> int:
        try:
            return subprocess.run(cmd, shell = True, check = True).returncode
        except subprocess.CalledProcessError as e:
            if self.verbose:
                logging.error(f"命令执行失败：{cmd}（返回码 {e.returncode}）")
            sys.exit(e.returncode)

    def add_args(self, *args: str):
        self.preset_args.extend(args)

    def build(self, src_path :str, dst_path :str = None, exec_program :bool = True):
        src_path = pack_path(src_path) # 重新打包源路径

        # 检查源路径是否存在
        if not os.path.exists(src_path):
            raise FileNotFoundError(f"源文件不存在：{src_path}")

        # 逐级创建指定目录
        os.makedirs(OUTPUT_DIR, exist_ok = True)

        if not dst_path:
            # 如果用户未指定输出路径，将以默认方式得到输出路径
            output_filename = extract_output_filename(src_path)
            dst_path = os.path.join(OUTPUT_DIR, output_filename)

        # 预设参数与用户参数结合
        args_str = ' '.join(self.preset_args + self.user_args)
        cmd = f"g++ {src_path} {args_str} -o {dst_path}"

        # 编译程序
        start = time.time()
        if self.verbose:
            logging.info(f"编译命令：{cmd}")
        self.command(cmd)
        stop = time.time()
        if self.verbose:
            logging.info(f"编译耗时：{stop-start:.2f} s。")

        # 执行程序
        if exec_program:
            if self.verbose:
                logging.info(f"运行程序：{dst_path}")
            self.command(dst_path)

# === 主函数 ===
def main(verbose: bool = True):
    logging.basicConfig(level = logging.INFO, format = '%(asctime)s - %(levelname)s - %(message)s')

    if len(sys.argv) < 2:
        logging.error(f"参数不足：{sys.argv[0]} [src_file] [options]")
        sys.exit(1)

    source_file = sys.argv[1]
    user_args = sys.argv[2:]

    compiler = Compiler(user_args, verbose)

    # im源文件
    compiler.add_args(
        # 'im/server/sources/WukAccountService.cc',
        # 'im/server/sources/WukDispatcher.cc',
        # 'im/server/sources/WukMessageService.cc',
        # 'im/server/sources/WukServer.cc',
        # 'im/server/sources/WukSession.cc',
        # 'im/server/sources/WukUtils.cc',

        # 'im/client/sources/WukAccountClient.cc',
        # 'im/client/sources/WukClient.cc',
        # 'im/client/sources/WukMessageClient.cc',
        # 'im/client/sources/WukPacketBuilder.cc',
        # 'im/client/sources/WukSession.cc',
    )

    # wuk源文件
    compiler.add_args(
        # 'wuk/sources/crypto/WukOP4.cc',
        'wuk/sources/crypto/WukOP4_sse4.cc',
        # 'wuk/sources/crypto/WukChaCha20.cc',
        # 'wuk/sources/net/WukSocket.cc',
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
        '-I im',

        '-O2',
        # '-Wcast-align',
        # '-Wpedantic',
        # '-Wformat',
        '-Wextra',
        '-Werror',
        '-Wall',

        ### 这四个仅用于测试时使用，其他情况应注释掉
        # '-Wno-unused-variable',
        '-Wno-unused-function',
        '-Wno-unused-parameter',
        # '-Wno-unused-but-set-variable',

        '--std=c++17',
        '-DWUK_EXPORTS',
        '-march=native',
    )

    # 库链接
    compiler.add_args(
        '-lfmt',
    )

    compiler.build(source_file)

# === 入口点 ===
if __name__ == '__main__':
    main()


