# C++的知识

### 类
```cpp
// 定义一个正常类
class Human {
    public:
        string name;
        void sleep() {
            cout << this->name + " sleep." << endl;
        }
};

// 定义一个基类（接口），可实例化
class Human {
    public:
        string name;
        virtual void sleep() {} // 虚函数
        virtual void sleep(); // 也是虚函数，但是需要有实现
};

// 定义一个抽象类（接口），不可实例化，只要包含一个纯虚函数那么这个类就是抽象类
class Human {
    public:
        string name;
        virtual void sleep() = 0; // 纯虚函数
};

// 定义一个派生类，必须使用public关键字来继承基类，否则不能使用其中的成员
class Tom : public Human {
    public:
        void sleep() override {
            cout << this->name + " sleep." << endl;
        }
}

// 初始化列表
class Human {
    public:
        double money;
        Human(double m): money(m) { // 此处是将参数为成员monry赋值
            // ...
        }
};
class Human {
    public:
        double money;
        Human() : money() { // 此处是将money初始化为0
            // ...
        }
};

/**
 * 此处有一个注意事项，如果派生类中使用的是父类的数据成员并且需要初始化
 * 那么必须在父类定义构造函数并初始化列表，不能直接在派生类调用初始化列表来
 * 初始化父类的数据成员
 */
// 正确代码
class information {
    public:
        string name, like;
        ushort age;
        information(string Name, string Like, ushort Age)
        : name(Name), like(Like), age(Age) {}
};
class Human :public information {
    public:
        Human(string _name, string _like, ushort _age)
        : information(_name, _like, _age) {}
        void sleep() override {/**/}
        void walk() override {/**/}
        void play() override {/**/}
};
// 错误代码
class information {
    public:
        string name, like;
        ushort age;
};
class Human :public information {
    public:
        Human(string _name, string _like, ushort _age)
        : name(_name), like(_like), age(_age) {}
        // .....
};


```
