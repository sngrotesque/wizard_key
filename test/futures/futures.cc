#include <net/WukSocket.hh>

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const TIMEVAL *timeout);

/** 说明 ********************
 * -- 参数 ---------------------------------------------------------------
 * 
 * Platform:           Windows
 * Reference: https://learn.microsoft.com/zh-cn/windows/win32/api/winsock2/nf-winsock2-select
 * 
 * [in] nfds           已忽略。 包含 nfds 参数只是为了与 Berkeley 套接字兼容。
 * [in, out] readfds   一个可选指针，指向要检查的一组套接字的可读性。
 * [in, out] writefds  指向要检查可写性的一组套接字的可选指针。
 * [in, out] exceptfds 指向要检查错误的一组套接字的可选指针。
 * [in] timeout        选择等待的最长时间，以 TIMEVAL 结构的形式提供。 将阻止操作的 超时 参数设置为 null 。
 * 
 * Platform:           Linux
 * Reference: https://man7.org/linux/man-pages/man2/select.2.html
 * 
 * [in] nfds           需要监视的最大文件描述符加1，即待监视的文件描述符的最大值加1。
 * [in, out] readfds   一个可选指针，指向要检查的一组套接字的可读性。
 * [in, out] writefds  指向要检查可写性的一组套接字的可选指针。
 * [in, out] exceptfds 指向要检查错误的一组套接字的可选指针。
 * [in] timeout        选择等待的最长时间，以 TIMEVAL 结构的形式提供。 将阻止操作的 超时 参数设置为 null 。
 * 
 * -- 返回值 ---------------------------------------------------------------
 * 
 * >=0：返回当前有事件发生的文件描述符的总数
 * ==0：表示超时，没有事件发生，不是错误。
 * <=0：出错，可使用全局错误代码查看代码。
 * 
 * -- 操作API ---------------------------------------------------------------
 * 
 * FD_ZERO：初始化为空集（使用之前都应初始化）
 * FD_CLR：从集中删除套接字
 * FD_ISSET：检查是否为SET的成员，如果是将返回True
 * FD_SET：添加要设置的套接字
 * 
*/

void func()
{
    
}

