# 第二课学习笔记：CMake 与 Catkin

## 一、CMake 基础

### 1. 为什么需要 CMake？

当 C++ 项目只有一个源文件时，可以直接使用 g++ 编译。但随着项目规模扩大，源文件和依赖库增加，
手动管理编译命令会越来越麻烦。因此可以使用 Makefile 管理构建规则，再使用 CMake 简化大型项目的构建配置。

### 2. g++、Makefile、CMake 的区别

- g++：负责实际编译和链接 C++ 程序。
- Makefile：描述文件之间的依赖关系和构建规则。
- make：读取 Makefile 并执行相应命令。
- CMake：读取 CMakeLists.txt，生成对应的构建系统。

## 二、Catkin 基础

### 1. Catkin 是什么？

Catkin 是 ROS 1 使用的构建系统，它建立在 CMake 的基础上。

CMake 主要负责普通 C/C++ 项目的构建配置，而 Catkin 在此基础上增加了对 ROS 功能包、包之间的依赖关系以及消息生成等功能的支持。

两者的关系可以理解为：**CMake 负责组织代码的编译过程，而 Catkin 在此基础上进一步解决 ROS 多功能包协作构建的问题。**

例如，一个机器人项目可能同时包含视觉识别、电机控制和运动规划等多个 ROS 功能包。这些功能包可能存在依赖关系，Catkin 可以帮助我们按照构建依赖组织整个项目。

因此，Catkin 并不是用来替代 CMake 的，而是在 CMake 基础上提供适合 ROS 1 开发的构建功能。

### 2. Catkin 工作空间

典型目录：

- src：存放 ROS 功能包的源代码。
- build：存放构建过程中的中间文件。
- devel：存放开发阶段的构建产物与环境配置。

### 3. ROS 功能包

重要文件：

- CMakeLists.txt：描述构建规则。
- package.xml：描述功能包信息及依赖关系。

### 4. 常用命令

- catkin_init_workspace：初始化工作空间。
- catkin_create_pkg：创建功能包。
- catkin build：构建工作空间或功能包。

### 5. 我的理解

CMake 解决的是项目如何构建的问题，而 Catkin 进一步解决了 ROS 多功能包如何高效协同构建的问题。

## 三、大模型基础与辅助开发

### 1. 如何有效使用大模型？

1. 明确目标：说明最终需要实现什么功能。
2. 提供上下文：提供必要的代码、硬件信息和项目背景。
3. 明确约束：指出哪些功能、文件或参数不能随意修改。
4. 规定验证标准：说明怎样判断任务是否完成。
5. 避免过度规定步骤：允许 Agent 在约束范围内选择合适的方法。

### 2. 我对 AI 辅助开发的理解

AI 适合作为辅助开发工具。开发者仍然需要理解代码的工作原理，并通过编译、调试和实际运行来验证结果。

---

## 四、补充资料学习

### 1. CMake 基础教程

资料：<https://www.runoob.com/cmake/cmake-basic.html>

CMake 使用 `CMakeLists.txt` 描述项目构建规则。

除了之前实验中使用的 `project()` 和 `add_executable()`，还可以进一步了解：

| 命令 | 作用 |
|---|---|
| `add_library()` | 创建库目标 |
| `target_link_libraries()` | 为目标链接库 |
| `target_include_directories()` | 设置目标的头文件搜索路径 |
| `set()` | 设置变量 |
| `find_package()` | 查找外部依赖包 |

例如：

```cmake
add_library(motor STATIC motor.cpp)

add_executable(robot main.cpp)

target_link_libraries(robot PRIVATE motor)
```

这里首先将 `motor.cpp` 构建为静态库，然后将其链接到 `robot` 可执行程序。

通过这个例子可以看出，CMake 不仅能够编译单个程序，还能够管理库与程序之间的依赖关系。

### 2. Catkin 官方教程

资料：<https://wiki.ros.org/catkin/Tutorials>

Catkin 工作空间可以包含多个 ROS 功能包。

学习时需要重点关注：

- 工作空间的创建与初始化
- 功能包的基本结构
- `CMakeLists.txt` 的配置
- `package.xml` 中的依赖声明
- 功能包之间的构建关系

Catkin 与普通 CMake 的主要区别，在于它提供了 ROS 专用的构建功能。

### 3. Agent 工作原理补充阅读

资料：<https://www.mihaileric.com/The-Emperor-Has-No-Clothes/>

文章通过实现一个简化的编程 Agent，介绍大模型与工具交互的基本原理。

其中一个重要观点是：模型本身并不是直接操作计算机文件，而是提出工具调用请求，再由外部程序执行对应操作。

一个简单的编程 Agent 可以通过以下能力完成基本工作：

1. 读取文件
2. 列出目录
3. 修改文件

模型获得工具返回的结果后，可以继续分析和执行后续任务。

这说明编程 Agent 的重要组成部分不仅是大模型，还包括工具接口、执行环境和持续交互的流程。

### 4. 知乎补充资料

资料：<https://zhuanlan.zhihu.com/p/476697014>

---

## 五、个人学习总结

### 1. 对 CMake 的理解

分别使用 G++ 和 CMake 编译了同一个 C++ 程序，最终都成功输出了 `Hello CMake!`。

通过对比，我理解了 G++ 和 CMake 的区别：G++ 负责实际编译，而 CMake 负责描述和组织整个项目的构建过程。

对于只有一个源文件的小程序，直接使用 G++ 更简单；但当项目包含多个源文件和依赖库时，CMake 更方便维护。

### 2. 对 Catkin 的理解

Catkin 建立在 CMake 的基础上，主要面向 ROS 1 开发。

它不仅需要完成普通 C++ 程序的构建，还需要处理 ROS 功能包之间的依赖以及消息生成等问题。

这让我理解了为什么 ROS 项目需要专门的构建系统。

### 3. 对 AI 辅助学习的思考

我认为学习过程中应该先理解问题，再利用 AI 提供解释和思路，最后通过实际操作验证结果。

对于涉及硬件控制的程序，不能只看代码是否能够编译，还需要结合实际运行效果判断其正确性。
