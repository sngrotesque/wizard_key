#include <config/exception.hpp>

wmkc::Exception::Exception(wS32 code, std::string function, std::string message)
{
    this->output_message = function + "[" + std::to_string(code) + "]: " + message;
}

const char *wmkc::Exception::what() const noexcept
{
    return this->output_message.c_str();
}
