# CMake 与 catkin 学习笔记

> 课程：机器人队通识第二课 —— CMake 与 catkin 基础、大模型使用入门
> 主题：从源码到可执行文件、CMake 构建、catkin 工作空间，以及如何用大模型辅助学习。

## 目录

1. [学习资料与本地环境](#1-学习资料与本地环境)
2. [为什么需要构建系统](#2-为什么需要构建系统)
3. [Makefile 速览](#3-makefile-速览)
4. [CMake：跨平台的构建系统生成器](#4-cmake跨平台的构建系统生成器)
5. [catkin：ROS 1 的构建系统](#5-catkinros-1-的构建系统)
6. [补充资料笔记](#6-补充资料笔记)
7. [用大模型辅助学习的实践与思考](#7-用大模型辅助学习的实践与思考)
8. [小结](#8-小结)

---

## 1. 学习资料与本地环境

### 1.1 资料来源

| 资料 | 链接 | 阅读情况 |
| --- | --- | --- |
| 课程 PPT《第二课 cmake 与 catkin 基础》 | 课程群文件 | 已读，并用脚本提取了全部 18 页文字 |
| 菜鸟教程：CMake 基础 | <https://www.runoob.com/cmake/cmake-basic.html> | 已读 |
| ROS Wiki：catkin/Tutorials | <https://wiki.ros.org/catkin/Tutorials> | 已读，并继续读了 create_a_workspace、CreatingPackage、using_a_workspace 三篇子教程 |
| The Emperor Has No Clothes | <https://www.mihaileric.com/The-Emperor-Has-No-Clothes/> | 已读 |
| 知乎：p/476697014 | <https://zhuanlan.zhihu.com/p/476697014> | **未能读取**：直接访问返回 403，见 [6.4](#64-知乎文章未能访问的一篇) |

### 1.2 本地实验环境

为了不只是在纸面上抄命令，我在本机实际编译了一个最小 CMake 工程。

```text
OS      : Ubuntu (Linux 6.x)
Compiler: g++ (Ubuntu 15.2.0)
CMake   : 4.2.3（从 Ubuntu 软件包中解压到本地目录后运行）
catkin  : 未安装（本机没有 ROS 环境，catkin 部分依据官方文档整理，未实机验证）
```

> 说明：系统里原本没有 `cmake`，也没有 sudo 权限，于是用 `apt-get download cmake` 下载 deb 包，再用 `dpkg-deb -x` 解压到 `/tmp/cmake-pkg`，补上 `librhash1`、`libjsoncpp26` 和 `cmake-data` 后即可运行。这个过程本身也让我理解了 CMake 并不是一个孤立的二进制，它依赖数据模块和系统库。

---

## 2. 为什么需要构建系统

### 2.1 从源代码到可执行程序

计算机只能执行二进制机器码，而我们写的是 C/C++ 这样的高级语言。中间要经过四步：

1. **预处理（Preprocessing）**：展开宏、处理 `#include`、删除注释，生成 `.i` 文件。
2. **编译（Compilation）**：把预处理后的代码翻译成汇编代码，生成 `.s` 文件。
3. **汇编（Assembly）**：把汇编代码翻译成机器指令，生成 `.o` 目标文件。
4. **链接（Linking）**：把多个 `.o` 文件和库按符号解析、重定位，合并成可执行文件。

对一个文件可以直接：

```bash
g++ -std=c++17 -O2 main.cpp -o app
```

但工程一旦有几十个源文件、多个目录、第三方库，手写编译命令就会变得又长又容易错，而且每次改一个文件都全量重编，效率很低。构建系统就是来解决“**编译哪些文件、按什么顺序、用什么参数、哪些可以跳过**”这些问题的。

### 2.2 三代工具的演进

| 阶段 | 工具 | 解决的问题 | 遗留的问题 |
| --- | --- | --- | --- |
| 手写命令 | `gcc` / `g++` | 能编译 | 无法管理大型工程，重复劳动 |
| 规则化 | `make` + `Makefile` | 依赖关系、增量编译、一条命令构建 | 平台相关、语法晦涩、跨平台要重写 |
| 元构建 | `CMake` | 用一份配置生成各平台构建文件 | 需要学习自己的语言和变量模型 |
| 领域扩展 | `catkin` | 在 CMake 上加入 ROS 包管理、依赖解析、环境设置 | 与 ROS 1 生态绑定 |

一句话总结：

> **Makefile 是“怎么编译”的说明书，CMake 是“生成这份说明书”的工厂，catkin 是“面向 ROS 工程的 CMake 发行版 + 包管理器”。**

---

## 3. Makefile 速览

Makefile 的核心是规则：

```make
目标: 依赖1 依赖2 ...
<TAB>命令
```

一个最小例子：

```make
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -O2
TARGET   := app
OBJS     := main.o utils.o

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
```

几个关键点：

- **增量构建**：`make` 比较目标与依赖的修改时间，只重建过期的目标。
- **变量与自动变量**：`$@` 是目标，`$<` 是第一个依赖，`$^` 是所有依赖。
- **`.PHONY`**：声明伪目标，避免目录里恰好有同名文件时规则失效。
- **缩进必须是 Tab**，这是初学者最常踩的坑之一。
- Makefile 本身没有包管理，第三方库路径、平台差异都要自己处理。

在 Linux 内核、嵌入式项目里 Makefile 仍然很常见，所以理解它有助于看懂 CMake 生成的 `build/` 目录。

---

## 4. CMake：跨平台的构建系统生成器

### 4.1 CMake 到底做了什么

CMake **不直接编译代码**，它读取 `CMakeLists.txt`，完成：

```text
配置（configure） → 生成（generate） → 构建（build） → 安装（install）
```

在 Unix 上通常生成 Makefile（也可以生成 Ninja、Visual Studio、Xcode 工程）。真正调用编译器的是 `make`/`ninja`/IDE。

CMake 的两阶段工作方式：

1. **配置阶段**：检查编译器、系统、依赖，计算变量，写入 `CMakeCache.txt` 和内部文件。
2. **生成阶段**：根据配置生成具体构建系统的文件。
3. 之后的 `cmake --build` 只是调用底层构建工具，增量编译由底层工具负责。

这种设计带来两个好处：

- **跨平台**：同一份 `CMakeLists.txt` 在 Linux/Windows/macOS 都能生成对应工程。
- **可组合**：库、可执行文件、子目录可以声明依赖关系，而不是手写路径。

### 4.2 一个可运行的最小工程

目录结构：

```text
cmake-demo/
├── CMakeLists.txt
├── include/
│   └── hello.h
└── src/
    ├── hello.cpp
    └── main.cpp
```

`CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.16)
project(CmakeDemo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 静态库 hello
add_library(hello STATIC src/hello.cpp)
target_include_directories(hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 可执行文件 demo，链接 hello
add_executable(demo src/main.cpp)
target_link_libraries(demo PRIVATE hello)
```

构建与运行：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/demo
```

我在本机得到的实际输出（节选）：

```text
-- The CXX compiler identification is GNU 15.2.0
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features - done
-- Configuring done (0.1s)
-- Generating done (0.0s)
-- Build files have been written to: /tmp/cmake-demo/build

[ 25%] Building CXX object CMakeFiles/hello.dir/src/hello.cpp.o
[ 50%] Linking CXX static library libhello.a
[ 50%] Built target hello
[ 75%] Building CXX object CMakeFiles/demo.dir/src/main.cpp.o
[100%] Linking CXX executable demo
[100%] Built target demo

Hello, CMake!
```

几个观察：

- `-S . -B build` 是**外部构建**，所有中间产物都在 `build/`，源码目录保持干净。
- `target_link_libraries(demo PRIVATE hello)` 只需要写目标名，CMake 自动处理库文件路径和链接顺序。
- `hello` 的 `PUBLIC` include 路径会自动传递给 `demo`，`demo` 不需要再写 `include/` 的路径。
- `CMAKE_BUILD_TYPE=Release` 会使用 `-O3 -DNDEBUG`（具体 flags 因编译器而异），Debug 则带 `-g`。

### 4.3 常用指令

| 指令 | 作用 | 备注 |
| --- | --- | --- |
| `cmake_minimum_required(VERSION x.y)` | 声明最低 CMake 版本 | 必须放在最前面 |
| `project(Name VERSION 1.0 LANGUAGES CXX)` | 定义项目 | 会设置 `PROJECT_NAME`、`PROJECT_SOURCE_DIR` 等变量 |
| `add_executable(target src...)` | 生成可执行文件 | 源文件可以直接列，也可以用变量 |
| `add_library(target STATIC/SHARED src...)` | 生成库 | 不写类型时由 `BUILD_SHARED_LIBS` 决定 |
| `target_link_libraries(target PRIVATE lib...)` | 链接库 | 优先写目标名，而不是绝对路径 |
| `target_include_directories(target PUBLIC dir)` | 头文件搜索路径 | 可控制传递性 |
| `target_compile_options(target PRIVATE -Wall)` | 编译选项 | 只影响该目标 |
| `target_compile_features(target PUBLIC cxx_std_17)` | 声明语言特性 | 比全局设置更现代 |
| `set(VAR value...)` | 定义变量 | 用 `${VAR}` 引用 |
| `option(OPT "desc" ON)` | 布尔缓存变量 | 可用 `-DOPT=OFF` 覆盖 |
| `find_package(Pkg REQUIRED)` | 查找外部包 | 优先使用导入目标 |
| `add_subdirectory(dir)` | 加入子目录 | 适合多模块工程 |
| `install(TARGETS ...)` | 安装规则 | 配合 `cmake --install` |
| `message(STATUS "...")` | 打印信息 | 调试配置阶段很有用 |

### 4.4 target-based 与 PUBLIC / PRIVATE / INTERFACE

现代 CMake 的核心思想是：**一切围绕 target**。不要用全局的 `include_directories()`、`link_directories()` 污染所有目标，而要把属性绑定到具体目标上。

三个可见性关键字可以这样记：

| 关键字 | 当前目标使用 | 依赖它的目标使用 | 典型场景 |
| --- | --- | --- | --- |
| `PRIVATE` | ✅ | ❌ | 只有自己实现需要的头文件、编译选项 |
| `PUBLIC` | ✅ | ✅ | 库的公开 API 头文件 |
| `INTERFACE` | ❌ | ✅ | 纯头文件库（header-only） |

例子：

```cmake
# 库的公开头文件：自己和消费者都要用
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 可执行文件内部用的头文件：只给自己用
target_include_directories(myapp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/internal)

# header-only 库：自己不编译，只把路径传给消费者
add_library(header_only INTERFACE)
target_include_directories(header_only INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
```

如果给库用了 `PRIVATE` 却把公开头文件放进去，消费者编译时就会找不到头文件；反过来给内部实现用了 `PUBLIC`，又会把实现细节泄露给所有下游。这是 CMake 最常见的坑之一。

### 4.5 变量、缓存与构建类型

CMake 变量分两类：

- **普通变量**：每次配置重新计算，作用域是当前目录及子目录。
- **缓存变量**：写入 `CMakeCache.txt`，跨多次配置保留，适合用户可配置的开关和路径。

```cmake
set(MY_INSTALL_PATH "/usr/local" CACHE PATH "安装路径")
option(ENABLE_LOGGING "是否启用日志" ON)

if(ENABLE_LOGGING)
    target_compile_definitions(myapp PRIVATE ENABLE_LOGGING)
endif()
```

命令行覆盖：

```bash
cmake -S . -B build -DMY_INSTALL_PATH=/opt/demo -DENABLE_LOGGING=OFF
```

一个容易忽略的点：**修改 `set(... CACHE ...)` 的默认值不会自动更新已有的缓存**，必须删掉 `CMakeCache.txt` 或整个 `build/` 重新配置。

常见构建类型：

| 类型 | 典型 flags | 用途 |
| --- | --- | --- |
| `Debug` | `-g` | 调试，保留符号 |
| `Release` | `-O3 -DNDEBUG` | 发布，优化 |
| `RelWithDebInfo` | `-O2 -g` | 既要优化又要调试 |
| `MinSizeRel` | `-Os` | 最小体积 |

只对单配置生成器（Makefile、Ninja）有意义；Visual Studio 是多配置生成器，在构建时选择配置。

### 4.6 查找外部依赖

```cmake
find_package(OpenCV REQUIRED)
find_package(Boost 1.70 REQUIRED)

target_link_libraries(myapp PRIVATE OpenCV::OpenCV Boost::Boost)
```

`find_package` 成功后通常提供两类东西：

- **导入目标**：`OpenCV::OpenCV`、`Boost::Boost`，现代 CMake 推荐，自动携带 include 路径、库路径和编译选项。
- **旧式变量**：`${OpenCV_INCLUDE_DIRS}`、`${OpenCV_LIBS}`，维护旧项目时才用。

`find_package` 有两种模式：

- **Config 模式**：包自己提供 `<Package>Config.cmake` 或 `<package>-config.cmake`，最可靠。
- **Module 模式**：CMake 自带 `Find<Package>.cmake`，靠查找常见路径。

### 4.7 安装

```cmake
include(GNUInstallDirs)

install(TARGETS myapp
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS mylib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
```

```bash
cmake --install build --prefix /tmp/demo-install
```

安装规则把“构建产物”和“最终部署布局”分开，打包和交叉编译时很有用。

### 4.8 常见错误与最佳实践

| 现象 | 常见原因 | 处理 |
| --- | --- | --- |
| 找不到头文件 | 只写了 `PRIVATE`，或路径写错 | 检查 `target_include_directories`，确认路径基于 `CMAKE_CURRENT_SOURCE_DIR` |
| 链接失败 `undefined reference` | 库顺序不对 / 没链接依赖 | 用目标名链接，CMake 会处理顺序；检查 `target_link_libraries` |
| 改了 CMakeLists 不生效 | 缓存未更新 | 删除 `CMakeCache.txt` 或重新配置 |
| 在源码目录生成一堆文件 | 用了内部构建 | 改用 `cmake -S . -B build` |
| 全局 flags 互相污染 | 大量使用 `include_directories`/`add_definitions` | 改用 `target_*` 系列 |
| Release/Debug 混用 | 多配置生成器直接 `make` | 指定 `--config Release` 或使用单配置生成器 |

最佳实践：

1. **始终外部构建**，`build/` 加进 `.gitignore`。
2. **优先 target-based 指令**，少用全局命令。
3. **显式声明 C++ 标准**：`CMAKE_CXX_STANDARD` + `CMAKE_CXX_STANDARD_REQUIRED`，或用 `target_compile_features`。
4. **不要硬编码绝对路径**，用 `CMAKE_CURRENT_SOURCE_DIR`、`PROJECT_SOURCE_DIR`、`GNUInstallDirs`。
5. **库的公开接口用 `PUBLIC`，内部实现用 `PRIVATE`**。
6. **提交前在干净目录重新配置一次**，避免“我本地能编译”。

---

## 5. catkin：ROS 1 的构建系统

### 5.1 catkin 是什么

catkin 是 ROS 1 的官方构建系统，它**基于 CMake 扩展**而来。可以把它理解为：

```text
catkin = CMake + ROS 包规范（package.xml） + 依赖解析 + 工作空间环境（setup.bash）
```

它解决的问题不只是“编译一个 C++ 程序”，而是：

- 一个工作空间里有很多 ROS 包，包之间互相依赖；
- 包需要声明对 `roscpp`、`rospy`、`std_msgs` 等依赖；
- 编译产物要能被 `rosrun`、`roslaunch` 找到；
- 多个工作空间可以**叠加（overlay）**，开发时覆盖系统安装的版本。

### 5.2 工作空间布局

标准 catkin 工作空间：

```text
catkin_ws/
├── src/                 # 源码空间：每个包一个目录
│   ├── CMakeLists.txt   # catkin 提供的顶层 CMake 文件（catkin_make 首次运行生成）
│   └── my_pkg/
│       ├── CMakeLists.txt
│       ├── package.xml
│       ├── include/
│       ├── src/
│       └── launch/
├── build/               # 构建空间：CMake 缓存、中间文件
├── devel/               # 开发空间：编译出的库、可执行文件、setup.bash
└── install/             # 安装空间（执行 install 后才有）
```

三个空间的区别：

| 空间 | 内容 | 是否提交到 Git |
| --- | --- | --- |
| `src/` | 源码、`package.xml`、`CMakeLists.txt` | ✅ 提交 |
| `build/` | CMakeCache、Makefile、`.o` 等中间文件 | ❌ 忽略 |
| `devel/` | 开发用的可执行文件、库、环境脚本 | ❌ 忽略 |
| `install/` | `make install` 后的最终布局 | ❌ 忽略 |

catkin 的一个关键设计是：**整个工作空间当成一个大 CMake 工程来配置**，但每个包仍然保留自己的 `CMakeLists.txt`。这解决了包间依赖和构建顺序问题。

### 5.3 基本工作流

以下命令来自 ROS Wiki（本机没有 ROS，未实际执行）：

```bash
# 1. 创建并初始化工作空间
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws
catkin_make

# 2. 让当前 shell 找到这个工作空间
source devel/setup.bash

# 3. 创建功能包，并声明依赖
cd ~/catkin_ws/src
catkin_create_pkg my_pkg std_msgs rospy roscpp

# 4. 回到工作空间根目录编译
cd ~/catkin_ws
catkin_make

# 5. 再次 source，确保新包的可执行文件在环境中
source devel/setup.bash
```

`catkin_make` 第一次运行时会：

- 在 `src/` 下创建指向 `/opt/ros/$ROS_DISTRO/share/catkin/cmake/toplevel.cmake` 的 `CMakeLists.txt` 链接；
- 创建 `build/` 和 `devel/`；
- 生成 `devel/setup.bash`、`setup.zsh` 等环境脚本。

验证环境：

```bash
echo $ROS_PACKAGE_PATH
# 应该包含 ~/catkin_ws/src
```

### 5.4 package.xml 与 CMakeLists.txt

一个 catkin 包必须满足：

1. 有符合 catkin 规范的 `package.xml`；
2. 有使用 catkin 的 `CMakeLists.txt`；
3. 每个包独占一个目录，不能嵌套包。

`package.xml` 的核心字段：

```xml
<?xml version="1.0"?>
<package format="2">
  <name>my_pkg</name>
  <version>0.1.0</version>
  <description>My first catkin package</description>

  <maintainer email="you@example.com">Your Name</maintainer>
  <license>BSD</license>

  <buildtool_depend>catkin</buildtool_depend>
  <build_depend>roscpp</build_depend>
  <build_depend>rospy</build_depend>
  <build_depend>std_msgs</build_depend>

  <exec_depend>roscpp</exec_depend>
  <exec_depend>rospy</exec_depend>
  <exec_depend>std_msgs</exec_depend>
</package>
```

依赖标签的含义：

| 标签 | 何时需要 |
| --- | --- |
| `buildtool_depend` | 构建工具本身，通常只有 `catkin` |
| `build_depend` | 编译期需要的包 |
| `exec_depend` | 运行期需要的包 |
| `test_depend` | 仅测试需要 |
| `depend` | 构建 + 运行 + 测试（format 2 的简写） |

对应的 `CMakeLists.txt` 关键片段：

```cmake
cmake_minimum_required(VERSION 3.0.2)
project(my_pkg)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs
  INCLUDE_DIRS include
  LIBRARIES my_pkg
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(my_pkg src/my_pkg.cpp)
target_link_libraries(my_pkg ${catkin_LIBRARIES})

add_executable(my_node src/my_node.cpp)
target_link_libraries(my_node ${catkin_LIBRARIES})
```

和纯 CMake 相比，多了三件事：

- `find_package(catkin REQUIRED COMPONENTS ...)`：让 catkin 找到 ROS 包；
- `catkin_package(...)`：声明这个包对外导出什么（头文件、库、依赖）；
- `${catkin_INCLUDE_DIRS}`、`${catkin_LIBRARIES}`：使用 catkin 汇总好的路径和库。

### 5.5 devel 与 install

catkin 的 `devel` 空间通过**符号链接和路径重定向**，把源码目录里的 Python 文件、头文件直接“映射”到开发空间。好处是：

- 改 Python 代码不需要重新 `make install`；
- 改头文件后重新编译即可；
- 开发迭代速度快。

`install` 空间则是最终部署布局，类似 `/opt/ros/...`，适合分发。

> 注意：devel 和 install 不要同时 source，容易造成路径冲突。开发时用 `devel/setup.bash`，部署时用 `install/setup.bash`。

### 5.6 catkin_make 的局限与替代工具

`catkin_make` 是包装脚本，本质上是在 `build/` 里调用 `cmake ../src` + `make`。它的局限：

- 所有包在同一个 CMake 进程里配置，包之间环境隔离不彻底；
- 某些测试依赖生成代码的 `PYTHONPATH` 时可能失败；
- 不支持并行构建多个独立包的高级调度。

替代方案：

| 工具 | 特点 |
| --- | --- |
| `catkin_make` | ROS 1 最常用，简单直接 |
| `catkin_make_isolated` | 每个包单独配置/构建，隔离性好 |
| `catkin build`（catkin_tools） | 更现代的调度、并行、日志，需额外安装 |
| `colcon` | ROS 2 的构建工具，也支持部分 ROS 1 包 |

### 5.7 catkin 与 CMake 的关系

用一张表理清：

| 维度 | CMake | catkin |
| --- | --- | --- |
| 定位 | 通用构建系统生成器 | ROS 1 的构建系统 |
| 配置文件 | `CMakeLists.txt` | `CMakeLists.txt` + `package.xml` |
| 依赖管理 | `find_package` | `find_package(catkin COMPONENTS ...)` + `package.xml` |
| 工作空间 | 自由布局 | 固定 `src/build/devel/install` |
| 环境设置 | 无 | `setup.bash` 注入 `ROS_PACKAGE_PATH`、`LD_LIBRARY_PATH` 等 |
| 构建命令 | `cmake` + `make` | `catkin_make` / `catkin build` |

所以学 catkin 不能跳过 CMake：catkin 的 `CMakeLists.txt` 仍然是 CMake 语法，`catkin_package()` 只是 CMake 宏/函数。把 CMake 的 target、变量、`find_package` 搞清楚，catkin 就只剩下“包规范 + 环境脚本”这两层。

### 5.8 catkin 常见坑

1. **忘记 source**：新开终端后必须 `source devel/setup.bash`，否则找不到包。
2. **在错误目录运行 catkin_make**：必须在工作空间根目录（有 `src/` 的那层）。
3. **包嵌套**：`src/pkg1/pkg2` 这种结构会让 catkin 找不到 `pkg2`。
4. **package.xml 缺依赖**：编译能过但运行时报找不到消息/库。
5. **混用 devel 和 install**：同时 source 可能导致库版本冲突。
6. **把 build/devel 提交进 Git**：体积大且机器相关，应写进 `.gitignore`。

---

## 6. 补充资料笔记

### 6.1 菜鸟教程：CMake 基础

这篇教程把 CMake 的核心指令过了一遍，我的收获主要有四点：

1. **`CMakeLists.txt` 是声明式的**：描述“有什么目标、依赖什么”，而不是写编译命令。
2. **`target_*` 比全局指令更好**：`target_include_directories` 能精确控制 `PUBLIC/PRIVATE/INTERFACE`，避免路径污染。
3. **缓存变量不会自动更新默认值**：这是实际调试时很容易遇到的问题。
4. **`find_package` 优先用导入目标**（如 `Boost::Boost`），不要手动拼 `${Boost_INCLUDE_DIRS}` 和 `${Boost_LIBRARIES}`。

教程里的完整示例和我在 [4.2](#42-一个可运行的最小工程) 写的 demo 结构一致：先 `add_library`，再 `add_executable`，最后 `target_link_libraries`。自己跑一遍之后，对“库的 `PUBLIC` 头文件会自动传递给消费者”这一点印象比只看文字深很多。

### 6.2 ROS Wiki：catkin 教程

我读了总览和三个入门子教程，主线很清楚：

1. **create_a_workspace**：`mkdir -p ~/catkin_ws/src` → `catkin_make` → `source devel/setup.bash`。
2. **CreatingPackage**：`catkin_create_pkg my_pkg std_msgs rospy roscpp`，然后修改 `package.xml` 的 description、maintainer、license 和依赖。
3. **using_a_workspace**：`catkin_make` 构建、`catkin_make install` 安装、`catkin_make --force-cmake` 强制重新配置。

几个文档里值得记下的细节：

- `catkin_make` 首次运行会在 `src/` 生成顶层 `CMakeLists.txt` 链接。
- `rospack depends1 my_pkg` 查看直接依赖，`rospack depends my_pkg` 递归查看全部依赖。
- `build/` 里只有一个 `CMakeCache.txt`，整个工作空间共享一份配置。
- 可以用 `catkin_make -DCMAKE_BUILD_TYPE=Release` 指定构建类型。
- 文档特别提醒：**不要同时 source devel 和 install**。
- 安装前缀默认是 `/usr/local`，开发时建议用工作空间内的目录，方便卸载。

> ROS Wiki 顶部提示 ROS 1 Noetic 已于 2025 年 5 月 31 日 EOL，Wiki 将转为归档。这也说明为什么现在新项目更多使用 ROS 2 的 `ament` + `colcon`；但理解 catkin 仍然有助于读旧代码和维护现有机器人项目。

### 6.3 The Emperor Has No Clothes：200 行实现一个 Coding Agent

这篇文章的标题很吸引人：“皇帝没有穿衣服”。作者的核心观点是：**AI 编程助手看起来像魔法，但核心只是一个带工具箱的 LLM 对话循环**。

文章拆解的循环：

```text
用户输入
  → LLM 决定调用工具（返回结构化的 tool call）
  → 本地程序执行工具（读文件 / 列目录 / 改文件）
  → 工具结果回填给 LLM
  → LLM 继续推理，直到不需要工具
```

它只实现了三个工具：

| 工具 | 作用 |
| --- | --- |
| `read_file` | 读取文件内容 |
| `list_files` | 列出目录 |
| `edit_file` | 创建文件或用 `old_str` → `new_str` 替换 |

关键设计：

1. **工具描述就是提示词**：用函数的 docstring 和签名动态生成 system prompt，告诉模型“你有哪些工具、怎么调用”。
2. **工具调用格式很简单**：要求模型输出 `tool: TOOL_NAME({JSON_ARGS})`，程序用正则/字符串解析。
3. **对话历史就是状态**：每轮把 assistant 的回复和 `tool_result(...)` 追加到 messages，模型据此继续。
4. **外层循环处理用户输入，内层循环处理工具调用**，直到模型不再请求工具。

文章也指出生产级工具（Claude Code 等）多了错误处理、流式输出、上下文压缩、更多工具（bash、grep、web search）和危险操作审批，但**核心架构没有变**。

我的理解：

- Agent 的能力上限主要由 **模型 + 工具 + 上下文管理** 决定；
- 工具不在多，而在“描述清楚、返回结构化结果、错误可恢复”；
- 让 Agent 改代码时，**最小 diff、可验证、可回滚** 比“一次生成很多”更重要；
- 这也解释了为什么本课件的 Agent 能直接读写本地文件：它只是在调用文件工具，并不是真的“理解”了整个工程。

### 6.4 知乎文章（未能访问的一篇）

资料列表里的知乎链接：

<https://zhuanlan.zhihu.com/p/476697014>

我尝试了以下方式，均未能读到正文：

- 浏览器/命令行直接访问：返回 **403**，页面是知乎的反爬提示；
- 移动端 API：返回“请求参数异常”或“访问异常”；
- Web Archive / archive.today：没有可用快照或请求超时；
- 搜索引擎：只能看到其他博客对同一 URL 的零散引用，无法确认原文标题和完整内容。

所以这一节我**不编造内容**，只记录访问失败的事实。等后续能正常打开原文，再补充这一篇的笔记。这里也提醒自己：**遇到打不开的资料，应该如实标注，而不是让 AI“根据标题猜一篇”**——那正是大模型使用中最危险的行为之一。

---

## 7. 用大模型辅助学习的实践与思考

这一课特别提到可以用大模型帮助学习。我在这份笔记的整理过程中，确实把 AI 当成了“助教”，但尽量把它放在**可验证**的位置上。

### 7.1 我实际怎么用

| 环节 | AI 帮了什么 | 我怎么验证 |
| --- | --- | --- |
| 读 PPT | 用脚本提取 18 页幻灯片的文字，避免漏页 | 对照原始 PPT 页码检查 |
| 查文档 | 抓取菜鸟教程、ROS Wiki、Emperor 文章 | 打开原文核对关键命令 |
| 理解概念 | 解释 `PUBLIC/PRIVATE/INTERFACE`、catkin 三个空间的区别 | 用一个最小 CMake 工程实际编译 |
| 排查环境 | 系统没有 cmake 时，讨论如何本地解压 deb 包 | 实际运行 `cmake --version` 和构建 demo |
| 组织笔记 | 生成大纲、表格、代码块 | 自己调整结构和措辞，删除不确定的内容 |

### 7.2 有效的地方

- **降低检索成本**：不用在多个文档之间来回翻，能快速建立“概念地图”。
- **解释差异**：比如 `include_directories` 和 `target_include_directories` 的区别，用表格总结后更容易记。
- **生成可运行的最小例子**：让 AI 给出最小 `CMakeLists.txt`，再自己跑一遍，比看大段文档快。
- **检查遗漏**：让 AI 根据 PPT 目录反问我“catkin 的 install 空间有什么用”，能暴露自己没掌握的点。

### 7.3 需要警惕的地方

1. **版本差异**：AI 可能给旧版本 CMake 的写法，比如 `cmake_minimum_required(VERSION 2.8)`，或者混用新旧命令。
2. **命令未经验证**：catkin 相关命令我本机没有 ROS，无法执行，只能标注“来自文档，未实机验证”。
3. **自信地编造**：尤其是打不开的链接，模型可能“根据标题写一篇”，必须明确拒绝。
4. **安全风险**：不要把 token、密码、私有路径贴进对话；执行 `rm -rf`、`git reset --hard` 前要自己确认。
5. **理解幻觉**：能背出 `target_link_libraries` 的语法，不等于理解链接顺序和符号解析；构建系统的坑必须自己踩。

### 7.4 我的原则

> **AI 可以帮你查、帮你解释、帮你写样板，但不能替你建立心智模型。**

具体做法：

- 关键命令一定自己跑一遍，至少跑一个最小例子；
- 让 AI 给出处，再打开原文核对；
- 对“为什么”追问到底，不满足于“就是这么写”；
- 笔记用自己的话重写，保留自己的疑问和踩坑记录；
- 不确定的内容明确标注“未验证/待补充”，不装作懂。

---

## 8. 小结

| 主题 | 最需要记住的内容 |
| --- | --- |
| 编译流程 | 预处理 → 编译 → 汇编 → 链接；构建系统负责依赖和增量 |
| Makefile | 目标: 依赖 + Tab 命令；增量构建；`.PHONY` |
| CMake | 构建系统生成器；`cmake -S . -B build`；target-based；`PUBLIC/PRIVATE/INTERFACE` |
| CMake 缓存 | `CMakeCache.txt` 持久化；改默认值要清缓存；`-D` 覆盖 |
| catkin | ROS 1 的 CMake 扩展；`src/build/devel/install`；`package.xml` + `CMakeLists.txt` |
| catkin 工作流 | `catkin_make` → `source devel/setup.bash` → `catkin_create_pkg` |
| 大模型 | 加速检索和解释，但必须验证；不编造、不泄露、不盲信 |

下一步我打算：

1. 在能安装 ROS 的环境里实际跑一遍 `catkin_make` 和 `catkin_create_pkg`；
2. 给 demo 增加 `install` 规则和 `find_package` 示例；
3. 读一下 catkin 的 `toplevel.cmake`，理解它如何把多个包组织成一个 CMake 工程；
4. 补上知乎那篇资料的学习笔记。
