#include <snWinAPI.h>

WMKC_PUBLIC(wmkcVoid) snWinAPI_fileHandler WMKC_OPEN_API
WMKC_OF(())
{
    HANDLE hFile;
    hFile = CreateFileW(L"Documents/QQ公告_GB18030编码1232323.txt", GENERIC_READ,
        FILE_SHARE_READ, wmkcNull, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, wmkcNull);
    ReadFile(hFile, wmkcNull, 4096, wmkcNull, wmkcNull);
}

