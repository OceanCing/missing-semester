---
title: 指针数组、数组指针与数组引用
created: 2026-10-06
updated: 2026-10-06
tags: [cpp, pointer, array, reference, declarator]
status: active
source: C++ Primer 第二章 2.3.3；学习交流整理
---

# 指针数组、数组指针与数组引用

> 用声明拆解法区分 `int *ptrs[10]`、`int (*Parray)[10]`、`int (&arrRef)[10]` 和 `int *(&arry)[10]`。

## 核心结论

| 声明 | 类型含义 | 是否合法 |
| --- | --- | --- |
| `int *ptrs[10];` | 含 10 个 `int*` 的数组 | 合法 |
| `int &refs[10];` | 含 10 个 `int&` 的数组 | 非法 |
| `int (*Parray)[10];` | 指向 `int[10]` 的指针 | 合法 |
| `int (&arrRef)[10];` | 引用 `int[10]` 的引用 | 合法 |
| `int *(&arry)[10];` | 引用 `int*[10]` 的引用 | 合法 |

一句话区分：

```text
int *ptrs[10]       -> 数组，元素是指针
int (*Parray)[10]   -> 指针，指向数组
int (&arrRef)[10]   -> 引用，引用数组
int *(&arry)[10]    -> 引用，引用“指针数组”
```

## 如何拆解声明

阅读 C++ 复杂声明时要注意：

- `[]` 的优先级高于 `*` 和 `&`。
- 括号可以改变结合顺序。
- 从变量名开始，再结合左右两边理解。

例如：

```cpp
int *ptrs[10];
```

由于 `[]` 先和 `ptrs` 结合，所以它等价于：

```cpp
int *(ptrs[10]);
```

`ptrs` 是一个长度为 10 的数组，每个元素都是 `int*`。

加上括号后：

```cpp
int (*Parray)[10];
```

`Parray` 先和 `*` 结合，表示它是一个指针，指向一个长度为 10 的 `int` 数组。

## 1. 指针数组

### 正确写法

```cpp
int *ptrs[10];
```

也可以写成：

```cpp
int* ptrs[10];
```

但 `int *ptrs[10]` 更容易看出 `ptrs` 是数组，推荐使用这种写法。

单独写：

```cpp
int*[10];
```

在变量声明中是非法的，因为缺少数组名。通常想表达的是：

```cpp
int *ptrs[10];
```

### 类型拆解

```text
ptrs        -> 数组
ptrs[10]    -> 有 10 个元素
int *       -> 每个元素是 int*
```

因此：

```cpp
ptrs[0]     // 类型是 int*
*ptrs[0]    // 类型是 int
```

### 初始化示例

```cpp
int x = 1;
int *ptrs[10] = {&x};
```

含义：

- `x` 是值为 `1` 的整数。
- `&x` 是 `x` 的地址，类型为 `int*`。
- `{&x}` 使用 `&x` 初始化第一个元素 `ptrs[0]`。
- 其余 9 个元素会被初始化为空指针。

等价于：

```cpp
int x = 1;

int *ptrs[10];
ptrs[0] = &x;
ptrs[1] = nullptr;
ptrs[2] = nullptr;
// ...
ptrs[9] = nullptr;
```

内存关系可以表示为：

```text
x:      1

ptrs:
ptrs[0] -> x
ptrs[1] = nullptr
ptrs[2] = nullptr
...
ptrs[9] = nullptr
```

因此：

```cpp
*ptrs[0]   // 得到 x，也就是 1
```

注意：

```cpp
*ptrs[0]
```

等价于：

```cpp
*(ptrs[0])
```

因为 `[]` 的优先级高于 `*`。

## 2. 引用数组为什么非法

```cpp
int &refs[10];   // 错误
```

它的字面含义是：

> 一个包含 10 个 `int&` 的数组。

但 C++ 不允许引用数组，主要原因是：

- 引用不是对象，而数组元素必须是对象。
- 引用必须在定义时初始化。
- 引用不能重新绑定。
- C++ 中没有“引用数组”这种类型。

需要注意：

```cpp
int &refs[10];    // 非法：引用数组
int (&refs)[10];  // 合法：引用一个含 10 个 int 的数组
```

括号的位置会完全改变含义。

## 3. 数组指针

### 声明

```cpp
int (*Parray)[10] = &arr;
```

假设：

```cpp
int arr[10];
```

### 类型拆解

```cpp
int (*Parray)[10]
```

先看括号：

```text
(*Parray)   -> Parray 是一个指针
[10]        -> 它指向一个长度为 10 的数组
int         -> 数组元素是 int
```

所以：

```cpp
Parray 的类型是 int (*)[10]
```

含义是：

> `Parray` 是一个指针，指向含有 10 个 `int` 的数组。

注意：

```cpp
Parray  不是 int**
Parray  是 int (*)[10]
```

这两者不同。

### 初始化与访问

```cpp
int arr[10] = {1, 2, 3};
int (*Parray)[10] = &arr;
```

访问方式：

```cpp
(*Parray)[0]   // 等价于 arr[0]
Parray[0][1]   // 等价于 arr[1]
```

原因：

- `Parray` 指向整个数组。
- `*Parray` 得到整个数组。
- `(*Parray)[i]` 得到数组的第 `i` 个元素。

### 指针运算

