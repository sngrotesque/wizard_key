from base64 import b64encode as ben, b64decode as bde
from random import randint

plain = "".join(["0" for x in range(randint(1, 4096))]).encode()
plain_n = len(plain)
cipher = ben(plain)
cipher_n = len(cipher)

guess = 0
if not plain_n % 3:
    print("余数为 0")
    guess = plain_n / 3 * 4
elif plain_n % 3 == 1:
    print("余数为 1")
    guess = (plain_n + 2) / 3 * 4
elif plain_n % 3 == 2:
    print("余数为 2")
    guess = (plain_n + 1) / 3 * 4


print(
    f"plain length: {plain_n}\n"
    f"cipher length: {cipher_n}\n"
    f"guess length: {guess}\n"
    f"guess length == cipher length: {guess == cipher_n}"
)





