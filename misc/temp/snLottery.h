#ifndef WMKC_LOTTERY
#define WMKC_LOTTERY

#include <wmkc_random.h>
#include <wmkc_sort.h>

#define DLT_RED_SIZE 5
#define DLT_BLUE_SIZE 2
#define DLT_RED_MAX 35
#define DLT_BLUE_MAX 12

#define SSQ_RED_SIZE 6
#define SSQ_BLUE_SIZE 1
#define SSQ_RED_MAX 33
#define SSQ_BLUE_MAX 16

typedef struct {
    wmkcByte dlt_number[7];
    wmkcByte ssq_number[7];
    wmkcChar dlt_text[32];
    wmkcChar ssq_text[32];
} snLottery_ctx;

WMKC_PUBLIC(wmkcVoid) snLottery_DLT WMKC_OPEN_API WMKC_OF((snLottery_ctx *ctx))
{
    wmkc_s32 dlt_number_red[DLT_RED_SIZE];
    wmkc_s32 dlt_number_blue[DLT_BLUE_SIZE];
    wmkc_s32 index, temp;
    for(index = 0; index < DLT_RED_SIZE; ++index)
        dlt_number_red[index] = wmkcRandom_randint(1, DLT_RED_MAX);
    for(index = 0; index < DLT_BLUE_SIZE; ++index)
        dlt_number_blue[index] = wmkcRandom_randint(1, DLT_BLUE_MAX);
    bubbleSort(dlt_number_red, DLT_RED_SIZE);
    bubbleSort(dlt_number_blue, DLT_BLUE_SIZE);

    for(index = 0; index < DLT_RED_SIZE; ++index)
        ctx->dlt_number[index] = dlt_number_red[index];
    for(index = 0; index < DLT_BLUE_SIZE; ++index)
        ctx->dlt_number[index + DLT_RED_SIZE] = dlt_number_blue[index];

    snprintf(ctx->dlt_text, sizeof(ctx->dlt_text),
        "%02u %02u %02u %02u %02u - %02u %02u",
        ctx->dlt_number[0],
        ctx->dlt_number[1],
        ctx->dlt_number[2],
        ctx->dlt_number[3],
        ctx->dlt_number[4],
        ctx->dlt_number[5],
        ctx->dlt_number[6]
    );
}

WMKC_PUBLIC(wmkcVoid) snLottery_SSQ WMKC_OPEN_API WMKC_OF((snLottery_ctx *ctx))
{
    wmkc_s32 ssq_number_red[SSQ_RED_SIZE];
    wmkc_s32 ssq_number_blue[SSQ_BLUE_SIZE];
    wmkc_s32 index, temp;
    for(index = 0; index < SSQ_RED_SIZE; ++index)
        ssq_number_red[index] = wmkcRandom_randint(1, SSQ_RED_MAX);
    for(index = 0; index < SSQ_BLUE_SIZE; ++index)
        ssq_number_blue[index] = wmkcRandom_randint(1, SSQ_BLUE_MAX);
    bubbleSort(ssq_number_red, SSQ_RED_SIZE);
    bubbleSort(ssq_number_blue, SSQ_BLUE_SIZE);

    for(index = 0; index < SSQ_RED_SIZE; ++index)
        ctx->ssq_number[index] = ssq_number_red[index];
    for(index = 0; index < SSQ_BLUE_SIZE; ++index)
        ctx->ssq_number[index + SSQ_RED_SIZE] = ssq_number_blue[index];

    snprintf(ctx->ssq_text, sizeof(ctx->ssq_text),
        "%02u %02u %02u %02u %02u %02u - %02u",
        ctx->ssq_number[0],
        ctx->ssq_number[1],
        ctx->ssq_number[2],
        ctx->ssq_number[3],
        ctx->ssq_number[4],
        ctx->ssq_number[5],
        ctx->ssq_number[6]
    );
}

#endif