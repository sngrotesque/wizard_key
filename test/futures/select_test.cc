#include <net/WukSocket.hh>
#include <net/WukError.hh>
#include <config/WukEndianness.hh>

#include <cmath>

#include <vector>
#include <iostream>
#include <functional>

namespace wn = wuk::net;

static timeval create_timeval(double t)
{
    timeval tv {0};

    double int_part;
    double float_part;

    float_part = modf(t, &int_part);

    tv.tv_sec = static_cast<time_t>(int_part);
    tv.tv_usec = static_cast<time_t>(float_part * 1e6);

    return tv;
}

inline void throw_exception(const std::string &func, wI32 code = 0, const std::string &msg = std::string{})
{
    wI32 err_code = (code) ? code : wn::SystemError::code();
    std::string err_msg = (msg.empty()) ? wn::SystemError::message(err_code) : msg;
    throw wuk::Exception(err_code, func, err_msg);
}

static std::string recv_data(wuk::net::WukSocket &fd)
{
    std::string packet_length = fd.recv(4);
    wI32 data_length {0};

    memcpy(&data_length, packet_length.data(), 4);
    wI32 tmp_length = data_length;

    if (tmp_length == 0) {
        return {};
    }

    printf("TEST data length: %d\n", tmp_length);

    std::string result;
    while (tmp_length) {
        std::string tmp_data = fd.recv(wuk::min(2048, tmp_length));
        if (tmp_data.empty()) {
            break;
        }
        result += tmp_data;
        tmp_length -= tmp_data.length();
    }

    return result;
}

void connect_test(wn::WukSocket &fd, const std::string &remote_addr, const wU16 &remote_port, double timeout)
{
    fd.set_blocking(false);

    try {
        fd.connect(remote_addr, remote_port);
    } catch (wuk::Exception &e) {
        timeval timeout_tv = create_timeval(timeout);
        fd_set my_fd_set;

        wn::SocketError sock_err = wn::from_code(e.get_err_code());
        if ((sock_err == wn::SocketError::WOULDBLOCK) /* Windows */ ||
            (sock_err == wn::SocketError::INPROGRESS) /* Linux */ ) {
            std::cout << "WSAEWOULDBLOCK in connect() - selecting.\n";
            while (true) {
                FD_ZERO(&my_fd_set);
                FD_SET(fd.get_fd(), &my_fd_set);

                wI32 err = select(fd.get_fd() + 1, nullptr, &my_fd_set, nullptr, &timeout_tv);

                if ((err == NETERROR) && (wn::from_code(wn::SystemError::code()) != wn::SocketError::INTR)) {
                    throw_exception("connect_test");
                } else if (err != 0) {
                    wI32 err = fd.getsockopt<wI32>(SOL_SOCKET, SO_ERROR);
                    if (err) {
                        throw_exception("connect_test", err);
                    }
                    break;
                } else {
                    throw_exception("connect_test", wuk::Error::OK, "timeout.");
                }
            }
        } else {
            throw_exception("connect_test");
        }
    }

    fd.set_blocking(true);
}

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws);
#   endif

    try {
        wn::WukSocket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        std::string local_addr("0.0.0.0");
        constexpr wU16 local_port{48888};

        connect_test(fd, local_addr, local_port, 30);

        fd.close();

    } catch (wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "unknown exception..." << std::endl;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
