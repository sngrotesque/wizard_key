from binascii import a2b_hex as a2b
from random import randint as rand
from ctypes import c_uint8 as uint8_t

def PRINT(size = 8):
    data = [rand(0, 255) for x in range(32)]
    for x in range(32):
        print("0x%02x"%data[x], end="")
        if (x + 1) != 32:
            print(", ", end="")
        if (x + 1) % size == 0:
            print("\n", end="")

def list2hex(data :list):
    return "".join(["%02x"%x for x in data])

def main():
    with open("KEY_HEX.txt", "r") as f:
        data = f.read()
    data = data.strip("\n")
    data = data.split("\n")
    print(len(data))
    data = set(data)
    print(len(data))

if __name__ == "__main__":
    main()


