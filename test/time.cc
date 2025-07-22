#include <config/WukConfig.hh>

#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std;

#ifdef WUK_PLATFORM_WINOS
#include <windows.h>

void SetFileTimes(const char* filePath, SYSTEMTIME creationTime, SYSTEMTIME lastAccessTime, SYSTEMTIME lastWriteTime) {
    FILETIME ftCreationTime, ftLastAccessTime, ftLastWriteTime;

    // 将SYSTEMTIME转换为FILETIME
    SystemTimeToFileTime(&creationTime, &ftCreationTime);
    SystemTimeToFileTime(&lastAccessTime, &ftLastAccessTime);
    SystemTimeToFileTime(&lastWriteTime, &ftLastWriteTime);

    // 打开文件
    HANDLE hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("无法打开文件: %s\n", filePath);
        return;
    }

    // 设置文件时间
    if (!SetFileTime(hFile, &ftCreationTime, &ftLastAccessTime, &ftLastWriteTime)) {
        printf("无法设置文件时间: %s\n", filePath);
    }

    // 关闭文件
    CloseHandle(hFile);
}

void test()
{
    const char* filePath = "C:/Users/sn/Desktop/PNG_TEST/122436328_p0_unpack.png.bin";

    SYSTEMTIME basicTime = {
        .wYear = 2222,
        .wMonth = 2,
        .wDayOfWeek = 0,
        .wDay = 22,
        .wHour = 22 - 8,
        .wMinute = 22,
        .wSecond = 22,
        .wMilliseconds = 22
    };

    SYSTEMTIME creationTime = basicTime;   // 设置创建时间
    SYSTEMTIME lastAccessTime = basicTime; // 设置访问时间
    SYSTEMTIME lastWriteTime = basicTime;  // 设置修改时间

    SetFileTimes(filePath, creationTime, lastAccessTime, lastWriteTime);

    printf("The file timestamp has been updated: %s\n", filePath);
}
#endif

int main() {
    return 0;
}
