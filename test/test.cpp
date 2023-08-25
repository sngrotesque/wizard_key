#include <wmkc_memory.c>

#include <iostream>
#include <fstream>

using namespace std;

int file_read(const char *fn, string *FileStream)
{
    fstream *fp = new fstream(fn, ios::in | ios::binary);
    streamsize fileReadSize;
    wmkcChar _tmp_buffer[2048];

    if(!fp->is_open()) {
        cout << "Failed to open file." << endl;
        return EOF;
    }

    do {
        wmkcMem_zero(_tmp_buffer, sizeof(_tmp_buffer));
        fileReadSize = fp->read(_tmp_buffer, sizeof(_tmp_buffer)).gcount();
        FileStream->append(_tmp_buffer, fileReadSize);
    } while(fileReadSize);

    fp->close();

    delete fp;
    return 0;
}

int main()
{
    string FileStream;

    file_read("test/test.c", &FileStream);
    cout << FileStream << endl;

    return 0;
}
