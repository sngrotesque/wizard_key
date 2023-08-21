#include <network/wmkc_chunk.h>

WMKC_PUBLIC(wmkcVoid) wmkcChunk_recv WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBufT **buf, socklen_t *size))
{
    wmkcNetBufT *p = wmkcNull;
    wmkcNetBufT size_array[4] = {0};
    wmkcNetBufT crc32_array[4] = {0};
    socklen_t totalLengthReceived;
    wmkc_u32 crc32_n;

    wmkcNet_recv(obj, size_array, 4, 0);
    wmkcStruct_unpack(">I", size, (wmkcByte *)size_array);
    totalLengthReceived = *size;

    if(!wmkcMem_new(wmkcNetBufT *, (*buf), totalLengthReceived + 1)) {
        printf("Failed to allocate memory for (*buf).\n");
        return;
    }
    (*buf)[totalLengthReceived] = 0x00;

    p = (*buf);
    while(totalLengthReceived) {
        wmkcNet_recv(obj, p, wmkc_min(totalLengthReceived, 4096), 0);
        p += obj->tSize;
        totalLengthReceived -= obj->tSize;
    }
    wmkcNet_recv(obj, crc32_array, 4, 0);
    wmkcStruct_unpack(">I", &crc32_n, (wmkcByte *)crc32_array);

    if(crc32(0, (wmkcByte *)(*buf), *size) != crc32_n) {
        wmkcMem_free((*buf));
        return;
    }
}

WMKC_PUBLIC(wmkcVoid) wmkcChunk_send WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBufT *buf, socklen_t size))
{
    wmkcNetBufT size_array[4] = {0};
    wmkcNetBufT crc32_array[4] = {0};

    wmkcStruct_pack(">I", (wmkcByte *)size_array, size);
    wmkcStruct_pack(">I", (wmkcByte *)crc32_array, crc32(0, (wmkcByte *)buf, size));

    wmkcNet_send(obj, size_array, 4, 0);
    wmkcNet_sendall(obj, buf, size, 0);
    wmkcNet_send(obj, crc32_array, 4, 0);
}
