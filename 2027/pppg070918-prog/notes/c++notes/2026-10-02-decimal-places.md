---
title: C++ 中如何保留小数
created: 2026-10-02
updated: 2026-10-02
tags: [cpp, iostream, iomanip, floating-point, formatting]
status: active
source: 对话整理
---

# C++ 中如何保留小数

本文整理 C++ 中控制浮点数小数位、格式化输出和数值舍入的常见方法。

## 核心写法

如果已经使用了：

```cpp
using namespace std;
```

保留小数最常见的写法是：

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double x = 3.1415926;

    cout << fixed << setprecision(2) << x << endl;
}
```

输出：

```text
3.14
```

通用格式：

```cpp
cout << fixed << setprecision(n) << x;
```

含义：

- `fixed`：使用普通小数形式输出。
- `setprecision(n)`：在 `fixed` 模式下，表示小数点后保留 `n` 位。
- 输出时会进行四舍五入，但不会改变变量本身的值。

## `fixed` 是什么

`fixed` 是 C++ 中用于控制浮点数输出格式的流操作符。

它的主要作用是：

> 强制以普通小数形式输出，并让 `setprecision(n)` 表示小数点后保留 `n` 位。

例如：

```cpp
double x = 3.1415926;

cout << fixed << setprecision(2) << x << endl;
```

输出：

```text
3.14
```

如果不使用 `fixed`：

```cpp
cout << setprecision(2) << x << endl;
```

此时 `setprecision(2)` 表示总共保留两位有效数字，输出通常为：

```text
3.1
```

因此，如果要保留指定的小数位数，通常需要同时使用：

```cpp
fixed
setprecision(n)
```

### 输出格式对比

```cpp
double x = 12345.678;

cout << fixed << setprecision(2) << x << endl;
cout << scientific << setprecision(2) << x << endl;
```

输出：

```text
12345.68
1.23e+04
```

### 格式会持续生效

`fixed` 会影响之后的浮点数输出，直到使用 `scientific` 或 `defaultfloat` 修改格式：

```cpp
cout << fixed << setprecision(2) << 1.23456 << endl;
// 1.23

cout << defaultfloat << setprecision(2) << 1.23456 << endl;
// 1.2
```

> [!NOTE]
> `fixed` 只改变输出格式，不会改变变量本身的数值。

## `#include <iomanip>` 有什么用

`iomanip` 全称可以理解为：

> Input/Output Manipulators，输入输出流格式控制工具。

它主要用于控制 `cout`、`cin` 等流对象的显示格式。

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double x = 3.1415926;

    cout << fixed << setprecision(2) << x << endl;
}
```

常用工具包括：

| 工具 | 作用 |
| --- | --- |
| `setprecision(n)` | 设置浮点数精度 |
| `setw(n)` | 设置输出宽度 |
| `setfill(c)` | 设置填充字符 |
| `left` | 左对齐 |
| `right` | 右对齐 |
| `hex`、`oct`、`dec` | 设置整数进制 |

示例：

```cpp
cout << setw(5) << setfill('0') << 42 << endl;
```

输出：

```text
00042
```

> [!TIP]
> `using namespace std;` 只是省略 `std::` 前缀，不会代替头文件，因此仍然需要引入 `<iostream>`、`<iomanip>` 等头文件。

## 真正舍入数值

前面的方法只改变输出格式。

如果希望把数值本身舍入，可以使用 `std::round`：

```cpp
#include <cmath>

double x = 3.1415926;

double y = round(x * 100.0) / 100.0;
// y == 3.14
```

保留 `n` 位小数可以写成函数：

```cpp
double roundTo(double x, int n) {
    double scale = pow(10.0, n);
    return round(x * scale) / scale;
}
```

使用前需要包含：

```cpp
#include <cmath>
```

例如：

```cpp
double x = 3.1415926;

cout << fixed << setprecision(2)
     << roundTo(x, 2) << endl;
```

输出：

```text
3.14
```

> [!WARNING]
> `double` 是二进制浮点数，不能精确表示所有十进制小数。例如 `0.1` 在二进制中并不是一个精确值。

## 金额与高精度场景

涉及金额等精度要求较高的场景，不建议直接使用 `double` 进行连续运算。

推荐使用整数保存最小单位：

```cpp
long long cents = 12345;
// 表示 123.45 元
```

需要显示时再转换：

```cpp
cout << fixed << setprecision(2)
     << cents / 100.0 << endl;
```

输出：

```text
123.45
```

这样做可以避免部分浮点数精度问题。

## 常见问题

### `fixed` 会改变变量吗？

不会。

```cpp
double x = 3.1415926;

cout << fixed << setprecision(2) << x << endl;
// 输出 3.14
```

变量 `x` 的值并没有变成 `3.14`，只是输出格式发生了变化。

### `setprecision(2)` 为什么有时不是保留两位小数？

因为没有结合 `fixed`。

```cpp
cout << setprecision(2) << 3.1415926 << endl;
// 通常输出 3.1
```

加上 `fixed` 后：

```cpp
cout << fixed << setprecision(2) << 3.1415926 << endl;
// 输出 3.14
```

### 为什么输出时会四舍五入？

例如：

```cpp
cout << fixed << setprecision(2) << 3.145 << endl;
```

通常输出：

```text
3.15
```

具体结果可能受到浮点数的二进制表示和舍入模式影响。

### 为什么金额最好不要直接用 `double`？

因为浮点数存在精度误差，多次计算后可能出现类似结果：

```text
0.1 + 0.2 != 0.3
```

金额建议使用整数保存最小单位，例如“分”，或者使用专门的十进制数学库。

## 快速参考

| 场景 | 写法 |
| --- | --- |
| 保留两位小数输出 | `cout << fixed << setprecision(2) << x;` |
| 保留 `n` 位小数输出 | `cout << fixed << setprecision(n) << x;` |
| 将数值舍入到两位小数 | `round(x * 100.0) / 100.0` |
| 将数值舍入到 `n` 位小数 | `round(x * pow(10.0, n)) / pow(10.0, n)` |
| 金额存储 | 使用整数保存“分” |

## 最常用的代码

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double x = 3.1415926;

    cout << fixed << setprecision(2) << x << endl;
    // 3.14

    return 0;
}
```

核心口诀：

```text
fixed 控制小数格式
setprecision 控制小数位数
iomanip 提供格式控制工具
```