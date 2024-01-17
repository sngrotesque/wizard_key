#include <network/wmkc_net.hpp>

using namespace wmkcNet;
using namespace std;

int main(int argc, char **argv)
{
	Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	fd.connect("www.baidu.com", 80);
	fd.send("GET / HTTP/1.1\r\nHost: www.baidu.com\r\nUser-Agent: Android\r\n\r\n");

	cout << fd.recv(4096) << endl;

	return 0;
}
