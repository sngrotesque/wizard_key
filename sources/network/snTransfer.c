#include <network/snTransfer.h>

SN_PRIVATE(snError) _snTransfer_sendall
SN_FUNC_OF((snNet_ctx *net, snSize fileSize, const snChar *fn))
{
    snNetSize quotient;  // 文件长度除块大小的商
    snNetSize leftover;  // 文件剩余长度
    snNetSize tSize;     // 单次传输长度
    snFile *fp = snNull; // 文件指针

    snNetBuf fileBuf[SN_NET_BLOCKLEN];         // 建立缓冲区用于读取文件数据

    snZeroObject(&fileBuf, SN_NET_BLOCKLEN); // 初始化文件数据缓冲区

    quotient = fileSize / SN_NET_BLOCKLEN;   // 文件长度除块大小的商
    leftover = fileSize % SN_NET_BLOCKLEN;   // 文件剩余长度

    fp = fopen(fn, "rb");
    for(snNetSize i = 0; i < quotient; ++i) {
        fread(fileBuf, 1, SN_NET_BLOCKLEN, fp);
        snNet_send(net, &tSize, fileBuf, SN_NET_BLOCKLEN);
    }
    if(leftover) {
        fread(fileBuf, 1, leftover, fp);
        snNet_send(net, &tSize, fileBuf, leftover);
    }

    fclose(fp);
    return snErr_OK;
}

SN_PRIVATE(snError) _snTransfer_recvall
SN_FUNC_OF((snNet_ctx *net, snSize fileSize, const snChar *fn))
{
    snNetSize tSize;     // 单次传输长度
    snFile *fp = snNull; // 文件指针

    snNetBuf fileBuf[SN_NET_BLOCKLEN];         // 建立缓冲区用于读取文件数据
    snZeroObject(&fileBuf, SN_NET_BLOCKLEN); // 初始化文件数据缓冲区

    fp = fopen(fn, "wb");

    for(; fileSize; fileSize -= tSize) {
        snNet_recv(net, &tSize, fileBuf, SN_NET_BLOCKLEN);
        fwrite(fileBuf, 1, tSize, fp);
    }

    fclose(fp);
    return snErr_OK;
}

SN_PUBLIC(snError) snTransfer_new SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx **ctx, const snChar *addr, sn_u16 port, sn_u32 maxRetry))
{
    if(!((*ctx) = (snTransfer_ctx *)malloc(sizeof(snTransfer_ctx)))) {
        return snErr_Memory;
    }
    if(!addr) {
        return snErr_ErrNullData;
    }
    (*ctx)->addr = addr;
    if(port) {
        (*ctx)->port = port;
    } else {
        (*ctx)->port = SN_FT_DEFAULT_PORT;
    }
    if(maxRetry) {
        (*ctx)->maxRetry = maxRetry;
    } else {
        (*ctx)->maxRetry = SN_FT_DEFAULT_MAX_RETRY;
    }
    (*ctx)->snc = snNull;
    // SNC_new(&(*ctx)->snc, SNC_256);
    return snErr_OK;
}

SN_PUBLIC(snError) snTransfer_release SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx **ctx))
{
    if((*ctx)->snc) {
        SNC_release(&(*ctx)->snc);
    }
    free((*ctx));
    (*ctx) = snNull;
    return snErr_OK;
}

