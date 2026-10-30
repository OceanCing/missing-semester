# 第二课笔记：CMake、catkin 和 AI 辅助学习

## 1. 从之前的海龟工程开始

之前自学 ROS 时，我已经用过 `catkin build`，也运行过自己包里的海龟控制程序。

这篇笔记结合第二课的补充资料、之前的自学记录，以及我和 AI 的讨论来整理。环境配置、示例和部分验证由 AI 协助完成，资料检索、技术核对和文字整理也有 AI 参与。

之前使用的是 Docker 中的 Ubuntu 20.04 + ROS 1 Noetic。相关记录见[环境与构建笔记][self-build]，下面主要沿用里面的 `turtle_control` 包。

## 2. 源码怎样变成能运行的程序

[《程序详细编译过程》][compilation]把这个过程分成预处理、编译、汇编和链接。先把这四步分清楚，再看 CMake 配置会更容易理解。

| 阶段 | 主要做什么 | 以 C 程序为例的结果 |
| --- | --- | --- |
| 预处理 | 展开宏、处理条件编译和头文件包含等指令 | `hello.i`，仍然是文本 |
| 编译 | 分析代码并生成汇编代码 | `hello.s` |
| 汇编 | 把汇编代码转换成机器指令 | `hello.o`，可重定位目标文件 |
| 链接 | 处理目标文件和库之间的符号引用与地址，形成可执行文件 | `hello` |

下面是分阶段查看结果的命令示意，停止阶段也对照了 [GCC 官方说明][gcc-options]。

```bash
gcc -E hello.c -o hello.i
gcc -S hello.i -o hello.s
gcc -c hello.s -o hello.o
gcc hello.o -o hello
```

`gcc hello.c -o hello` 可以把这些步骤连续完成。平时说“编译一下”通常包括整套过程，讨论报错时则要分清具体卡在哪一步。

文章后面还讲了静态链接、动态链接和共享库。静态库中的所需目标代码会被链接进最终程序；动态库的代码可以在加载或运行时与程序连接。头文件主要提供声明，写了 `#include` 并不等于已经链接了实现。

对应到海龟工程，头文件找不到通常与依赖或头文件搜索路径有关；`undefined reference` 则属于链接阶段的问题，可能与缺少函数实现或链接库有关。[catkin_tools 的排错说明][catkin-migration]也区分了这两类问题。

## 3. CMake 负责组织构建

### CMake、构建工具和编译器的分工

[《CMake 基础》][cmake-basic]介绍了 `CMakeLists.txt` 和常用指令。对照已有工程，可以把几个工具的关系写成：

```text
CMakeLists.txt
    ↓ CMake 配置并生成
构建文件，例如 Makefile 或 build.ninja
    ↓ Make 或 Ninja 按规则调用工具
编译、汇编和链接
    ↓
可执行程序或库
```

CMake 读取项目配置，查找依赖并生成构建系统；编译器负责处理源码。`cmake --build` 可以统一调用生成器对应的构建工具。这些步骤在 [CMake 命令手册][cmake-cli]中也分别列出了入口。

普通 CMake 项目的命令示意如下：

```bash
cmake -S . -B build
cmake --build build
```

第一行指定源码目录和构建目录，完成配置与生成；第二行执行构建。构建产物集中放在 `build` 中，源码目录更容易保持清楚。

### 回到我用过的 CMakeLists.txt

海龟包里实际使用过的配置片段是：

```cmake
cmake_minimum_required(VERSION 3.8)
project(turtle_control)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs roscpp turtlesim tf2 tf2_ros tf2_geometry_msgs)
catkin_package(CATKIN_DEPENDS
  geometry_msgs roscpp turtlesim tf2 tf2_ros tf2_geometry_msgs)

add_executable(turtle_motion src/turtle_motion.cpp)
target_compile_features(turtle_motion PRIVATE cxx_std_14)
target_compile_options(turtle_motion PRIVATE -Wall -Wextra)
target_include_directories(turtle_motion PRIVATE ${catkin_INCLUDE_DIRS})
target_link_libraries(turtle_motion ${catkin_LIBRARIES})
```

