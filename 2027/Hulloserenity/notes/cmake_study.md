# CMake 与 Catkin 学习笔记

## 0. 总览

一个 C++ 项目从源码到可执行文件，通常会经过下面几层工具：

| 层次 | 作用 | 文件或命令 |
| --- | --- | --- |
| 编译器 | 完成预处理、编译、汇编和链接 | `g++` |
| Make | 根据依赖关系执行编译命令，避免无意义的重复编译 | `Makefile`、`make` |
| CMake | 生成 Makefile 等构建文件，让项目更容易跨平台构建 | `CMakeLists.txt`、`cmake` |
| Catkin | ROS 1 的构建工具，基于 CMake 管理 ROS 工作区和功能包 | `catkin_make`、`catkin_create_pkg` |

---

## 1. C++ 从源码到可执行文件

以 `main.cpp` 为例，完整过程可以分成四个阶段：

| 阶段 | 主要工作 | 输出 | 示例命令 |
| --- | --- | --- | --- |
| 预处理 | 展开头文件和宏，处理条件编译，删除注释 | 预处理后的源码 `.i` | `g++ -E main.cpp -o main.i` |
| 编译 | 将预处理后的 C++ 代码转换为汇编代码 | 汇编文件 `.s` | `g++ -S main.i -o main.s` |
| 汇编 | 将汇编代码转换为机器指令 | 目标文件 `.o` | `g++ -c main.s -o main.o` |
| 链接 | 合并目标文件与所需的库，解析函数和变量引用 | 可执行文件 | `g++ main.o add.o -o main` |

### 分步执行

```bash
g++ -E main.cpp -o main.i
g++ -S main.i -o main.s
g++ -c main.s -o main.o
g++ main.o add.o -o main
./main
```

各选项的含义：

- `-E`：只进行预处理。
- `-S`：编译到汇编代码为止。
- `-c`：编译并汇编为目标文件，但暂不链接。
- `-o`：指定输出文件名。

---

## 2. Makefile 与 make

Make 是构建自动化工具，它根据 `Makefile` 中描述的目标、依赖和命令来执行构建：

```bash
make
```

Make 会根据依赖关系和文件修改时间进行增量构建。例如某个 `.cpp` 文件发生变化后，通常只需要重新编译它，再重新链接，而不是重新编译整个项目。

> 实际项目中不需要手动编写 Makefile 里复杂的依赖关系，用 CMake 根据 `CMakeLists.txt` 自动生成即可。

---

## 3. CMake

### 3.1 CMake 是什么

CMake 是一个跨平台的构建系统生成器。它的作用是读取 `CMakeLists.txt` 并生成适合当前平台的构建文件，例如 Makefile。

`CMakeLists.txt` 是 CMake 项目的核心配置文件。

一个典型项目结构如下：

```text
project/
├── CMakeLists.txt
├── include/
│   └── demo.hpp
└── src/
    └── main.cpp
```

### 3.2 基本语法

#### 指定最低 CMake 版本

```cmake
cmake_minimum_required(VERSION 3.16)
```

这个命令应放在 `CMakeLists.txt` 的最开头，且必须位于 `project()` 等其他命令之前。版本号应根据项目实际需要填写。

#### 定义项目名称和语言

```cmake
project(demo LANGUAGES CXX)
```

- `demo`：项目名称。
- `LANGUAGES CXX`：启用 C++。省略 `LANGUAGES` 时，CMake 默认启用 C 和 C++。

#### 添加可执行目标

```cmake
add_executable(demo src/main.cpp)
```

这里的 `demo` 是 **CMake 目标名称**，也是默认生成的可执行文件名。源文件可以有多个：

```cmake
add_executable(demo
    src/main.cpp
    src/A.cpp
    src/B.cpp
)
```

#### 指定头文件目录

```cmake
target_include_directories(demo PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
```

`${CMAKE_CURRENT_SOURCE_DIR}` 表示当前 `CMakeLists.txt` 所在目录的绝对路径。

### 3.3 PUBLIC / PRIVATE / INTERFACE 的含义

`target_include_directories()` 的关键字优势在于可以通过可见性关键字精确控制头文件路径的传播。

| 关键字 | 当前目标可用 | 依赖此目标的其他目标可用 | 典型场景 |
| --- | --- | --- | --- |
| `PRIVATE` | 是 | 否 | 仅自身实现需要的内部头文件 |
| `PUBLIC` | 是 | 是 | 库的公开 API 头文件，自身和消费者都需要 |
| `INTERFACE` | 否 | 是 | 仅头文件的库（Header-only library） |

**实例**

