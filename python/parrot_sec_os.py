import subprocess
import shutil
import os
import re

default_source_list = b'''\
# deb https://deb.parrot.sh/parrot lory main contrib non-free
# deb-src https://deb.parrot.sh/parrot lory main contrib non-free
'''

default_source_list_d_parrot_list = b'''\
### Default Parrot Repository
# deb https://deb.parrot.sh/parrot lory main contrib non-free non-free-firmware

### Security Updates
# deb https://deb.parrot.sh/direct/parrot lory-security main contrib non-free non-free-firmware

### Backports
# deb https://deb.parrot.sh/parrot lory-backports main contrib non-free non-free-firmware

### Updates / Testing
# deb https://deb.parrot.sh/parrot lory-updates main contrib non-free non-free-firmware

########################################################################################
### Some popular mirrors

## USTC University
deb https://mirrors.ustc.edu.cn/parrot/ lory main contrib non-free non-free-firmware

## Aliyun
# deb deb https://mirrors.aliyun.com/parrot lory main contrib non-free non-free-firmware

## Taiwan
# deb https://free.nchc.org.tw/parrot/ lory main contrib non-free non-free-firmware

## We strongly invite you to visit https://www.parrotsec.org/docs/mirrors/mirrors-list/
## to find the mirror that better fits your needs
'''

def fwrite(fn :str, data :bytes):
    with open(fn, 'wb') as f:
        return f.write(data)

def shell_run(cmd :str):
    return subprocess.call(cmd, shell=True)

class py3:
    def __init__(self, python = 'python3'):
        self.source_url  = 'http://mirrors.aliyun.com/pypi/simple/'
        self.source_host = 'mirrors.aliyun.com'
        self.py          = python

        self.package_lists = [
            'requests', 'pillow', 'qrcode', 'paramiko', 'colorama',
            'numpy', 'scipy', 'lxml', 'scrapy', 'flask', 'django',
            'pycryptodome', 'rsa', 'hexdump', 'opencv-python',
            'git-python'
        ]

    def init_source(self):
        shell_run(f'{self.py} -m pip config set global.index-url {self.source_url}')
        shell_run(f'{self.py} -m pip config set install.trusted-host {self.source_host}')

    def package_add(self, name :str):
        self.package_list.append(name)

    def package_list(self):
        c   = '\x1b[94m' # color
        rst = '\x1b[0m'
        print('##### Python source package #####')
        for name in self.package_lists:
            print(f'## {c}{name:<27}{rst} ##')
        print('##### Python source package #####')

    def package_install(self):
        command = f'{self.py} -m pip install {" ".join(self.package_lists)}'
        print(command)

def main(py_config :bool = True):
    # 如果是root用户
    if not os.getuid():
        # 备份默认的source.list与source.list.d/parrot.list
        shutil.copy('/etc/apt/sources.list', '/etc/apt/sources.list.bak')
        shutil.copy('/etc/apt/sources.list.d/parrot.list', '/etc/apt/sources.list.d/parrot.list.bak')

        # 覆盖默认的source.list与source.list.d/parrot.list
        fwrite('/etc/apt/sources.list', default_source_list)
        fwrite('/etc/apt/sources.list.d/parrot.list', default_source_list_d_parrot_list)

        apt_package_lists = ' '.join([
            'libpcre3-dev', 'zlib1g', 'zlib1g-dev',
            'libdnet', 'libdnet-dev',
            'gcc', 'g++', 'make', 'automake',
            'tar', 'zip', 'rar',
            'curl', 'wget', 'net-tools', 'dnsutils', 'libssl-dev', 'libssl1.1', 'openssl',
            'build-essential', 'libncursesw5-dev',
            'libgdbm-dev', 'libc6-dev', 'libffi-dev', 'git', 'vim', 'pluma'
        ])

        shell_run('apt update')
        shell_run(f'apt install {apt_package_lists}')

        if py_config:
            py_ctx = py3()
            py_ctx.init_source()
            py_ctx.package_install()

    # 否则终止代码
    else:
        print('not root.')

if __name__ == '__main__':
    main()
