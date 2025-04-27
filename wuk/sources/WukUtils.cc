#include <WukUtils.hh>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

wSize get_file_size(const std::string &path)
{
    return fs::file_size(path);
}

void overwrite_file(const std::string &path, bool is_zero)
{
    wuk::Random random;
    wByte random_data[4096]{};

    std::fstream file(path, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open file: " + path);
    }
    file.sync_with_stdio(false);

    wSize file_size_left = get_file_size(path);

    for (wSize i = 0; i < file_size_left; i += sizeof(random_data)) {
        wSize bytes_to_write = std::min(sizeof(random_data), file_size_left - i);

        if (!is_zero) {
            random.urandom(random_data, bytes_to_write);
        }

        file.write(reinterpret_cast<const char *>(random_data), bytes_to_write);
        file.flush();
    }

    file.close();
}

void wuk::Utils::secure_delete_file(const std::string &path, wU32 passes)
{
    if (!fs::exists(path)) {
        throw std::ios_base::failure("File does not exist: " + path);
    }

    // 3 passes of overwriting the file with random data
    for (wU32 i = 0; i < passes; ++i) {
        overwrite_file(path, false);
    }
    // 1 pass of overwriting the file with zeros
    overwrite_file(path, true);

    if (fs::remove(path)) {
        std::cout << "File deleted successfully: " << path << std::endl;
    } else {
        std::cerr << "Failed to delete file: " << path << std::endl;
    }
}