| 配置 | 在这个例子中的作用 |
| --- | --- |
| `cmake_minimum_required` | 声明项目要求的最低 CMake 版本 |
| `project` | 定义项目名称 |
| `find_package` | 查找 catkin 及所需组件，找不到必需组件时停止配置 |
| `catkin_package` | 声明向其他包导出的 catkin 包信息，这里列出相关依赖 |
| `add_executable` | 指定用哪个源文件构建哪个可执行程序 |
| `target_compile_features` | 为这个目标要求 C++14 功能支持 |
| `target_compile_options` | 为这个目标添加编译警告选项 |
| `target_include_directories` | 指定查找头文件的目录 |
| `target_link_libraries` | 指定要链接的库 |

`${catkin_INCLUDE_DIRS}` 和 `${catkin_LIBRARIES}` 是 CMake 变量。前者提供头文件目录，后者提供库信息。构建目标把源码、编译选项和链接库关联起来，对应到这里就是 `turtle_motion` 这个程序。

### 这次补充的语法认识

菜鸟教程还介绍了 `add_library`、`install`、变量和缓存，以及 `PUBLIC`、`PRIVATE`、`INTERFACE`。结合[头文件路径的官方说明][cmake-includes]，这三个关键字可以先这样理解：

| 关键字 | 当前目标使用 | 传递给依赖该目标的其他目标 |
| --- | --- | --- |
| `PRIVATE` | 是 | 否 |
| `PUBLIC` | 是 | 是 |
| `INTERFACE` | 否 | 是 |

普通变量用 `set(NAME value)` 设置，通过 `${NAME}` 引用。缓存变量保存在构建目录的 `CMakeCache.txt` 中，可以通过 `-DNAME=value` 设置。`set(... CACHE ...)` 在已有缓存时默认保留原值，这点也对照了 [CMake 的 set 文档][cmake-set]。

## 4. catkin 把 ROS 软件包组织起来

### catkin 和 CMake 的关系

catkin 基于 CMake，增加了 ROS 软件包所需的构建约定和工具。一个工作空间可以放多个包，每个包有自己的依赖和构建配置。

我之前实际使用的是下面这套流程：

```bash
cd ~/catkin_ws/src
catkin_create_pkg turtle_control roscpp geometry_msgs
cd ~/catkin_ws
catkin init
catkin build turtle_control -j2
source devel/setup.bash
rospack find turtle_control
```

原记录中，最后一条命令找到了工作空间里的包。之后增加海龟和 TF 示例时，也补充了相应依赖。

[catkin 官方教程][catkin-tutorials]主要使用 `catkin_make`。`catkin build` 来自 catkin_tools，它按包隔离构建；`catkin_make` 把工作空间中的 catkin 包作为一个整体 CMake 工程处理。[catkin_tools 文档][catkin-migration]说明了两者的差别。

### package.xml 和 CMakeLists.txt 分别记录什么

在我的包里，`package.xml` 有：

```xml
<depend>tf2_ros</depend>
```

它声明包对 `tf2_ros` 的依赖。`CMakeLists.txt` 中的 `find_package`、头文件目录和链接规则，再描述构建这个程序需要怎样使用相关依赖。

`package.xml` 是依赖和包信息的清单，`CMakeLists.txt` 负责具体构建。官方的[创建包教程][catkin-package]介绍了包结构和依赖信息，[构建 C++ 程序的说明][catkin-executables]则给出了目标和链接配置。

### 构建、安装和 source

官方的[构建与使用教程][catkin-build]把工作空间分成几类目录：

| 目录 | 用途 |
| --- | --- |
| `src` | 保存源码和软件包配置 |
| `build` | 保存构建中间文件 |
| `devel` | 在开发时使用构建结果和环境脚本 |
| `install` | 按安装规则整理文件，用于安装和分发 |

我之前主要使用 `devel`。包里的 `install(...)` 定义了程序、launch 和配置等文件的安装位置，安装步骤按这些规则整理文件。

`source devel/setup.bash` 把工作空间加入当前 Shell 的查找环境，让 ROS 工具能够找到相应的包和程序。

[工作空间叠加教程][catkin-overlay]进一步解释了为什么系统安装的包和自己修改的包可以同时存在：在配置时建立底层工作空间关系，加载上层环境后，工具可以优先找到上层的同名包。

### 修改与构建的关系

回到之前的工程，可以把日常操作整理成：

| 修改内容 | 通常需要的操作 |
| --- | --- |
| 参与构建的 C++ 源码或头文件 | 重新构建，再重新运行节点 |
| 新增独立 C++ 节点 | 增加目标和必要依赖，再构建、运行 |
| launch 或启动参数 | 重新启动相关节点加载配置 |

