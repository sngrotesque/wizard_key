#include "ftp.hpp"

void reverse(wByte *array, wU32 size)
{
    wByte swap;
    for(wU32 i = 0; i < (size >> 1); ++i) {
        swap = *(array + i);
        *(array + i) = *(array + (size - i - 1));
        *(array + (size - i - 1)) = swap;
    }
}

template <typename T>
void ftp_send(T file_path, std::string addr, wU16 port)
{
    std::fstream f_obj(file_path, std::ios::binary | std::ios::in);
    if(!f_obj.is_open()) {
        throw wuk::Exception(wukErr_Err, "ftp_send",
                            "failed to file open.");
    }
    wByte length_BYTES[8]{};
    wSize length;

    printf("获取文件长度。\n");
    // 获取文件长度
    f_obj.seekg(0, std::ios::end);
    length = f_obj.tellg();
    f_obj.seekg(std::ios::beg);

    printf("将数字转为大端序字节串。\n");
    // 将数字转为大端序字节串
    memcpy(length_BYTES, &length, sizeof(length_BYTES));
    if(WUK_LE_ENDIAN) {
        reverse(length_BYTES, sizeof(length_BYTES));
    }

    try {
        printf("创建套接字。\n");
        wuk::net::Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        printf("与目标主机进行连接。\n");
        fd.connect(addr, port);

        printf("发送长度信息给对方。\n");
        // 发送文件长度信息给接收方
        fd.send(std::string((char *)length_BYTES, 8));

        printf("发送完整文件给对方。\n");
        char buffer[2048];
        wSize tmp_len;
        for(;;) {
            wuk::memory_zero(buffer, sizeof(buffer));
            tmp_len = f_obj.read(buffer, sizeof(buffer)).gcount();
            if(!tmp_len) {
                break;
            }
            fd.sendall(std::string(buffer, tmp_len));
        }

        printf("关闭套接字。\n");
        fd.shutdown(2);
        fd.close();
    } catch(wuk::Exception &e) {
        f_obj.close();
        printf("Error: %s\n", e.what());
        return;
    }

    printf("关闭文件。\n");
    f_obj.close();
}

template <typename T>
void ftp_recv(T file_path, std::string addr, wU16 port)
{

}

