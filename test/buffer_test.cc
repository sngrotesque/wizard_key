#include <network/WukSocket.hh>
#include <network/WukIPEndPoint.hh>
#include <network/WukSocketOptions.hh>

#include <WukBuffer.hh>
#include <WukBase64.hh>
#include <WukMisc.hh>

#include <network/WukSocket.cc>
#include <network/WukIPEndPoint.cc>
#include <network/WukSocketOptions.cc>

#include <WukBuffer.cc>
#include <WukBase64.cc>
#include <WukMisc.cc>

#include <iostream>
using namespace std;

void print_info(wuk::Buffer buffer)
{
    printf("%s[I]%s data:   %p\n",
            wuk::color::fore::cyan,
            wuk::color::all::reset,
            buffer.get_data());

    printf("%s[I]%s length: %zd\n",
            wuk::color::fore::cyan,
            wuk::color::all::reset,
            buffer.get_length());
    printf("%s[I]%s sizeof: %zd\n",
            wuk::color::fore::cyan,
            wuk::color::all::reset,
            buffer.get_size());
}

void method_1()
{
    wuk::Buffer buffer{312};
    // wuk::Buffer buffer = string{"this is buffer test for std::string1.\n"};
    wuk::Buffer text{"this is first.\n"};

    buffer = text;

    buffer.append("hello, world.\n");
    memcpy(buffer.append_write(13), "SN-Grotesque\n", 13);
    memcpy(buffer.append_write(text.get_length()), text.get_data(), text.get_length());

    wuk::misc::print_pybytes(buffer.get_data(), buffer.get_length(), true);

    wByte *p = const_cast<wByte *>(buffer.get_data());
    p[0] = 0x00;

    printf("length: %zd\n", buffer.get_length());
    printf("sizeof: %zd\n", buffer.get_size());
}

void method_2()
{
    wuk::Buffer buffer{9};
    buffer.append("abc123456.");

    print_info(buffer);

    buffer = buffer + wuk::Buffer{"'operator+() test.'"};

    print_info(buffer);

    buffer.append("done, test.");

    print_info(buffer);

    buffer += wuk::Buffer("operator+=() test.");

    print_info(buffer);

    cout << buffer.get_cstr() << endl;
}

void method_3()
{
    wuk::Buffer buffer1{"123"};
    wuk::Buffer buffer2{"1233"};

    (buffer1 == buffer2) ? (printf("true\n")) : (printf("false\n"));
}

void method_4()
{
    wuk::Base64 base64;

    wuk::Buffer buffer{"XFwoVXdVKS8="};
    wuk::Buffer result = base64.decode(buffer);

    cout << result.get_cstr() << endl;
}

void method_5()
{
    char _1[32] = {"this is write method.\n"};
    char _2[64] = {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};
    wuk::Buffer buffer{4096};

    buffer.write(reinterpret_cast<wByte *>(_1), strlen(_1));
    buffer.append("this is append method.\n");

    buffer.write(reinterpret_cast<wByte *>(_2), strlen(_2));

    buffer.write(reinterpret_cast<wByte *>(_1), strlen(_1));

    buffer.write("a data.");

    wuk::misc::print_pybytes(buffer.get_data(), buffer.get_length(), true);
}

void method_6()
{
    wuk::net::Socket fd{AF_INET, SOCK_STREAM, IPPROTO_TCP};
    fd.connect("passport.bilibili.com", 80);
    fd.send("GET /qrcode/getLoginUrl HTTP/1.1\r\n"
            "Host: passport.bilibili.com\r\n"
            "User-Agent: Android\r\n\r\n");
    cout << fd.recv(4096).get_cstr() << endl;
    fd.close();
}

void method_7()
{
    wuk::Buffer buffer;

    buffer.append_number(4);

    wuk::misc::print_hex(buffer.get_data(), buffer.get_length(), 16, true, false);
}

int main()
{
    try {
        method_7();
    } catch (wuk::Exception &e) {
        cout << e.what() << endl;
    }

    return 0;
}
