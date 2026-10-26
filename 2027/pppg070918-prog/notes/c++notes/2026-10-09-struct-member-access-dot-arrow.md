---
title: C++ 结构体成员访问：什么时候用 -> 什么时候用 .
created: 2026-10-09
updated: 2026-10-09
tags: [cpp, struct, pointer, reference, member-access]
status: active
source:
---

# C++ 结构体成员访问：什么时候用 `->` 什么时候用 `.`

## 摘要

> 一句话规则：**左边是对象（或引用）用 `.`，左边是指针用 `->`。**

`p->member` 只是 `(*p).member` 的简写，理解这一点就不会再混。

## 1. 核心规则

| 左边是什么 | 运算符 | 例子 |
| --- | --- | --- |
| 对象本身 | `.` | `student.id` |
| 引用 | `.` | `studentRef.id` |
| 指针 | `->` | `studentPtr->id` |
| 解引用后的值 | `.` | `(*studentPtr).id` |

判断方法只有一步：**先问左边这个变量本身是结构体，还是指向结构体的指针。**

## 2. 最小示例

~~~cpp
struct Point {
    int x;
    int y;
};

Point point{1, 2};        // 对象
Point* ptr = &point;      // 指针
Point& ref = point;       // 引用

point.x = 10;             // 对象 -> 用 .
ptr->x = 20;              // 指针 -> 用 ->
(*ptr).x = 20;            // 与上一行完全等价
ref.x = 30;               // 引用 -> 用 .
~~~

## 3. 为什么 `->` 等于 `(*p).`

~~~cpp
ptr->x
~~~

分两步理解：

1. `ptr` 是指针，先解引用得到对象：`*ptr`。
2. 再访问对象的成员：`(*ptr).x`。

因为 `(*ptr).x` 写起来啰嗦又容易漏括号，所以 C++ 提供了简写 `ptr->x`。

> 注意括号：`*ptr.x` 是错的。`.` 的优先级高于 `*`，它会被理解成 `*(ptr.x)`，也就是先访问指针自己的成员，而指针没有成员 `x`。

## 4. 常见场景

### 4.1 结构体数组

`arr[i]` 得到的是对象，所以用 `.`；`arr + i` 得到的是指针，所以用 `->`。

~~~cpp
Point arr[3] = {{1, 2}, {3, 4}, {5, 6}};

arr[0].x = 10;         // arr[0] 是对象，用 .
(arr + 1)->x = 20;     // arr + 1 是指针，用 ->
~~~

### 4.2 结构体指针作函数参数

~~~cpp
void moveRight(Point* p, int distance) {
    p->x += distance;  // p 是指针，用 ->
}

void printPoint(const Point& p) {
    cout << p.x << ", " << p.y << endl;  // p 是引用，用 .
}
~~~

**经验做法：**

- 需要修改原对象 → 传 `Point*` 或 `Point&`。
- 只读且结构体较大 → 传 `const Point&`。
- 只读但允许为空 → 传 `const Point*`。

### 4.3 嵌套结构体

指针访问每一层都用 `->`，对象访问每一层都用 `.`，可以混用。

~~~cpp
struct Date {
    int year;
    int month;
    int day;
};

struct Student {
    int id;
    Date birthday;
};

Student student{1001, {2006, 5, 18}};
Student* ptr = &student;

cout << student.birthday.year << endl;  // 全是对象，全用 .
cout << ptr->birthday.year << endl;     // 第一层是指针用 ->，第二层是对象用 .
~~~

### 4.4 `this` 指针

成员函数里的 `this` 是**指针**，所以访问成员要用 `->`。

~~~cpp
struct Counter {
    int value = 0;

    void add(int amount) {
        this->value += amount;  // this 是指针
    }
};
~~~

### 4.5 智能指针

`unique_ptr` / `shared_ptr` 虽然是对象，但重载了 `->`，用法和普通指针一致。

~~~cpp
#include <memory>

auto ptr = std::make_unique<Point>();
ptr->x = 5;      // 用 ->
(*ptr).y = 6;    // 也可以，但一般不这么写
~~~

## 5. 易错点

- **`.` 和 `->` 用反**：编译报错通常是 "request for member ... in something not a structure or union" 之类的提示。
- **`*ptr.x` 漏括号**：应写 `(*ptr).x` 或更清晰的 `ptr->x`。
- **只有指针才能用 `->`**：普通对象、引用不能直接写 `obj->member`。
- **引用不是指针**：`Point& ref` 用 `ref.x`，不要写 `ref->x`。
- **不要解引用空指针**：`ptr->x` 之前先确保 `ptr != nullptr`。
- **类也可以这样用**：规则不限于 `struct`，`class` 对象和指针完全一样。

## 6. 自测

1. `Point p; Point* q = &p;` 中，`p.x` 和 `q->x` 分别对吗？
2. `Point& r = p;` 访问成员应该用哪个符号？
3. `p->x` 的等价写法是什么？
4. `arr[i].x` 中为什么用 `.` 而不是 `->`？

**答案：**

1. 都对。`p` 是对象，`q` 是指针。
2. 用 `.`：`r.x`。引用是对象的别名。
3. `(*p).x`。
4. `arr[i]` 的结果是对象，不是指针。

## 7. 一句话总结

~~~text
对象 / 引用 -> .
指针        -> ->
p->x 就是 (*p).x 的简写
~~~

## 待办

- [ ] 用 `Point` 分别写一个按值、按引用、按指针传参的函数，观察能否修改原对象。
- [ ] 做 3 道结构体指针相关的练习题，检查是否还会混淆 `.` 和 `->`。

## 参考

- C++ Primer 结构体与指针相关章节
- [Day 4 指针与结构体](day04-pointers-structs.md)
