---
title: Day 7 继承、多态和虚函数强化
created: 2026-10-10
updated: 2026-10-10
tags: [cpp, day07, inheritance, polymorphism, virtual]
status: active
source: 黑马程序员 110-117、127-138；C++ Primer 15.2-15.4
---

# Day 7 继承、多态和虚函数强化

## 今日目标

- 复习类成员、静态成员和 const 成员函数。
- 掌握继承中的构造析构顺序。
- 掌握基类指针和基类引用。
- 掌握虚函数、动态绑定和虚析构函数。
- 掌握纯虚函数和抽象基类。
- 完成 `day07_polymorphism_abstract.cpp`。

## 1. 继承中的构造和析构

~~~cpp
class Base {
public:
    Base() {
        cout << "Base construct" << endl;
    }

    virtual ~Base() {
        cout << "Base destruct" << endl;
    }
};

class Derived : public Base {
public:
    Derived() {
        cout << "Derived construct" << endl;
    }

    ~Derived() override {
        cout << "Derived destruct" << endl;
    }
};
~~~

构造顺序：

~~~text
Base -> Derived
~~~

析构顺序：

~~~text
Derived -> Base
~~~

## 2. 虚函数

~~~cpp
class Employee {
public:
    virtual double salary() const = 0;

    virtual void print() const = 0;

    virtual ~Employee() {
    }
};
~~~

`virtual` 让函数在运行时根据实际对象类型选择版本。

## 3. 动态绑定

~~~cpp
Engineer engineer(1, 8000.0, 2000.0);
Employee* employee = &engineer;

employee->print();
~~~

只要调用的是虚函数，就会根据对象真实类型执行对应函数。

## 4. 抽象基类

含有纯虚函数的类不能创建对象：

~~~cpp
Employee employee; // 错误：抽象基类
~~~

但是可以使用基类指针或引用：

~~~cpp
Employee* employee = &engineer;
Employee& reference = manager;
~~~

## 5. override

派生类覆盖虚函数时，建议使用 `override`：

~~~cpp
class Engineer : public Employee {
public:
    double salary() const override {
        return baseSalary + bonus;
    }
};
~~~

如果函数签名写错，`override` 会帮助编译器发现错误。

## 6. 虚析构函数

如果使用基类指针管理派生类对象，基类析构函数通常应该是虚函数：

~~~cpp
virtual ~Employee() = default;
~~~

否则通过基类指针销毁派生类对象时，可能只调用基类析构函数。

## 7. 今日代码

- `cpp/day07/day07_polymorphism_abstract.cpp`

## 8. 今日验收

- 构造和析构顺序是什么？
- 虚函数解决了什么问题？
- 动态绑定需要哪些条件？
- 纯虚函数和普通虚函数有什么区别？
- 抽象基类能不能创建对象？
- `override` 有什么作用？
- 为什么基类析构函数通常要写成虚函数？
- 对象切片是什么？
