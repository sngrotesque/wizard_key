#include <network/wmkc_net.c>
#include <crypto/snc.c>
#include <wmkc_memory.c>

static const wmkcByte snc_Content[96];

int main(int argc, char **argv)
{
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);

    wmkcNet_obj *sockfd = wmkcNull;
    wmkcSNC_obj *snc = wmkcNull;

    wmkcNet_new(&sockfd);
    wmkcNet_socket(sockfd, AF_INET, SOCK_STREAM, 0);
    wmkcNet_connect(sockfd, "127.0.0.1", 49281);

    wmkcNetBufT content[32] = {"ABCDEF0123456789.AES_KEY_IV_128"};
    wmkcSNC_new(&snc, SNC_768);
    wmkcSNC_init(snc, snc_Content, snc_Content);
    wmkcSNC_ctr_xcrypt(snc, content, sizeof(content));

    wmkcNet_send(sockfd, content, sizeof(content), 0);

    wmkcNet_close(sockfd);
    wmkcNet_free(&sockfd);
    wmkcSNC_free(&snc);

    WSACleanup();
    return 0;
}

