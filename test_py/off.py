# 导入datetime模块，用于处理日期和时间
import datetime
# 导入time模块，用于延时和获取系统时间
import time
# 导入os模块，用于执行系统命令
import os

# 定义下班时间为17点27分
off_work_hour = 17
off_work_minute = 27

# 获取当前时间
now = datetime.datetime.now()
# 获取当前的小时和分钟
now_hour = now.hour
now_minute = now.minute

# 如果当前时间已经超过下班时间，直接关机
if now_hour > off_work_hour or (now_hour == off_work_hour and now_minute > off_work_minute):
    print("已经下班了，马上关机")
    os.system("shutdown -s -t 0") # 执行关机命令，-s表示关机，-t 0表示立即执行
# 如果当前时间还没有到下班时间，计算剩余的秒数，并延时相应的时间，然后关机
else:
    # 计算剩余的秒数，注意要考虑60进制的换算
    remain_second = (off_work_hour - now_hour) * 3600 + (off_work_minute - now_minute) * 60 - now.second
    # 打印剩余的秒数
    print(f"距离下班还有{remain_second}秒")
    # 延时相应的秒数
    time.sleep(remain_second)
    # 延时结束后，打印关机提示
    print("下班时间到了，马上关机")
    # 执行关机命令
    os.system("shutdown -s -t 0")

