#include <network/wmkc_net.c>
#include <network/wmkc_tp.c>
#include <crypto/snc.c>
#include <wmkc_memory.c>
#include <wmkc_misc.c>

#include <zlib/zlib.h>

// [chunk:[size][data][crc]]
typedef struct {
    wmkcByte *data;
    wmkc_u32 size;
    wmkc_u32 crc;
    wmkcByte *chunk;
    wmkcSize chunk_len;
} tp_obj;

tp_obj *build(wmkcNetBuf *content, wmkcNetSize content_len)
{
    if(!content || !content_len)
        return wmkcNull;
    tp_obj *result = wmkcNull;

    if(!wmkcMem_new(tp_obj *, result, sizeof(tp_obj)))
        return wmkcNull;

    result->chunk_len = sizeof(result->size) + content_len + sizeof(result->crc);
    if(!wmkcMem_new(wmkcByte *, result->chunk, result->chunk_len))
        return wmkcNull;
    wmkcMem_zero(result->chunk, result->chunk_len);

    result->data = content;
    result->size = content_len;
    result->crc = crc32(0, result->data, result->size);

    memcpy(result->chunk, &result->size, sizeof(result->size));
    memcpy(result->chunk + 4, result->data, result->size);
    memcpy(result->chunk + 4 + result->size, &result->crc, sizeof(result->crc));

    return result;
}

int main(int argc, char **argv)
{
    wmkcNet_init(wmkcNull, wmkcNull, 0);

    wmkcNet_obj *net = wmkcNull;
    wmkcNet_new(&net, wmkcNull, AF_INET, false);
    wmkcNet_init(net, "127.0.0.1", WMKC_TP_DEFAULT_PORT);

    wmkcNetBuf *content = (wmkcNetBuf *)(
        "POST /wp-login.php?id=1 HTTP/1.1\r\n"
        "Host: www.1309248281758923.com\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
        "Accept-Encoding: gzip, deflate, br\r\n"
        "Accept-Language: en-US,en;q=0.5\r\n"
        "Connection: keep-alive\r\n"
        "Cookie: sid=395814149827493284739817r19564275698324104298753981492432461783648726423\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 69\r\n"
        "DNT: 1\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "Sec-Fetch-Dest: document\r\n"
        "Sec-Fetch-Mode: navigate\r\n"
        "Sec-Fetch-Site: cross-site\r\n"
        "Sec-GPC: 1\r\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:95.0) Gecko/20100101 Firefox/95.0\r\n\r\n"
        "user=admin&password=12091384903aishuosdhf93y9371hlhfoua3141-_&99990=1");
    wmkcNetSize content_len = strlen(content);

    tp_obj *chunk = build(content, content_len);
    wmkcMisc_PRINT(chunk->chunk, chunk->chunk_len, 32, 1, 0);
    wmkcMem_free(chunk->chunk);
    wmkcMem_free(chunk);

    wmkcNet_close(net);
    wmkcNet_free(&net);
}
