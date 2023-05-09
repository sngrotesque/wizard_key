# 双系统使用世界协调时

### WIndows
```bash
Reg add HKLM\SYSTEM\CurrentControlSet\Control\TimeZoneInformation /v RealTimeIsUniversal /t REG_DWORD /d 1
```

### Linux
```bash
timedatectl set-local-rtc false
```
