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

// 定义一个派生类
class Tom : public Human {
    // 必须使用public关键字来继承基类，否则不能使用其中的成员
    public:
        void sleep() override {
            cout << this->name + " sleep." << endl;
        }
}
```
