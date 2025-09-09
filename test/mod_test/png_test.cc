#include <core/WukConfig.hh>
#include <utils/bytes.hh>
#include <WukRandom.hh>
#include <WukMemory.hh>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <cmath>

#include <png.h>

namespace fs = std::filesystem;

// 准备图像数据
std::vector<wuk::byte> prepare_image_data(const std::vector<wuk::byte> &file_data,
                                        wuk::u32 &width, wuk::u32 &height)
{
    wuk::u32 file_size = file_data.size();
    wuk::ulong required_pixels = [&](wuk::u32 file_size) {
        width = static_cast<wuk::u32>(std::sqrt(file_size));
        height = width;
        while ((width * height) < file_size) { width++; }
        return width * height;
    } (file_size);

    std::vector<wuk::byte> image_data(required_pixels);

    wuk::utils::pack_bytes<wuk::u32, true>(image_data.data(), image_data.size(), file_size);

    std::copy(file_data.begin(), file_data.end(), image_data.begin() + sizeof(file_size));

    if(required_pixels > (file_size + sizeof(file_size))) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        for(wuk::ulong i = file_size + sizeof(file_size); i < required_pixels; i++) {
            image_data[i] = static_cast<wuk::byte>(dis(gen));
        }
    }

    return image_data;
}

std::vector<wuk::byte> file_read(const fs::path &path)
{
    if (!fs::exists(path)) {
        return {};
    }
    wuk::ulong file_length = fs::file_size(path);
    if (file_length == 0) {
        return {};
    }
    std::fstream file(path, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        return {};
    }

    std::vector<wuk::byte> file_data(file_length, 0);

    file.read(reinterpret_cast<char *>(file_data.data()), file_length);

    return file_data;
}

class LIBWUK_API FileToPngConverter {
private:
    std::vector<wuk::byte> file_data;

public:
    bool export_to_png(const fs::path &input_path, const fs::path &output_path)
    {
        wuk::u32 width{0};
        wuk::u32 height{0};

        std::vector<wuk::byte> file_data = file_read(input_path);
        if (file_data.empty()) {
            std::cerr << "无法读取文件，文件不存在或文件为空或无权访问文件。" << std::endl;
            return false;
        }

        std::ofstream file(output_path, std::ios::binary);
        if(!file.is_open()) {
            std::cerr << "无法创建输出文件: " << output_path << std::endl;
            return false;
        }

        std::vector<wuk::byte> image_data = prepare_image_data(file_data, width, height);

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                    nullptr, nullptr, nullptr);
        if(!png_ptr) {
            std::cerr << "无法创建PNG写结构" << std::endl;
            return false;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if(!info_ptr) {
            png_destroy_write_struct(&png_ptr, nullptr);
            std::cerr << "无法创建PNG信息结构" << std::endl;
            return false;
        }

        if(setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            std::cerr << "PNG写入过程中发生错误" << std::endl;
            return false;
        }

        png_set_write_fn(
            png_ptr, &file,
            [](png_structp png_ptr, png_bytep data, png_size_t length) {
                std::ofstream *file = static_cast<std::ofstream *>(png_get_io_ptr(png_ptr));
                file->write(reinterpret_cast<char *>(data), length);
            },
            nullptr);

        png_set_IHDR(
            png_ptr, info_ptr, width, height,
            8,
            PNG_COLOR_TYPE_GRAY,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE,
            PNG_FILTER_TYPE_BASE
        );

        std::vector<png_bytep> row_pointers(height);
        for(wuk::u32 y = 0; y < height; y++) {
            row_pointers[y] = image_data.data() + y * width;
        }

        png_write_info(png_ptr, info_ptr);
        png_write_image(png_ptr, row_pointers.data());
        png_write_end(png_ptr, nullptr);

        png_destroy_write_struct(&png_ptr, &info_ptr);
        return true;
    }

    bool restore_from_png(const fs::path &pngPath, const fs::path &outputPath)
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

        wuk::u32 width = png_get_image_width(pngPtr, infoPtr);
        wuk::u32 height = png_get_image_height(pngPtr, infoPtr);
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
        wuk::u32 fileSize = (imageData[0] << 24) | (imageData[1] << 16) | (imageData[2] << 8) | imageData[3];

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
};

int main()
{
    fs::path input_path("D:/Z_SSS/131166877_p0.png");
    fs::path output_path("test.png");

    FileToPngConverter converter;

    constexpr auto encode_mode{true};
    if(encode_mode) {
        std::cout << "正在生成PNG图像..." << std::endl;
        if(!converter.export_to_png(input_path, output_path)) {
            return 1;
        }

        std::cout << "成功生成PNG图像: " << output_path << std::endl;
    } else {
        std::cout << "正在从PNG恢复文件..." << std::endl;
        if(!converter.restore_from_png(input_path, output_path)) {
            return 1;
        }

        std::cout << "成功恢复文件: " << output_path << std::endl;
    }

    return 0;
}
