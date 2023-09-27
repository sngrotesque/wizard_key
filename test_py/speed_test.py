import numpy as np
import time

def Counter():
    x = 0
    for i in range(10000000):
        x += i
    return x

def SpeedTest():
    count = []
    for _ in range(10):
        s = time.time()
        x = Counter()
        count.append(time.time() - s)
        print(x)
    return np.mean(count)

print(SpeedTest())
