#include <iostream>
#include <cstring>
#include <cstdlib>

#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define IP_HEADER_LEN 20
#define TCP_HEADER_LEN 20

using namespace std;

// IP header structure
struct ip_header {
    unsigned char ip_v:4; // version
    unsigned char ip_hl:4; // header length
    unsigned char ip_tos; // type of service
    unsigned short ip_len; // total length
    unsigned short ip_id; // identification
    unsigned short ip_off; // fragment offset
    unsigned char ip_ttl; // time to live
    unsigned char ip_p; // protocol
    unsigned short ip_sum; // checksum
    struct in_addr ip_src; // source address
    struct in_addr ip_dst; // destination address
};

// TCP header structure
struct tcp_header {
    unsigned short th_sport; // source port
    unsigned short th_dport; // destination port
    unsigned int th_seq; // sequence number
    unsigned int th_ack; // acknowledgement number
    unsigned char th_x2:4; // reserved
    unsigned char th_off:4; // data offset
    unsigned char th_flags; // flags
    unsigned short th_win; // window
    unsigned short th_sum; // checksum
    unsigned short th_urp; // urgent pointer
};

// Pseudo header structure for TCP checksum calculation
struct pseudo_header {
    unsigned int src_ip; // source IP address
    unsigned int dst_ip; // destination IP address
    unsigned char zero; // zero
    unsigned char proto; // protocol
    unsigned short len; // TCP length
};

// Calculate checksum
unsigned short checksum(unsigned short *buf, int size) {
    unsigned long sum = 0;
    while (size > 1) {
        sum += *buf++;
        size -= 2;
    }
    if (size == 1) {
        sum += *(unsigned char*)buf;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

// Initialize IP header
void init_ip_header(struct ip_header *header, const char *src_ip, const char *dst_ip) {
    header->ip_hl = 5; // header length = 5 * 4 = 20 bytes
    header->ip_v = 4; // version = 4
    header->ip_tos = 0; // type of service
    header->ip_len = htons(IP_HEADER_LEN + TCP_HEADER_LEN); // total length
    header->ip_id = 0; // identification
    header->ip_off = 0; // fragment offset
    header->ip_ttl = 64; // time to live
    header->ip_p = IPPROTO_TCP; // protocol = TCP
    header->ip_sum = 0; // checksum (set to 0 before calculation)
    header->ip_src.s_addr = inet_addr(src_ip); // source address
    header->ip_dst.s_addr = inet_addr(dst_ip); // destination address
    header->ip_sum = checksum((unsigned short*)header, IP_HEADER_LEN); // checksum
}

// Initialize TCP header
void init_tcp_header(struct tcp_header *header, int src_port, int dst_port) {
    header->th_sport = htons(src_port); // source port
    header->th_dport = htons(dst_port); // destination port
    header->th_seq = htonl(rand()); // sequence number
    header->th_ack = 0; // acknowledgement number
    header->th_x2 = 0; // reserved
    header->th_off = 5; // data offset = 5 * 4 = 20 bytes
    header->th_flags = 0x02; // flags = SYN
    header->th_win = htons(4096); // window
    header->th_sum = 0; // checksum (set to 0 before calculation)
    header->th_urp = 0; // urgent pointer
}

// Initialize pseudo header
void init_pseudo_header(struct pseudo_header *header, struct ip_header *ip_header) {
    header->src_ip = ip_header->ip_src.s_addr; // source IP address
    header->dst_ip = ip_header->ip_dst.s_addr; // destination IP address
    header->zero = 0; // zero
    header->proto = IPPROTO_TCP; // protocol = TCP
    header->len = htons(TCP_HEADER_LEN); // TCP length
}

// Send a SYN packet to the target host and port
void send_syn(const char *src_ip, const char *dst_ip, int src_port, int dst_port) {
    WSADATA wsa_data;
    SOCKET sock;
    struct sockaddr_in addr;
    char buffer[IP_HEADER_LEN + TCP_HEADER_LEN];
    struct ip_header *ip_header = (struct ip_header*)buffer;
    struct tcp_header *tcp_header = (struct tcp_header*)(buffer + IP_HEADER_LEN);
    struct pseudo_header pseudo_header;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        exit(1);
    }

    // Create a raw socket
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    // Set IP_HDRINCL socket option
    int optval = 1;
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char*)&optval, sizeof(optval)) == SOCKET_ERROR) {
        printf("setsockopt failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(1);
    }

    // Initialize IP header
    init_ip_header(ip_header, src_ip, dst_ip);

    // Initialize TCP header
    init_tcp_header(tcp_header, src_port, dst_port);

    // Initialize pseudo header
    init_pseudo_header(&pseudo_header, ip_header);

    // Calculate TCP checksum
    int total_len = sizeof(struct pseudo_header) + TCP_HEADER_LEN;
    char *tmp = (char*)malloc(total_len);
    memcpy(tmp, &pseudo_header, sizeof(struct pseudo_header));
    memcpy(tmp + sizeof(struct pseudo_header), tcp_header, TCP_HEADER_LEN);
    tcp_header->th_sum = checksum((unsigned short*)tmp, total_len);
    free(tmp);

    // Set destination address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(dst_port);
    addr.sin_addr.s_addr = inet_addr(dst_ip);

    // Send the packet
    if (sendto(sock, buffer, IP_HEADER_LEN + TCP_HEADER_LEN, 0, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("sendto failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(1);
    }

    // Close the socket
    closesocket(sock);
    WSACleanup();
}

int main() {
    cout << "start..." << endl;

    const char *src_ip = "192.168.1.221"; // fake source IP address
    const char *dst_ip = "223.5.5.5"; // target IP address
    int src_port = 51039; // source port
    int dst_port = 80; // target port
    send_syn(src_ip, dst_ip, src_port, dst_port); // send a SYN packet

    cout << "stop..." << endl;
    return 0;
}
