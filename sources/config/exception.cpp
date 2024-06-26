#include <config/exception.hpp>

/*
* // 如果wmkc::Exception::Exception中to_string出错，那么取消注释。
* #include <sstream>
* namespace std {
*     template <typename T>
*     std::string std::to_string(T value) {
*         std::ostringstream os;
*         os << value;
*         return os.str();
*     }
* };
*/

wmkc::Exception::Exception(wS32 code, std::string function, std::string message)
{
#   if __cplusplus >= 202002
    this->output_message = std::format("{0}[{1}]: {2}", function, code, message);
#   else
    this->output_message = function + "[" + std::to_string(code) + "]: " + message;
#   endif
}

const char *wmkc::Exception::what() const noexcept
{
    return this->output_message.c_str();
}
