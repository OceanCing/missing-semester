---
title: Day 5 内存模型、引用与类
created: 2026-10-09
updated: 2026-10-09
tags: [cpp, day05, memory, reference, class, constructor]
status: active
source: 黑马程序员 084-098；C++ Primer 036、038-041
---

# Day 5 内存模型、引用与类

## 今日目标

- 理解程序的内存分区。
- 掌握 `new`、`delete` 和基本内存管理。
- 掌握引用的定义、传参和返回值规则。
- 掌握默认参数、占位参数和函数重载。
- 掌握类、访问控制、成员函数和成员变量。
- 掌握 `this`、构造函数和初始化列表。
- 掌握 const 成员函数、const 成员变量和静态成员。
- 完成 `day05_memory_class.cpp`。

## 1. 程序内存分区

| 区域 | 主要用途 |
| --- | --- |
| 代码区 | 保存程序指令 |
| 全局区 | 保存全局变量和静态变量 |
| 栈区 | 保存局部变量、函数参数和返回地址 |
| 堆区 | 保存动态申请的内存 |

## 2. 栈和堆

栈区：

- 由编译器自动管理。
- 局部变量通常存放在栈上。
- 离开作用域后自动释放。
- 空间通常较小。

堆区：

- 由程序员主动申请和释放。
- 使用 `new` 申请。
- 使用 `delete` 或 `delete[]` 释放。
- 空间通常比栈大。
- 忘记释放会造成内存泄漏。

## 3. new 和 delete

申请一个整数：

~~~cpp
int* number = new int(42);

cout << *number << endl;

delete number;
number = nullptr;
~~~

申请数组：

~~~cpp
int* numbers = new int[3]{10, 20, 30};

for (int i = 0; i < 3; ++i) {
    cout << numbers[i] << endl;
}

delete[] numbers;
numbers = nullptr;
~~~

必须对应：

| 申请方式 | 释放方式 |
| --- | --- |
| `new int` | `delete pointer` |
| `new int[10]` | `delete[] pointer` |

错误：

- `new` 之后忘记 `delete`
- `new[]` 使用 `delete`
- 同一块内存重复释放
- 释放后将指针悬空，却继续使用

## 4. 引用

引用是变量的别名。

~~~cpp
int value = 10;
int& reference = value;

reference = 20;

cout << value << endl;     // 20
cout << reference << endl; // 20
~~~

引用必须初始化：

~~~cpp
int value = 10;
int& reference = value;
~~~

引用通常不能重新绑定到其他变量：

~~~cpp
int a = 10;
int b = 20;
int& reference = a;

reference = b; // 把b的值赋给a，不会让reference改为引用b
~~~

## 5. 引用作为函数参数

引用传参可以直接修改原变量：

