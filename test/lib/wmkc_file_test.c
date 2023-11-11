#include <wmkc_file.h>

void read()
{
    wmkcByte *buf = wmkcNull;
    wmkcSize size = 0;
    wmkcFile_fread(&buf, &size, "test.txt");
    wmkcMem_free(buf);
}

void write()
{
    wmkcByte *buf = (wmkcByte *)"12345678";
    wmkcSize size = strlen((char *)buf);
    wmkcFile_fwrite(buf, size, "test.txt");
}

void info()
{
    wmkcFile_obj *file = wmkcNull;
    wmkcFile_open(&file, "test.txt", "rb");
    printf("%s\n", file->fn);
    printf("%p\n", file->fp);
    printf("%u\n", (wmkc_u32)file->fsize);
    printf("%u\n", (wmkc_u32)file->status);
}

int main()
{
    read();
    write();
    info();
    return 0;
}