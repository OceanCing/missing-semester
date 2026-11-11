# CMake 和 catkin 笔记

## 编译过程


1. 预处理
2. 编译
3. 汇编
4. 链接

预处理会处理 `#include` 和宏定义，链接则是把不同的目标文件和库合到一起。

```text
源代码 -> 预处理 -> 编译 -> 汇编 -> 链接 -> 可执行文件
```

## CMake

代码少的时候可以直接编译：

```bash
g++ main.cpp -o main
```

文件多了以后，这样一个个写就很麻烦。Makefile 可以记录编译规则，CMake 又可以帮我们生成 Makefile。

我一开始没有分清这几个工具，现在的理解是：

- `g++`：真正编译代码
- `make`：按照 Makefile 执行编译
- `CMake`：生成 Makefile

最简单的 `CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.10)
project(test)
add_executable(test main.cpp)
```

使用方法：

```bash
mkdir build
cd build
cmake ..
make
```

## catkin

catkin 是 ROS 里面使用的编译工具，它是在 CMake 的基础上做的。

工作空间中几个常见的文件夹：

- `src`：放源代码
- `build`：放编译的中间文件
- `devel`：放编译结果

课件中的创建命令：

```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace
catkin_create_pkg test_pkg std_msgs rospy roscpp
cd ..
catkin build
```

资料中还使用了 `catkin_make`。查了一下才知道，它和 `catkin build` 不是同一条命令，学习时跟着一种教程使用就行。

功能包中比较重要的是 `CMakeLists.txt` 和 `package.xml`。编译完成后还要运行：

```bash
source devel/setup.bash
```

## 关于大模型

Agent 可以让大模型读取文件、修改文件和运行命令。补充文章里用很少的代码做出了一个简单 Agent，我看完后对它怎么调用工具有了一点认识。