~~~cpp
void swapByReference(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
~~~

调用：

~~~cpp
int x = 10;
int y = 20;

swapByReference(x, y);
~~~

## 6. const 引用

const 引用可以绑定临时值，也能避免复制：

~~~cpp
void printValue(const int& value) {
    cout << value << endl;
}
~~~

调用：

~~~cpp
printValue(10);
printValue(100);
~~~

函数内部不能通过 const 引用修改原对象。

## 7. 引用作为返回值

返回引用时，必须保证被引用对象在函数返回后仍然存在。

错误：

~~~cpp
int& badFunction() {
    int value = 10;
    return value;
}
~~~

`value` 是局部变量，函数结束后生命周期结束，返回引用会变成悬空引用。

正确思路是返回仍然存在的对象，例如数组元素或类成员。

## 8. 默认参数

~~~cpp
int add(int a, int b = 10) {
    return a + b;
}
~~~

调用：

~~~cpp
add(5);    // 15
add(5, 7); // 12
~~~

默认参数通常从右向左定义。

## 9. 占位参数

~~~cpp
void printValue(int value, int) {
    cout << value << endl;
}
~~~

占位参数只保留参数位置，调用时仍要传对应数量的实参。

## 10. 函数重载

函数名相同，参数列表不同：

~~~cpp
int add(int a, int b) {
    return a + b;
}

double add(double a, double b) {
    return a + b;
}
~~~

可以重载：

- 参数类型不同
- 参数数量不同
- 参数顺序不同

不能只依靠返回类型不同来重载。

## 11. 类与对象

~~~cpp
class Student {
private:
    int id;
    double score;

public:
    Student(int id = 0, double score = 0.0) {
        this->id = id;
        this->score = score;
    }

    double getScore() const {
        return score;
    }

    void setScore(double score) {
        this->score = score;
    }
};
~~~

- 类是一种自定义数据类型。
- 对象是类创建出来的实例。
- `private` 成员只能在类内部访问。
- `public` 成员可以在类外部访问。

class 和 struct 的默认访问权限：

| 类型 | 默认权限 |
| --- | --- |
| `class` | `private` |
| `struct` | `public` |

## 12. this 指针

`this` 指向当前调用成员函数的对象：

~~~cpp
void setScore(double score) {
    this->score = score;
}
~~~

`this->score` 表示当前对象的成员变量，右边的 `score` 是函数参数。

## 13. 构造函数

构造函数在创建对象时自动调用：

~~~cpp
Student(int id, double score) {
    this->id = id;
    this->score = score;
}
~~~

构造函数：

- 函数名和类名相同。
- 没有返回类型。
- 可以重载。
- 创建对象时自动调用。

## 14. 构造函数初始化列表

~~~cpp
Student(int id, double score)
    : id(id), score(score) {
}
~~~

初始化列表在构造函数体执行之前初始化成员变量。

对比：

~~~cpp
Student(int id, double score) {
    this->id = id;
    this->score = score;
}
~~~

这种写法是在构造函数体内赋值。

## 15. const 成员函数

~~~cpp
double getScore() const {
    return score;
}
~~~

const 成员函数不能修改普通成员变量。

## 16. const 成员变量

const 成员变量必须在初始化列表中初始化：

~~~cpp
class Student {
private:
    const int id;

public:
    Student(int id)
        : id(id) {
    }
};
~~~

## 17. 静态成员

静态成员变量属于类，不属于某个具体对象：

~~~cpp
class Student {
private:
    static int count;

public:
    Student() {
        ++count;
    }

    static int getCount() {
        return count;
    }
};

int Student::count = 0;
~~~

重点：

- 静态成员变量需要在类外定义。
- 所有对象共享同一个静态成员变量。
- 静态成员函数没有隐含的 `this` 指针。
- 静态成员函数不能直接访问普通成员变量。

## 18. 今日代码

- `cpp/day05/day05_memory_class.cpp`

## 19. 今日题目

### LintCode

从下面选两道没有做过的：

- https://www.lintcode.com/problem/1/
- https://www.lintcode.com/problem/37/
- https://www.lintcode.com/problem/145/
- https://www.lintcode.com/problem/366/

### 洛谷橙题

- https://www.luogu.com.cn/problem/P1047
- 如果已经做过，换：https://www.luogu.com.cn/problem/P1307

## 20. 易错点

- `new` 和 `delete` 没有配对。
- `new[]` 使用了普通 `delete`。
- 返回局部变量的引用。
- 把 `ref = other` 误认为改变引用目标。
- 默认参数顺序错误。
- 函数重载只有返回类型不同。
- 忘记 `this->`，导致同名变量赋值错误。
- const 成员函数修改成员变量。
- const 成员变量没有使用初始化列表。
- 静态成员变量忘记在类外定义。

## 21. 今日验收

### 1. 内存分为哪几个区域？

答案：代码区、全局区、栈区和堆区。

### 2. 栈区和堆区有什么区别？

答案：

- 栈区由编译器自动管理，函数或作用域结束后自动释放。
- 堆区由程序员使用 `new` 申请、`delete` 释放，忘记释放会造成内存泄漏。

### 3. new 和 delete 为什么要成对使用？

答案：`new` 从堆区申请内存，`delete` 负责把内存还给系统。不成对会导致内存泄漏或重复释放。

### 4. 引用和指针有什么区别？

答案：

- 引用必须初始化，不能为空。
- 指针可以不初始化，但空指针更安全。
- 引用通常不能改变绑定对象，指针可以改变指向。
- 引用直接使用，指针需要解引用。

### 5. 为什么不能返回局部变量的引用？

答案：局部变量在函数结束后被销毁，引用会指向已经失效的内存。

### 6. 默认参数为什么通常写在参数列表右边？

答案：调用时实参按从左到右匹配，如果左边有默认参数而右边没有，会造成调用歧义。

### 7. 只有返回类型不同，能不能构成函数重载？

答案：不能。函数重载要求参数列表不同。

### 8. class 和 struct 的默认访问权限有什么区别？

答案：

- `class` 默认是 `private`。
- `struct` 默认是 `public`。

### 9. this 指针有什么作用？

答案：`this` 指向当前调用成员函数的对象，可以区分成员变量和同名参数。

### 10. 初始化列表和构造函数体赋值有什么区别？

答案：初始化列表在构造函数体执行前初始化成员；构造函数体赋值是在成员完成初始化后再赋值。const 成员和引用成员必须在初始化列表中初始化。

### 11. const 成员函数有什么限制？

答案：const 成员函数不能修改普通成员变量，除非成员被声明为 `mutable`。

### 12. 静态成员变量属于对象还是类？

答案：静态成员变量属于类，所有对象共享同一份数据。

## 22. 今日复盘

- 完成黑马集数：
- 完成 Primer 集数：
- 完成的题目：
- 最不熟悉的知识点：
- 遇到的编译错误：
- 明日优先事项：
