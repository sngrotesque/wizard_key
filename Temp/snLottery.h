#ifndef __SN_LOTTERY__
#define __SN_LOTTERY__

#include <snRand.h>
#include <snSort.h>

#define DLT_RED_SIZE 5
#define DLT_BLUE_SIZE 2
#define DLT_RED_MAX 35
#define DLT_BLUE_MAX 12

#define SSQ_RED_SIZE 6
#define SSQ_BLUE_SIZE 1
#define SSQ_RED_MAX 33
#define SSQ_BLUE_MAX 16

typedef struct {
    snByte dlt_number[7];
    snByte ssq_number[7];
    snChar dlt_text[32];
    snChar ssq_text[32];
} snLottery_ctx;

SN_PUBLIC(snVoid) snLottery_DLT SN_OPEN_API SN_FUNC_OF((snLottery_ctx *ctx))
{
    sn_32 dlt_number_red[DLT_RED_SIZE];
    sn_32 dlt_number_blue[DLT_BLUE_SIZE];
    sn_32 index, temp;
    for(index = 0; index < DLT_RED_SIZE; ++index)
        dlt_number_red[index] = snRand_Randint(1, DLT_RED_MAX);
    for(index = 0; index < DLT_BLUE_SIZE; ++index)
        dlt_number_blue[index] = snRand_Randint(1, DLT_BLUE_MAX);
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

SN_PUBLIC(snVoid) snLottery_SSQ SN_OPEN_API SN_FUNC_OF((snLottery_ctx *ctx))
{
    sn_32 ssq_number_red[SSQ_RED_SIZE];
    sn_32 ssq_number_blue[SSQ_BLUE_SIZE];
    sn_32 index, temp;
    for(index = 0; index < SSQ_RED_SIZE; ++index)
        ssq_number_red[index] = snRand_Randint(1, SSQ_RED_MAX);
    for(index = 0; index < SSQ_BLUE_SIZE; ++index)
        ssq_number_blue[index] = snRand_Randint(1, SSQ_BLUE_MAX);
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