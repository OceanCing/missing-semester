# C++ 笔记

## 1. 从 C 到 C++

c语言和c++的区别

| C 语言写法 | C++ 对应写法 | 说明 |
| ---- | ---- | ---- |
| `#include <stdio.h>` | `#include <iostream>` | 输入输出头文件变了 |
| printf / scanf | cout / cin | 不再需要 %d、& 这些格式符 |
| `#include <string.h>` | `#include <string>` | 用 std::string |
| char 字符数组[] | string 字符串 | `string` 可以直接 `+`、比较、取长度 |
| strlen(字符串) | 字符串.length() / 字符串.size() | string 自带长度 |
| strcpy(a, b) | 字符串 = 另一个字符串 | string 可以直接赋值 |
| strcmp(a, b) | 字符串 == 另一个字符串 | string 可以直接比较 |
| malloc / free | `new` / `delete` | new 会自动计算类型大小 |
| NULL | nullptr | 更安全，不容易和整数混淆 |
| int *p 传参 | int &p 引用传参 | 引用相当于别名，比指针简单 |
| int 表示真假 | bool | 用 true / false |
| #define PI 3.14 | const double PI = 3.14; | 用const |
| struct 只放数据 | struct 可放函数 | C++ 的 struct 可以写成员函数 |
| 函数只能一个名字 | 同名函数可以重载 | 参数不同就可以写同名函数 |

## 2. string

字符串容器，本质是一个**类**，内部封装了 char*，不用自己处理 '\0' 结束符

### 常用操作
- #### 创建

  - 空字符串 **string 字符串;**
  - 用 C 风格字符串构造 **string 字符串("内容");**
  - 拷贝构造 **string 字符串(另一个字符串);**
  - 重复若干个相同字符 **string 字符串(个数, '字符');**  `5 个 'a' 得到 "aaaaa"`

- #### 赋值

  - 直接赋值 **字符串 = "内容";**
  - assign 赋值 **字符串.assign("内容");**
  - 取前 n 个字符赋值 **字符串.assign("内容", 个数);**  `"abcdef" 取 3 个得到 "abc"`
  - 赋若干相同字符 **字符串.assign(个数, '字符');**  `5 个 'x' 得到 "xxxxx"`

- #### 拼接

  - 尾部拼接 **字符串 += "内容";**  `"hello" 拼上 " world" 得到 "hello world"`
  - append 拼接 **字符串.append("内容");**

- #### 查找

  - 从左往右找，返回首次出现的下标 **字符串.find("内容");**  `"hello world" 里找 "world" 得到 6`
  - 从右往左找 **字符串.rfind("内容");**
  - 找不到时返回 **string::npos** （不是 -1，判断必须写 位置 == string::npos）

- #### 替换 / 插入 / 删除

  - 从下标开始的 n 个字符换成新内容 **字符串.replace(下标, 个数, "新内容");**  `"hello world" 的 0 起 5 个换成 "hi" 得到 "hi world"`
  - 在下标前面插入 **字符串.insert(下标, "内容");**  "`hello" 的下标 1 前插 "222" 得到 "h222ello"`
  - 从下标开始删 n 个 **字符串.erase(下标, 个数);**  `"h222ello" 从 1 删 3 个得到 "hello"`
  - 插入和删除的起始下标都从 0 开始

- #### 取子串

  - 从下标开始取 n 个 **字符串.substr(下标, 个数);**  `"abcdef" 取 1、3 得到 "bcd"`
  - 截取 @ 前面的用户名 **字符串.substr(0, 字符串.find("@"));**  `"zhangsan@sina.com" 得到 "zhangsan"`
  - 通常和 find 配合使用

- #### 字符存取

  - 下标访问，越界不报错 **字符串[下标];**
  - 会检查越界 **字符串.at(下标);**
  - 第一个字符 **字符串.front();**  最后一个字符 **字符串.back();**

- #### 比较

  - 直接用 == 比较 **字符串 == 另一个字符串**
  - 相等返回 0，大于返回正，小于返回负 **字符串.compare(另一个字符串);**

- #### 长度与判空

  - 长度 **字符串.size();**  同 size() **字符串.length();**
  - 为空返回 true **字符串.empty();**


## 3. vector

单端数组，和数组很像但可以动态扩展。扩展不是原地续接，而是：找更大的空间 → 拷贝原数据 → 释放旧空间

### 常用操作
- #### 创建

  - 默认构造，空 **vector<int> 数组;**
  - 区间拷贝 **vector<int> 数组(数组.begin(), 数组.end());**
  - n 个相同值 **vector<int> 数组(个数, 值);**  `10 个 100`
  - 拷贝构造 **vector<int> 数组(另一个数组);**

- #### 赋值

  - 赋 n 个相同值 **数组.assign(个数, 值);**  `5 个 10`
  - 直接赋值 **数组 = 另一个数组;**

- #### 容量与大小

  - 是否为空 **数组.empty();**
  - 元素个数 **数组.size();**
  - 能容纳的元素个数 **数组.capacity();**
  - 重新指定大小，多删少补 **数组.resize(个数);**
  - 多出来的补指定值 **数组.resize(个数, 值);**
  - resize 改的是 size，reserve 只改 capacity，不创建对象

- #### 插入

  - 尾部插入 **数组.push_back(值);**
  - 在迭代器位置插入 **数组.insert(数组.begin(), 值);**
  - 插入 n 个相同值 **数组.insert(数组.begin(), 个数, 值);**

- #### 删除

  - 尾部删除 **数组.pop_back();**
  - 删除迭代器位置的元素 **数组.erase(数组.begin());**
  - 删除区间 [begin, begin+n) **数组.erase(数组.begin(), 数组.begin() + 个数);**
  - 清空 **数组.clear();**

- #### 存取

  - 下标访问，不检查越界 **数组[下标];**
  - 检查越界 **数组.at(下标);**
  - 第一个元素 **数组.front();**  最后一个元素 **数组.back();**

### 遍历
- #### for 循环迭代器（最常用）

```cpp
for (vector<int>::iterator it = 数组.begin(); it != 数组.end(); it++) {
    cout << *it << endl;
}
```

- #### 起止迭代器 + while

```cpp
vector<int>::iterator itBegin = 数组.begin();
vector<int>::iterator itEnd   = 数组.end();
while (itBegin != itEnd) { cout << *itBegin << endl; itBegin++; }
```

- #### STL 遍历算法

  - 配合普通函数使用 **for_each(数组.begin(), 数组.end(), 函数名);**

### 自定义类型与嵌套
- #### 存放自定义类型

  - 尾插一个对象 **数组.push_back(对象);**  `Person("a", 10)`
  - 迭代器访问成员 **it->成员名**  `例如 it->name、it->age`

- #### 容器嵌套（二维数组）

  - 大容器里放小容器 **vector<vector<int>> 数组;**
  - 内层遍历 **it->begin();**  **it->end();**

```cpp
for (vector<vector<int>>::iterator it = 数组.begin(); it != 数组.end(); it++) {
    for (vector<int>::iterator vit = it->begin(); vit != it->end(); vit++) {
        cout << *vit << " ";
    }
}
```

