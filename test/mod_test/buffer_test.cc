#include <WukBuffer.hh>
#include <WukMisc.hh>
#include <WukTime.hh>

#include <iostream>
#include <iomanip>
#include <new>

void print_info(const wuk::Buffer &buffer)
{
    printf("%s[I]%s data:   %p\n",
            wuk::color::fore::cyan,
            wuk::color::all::reset,
            buffer.data());

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
    // wuk::Buffer buffer{312};
    wuk::Buffer buffer = std::string{"this is buffer test for std::string1.\n"};
    wuk::Buffer text{"this is first.\n"};

    buffer = text;

    buffer.append("hello, world.\n");
    memcpy(buffer.append(13), "SN-Grotesque\n", 13);
    memcpy(buffer.append(text.get_length()), text.data(), text.get_length());

    wuk::misc::print_pybytes(buffer.data(), buffer.get_length(), true);

    wuk::byte *p = const_cast<wuk::byte *>(buffer.data());
    p[0] = 0x00;

    printf("length: %zd\n", buffer.get_length());
    printf("sizeof: %zd\n", buffer.get_size());
}

void method_2()
{
    wuk::Buffer buffer{5};
    buffer.append("abc123456.");

    print_info(buffer);

    buffer = buffer + wuk::Buffer{"'operator+() test.'"};

    print_info(buffer);

    buffer.append("done, test.");

    print_info(buffer);

    buffer += wuk::Buffer("operator+=() test.");

    print_info(buffer);

    std::cout << buffer.c_str() << std::endl;
}

void method_3()
{
    wuk::Buffer buffer1{"123"};
    wuk::Buffer buffer2{"1233"};

    std::cout << ((buffer1 == buffer2) ? "Equal." : "Not equal.") << std::endl;
}

void method_4()
{
    char _1[32] = {"this is write method.\n"};
    char _2[64] = {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};
    wuk::Buffer buffer{4096};

    buffer.write(reinterpret_cast<wuk::byte *>(_1), strlen(_1));
    buffer.append("this is append method.\n");

    buffer.write(reinterpret_cast<wuk::byte *>(_2), strlen(_2));

    buffer.write(reinterpret_cast<wuk::byte *>(_1), strlen(_1));

    buffer.write("a data.");

    wuk::misc::print_pybytes(buffer.data(), buffer.get_length(), true);
}

void method_5()
{
    wuk::Buffer a(10);

    std::cout << a.get_size() << std::endl;
}

void method_6()
{
    wuk::byte buf[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0x7f
    };

    wuk::Buffer s(buf, sizeof buf);
    std::cout << s.to_hex() << std::endl;
}

void method_7()
{
    constexpr wuk::ulong length = 512ULL * 1024 * 1024;
    wuk::byte *p = new (std::nothrow) wuk::byte[length];
    wuk::Time time;

    wuk::f64 start = time.time<wuk::f64>();
    wuk::Buffer buffer(p, length);
    std::string hex_res = buffer.to_hex();
    wuk::f64 stop = time.time<wuk::f64>();

    std::cout << "Taken time: " << std::fixed << std::setprecision(2) << (stop - start) << std::endl;

    std::cout << hex_res[0] << hex_res[1] << std::endl;

    delete[] p;
}

void method_8()
{
    wuk::Buffer buffer;

    constexpr bool has_data = true;

    if constexpr (has_data) {
        buffer.write(std::string("hello, world"));
    }

    buffer.write(reinterpret_cast<const wuk::byte *>(""), 0);
    wuk::misc::print_pybytes(buffer.data(), buffer.get_length(), true);
    print_info(buffer);

    buffer.append(std::string("good boy."));
    wuk::misc::print_pybytes(buffer.data(), buffer.get_length(), true);
    print_info(buffer);

    memcpy(buffer.append(32), "abcdef0123456789abcdef0123456789", 32);
    wuk::misc::print_pybytes(buffer.data(), buffer.get_length(), true);
    print_info(buffer);

    memcpy(buffer.write(5), "hello", 5);
    wuk::misc::print_pybytes(buffer.data(), buffer.get_length(), true);
    print_info(buffer);
}

int main()
{
    try {
        std::cout << wuk::color::fore::lightGreen
                  << "========================="
                  << " method 1 test "
                  << "========================="
                  << wuk::color::all::reset << std::endl;
        method_1();

        std::cout << wuk::color::fore::lightGreen
                  << "========================="
                  << " method 2 test "
                  << "========================="
                  << wuk::color::all::reset << std::endl;
        method_2();

        std::cout << wuk::color::fore::lightGreen
                  << "========================="
                  << " method 3 test "
                  << "========================="
                  << wuk::color::all::reset << std::endl;
        method_3();

        std::cout << wuk::color::fore::lightGreen
                  << "========================="
                  << " method 4 test "
                  << "========================="
                  << wuk::color::all::reset << std::endl;
        method_4();

        std::cout << wuk::color::fore::lightGreen
                  << "========================="
                  << " method 5 test "
                  << "========================="
                  << wuk::color::all::reset << std::endl;
        method_5();

        std::cout << wuk::color::fore::lightGreen
                  << "========================="
                  << " method 6 test "
                  << "========================="
                  << wuk::color::all::reset << std::endl;
        method_6();

        std::cout << wuk::color::fore::lightGreen
                  << "========================="
                  << " method 7 test "
                  << "========================="
                  << wuk::color::all::reset << std::endl;
        method_7();

        std::cout << wuk::color::fore::lightGreen
                  << "========================="
                  << " method 8 test "
                  << "========================="
                  << wuk::color::all::reset << std::endl;
        method_8();
    } catch (const wuk::Exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
