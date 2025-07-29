#include <WukBuffer.hh>
#include <WukMisc.hh>

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

void method_7()
{
    wuk::Buffer buffer;

    buffer.append_number(4);

    wuk::misc::print_hex(buffer.get_data(), buffer.get_length(), 16, true, false);
}

void method_8()
{
    wuk::Buffer a(10);

    cout << a.get_size() << endl;
}

void method_9()
{
    wByte buf[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0x7f
    };

    wuk::Buffer s(buf, sizeof buf);
    std::cout << s.hex() << std::endl;
}

int main()
{
    try {
        method_9();
    } catch (wuk::Exception &e) {
        cout << e.what() << endl;
    }

    return 0;
}
