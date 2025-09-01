# Changelog

所有显著变更将记录在此文件中。

## v0.9 更新内容

1. 重新以现代化的方式优化了整体代码并移除了部分“远古”代码。
2. 设计了 [OP4 (four fundamental operations)](wuk/includes/crypto/WukOP4.hh) 加密算法。
3. 重新以现代化的标准封装了Socket类（用于未来IM的开发以及彻底替代原先`v0.8`版本时期的屎山代码）。  
   采用 [Protobuf](https://protobuf.dev/) 实现了网络包模块。
4. 以通用方式封装了 [OpenSSL](https://www.openssl.org/) 库提供的哈希算法工具函数，用于更方便的调用哈希算法。  
   当前支持以下哈希算法：  
   | Algorithm | subdivision | Support. | Recommended |
   | --------- | ----------- | -------- | ----------- |
   | MD5       | MD5         | Yes      | No          |
   | SHA-1     | SHA-1       | Yes      | No          |
   | SHA-2     | SHA-224     | Yes      | Yes         |
   |           | SHA-256     | Yes      | Yes         |
   |           | SHA-384     | Yes      | Yes         |
   |           | SHA-512     | Yes      | Yes         |
   | SHA-3     | SHA3-224    | Yes      | Yes         |
   |           | SHA3-256    | Yes      | Yes         |
   |           | SHA3-384    | Yes      | Yes         |
   |           | SHA3-512    | Yes      | Yes         |
5. 对于加密方式，未来网络传输时将采用 **OpenSSL** 库所提供的公钥密码/密钥交换算法，  
   对称加密算法将采用 **OP4** 或 **ChaCha20-Poly1305 (Base on Libsodium, Not OpenSSL)** 或 **AES-256-GCM** 。  
   目前将 **ChaCha20-Poly1305 (RFC 8439)** 作为主要加密算法。
6. 在未来的版本将完全支持 **wuk::net::WukSocket** 库的IO多路复用功能；  
   将利用包括但不限于 **select / poll / epoll / IOCP** 等技术实现。
7. 修复了[wuk::Buffer](wuk/includes/WukBuffer.hh)中对于内存空间的使用问题（即`wuk::Buffer::expand_memory`会过多申请所需内存空间的问题）以及部分潜在的内存泄漏风险。

## v0.8
1.  修改了代码文件的格式，以防止使用时与其他头文件冲突。
2.  ~~逐步将所有`new`方法改为C方法的`malloc`函数，后续考虑是否加入智能指针。~~
3.  修复了关于初始化列表中的一个可能存在的指针指向错误的问题。
4.  优化了wuk::net中的代码。
5.  将网络库中的使用堆空间的指针全部改为[wuk::Buffer](wuk/includes/WukBuffer.hh)。  
    暂时不会考虑使用智能指针，因其无法达成我的要求。
6.  完善了[wuk::Buffer](wuk/includes/WukBuffer.hh)模块的功能，未来也许会添加更多功能。
7.  完善并优化了一些细节与修复了一个空指针指向问题。
8.  优化了代码。
9.  修复了流密码算法[wuk::crypto::SSC](wuk/includes/crypto/WukSSC.hh)的弱点。

## v0.7.2
1.  修复了[Base64解码函数](wuk/includes/base64.hpp)在`v0.7.1`版本中的BUG，并修复了`v0.7.1`版本中解码函数未针对填充符进行处理的问题，现已经可以使用。  
    ~~但目前并未完全按照[RFC4648](https://datatracker.ietf.org/doc/html/rfc4648)实现，请等待后续的进一步完善。~~  
    使用Python中的方法来完善了Base64的解码功能，但存在一个处理受污染Base64编码串的小问题。  
    即：最终指针实际使用的内存空间会比解码后应实际占用的内存空间略大（这取决于Base64编码串被污染的程度），将在后续版本中修复此问题。  
    ~~目前又发现一个问题，一种极为特殊的被污染的Base64编码串会导致直接的内存泄露或者程序闪退。~~
    问题解决，问题的根源是`bin_data`指针指向错误，应使用`bin_data_start`指针来释放内存。**妈的终于把心头的一个大石头移开了，我就说这段时间怎么感觉胸闷。**
2.  提供了用于[公开库的API调用](wuk/includes/config/wuk.hpp#L36)，现在可以更好的支持编译器导入和导出库函数了。  
    请在编译时加入宏**WUK_EXPORTS**，否则很可能将编译失败。
3.  已修复一些编译器的警告项。
4.  此版本将着重检查和修复一些可能的隐患。
5.  完善了[密码库的计数器](wuk/includes/crypto/utils.hpp#L9)。
6.  完善了部分[SSL库](wuk/includes/network/ssl.hpp)的功能。
7.  完善了[Binascii](wuk/includes/binascii.hpp)，修复了[Base64](sources/base64.cpp)中的异常捕获隐患。
8.  修改并完善了[WUK库的代码和宏定义](wuk/includes/config/wuk.hpp)，以便未来更好的支持各个平台以及各个编译器。
9.  逐渐将所有以前的C语法改为C++语法。
10. 优化了[make.py](make.py)的代码。

## v0.7.1
1.  重写了wuk::Exception（抛弃原先的函数调用std::runtime_error的形式），将其作为一个完整的异常类使用，继承于std::exception。
2.  增加了对于wuk::crypto::fea::ctr_xcrypt的更多支持，现在请使用counter类来构建一个计数器。
3.  将所有类的名称使用大驼峰命名法，跟随编码规范。
4.  ~~更新了[终端字体颜色库](wuk/includes/config/color.hpp)对于C++的支持，抛弃了原先的C语法。~~
5.  添加了[ChaCha20](https://github.com/marcizhu/ChaCha20)加密算法的实现，如果你要使用其他实现，请自行调用（如OpenSSL库提供的ChaCha20）。
6.  **如果使用此库中提供的[Base64](wuk/includes/base64.hpp)进行解码操作，有可能会导致内存溢出或内存泄漏甚至更严重的后果，请不要使用！！！**  
    将重新实现有关Base64编码中的解码函数，目前发现了一个问题，具体描述：  
    在[base64.cpp 第93行](sources/base64.cpp#L93)与[base64.cpp 第94行](sources/base64.cpp#L94)，将值与解码表进行置换后，值甚至会大于`800`，并且
    让这个值与`0xff`进行与运算`v & 0xff`后依旧无法得出正确的值。  
    这个问题是我在使用Python进行此库的重新实现时发现的，不确定是否为Python的问题，但是此库会进行检查和修复，并重新实现，并且将完全按照规范进行编写。  
    此Base64算法的最初实现是[CSDN - C语言实现Base64编码/解码](https://blog.csdn.net/qq_26093511/article/details/78836087)，估计就是这个时候就
    留下了隐患，当初是直接拿这个代码然后对其进行优化和重新编码后得到目前的Base64版本。

等待添加的功能
- 为socket库增加简易版的setsockopt与getsockopt方法，以及待完善的Socket库和SSL库。（将逐步完成Socket库）

## v0.7.0
1.  所有C++代码重新组织代码结构，将所有功能列入wuk命名空间，并将原有的类更名。  
    例如：wukBase64变为`wuk::base64`，其中的方法encode变为`wuk::base64().encode()`
2.  重新优化所有代码。
3.  修复了FEA加密算法的一个密钥长度隐患。
4.  完成了`wuk::structure::pack`，但是需要使用者注意`C/C++`的特性，解释在[struct.hpp](wuk/includes/struct.hpp)头部。  

## v0.6.6
1.  C++代码为完全复刻C代码的内容，但是修复了各种问题与难用性。
2.  C代码暂时停止维护，其中存在内存泄露等问题，如需使用，请尽可能使用C++代码。

## v0.6.5
1.  新增C++代码

## v0.6.4
1.  重构wukNet库与wukError结构。
2.  优化了wuk_obj结构。
3.  优化了wuk_file库的编码转换功能。
4.  新增了wukSSL库用于进行HTTPS通信。
5.  新增了C++代码，以便于实现各种面向对象的功能。

## v0.6.3
1.  预计将所有实现改为内部实现，然后将函数作为类成员的方式使用（由于效率原因已弃之）。
2.  修复了network/wuk_net库中对于wukNet_timeout函数的实现，  
    傻逼微软你妈死了，弄你妈的[毫秒单位](https://learn.microsoft.com/zh-cn/windows/win32/api/winsock/nf-winsock-setsockopt)啊。

## v0.6.2
1.  将修改wukNet库，以支持一定情况下的OpenSSL组件，以便于进行HTTPS通信。
2.  将完善wukFile库，更好的支持对于不同操作系统的文件操作函数。
3.  将针对所有WUK库进行优化。

## v0.6.1
1.  将wuk_chat库中的对象指针成员修改为数组成员，  
    否则会导致无意义的代码过长。
2.  重构了wuk_file库，删除了wukFile对象。
3.  放弃使用icu4c库对编码进行转换，改用iconv库。
4.  小幅度修改了wuk_pad，wuk_base64库。
5.  删除了wukFloat，wukFile类型，多余而且扰乱思绪。
6.  将wukMisc_PRINT_RAW函数修改为了类似Python中的print bytes类型的实现。

## v0.6.0
1.  将wuk_base修改为wuk_base64，因考虑到  
    不会添加base16（因为此编码可以使用wuk_binascii库实现）  
    不会添加Base32（因为此编码实在没有必要添加）  
    不会添加Base85（因为此编码（又称Ascii85）目前还没有一个完美的标准）  
    并将Base64的编码与解码库重新设计，去掉了基于wuk对象的形式。
2.  添加了wuk_dict库，更好的操作键值对类型的数据。
3.  修改了关于颜色的宏。
4.  修复了wuk_misc库的BUG。
5.  部署了libpng库，重新构建了zlib库文件。
6.  修复了wuk_stream库中wuk_scanf函数的隐藏BUG。
7.  将wukStream库中的wukStream_scanf函数移动到wuk_common库并更名为wuk_scanf。

## v0.0.51
1.  修改了wuk_random，wuk_time库的实现。
2.  修复了wuk_base库的错误信息不全的问题。
3.  修改了对于系统宏的名称标识。
4.  增加了适当的文档与注释。
5.  增加了wuk_thread库用于多线程编程。

## v0.0.50
1.  将库更名为Wizard Magic Key Cyber，原因是与目前某一组织名称重合。
2.  将库函数与一般变量类型与宏函数做了更名。
3.  修改了一些针对不同操作系统的支持。
