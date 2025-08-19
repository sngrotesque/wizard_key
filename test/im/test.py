from datetime import datetime

def str_to_unix_timestamp(time_str, format='%Y-%m-%d %H:%M:%S'):
    dt = datetime.strptime(time_str, format)  # 解析为datetime对象
    return int(dt.timestamp() * 1000)  # 乘以1000转为毫秒级

time_str = "2025-07-01 00:00:00"
unix_timestamp = str_to_unix_timestamp(time_str)
print(unix_timestamp)  # 输出毫秒级时间戳