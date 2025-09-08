#include <png.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

class FileToPngConverter
{
private:
    std::vector<uint8_t> fileData;
    uint32_t fileSize;
    uint32_t width;
    uint32_t height;
    size_t requiredPixels;

    // 计算图像尺寸
    void calculateDimensions()
    {
        width = static_cast<uint32_t>(std::sqrt(fileSize));
        height = width;

        while(width * height < fileSize) {
            width++;
        }

        requiredPixels = width * height;
    }

    // 准备图像数据
    std::vector<uint8_t> prepareImageData()
    {
        std::vector<uint8_t> imageData(requiredPixels);

        // 前4个像素存储文件大小
        imageData[0] = (fileSize >> 24) & 0xFF;
        imageData[1] = (fileSize >> 16) & 0xFF;
        imageData[2] = (fileSize >> 8) & 0xFF;
        imageData[3] = fileSize & 0xFF;

        std::copy(fileData.begin(), fileData.end(), imageData.begin() + 4);

        if(requiredPixels > fileSize + 4) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 255);

            for(size_t i = fileSize + 4; i < requiredPixels; i++) {
                imageData[i] = static_cast<uint8_t>(dis(gen));
            }
        }

        return imageData;
    }

public:
    // 读取文件数据
    bool readFile(const fs::path &filePath)
    {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if(!file.is_open()) {
            std::cerr << "无法打开文件: " << filePath << std::endl;
            return false;
        }

        fileSize = static_cast<uint32_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        fileData.resize(fileSize);
        if(!file.read(reinterpret_cast<char *>(fileData.data()), fileSize)) {
            std::cerr << "读取文件失败: " << filePath << std::endl;
            return false;
        }

        calculateDimensions();
        return true;
    }

    // 导出为PNG
    bool exportToPng(const fs::path &outputPath)
    {
        auto imageData = prepareImageData();

        std::ofstream file(outputPath, std::ios::binary);
        if(!file.is_open()) {
            std::cerr << "无法创建输出文件: " << outputPath << std::endl;
            return false;
        }

        png_structp pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if(!pngPtr) {
            std::cerr << "无法创建PNG写结构" << std::endl;
            return false;
        }

        png_infop infoPtr = png_create_info_struct(pngPtr);
        if(!infoPtr) {
            png_destroy_write_struct(&pngPtr, nullptr);
            std::cerr << "无法创建PNG信息结构" << std::endl;
            return false;
        }

        if(setjmp(png_jmpbuf(pngPtr))) {
            png_destroy_write_struct(&pngPtr, &infoPtr);
            std::cerr << "PNG写入过程中发生错误" << std::endl;
            return false;
        }

        png_set_write_fn(
            pngPtr, &file,
            [](png_structp pngPtr, png_bytep data, png_size_t length) {
                std::ofstream *file = static_cast<std::ofstream *>(png_get_io_ptr(pngPtr));
                file->write(reinterpret_cast<char *>(data), length);
            },
            nullptr);

        png_set_IHDR(pngPtr, infoPtr, width, height, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        std::vector<png_bytep> rowPointers(height);
        for(uint32_t y = 0; y < height; y++) {
            rowPointers[y] = imageData.data() + y * width;
        }

        png_write_info(pngPtr, infoPtr);
        png_write_image(pngPtr, rowPointers.data());
        png_write_end(pngPtr, nullptr);

        png_destroy_write_struct(&pngPtr, &infoPtr);
        return true;
    }

    // 从PNG恢复文件
    bool restoreFromPng(const fs::path &pngPath, const fs::path &outputPath)
    {
        std::ifstream file(pngPath, std::ios::binary);
        if(!file.is_open()) {
            std::cerr << "无法打开PNG文件: " << pngPath << std::endl;
            return false;
        }

        // 读取PNG文件头
        png_byte header[8];
        file.read(reinterpret_cast<char *>(header), 8);
        if(png_sig_cmp(header, 0, 8)) {
            std::cerr << "不是有效的PNG文件: " << pngPath << std::endl;
            return false;
        }

        png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if(!pngPtr) {
            std::cerr << "无法创建PNG读结构" << std::endl;
            return false;
        }

        png_infop infoPtr = png_create_info_struct(pngPtr);
        if(!infoPtr) {
            png_destroy_read_struct(&pngPtr, nullptr, nullptr);
            std::cerr << "无法创建PNG信息结构" << std::endl;
            return false;
        }

        if(setjmp(png_jmpbuf(pngPtr))) {
            png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
            std::cerr << "PNG读取过程中发生错误" << std::endl;
            return false;
        }

        png_set_read_fn(pngPtr, &file, [](png_structp pngPtr, png_bytep data, png_size_t length) {
            std::ifstream *file = static_cast<std::ifstream *>(png_get_io_ptr(pngPtr));
            file->read(reinterpret_cast<char *>(data), length);
        });

        png_read_info(pngPtr, infoPtr);

        width = png_get_image_width(pngPtr, infoPtr);
        height = png_get_image_height(pngPtr, infoPtr);
        png_byte colorType = png_get_color_type(pngPtr, infoPtr);
        png_byte bitDepth = png_get_bit_depth(pngPtr, infoPtr);

        if(colorType != PNG_COLOR_TYPE_GRAY || bitDepth != 8) {
            std::cerr << "不支持的PNG格式: 必须是8位灰度图像" << std::endl;
            png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
            return false;
        }

        std::vector<png_bytep> rowPointers(height);
        std::vector<uint8_t> imageData(width * height);

        for(uint32_t y = 0; y < height; y++) {
            rowPointers[y] = imageData.data() + y * width;
        }

        png_read_image(pngPtr, rowPointers.data());
        png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);

        // 从图像数据中提取文件大小
        fileSize = (imageData[0] << 24) | (imageData[1] << 16) | (imageData[2] << 8) | imageData[3];

        // 检查是否有足够的数据
        if(width * height < fileSize + 4) {
            std::cerr << "PNG文件不包含足够的原始数据" << std::endl;
            return false;
        }

        // 写入原始文件
        std::ofstream outFile(outputPath, std::ios::binary);
        if(!outFile.is_open()) {
            std::cerr << "无法创建输出文件: " << outputPath << std::endl;
            return false;
        }

        outFile.write(reinterpret_cast<char *>(imageData.data() + 4), fileSize);
        return true;
    }

    void printInfo() const
    {
        std::cout << "文件大小: " << fileSize << " 字节\n";
        std::cout << "图像尺寸: " << width << " x " << height << "\n";
        std::cout << "总像素数: " << requiredPixels << "\n";
        std::cout << "填充像素: " << (requiredPixels - fileSize - 4) << "\n";
    }
};

int main()
{
    auto encode_mode{true};
    fs::path inputPath("");
    fs::path outputPath("");

    FileToPngConverter converter;

    if(encode_mode) {
        if(!fs::exists(inputPath)) {
            std::cerr << "输入文件不存在: " << inputPath << std::endl;
            return 1;
        }

        std::cout << "正在处理文件..." << std::endl;
        if(!converter.readFile(inputPath)) {
            return 1;
        }

        converter.printInfo();

        std::cout << "正在生成PNG图像..." << std::endl;
        if(!converter.exportToPng(outputPath)) {
            return 1;
        }

        std::cout << "成功生成PNG图像: " << outputPath << std::endl;
    } else {
        std::cout << "正在从PNG恢复文件..." << std::endl;
        if(!converter.restoreFromPng(inputPath, outputPath)) {
            return 1;
        }

        std::cout << "成功恢复文件: " << outputPath << std::endl;
    }

    return 0;
}
