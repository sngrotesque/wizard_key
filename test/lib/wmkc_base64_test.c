#include <wmkc_base64.h>
#include <wmkc_memory.c>

void encode()
{
    wmkcByte *dst = wmkcNull;
    wmkcByte *src = (wmkcByte *)"hello, world";
    wmkcBase64_encode(&dst, src, strlen((char *)src));
    wmkcMem_free(dst);
}

void decode()
{
    wmkcByte *dst = wmkcNull;
    wmkcByte *src = (wmkcByte *)"aGVsbG8sIHdvcmxk";
    wmkcBase64_decode(&dst, src, strlen((char *)src));
    wmkcMem_free(dst);
}

int main()
{
    encode();
    decode();
    return 0;
}
