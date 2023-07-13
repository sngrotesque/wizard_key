from os import urandom
from sys import getsizeof as sizeof

RANDOM_DATA_FILE_NAME = "SN_RANDOM_DATA_TEST_FILE.bin"

def start_time(func):
    from time import time
    s = time()
    func()
    e = time() - s
    return e

datasize = 500 * 1024 ** 2
data = urandom(datasize)

with open(RANDOM_DATA_FILE_NAME, "wb") as f:
    f.write(data)




