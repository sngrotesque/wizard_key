#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

// 定义遍历并输出指定路径下所有文件和子目录的函数
void list_dir(const char *path)
{
    // 打开指定路径下的目录
    DIR *dir = opendir(path);
    // 如果打开失败，则输出错误信息并返回
    if (dir == NULL)
    {
        printf("Error: cannot open %s\n", path);
        return;
    }
    // 读取该目录下所有文件和子目录
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // 获取每个文件或子目录的名称
        char *name = entry->d_name;
        // 如果名称为"."或".."则跳过
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;
        // 拼接完整路径名
        char full_path[256];
        sprintf(full_path, "%s/%s", path, name);
        // 获取每个文件或子目录的信息
        struct stat st;
        stat(full_path, &st);
        // 判断是否为普通文件或子目录
        if (S_ISREG(st.st_mode))
            printf("%s is a file\n", full_path); // 如果是普通文件，则输出其名称
        else if (S_ISDIR(st.st_mode))
            list_dir(full_path); // 如果是子目录，则递归调用本函数，遍历其下所有内容
    }
    // 关闭打开的目录
    closedir(dir);
}

// 主函数
int main()
{
    // 定义要遍历的路径名（可以修改）
    const char *path = ".";
    // 调用list_dir函数，遍历并输出该路径下所有内容
    list_dir(path);
}