## 5. 我对 AI 辅助学习的思考

[《The Emperor Has No Clothes》][agent-article]用一个简化的编程 Agent 说明了流程：模型根据对话提出工具调用，外部程序执行读取、列目录或修改文件，把结果交回模型，再继续下一步。

从这个原理看，AI 能做什么，和它拿到了什么信息、能够使用哪些工具有关。学习 CMake 和 catkin 时，如果把环境、构建文件和运行结果放进上下文，就能让它结合具体工程解释。对我来说，直接看 `turtle_control` 中的配置，再理解这些配置和程序的关系，会更容易把概念连起来。

我觉得，AI 能完成的环境配置、示例编写和错误定位，可以持续借助它来做。自己更需要理解程序的输入、输出、关键参数，以及为什么要这样改。这样可以把更多时间放在程序的功能和关键逻辑上，工程里的细节随着实际需要逐步补充。

比如海龟控制，AI 可以准备控制程序和构建配置，我关注速度、方向和目标位置怎样影响运动。改参数之前，先想一下结果会怎样，再观察实际运动是否符合预期。遇到报错也可以让 AI 定位，修复以后再看它改了什么、解决了什么问题。这和[之前自学时的分工][self-overview]是一致的。

对我来说，判断自己有没有学会，可以看能不能用自己的话解释程序，能不能预测修改后的变化，以及实际结果和预期不同时能不能把问题描述清楚。AI 可以继续帮助分析和实现，我需要对学习目标、关键逻辑和最终效果有自己的判断。

## 6. 参考资料

### 课程指定阅读

1. [程序详细编译过程（预处理、编译、汇编、链接）][compilation]：编译阶段、链接和共享库。
2. [CMake 基础][cmake-basic]：构建目标、变量、头文件路径、链接与安装规则。
3. [The Emperor Has No Clothes: How to Code Claude Code in 200 Lines of Code][agent-article]：工具调用与 Agent 循环。
4. [Catkin Tutorials][catkin-tutorials]：创建工作空间、创建包、构建与安装、工作空间叠加。

### 技术核对与已有记录

- [GCC：控制编译输出阶段][gcc-options]。
- [CMake 3.16 命令手册][cmake-cli]、[目标头文件路径][cmake-includes]、[变量与缓存][cmake-set]。
- [catkin_tools：与 catkin_make 的区别及迁移][catkin-migration]。
- [catkin：构建和安装 C++ 可执行程序][catkin-executables]。
- [自己的环境与构建记录][self-build]、[自学总览与 AI 学习分工][self-overview]。引用固定提交版本，便于核对当时的内容。

[compilation]: https://zhuanlan.zhihu.com/p/476697014
[cmake-basic]: https://www.runoob.com/cmake/cmake-basic.html
[agent-article]: https://www.mihaileric.com/The-Emperor-Has-No-Clothes/
[catkin-tutorials]: https://wiki.ros.org/catkin/Tutorials
[catkin-package]: https://wiki.ros.org/catkin/Tutorials/CreatingPackage
[catkin-build]: https://wiki.ros.org/catkin/Tutorials/using_a_workspace
[catkin-overlay]: https://wiki.ros.org/catkin/Tutorials/workspace_overlaying
[gcc-options]: https://gcc.gnu.org/onlinedocs/gcc/Overall-Options.html
[cmake-cli]: https://cmake.org/cmake/help/v3.16/manual/cmake.1.html
[cmake-includes]: https://cmake.org/cmake/help/v3.16/command/target_include_directories.html
[cmake-set]: https://cmake.org/cmake/help/v3.16/command/set.html
[catkin-migration]: https://catkin-tools.readthedocs.io/en/latest/migration.html
[catkin-executables]: https://github.com/ros/catkin/blob/noetic-devel/doc/howto/format2/building_executables.rst
[self-build]: https://github.com/Wit-1/missing-semester/blob/a12e74a6db546f49e0f745b83cce3f6207ae781f/2027/Wit-1/self-study/ros1/01-environment-build.md
[self-overview]: https://github.com/Wit-1/missing-semester/blob/a12e74a6db546f49e0f745b83cce3f6207ae781f/2027/Wit-1/self-study/README.md
