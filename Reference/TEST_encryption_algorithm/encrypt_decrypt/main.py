from base64 import b64encode as ben, b64decode as bde
from binascii import a2b_hex as a2b, b2a_hex as b2a
from os import system as sy_cmd
from random import sample, choice

def B64(source):
    _input = bde(source)
    str_input = str(_input, encoding="UTF-8", errors="ignore")

    _hex_input = b2a(_input).decode()
    hex_input = ""
    index = 0
    for x in range(len(_hex_input)//2):
        hex_input += (_hex_input[index:index+2] + "  ").upper()
        if (x + 1) % 42 == 0:
            hex_input += "\n"
        index += 2

    print(f"以下是原Base64编码数据:\n{source}\n")
    print(f"以下是解码之后的数据(二进制):\n{_input}\n")
    print(f"以下是解码之后的数据(硬解UTF-8):\n{str_input}\n")
    print(f"以下是解码之后的数据(十六进制):\n{hex_input}\n")

def Random(arrLen :int, char :int):
    if char == 1:
        arr = [choice(range(arrLen)) for x in range(arrLen)]
        return arr
    elif char == 2:
        arr = [hex(choice(range(arrLen))).replace("0x", "") for x in range(arrLen)]
        for x in range(arrLen):
            if (len(arr[x]) == 1):
                arr[x] = "0" + arr[x]
            arr[x] = "\\x" + arr[x]
        return "".join(arr)
    else:
        return "Error."

def del_space(space):
    print(B64(a2b(space.replace("\n","").replace(" ",""))))

if __name__ == "__main__":
    sy_cmd("clear")
    B64("j85Csho5Ew==")

