#include <network/wmkc_chunk.h>

WMKC_PUBLIC(wmkcVoid) wmkcChunk_recv WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBufT **buf, socklen_t *size))
{
    wmkcNetBufT *p = wmkcNull;
    socklen_t totalLengthReceived;
    wmkcByte size_array[4] = {0};
    wmkcByte crc32_array[4] = {0};
    wmkc_u32 crc32_n;

    wmkcNet_recv(obj, size_array, 4, 0);
    wmkcStruct_unpack(">I", size, size_array);
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
    wmkcStruct_unpack(">I", &crc32_n, crc32_array);

    if(crc32(0, (*buf), *size) != crc32_n) {
        wmkcMem_free((*buf));
        return;
    }
}

WMKC_PUBLIC(wmkcVoid) wmkcChunk_send WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBufT *buf, socklen_t size))
{
    wmkcByte size_array[4] = {0};
    wmkcByte crc32_array[4] = {0};

    wmkcStruct_pack(">I", size_array, size);
    wmkcStruct_pack(">I", crc32_array, crc32(0, buf, size));

    wmkcNet_send(obj, size_array, 4, 0);
    wmkcNet_sendall(obj, buf, size, 0);
    wmkcNet_send(obj, crc32_array, 4, 0);
}
