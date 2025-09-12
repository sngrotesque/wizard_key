#include <core/WukConfig.hh>
#include <utils/bytes.hh>
#include <WukRandom.hh>
#include <WukMemory.hh>
#include <WukMisc.hh>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <cmath>
#include <functional>

#include <png.h>

namespace fs = std::filesystem;
std::function<std::string(const std::string &)> wuk_misc_log = wuk::misc::log_utf8;

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
            std::cerr << wuk_misc_log("无法读取文件，文件不存在或文件为空或无权访问文件。") << std::endl;
            return false;
        }

        std::ofstream file(output_path, std::ios::binary);
        if(!file.is_open()) {
            std::cerr << wuk_misc_log(fmt::format("无法创建输出文件: {0}", output_path.string())) << std::endl;
            return false;
        }

        std::vector<wuk::byte> image_data = prepare_image_data(file_data, width, height);

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                    nullptr, nullptr, nullptr);
        if(!png_ptr) {
            std::cerr << wuk_misc_log("无法创建PNG写结构") << std::endl;
            return false;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if(!info_ptr) {
            png_destroy_write_struct(&png_ptr, nullptr);
            std::cerr << wuk_misc_log("无法创建PNG信息结构") << std::endl;
            return false;
        }

        if(setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            std::cerr << wuk_misc_log("PNG写入过程中发生错误") << std::endl;
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

    /* ---------- 辅助：把 istream 包装成 libpng 能用的“读回调” ---------- */
    struct PngIstreamWrapper {
        std::istream &in;
        static void readCallback(png_structp png, png_bytep dst, png_size_t size) {
            auto *self = static_cast<PngIstreamWrapper*>(png_get_io_ptr(png));
            if (!self->in.read(reinterpret_cast<char*>(dst), size))
                png_error(png, "istream::read 失败（可能提前到达 EOF）");
        }
    };

    /* -------------------------- restore_from_png -------------------------- */
    bool restore_from_png(const fs::path &pngPath, const fs::path &outputPath)
    {
        /* 1. 打开文件 */
        std::ifstream fin(pngPath, std::ios::binary);
        if (!fin)
            return [](const auto &e){ std::cerr << wuk_misc_log(e) << std::endl; return false; }
                ("无法打开输入 PNG 文件");

        /* 2. 校验 PNG 签名（8 字节） */
        char sig[8];
        if (!fin.read(sig, 8) || !fin.good() || png_sig_cmp(reinterpret_cast<png_bytep>(sig), 0, 8))
            return [](const auto &e){ std::cerr << wuk_misc_log(e) << std::endl; return false; }
                ("输入文件不是有效的 PNG");

        /* 3. 创建读结构 */
        png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                                nullptr, nullptr, nullptr);
        if (!png)
            return [](const auto &e){ std::cerr << wuk_misc_log(e) << std::endl; return false; }
                ("png_create_read_struct 失败");

        png_infop info = png_create_info_struct(png);
        if (!info) {
            png_destroy_read_struct(&png, nullptr, nullptr);
            return [](const auto &e){ std::cerr << wuk_misc_log(e) << std::endl; return false; }
                ("png_create_info_struct 失败");
        }

        if (setjmp(png_jmpbuf(png))) {          /* libpng 内部错误跳转点 */
            png_destroy_read_struct(&png, &info, nullptr);
            return [](const auto &e){ std::cerr << wuk_misc_log(e) << std::endl; return false; }
                ("PNG 解析过程中发生错误");
        }

        /* 4. 把自定义读回调挂到 libpng */
        PngIstreamWrapper wrapper{fin};
        png_set_read_fn(png, &wrapper, PngIstreamWrapper::readCallback);

        /* 5. 读取 IHDR */
        png_set_sig_bytes(png, 8);              /* 告诉 libpng 签名已读过 */
        png_read_info(png, info);

        // png_uint_32 width  = png_get_image_width(png, info);
        png_uint_32 height = png_get_image_height(png, info);
        int bit_depth  = png_get_bit_depth(png, info);
        int color_type = png_get_color_type(png, info);

        if (bit_depth != 8 || color_type != PNG_COLOR_TYPE_GRAY)
            png_error(png, "PNG 必须是 8bit 灰度图");

        /* 6. 读像素 */
        std::size_t rowBytes = png_get_rowbytes(png, info);
        std::vector<png_byte> img(rowBytes * height);
        std::vector<png_bytep> rows(height);
        for (png_uint_32 y = 0; y < height; ++y)
            rows[y] = img.data() + y * rowBytes;

        png_read_image(png, rows.data());
        png_read_end(png, nullptr);
        png_destroy_read_struct(&png, &info, nullptr);
        fin.close();

        /* 7. 解出文件长度 */
        if (img.size() < sizeof(wuk::u32))
            return [](const auto &e){ std::cerr << wuk_misc_log(e) << std::endl; return false; }
                ("像素区不足以存放文件长度");

        wuk::u32 fileSize = wuk::utils::unpack_bytes<wuk::u32, true>(img.data(), sizeof(wuk::u32));
        if (fileSize + sizeof(wuk::u32) > img.size())
            return [](const auto &e){ std::cerr << wuk_misc_log(e) << std::endl; return false; }
                ("文件长度字段与实际像素数据不符");

        /* 8. 写回原始文件 */
        std::ofstream out(outputPath, std::ios::binary);
        if (!out)
            return [](const auto &e){ std::cerr << wuk_misc_log(e) << std::endl; return false; }
                ("无法创建输出文件");

        out.write(reinterpret_cast<const char*>(img.data() + sizeof(wuk::u32)), fileSize);
        return out.good();
    }
};

int main()
{
    fs::path original_path(R"(F:\Misc\doi\VID_Full.mp4)");
    fs::path encoded_path("L:/test测试.png");
    fs::path restored_path("L:/test.mp4");

    try {
        FileToPngConverter converter;

        std::cout << wuk_misc_log("正在生成PNG图像...") << std::endl;
        if(!converter.export_to_png(original_path, encoded_path)) {
            return 1;
        }
        std::cout << wuk_misc_log(fmt::format("成功生成PNG图像：{0}", encoded_path.string())) << std::endl;

        std::cout << wuk_misc_log("正在从PNG恢复文件...") << std::endl;
        if(!converter.restore_from_png(encoded_path, restored_path)) {
            return 1;
        }
        std::cout << wuk_misc_log(fmt::format("成功恢复文件：{0}", restored_path.string())) << std::endl;
    } catch (const wuk::Exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
