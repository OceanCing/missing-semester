---
title: Day 4 指针与结构体
created: 2026-10-09
updated: 2026-10-09
tags: [cpp, day04, pointer, reference, struct, function]
status: active
source: 黑马程序员 056-071；C++ Primer 030-034
---

# Day 4 指针与结构体

## 今日目标

- 掌握取地址 `&`、解引用 `*` 和指针定义。
- 理解空指针、野指针和 `const` 指针。
- 理解指针与数组的关系。
- 掌握结构体定义、初始化、成员访问和结构体数组。
- 掌握结构体指针与 `->`。
- 理解值传递、引用传递和 `const` 引用传递。
- 了解函数重载、默认参数和内联函数。
- 完成 `day04_pointers_structs.cpp`。

## 1. 指针基础

指针保存变量的地址。

~~~cpp
int value = 10;
int* pointer = &value;

cout << value << endl;    // 10
cout << *pointer << endl; // 10
~~~

- `&value`：取得变量的地址。
- `*pointer`：访问指针指向的值。
- `pointer`：保存的地址。
- `nullptr`：空指针。

~~~cpp
int* emptyPointer = nullptr;

if (emptyPointer == nullptr) {
    cout << "pointer is null" << endl;
}
~~~

空指针与野指针：

- 空指针：明确指向 `nullptr`，使用前可以判断。
- 野指针：指向未知或已经失效的内存地址。
- 野指针不能随意解引用。

## 2. const 与指针

~~~cpp
int value = 10;

const int* p1 = &value;
int* const p2 = &value;
const int* const p3 = &value;
~~~

| 写法 | 能否修改指向的值 | 能否改变指向 |
| --- | --- | --- |
| `const int* p` | 不可以 | 可以 |
| `int* const p` | 可以 | 不可以 |
| `const int* const p` | 不可以 | 不可以 |

记忆方式：看 `const` 在 `*` 的左边还是右边。

## 3. 指针与数组

数组名在表达式中通常会转换为首元素地址。

~~~cpp
int numbers[5] = {10, 20, 30, 40, 50};
int* pointer = numbers;

cout << *pointer << endl;       // 10
cout << *(pointer + 1) << endl; // 20
cout << pointer[2] << endl;     // 30
~~~

数组作为函数参数时仍然需要传长度：

~~~cpp
void printArrayByPointer(const int* numbers, int size);
~~~

## 4. 结构体基础

~~~cpp
struct Student {
    int id;
    double score;
};
~~~

定义并初始化：

~~~cpp
Student student = {1001, 92.5};
~~~

使用点运算符访问成员：

~~~cpp
cout << student.id << endl;
cout << student.score << endl;
~~~

结构体数组：

~~~cpp
Student students[3] = {
    {1001, 85.5},
    {1002, 92.0},
    {1003, 78.5}
};
~~~

结构体指针使用箭头 `->`：

~~~cpp
Student* pointer = &student;

cout << pointer->id << endl;
cout << pointer->score << endl;
~~~

## 5. 结构体嵌套

~~~cpp
struct Date {
    int year;
    int month;
    int day;
};

struct Student {
    int id;
    Date birthday;
    double score;
};
~~~

访问嵌套成员：

~~~cpp
Student student = {
    1001,
    {2006, 5, 18},
    90.5
};

cout << student.birthday.year << endl;
~~~

## 6. 结构体与函数

结构体可以按值传递：

~~~cpp
void printStudent(Student student);
~~~

如果结构体较大，按值传递会产生一次拷贝，因此通常使用 `const` 引用：

~~~cpp
void printStudent(const Student& student);
~~~

结构体数组仍然需要传长度：

~~~cpp
double averageScore(const Student students[], int size);
~~~

返回结构体数组中的元素地址时，要保证原数组仍然存在：

~~~cpp
Student* findTopStudent(Student students[], int size);
~~~

## 7. 参数传递与函数特性

### 值传递

~~~cpp
void changeValue(int value) {
    value = 100;
}
~~~

不会修改原变量。

### 引用传递

~~~cpp
void changeValue(int& value) {
    value = 100;
}
~~~

会直接修改原变量。

### const 引用传递

~~~cpp
void printValue(const int& value);
~~~

既不复制大对象，又禁止函数修改它。

### 函数重载

函数名相同，参数列表不同：

~~~cpp
int add(int a, int b) {
    return a + b;
}

double add(double a, double b) {
    return a + b;
}
~~~

### 默认参数

~~~cpp
int add(int a, int b = 10) {
    return a + b;
}
~~~

默认参数通常从右向左定义。

### 内联函数

~~~cpp
inline int square(int value) {
    return value * value;
}
~~~

`inline` 只是给编译器建议，不能保证一定展开。

## 8. 今日代码

- `cpp/day04/day04_pointers_structs.cpp`

## 9. 今日题目

### LintCode 366 Fibonacci

- 链接：https://www.lintcode.com/problem/366/
- 训练：循环、函数返回值。

### LintCode 145 Lowercase to Uppercase

- 链接：https://www.lintcode.com/problem/145/
- 训练：字符和 ASCII 码。

### 洛谷 P1047 校门外的树

- 链接：https://www.luogu.com.cn/problem/P1047
- 训练：数组、循环和区间标记。

## 10. 易错点

- 指针未初始化就使用。
- 解引用空指针。
- 数组作为参数后无法直接求长度。
- 混淆 `.` 和 `->`。
- 搞混 `const int*` 和 `int* const`。
- 返回局部变量的地址。
- 结构体按值传递造成不必要的复制。
- 函数重载只有返回类型不同时不构成重载。
- 默认参数定义顺序错误。

## 11. 今日验收

### 1. `&` 和 `*` 分别做什么？

答案：

- `&` 用于取变量地址。
- `*` 用于声明指针或访问指针指向的值。

### 2. 空指针和野指针有什么区别？

答案：

- 空指针明确保存 `nullptr`。
- 野指针保存未初始化、已经释放或无效的地址。
- 空指针可以判断，野指针非常危险。

### 3. 指针和引用有什么区别？

答案：

- 引用必须初始化，指针可以不初始化，但危险。
- 引用不能为空，指针可以为 `nullptr`。
- 引用通常不能改变绑定对象，指针可以改变指向。
- 引用直接使用，指针需要解引用。

### 4. `const int*` 和 `int* const` 有什么区别？

答案：

- `const int*`：不能通过指针修改指向的值，但可以改变指向。
- `int* const`：可以修改指向的值，但不能改变指针本身。

### 5. 结构体变量和结构体指针如何访问成员？

答案：

~~~cpp
Student student;
Student* pointer = &student;

student.id = 1001;
pointer->id = 1001;
~~~

变量使用 `.`，指针使用 `->`。

### 6. 结构体按值传递和按 const 引用传递有什么区别？

答案：

- 按值传递会复制一份结构体，修改副本不会影响原对象。
- const 引用不会复制，且函数不能修改原对象。

### 7. 函数重载的条件是什么？

答案：

函数名相同，参数列表不同。只改变返回类型不能构成重载。

### 8. `findTopStudent` 返回的指针为什么有效？

答案：

返回的是主函数中结构体数组元素的地址。只要原数组仍然存在，该地址就有效。

## 12. 今日复盘

- 完成黑马集数：
- 完成 Primer 集数：
- 完成的题目：
- 最不熟悉的知识点：
- 遇到的编译错误：
- 明日优先事项：

## 延伸内容

- [Day 4 OpenCV 图像读取、灰度化与二值化](2026-10-07-day04-opencv.md)
