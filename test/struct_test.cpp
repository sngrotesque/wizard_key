#include <config/wmkc.hpp>

#include <cstdarg>

#include <iostream>
#include <vector>
#include <any>

using namespace std;

// python run.py test\struct_test.cpp -O3 -Wall -DWMKC_EXPORTS

// 后续考虑使用vector<any>来实现编组，可能比c_stdarg更复杂。
void analysis_single_group(std::string format, ...)
{
    const char *format_string = format.c_str();
    const wU32 format_length = format.size();

    const char *args_length_begin = nullptr;
    const char *args_length_end = nullptr;

    va_list va;
    va_start(va, format);






    va_end(va);
}

int main(int argc, char **argv)
{
    const char *buffer = "1231231537518374928798517aaaaaaaaaaa";
    const wU32 length = static_cast<wU32>(strlen(buffer));

    const char *p_b = buffer;
    const char *p_e = buffer;

    while(*p_e++ && isdigit(*p_e));

    printf("begin: %c, end: %c, string_number_length: %zd\n",
        *p_b, *p_e, static_cast<ptrdiff_t>(p_e - p_b));

    return 0;
}
