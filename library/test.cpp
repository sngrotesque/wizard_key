#include <iostream>
#include <fstream>

#include "qrcode/qrcode.hpp"
#include "qrcode/qrcode.cpp"

void generateQrCode(const std::string& textToEncode) {
    // 设置误差校正级别
    const qrcodegen::QrCode::Ecc errorCorrectionLevel = qrcodegen::QrCode::Ecc::MEDIUM;

    // 创建二维码
    const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(textToEncode.c_str(), errorCorrectionLevel);

    // 定义二维码的大小
    constexpr int qrSize = 4;

    // 创建空白图片
    std::string svgContent = qr.toSvgString(qrSize);

    // 保存为SVG文件
    std::ofstream svgFile("qrcode.svg");
    svgFile << svgContent;
    svgFile.close();

    // 使用Inkscape或其他图像处理工具将SVG转换为PNG
    std::system("inkscape -z -e qrcode.png qrcode.svg");

    std::cout << "二维码已生成并保存为qrcode.png" << std::endl;
}

int main() {
    std::string textToEncode = "Hello, World!";
    generateQrCode(textToEncode);

    // 打开生成的二维码图像
    std::system("start qrcode.png");

    return 0;
}
