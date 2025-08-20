#include <crypto/WukCommon.hh>
#include <WukBuffer.hh>
#include <WukBase64.hh>

#include <iostream>
#include <vector>

int main()
{
    std::cout << wuk::base64::encode("\\(UwU)/") << std::endl;
    std::cout << wuk::base64::decode("XChVd1UpLw==") << std::endl;


    return 0;
}
