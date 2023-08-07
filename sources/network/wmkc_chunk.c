#include <network/wmkc_chunk.h>

WMKC_PUBLIC(wmkcVoid) wmkcChunk_recv WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBufT **buf, socklen_t *size))
{
    wmkcNetBufT *p = wmkcNull;
    socklen_t totalLengthReceived;
    socklen_t _tSize = 0;
    wmkcByte size_array[4];
    wmkcByte crc32_array[4];
    wmkc_u32 crc32_n;

    wmkcNet_recv(obj, wmkcNull, size_array, 4);
    wmkcStruct_unpack(">I", size, size_array);
    totalLengthReceived = *size;

    if(!wmkcMem_new(wmkcNetBufT *, (*buf), totalLengthReceived + 1)) {
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

WMKC_PUBLIC(wmkcVoid) wmkcChunk_send WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBufT *buf, socklen_t size))
{
    wmkcByte size_array[4];
    wmkcByte crc32_array[4];

    wmkcStruct_pack(">I", size_array, size);
    wmkcStruct_pack(">I", crc32_array, crc32(0, buf, size));

    wmkcNet_send(obj, wmkcNull, size_array, 4);
    wmkcNet_sendall(obj, buf, size);
    wmkcNet_send(obj, wmkcNull, crc32_array, 4);
}
