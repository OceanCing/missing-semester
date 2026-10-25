# CMake & Catkin

## 1. From Source Code to Executable Program

C/C++ 源代码不能直接由 CPU 执行，需要经过预处理、编译、汇编和链接四个阶段。

```text
源文件 .c/.cpp
    ↓ 预处理
预处理文件 .i
    ↓ 编译
汇编文件 .s
    ↓ 汇编
目标文件 .o
    ↓ 链接
可执行文件
```

### 1.1 预处理

预处理器主要完成以下工作：

- 展开 `#define` 定义的宏；
- 处理 `#if`、`#ifdef` 等条件编译指令；
- 将 `#include` 引入的文件插入源文件；
- 删除注释；
- 保留编译所需的文件和行号信息。



### 1.2 编译

编译器对预处理后的代码进行词法分析、语法分析、语义分析和优化，将其转换为汇编代码。

```bash
gcc -S hello.i -o hello.s
```

### 1.3 汇编

汇编器把汇编代码转换为机器指令，并生成可重定位目标文件：

```bash
gcc -c hello.s -o hello.o
```

目标文件已经包含机器代码，但其中引用的外部函数和变量可能尚未确定最终地址，因此还不能直接运行。

### 1.4 链接

链接器将目标文件和依赖库组合起来，解析符号引用并进行地址重定位，最终生成可执行文件：

```bash
gcc hello.o -o hello
```

静态链接会把所需代码复制进可执行文件；动态链接则在加载或运行过程中使用共享库。

### 1.5 虚拟内存

虚拟内存又叫虚拟存储器，它提供了三个重要的能力：

   1. 将主存看成一个存储在磁盘上的地址空间的高速缓存，在主存中只保存活动区域，并根据需要在磁盘和主存之间来回传送数据（这里存在“交换空间”以及“页面调度”等概念），通过这种方式，高效地利用主存；
   2. 为每个进程提供了统一的地址空间（以虚拟地址编址），从而简化了存储器管理；
   3. 操作系统会为每个进程提供独立的地址空间，从而保护了每个进程的地址空间不被其他进程破坏。 

## 2. CMake

### 2.1 What Is CMake?

CMake 是一个跨平台的构建系统生成工具，它不是编译器。

CMake 读取项目中的 `CMakeLists.txt`，根据其中的规则生成 Makefile、Ninja 文件或 Visual Studio 工程，再由对应的构建工具调用 GCC、Clang等编译器。

```text
CMakeLists.txt
    ↓ CMake 配置
Makefile / Ninja 构建文件
    ↓ 构建工具
编译器和链接器
    ↓
可执行文件或库
```

### 2.2 Basic CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

project(HelloProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hello main.cpp)
```

各条指令的作用：

- `cmake_minimum_required`：指定所需的最低 CMake 版本；
- `project`：定义项目名称和使用的语言；
- `set`：设置变量，这里指定 C++17；
- `add_executable`：由源文件创建可执行目标。

### 2.3 Build Process

推荐使用外部构建，把生成文件放在独立的 `build` 目录：

```bash
cmake -S . -B build
cmake --build build
./build/hello
```

- `-S .`：源代码目录是当前目录；
- `-B build`：生成文件放入 `build`；
- `cmake --build build`：使用生成的构建系统进行编译。

这样可以避免中间文件污染源代码目录。需要完全重新配置时，可以删除 `build` 后重新生成。

### 2.4 Libraries and Dependencies

创建静态库：

```cmake
add_library(math_lib STATIC math.cpp)
```

将库链接到程序：

```cmake
target_link_libraries(hello PRIVATE math_lib)
```

设置头文件搜索路径：

```cmake
target_include_directories(math_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
```

可见性关键字的含义：

| Keyword | 当前目标使用 | 依赖当前目标的其他目标使用 |
| --- | --- | --- |
| `PRIVATE` | 是 | 否 |
| `PUBLIC` | 是 | 是 |
| `INTERFACE` | 否 | 是 |


## 3. Catkin

### 3.1介绍

Catkin 是 ROS1 使用的、基于 CMake 的构建系统。官方定义中，Catkin 是一组 CMake 宏和相关 Python 代码，用于构建 ROS 包。

Catkin 并没有替代 CMake，而是在 CMake 基础上增加了：

- ROS package 的组织方式；
- package 依赖管理；
- 消息和服务代码生成；
- 工作空间管理；
- ROS 环境配置文件生成。

### 3.2 Catkin Package Structure

一个常见的 Catkin package 包含：

```text
my_package/
├── CMakeLists.txt
├── package.xml
├── include/
└── src/
```

- `package.xml`：描述包的名称、版本、维护者和依赖；
- `CMakeLists.txt`：定义源文件、目标、依赖和构建规则；
- `src/`：源代码；
- `include/`：头文件。

### 3.3 Catkin 配置

```cmake
cmake_minimum_required(VERSION 3.0.2)
project(my_package)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
)

