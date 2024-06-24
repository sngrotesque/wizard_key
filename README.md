# Wizard Magic Key Cyber
> `Wizard Magic Key Cyber` by SN-Grotesque

此库已在以下环境下测试通过：
1. `Ubuntu/Debian`系Linux操作系统 - GCC套件
2. `Windows 10` - MinGW64 (MSYS2) - Visual Studio (2019~2022)

### Author(s):
 - [SN-Grotesque](https://github.com/sngrotesque)
 - [R7](https://github.com/r7z7)

### 开放源代码许可（Open Source License）
 - [OpenSSL](https://github.com/openssl/openssl) - 使用OpenSSL的各种算法库
 - [libpng](http://libpng.org/pub/png/) - 未来将用于PNG图像的读取和写入
 - [Python](https://github.com/python/cpython) - 参考并借鉴了Python的C源代码（包括语法和部分功能的实现），可以说我的开发者生涯都深受Python的影响。
 - [cJSON](https://github.com/DaveGamble/cJSON/) - 前期将使用cJSON来读取和写入json数据，后期再考虑是否使用更复杂的库
 - [Zlib](https://github.com/madler/zlib) - 进行CRC32校验与后续可能的压缩功能
 - [iconv](https://www.gnu.org/software/libiconv/) - 作为编码转换时使用的库，后续考虑自行实现（如果可能）

### 提问与解答（Q&A）
**Q**：为什么想开发这个库？  
A：因为我在未来会制作一个属于自己的即时通讯软件，想以这个库为基础进行开发。

**Q**：为什么使用C++？  
A：其实最开始使用的是C，但是后来因为一些功能的实现难度，我最终考虑使用了C++，至于为什么不是C#或者Java以及Python，那是因为出于[性能](https://goodmanwen.github.io/Programming-Language-Benchmarks-Visualization/)考虑。

**Q**：有些功能，目前市面上一些库已经包含了，为什么要自主开发一个类似的？  
A：人嘛，有时候总是想要有一些自己能掌握的东西，自己决定更新进度，同时也可以学习这些功能的底层实现原理，其实说白了就是可以自己定义很多市面上的库没有的功能。

**Q**：为什么没有`master`这种主分支？  
A：在功能还没完全成型以及Bug还没排除掉绝大多数的情况下，我并不想污染主分支。

**Q**：为什么没有明确支持`Mac OS`系统？  
A：因为我从未真正使用过`Mac OS`[^macos]，并且也不熟悉它的操作和工作原理，不过正常来说，既然它是[POSIX标准](https://pubs.opengroup.org/onlinepubs/9699919799/)[^what_is_posix]的产物，那其实应该也是支持的。

### 关于（About）
1.  此库拥有多套加密算法，其中包含了[FEA（Fast encryption algorithm）](includes/crypto/fea.hpp)，由[SN-Grotesque](https://github.com/sngrotesque)自制研发的一款分组对称加密算法。更多关于此算法的描述请[查看](Reference.md#L53)。
2.  此库还在逐步不断完善中，缺少开发者的支持，如果你有能力，请和我一起开发这个库，非常感谢。

### 使用（Usage）
1.  此库仅支持64位操作系统（Windows，Linux），请不要在使用32位操作系统的情况下使用此库，否则可能会出现意想不到的情况。
2.  对于`MingW64`与`GCC套件`的使用，可以直接调用根目录的[run.py](run.py)代码来执行（功能基本上等同于make）。  
    而对于`Visual Studio`，需要自行将库编译为dll文件或者在主程序代码文件中包含`*.cpp`文件。  
    未来会使用[CMakeList.txt (Cmake)](https://cmake.org/)来强化编译步骤。

### 开发文档（Developer's guide）
 -  开发标准：
>   C++ standard >= `C++11`，同时尽可能避免使用`C++20`及以上标准的语法和头文件[^no_cpp20]。
 -  代码风格：
>   每行不超过90个字符，注释请添加在可能会引起困惑或误会的部分。  
>   对于类型的初始化，只使用两种初始化方法，第一种是[int a = 0](https://en.cppreference.com/w/cpp/language/copy_initialization)，第二种是[int a{0};](https://en.cppreference.com/w/cpp/language/list_initialization)。~~其实我不应该加引用的，毕竟这都看不懂还看什么。~~
 -  代码层级：
>   尽可能的少，如果可以，请按照Linux开发标准中建议的最多3层嵌套来编写代码[^linux_coding_style]。  
由于C++不同于C，很多语法不能完全按照C的语法来实现，针对于嵌套层级这点不强求。

### 版本更新日志（Change log）
> 修改了版本后需要更改的文件  
> [upload.py](upload.py)  
> [READMKE.md](README.md)

#### v0.7.2
1.  修复了[Base64解码函数](includes/base64.hpp)在`v0.7.1`版本中的BUG，并修复了`v0.7.1`版本中解码函数未针对填充符进行处理的问题，现已经可以使用。  
    ~~但目前并未完全按照[RFC4648](https://datatracker.ietf.org/doc/html/rfc4648)实现，请等待后续的进一步完善。~~  
    使用Python中的方法来完善了Base64的解码功能，但存在一个处理受污染Base64编码串的小问题。  
    即：最终指针实际使用的内存空间会比解码后应实际占用的内存空间略大（这取决于Base64编码串被污染的程度），将在后续版本中修复此问题。  
    ~~目前又发现一个问题，一种极为特殊的被污染的Base64编码串[^?base64_encoded]会导致直接的内存泄露或者程序闪退。~~
    问题解决，问题的根源是`bin_data`指针指向错误，应使用`bin_data_start`指针来释放内存。**妈的终于把心头的一个大石头移开了，我就说这段时间怎么感觉胸闷。**
2.  提供了用于[公开库的API调用](includes/config/wmkc.hpp#L36)，现在可以更好的支持编译器导入和导出库函数了。  
    请在编译时加入宏**WMKC_EXPORTS**，否则很可能将编译失败。
3.  已修复一些编译器的警告项。
4.  此版本将着重检查和修复一些可能的隐患。
5.  完善[密码库的计数器](includes/crypto/utils.hpp#L9)。

#### v0.7.1
1.  重写了wmkc::Exception（抛弃原先的函数调用std::runtime_error的形式），将其作为一个完整的异常类使用，继承于std::exception。
2.  增加了对于wmkc::crypto::fea::ctr_xcrypt的更多支持，现在请使用counter类来构建一个计数器。
3.  将所有类的名称使用大驼峰命名法，跟随编码规范。
4.  ~~更新了[终端字体颜色库](includes/config/color.hpp)对于C++的支持，抛弃了原先的C语法。~~
5.  将Python代码移动到了[WMKC_for_Python](https://github.com/sngrotesque/WMKC_Python)仓库，如果要使用，请去对应仓库查看，谢谢。
6.  添加了[ChaCha20](https://github.com/marcizhu/ChaCha20)加密算法的实现，如果你要使用其他实现，请自行调用（如OpenSSL库提供的ChaCha20）。
7.  **如果使用此库中提供的[Base64](includes/base64.hpp)进行解码操作，有可能会导致内存溢出或内存泄漏甚至更严重的后果，请不要使用！！！**  
    将重新实现有关Base64编码中的解码函数，目前发现了一个问题，具体描述：  
    在[base64.cpp 第93行](sources/base64.cpp#L93)与[base64.cpp 第94行](sources/base64.cpp#L94)，将值与解码表进行置换后，值甚至会大于`800`，并且  
    让这个值与`0xff`进行与运算`v & 0xff`后依旧无法得出正确的值。  
    这个问题是我在使用Python进行此库的重新实现时发现的，不确定是否为Python的问题，但是此库会进行检查和修复，并重新实现，并且将完全按照规范进行编写。  
    此Base64算法的最初实现是[CSDN - C语言实现Base64编码/解码](https://blog.csdn.net/qq_26093511/article/details/78836087)，估计就是这个时候就  
    留下了隐患，当初是直接拿这个代码然后对其进行优化和重新编码后得到目前的Base64版本。

等待添加的功能
- 为socket库增加简易版的setsockopt与getsockopt方法，以及待完善的Socket库和SSL库。（将逐步完成Socket库）

#### v0.7.0
1.  所有C++代码重新组织代码结构，将所有功能列入wmkc命名空间，并将原有的类更名。  
    例如：wmkcBase64变为`wmkc::base64`，其中的方法encode变为`wmkc::base64().encode()`
2.  重新优化所有代码。
3.  修复了FEA加密算法的一个密钥长度隐患。
4.  完成了`wmkc::structure::pack`，但是需要使用者注意`C/C++`的特性，解释在[struct.hpp](includes/struct.hpp)头部。  

#### v0.6.6
1.  C++代码为完全复刻C代码的内容，但是修复了各种问题与难用性。
2.  C代码暂时停止维护，其中存在内存泄露等问题，如需使用，请尽可能使用C++代码。

#### v0.6.5
1.  新增C++代码

#### v0.6.4
1.  重构wmkcNet库与wmkcError结构。
2.  优化了wmkc_obj结构。
3.  优化了wmkc_file库的编码转换功能。
4.  新增了wmkcSSL库用于进行HTTPS通信。
5.  新增了C++代码，以便于实现各种面向对象的功能。

#### v0.6.3
1.  预计将所有实现改为内部实现，然后将函数作为类成员的方式使用（由于效率原因已弃之）。
2.  修复了network/wmkc_net库中对于wmkcNet_timeout函数的实现，  
    傻逼微软你妈死了，弄你妈的[毫秒单位](https://learn.microsoft.com/zh-cn/windows/win32/api/winsock/nf-winsock-setsockopt)啊。

#### v0.6.2
1.  将修改wmkcNet库，以支持一定情况下的OpenSSL组件，以便于进行HTTPS通信。
2.  将完善wmkcFile库，更好的支持对于不同操作系统的文件操作函数。
3.  将针对所有WMKC库进行优化。

#### v0.6.1
1.  将wmkc_chat库中的对象指针成员修改为数组成员，  
    否则会导致无意义的代码过长。
2.  重构了wmkc_file库，删除了wmkcFile对象。
3.  放弃使用icu4c库对编码进行转换，改用iconv库。
4.  小幅度修改了wmkc_pad，wmkc_base64库。
5.  删除了wmkcFloat，wmkcFile类型，多余而且扰乱思绪。
6.  将wmkcMisc_PRINT_RAW函数修改为了类似Python中的print bytes类型的实现。

#### v0.6.0
1.  将wmkc_base修改为wmkc_base64，因考虑到  
    不会添加base16（因为此编码可以使用wmkc_binascii库实现）  
    不会添加Base32（因为此编码实在没有必要添加）  
    不会添加Base85（因为此编码（又称Ascii85）目前还没有一个完美的标准）  
    并将Base64的编码与解码库重新设计，去掉了基于wmkc对象的形式。
2.  添加了wmkc_dict库，更好的操作键值对类型的数据。
3.  修改了关于颜色的宏。
4.  修复了wmkc_misc库的BUG。
5.  部署了libpng库，重新构建了zlib库文件。
6.  修复了wmkc_stream库中wmkc_scanf函数的隐藏BUG。
7.  将wmkcStream库中的wmkcStream_scanf函数移动到wmkc_common库并更名为wmkc_scanf。

#### v0.0.51
1.  修改了wmkc_random，wmkc_time库的实现。
2.  修复了wmkc_base库的错误信息不全的问题。
3.  修改了对于系统宏的名称标识。
4.  增加了适当的文档与注释。
5.  增加了wmkc_thread库用于多线程编程。

#### v0.0.50
1.  将库更名为Wizard Magic Key Cyber，原因是与目前某一组织名称重合。
2.  将库函数与一般变量类型与宏函数做了更名。
3.  修改了一些针对不同操作系统的支持。

[^macos]: [Is Mac OS X a POSIX OS?](https://stackoverflow.com/questions/5785516/is-mac-os-x-a-posix-os), [OS X for UNIX Users](https://images.apple.com/media/us/osx/2012/docs/OSX_for_UNIX_Users_TB_July2011.pdf), [Programming MacOS-X and the Linux API - POSIX compatible?](https://stackoverflow.com/questions/9013717/programming-macos-x-and-the-linux-api-posix-compatible)

[^what_is_posix]: [What is the meaning of "POSIX"?](https://stackoverflow.com/questions/1780599/what-is-the-meaning-of-posix), [POSIX - Wiki](https://en.wikipedia.org/wiki/POSIX)

[^no_cpp20]: 有些Linux系统搭配的GCC套件不完全支持C++20标准，你总不能让人去更换自己熟悉的开发环境吧？

[^linux_coding_style]: [Linux 内核代码风格](https://www.kernel.org/doc/html/latest/translations/zh_CN/process/coding-style.html)

[^?base64_encoded]: "UwU//"