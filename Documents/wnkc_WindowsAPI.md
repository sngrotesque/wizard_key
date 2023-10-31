# Windows API

[windows中操作文件和目录的函数](https://www.cnblogs.com/phpzhou/p/6050734.html)

## 1. 文件操作函数
  - `CreateFile();`     **// 创建或打开文件**
  - `ReadFile();`       **// 从文件读**
  - `WriteFile();`      **// 向文件写**
  - `SetFilePointer();` **// 设置文件指针**
  - `CopyFile();`       **// 复制文件**
  - `MoveFile();`       **// 移动或重命名文件**
  - `DeleteFile();`     **// 删除文件**

## 2. 文件属性函数
  - `GetFileSize();`         **// 获取文件大小**
  - `GetFileTime();`         **// 获取文件或目录的时间信息，如创建时间、最后修改时间、最后访问时间**
  - `SetFileTime();`         **// 设置文件或目录的时间信息**
  - `GetFileAttributesEx();` **// 获取文件属性**
  - `SetFileAttributes();`   **// 设置文件属性**

## 3. 文件的遍历和查找
  - `FindFirstFile();` **// 查找第一个文件**
  - `FindNextFile();`  **// 查找下一个文件**
  - `FindClose();`     **// 关闭查找**

## 4. 目录操作函数
  - `CreateDirectory();` **// 创建目录，只能创建一层目录**
  - `RemoveDirectory();` **// 删除一个空目录**

## 5. 判断文件或目录是否存在

C中函数`access()`可以用来判断文件或文件夹是否存在。
函数原型： `int access(const char *filename, int mode);`

所属头文件：`io.h`
`filename`：**可以填写文件夹路径或者文件路径**
`mode`:
  - `0 (F_OK)` **只判断是否存在**
  - `2 (R_OK)` **判断写入权限**
  - `4 (W_OK)` **判断读取权限**
  - `6 (X_OK)` **判断执行权限**

> 用于判断文件夹是否存在的时候，mode取0，判断文件是否存在的时候，mode可以取0. 2. 4. 6。
> 若存在或者具有权限，返回值为0。不存在或者无权限，返回值为-1。

#### **错误代码**
  - `EACCESS`      **参数pathname 所指定的文件不符合所要求测试的权限**
  - `EROFS`        **欲测试写入权限的文件存在于只读文件系统内**
  - `EFAULT`       **参数pathname指针超出可存取内存空间**
  - `EINVAL`       **参数mode 不正确**
  - `ENAMETOOLONG` **参数pathname太长**
  - `ENOTDIR`      **参数pathname为一目录**
  - `ENOMEM`       **核心内存不足**
  - `ELOOP`        **参数pathname有过多符号连接问题**
  - `EIO`          **I/O 存取错误**

> 特别提醒：使用access()作用户认证方面的判断要特别小心，例如在access()后再做open()的空文件
> 可能会造成系统安全上的问题。在windows平台下还可以用API函数FindFirstFile()来判断文件或
> 文件夹是否存在。

## 6. 获取指定的目录
  - `GetCurrentDirectory();`        **// 获得当前目录**
  - `SetCurrentDirectory();`        **// 设置当前目录**
  - `GetSystemDirectory();`         **// 获得Windows系统目录**
  - `GetWindowsDirectory();`        **// 获得Windows目录**
  - `GetTempPath();`                **// 获取Windows临时文件目录**
  - `GetFullPathName();`            **// 获取指定文件的全路径**
  - `SHGetSpecialFolderLocation();` **// 函数可以获取Windows桌面目录、启动目录、我的文档目录等**

> 第一个参数为“所有者窗口”，调用这个函数的时候可能出现对话框或消息框。
> 第二个参数是一个整数id，决定哪个目录是待查找目录，它的取值可能是：

  - `CSIDL_BITBUCKET` **回收站**
  - `CSIDL_CONTROLS`  **控制面板**
  - `CSIDL_DESKTOP`   **Windows桌面Desktop**
  - `CSIDL_DRIVES`    **我的电脑**
  - `CSIDL_FONTS`     **字体目录**
  - `CSIDL_NETHOOD`   **网上邻居**
  - `CSIDL_NETWORK`   **网上邻居虚拟目录**
  - `CSIDL_PERSONAL`  **我的文档**
  - `CSIDL_PRINTERS`  **打印机**
  - `CSIDL_PROGRAMS`  **程序组**
  - `CSIDL_RECENT`    **最近打开文档**
  - `CSIDL_SENDTO`    **发送到"菜单项"**
  - `CSIDL_STARTMENU` **任务条启动菜单项**
  - `CSIDL_STARTUP`   **启动目录**
  - `CSIDL_TEMPLATES` **临时文档**

> 第三个参数为pidl地址. SHGetSpecialFolderLocation把地址写到pidl

## 7. 缩写的全路径和不缩写的全路径
  - `GetLongPathName();`  **// 将文件全路径转换为不缩写的全路径**
  - `GetLongShortName();` **// 将文件全路径转换为缩写的全路径**

> 这两个函数极其重要！！！很多人写程序时不注意这一点——包括我。
> 比如，使用GetModuleFileName获取文件路径，我们一般都将获取到的路径直接拿来用。
> 在99.9％的情况下，我们获取到的是没有缩写的全路径。
> 但是，某些情况下，获取到的却是缩写的，例如：C:\Prog~1\Comm~1\aaa~1.exe。
> 如果这个时候，取进程的名称，将得到aaa~1.exe，很明显，这种结果是我们所不愿意看到的。
> 它会导致很严重的问题，而这种错误又是极其难以查找的。
> 所以，个人认为，在获取文件全路径后，都应该调用GetLongPathName，以便保证得到的路径不是缩写形式。

## 8. C/C++中文件、目录操作函数
  - `remove();` **// 删除一个文件或目录**
  - `rename()`; **// 重命名或移动(如果系统支持的话)一个文件或目录**

------------------------------------------------------------------------------

[CreateFileW](https://learn.microsoft.com/zh-cn/windows/win32/api/fileapi/nf-fileapi-createfilew)
```c
HANDLE CreateFileW(
    /* 文件路径 */
    LPCWSTR lpFileName,

    /* 请求对文件或设备的访问权限，可汇总为读取、写入或两者均不) 
    *      GENERIC_ALL       所有可能的权限
    *      GENERIC_EXECUTE   执行权限
    *      GENERIC_READ      读取权限
    *      GENERIC_WRITE     写入权限*/
    DWORD dwDesiredAccess,

    /* 文件或设备请求的共享模式，可以读取、写入、删除、所有这些或无 (引用下表) 。
    *      0x00000000           如果文件或设备请求删除、读取或写入访问权限。
    *                           则阻止对文件或设备上的后续打开操作。
    *      FILE_SHARE_DELETE    在文件或设备上启用后续打开操作以请求删除访问权限。
    *      0x00000004           否则，如果文件或设备请求删除访问权限，则无法打开该文件或设备。
    *                           如果未指定此标志，但文件或设备已打开以删除访问权限，则函数将失败。
    *                           注意: 删除访问权限允许删除和重命名操作。
    *      FILE_SHARE_READ      在文件或设备上启用后续打开操作以请求读取访问权限。
    *      0x00000001           否则，如果文件或设备请求读取访问权限，则无法打开该文件或设备。
    *                           如果未指定此标志，但文件或设备已打开以供读取访问，则函数将失败。
    *      FILE_SHARE_WRITE     在文件或设备上启用后续打开操作以请求写入访问权限。
    *      0x00000002           否则，如果文件或设备请求写入访问权限，则无法打开该文件或设备。
    *                           如果未指定此标志，但文件或设备已打开进行写入访问或具有写入访问
    *                           权限的文件映射，则函数将失败。*/
    DWORD dwShareMode,
    
    /* 指向包含两个独立但相关数据成员的 SECURITY_ATTRIBUTES 结构的指针：
    *  可选的安全描述符，以及一个布尔值，该值确定返回的句柄是否可以由子进程继承。
    *  此参数可以为 NULL。
    *  如果此参数为 NULL，则 由 CreateFile 返回的句柄不能由应用程序可能创建的任何子进程继承，
    *  并且与返回的句柄关联的文件或设备获取默认的安全描述符。
    *  结构的 lpSecurityDescriptor 成员指定文件或设备的 SECURITY_DESCRIPTOR 。 
    *  如果此成员为 NULL，则会为与返回的句柄关联的文件或设备分配一个默认的安全描述符。*/
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    
    /* 对存在或不存在的文件或设备执行的操作。
    *      CREATE_ALWAYS     始终创建新文件。
    *      2                 如果指定的文件存在且可写，则函数将覆盖文件、函数成功，
    *                        最后错误代码设置为 ERROR_ALREADY_EXISTS (183) 。
    *                        如果指定的文件不存在且路径有效，则会创建一个新文件，
    *                        该函数成功，最后一个错误代码设置为零。
    *      CREATE_NEW        仅当该文件尚不存在时才创建一个新文件。
    *      1                 如果指定的文件存在，函数将失败，最后一个错误代码设置为
    *                        ERROR_FILE_EXISTS ( 80) 。如果指定的文件不存在并且是可写位置的
    *                        有效路径，则会创建新文件。
    *      OPEN_ALWAYS       始终打开文件。
    *      4                 如果指定文件存在，则函数成功，最后一个错误代码设置为
    *                        ERROR_ALREADY_EXISTS ( 183) 。如果指定的文件不存在并且是可写位置的
    *                        有效路径，则函数将创建一个文件，最后一个错误代码设置为零。
    *      OPEN_EXISTING     仅当文件或设备存在时，才打开该文件或设备。
    *      3                 如果指定的文件或设备不存在，函数将失败，最后一个错误代码
    *                        设置为 ERROR_FILE_NOT_FOUND ( 2) 。
    *      TRUNCATE_EXISTING 打开文件并截断该文件，使其大小为零字节，仅当它存在时。
    *      5                 如果指定的文件不存在，函数将失败，最后一个错误代码
    *                        设置为ERROR_FILE_NOT_FOUND (2) 。
    *                        调用过程必须打开文件，并将 GENERIC_WRITE 位
    *                        设置为 dwDesiredAccess 参数的一部分。*/
    DWORD dwCreationDisposition,
    
    /* 太长，请自行前往源网址查看
    * 文件或设备属性和标志， FILE_ATTRIBUTE_NORMAL 是文件最常见的默认值。*/
    DWORD dwFlagsAndAttributes,
    
    /* 具有 GENERIC_READ 访问权限的模板文件的有效句柄。
    *  模板文件为正在创建的文件提供文件属性和扩展属性。
    *  此参数可以为 NULL。
    *  打开现有文件时， CreateFile 将忽略此参数。*/
    HANDLE hTemplateFile
);
```





