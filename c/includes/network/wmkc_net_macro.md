# 套接字具体错误代码

### General error

------

##### WMKC_NET_ERR_EINTR - [accept, send, recv, shutdown, close, connect]
    执行的调用被操作系统截断。

##### WMKC_NET_ERR_EACCES - [bind, send, socket, connect]
    以不被允许的方式访问套接字。

##### WMKC_NET_ERR_EFAULT - [bind, accept, send, recv, connect]
    地址指向了用户的不可访问的地址空间。

##### WMKC_NET_ERR_EINVAL - [bind, shutdown, listen, accept, sned, recv, socket, connect]
    提供了无效的参数或套接字已绑定某地址或在接受之前未调用侦听函数或套接字是监听套接字。

##### WMKC_NET_ERR_EMFILE - [accept, listen, socket]
    无法提供更多套接字描述符，已达到每个进程对套接字数量的限制。

##### WMKC_NET_ERR_EISCONN - [listen, send, connect]
    套接字已被指定连接。

##### WMKC_NET_ERR_ENOBUFS - [bind, accept, send, listen, socket, connect]
    未提供任何缓冲区空间。

##### WMKC_NET_ERR_EALREADY - [send, connect]
    另一个快速打开操作（TCP Fast Open）正在执行。指定的套接字上正在进行非阻塞模式connect调用。

##### WMKC_NET_ERR_ENOTCONN - [listen, shutdown, send, recv]
    套接字已经连接了 or 套接字未连接

##### WMKC_NET_ERR_EMSGSIZE - [send, recv]
    套接字面向消息，消息大于基础传输支持的最大大小

##### WMKC_NET_ERR_ENOTSOCK - [bind, listen, shutdown, accept, send, recv, closesocket, connect]
    描述符不是套接字

##### WMKC_NET_ERR_ETIMEDOUT - [send, recv, connect]
    由于超时导致连接已断开。

##### WMKC_NET_ERR_EPROTOTYPE - [socket, accept, connect]
    指定的协议是此套接字的错误类型，套接字类型不支持请求的通信协议。

##### WMKC_NET_ERR_EOPNOTSUPP - [send, recv, listen, accept]
    引用的套接字不是支持面向连接（监听）的服务的类型，或者套接字是单向的，只支持发送或接收操作。

##### WMKC_NET_ERR_EADDRINUSE - [bind, listen, accept, connect]
    本地地址或端口已被使用。套接字没有绑定到地址。

##### WMKC_NET_ERR_ECONNRESET - [shutdown, accept, send, recv]
    连接被重置。套接字不可再用，应关闭套接字（此错误仅适用于面向连接的套接字）。

##### WMKC_NET_ERR_ENETUNREACH - [connect]
    此时不可以从此主机访问该网络。网络不可用。

##### WMKC_NET_ERR_EWOULDBLOCK - [accept, send, recv, closesocket, connect]
    套接字标记为非阻塞模式，没有要接受的连接，请求的操作会进行阻止。套接字标记为非阻塞模式，无法立即完成连接。

##### WMKC_NET_ERR_EINPROGRESS - [bind, listen, shutdown, accept, send, recv, closesocket, socket, connect]
    套接字是非阻塞的，调用操作无法立即完成。

##### WMKC_NET_ERR_ECONNREFUSED - [recv, connect]
    连接被远程主机强制性拒绝。

##### WMKC_NET_ERR_ECONNABORTED - [shutdown, send, recv, accept]
    面向连接的套接字由于超时或其他故障线路已终止，套接字不可再用。

##### WMKC_NET_ERR_EAFNOSUPPORT - [socket, connect]
    指定网络族中的地址无法与此套接字一起使用。

##### WMKC_NET_ERR_EADDRNOTAVAIL - [bind, connect]
    请求的地址在上下文无效或请求的地址不是本地地址。远程地址不是有效的地址。

##### WMKC_NET_ERR_EPROTONOSUPPORT - [socket]
    不支持指定的协议

------

### Linux error

------

##### WMKC_NET_ERR_EIO - [close]
    出现I/O错误

##### WMKC_NET_ERR_EPIPE - [send]
    本地端已在面向连接的套接字上关闭，在这种情况下，除非设置了MSG_NOSIGNAL，否则进程也将接收SIGPIPE。

##### WMKC_NET_ERR_EPERM - [accept, connect]
    防火墙规则禁止连接

##### WMKC_NET_ERR_EROFS - [bind]
    套接字索引节点将驻留在只读文件系统上

##### WMKC_NET_ERR_EBADF - [bind, accept, close, shutdown, send, recv, listen, connect]
    参数sockfd不是有效的文件描述符

##### WMKC_NET_ERR_ELOOP - [bind]
    解析地址时遇到太多符号链接

##### WMKC_NET_ERR_EDQUOT
##### WMKC_NET_ERR_ENOSPC - [close]
    存储空间不足或已达到磁盘配额限制。

##### WMKC_NET_ERR_ENOENT - [bind]
    套接字路径名的目录前缀中的组件不存在

##### WMKC_NET_ERR_ENOMEM - [socket, accept, send, recv]
    可用的内核内存不足

##### WMKC_NET_ERR_ENFILE - [socket, accept]
    已达到系统范围内打开文件总数的限制

##### WMKC_NET_ERR_EPROTO - [accept]
    错误的协议

##### WMKC_NET_ERR_EAGAIN - [accept, send, recv, connect]
    套接字被标记为非阻塞，并且不存在任何可接受的连接。
    sockfd引用的套接字以前没有绑定到地址，在尝试将其绑定到临时端口时，确定临时端口范围内的所有端口号当前都在使用中。

##### WMKC_NET_ERR_ENOTDIR - [bind]
    路径前缀的组件不是目录

##### WMKC_NET_ERR_ENAMETOOLONG - [bind]
    地址太长

##### WMKC_NET_ERR_EDESTADDRREQ - [send]
    套接字不是连接模式，并且没有设置对等地址。

------

### Windows error

------

##### WMKC_NET_ERR_ENETDOWN - [bind, listen, shutdown, accept, send, recv, closesocket, socket, connect]
    网络子系统发生故障

##### WMKC_NET_ERR_ESHUTDOWN - [send, recv]
    套接字已被shutdown函数关闭

##### WMKC_NET_ERR_EHOSTUNREACH - [send, connect]
    此时无法从此主机访问远程主机

##### WMKC_NET_ERR_NOTINITIALISED - [bind, listen, shutdown, accept, send, recv, closesocket, socket, connect]
    未完成对WSAStartup函数的调用

##### WMKC_NET_ERR_ESOCKTNOSUPPORT - [socket]
    此地址族不支持指定的套接字类型

##### WMKC_NET_ERR_EINVALIDPROVIDER - [socket]
    服务提供商返回了2.2以外的版本

##### WMKC_NET_ERR_EINVALIDPROCTABLE - [socket]
    服务提供程序向WSPStartup返回了无效或不完整的过程表

##### WMKC_NET_ERR_EPROVIDERFAILEDINIT - [socket]
    服务提供程序未能初始化。如果分层服务提供程序（LSP）或命名空间提供程序安装不正确或提供程序无法正常运行则会返回此错误。
