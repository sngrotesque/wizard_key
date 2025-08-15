#include <config/WukConfig.hh>

#include <iostream>
using namespace std;

class MyClass {
private:
    uint32_t val;

public:
    MyClass() noexcept
    : val(0)
    {
        cout << "MyClass::MyClass 1\n";
    }

    MyClass(uint32_t val) noexcept
    : val(val)
    {
        cout << "MyClass::MyClass 2\n";
    }

    ~MyClass()
    {
        cout << "MyClass::~MyClass\n";
    }

    MyClass(const MyClass &other) noexcept
    {
        this->val = other.val;
    }

    MyClass(MyClass &&other) noexcept
    {
        this->val = other.val;
        other.val = 0;
    }

    MyClass &operator=(const MyClass &other) noexcept
    {
        if(this == &other) {
            return *this;
        }
        this->val = other.val;
        return *this;
    }

    MyClass &operator=(MyClass &&other) noexcept
    {
        if(this == &other) {
            return *this;
        }
        this->val = other.val;
        other.val = 0;
        return *this;
    }

    MyClass operator+(size_t n) noexcept
    {
        MyClass result{*this};
        result.val += n;
        return result;// 返回临时对象（将触发拷贝构造函数或RVO）
    }

    MyClass operator+(MyClass &other) noexcept
    {
        MyClass result{*this};
        result.val += other.val;
        return result;// 返回临时对象（将触发拷贝构造函数或RVO）
    }

    // 如果不返回引用则会性能问题和定义问题和赋值问题
    MyClass &operator+=(size_t n) noexcept
    {
        this->val += n;
        return *this;
    }

    MyClass &operator++() noexcept
    {
        ++this->val;
        return *this;
    }

    MyClass &operator--() noexcept
    {
        --this->val;
        return *this;
    }

    MyClass operator++(int) noexcept
    {
        MyClass temp{*this};
        ++(*this);
        return temp;
    }

    MyClass operator--(int) noexcept
    {
        MyClass temp{*this};
        --(*this);
        return temp;
    }

    bool operator==(MyClass &other) noexcept
    {
        return (this->val == other.val);
    }

    wU32 get_val() const
    {
        return this->val;
    }
};

int main()
{
    MyClass a;
    MyClass b{2};
    a = 1;

    MyClass c = a + b;

    cout << c.get_val() << endl;

    c += 45;

    cout << c.get_val() << endl;

    b = 48;

    cout << (c == b) << endl;

    return 0;
}