```cpp
Parray + 1
```

不是跳过一个 `int`，而是跳过整个 `int[10]` 数组。

它常用于二维数组：

```cpp
int a[3][10];
int (*row)[10] = &a[0];
```

此时 `row` 可以看作指向一行，每行包含 10 个 `int`。

## 4. 数组引用

### 声明

```cpp
int (&arrRef)[10] = arr;
```

### 类型拆解

```text
(&arrRef)   -> arrRef 是一个引用
[10]        -> 它引用一个长度为 10 的数组
int         -> 数组元素是 int
```

所以：

```cpp
arrRef 的类型是 int (&)[10]
```

含义是：

> `arrRef` 是数组 `arr` 的引用，也就是数组的别名。

### 使用

```cpp
int arr[10] = {1, 2, 3};
int (&arrRef)[10] = arr;

arrRef[0] = 100;   // 等价于 arr[0] = 100
```

数组引用可以保留数组大小，常用于模板：

```cpp
template <std::size_t N>
void printSize(int (&a)[N]) {
    std::cout << N << '\n';
}
```

### 与数组指针的区别

```cpp
int (*Parray)[10] = &arr;   // 指针，初始化要用 &arr
int (&arrRef)[10] = arr;    // 引用，初始化直接用 arr
```

区别：

- `Parray` 是独立变量，可以改变指向。
- `arrRef` 是引用，绑定后不能改为引用另一个数组。
- `&arrRef` 得到的是 `int (*)[10]`。
- `sizeof(arrRef)` 得到整个数组的大小。

## 5. 指针数组的引用

### 声明

```cpp
int *(&arry)[10] = ptrs;
```

这里的 `ptrs` 必须已经定义：

```cpp
int *ptrs[10];
int *(&arry)[10] = ptrs;
```

### 类型拆解

```text
(&arry)     -> arry 是一个引用
[10]        -> 它引用一个长度为 10 的数组
int *       -> 数组元素是 int*
```

所以：

```cpp
arry 的类型是 int *(&)[10]
```

含义是：

> `arry` 是一个引用，引用一个含有 10 个 `int*` 的数组。

`arry` 不是新数组，也不会复制 `ptrs`，它只是 `ptrs` 的别名。

### 示例

```cpp
int x = 1;
int y = 2;

int *ptrs[10] = {&x};
int *(&arry)[10] = ptrs;

arry[0] = &y;    // 同时修改 ptrs[0]

*arry[0] = 100;  // y 变成 100
```

`arry` 和 `ptrs` 指向同一个数组：

```cpp
arry[0]       // 就是 ptrs[0]
&arry == &ptrs
```

修改 `arry` 的元素，就是修改 `ptrs` 的元素：

```cpp
arry[0] = nullptr;

// 等价于
ptrs[0] = nullptr;
```

## 完整示例

```cpp
#include <iostream>

int main() {
    int arr[10] = {};

    int x = 1;
    int y = 2;
    int *ptrs[10] = {&x};

    int (*Parray)[10] = &arr;
    int (&arrRef)[10] = arr;
    int *(&arry)[10] = ptrs;

    (*Parray)[0] = 10;
    arrRef[1] = 20;

    arry[0] = &y;
    *arry[0] = 30;

    std::cout << arr[0] << '\n';   // 10
    std::cout << arr[1] << '\n';   // 20
    std::cout << x << '\n';        // 1
    std::cout << y << '\n';        // 30

    return 0;
}
```

## 常见误区

### 误混淆指针数组和数组指针

```cpp
int *a[10];    // 指针数组：10 个 int*
int (*b)[10];  // 数组指针：指向 int[10]
```

### 写成引用数组

```cpp
int &refs[10];    // 错误
int (&refs)[10];  // 正确，但这是数组引用
```

### 数组指针初始化错误

```cpp
int arr[10];
int (*p)[10] = arr;   // 错误，类型不匹配
int (*p)[10] = &arr;  // 正确
```

### 以为 `arry` 是独立数组

```cpp
int *ptrs[10];
int *(&arry)[10] = ptrs;
```

`arry` 只是 `ptrs` 的引用，不会创建新数组。

## 类型速查

| 写法 | 变量类型 | 含义 |
| --- | --- | --- |
| `int *ptrs[10]` | `int *[10]` | 10 个整型指针组成的数组 |
| `int &refs[10]` | 非法 | 引用数组不存在 |
| `int (*Parray)[10]` | `int (*)[10]` | 指向 10 个整数数组的指针 |
| `int (&arrRef)[10]` | `int (&)[10]` | 引用 10 个整数数组 |
| `int *(&arry)[10]` | `int *(&)[10]` | 引用 10 个整型指针数组 |

使用 `using` 可以更直观地表达：

```cpp
using IntArray = int[10];
using IntPtrArray = int*[10];

IntArray* p = &arr;         // 指向整数数组的指针
IntArray& r = arr;          // 引用整数数组
IntPtrArray& rp = ptrs;     // 引用指针数组
```

## 一句话记忆

```text
[] 优先级高于 * 和 &。
括号决定谁先和变量名结合。

int *p[10]      -> p 是数组，元素是指针
int (*p)[10]    -> p 是指针，指向数组
int (&p)[10]    -> p 是引用，引用数组
int *(&p)[10]   -> p 是引用，引用指针数组
```