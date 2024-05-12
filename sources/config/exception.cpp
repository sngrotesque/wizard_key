#include <config/exception.hpp>

void wmkc::exception(wS32 errCode, std::string funcName, std::string errMessage)
{
    throw std::runtime_error(std::string(funcName +
                            "[" + std::to_string(errCode) + "]: " + errMessage));
}

#if(WMKC_ERR_EXCEPTION_TO_STRING == true)
#   include <sstream>
template <typename T>
std::string std::to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}
#endif
