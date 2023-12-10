#include <network/wmkc_dns.hpp>

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
        wmkcChar _tmp[2] = {(wmkcChar)(label.size() & 0xff), 0x00};
        query.append(_tmp, 1);
        query.append(label);
    }

    query.append("\x00", 1);
    query.append("\x00\x01", 2);
    query.append("\x00\x01", 2);

    return query;
}

std::string wmkcNet::wmkcDNS(std::string hostname)
{
    wmkcNet::Socket sockfd(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    std::string query = create_dns_qurty(hostname);



    return query;
}


