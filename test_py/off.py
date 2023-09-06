import subprocess
import datetime
import time

def shutdown():
    subprocess.call('shutdown /f /p', shell = True)
    # raise NotImplementedError

_now_time = datetime.datetime.now()

off = [17, 27]
now = [_now_time.hour, _now_time.minute, _now_time.second]

if now >= off:
    shutdown()
else:
    remain_second = (off[0] - now[0]) * 3600 + (off[1] - now[1]) * 60 - now[2]
    while remain_second:
        print(f'\r距离下班还有{remain_second:>5d}秒', end='')
        remain_second -= 1
        time.sleep(1)
    shutdown()

