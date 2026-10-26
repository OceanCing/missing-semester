---
title: Day 1 变量、初始化、auto 和 using
created: 2026-09-29
updated: 2026-09-29
tags: [cpp, day01, variables, initialization, auto, using]
status: active
source: 黑马程序员 004-023；C++ Primer 005、006、009、011
---

# Day 1 变量、初始化、`auto` 和 `using`

## 今日目标

- 掌握 C++ 基本数据类型、变量声明和定义。
- 区分默认初始化、值初始化、拷贝初始化、直接初始化和列表初始化。
- 理解 `auto` 的基本类型推断。
- 学会使用 `using std::cout;`、`using std::cin;` 等 using 声明。
- 理解全局变量、局部变量、静态局部变量和代码块变量的生命周期。
- 掌握算术、赋值、递增递减、比较、逻辑和条件运算符。

## 1. 声明与定义

- 声明：告诉编译器名字和类型，不负责创建完整对象。
- 定义：为变量分配存储空间，并完成必要的初始化。
- 一个变量可以有多次声明，但通常只能定义一次。

```cpp
extern int externalValue; // 声明
int globalCounter = 0;    // 定义
```

## 2. 常见初始化方式

```cpp
int defaultInitialized;    // 默认初始化，当前值不确定，不能直接读取
defaultInitialized = 10;   // 先赋值，再使用

int valueInitialized{};    // 值初始化，结果为0
int copyInitialized = 20;  // 拷贝初始化
int directInitialized(30); // 直接初始化
int braceInitialized{40};  // 列表初始化，推荐方式
```

重点：局部变量 `int a;` 不会自动变成0；`int a{};` 会进行值初始化。

## 3. `auto` 类型推断

```cpp
auto integerNumber = 18;     // int
auto doubleNumber = 2.5;     // double
auto letter = 'A';           // char
auto result = integerNumber + doubleNumber; // double
```

注意：`auto` 必须有初始值，不能写成 `auto x;`。

## 4. `using` 声明

```cpp
using std::cin;
using std::cout;
using std::endl;
```

- `using std::cout;` 只引入一个名字。
- `using namespace std;` 引入整个命名空间中的名字。
- 项目代码中优先使用 using 声明，减少名字冲突。

## 5. 变量的作用域和生命周期

```cpp
int globalCounter = 0; // 全局变量，程序运行期间都存在

void lifetimeDemo() {
    static int staticCounter = 0; // 静态局部变量，只初始化一次
    ++staticCounter;
}
```

- 全局变量：程序开始到结束。
- 局部变量：从定义位置到当前作用域结束。
- 静态局部变量：程序开始到结束，但只能在函数内部访问。
- 代码块变量：离开 `{}` 后不能再访问。

## 6. 运算符基础

- 算术运算符：`+`、`-`、`*`、`/`、`%`
- 赋值运算符：`=`、`+=`、`-=`、`*=`、`/=`
- 递增递减：`++i`、`i++`、`--i`、`i--`
- 比较运算符：`==`、`!=`、`<`、`>`、`<=`、`>=`
- 逻辑运算符：`!`、`&&`、`||`
- 条件运算符：`condition ? value1 : value2`

## 7. 易错点

- 读取未初始化的局部变量。
- 把 `=` 写成 `==`。
- 混淆前置递增和后置递增。
- 忘记整数除法会丢失小数部分。
- 不清楚变量离开作用域后不能再使用。

## 8. 今日代码

建议代码路径：

- `cpp/day01/day01_variables.cpp`
- `cpp/day01/day01_operators.cpp`

代码必须能直接在 C++17 环境编译运行。

## 9. 今日练习

- [ ] [LintCode 1 A + B Problem](https://www.lintcode.com/problem/1/)
- [ ] [洛谷 P1001 A+B Problem](https://www.luogu.com.cn/problem/P1001)

每道题记录思路、复杂度、错误原因和二次订正结果。

## 10. 今日复盘

- 完成的集数：
- 最不熟悉的知识点：
- 需要重看的视频：
- 明天优先解决的问题：
