---
title: Day 2 分支、循环、类型转换与引用
created: 2026-10-02
updated: 2026-10-02
tags: [cpp, day02, control-flow, loops, type-conversion, pointer, reference]
status: active
source: 黑马程序员 024-041；C++ Primer 007、023、025-027
---

# Day 2 分支、循环、类型转换与引用

## 今日目标

- 掌握 `if / else if / else`、`switch` 和条件运算符。
- 掌握 `while`、`do while`、`for` 和嵌套循环。
- 理解 `break`、`continue` 和 `goto` 的作用。
- 区分隐式类型转换和显式类型转换。
- 掌握 `static_cast` 的基本用法。
- 理解引用和指针的基本区别。
- 完成 `day02_control.cpp` 和相关练习题。

---

## 1. 条件语句

### 1.1 if / else if / else

适合处理有先后顺序的多个条件：

```cpp
int score = 85;

if (score >= 90) {
    cout << "A" << endl;
} else if (score >= 80) {
    cout << "B" << endl;
} else if (score >= 60) {
    cout << "C" << endl;
} else {
    cout << "D" << endl;
}
```

判断顺序很重要。如果条件范围重叠，应先判断范围更小的条件。

### 1.2 switch

适合判断固定整数或字符：

```cpp
int day = 3;

switch (day) {
    case 1:
        cout << "Monday" << endl;
        break;
    case 2:
        cout << "Tuesday" << endl;
        break;
    case 3:
        cout << "Wednesday" << endl;
        break;
    default:
        cout << "Other day" << endl;
        break;
}
```

如果没有写 `break`，程序会继续执行后面的 `case`，这称为贯穿。

### 1.3 条件运算符

```cpp
int a = 10;
int b = 20;

int maxValue = (a > b) ? a : b;
cout << maxValue << endl;
```

条件运算符适合简单的二选一表达式。复杂逻辑仍然建议使用 `if`。

---

## 2. 循环语句

### 2.1 while

先判断条件，再执行循环体：

```cpp
int i = 1;

while (i <= 5) {
    cout << i << endl;
    ++i;
}
```

如果一开始条件不成立，循环体一次都不会执行。

### 2.2 do while

先执行一次循环体，再判断条件：

```cpp
int count = 0;

do {
    cout << "execute once" << endl;
    ++count;
} while (count < 1);
```

`do while` 至少会执行一次。

### 2.3 for

适合次数明确的循环：

```cpp
int sum = 0;

for (int number = 1; number <= 10; ++number) {
    sum += number;
}

cout << sum << endl;
```

`for` 的执行顺序：

1. 执行初始化。
2. 判断条件。
3. 执行循环体。
4. 执行递增或递减。
5. 回到第2步继续判断。

### 2.4 嵌套循环

外层循环执行一次，内层循环通常完整执行一遍。

#### 示例1：打印矩形

```cpp
for (int row = 1; row <= 3; ++row) {
    for (int column = 1; column <= 5; ++column) {
        cout << "* ";
    }
    cout << endl;
}
```

输出：

```text
* * * * *
* * * * *
* * * * *
```

#### 示例2：打印乘法表

```cpp
for (int i = 1; i <= 5; ++i) {
    for (int j = 1; j <= i; ++j) {
        cout << j << "*" << i << "=" << i * j << "\t";
    }
    cout << endl;
}
```

#### 示例3：遍历二维数组

```cpp
int matrix[2][3] = {
    {1, 2, 3},
    {4, 5, 6}
};

int sum = 0;

for (int row = 0; row < 2; ++row) {
    for (int column = 0; column < 3; ++column) {
        sum += matrix[row][column];
    }
}

cout << "sum = " << sum << endl;
```

#### 示例4：内层循环中的 break 和 continue

```cpp
for (int i = 1; i <= 2; ++i) {
    for (int j = 1; j <= 5; ++j) {
        if (j == 2) {
            continue;
        }

        if (j == 5) {
            break;
        }

        cout << "i=" << i << ", j=" << j << endl;
    }
}
```

重点：

- `break` 只结束当前所在的最内层循环。
- `continue` 只跳过当前最内层的本次循环。
- 两层循环的总执行次数通常是：外层次数 × 内层次数。

