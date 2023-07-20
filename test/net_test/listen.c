#include <network/wmkc_net.h>
#include <wmkc_struct.h>
#include <wmkc_hash.h>
#include <wmkc_file.h>

#include <zlib/zlib.h>

#define DEFAULT_PORT 49281

WMKC_PUBLIC(wmkcVoid) readChunk WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBuf **buf, wmkcNetSize *size))
{
    wmkcNetBuf *p = wmkcNull;
    wmkcNetSize totalLengthReceived;
    wmkcNetSize _tSize = 0;
    wmkcByte size_array[4];
    wmkcByte crc32_array[4];
    wmkc_u32 crc32_n;

    wmkcNet_recv(obj, wmkcNull, size_array, 4);
    wmkcStruct_unpack(">I", size, size_array);
    totalLengthReceived = *size;

    if(!wmkcMem_new(wmkcNetBuf *, (*buf), totalLengthReceived + 1)) {
        printf("为buf申请内存失败。\n");
        return;
    }
    (*buf)[totalLengthReceived] = 0x00;

    p = (*buf);
    while(totalLengthReceived) {
        wmkcNet_recv(obj, &_tSize, p, wmkc_min(totalLengthReceived, 4096));
        p += _tSize;
        totalLengthReceived -= _tSize;
    }
    wmkcNet_recv(obj, wmkcNull, crc32_array, 4);
    wmkcStruct_unpack(">I", &crc32_n, crc32_array);

    if(crc32(0, (*buf), *size) != crc32_n) {
        wmkcMem_free((*buf));
        return;
    }
}

WMKC_PUBLIC(wmkcVoid) writeChunk WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBuf *buf, wmkcNetSize size))
{
    wmkcByte size_array[4];
    wmkcByte crc32_array[4];

    wmkcStruct_pack(">I", size_array, size);
    wmkcStruct_pack(">I", crc32_array, crc32(0, buf, size));

    wmkcNet_send(obj, wmkcNull, size_array, 4);
    wmkcNet_sendall(obj, buf, size);
    wmkcNet_send(obj, wmkcNull, crc32_array, 4);
}

WMKC_PUBLIC(wmkc_s32) main WMKC_OPEN_API
WMKC_OF((wmkc_s32 argc, wmkcChar **argv))
{
#   if defined(WMKC_PLATFORM_WINOS)
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    wmkcNet_obj *sockfd = wmkcNull;
    wmkcNet_obj *client_sockfd = wmkcNull;

    printf("创建两个套接字...\n");
    wmkcNet_new(&sockfd, wmkcNull, AF_INET, false);
    wmkcNet_new(&client_sockfd, wmkcNull, AF_INET, false);

    printf("初始化监听端套接字...\n");
    wmkcNet_init(sockfd, "0.0.0.0", DEFAULT_PORT);
    wmkcNet_bind(sockfd, 1);
    wmkcNet_listen(sockfd, 1);
    printf("等待客户端连接...\n");
    wmkcNet_accept(client_sockfd, sockfd);

    // printf("正在向客户端发送数据...\n");
    // writeChunk(client_sockfd, (wmkcNetBuf *)"TEST_WMKC_CHUNK!", 16);

    printf("正在从客户端接收数据...\n");
    wmkcHash_obj *hash = wmkcNull;
    wmkcNetBuf *buf = wmkcNull;
    wmkcNetSize size = 0;
    readChunk(client_sockfd, &buf, &size);
    // printf("收到客户端发送的数据：%s\n", buf);
    printf("收到客户端发送的数据长度: %u\n", size);

    wmkcHash_new(&hash, wmkcHash_SHA256);
    wmkcHash(hash, buf, size);
    printf("sha256: %s\n", hash->hexdigest);

    printf("完成，关闭套接字。\n");
    wmkcNet_close(client_sockfd);
    wmkcNet_close(sockfd);

#   if defined(WMKC_PLATFORM_WINOS)
    WSACleanup();
#   endif
    return 0;
}
