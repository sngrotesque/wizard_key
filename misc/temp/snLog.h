#ifndef WMKC_LOG
#define WMKC_LOG

#include <wmkc_conf.h>
#include <wmkc_time.h>

#define SN_LOG_FILE_NAME "log/sn_log.txt"
#define LOG_ERROR_NAME "log/error_log.txt"

WMKC_PUBLIC(wmkcErr_ctx) snLog_write WMKC_OPEN_API WMKC_OF((FILE *fp, wmkcStreammessage))
{
    FILE *SN_STDERR = fopen(LOG_ERROR_NAME, "a+");

    if (!fp) {
        fprintf(SN_STDERR, "[%s] \"log_write函数参数FILE *fp打开指定文件失败\"\n", wmkcTime_GetNowTime());
        return wmkcErr_Err32;
    }

    if (!message) {
        fprintf(SN_STDERR, "[%s] \"log_write函数参数message指向NULL\"\n", wmkcTime_GetNowTime());
        return wmkcErr_Err32;
    }

    wmkcSize message_length = strlen(message);
    if (message_length == 0) {
        fprintf(SN_STDERR, "[%s] \"log_write函数参数message长度为0\"\n", wmkcTime_GetNowTime());
        return wmkcErr_Err32;
    }

    wmkcChar text[256] = {0};
    sprintf(text, "[%s] %s", wmkcTime_GetNowTime(), message);
    wmkcSize text_len = strlen(text);

    fwrite(text, 1, text_len, fp);
    fclose(SN_STDERR);
    return wmkcErr_OK;
}

#endif