from subprocess import call
from sys import platform
from os import remove

def action(cmd :str):
    call(cmd, shell=True)

if platform == 'win32':
    print('检测到本机为Windows操作系统。')
    library_fn = 'libsnc.dll'
elif platform == 'linux':
    print('检测到本机为Linux操作系统。')
    library_fn = 'libsnc.so'
else:
    print(f'请使用Windows或Linux操作系统，不支持其他操作系统。')
    exit(-1)

print('初始化程序中...')

# 是否展示nm指令
view_nm = 1

# 初始化路径
include_path = 'includes'
source_path = 'source'
source_fn = 'snc.c'
temp_fn = f'{source_fn[:-2]}.o'

# 编译指令
GCC_CompilationInstructions = (
    f'gcc -I {include_path} -I {source_path} -static -Ofast --std=c18 '
    f'-c {source_path}/crypto/{source_fn}')

print(f'开始编译，编译指令为：{GCC_CompilationInstructions}')
action(GCC_CompilationInstructions)
print(f'使用{temp_fn}文件生成库文件中...')
action(f'ar cr {library_fn} {temp_fn}')
print(f'生成完毕，删除{temp_fn}文件。')
remove(f'{temp_fn}') # 删除临时生成的文件

if view_nm:
    print(f'将展示{library_fn}文件内部。')
    action(f'nm -s {library_fn}')

print('程序结束。')
