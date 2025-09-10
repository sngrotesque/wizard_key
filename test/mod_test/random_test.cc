#include <WukBuffer.hh>
#include <WukRandom.hh>

#include <iostream>

void test_1()
{
    std::vector<int> arr{1,2,3,4,5,6,7,8,9};
    wuk::Random random;

    random.shuffle(arr);

    for (auto &item : arr) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

void test_2()
{
    wuk::Random random;
    std::cout << "random.random:  " << random.random() << std::endl;
    std::cout << "random.randint: " << random.randint(0x00, 0xff) << std::endl;
    std::cout << "random.uniform: " << random.uniform(3.0, 7.2) << std::endl;
    std::cout << "random.bytes:   " << random.bytes(4).to_hex() << std::endl;
}

int main()
{
    test_2();

    return 0;
}
