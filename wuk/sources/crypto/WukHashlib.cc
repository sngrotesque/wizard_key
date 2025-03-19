#include <crypto/WukHashlib.hh>

void wukSSL_exception(const char *funcName)
{
    wU32 err_code = ERR_get_error();
    char err_msg[256]{};
    ERR_error_string(err_code, err_msg);
    throw wuk::Exception(static_cast<wuk::Error>(err_code), funcName, err_msg);
}

wuk::crypto::Hashlib::Hashlib()
{
    this->md_ctx = EVP_MD_CTX_new();
    if (!this->md_ctx) {
        wukSSL_exception("wuk::crypto::Hashlib::Hashlib");
    }
}



void wuk::crypto::Hashlib::init_ctx(EVP_MD *type, ENGINE *impl)
{

}

