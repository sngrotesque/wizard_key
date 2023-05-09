/*
* 此为扫雷游戏的C语言代码实现
* 作者：SN-Grotesque
*
* 优先使用”深度优先搜索“算法
*
* 如实在无思路，请查看此实现
* Projects/QQ_Bot/AngelinaBot/AngelinaBot/src/main/java/top/strelitzia/service/MineSweepingService.java
*/

#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#include <snColor.h>
#include <snRand.h>
#include <snConf.h>
#include <snTime.h>

#define numberRows    9 // 行数
#define numberColumns 9 // 列数
#define numberMines   ((numberRows * numberColumns) / 8 + 1) // 雷的数量

#define basicSymbol '#' // 标准图标（未点击未标记）
#define mineSymbol  '%' // 地雷图标
#define openSymbol  '-' // 已点击图标（无地雷无标记）
#define askSymbol   '?' // 不确定图标
#define signSymbol  '$' // 标记图标

typedef snByte game_state_t[numberRows][numberColumns];

typedef struct {
    snByte maps[numberRows][numberColumns];
    snByte mine[numberRows][numberColumns];
} mines_ctx;

/**********************************************
* Public Function                             *
**********************************************/
SN_PRIVATE(snVoid) minesGame_init(mines_ctx *ctx);
SN_PRIVATE(snVoid) minesGame();
SN_PRIVATE(snVoid) minesGame_sleep(snFloat time);

/**********************************************
* Private Function                            *
**********************************************/
SN_PRIVATE(snVoid) minesGame_print(game_state_t *matrix);

/*********************************************/

SN_PRIVATE(snVoid) minesGame_init(mines_ctx *ctx)
{
    snSetRandomTimerSeed();
    snByte i;
    for(i = 0; i < numberRows; ++i) {
        memset(ctx->maps[i], basicSymbol, numberRows);
        memset(ctx->mine[i], basicSymbol, numberRows);
    }
    for(i = 0; i < numberMines; ++i) {
        ctx->mine[randint(0, numberRows - 1)][randint(0, numberColumns - 1)]\
            = mineSymbol;
    }
}

SN_PRIVATE(snVoid) minesGame()
{
    static mines_ctx *ctx = snNull;
    static snTimer_ctx *timer = snNull;
    static snSize count;
    static snChar mine_x[6];
    static snChar mine_y[6];
    static snByte inputBuff;
    static sn_u32 x, y;
    static snBool gameOver;

    ctx = (mines_ctx *)malloc(sizeof(mines_ctx));
    timer = (snTimer_ctx *)malloc(sizeof(snTimer_ctx));

    minesGame_init(ctx);

    snTime_TimerBegin(timer);
    for(count = 1; !gameOver; ++count) {
        system("clear");
        printf("第%lu次....\n", count);

        minesGame_print((game_state_t *)ctx->maps);
        printf("请输入坐标: ");
        scanf("%s%s", mine_x, mine_y);
        scanf("%c", &inputBuff);

        x = strtol(mine_x, NULL, 10);
        y = strtol(mine_y, NULL, 10);

        if(x > (numberRows - 1) || y > (numberColumns - 1)) {
            printf("错误坐标，请重新输入...\n");
            minesGame_sleep(0.85);
            continue;
        }

        switch(ctx->mine[x][y]) {
            case mineSymbol:
                printf("Boom!!! You're Dead.\n");
                gameOver = true;
                break;
            case basicSymbol:
                ctx->maps[x][y] = openSymbol;
                break;
        }
    }
    snTime_TimerEnd(timer);

    printf("用时: %.2lf秒.\n", timer->totalTime);

    free(timer);
    free(ctx);
}

/*********************************************/

SN_PRIVATE(snVoid) minesGame_print(game_state_t *matrix)
{
    snByte i, j;

    printf("   ");
    for(i = 0; i < numberRows; ++i) {
        printf("%3.02d", i);
    }
    printf("\n");

    for(i = 0; i < numberRows; ++i) {
        printf("%3.02d", i);
        for(j = 0; j < numberColumns; ++j) {
            switch((*matrix)[j][i]) {
                case basicSymbol:
                    printf("%s%3c%s",
                        _SN_BACK_LIGHTBLACK, (*matrix)[j][i], _SN_C_ALL_RESET);
                    break;
                case openSymbol:
                    printf("%s%3c%s",
                        _SN_BACK_LIGHTWHITE, (*matrix)[j][i], _SN_C_ALL_RESET);
                    break;
                case mineSymbol:
                    printf("%s%3c%s",
                        _SN_BACK_LIGHTRED, (*matrix)[j][i], _SN_C_ALL_RESET);
                    break;
                default:
                    printf("%3c", (*matrix)[j][i]);
                    break;
            }
        }
        printf("\n");
    }
}

SN_PRIVATE(snVoid) minesGame_sleep(snFloat time)
{
    usleep(time * 1000000);
}
