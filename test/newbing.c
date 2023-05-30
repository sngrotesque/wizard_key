#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <conio.h>

void inputc(void*);
void timec(void*);
void gotoxy(int x, int y);

int isrun = 1; //控制主线程运行或结束
int timerest = 10; //设置初始时间
char word[10]; //接收单词输入

int main() {
    system("cls");
    _beginthread(timec, 0, NULL); //启动倒计时显示线程
    _beginthread(inputc, 0, NULL); //启动输入单词线程
    while (isrun); //阻塞主线程，让子线程执行
    return 0;
}

//显示信息
void update() {
    gotoxy(0, 0);
    printf("\n\n 剩余时间：%d s", timerest);
    printf("\n\n 中文：你好");
    printf("\n\n "); //清空这一行
    gotoxy(0, 4);
    printf("\n\n 英文：%s", word); //重新显示
}

//输入控制线程
void inputc(void*) {
    int i = 0;
    char ch; //单个输入检查
    while (1) //执行输入
    {
        ch = _getch(); //读取字符
        if (ch=='@') //检查是否退出
        {
            isrun = 0; //输入@，结束主线程，即程序结束
            _endthread();
        }
        if (ch == '\b') //检测是否为退格
        {
            if (i > 0) {
                word[--i] = 0; //是的话将一个字符删掉并向前移动一位
                putchar('\b');
                putchar(' ');
                putchar('\b'); //这三句实现删掉已经输出的字符
            }
        } else {
            word[i] = ch; //放进数组
            putchar(ch);
            i++; //不是则向下移动并输出
        }
        if (ch == '\n') //检查是否输入完毕
        {
            if (strcmp(word, "hello")) //判断输入是否正确
            {
                /* ... */
            }
            word[0] = 0;
            i = 0; //还原数组
        }
    }
}

//时间控制线程
void timec(void*) {
    timerest = 10;
    while (timerest > 0) //倒计时
    {
        timerest--;
        update(); //刷新
        Sleep(1000); //每秒执行一次
    }
    _beginthread(timec, 0, NULL); //重新启动倒计时显示线程
    _endthread(); //关闭当前线程
}

//改变光标位置
void gotoxy(int x, int y) {
    // 更新光标位置
    COORD pos;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hOutput, pos);
}
