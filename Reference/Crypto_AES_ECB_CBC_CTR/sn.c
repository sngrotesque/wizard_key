#include <stdio.h>
#include <stdlib.h>

#include "aes.h"
#include "aes.c"

static void PRINT(uint8_t *arr, size_t size)
{
    for(int x = 0; x < size; ++x) {
        printf("%02x", arr[x]);
        if((x+1) % 16 == 0) {
            printf("\n");
        } else {
            printf("  ");
        }
    }
}

int main(int argc, char **argv)
{
    static const int size = 80;
    struct AES_ctx *ctx = (struct AES_ctx *)malloc(sizeof(struct AES_ctx));
    uint8_t message[size];

    memcpy(ctx->Iv, "abcdef0123456789", 16);
    memcpy(message,
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
        size);

    AES_init_ctx(ctx, (uint8_t *)"10000000000000000000000000000000");
    AES_CBC_encrypt_buffer(ctx, message, size);

    PRINT(message, size);

    free(ctx);
    return 0;
}



