#include <network/wmkc_net.hpp>

using namespace wmkcNet;
using namespace std;

int main(int argc, char **argv)
{
#	ifdef WMKC_PLATFORM_WINOS
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
#	endif

	Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	fd.connect("47.243.162.23", 48267);
	fd.send("GET / HTTP/1.1\r\nHost: www.baidu.com\r\nUser-Agent: Android\r\n\r\n");

	cout << fd.recv(4096) << endl;

#	ifdef WMKC_PLATFORM_WINOS
	WSACleanup();
#	endif
	return 0;
}
