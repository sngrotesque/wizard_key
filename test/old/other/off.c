#include <wmkc_time.h>

typedef struct tm TM;
typedef struct {
    wU32 hour;
    wU32 min;
    wU32 sec;
} offTimeStructure;

// 默认下班时间
static const offTimeStructure defaultOffTime = {17, 27, 15};

wVoid system_off(wVoid)
{
    // system("shutdown /f /p");
    MessageBoxExW(wmkcNull, L"将系统关机", L"测试窗口", MB_OK, 0);
}

wBool check_chcp(wVoid)
{
    if(GetConsoleOutputCP() != 65001) {
        if(SetConsoleOutputCP(65001)) {
            return true;
        }
    } else {
        return true;
    }
    return false;
}

wS32 main(wS32 argc, wChar **argv)
{
    TM *now = wmkcNull;
    time_t now_time;
    wS32 remain_second;
    wBool directlyShutdown;
    offTimeStructure now_arr;

    now_time = time(NULL);
    now = localtime(&now_time);
    now_arr.hour = now->tm_hour;
    now_arr.min = now->tm_min;
    now_arr.sec = now->tm_sec;

    directlyShutdown = ((now_arr.hour == defaultOffTime.hour) &&
                        (now_arr.min >= defaultOffTime.min) &&
                        (now_arr.sec >= defaultOffTime.sec)) ||
                        (now_arr.hour > defaultOffTime.hour);

    if(GetConsoleWindow()) {
        if(!check_chcp()) {
            printf("An error occurred while setting the code page.\n");
            return -1;
        }
    }
    if(!directlyShutdown) {
        remain_second = (defaultOffTime.hour - now_arr.hour) * 3600 +
                        (defaultOffTime.min - now_arr.min) * 60 + 
                        (defaultOffTime.sec - now_arr.sec);
        while(remain_second) {
            printf("\r现在是%02u:%02u:%02u，距离下班还有%5d秒",
                now->tm_hour, now->tm_min, now->tm_sec++, remain_second--);
            mktime(now);
            now->tm_min = now->tm_min; // 获取调整后的分钟数
            now->tm_sec = now->tm_sec; // 获取调整后的秒数
            wmkcTime_sleep(1);
        }
    }

    system_off();
    return 0;
}
