#include <cpp/includes/crypto/snc.hpp>
#include <cpp/includes/wmkc_base64.hpp>
#include <c/includes/wmkc_misc.h>

int main(int argc, char **argv)
{
    std::string message("hello\x00\x00\x00world", 13);
    std::cout << message.length() << std::endl;
    return 0;
}
