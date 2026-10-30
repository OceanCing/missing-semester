# CMake 和 catkin 初学者笔记

> 这份笔记先记录基础概念，后面的内容再通过实验慢慢补充。


## 我目前的理解

一开始我容易把 CMake 和编译器混在一起。现在先这样记：

```text
源码 + CMakeLists.txt
        ↓ CMake 生成构建文件
Makefile、Ninja 或 Visual Studio 工程
        ↓ 构建工具调用编译器和链接器
可执行文件或库
```

所以 CMake 本身不负责把每个 `.cpp` 编译成机器码，它主要负责描述工程并生成构建规则。

catkin 是 ROS 1 的构建工具，它使用 CMake，并增加了 ROS 包、依赖、消息生成和工作空间管理等功能。catkin 不能和 CMake 简单说成互相替代。

## 程序是怎么编译出来的

资料把过程分成四步：

| 步骤 | 做什么 | 产物 |
| --- | --- | --- |
| 预处理 | 展开头文件和宏，处理条件编译，去掉注释 | `.i` |
| 编译 | 检查语法，生成汇编代码 | `.s` |
| 汇编 | 把汇编代码变成机器码 | `.o` 或 `.obj` |
| 链接 | 合并目标文件和库，处理函数、变量地址 | 可执行文件 |


两个容易混淆的地方：

1. 头文件已经找到了，函数仍然可能在链接时报“未定义引用”。
2. `.o` 文件虽然有机器码，但还不能单独运行，需要经过链接。

## CMake 最小项目

一个最小项目大概是这样：

```text
hello_cmake/
├── CMakeLists.txt
└── main.cpp
```

```cmake
cmake_minimum_required(VERSION 3.20)
project(hello_cmake LANGUAGES CXX)

add_executable(hello main.cpp)
```

常用命令：

```bash
cmake -S . -B build
cmake --build build
```

记住这些名词！！！：

- `add_executable()`：创建可执行文件。
- `add_library()`：创建库。
- `target_link_libraries()`：给目标链接库。
- `target_include_directories()`：给目标添加头文件路径。
- `find_package()`：查找已经安装的第三方库。
- `PUBLIC`、`PRIVATE`、`INTERFACE`：控制依赖是否传给使用当前目标的其他目标。




## catkin 工作空间

一个常见工作空间大致是：

```text
catkin_ws/
├── src/       # 源码和 ROS 包
├── build/     # CMake 构建文件
├── devel/     # 开发时生成的程序和环境脚本
└── log/       # 构建日志
```

在自己的 ROS 1 环境中，大致流程是：

```bash
source /opt/ros/noetic/setup.bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace

cd ..
catkin_make
source devel/setup.bash
```

创建测试包：

```bash
cd ~/catkin_ws/src
catkin_create_pkg study_pkg roscpp std_msgs
```
现在需要搞清楚的问题：

- `package.xml` 和 `CMakeLists.txt` 各自负责什么？
- 为什么新增依赖后两边都要改？
- `build/` 和 `devel/` 有什么区别？
- 为什么打开新终端后要重新 `source`？
- ROS 1 的 catkin 和 ROS 2 的 ament、colcon 不要混用。

## 200 行代码的 coding agent

这篇文章让我知道，coding agent 的核心并没有想象中神秘：

```text
用户提出任务
→ 模型判断要不要使用工具
→ 程序运行工具
→ 工具结果返回模型
→ 模型继续或结束
```

模型自己不能直接改文件，真正读写文件的是外层程序。文章原型提供了读文件、列文件和修改文件三个工具。

但它只是原型，不能直接当成安全的生产工具。真实使用还要考虑路径限制、危险操作确认、错误处理、上下文过长、并发和测试验证。

## 用大模型学习时，我会

1. 先写下自己的猜测。
2. 让模型解释，不急着让它直接改项目。
3. 运行最小例子，看看真实结果。
4. 查官方文档，检查版本和命令是否正确。
5. 把“我原来怎么想”和“后来为什么改”记下来。


```text
我理解 CMake 是“先生成构建文件，再由别的工具编译”。请指出这个理解哪里不完整，并给一个我能自己运行的小实验。
```

```text
这是我遇到的 CMake 报错。请先判断它属于配置、编译还是链接阶段，再说明依据。不要直接把整份文件重写。
```


模型说得通，不代表一定对。至少要通过编译器、构建日志、官方文档或小实验中的一种来确认。



## 我的学习记录

### 我实际做了什么

- 阅读的资料：
- 使用的系统和版本：
- 执行的命令：

### 遇到的第一个问题

- 报错或现象：
- 我一开始猜的原因：

### 我怎样验证

- 查了什么资料：
- 跑了什么命令：
- 关键结果：

### 我修正后的理解

-

### 还没弄懂的问题

-

## 欢迎继续补充

```markdown
### YYYY-MM-DD｜昵称

- 想学的部分：
- 我的预判：
- 实际操作：
- 真实报错或结果：
- 问过 AI 的问题：
- AI 哪里说得不清楚：
- 我最后确认的结论：
- 还没解决：
```

## 参考资料

- [程序详细编译过程（预处理、编译、汇编、链接）](https://zhuanlan.zhihu.com/p/476697014)
- [CMake 基础](https://www.runoob.com/cmake/cmake-basic.html)
- [The Emperor Has No Clothes](https://www.mihaileric.com/The-Emperor-Has-No-Clothes/)
- [catkin Tutorials](https://wiki.ros.org/catkin/Tutorials)
