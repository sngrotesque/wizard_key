#include <iostream>
#include <chrono>
#include <thread>

#include <network/WukNetwork.hh>
#include <network/WukSocket.hh>
#include <network/WukIPEndPoint.hh>
#include <network/WukSocketOptions.hh>
#include <WukBuffer.hh>

#include <network/WukSocket.cc>
#include <network/WukIPEndPoint.cc>
#include <network/WukSocketOptions.cc>
#include <WukBuffer.cc>

#if !defined(WUK_NET_ERROR) || !defined(WUK_PLATFORM_LINUX)
#include <unistd.h>
#endif

constexpr int PORT = 12345;
constexpr int BUFFER_SIZE = 1024;