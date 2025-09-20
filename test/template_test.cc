#include <core/WukConfig.hh>
#include <core/WukException.hh>

#include <iostream>

namespace template_test {
    enum class ExecType {
        WRITE, READ
    };

    class MyClass {
    public:
        MyClass() = default;

        template <ExecType T>
        void exec(const std::string &sql) = delete;
    };

    template <>
    void MyClass::exec<ExecType::WRITE>(const std::string &sql)
    {
        std::cout << "Executing WRITE operation: " << sql << std::endl;
    }

    template <>
    void MyClass::exec<ExecType::READ>(const std::string &sql)
    {
        std::cout << "Executing READ operation: " << sql << std::endl;
    }
}

void test()
{
    template_test::MyClass a;
    a.exec<template_test::ExecType::READ>("SELECT * FROM users;");
}

int main()
{
    test();

    return 0;
}
