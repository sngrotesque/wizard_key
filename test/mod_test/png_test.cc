#include <core/WukConfig.hh>
#include <utils/number.hh>
#include <WukBuffer.hh>
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

// 准备图像数据
wuk::Buffer prepare_image_data(const wuk::Buffer &file_data,
                            wuk::u32 &width,
                            wuk::u32 &height)
{
    wuk::u32 file_size = static_cast<wuk::u32>(file_data.size());
    wuk::u32 required_pixels = [&]()
    {
        width = static_cast<wuk::u32>(std::sqrt(file_size));
        height = width;
        while ((width * height) < file_size) { width++; }
        return width * height;
    } ();

    wuk::Buffer image_data(required_pixels);
    wuk::Random random;

    image_data += wuk::utils::pack_bytes<wuk::u32, true>(file_size);
    image_data += file_data;

    if (required_pixels < (file_size + sizeof(file_size))) {
        return image_data;
    }

    for(wuk::ulong i = file_size + sizeof(file_size); i < required_pixels; ++i) {
        image_data[i] = static_cast<wuk::byte>(random.randint(0x00, 0xff));
    }
    return image_data;
}

static wuk::Buffer file_read(const fs::path &path)
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
    wuk::Buffer file_data(file_length, 0);

    file.read(
        reinterpret_cast<char *>(file_data.write(file_length)),
        file_length
    );

    return file_data;
}

class LIBWUK_API FileToPngConverter {
public:
    bool export_to_png(const fs::path &input_path, const fs::path &output_path)
    {
        wuk::u32 width{0};
        wuk::u32 height{0};

        wuk::Buffer file_data = file_read(input_path);
        if (file_data.empty()) {
            fmt::println("无法读取文件，文件不存在或文件为空或无权访问文件。");
            return false;
        }

        std::ofstream file(output_path, std::ios::binary);
        if(!file.is_open()) {
            fmt::println("无法创建输出文件: {0}。", output_path.string());
            return false;
        }

        wuk::Buffer image_data = prepare_image_data(file_data, width, height);

        png_structp png_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING,
            nullptr, nullptr, nullptr
        );
        if(!png_ptr) {
            fmt::println("无法创建PNG写结构。");
            return false;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if(!info_ptr) {
            png_destroy_write_struct(&png_ptr, nullptr);
            fmt::println("无法创建PNG信息结构。");
            return false;
        }

        if(setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            fmt::println("PNG写入过程中发生错误。");
            return false;
        }

        png_set_write_fn(
            png_ptr,
            &file,
            [](png_structp png_ptr, png_bytep data, png_size_t length)
            {
                std::ofstream *file = static_cast<std::ofstream *>(png_get_io_ptr(png_ptr));
                file->write(reinterpret_cast<char *>(data), length);
            },
            nullptr
        );

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
            row_pointers[y] = const_cast<wuk::byte *>(image_data.data() + y * width);
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

        static void readCallback(png_structp png, png_bytep dst, png_size_t length)
        {
            auto *self = static_cast<PngIstreamWrapper *>(png_get_io_ptr(png));
            if (!self->in.read(reinterpret_cast<char *>(dst), length)) {
                png_error(png, "istream::read 失败（可能提前到达 EOF）");
            }
        }
    };

    /* -------------------------- restore_from_png -------------------------- */
    bool restore_from_png(const fs::path &pngPath, const fs::path &outputPath)
    {
        /* 1. 打开文件 */
        std::ifstream fin(pngPath, std::ios::binary);
        if (!fin) {
            fmt::println("无法打开文件。"); return false;
        }

        /* 2. 校验 PNG 签名（8 字节） */
        char sig[8] {};
        if (!fin.read(sig, 8) || !fin.good() || png_sig_cmp(reinterpret_cast<png_bytep>(sig), 0, 8)) {
            fmt::println("输入的文件不是有效的PNG文件。"); return false;
        }

        /* 3. 创建读结构 */
        png_structp png = png_create_read_struct(
            PNG_LIBPNG_VER_STRING,
            nullptr, nullptr, nullptr
        );
        if (!png) {
            fmt::println("png_create_read_struct 失败。"); return false;
        }

        png_infop info = png_create_info_struct(png);
        if (!info) {
            png_destroy_read_struct(&png, nullptr, nullptr);
            fmt::println("png_create_info_struct 失败。"); return false;
        }

        /* libpng 内部错误跳转点 */
        if (setjmp(png_jmpbuf(png))) {
            png_destroy_read_struct(&png, &info, nullptr);
            fmt::println("PNG 解析过程中发生错误。"); return false;
        }

        /* 4. 把自定义读回调挂到 libpng */
        PngIstreamWrapper wrapper{fin};
        png_set_read_fn(png, &wrapper, PngIstreamWrapper::readCallback);

        /* 5. 读取 IHDR */
        png_set_sig_bytes(png, 8); // 告诉 libpng 签名已读过
        png_read_info(png, info);

        [[maybe_unused]] png_uint_32 width  = png_get_image_width(png, info);
        png_uint_32 height = png_get_image_height(png, info);
        int bit_depth  = png_get_bit_depth(png, info);
        int color_type = png_get_color_type(png, info);

        if (bit_depth != 8 || color_type != PNG_COLOR_TYPE_GRAY) {
            png_error(png, "PNG 必须是 8bit 灰度图");
        }

        /* 6. 读像素 */
        std::size_t rowBytes = png_get_rowbytes(png, info);
        std::vector<png_byte> img(rowBytes * height);
        std::vector<png_bytep> rows(height);
        for (png_uint_32 y = 0; y < height; ++y) {
            rows[y] = img.data() + y * rowBytes;
        }

        png_read_image(png, rows.data());
        png_read_end(png, nullptr);
        png_destroy_read_struct(&png, &info, nullptr);
        fin.close();

        /* 7. 解出文件长度 */
        if (img.size() < sizeof(wuk::u32)) {
            fmt::println("像素区不足以存放文件长度。"); return false;
        }

        wuk::Buffer image_data{img.data(), img.size()};
        wuk::u32 fileSize = wuk::utils::unpack_bytes<wuk::u32, true>(image_data);
        if ((fileSize + sizeof(wuk::u32)) > img.size()) {
            fmt::println("文件长度字段与实际像素数据不符。"); return false;
        }

        /* 8. 写回原始文件 */
        std::ofstream fout(outputPath, std::ios::binary);
        if (!fout) {
            fmt::println("无法创建输出文件。"); return false;
        }
        fout.write(image_data.c_str() + sizeof(wuk::u32), fileSize);
        return fout.good();
    }
};

int main()
{
    fs::path original_path(R"(L:\p_cookie.txt)");
    fs::path encoded_path("L:/12321312312312312312331232131232321323.png");
    fs::path restored_path("L:/12321312312312312312331232131232321323.plain.txt");

    try {
        FileToPngConverter converter;

        fmt::println("正在生成PNG图像。");
        if(!converter.export_to_png(original_path, encoded_path)) {
            return 1;
        }
        fmt::println("成功生成了PNG图像：{0}。", encoded_path.string());

        fmt::println("正在从PNG恢复文件。");
        if(!converter.restore_from_png(encoded_path, restored_path)) {
            return 1;
        }
        fmt::println("成功恢复文件：{0}。", restored_path.string());
    } catch (const wuk::Exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