---

## 3. 跳转语句

- `break`：立即结束当前循环或 `switch`。
- `continue`：跳过本次循环剩余部分，进入下一次循环。
- `goto`：无条件跳转，会破坏代码结构，不建议频繁使用。

```cpp
for (int number = 1; number <= 10; ++number) {
    if (number == 3) {
        continue;
    }

    if (number == 7) {
        break;
    }

    cout << number << endl;
}
```

---

## 4. 类型转换

### 4.1 隐式类型转换

隐式类型转换由编译器自动完成。

#### 示例1：int转double

```cpp
int count = 5;
double average = count;

cout << average << endl; // 5
```

#### 示例2：int和double混合运算

```cpp
int a = 5;
double b = 2.0;
double result = a + b;

cout << result << endl; // 7
```

#### 示例3：char转int

```cpp
char letter = 'A';
int asciiCode = letter;

cout << asciiCode << endl; // 65
```

#### 示例4：bool转int

```cpp
bool passed = true;
int number = passed;

cout << number << endl; // 1
```

#### 示例5：double转int

```cpp
double price = 3.99;
int integerPrice = price;

cout << integerPrice << endl; // 3
```

从浮点数转换为整数时，小数部分会被截断。

#### 示例6：整数除法先得到整数

```cpp
int x = 7;
int y = 2;
double result = x / y;

cout << result << endl; // 3
```

程序先计算 `x / y`，得到整数 `3`，然后才转换为 `double`。

#### 示例7：函数参数隐式转换

```cpp
#include <iostream>
using namespace std;

void printDouble(double value) {
    cout << value << endl;
}

int main() {
    int number = 10;
    printDouble(number); // int隐式转换为double
    return 0;
}
```

#### 隐式转换注意点

- 从小范围类型转到大范围类型，通常比较安全。
- 从浮点类型转到整数类型，小数部分会被截断。
- 有符号数和无符号数混合计算时，可能发生意外转换。

```cpp
int signedValue = -1;
unsigned int unsignedValue = 10;

if (signedValue < unsignedValue) {
    // signedValue可能被转换为一个很大的无符号数
}
```

---

### 4.2 显式类型转换和 static_cast

显式类型转换表示程序员明确要求改变类型。C++ 中优先使用 `static_cast`。

#### 示例1：整数除法转成浮点除法

```cpp
int total = 7;
int count = 2;

double result1 = total / count;
double result2 = static_cast<double>(total) / count;

cout << result1 << endl; // 3
cout << result2 << endl; // 3.5
```

#### 示例2：double转int

```cpp
double price = 9.87;
int integerPrice = static_cast<int>(price);

cout << integerPrice << endl; // 9
```

#### 示例3：int转char

```cpp
int code = 65;
char letter = static_cast<char>(code);

cout << letter << endl; // A
```

#### 示例4：char转int

```cpp
char grade = 'A';
int gradeCode = static_cast<int>(grade);

cout << gradeCode << endl; // 65
```

#### 示例5：int转long long

```cpp
int smallNumber = 100;
long long largeNumber = static_cast<long long>(smallNumber);

cout << largeNumber << endl;
```

#### 示例6：不推荐C风格转换

```cpp
int badStyle = (int)3.99;
```

更推荐：

```cpp
int goodStyle = static_cast<int>(3.99);
```

#### 转换方式对比

| 方式 | 写法 | 特点 |
| --- | --- | --- |
| 隐式转换 | `double x = 10;` | 编译器自动转换 |
| C风格转换 | `(int)3.9` | 转换目的不清晰，不推荐 |
| `static_cast` | `static_cast<int>(3.9)` | 转换目的明确，推荐 |

注意：`static_cast` 只表示程序员明确要求转换，不代表转换一定安全。转换前仍然要考虑范围和精度。

---

## 5. 引用和指针

### 5.1 引用

引用是变量的别名，必须初始化。

```cpp
int a = 10;
int& reference = a;

reference = 20;

cout << a << endl;          // 20
cout << reference << endl;  // 20
```

### 5.2 指针