SN_PUBLIC(snError) snTransfer_Listen SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx *ctx, const snChar *fn))
{
    if(!ctx->addr || !ctx->port) {
        return snErr_ErrNullData;
    }
    if(!ctx->maxRetry) {
        ctx->maxRetry = SN_FT_DEFAULT_MAX_RETRY;
    }

    snHash_ctx *hash = snNull; // snHash对象
    snNet_ctx *net = snNull;   // snHash对象
    snNetBuf ClientHash[32];   // 临时存放客户端发送的哈希值
    snNetBuf fSizeByte[8];     // 文件长度数组
    snNetSize tSize;           // 单次传输长度
    snSize fileSize;           // 文件长度数字
    snError code = 0;          // 错误代码

    printf("初始化程序中...\n");
    snZeroObject(fSizeByte, 8);               // 初始化fileSizeBytes数组
    snHash_new(&hash, SN_HASH_TYPE_SHA256);   // 为snHash对象申请内存空间
    snNet_new(&net, AF_INET);                 // 为snNet对象申请内存空间
    snNet_init(net, ctx->addr, ctx->port, 0); // 初始化snNet对象
    snNet_timeout(net, SN_FT_SDRV_TO);        // 设置超时时间

    if((code = snNet_bind(net, 1)))           // 绑定本机
        return code;
    if((code = snNet_listen(net, 5)))         // 开启监听
        return code;
    if((code = snNet_accept(net)))            // 等待客户端连接
        return code;

    /*************************************************/
    SOCKADDR_IN *ipv4 = snNull;
    ipv4 = (SOCKADDR_IN *)net->info->client;
    printf("客户端已连接：%s:%d\n",
        snNet_GetAddr(ipv4->sin_addr),
        snNet_GetPort(ipv4->sin_port));
    /*************************************************/

    snNet_recv(net, &tSize, fSizeByte, 8);
    snNet_recv(net, &tSize, ClientHash, 32);
    memcpy(&fileSize, fSizeByte, 8);
    printf("文件长度：%"PRIu64"\n", fileSize);
    printf("文件哈希：");
    for(int x = 0; x < 32; ++x) {
        printf("%02x", ClientHash[x]);
    }
    printf("\n");

    _snTransfer_recvall(net, fileSize, fn);

    snNet_send(net, &tSize, (snNetBuf *)&SN_FT_SIGNAL_DONE, 4);

    snNet_close(net);
    snNet_release(&net);
    snHash_release(&hash, SN_RELEASE_NORMAL);

    return snErr_OK;
}

SN_PUBLIC(snError) snTransfer_Client SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx *ctx, const snChar *fn))
{
    if(!snFile_exists(fn))
        return snErr_FileFolderPath;
    if(!ctx->addr || !ctx->port) {
        return snErr_ErrNullData;
    }
    if(!ctx->maxRetry) {
        ctx->maxRetry = SN_FT_DEFAULT_MAX_RETRY;
    }

    snHash_ctx *hash = snNull; // snHash对象
    snNet_ctx *net = snNull;   // snHash对象
    snNetBuf listenMsg[4];     // 接收端返回的信息
    snNetBuf fSizeByte[8];       // 文件长度数组
    snNetSize tSize;           // 单次传输长度
    snSize fileSize;           // 文件长度数字
    snError code = 0;          // 错误代码

    printf("初始化程序中...\n");
    snZeroObject(fSizeByte, 8);               // 初始化fileSizeBytes数组
    snZeroObject(listenMsg, 4);               // 初始化Message数组
    snHash_new(&hash, SN_HASH_TYPE_SHA256);   // 为snHash对象申请内存空间
    snNet_new(&net, AF_INET);                 // 为snNet对象申请内存空间
    snNet_init(net, ctx->addr, ctx->port, 0); // 初始化snNet对象
    snNet_timeout(net, SN_FT_SDRV_TO);        // 设置超时时间

    snHash_file(hash, fn);                    // 获取文件的哈希值：SHA-256
    snFile_fileSize(&fileSize, fn);           // 获取文件大小

    memcpy(fSizeByte, &fileSize, 8);          // 将文件大小复制到数组中

    code = snNet_connect(net);                // 连接至接收端
    if(!code) {
        printf("连接至接收端成功：%s:%d\n", ctx->addr, ctx->port);
    } else {
        printf("连接至接收端失败：%s:%d\n", ctx->addr, ctx->port);
        return code;
    }

    printf("将文件长度与哈希值发送至接收端。\n");
    snNet_send(net, &tSize, fSizeByte, 8);    // 发送文件的长度
    snNet_send(net, &tSize, (snNetBuf *)hash->digest,     // 发送文件的哈希值
        hash->digestSize);

    printf("将文件数据完整发送至接收端。\n");
    _snTransfer_sendall(net, fileSize, fn);   // 发送文件数据

    snNet_recv(net, &tSize, listenMsg, 4);    // 接收来自接收端返回的信号
    if(0 == strncmp((char *)listenMsg, (char *)&SN_FT_SIGNAL_DONE, 4)) {
        printf("收到来自接收端发送的完成信号。\n");
    } else {
        printf("收到来自接收端发送的未知信号。\n"
            "信号：%02x %02x %02x %02x\n",
            listenMsg[0], listenMsg[1], listenMsg[2], listenMsg[3]);
    }

    snNet_close(net);
    snNet_release(&net);
    snHash_release(&hash, SN_RELEASE_NORMAL);

    return snErr_OK;
}