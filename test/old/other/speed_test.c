#include <wmkc_conf.h>
#include <wmkc_color.h>

#include <wmkc_time.h>
#include <wmkc_random.h>

#ifndef WMKC_PLATFORM_WINOS
#   error "No, The platform not is Windows."
#endif

#include <conio.h>

#define SPACE "                                                                         "

void print_red()
{
    printf("\r   [%s]   ", WMKC_COLOR_SET(WMKC_COLOR_BACK_RED, SPACE));
}

void print_green()
{
    printf("\r   [%s]   ", WMKC_COLOR_SET(WMKC_COLOR_BACK_GREEN, SPACE));
}

void wait()
{
    wmkcTime_sleep((double)wmkcRandom_randint(1500, 4500) / 1000);
}

void check()
{
#   if defined(WMKC_PLATFORM_WINOS)
    system("chcp 65001 && cls");
#   endif
}

int main()
{
    wmkcTime_obj timer;
    wByte key;
    double _timer;
    wU32 res;

    check();

    _timer = 0;
    for(wU32 count = 0; count < 3; ++count) {
        do {
            print_red();
            wait();
            print_green();

            wmkcTime_TimerBegin(&timer);
            getch();
            wmkcTime_TimerEnd(&timer);
        } while(!timer.totalTime);

        // printf("Time: %.2lf s.\n", timer.totalTime);
        _timer += timer.totalTime;
    }

    res = (wU32)((_timer / 3) * 1000);
    if(res < 150) {
        printf(WMKC_COLOR_SET(WMKC_COLOR_BACK_CYAN, "-> Speed: %ums.\n"), res);
    } else if(res >= 150 && res < 260) {
        printf(WMKC_COLOR_SET(WMKC_COLOR_BACK_GREEN, "-> Speed: %ums.\n"), res);
    } else {
        printf(WMKC_COLOR_SET(WMKC_COLOR_BACK_RED, "-> Speed: %ums.\n"), res);
    }

    printf("%s\n", WMKC_COLOR_SET(WMKC_COLOR_ALL_FLICKER, "   [Press any key to exit.]"));
    getch();

    return 0;
}
