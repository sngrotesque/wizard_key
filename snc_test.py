from subprocess import call
from os import remove
from time import sleep
from sys import platform

code_path = 'test/snc_test.c'
if platform == 'win32':
    program_name = 'snc_test.exe'
else:
    program_name = './snc_test'
parameter = '-I includes -I source --std=c18 -Ofast'
command = f'gcc {code_path} {parameter} -o {program_name}'

def run(cmd :str):
    call(cmd, shell = True)

run(command)
run(program_name)

# remove(program_name)
