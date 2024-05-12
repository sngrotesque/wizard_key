#include <cjson/cJSON.h>

#include <cpp/includes/network/wmkc_ssl.hpp>
#include <library/qrcode/qrcode.hpp>

using namespace std;

namespace use_qrcode {
    using qrcodegen::QrCode;
    using qrcodegen::QrSegment;

    static void printQr(const QrCode &qr) {
        int border = 4;
        for (int y = -border; y < qr.getSize() + border; y++) {
            for (int x = -border; x < qr.getSize() + border; x++) {
                std::cout << (qr.getModule(x, y) ? "  " : "##");
                }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void qrcode_generator(const char *content)
    {
        printQr(QrCode::encodeText(content, QrCode::Ecc::LOW));
    }
}

namespace use_network {
    using namespace wmkcNet;
    const std::string userAgent = "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/116.0";
}

int main(int argc, char **argv)
{
    use_qrcode::qrcode_generator("hello, world");
    return 0;
}
