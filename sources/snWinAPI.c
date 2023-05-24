#include <snWinAPI.h>

SN_PUBLIC(snVoid) snWinAPI_fileHandler SN_OPEN_API
SN_FUNC_OF(())
{
    HANDLE hFile;
    hFile = CreateFileW(L"Documents/QQ公告_GB18030编码1232323.txt", GENERIC_READ,
        FILE_SHARE_READ, snNull, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, snNull);
    ReadFile(hFile, snNull, 4096, snNull, snNull);
}

