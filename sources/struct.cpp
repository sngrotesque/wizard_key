#include <struct.hpp>

void swap_byte(char *array, wU32 l, wU32 r)
{
    char swap;
    swap = *(array + l);
    *(array + l) = *(array + r);
    *(array + r) = swap;
}

void change_endian(char *array, wU32 size)
{
    switch(size) {
        case 2:
            swap_byte(array, 0, 1);
            break;
        case 4:
            swap_byte(array, 0, 3);
            swap_byte(array, 1, 2);
            break;
        case 8:
            swap_byte(array, 0, 7);
            swap_byte(array, 1, 6);
            swap_byte(array, 2, 5);
            swap_byte(array, 3, 4);
            break;
    }
}

template <typename T>
std::string get_bytes_result(T n, wU32 size, bool change)
{
    char buffer[sizeof(T)] = {0};
    memcpy(buffer, &n, sizeof(T));
    if(change) {
        change_endian(buffer, sizeof(T));
    }
    return std::string(buffer, sizeof(T));
}

template <typename T>
std::string single_pack(char format,
                        wmkc::endianness current, wmkc::endianness specify, T arg)
{
    bool change = ((specify!=wmkc::endianness::NO)&&(current!=specify))?(true):(false);
    std::string results;

    switch(format) {
        case 'B':
            results = get_bytes_result(arg, 1, change); break;
        case 'H':
            results = get_bytes_result(arg, 2, change); break;
        case 'I':
        case 'f':
            results = get_bytes_result(arg, 4, change); break;
        case 'Q':
        case 'N':
        case 'd':
            results = get_bytes_result(arg, 8, change); break;
    }
    return results;
}

std::string wmkc::Struct::pack(std::string format, ...)
{
    wmkc::endianness current = (WMKC_LE_ENDIAN)?(wmkc::endianness::LE):(wmkc::endianness::BE);
    wmkc::endianness specify = wmkc::endianness::NO;
    std::string results;

    switch(format[0]) {
        case '>':
        case '!':
            specify = wmkc::endianness::BE; goto del;
        case '<':
            specify = wmkc::endianness::LE; goto del;
        case '@':
        case '=':
            del:
            format.erase(0, 1);
        default:
            break;
    }

    va_list va;
    va_start(va, format);

    for(const char &fm : format) {
        switch(fm) {
            case 'b':
            case 'B':
                results.append(single_pack(fm, current, specify,
                                        (wByte)va_arg(va, int)));
                break;
            case 'h':
            case 'H':
                results.append(single_pack(fm, current, specify,
                                        (wU16)va_arg(va, int)));
                break;
            case 'i':
            case 'I':
                results.append(single_pack(fm, current, specify,
                                        (wU32)va_arg(va, int)));
                break;
            case 'q':
            case 'Q':
            case 'n':
            case 'N':
                results.append(single_pack(fm, current, specify,
                                        (wSize)va_arg(va, wSSize)));
                break;
            case 'f':
                results.append(single_pack(fm, current, specify,
                                        (float)va_arg(va, double)));
                break;
            case 'd':
                results.append(single_pack(fm, current, specify,
                                        (double)va_arg(va, double)));
                break;
            default:
                throw wmkc::Exception(wmkcErr_Err, "wmkc::Struct::pack",
                                            "bad char in struct format");
        }
    }

    va_end(va);

    return results;
}

std::vector<std::any> wmkc::Struct::unpack(std::string format, std::string buffer)
{
    std::vector<std::any> results;


    return results;
}
