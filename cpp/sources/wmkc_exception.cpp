#include <wmkc_exception.hpp>

#if (_MSC_VER >= 1930)
template <typename T>
std::string std::to_string(T value)
{
    std::ostringstream os;
    os << value;
    return os.c_str();
}
#endif

void wmkcErr_exception(wmkcErr_obj err)
{
    std::string message = std::string(err.func) + "[" + std::to_string(err.code) + "]: " + err.message;
    throw std::runtime_error(message);
}

void wmkcErr_exception(wmkcSSize errCode, std::string funcName, std::string errMessage)
{
    throw std::runtime_error(std::string(funcName + "[" + std::to_string(errCode) + "]: " + errMessage));
}
