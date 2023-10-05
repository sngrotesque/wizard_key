#include <iostream>
#include <cstdint>
#include <format>

using namespace std;

typedef uint16_t ushort;

class behavior {
    public:
        virtual void sleep() = 0;
        virtual void walk() = 0;
        virtual void play() = 0;
};

class information {
    public:
        string name;
        string like;
        ushort age;

        information(string Name, string Like, ushort Age)
        : name(Name), like(Like), age(Age) {}
};

class Human : public behavior, public information {
    public:
        Human(string _name, string _like, ushort _age)
        : information(_name, _like, _age)
        {
            cout << "初始化成员。" << endl;
        }
        
        void sleep() override {
            cout << format("{0} is sleeping.", this->name) << endl;
        }
        
        void walk() override {
            cout << format("{0} is walking.", this->name) << endl;
        }
        
        void play() override {
            cout << format("{0} is playing the piano.", this->name) << endl;
        }
};

int main(int argc, char **argv)
{
    Human *SN = new Human("SN-Grotesque", "Develop", 21);
    
    cout << format("name: {}, like: {}, age: {}", SN->name, SN->like, SN->age) << endl;
    
    SN->sleep();
    SN->walk();
    SN->play();
    
    delete SN;
    return 0;
}
