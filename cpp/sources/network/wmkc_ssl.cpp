/**
 * 此库出现了未知的错误，无法修复，后续将重构这整个SSL库
*/
#include <network/wmkc_ssl.hpp>

void wmkcNet::wmkcSSL_Socket::settimeout(double _time)
{
    
}

// wmkcSSL_Socket
void wmkcNet::wmkcSSL_Socket::connect(std::string addr, const uint16_t port)
{
    this->_fd->connect(addr, port);
    SSL_connect(this->ssl);
}

void wmkcNet::wmkcSSL_Socket::bind(std::string addr, wmkc_u16 port)
{
    
}

void wmkcNet::wmkcSSL_Socket::listen(wmkc_s32 backlog)
{
    
}

wmkcNet::wmkcSSL_Socket wmkcNet::wmkcSSL_Socket::accept()
{
    return wmkcNet::wmkcSSL_Socket(new wmkcNet::Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), wmkcNull);
}

void wmkcNet::wmkcSSL_Socket::send(wmkcNetBufT *buf, wmkc_s32 len, wmkc_s32 flag)
{
    
}

void wmkcNet::wmkcSSL_Socket::sendall(wmkcNetBufT *buf, wmkc_s32 len, wmkc_s32 flag)
{
    
}

void wmkcNet::wmkcSSL_Socket::send(std::string content, wmkc_s32 flag)
{
    SSL_write(this->ssl, content.c_str(), content.size());
}

void wmkcNet::wmkcSSL_Socket::sendall(std::string content, wmkc_s32 flag)
{
    
}

void wmkcNet::wmkcSSL_Socket::recv(wmkcNetBufT *buf, wmkc_s32 len, wmkc_s32 flag)
{
    
}

std::string wmkcNet::wmkcSSL_Socket::recv(wmkc_s32 len, wmkc_s32 flag)
{
    wmkcNetBufT *_tmp = new wmkcNetBufT[len];
    std::string message(_tmp, SSL_read(this->ssl, _tmp, len));
    delete[] _tmp;
    return message;
}

void wmkcNet::wmkcSSL_Socket::shutdown(wmkc_s32 how)
{
    
}

void wmkcNet::wmkcSSL_Socket::close()
{
    
}

// wmkcSSL_Context
wmkcNet::wmkcSSL_Socket wmkcNet::wmkcSSL_Context::wrap_socket(wmkcNet::Socket *sock,
    std::string server_hostname)
{
    wmkcNet::wmkcSSL_Socket sslSocket(sock, this->ssl);
    SSL_set_fd(this->ssl, sock->fd);
    SSL_set_tlsext_host_name(this->ssl, server_hostname.c_str());
    return wmkcSSL_Socket(sock, this->ssl);
}
