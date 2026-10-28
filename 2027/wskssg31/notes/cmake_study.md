# 第二课：CMake 与 Catkin 学习笔记

## 一、CMake 基础

### 1. 为什么需要 CMake？
- 尽管一开始比较麻烦，但是 CMake 可以更好地管理项目的编译和构建过程，方便修改和管理
- CMake 还可以帮助管理第三方库、编译选项和不同操作系统上的构建配置。

### 2. CMake 与 g++ 的区别
- 实际编译需要 g++ 等编译器
- CMake 本身不是 C++ 编译器。它负责组织和配置构建过程
- CMake 不会代替 g++
- 尽管一开始比较麻烦，但是 CMake 可以更好地管理代码，方便修改和管理。

### 3. 第一次使用 CMake
`-cmake -S . -B build`
调用 CMake   将构建文件放到 build 文件夹

`-cmake --build build`
调用相应的构建工具和编译器，构建我们的 C++ 程序

`-./build/hello`
运行程序

### 4. 实验中遇到的问题与收获


## 二、Catkin 基础

### 1. 什么是 Catkin？
- Catkin 是 ROS 1 使用的构建系统，它建立在 CMake 的基础上

### 2. Catkin 与 CMake 的区别
- Cmake通用 C/C++ 项目构建
- Catkin用于ROS 1 项目构建
- Catkin 不是 CMake 的替代品，而是在 CMake 基础上提供了 ROS 相关功能

### 3. Catkin 工作空间
```catkin_ws/
├── src/  存放源代码和功能包
├── build/  存放构建过程产生的中间文件
└── devel/  存放开发环境相关的构建产物
```

### 4. ROS 功能包（Package）
- 在 Catkin 功能包中，CMakeLists.txt 同样负责描述构建规则，不过通常还会包含 Catkin 相关配置。
- CMakeLists.txt  用于描述如何构建程序
- package.xml  用于描述功能包的信息和依赖关系

### 5. 实验过程与个人收获
之前我认为 Catkin 包含机器人控制系统，后来通过和ai交流发现，Catkin 实际上主要负责 ROS 1 软件包的构建。


## 三、AI 辅助学习与个人思考

### 1. 学习过程中遇到的问题

 在学习 CMake 和 Catkin 时，我主要遇到了以下问题：
- 一开始不理解为什么有了 g++ 编译器还需要 CMake。
- 最初认为 Catkin 包含机器人控制系统。
- 对 Catkin 工作空间中的 src、build 和 devel 目录不太熟悉。
- 不清楚 CMakeLists.txt 和 package.xml 的区别。

### 2. AI 如何帮助我解决问题

在 AI 的指导下，我进行了以下操作：
- 使用 g++ 编译并运行 Hello CMake 程序。
- 编写 CMakeLists.txt，使用 CMake 构建同一个程序。
- 创建模拟 Catkin 工作空间，理解目录结构。
- 创建模拟 ROS 功能包 motor_pkg。
- 通过问答纠正了对 Catkin 的错误认识。

### 3. 我的学习收获
- 通过这次学习，我理解了 CMake 和 Catkin 的基本作用。
- CMake 主要用于管理通用 C/C++ 项目的构建过程，而 Catkin 是基于 CMake 的 ROS 1 构建系统。
- 我还认识到，源代码和构建文件分开存放，可以让项目结构更加清晰，方便维护和重新构建。
- 在使用 AI 辅助学习的过程中，我发现 AI 可以帮助解释知识点和检查错误，但仍然需要自己实际操作和思考，才能真正理解这些工具。
