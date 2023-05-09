from binascii import a2b_hex
from os import system, _exit as exit
from hashlib import sha256 as SHA_256

def sha256(data):
    info = SHA_256()
    info.update(data)
    return info.hexdigest()

cipher_table_start = "啊汪哇呜~"
cipher_table_end = "+汪汪~"

def encrypt(data :bytes):
    cipher_table_encrypt = {
        "0": "Z",  "1": "F",  "2": "B",  "3": "G",  "4": "O",
        "5": "T",  "6": "C",  "7": "K",  "8": "A",  "9": "P",
        "a": "5",  "b": "r",  "c": "l",  "d": "q",  "e": "i",
        "f": "b"
    }
    data = list(data.hex())
    for x in range(len(data)):
        data[x] = cipher_table_encrypt[data[x]]
        data[x] = chr(ord(data[x]) - 10)
    data = "".join(data)
    return cipher_table_start + data + sha256(data.encode()) + cipher_table_end

def decrypt(data :str):
    cipher_table_decrypt = {
        "Z": "0",  "F": "1",  "B": "2",  "G": "3",  "O": "4",
        "T": "5",  "C": "6",  "K": "7",  "A": "8",  "P": "9",
        "5": "a",  "r": "b",  "l": "c",  "q": "d",  "i": "e",
        "b": "f"
    }
    if data[:5] != cipher_table_start:
        return False
    if data[-4:] != cipher_table_end:
        return False
    
    res = data[5:-68]
    hash_value = data[-68:-4]
    if sha256(res.encode()) != hash_value:
        return False
    
    res = list(res)
    for x in range(len(res)):
        try:
            res[x] = chr(ord(res[x]) + 10)
            res[x] = cipher_table_decrypt[res[x]]
        except KeyError:
            print("密文错误，请检查密文正确性，请不要试图使用反推来求得解密算法机制。")
            exit(-1)
    return a2b_hex("".join(res)).decode('utf-8')

def main():
    xx = input(
        "菜单:\n"
        "\t[0]. 退出\n"
        "\t[1]. 加密\n"
        "\t[2]. 解密\n\n"
        "请输入(数字): ")
    if xx == '1':
        res = input("请输入内容: ")
        res = encrypt(res.encode())
        print(f"加密结果:\n{res}")
        system("pause")
    elif xx == '2':
        res = input('请输入内容: ')
        res = decrypt(res)
        if res == False:
            print("密文错误，请检查密文完整性。")
            system("pause")
            exit(-1)
        print(f"解密结果:\n{res}")
        system("pause")
    elif xx == '0':
        exit(0)
    else:
        print("请输入数字吖！！！！")
        exit(-1)

if __name__ == "__main__":
    main()

