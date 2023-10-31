#include <network/wmkc_net.c>
#include <crypto/snc.c>
#include <wmkc_memory.c>

static const wmkcByte snc_Content[96];

void REUSEADDR_settimeout(wmkcNet_obj *obj, double _val)
{
#   if defined(WMKC_PLATFORM_WINOS)
    DWORD _timeout = (DWORD)(_val * 1000);
    wmkcChar *optval = (wmkcChar *)&_timeout;
#   elif defined(WMKC_PLATFORM_LINUX)
    double intpart = 0;
    double fracpart = modf(_val, &intpart);
    struct timeval _timeout = {.tv_sec=(long)intpart, .tv_usec=(long)(fracpart * 1000000)};
    wmkcVoid *optval = (wmkcVoid *)&_timeout;
#   endif
    setsockopt(obj->sockfd, SOL_SOCKET, SO_REUSEADDR, optval, sizeof(_timeout));
}

int main(int argc, char **argv)
{
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);

    wmkcNet_obj *listenSockfd = wmkcNull;
    wmkcNet_obj *clientSockfd = wmkcNull;
    wmkcSNC_obj *snc = wmkcNull;
    wmkcNetBufT content[32];
    wmkcErr_obj error;

    wmkcSNC_new(&snc, SNC_768);
    wmkcSNC_init(snc, snc_Content, snc_Content);

    wmkcNet_new(&listenSockfd);
    wmkcNet_socket(listenSockfd, AF_INET, SOCK_STREAM, 0);
    REUSEADDR_settimeout(listenSockfd, 1);
    wmkcNet_bind(listenSockfd, "0.0.0.0", 49281);
    wmkcNet_listen(listenSockfd, 5);
    printf("Waiting for client connection...\n");
    wmkcNet_accept(&clientSockfd, listenSockfd);

    wmkcNet_recv(clientSockfd, content, sizeof(content), 0);
    wmkcSNC_ctr_xcrypt(snc, content, sizeof(content));

    printf("%s\n", content);

    wmkcNet_close(clientSockfd);
    wmkcNet_close(listenSockfd);
    wmkcNet_free(&clientSockfd);
    wmkcNet_free(&listenSockfd);
    wmkcSNC_free(&snc);

    WSACleanup();
    return 0;
}

