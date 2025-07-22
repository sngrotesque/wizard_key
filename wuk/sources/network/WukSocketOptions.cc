#include <network/WukSocketOptions.hh>

wuk::net::SockOpt::SockOpt() : val(nullptr), val_len() {}

wuk::net::SockOpt::SockOpt(char *val, socklen_t val_len) : val(val), val_len(val_len) {}

wuk::net::SockOpt::SockOpt(socklen_t val)
{
    this->val = reinterpret_cast<char *>(&val);
    this->val_len = sizeof(val);
}

wuk::net::SockOpt::SockOpt(std::string val)
{
    this->val = const_cast<char *>(val.c_str());
    this->val_len = val.size();
}

