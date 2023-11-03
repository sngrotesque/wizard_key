#include <iostream>
#include <fstream>

#include <windows.h>
#include <cstdint>

#include <ctime>

using namespace std;

LPWSTR utf8_to_utf16le(LPCCH path)
{
    INT path_len = MultiByteToWideChar(CP_UTF8, 0, path, -1, nullptr, 0);
    LPWSTR win_path = new WCHAR[path_len];
    memset(win_path, 0x00, sizeof(WCHAR) * path_len);
    MultiByteToWideChar(CP_UTF8, 0, path, -1, win_path, path_len);
    return win_path;
}

VOID writeRandomData(LPWSTR path, UINT64 fileLegnth)
{
    FILE *fp = _wfopen(path, L"wb");
    if(!fp) {
        throw runtime_error("Failed to open file filePtr.");
    }

    UINT32 count = fileLegnth / 65536;
    UINT32 leftover = fileLegnth % 65536;
    LPBYTE randomData = new BYTE[65536];

    


    delete randomData;
    fclose(fp);
}

UINT64 getFileLength(LPWSTR path)
{
    LARGE_INTEGER ExtraLongLength; // Windows大整数类型
    HANDLE hFile = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    GetFileSizeEx(hFile, &ExtraLongLength); // 将文件大小写入大整数类型结构体
    CloseHandle(hFile); // 关闭文件句柄
    return (UINT64)ExtraLongLength.QuadPart;
}

class Delete {
    private:
        UINT16 owCount; // Overwrite count

    public:
        Delete(UINT16 overwrite_count = 3)
        : owCount(overwrite_count)
        {
            srand((unsigned)time(NULL));
        }

        ~Delete() {}

        void singleFile(string path)
        {
            
        }
};

int main(int argc, char **argv)
{
    // Delete *del = new Delete();
    // del->singleFile("I:/Pitchers/传送手机/13.png");
    // delete del;
    LPWSTR path = utf8_to_utf16le("C:/Users/z7z-h/Desktop/测试/测试文件.txt");
    writeRandomData(path, getFileLength(path));
    delete[] path;
}
