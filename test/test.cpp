#include <fstream>
#include <stdexcept>

#include <iostream>
#include <filesystem>

#include <network/wmkc_net.c>
#include <network/wmkc_ssl.c>
#include <crypto/snc.c>
#include <wmkc_memory.c>
#include <wmkc_struct.c>
#include <wmkc_file.c>
#include <wmkc_misc.c>
#include <wmkc_time.c>

using namespace std;

class Human {
    public:
        string name;
        string like;
        wmkc_u32 age;

        virtual void sleep() = 0;
        virtual void eat() = 0;
};

class Tom : public Human {
    public:
        void eat() override {
            cout << this->name + " Eat." << endl;
        }

        void sleep() override {
            cout << this->name + " Sleep." << endl;
        }
};

int main(int argc, char **argv)
{
    Tom a;
    a.name = "Tom";
    a.age = 17;
    a.like = "play game";
    a.sleep();
    return 0;
}