```cmake
# 场景: MyLib 的头文件在 include/ 目录下
# MyLib 自身需要 include/，使用 MyLib 的 MyApp 也需要 include/

# PUBLIC: 自身和消费者都能访问该路径
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# PRIVATE: 只有 MyApp 自己能访问（内部工具函数头文件）
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/src/internal)

# INTERFACE: MyLib 是纯头文件库，自身无需编译，只传递路径给消费者
add_library(HeaderOnlyLib INTERFACE)
target_include_directories(HeaderOnlyLib INTERFACE ${PROJECT_SOURCE_DIR}/header_only)
```

### 3.4 自动收集源文件

可以使用 `aux_source_directory` 收集某个目录下的源文件：

```cmake
aux_source_directory(
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    SOURCES
)

add_executable(demo ${SOURCES})
```

这段代码的作用是：

1. 搜索 `src/` 目录中的源文件；
2. 将文件列表保存到变量 `SOURCES`；
3. 使用 `${SOURCES}` 将文件列表传给 `add_executable`。

注意：

- `aux_source_directory` 只搜索指定目录，不会自动递归搜索子目录。
- 如果在初次配置后又新增了源文件，可能需要重新运行 CMake 配置。

---

## 4. Catkin

### 4.1 Catkin 是什么

Catkin 是 ROS 1 的构建系统，建立在 CMake 之上，用于管理 ROS 工作区、功能包及其依赖。

### 4.2 创建工作区

以 Ubuntu 下的 ROS Noetic 为例，先加载 ROS 环境：

```bash
source /opt/ros/noetic/setup.bash
```

然后创建工作区：

```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws
catkin_make
```

此时目录结构大致为：

```text
catkin_ws/
├── src/       # 源代码和 ROS 功能包
├── build/     # 构建过程中的中间文件
├── devel/     # 开发空间中的生成结果
└── logs/      # 构建日志
```

> 注意：`catkin_make` 应在**工作区根目录** `~/catkin_ws` 中运行，而不是在 `src/` 中。

### 4.3 加载并验证环境

当前终端要使用刚构建好的工作区，需要执行：

```bash
source ~/catkin_ws/devel/setup.bash
```

验证 ROS 是否能找到功能包：

```bash
echo "$ROS_PACKAGE_PATH"
```

输出的路径中应包含当前工作区的相关目录。

### 4.4 创建 ROS 功能包

进入工作区的 `src/` 目录：

```bash
cd ~/catkin_ws/src
```

创建功能包，并声明常用依赖：

```bash
catkin_create_pkg my_pkg roscpp rospy std_msgs
```

其中：

- `my_pkg`：功能包名称；
- `roscpp`：ROS 的 C++ 客户端库；
- `rospy`：ROS 的 Python 客户端库；
- `std_msgs`：标准消息类型。

功能包名后面的参数都是新功能包的依赖，会自动写入 `package.xml` 和 `CMakeLists.txt`，实际按项目需要增减即可。

创建完成后的目录如下：

```text
my_pkg/
├── CMakeLists.txt        # 编译规则配置文件
├── package.xml           # 功能包清单文件
├── include/
│   └── my_pkg/           # C++ 头文件存放目录（初始为空）
└── src/                  # C++ 源文件存放目录（初始为空）
```

### 4.5 构建功能包

无论功能包在哪里，都回到工作区根目录构建：

```bash
cd ~/catkin_ws
catkin_make
source ~/catkin_ws/devel/setup.bash
```

可以用下面的命令验证功能包是否已进入 ROS 环境：

```bash
rospack find my_pkg
```

如果能够输出功能包的绝对路径，说明环境配置基本正确。

---

## 5. 命令速查

### CMake

```bash
cmake -S . -B build       # 配置项目
cmake --build build       # 构建项目
./build/demo              # 运行示例程序
```

### Catkin

初始化工作区：

```bash
source /opt/ros/noetic/setup.bash   # 加载 ROS 环境
mkdir -p ~/catkin_ws/src            # 创建工作区
cd ~/catkin_ws                      # 进入工作区根目录
catkin_make                         # 构建工作区
source devel/setup.bash             # 加载当前工作区
```

创建功能包：

```bash
cd ~/catkin_ws/src                             # 进入源码目录
catkin_create_pkg my_pkg roscpp rospy std_msgs # 创建功能包
```

构建与验证：

```bash
cd ~/catkin_ws                       # 回到工作区根目录
catkin_make                          # 构建功能包
source devel/setup.bash              # 加载当前工作区
echo "$ROS_PACKAGE_PATH"             # 检查 ROS 包路径
rospack find my_pkg                  # 验证功能包是否可找到
```