指针保存变量的地址，可以通过 `*` 解引用。

```cpp
int b = 30;
int* pointer = &b;

cout << "b address: " << pointer << endl;
cout << "b value: " << *pointer << endl; // 30

*pointer = 40;
cout << "b = " << b << endl; // 40
```

### 5.3 指针改变指向

```cpp
int b = 30;
int c = 50;

int* pointer = &b;
pointer = &c;

cout << *pointer << endl; // 50
```

### 5.4 引用重新赋值

```cpp
int x = 1;
int y = 2;
int& ref = x;

ref = y; // 不是改变引用目标，而是把y的值赋给x

cout << x << endl; // 2
cout << y << endl; // 2
```

引用通常不能重新绑定到另一个变量。

### 5.5 三种函数传参方式

```cpp
#include <iostream>
using namespace std;

void changeByValue(int value) {
    value = 100;
}

void changeByReference(int& value) {
    value = 100;
}

void changeByPointer(int* value) {
    if (value != nullptr) {
        *value = 100;
    }
}

int main() {
    int number1 = 10;
    int number2 = 10;
    int number3 = 10;

    changeByValue(number1);     // 不影响原变量
    changeByReference(number2); // 修改原变量
    changeByPointer(&number3);  // 修改原变量

    cout << number1 << endl; // 10
    cout << number2 << endl; // 100
    cout << number3 << endl; // 100

    return 0;
}
```

### 5.6 const引用和空指针

```cpp
const int& constReference = 100;
cout << constReference << endl;

int* emptyPointer = nullptr;

if (emptyPointer == nullptr) {
    cout << "pointer is null" << endl;
}
```

### 5.7 引用和指针的核心区别

| 对比项 | 引用 | 指针 |
| --- | --- | --- |
| 是否必须初始化 | 必须 | 可以不初始化，但很危险 |
| 是否可以为空 | 不可以 | 可以是 `nullptr` |
| 是否可以改变指向 | 通常不能 | 可以 |
| 是否需要解引用 | 直接使用 | 需要 `*` |
| 是否保存地址 | 本质上是别名 | 保存地址 |
| 是否可以为 `const` | 可以，例如 `const int&` | 可以，例如 `const int*` |

---

## 6. 今日代码

代码路径：

- `cpp/day02/day02_control.cpp`

代码应包含：

- `if / else if / else`
- `switch`
- `while`
- `do while`
- `for`
- 嵌套循环
- `break` 和 `continue`
- 条件运算符
- 隐式类型转换
- `static_cast`
- 引用和指针示例

---

## 7. 今日题目

### LintCode 766 Leap Year

- 题目链接：https://www.lintcode.com/problem/766/
- 核心条件：能被 400 整除，或者能被 4 整除但不能被 100 整除。
- 时间复杂度：O(1)
- 空间复杂度：O(1)

```cpp
bool isLeapYear(int year) {
    return year % 400 == 0 ||
           (year % 4 == 0 && year % 100 != 0);
}
```

### LintCode 9 Fizz Buzz

- 题目链接：https://www.lintcode.com/problem/9/
- 核心思路：从 1 循环到 `n`，先判断能否同时被 3 和 5 整除。
- 时间复杂度：O(n)
- 空间复杂度：O(n)

### 洛谷红题

- 题目编号：
- 题目链接：
- 核心思路：
- 错误原因：
- 二次订正结果：

---

## 8. 易错点

- `switch` 忘记写 `break`。
- `do while` 至少执行一次。
- `break` 和 `continue` 的作用范围混淆。
- 循环边界写错，造成多执行或少执行一次。
- 整数除法导致小数部分丢失。
- 隐式转换发生但没有察觉。
- 使用 `static_cast` 时没有考虑范围和精度。
- 引用没有初始化。
- 对空指针解引用。
- 混淆指针本身的地址和指针指向的值。
- 误以为 `ref = other` 会让引用改为引用另一个变量。

---

## 9. 今日复盘

- 完成黑马集数：
- 完成 Primer 集数：
- 完成的题目：
- 最不熟悉的知识点：
- 遇到的编译错误：
- 明日优先事项：
