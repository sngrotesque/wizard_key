from typing import Union, List
import random

def PRINT_BOX(data :Union[List[int], bytes, bytearray], line_length :int = 16, newLine :bool = False):
    size = len(data)
    for x in range(size):
        print(f'0x{data[x]:>02x}', end = '')
        if (x + 1) != size:
            if not ((x + 1) % line_length):
                print(',')
            else:
                print(', ', end = '')
        else:
            print()
    if newLine:
        print()

sbox = [x for x in range(256)]
for _ in range(32):
    random.shuffle(sbox)

src = 191
dst = src
for i in range(256):
    dst = sbox[dst]
    if dst == src:
        if i >= 200:
            print(f'sbox OK!')
            break
        else:
            print(f'sbox NO!')
            exit()

rsbox = [0 for _ in range(256)]
for x in range(256):
    rsbox[sbox[x]] = x

print(f'sbox:')
PRINT_BOX(sbox, newLine=True)
print(f'rsbox:')
PRINT_BOX(rsbox)

