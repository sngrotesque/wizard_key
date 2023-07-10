#include <network/wmkc_chunk.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcChunk_new WMKC_OPEN_API
WMKC_OF((wmkcChunk_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcChunk_new: obj is NULL.");
    }

    if(!wmkcMem_new(wmkcChunk_obj *, (*obj), sizeof(wmkcChunk_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcChunk_new: "
            "Failed to allocate memory for (*obj).");
    }
    wmkcMem_zero((*obj)->packetNumber, sizeof((*obj)->packetNumber));
    wmkcMem_zero((*obj)->packetSize, sizeof((*obj)->packetSize));
    wmkcMem_zero((*obj)->packetCRC, sizeof((*obj)->packetCRC));
    (*obj)->packetContent = wmkcNull;
    (*obj)->packetSize_n = 0U;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcChunk_free WMKC_OPEN_API
WMKC_OF((wmkcChunk_obj **obj))
{
    wmkcErr_obj error;
    if(!obj || !(*obj)) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcChunk_new: obj is NULL.");
    }
    if((*obj)->packetContent) {
        wmkcMem_free((*obj)->packetContent);
    }
    wmkcMem_free(*obj);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcChunk_encrypt WMKC_OPEN_API
WMKC_OF((wmkcChunk_obj *obj, wmkcByte *key, wmkcByte *iv))
{
    wmkcErr_obj error;
    if(!obj || !obj->packetContent || !obj->packetSize_n) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcChunk_new: "
            "obj or obj->packetContent or obj->packetSize_n is NULL.");
    }
    wmkcSNC_obj *snc = wmkcNull;

    error = wmkcSNC_new(&snc, SNC_512);
    if(error.code) return error;
    error = wmkcSNC_init(snc, key, iv);
    if(error.code) return error;

    wmkcSNC_cbc_encrypt(snc, obj->packetContent, obj->packetSize_n);

    error = wmkcSNC_free(&snc);
    if(error.code) return error;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcChunk_decrypt WMKC_OPEN_API
WMKC_OF((wmkcChunk_obj *obj, wmkcByte *key, wmkcByte *iv))
{
    wmkcErr_obj error;
    if(!obj || !obj->packetContent || !obj->packetSize_n) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcChunk_new: "
            "obj or obj->packetContent or obj->packetSize_n is NULL.");
    }
    wmkcSNC_obj *snc = wmkcNull;

    error = wmkcSNC_new(&snc, SNC_512);
    if(error.code) return error;
    error = wmkcSNC_init(snc, key, iv);
    if(error.code) return error;

    wmkcSNC_cbc_decrypt(snc, obj->packetContent, obj->packetSize_n);

    error = wmkcSNC_free(&snc);
    if(error.code) return error;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcChunk_buildChunk WMKC_OPEN_API
WMKC_OF((wmkcChunk_obj *obj, wmkc_u32 number, wmkcByte *content, wmkc_u32 size))
{
    wmkcErr_obj error;
    if(!obj || !content || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcChunk_buildChunk: "
            "obj or content or size is NULL.");
    }

    obj->packetSize_n = size + wmkcPad_offset(SNC_BLOCKLEN, size);

    if(!wmkcMem_new(wmkcByte *, obj->packetContent, obj->packetSize_n)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcChunk_buildChunk: "
            "Failed to allocate memory for obj->packetContent.");
    }
    memcpy(obj->packetContent, content, size);

    wmkcSize tmp_pad_size = size;
    wmkcPad_add(obj->packetContent, &tmp_pad_size, SNC_BLOCKLEN, false);

    wmkcStruct_pack(">I", obj->packetNumber, number);
    wmkcStruct_pack(">I", obj->packetSize, obj->packetSize_n);
    wmkcStruct_pack(">I", obj->packetCRC, crc32(0, obj->packetContent, obj->packetSize_n));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcChunk_parseChunk WMKC_OPEN_API
WMKC_OF((wmkcChunk_obj *obj, wmkcByte *chunk, wmkc_u32 chunk_size))
{
    wmkcErr_obj error;
    if(!obj || !chunk || !chunk_size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcChunk_parseChunk: "
            "obj or chunk or chunk_size is NULL.");
    }
    memcpy(obj->packetNumber, chunk, sizeof(obj->packetNumber));
    memcpy(obj->packetSize, chunk + sizeof(obj->packetNumber), sizeof(obj->packetSize));
    wmkcStruct_unpack(">I", &obj->packetSize_n, obj->packetSize);

    if(!wmkcMem_new(wmkcByte *, obj->packetContent, obj->packetSize_n)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcChunk_parseChunk: "
            "Failed to allocate memory for obj->packetContent.");
    }
    memcpy(obj->packetContent, chunk + sizeof(obj->packetNumber) + sizeof(obj->packetSize),
        obj->packetSize_n);
    memcpy(obj->packetCRC, chunk + sizeof(obj->packetNumber) + sizeof(obj->packetSize) + \
        obj->packetSize_n, sizeof(obj->packetCRC));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
