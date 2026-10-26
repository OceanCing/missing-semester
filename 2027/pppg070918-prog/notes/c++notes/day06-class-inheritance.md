---
title: Day 6 封装、构造、继承与多态
created: 2026-10-10
updated: 2026-10-10
tags: [cpp, day06, class, inheritance, polymorphism]
status: active
source: 黑马程序员 099-109；C++ Primer 074-077
---

# Day 6 封装、构造、继承与多态

## 今日目标

- 掌握封装和访问权限。
- 掌握构造函数、析构函数和拷贝构造。
- 掌握继承的基本语法。
- 理解基类和派生类的关系。
- 掌握虚函数、动态绑定和抽象基类。
- 完成 `day06_class_inheritance.cpp`。

## 1. 封装

封装是把数据和操作数据的方法放在类中，并通过访问权限控制外部如何使用。

~~~cpp
class Student {
private:
    int id;
    double score;

public:
    Student(int id, double score)
        : id(id), score(score) {
    }

    int getId() const {
        return id;
    }

    double getScore() const {
        return score;
    }

    void setScore(double score) {
        this->score = score;
    }
};
~~~

访问权限：

| 权限 | 类内部 | 派生类 | 类外部 |
| --- | --- | --- | --- |
| `public` | 可以 | 可以 | 可以 |
| `protected` | 可以 | 可以 | 不可以 |
| `private` | 可以 | 不可以 | 不可以 |

封装的好处：

- 隐藏内部实现。
- 控制数据是否合法。
- 减少外部代码对内部数据的直接依赖。

## 2. 构造函数和析构函数

~~~cpp
class Student {
public:
    Student() {
        cout << "constructor" << endl;
    }

    ~Student() {
        cout << "destructor" << endl;
    }
};
~~~

构造函数：

- 创建对象时自动调用。
- 函数名和类名相同。
- 没有返回类型。
- 可以重载。

析构函数：

- 对象销毁时自动调用。
- 函数名为 `~类名`。
- 没有参数。
- 一个类通常只有一个析构函数。

## 3. 拷贝构造

~~~cpp
Student first(1001, 90.0);
Student second = first;
~~~

拷贝构造函数的基本形式：

~~~cpp
Student(const Student& other) {
    id = other.id;
    score = other.score;
}
~~~

如果类中有指针成员，需要重点考虑深拷贝和浅拷贝。

## 4. 继承

~~~cpp
class Animal {
public:
    void eat() {
        cout << "eat" << endl;
    }
};

class Dog : public Animal {
public:
    void bark() {
        cout << "bark" << endl;
    }
};
~~~

`Dog` 是派生类，`Animal` 是基类。

继承后的构造顺序：

1. 基类构造函数。
2. 成员对象构造函数。
3. 派生类构造函数体。

析构顺序相反。

## 5. 虚函数与多态

~~~cpp
class Animal {
public:
    virtual void speak() const {
        cout << "animal" << endl;
    }

    virtual ~Animal() {
    }
};

class Dog : public Animal {
public:
    void speak() const override {
        cout << "dog" << endl;
    }
};
~~~

动态绑定条件：

- 使用基类指针或引用。
- 调用虚函数。
- 指针或引用实际指向派生类对象。

~~~cpp
Dog dog;
Animal& animal = dog;

animal.speak(); // dog
~~~

## 6. 纯虚函数和抽象基类

~~~cpp
class Shape {
public:
    virtual double area() const = 0;

    virtual ~Shape() {
    }
};
~~~

`= 0` 表示纯虚函数。含有纯虚函数的类是抽象类，不能直接创建对象。

派生类必须实现纯虚函数，否则仍然是抽象类。

## 7. 今日代码

- `cpp/day06/day06_class_inheritance.cpp`

## 8. 今日验收

- 封装的目的是什么？
- 构造函数和析构函数的调用顺序是什么？
- `public`、`protected`、`private` 有什么区别？
- 为什么基类指针可以指向派生类对象？
- 动态绑定需要满足什么条件？
- 虚函数和普通成员函数有什么区别？
- 纯虚函数有什么特点？
- 为什么多态基类通常需要虚析构函数？
- 对象切片是什么？
