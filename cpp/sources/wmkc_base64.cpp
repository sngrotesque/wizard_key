#include <wmkc_base64.hpp>

std::string wmkcBase64::encode(std::string content)
{
    wmkcByte *dst = wmkcNull;
    wmkcSize dst_size = 0;

    this->error = wmkcBase64_encode(&dst, &dst_size, (wmkcByte *)content.c_str(), content.size());
    if(this->error.code) {
        wmkcErr_exception(this->error);
    }

    return std::string((wmkcChar *)dst, dst_size);
}

std::string wmkcBase64::decode(std::string content)
{
    wmkcByte *dst = wmkcNull;
    wmkcSize dst_size = 0;

    this->error = wmkcBase64_decode(&dst, &dst_size, (wmkcByte *)content.c_str(), content.size());
    if(this->error.code) {
        wmkcErr_exception(this->error);
    }

    return std::string((wmkcChar *)dst, dst_size);
}
