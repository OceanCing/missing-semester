---
title: Day 3 数组与函数
created: 2026-10-06
updated: 2026-10-06
tags: [cpp, day03, array, function]
status: active
source: 黑马程序员 042-055；C++ Primer 015、016
---

# Day 3 数组与函数

## 今日目标

- 掌握一维数组的定义、初始化和遍历。
- 理解数组下标从 0 开始。
- 掌握二维数组的定义和嵌套循环遍历。
- 掌握函数的定义、声明、调用和返回。
- 理解值传递和数组参数的区别。
- 理解数组作为函数参数时会发生退化。
- 完成 `day03_arrays_functions.cpp`。

## 1. 一维数组

数组用于保存一组相同类型的数据，内存中连续存放。

~~~cpp
int numbers[5] = {10, 20, 30, 40, 50};
~~~

访问数组元素：

~~~cpp
cout << numbers[0] << endl; // 10
cout << numbers[4] << endl; // 50
~~~

遍历数组：

~~~cpp
for (int i = 0; i < 5; ++i) {
    cout << numbers[i] << endl;
}
~~~

重点：

- 数组下标从 `0` 开始。
- 最后一个元素的下标是长度减 1。
- 访问数组范围之外的数据属于越界访问。
- 数组越界可能导致程序崩溃或产生错误结果。

## 2. 数组初始化

~~~cpp
int a[5];                    // 局部数组元素值不确定
int b[5] = {1, 2, 3};       // 后面的元素自动补0
int c[5] = {};               // 全部初始化为0
int d[] = {1, 2, 3, 4, 5}; // 根据数据数量确定长度
~~~

在数组定义所在作用域中，可以这样求元素个数：

~~~cpp
int length = sizeof(numbers) / sizeof(numbers[0]);
~~~

## 3. 数组名和函数参数

数组作为函数参数时，通常会退化为指向首元素的指针。

~~~cpp
int sumArray(const int numbers[], int size);
~~~

因此必须额外传递数组长度：

~~~cpp
int result = sumArray(numbers, 5);
~~~

错误做法：

~~~cpp
void printArray(int numbers[]) {
    int size = sizeof(numbers) / sizeof(numbers[0]);
    // numbers在这里是指针，得到的不是数组元素个数
}
~~~

正确做法：

~~~cpp
void printArray(const int numbers[], int size);
~~~

## 4. 二维数组

~~~cpp
int matrix[2][3] = {
    {1, 2, 3},
    {4, 5, 6}
};
~~~

二维数组需要两层循环：

~~~cpp
for (int row = 0; row < 2; ++row) {
    for (int column = 0; column < 3; ++column) {
        cout << matrix[row][column] << " ";
    }
    cout << endl;
}
~~~

二维数组作为函数参数时，通常要明确第二维长度：

~~~cpp
int sumMatrix(const int matrix[][3], int rows);
~~~

## 5. 函数

函数用于把一段功能拆分成可以重复调用的代码块。

~~~cpp
int add(int a, int b) {
    return a + b;
}
~~~

组成部分：

- 返回类型：`int`
- 函数名：`add`
- 参数列表：`int a, int b`
- 函数体：`{ return a + b; }`

函数声明只告诉编译器函数的类型：

~~~cpp
int add(int a, int b);
~~~

函数定义负责实现具体功能：

~~~cpp
int add(int a, int b) {
    return a + b;
}
~~~

## 6. 值传递和数组参数

普通参数采用值传递：

~~~cpp
void changeValue(int value) {
    value = 100;
}
~~~

修改函数内部的 `value` 不会影响调用者。

数组参数通常传递首元素地址，因此函数可以修改原数组：

~~~cpp
void reverseArray(int numbers[], int size);
~~~

如果只是读取数组，建议加 `const`：

~~~cpp
void printArray(const int numbers[], int size);
~~~

## 7. 今日代码

- `cpp/day03/day03_arrays_functions.cpp`

## 8. 今日题目

### LintCode 37 Reverse 3-digit Integer

- 链接：https://www.lintcode.com/problem/37/
- 训练：整数取余、整除和函数返回值。
- 例子：`123 -> 321`

### LintCode 145 Lowercase to Uppercase

- 链接：https://www.lintcode.com/problem/145/
- 训练：字符类型和 ASCII 码。
- 提示：小写字母和大写字母之间有固定差值。

### 洛谷 P1046 陶陶摘苹果

- 链接：https://www.luogu.com.cn/problem/P1046
- 训练：数组遍历和条件判断。
- 核心：先保存苹果高度，再统计能够到的苹果数量。

## 9. 易错点

- 数组下标从 0 开始，最后一个下标是 `size - 1`。
- 数组越界不会总是立刻报错，但结果可能错误。
- 数组作为函数参数后，`sizeof` 得到的是指针大小。
- 函数声明和定义必须匹配。
- `return` 后面的代码不会执行。
- 二维数组函数参数不能只写 `matrix[][]`。
- 值传递不能修改原变量，数组参数通常可以修改原数组。

