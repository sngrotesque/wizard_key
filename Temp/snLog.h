#ifndef __SN_LOG__
#define __SN_LOG__

#include <snConf.h>
#include <snTime.h>

#define SN_LOG_FILE_NAME "log/sn_log.txt"
#define LOG_ERROR_NAME "log/error_log.txt"

SN_PUBLIC(snError) snLog_write SN_OPEN_API SN_FUNC_OF((snFile *fp, const snChar *message))
{
    snFile *SN_STDERR = fopen(LOG_ERROR_NAME, "a+");

    if (!fp) {
        fprintf(SN_STDERR, "[%s] \"log_write函数参数FILE *fp打开指定文件失败\"\n", snTime_GetNowTime());
        return snErr_Err32;
    }

    if (!message) {
        fprintf(SN_STDERR, "[%s] \"log_write函数参数message指向NULL\"\n", snTime_GetNowTime());
        return snErr_Err32;
    }

    snSize message_length = strlen(message);
    if (message_length == 0) {
        fprintf(SN_STDERR, "[%s] \"log_write函数参数message长度为0\"\n", snTime_GetNowTime());
        return snErr_Err32;
    }

    snChar text[256] = {0};
    sprintf(text, "[%s] %s", snTime_GetNowTime(), message);
    snSize text_len = strlen(text);

    fwrite(text, 1, text_len, fp);
    fclose(SN_STDERR);
    return snErr_OK;
}

#endif