#include <crypto/WukHash.hh>
#include <WukBuffer.hh>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>

namespace fs = std::filesystem;

constexpr wuk::ulong block_size = 16777216; // 16 MB

template <typename T>
wuk::Buffer sha256_digest(const T &);

template <>
wuk::Buffer sha256_digest<fs::path>(const fs::path &path)
{
    if (!fs::exists(path)) {
        throw wuk::Exception(wuk::Error::FNOTF, "sha256_digest<fs::path>",
            "File path not found.");
    }
    wuk::crypto::Hashlib<wuk::crypto::HashlibType::SHA_256> hash;

    std::fstream file(path, std::ios::binary | std::ios::in);

    wuk::Buffer file_buffer(block_size);
    for (;;) {
        char *ptr = reinterpret_cast<char *>(file_buffer.write(block_size));
        wuk::ulong read_length = file.read(ptr, block_size).gcount();
        if (read_length == 0) {
            break;
        }
        hash.update(file_buffer.data(), read_length);
    }

    return hash.digest();
}

template <>
wuk::Buffer sha256_digest<wuk::Buffer>(const wuk::Buffer &buffer)
{
    wuk::crypto::Hashlib<wuk::crypto::HashlibType::SHA_256> hash;

    hash.update(buffer);

    return hash.digest();
}

int main()
{
    {
        auto digest = sha256_digest<fs::path>("F:/Pitchers/QQ/QQ_Images/a58e1d2686f1df5db1ea23a576a97b31.jpg");
        std::cout << "file digest: " << digest.to_hex() << std::endl;
    }

    {
        auto digest = sha256_digest<wuk::Buffer>(wuk::Buffer("hello, world"));
        std::cout << "data digest: " << digest.to_hex() << std::endl;
    }

    return 0;
}