## 10. 今日验收

不看教程，能回答：

### 1. 一维数组如何初始化？

答案：

- `int a[5];`：局部数组默认初始化，元素值不确定，不能直接读取。
- `int b[5] = {1, 2, 3};`：前三个元素为 `1、2、3`，剩余元素自动补 `0`。
- `int c[5] = {};`：所有元素都初始化为 `0`。
- `int d[] = {1, 2, 3, 4};`：数组长度根据初始化数据自动确定为 `4`。
- 全局数组和静态数组如果没有显式初始化，会自动补 `0`。

重点：局部数组 `int a[5];` 不会自动全部变成 `0`。

### 2. 数组越界为什么危险？

答案：

- C++ 通常不会自动检查数组下标是否越界。
- 越界访问属于未定义行为。
- 程序可能继续运行，但结果错误。
- 可能修改其他变量或程序数据。
- 可能导致程序崩溃，甚至产生安全问题。

重点：程序没有报错，不代表数组越界是正确的。

### 3. sizeof 在数组和指针上有什么区别？

答案：

在同一作用域中，数组名用于 `sizeof` 时可以表示整个数组：

~~~cpp
int numbers[5];

sizeof(numbers);                      // 整个数组的字节数
sizeof(numbers) / sizeof(numbers[0]); // 数组元素个数
~~~

数组作为函数参数后会退化为指针：

~~~cpp
void printArray(int numbers[]) {
    // numbers实际上是指针
    // sizeof(numbers)得到的是指针大小，不是数组大小
}
~~~

重点：

- 数组定义所在作用域中，`sizeof` 可以得到数组总大小。
- 函数参数中的数组名本质上是指针。
- 函数内部不能依靠 `sizeof(numbers) / sizeof(numbers[0])` 求长度。

### 4. 函数声明和函数定义有什么区别？

答案：

函数声明只告诉编译器函数的返回类型、函数名和参数类型：

~~~cpp
int add(int a, int b);
~~~

函数定义包含具体实现：

~~~cpp
int add(int a, int b) {
    return a + b;
}
~~~

区别：

- 声明没有函数体，定义有函数体。
- 声明可以出现多次，普通函数定义通常只能出现一次。
- 调用函数前，编译器必须先看到函数声明或定义。
- 多文件项目中，函数声明通常放在头文件，函数定义放在源文件。

### 5. 数组作为函数参数时，为什么要额外传长度？

答案：

数组作为函数参数时，会退化为指向首元素的指针：

~~~cpp
void printArray(const int numbers[], int size);
~~~

`numbers` 在函数内部不再保存完整的数组长度，因此：

- 函数不知道数组有多少个元素。
- 不能安全地用 `sizeof` 求长度。
- 需要使用 `size` 控制循环范围，避免越界。

重点：数组和长度通常要一起传入函数。

### 6. 为什么 sumMatrix(matrix, 2) 中的二维数组参数要写成 matrix[][3]？

答案：

二维数组作为函数参数时，会转换为指向第一行的指针：

~~~cpp
int sumMatrix(const int matrix[][3], int rows);
~~~

这里 `matrix` 的实际参数类型类似：

~~~cpp
const int (*matrix)[3]
~~~

第二维长度 `3` 是必要的，因为编译器计算地址时需要知道一行有多少个元素：

~~~cpp
matrix[row][column]
~~~

本质上需要计算：

~~~text
第row行起始位置 + column
~~~

缺少第二维长度，编译器就无法正确计算每一行的跨度。

第一维长度可以省略，因为函数接收的是指向第一行的指针，不是整个二维数组的拷贝。

### 7. reverseArray 为什么能修改主函数中的原数组？

答案：

数组作为函数参数时会退化为指针，函数接收到的是原数组首元素的地址：

~~~cpp
void reverseArray(int numbers[], int size);
~~~

函数中的 `numbers[i]` 实际访问的就是主函数原数组所在的内存，所以修改会直接影响原数组。

对比普通值传递：

~~~cpp
void changeValue(int value) {
    value = 100; // 只修改副本
}
~~~

数组不是按普通值传参复制一份，而是传递首元素地址，因此可以修改原数组。

如果函数只读取数组，应该加上 `const`：

~~~cpp
void printArray(const int numbers[], int size);
~~~

### 记忆口诀

- 数组有大小，指针只有地址。
- 数组进函数，长度要一起传。
- 二维数组进函数，必须说明一行的长度。
- 数组参数不是复制，函数可能修改原数组。

## 11. 今日复盘

- 完成黑马集数：14
- 完成 Primer 集数：2
- 完成的题目：LintCode 37   LintCode 145  洛谷红题：P1046
- 最不熟悉的知识点：指针
- 遇到的编译错误：无
- 明日优先事项：加油
