#ifndef _FTP_HEADER
#define _FTP_HEADER

//---------------------------//
#include <network/socket.hpp>
//---------------------------//
#include <network/socket.cpp>
#include <network/exception.cpp>
//---------------------------//

#include <fstream>

#define DEFAULT_PORT 22734

template <typename T>
void ftp_send(T file_path,
    std::string addr, wU16 port = DEFAULT_PORT);

template <typename T>
void ftp_recv(T file_path,
    std::string addr, wU16 port = DEFAULT_PORT);

#endif
