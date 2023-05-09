from subprocess import call

def action(cmd :str):
    call(cmd, shell=True)

code_fn = 'snc.c'
lib_fn = 'libsnc.so'
view_nm = True

cmd_list = [
    f'gcc -I /home/sng/code/Projects/Shark_Coast/includes/ -Ofast --std=c18 -c {code_fn}',
    f'ar cr {lib_fn} {code_fn[:-2]}.o',
    f'rm -fr {code_fn[:-2]}.o'
]

for cmd in cmd_list:
    action(cmd)

if view_nm:
    action(f'nm -s {lib_fn}')


