#include <network/dns.hpp>

// 需要根据RFC 1035标准来构造和解析DNS报文
std::string create_dns_qurty(std::string hostname)
{
    std::vector<std::string> tokens;
    std::stringstream sStream(hostname);
    std::string token;
    std::string query;

    query.append("\x04\xd2", 2); // generate random ID
    query.append("\x01\x00", 2); // set recursion desired flag
    query.append("\x00\x01", 2); // set number of questions
    query.append("\x00\x00", 2); // set number of answers
    query.append("\x00\x00", 2); // set number of authority records
    query.append("\x00\x00", 2); // set number of additional records

    while(std::getline(sStream, token, '.')) {
        tokens.push_back(token);
    }
    for(const auto &label : tokens) {
        wChar _tmp[2] = {(wChar)(label.size() & 0xff), 0x00};
        query.append(_tmp, 1);
        query.append(label);
    }

    query.append("\x00", 1);
    query.append("\x00\x01", 2);
    query.append("\x00\x01", 2);

    return query;
}

// 此函数需要优化，目前只是返回收到的数据的完整内容。
// 需要针对DNS响应包进行修改
std::string wmkc::net::DNS(std::string hostname, std::string dnsServer)
{
    wmkc::net::Socket sockfd(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    std::string query = create_dns_qurty(hostname);

    sockfd.sendto(query, wmkc::net::IPEndPoint(dnsServer, 53));
    std::string result = sockfd.recvfrom(1024);

    return result;
}


