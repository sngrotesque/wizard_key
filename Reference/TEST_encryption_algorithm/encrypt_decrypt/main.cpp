#include "libc/algorithm.h"

int main(int argc, char *argv[])
{
    system("clear");
    const uint8_t str[] = {
		"\x00\x00\x01\xFF\x0F\xFE\xB7"
        "\xB7\x77\xF7\xC7"
    };
    
    const uint8_t *res = encrypt(str, Plen(str));
    PRINT_HEX(res, strlen((char *)res));
    

    return 0;
}








