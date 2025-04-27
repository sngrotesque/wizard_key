#include <WukUtils.hh>

#include <WukUtils.cc>
#include <WukRandom.cc>

int main()
{
    std::string path = "W:/secure_delete_file_test.txt";
    wuk::Utils utils;
    wU32 passes = 13;

    try {
        utils.secure_delete_file(path, passes);
    } catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    } catch (const wuk::Exception &e) {
        std::cerr << "Wuk Exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        return 1;
    }

    return 0;
}
