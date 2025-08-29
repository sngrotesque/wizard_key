// pacman -S mingw-w64-x86_64-fmt
// pacman -S mingw-w64-x86_64-cpr
#include <WukRandom.hh>

#include <regex>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <fmt/core.h>
#include <cpr/cpr.h>

namespace fs = std::filesystem;

static const std::vector<std::string> UserAgentList = {
    "Mozilla/5.0 (X11; Linux x86_64; rv:129.0) Gecko/20100101 Firefox/129.0",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/127.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; ) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.6478.61 Chrome/126.0.6478.61 Not/A)Brand/8  Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:127.0) Gecko/20100101 Firefox/127.0 Config/100.2.9281.82",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/125.0.0.0 Safari/537.36 OPR/111.0.0.0 (Edition Yx 05)",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/127.0.0.0 Safari/537.36 Edg/128.0.0.0",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:129.0) Gecko/20100101 Firefox/129.0",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/127.0.0.0 Safari/537.36 Edg/128.0.0.0"
};
static wuk::Random random;

void test()
{
    cpr::Header headers = {
        {"Referer", "https://www.pixiv.net/"},
        {"User-Agent", UserAgentList[random.randint(0, UserAgentList.size() - 1)]}
    };

    cpr::Proxies proxies = {
        {"http", "http://127.0.0.1:1080"},
        {"https", "http://127.0.0.1:1080"}
    };

    cpr::Response r = cpr::Get(
        cpr::Url{"https://i.pximg.net/img-original/img/2025/04/09/15/32/18/129121628_p0.jpg"},
        headers,
        proxies
    );

    std::string path("test/http/test.jpg");

    if (r.status_code == 200) {
        std::cout << fmt::format("Download successful, save to {}.", path) << std::endl;
        std::fstream out(fs::path(path), std::ios::binary | std::ios::out);
        out.write(r.text.c_str(), r.text.size());
    } else {
        std::cerr << fmt::format("Download failed, error code: {}.", r.status_code) << std::endl;
    }
}

int main()
{
    test();

    return 0;
}
