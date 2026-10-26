---
title: Day 8 vector 创建、操作与遍历
created: 2026-10-10
updated: 2026-10-10
tags: [cpp, day08, vector, iterator, traversal]
status: active
source: 黑马程序员 197-203；C++ Primer 013-014、045-048
---

# Day 8 vector 创建、操作与遍历

## 今日目标

- 掌握 vector 的创建和初始化。
- 掌握 `size`、`capacity`、`empty`、`reserve`。
- 掌握 `push_back`、`pop_back`、`insert` 和 `erase`。
- 掌握下标、`at`、`front` 和 `back`。
- 掌握三种遍历方式。
- 完成 `day08_vector.cpp`。

更完整的扩展内容见：[std::vector 动态数组](2026-10-02-vector.md)。

## 1. 创建 vector

~~~cpp
vector<int> first;
vector<int> second(5, 3);
vector<int> third = {10, 20, 30};
~~~

- `first`：空 vector。
- `second`：5个元素，每个都是3。
- `third`：使用初始化列表。

## 2. 常用操作

~~~cpp
vector<int> numbers = {10, 20, 30};

numbers.push_back(40);
numbers.pop_back();

numbers.insert(numbers.begin() + 1, 15);
numbers.erase(numbers.begin() + 2);

cout << numbers.front() << endl;
cout << numbers.back() << endl;
cout << numbers.at(1) << endl;
~~~

## 3. 容量和大小

~~~cpp
cout << numbers.size() << endl;
cout << numbers.capacity() << endl;
cout << numbers.empty() << endl;

numbers.reserve(20);
~~~

区别：

- `size`：当前元素个数。
- `capacity`：当前已申请的内存能容纳多少个元素。
- `reserve`：提前申请容量，不改变元素个数。

## 4. vector 遍历

### 下标遍历

~~~cpp
for (size_t i = 0; i < numbers.size(); ++i) {
    cout << numbers[i] << " ";
}
~~~

### 迭代器遍历

~~~cpp
for (vector<int>::iterator it = numbers.begin();
     it != numbers.end(); ++it) {
    cout << *it << " ";
}
~~~

### 范围 for 遍历

~~~cpp
for (int number : numbers) {
    cout << number << " ";
}
~~~

## 5. 修改 vector 中的元素

范围 for 使用引用可以修改元素：

~~~cpp
for (int& number : numbers) {
    number *= 2;
}
~~~

## 6. vector 中存放结构体

~~~cpp
struct Student {
    int id;
    double score;
};

vector<Student> students;
students.push_back({1001, 90.5});
~~~

## 7. 今日代码

- `cpp/day08/day08_vector.cpp`

## 8. 今日验收

- vector 和普通数组有什么区别？
- `size` 和 `capacity` 有什么区别？
- `push_back` 和 `insert` 有什么区别？
- `erase` 会删除哪些元素？
- 三种 vector 遍历方式分别怎么写？
- 为什么范围 for 需要引用才能修改元素？
- vector 插入元素后，原来的迭代器一定还有效吗？