catkin_package()

include_directories(
    ${catkin_INCLUDE_DIRS}
)

add_executable(my_node src/my_node.cpp)

target_link_libraries(my_node
    ${catkin_LIBRARIES}
)
```

其中：

- `find_package(catkin ...)`：查找 Catkin 和 ROS 组件；
- `catkin_package()`：声明当前项目是 Catkin package；
- `${catkin_INCLUDE_DIRS}`：ROS 依赖提供的头文件路径；
- `${catkin_LIBRARIES}`：需要链接的 ROS 库。

### 3.4 Catkin Workspace

典型 ROS1 工作空间：

```text
catkin_ws/
├── src/
├── build/
└── devel/
```

基本构建流程：

```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

- `catkin_make`：配置并构建工作空间中的 Catkin packages；
- `build/`：保存构建过程产生的中间文件；
- `devel/`：保存开发环境下生成的程序、库及环境配置；
- `source devel/setup.bash`：让当前 Shell 能找到新构建的 ROS package。

> Catkin 属于 ROS1。ROS2 通常使用 `ament` 和 `colcon build`。如果电脑安装的是 ROS2 Humble，不应直接把 ROS1 的 `catkin_make` 当作 ROS2 构建命令。


## 4. Agent使用

基本工作过程是：

1. 用户提出任务；
2. LLM 根据任务决定是否调用工具；
3. 本地程序执行工具；
4. 工具执行结果返回给 LLM；
5. LLM 根据新信息继续推理，直到完成任务。

```text
用户请求
  ↓
LLM 选择工具
  ↓
程序执行工具
  ↓
结果返回 LLM
  ↓
继续推理或输出答案
```

最小的编码代理可以只提供三个工具：

- `read_file`：读取文件；
- `list_files`：查看目录；
- `edit_file`：创建或修改文件。

LLM 本身并不会直接操作文件系统，它只产生结构化的工具调用请求；真正执行读写操作的是外部程序。因此，工具权限、参数检查、错误处理和用户确认仍然十分重要。

生产环境中的代理还会增加命令执行、文本搜索、网络搜索、上下文管理以及危险操作确认等能力。

## 6. Agent学习的个人见解：
使用AI学习时应首先明确当前工作的目的，学习时遇到当前不用马上掌握且阻碍当前进度的内容时可使ai先跨过这一问题。但如果社涉及到本次工作需要掌握的内容时应有限自己思考，尽量自己完成，遇到困难和错误可询问ai原因和方法，但不用ai一步到位帮忙完成。让ai提供帮助时详细描述当前进度，可询问ai完成当前任务需要哪些硬相关知识，哪些可以暂时忽略，可加快学习进度明确思路。过程中尝试ai引导完成，一步一个提示的操作，完成一阶段让ai检查，即时改正错误。完成任务后可让ai结合前面的问题提出检测问题，防止有关键步骤不理解。


