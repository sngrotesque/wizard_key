#include <wmkc_exception.hpp>

void wmkcErr_exception(wmkcErr_obj err)
{
    std::string message = std::string(err.func) + "[" + std::to_string(err.code) + "]: " + err.message;
    throw std::runtime_error(message);
}
