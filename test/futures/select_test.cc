#include <net/WukSocket.hh>
#include <net/WukError.hh>

#include <iostream>
#include <cmath>

namespace wn = wuk::net;

void connect_test(const std::string &addr, const wU16 &port, double timeout)
{
    auto create_timeval = [](double t) -> timeval {
        timeval tv {0};

        double int_part;
        double float_part;

        float_part = modf(t, &int_part);

        tv.tv_sec = static_cast<time_t>(int_part);
        tv.tv_usec = static_cast<time_t>(float_part * 1e6);

        return tv;
    };

    auto throw_exception = [](wI32 code = 0, const std::string &msg = std::string{}) {
        wI32 err_code = (code) ? code : wn::SystemError::code();
        std::string err_msg = (msg.empty()) ? wn::SystemError::message(err_code) : msg;
        throw wuk::Exception(err_code, "connect_test", err_msg);
    };

    wn::WukSocket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    fd_set my_fd_set;

    fd.set_blocking(false);

    try {
        fd.connect(addr, port);
    } catch (wuk::Exception &e) {
        try {
            wn::SocketError serr = wn::from_code(e.get_err_code());
            if ((serr == wn::SocketError::WOULDBLOCK) || (serr == wn::SocketError::INPROGRESS)) {
                std::cout << "WSAEWOULDBLOCK in connect() - selecting.\n";
                while (true) {
                    FD_ZERO(&my_fd_set);
                    FD_SET(fd.get_fd(), &my_fd_set);

                    timeval timeout_tv = create_timeval(timeout);
                    wI32 err = select(fd.get_fd() + 1, nullptr, &my_fd_set, nullptr, &timeout_tv);

                    if ((err == NETERROR) && (wn::from_code(wn::SystemError::code()) != wn::SocketError::INTR)) {
                        throw_exception();
                    } else if (err != 0) {
                        wI32 err = fd.getsockopt<wI32>(SOL_SOCKET, SO_ERROR);
                        if (err) {
                            throw_exception(err);
                        }
                        break;
                    } else {
                        throw_exception(wuk::Error::OK, "timeout.");
                    }
                }
            } else {
                throw_exception();
            }
        } catch (wuk::Exception &e) {
            fd.set_blocking(true);
            fd.close();
            throw;
        }
    }
}

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    try {
        connect_test("www.pixiv.net", 80, 2);
    } catch (wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
