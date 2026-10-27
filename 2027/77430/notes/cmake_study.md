# 第二课笔记：CMake 与 catkin

> 课程仓库：<https://github.com/OceanCing/missing-semester>
> 整理人：`77430`　｜　更新日期：2026-10-27
> 本机环境：CMake 4.4.4（pip wheel 安装）、GCC 15.2.0、GNU Make 4.4、Ubuntu 24.04

## 目录

1. [写在前面：为什么要有构建系统](#1-写在前面为什么要有构建系统)
2. [CMake 基础](#2-cmake-基础)
3. [目标（target）与三个关键字](#3-目标target与三个关键字)
4. [变量、缓存与作用域](#4-变量缓存与作用域)
5. [安装、测试与配置](#5-安装测试与配置)
6. [一次真实的 CMake 实验](#6-一次真实的-cmake-实验)
7. [catkin：ROS 1 的构建系统](#7-catkinros-1-的构建系统)
8. [catkin 工作空间与一次 `catkin_make`](#8-catkin-工作空间与一次-catkin_make)
9. [补充资料学习：The Emperor Has No Clothes](#9-补充资料学习the-emperor-has-no-clothes)
10. [用大模型学构建系统：我的做法和边界](#10-用大模型学构建系统我的做法和边界)
11. [速查表](#11-速查表)
12. [参考资料](#12-参考资料)

---

## 1. 写在前面：为什么要有构建系统

一个 C/C++ 项目从源码到可执行文件，至少四步：**预处理 → 编译 → 汇编 → 链接**。
手工敲 `g++ main.cpp -o app` 在只有一个文件时没问题，一旦变成几十个文件就会立刻失控：

- 改一个 `.cpp` 不应该重编全部；
- 头文件改了，依赖它的 `.cpp` 要重编；
- 库的链接顺序、宏定义、编译选项、平台差异要能复现；
- 交付给别人时，对方机器上的编译器可能完全不同。

`make` 解决"增量构建 + 依赖关系"，但它把**编译器、路径、平台细节**都写死在 `Makefile` 里，换个平台就得重写。
于是有了**构建系统生成器（build system generator）**：CMake 不亲自编译，它读 `CMakeLists.txt`，生成当前平台能用的 `Makefile` 或 `build.ninja`，再交给 `make`/`ninja` 去执行。

所以 CMake 有**两段式**：

```
CMakeLists.txt ──cmake(配置/生成)──> Makefile / build.ninja ──make/ninja(构建)──> 目标文件
```

配置文件是**跨平台**的，生成的构建文件是**平台相关**的，这是整套设计的核心取舍。

---

## 2. CMake 基础

### 2.1 语法骨架

`CMakeLists.txt` 由一条条**命令**组成，形式统一为 `命令名(参数1 参数2 ...)`：

- 命令名**大小写不敏感**（`add_executable` 和 `ADD_EXECUTABLE` 等价），但**约定全小写**；
- 参数之间用空格或换行分隔，含空格的参数加引号；
- 用 `#` 注释，`#[[ ... ]]` 是多行注释；
- 变量引用写作 `${VAR}`，判断变量是否存在写作 `if(VAR)`。

一个能跑的最小项目只要四行：

```cmake
cmake_minimum_required(VERSION 3.16)   # 必须第一行，声明兼容的 CMake 版本
project(hello LANGUAGES CXX)           # 项目名 + 语言，会顺带探测编译器
add_executable(hello main.cpp)         # 目标名 + 源文件
```

> CMake 4.x 起，`cmake_minimum_required` 低于 3.5 会**直接报错**，老教程里的 `VERSION 2.8.3` 已经跑不通了。这是我在本机实测时踩到的第一个坑。

### 2.2 最常用的命令

| 命令 | 作用 | 备注 |
| :--- | :--- | :--- |
| `cmake_minimum_required(VERSION x.y)` | 声明最低版本 | 必须在最前 |
| `project(名字 VERSION 1.0 LANGUAGES CXX)` | 定义项目 | 会设置 `PROJECT_NAME`、`PROJECT_VERSION` |
| `add_executable(目标 源文件...)` | 生成可执行文件 | 目标名不能与已有目标重名 |
| `add_library(目标 [STATIC\|SHARED] 源文件...)` | 生成库 | 不写类型时由 `BUILD_SHARED_LIBS` 决定 |
| `target_link_libraries(目标 PRIVATE 库...)` | 链接依赖 | 优先链接"目标"，不要拼 `-l` |
| `target_include_directories(目标 PUBLIC 路径)` | 头文件搜索路径 | 关键字决定传递性 |
| `target_compile_options(目标 PRIVATE -Wall)` | 编译选项 | 不要直接改全局 `CMAKE_CXX_FLAGS` |
| `set(变量 值...)` | 定义变量/列表 | 列表用 `;` 分隔 |
| `option(开关 "说明" OFF)` | 布尔缓存变量 | 命令行 `-D开关=ON` |
| `if()/elseif()/else()/endif()` | 条件 | 注意 `STREQUAL` 是字符串比较 |
| `foreach(x IN LISTS 列表)` | 循环 | 配合 `set`/`target_sources` |
| `function()/endfunction()` | 函数 | **有自己的作用域** |
| `macro()/endmacro()` | 宏 | **没有新作用域**，只是文本替换 |
| `add_subdirectory(dir)` | 引入子目录 | 子目录里要有 `CMakeLists.txt` |
| `find_package(Foo REQUIRED)` | 找外部依赖 | 成功后得到 `Foo::Foo` 目标 |
| `message(STATUS "...")` | 打印信息 | `STATUS` 会带 `--` 前缀 |
| `install(TARGETS ...)` | 安装规则 | 配合 `cmake --install` |

### 2.3 一个"像样"的 CMakeLists.txt

下面是我实验里实际用的版本（库 + 可执行文件 + 测试 + 安装）：

```cmake
cmake_minimum_required(VERSION 3.16)
project(hello_demo VERSION 1.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(HELLO_ENABLE_VERBOSE "打印详细编译信息" OFF)
if(HELLO_ENABLE_VERBOSE)
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

add_library(greet STATIC src/greet.cpp)
target_include_directories(greet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(hello src/main.cpp)
target_link_libraries(hello PRIVATE greet)

include(CTest)
if(BUILD_TESTING)
  add_executable(test_greet tests/test_greet.cpp)
  target_link_libraries(test_greet PRIVATE greet)
  add_test(NAME greet_says_hi COMMAND test_greet)
endif()

install(TARGETS hello RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
```

注意这里**没有一行** `include_directories()`、`link_directories()`、`link_libraries()`。这不是遗漏，而是现代 CMake 的分水岭。

---

## 3. 目标（target）与三个关键字

旧写法（CMake 2.x 时代）把所有设置塞进**目录级**变量：

```cmake
include_directories(include)        # 影响此后所有目标，污染全局
add_executable(a a.cpp)
add_executable(b b.cpp)             # b 莫名其妙也拿到了 include
```

新写法**一切围绕 target**，依赖关系连"使用要求"一起传递：

```cmake
target_include_directories(greet PUBLIC include)
```

三个关键字决定"这套要求传给谁"：

| 关键字 | 自己编译时用 | 别人链接我时用 | 典型场景 |
| :--- | :---: | :---: | :--- |
| `PRIVATE` | ✅ | ❌ | 内部实现头文件、私有编译选项 |
| `PUBLIC` | ✅ | ✅ | 对外暴露的头文件、必需的依赖 |
| `INTERFACE` | ❌ | ✅ | 纯头文件库、只传递不编译 |

这就是所谓 **usage requirements（使用要求）**：`target_link_libraries(hello PRIVATE greet)` 之后，`hello` 自动获得了 `greet` 的 `PUBLIC` 头文件路径，**不需要再写一次**。
也可以配合生成器表达式按配置区分：

```cmake
target_compile_definitions(greet PRIVATE $<IF:$<CONFIG:Debug>,DEBUG_BUILD,NDEBUG>)
```

一句话总结：**能用 target 命令就别用目录级命令**。目录级命令只在你确定"这个目录下所有目标都要"时才用。

---

## 4. 变量、缓存与作用域

### 4.1 三种"变量"

| 类型 | 写法 | 生存范围 | 典型用途 |
| :--- | :--- | :--- | :--- |
| 普通变量 | `set(VAR value)` | 当前目录 + 其子目录 | 临时值、源文件列表 |
| 缓存变量 | `set(VAR value CACHE STRING "说明")` | 存进 `CMakeCache.txt`，跨次保留 | 用户可配置项 |
| 环境变量 | `$ENV{PATH}` | 进程环境 | 读外部环境，**修改不写回** |

`option()` 本质就是创建一个 `BOOL` 类型的缓存变量：

```cmake
option(BUILD_TESTING "是否构建测试" ON)
```

命令行传参的优先级最高，而且**一旦写进缓存，下次再改 CMakeLists 里的默认值就不生效了**——必须 `-D` 覆盖或删掉 `CMakeCache.txt`，这是新手最容易困惑的行为。

### 4.2 `CMAKE_BUILD_TYPE`

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

它只对 **单配置生成器**（Unix Makefiles、Ninja）有意义；Visual Studio、Xcode 这类**多配置生成器**会忽略它，改用 `cmake --build build --config Release`。
所以写 CMakeLists 时**不要**用 `if(CMAKE_BUILD_TYPE STREQUAL "Debug")` 来决定编译选项（多配置下永远不成立），应该用生成器表达式 `$<CONFIG:Debug>` 或 `$<$<CONFIG:Debug>:...>`。

### 4.3 作用域

- `add_subdirectory()` 会创建**子作用域**：子目录能读父目录的普通变量，**改不回父目录**；
- `function()` 是新作用域，`macro()` 不是（宏里的变量会泄漏到调用处）；
- 需要"往上写"时用 `set(... PARENT_SCOPE)`；
- `include()` 不创建作用域，相当于把文件内容原地粘贴。

---

## 5. 安装、测试与配置

### 5.1 安装

`install()` 只是**声明规则**，真正执行要跑 `cmake --install`：

```bash
cmake --install build --prefix /opt/myapp
```

- 不写 `--prefix` 时用 `CMAKE_INSTALL_PREFIX`（默认 `/usr/local`，装系统目录要 sudo）；
- 工程里推荐 `include(GNUInstallDirs)`，然后用 `CMAKE_INSTALL_BINDIR`、`CMAKE_INSTALL_LIBDIR`，交给 CMake 处理 lib/lib64 的差异；
- `DESTDIR` 用于打包时的"假根目录"：`DESTDIR=/tmp/pkg cmake --install build`。

### 5.2 测试

`include(CTest)` 会定义 `BUILD_TESTING` 选项（默认 ON）并提供 `add_test`：

```cmake
add_test(NAME greet_says_hi COMMAND test_greet)
```

运行：

```bash
ctest --test-dir build --output-on-failure
```

`ctest` 只知道"运行哪个命令、成功与否"，断言本身要写在测试程序里（我用的是 `<cassert>`；工程里一般上 GoogleTest / Catch2）。

---

## 6. 一次真实的 CMake 实验

为了不只停留在"看懂语法"，我在本机搭了一个最小但完整的工程，把配置、构建、运行、测试、安装跑了一遍。

### 6.1 目录结构

```
demo/
├── CMakeLists.txt
├── include/greet.h
├── src/greet.cpp
├── src/main.cpp
└── tests/test_greet.cpp
```

`greet` 是静态库，`hello` 是链接它的可执行文件，`test_greet` 是测试程序，头文件通过 `PUBLIC` 暴露给使用者。

### 6.2 配置 + 构建

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
```

实际输出（节选）：

```text
-- The CXX compiler identification is GNU 15.2.0
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Configuring done (0.1s)
-- Generating done (0.0s)
-- Build files have been written to: /home/.../demo/build

[ 16%] Building CXX object CMakeFiles/greet.dir/src/greet.cpp.o
[ 33%] Linking CXX static library libgreet.a
[ 33%] Built target greet
[ 66%] Building CXX object CMakeFiles/hello.dir/src/main.cpp.o
[100%] Linking CXX executable hello
[100%] Built target hello
```

**这里验证了两件事**：一是 `-B build` 属于外部构建（out-of-source），源码目录里不会出现 `.o` 和 `CMakeCache.txt`；二是渐进式百分比来自 `make` 的进度，CMake 只负责生成。

### 6.3 运行与测试

```bash
./build/hello
# Hello, CMake!

cd build && ctest --output-on-failure
# 1/1 Test #1: greet_says_hi ....................   Passed    0.00 sec
# 100% tests passed out of 1
```

### 6.4 安装

```bash
cmake --install build --prefix ../stage
```

```text
-- Install configuration: "Release"
-- Installing: .../stage/bin/hello
-- Installing: .../stage/include
-- Installing: .../stage/include/greet.h
```

`find ../stage -type f` 只有 `bin/hello` 和 `include/greet.h` 两个文件，和 `install()` 里写的规则完全一致——`install(TARGETS hello RUNTIME DESTINATION bin)` 只装可执行文件，头文件是 `install(DIRECTORY ...)` 单独装的。

### 6.5 缓存长什么样

`build/CMakeCache.txt` 里能直接看到生成时定下的值：

```text
BUILD_TESTING:BOOL=ON
CMAKE_BUILD_TYPE:STRING=Release
CMAKE_INSTALL_PREFIX:PATH=/usr/local
HELLO_ENABLE_VERBOSE:BOOL=OFF
```

`HELLO_ENABLE_VERBOSE` 就是 `option()` 的产物，`-DHELLO_ENABLE_VERBOSE=ON` 即可开启。

### 6.6 踩到/看到的坑

1. **CMake 版本门槛**：旧教程常用 `cmake_minimum_required(VERSION 2.8)`，CMake 4 直接报错。
2. **忘了 `--prefix`**：`cmake --install build` 会尝试写 `/usr/local`，普通用户必然 Permission denied。
3. **改 `CMakeLists.txt` 里的默认 `option` 不生效**：缓存优先，要 `-D` 或清缓存。
4. **`ctest` 找不到测试**：没写 `include(CTest)` 或 `BUILD_TESTING=OFF`。
5. **in-source build 污染**：直接在源码目录 `cmake .`，生成的 `CMakeFiles/`、`CMakeCache.txt` 很难清理干净，务必用 `-B`。

---

## 7. catkin：ROS 1 的构建系统

### 7.1 catkin 是什么

catkin 是 ROS 1 的官方构建系统，**本质是 CMake 的一层扩展**：

- 它提供一组 CMake 宏/函数（`catkin_package()`、`catkin_python_setup()`、`generate_messages()` 等），
- 再加一套 Python 工具（`catkin_make`、`catkin_create_pkg`、`catkin_init_workspace`），
- 用 `package.xml` 描述包元数据和依赖，用 CMake 完成实际的编译链接。

所以**学 catkin 的前提是先懂 CMake**：`add_executable`、`target_link_libraries`、`install` 这些在 ROS 包里照样用，catkin 只是额外负责"包的发现、依赖排序、环境变量"。

### 7.2 一个 ROS 包的最小构成

```
my_pkg/
├── CMakeLists.txt
├── package.xml
├── include/my_pkg/      # C++ 头文件
├── src/                 # C++ 源文件
├── scripts/             # Python 脚本（要 chmod +x）
├── launch/              # .launch 文件
└── msg/ srv/ action/    # 自定义消息/服务/动作
```

`package.xml` 是**包的身份证**，catkin 靠它建依赖图。format 2 的常见写法：

```xml
<package format="2">
  <name>my_pkg</name>
  <version>0.1.0</version>
  <description>示例包</description>
  <maintainer email="me@example.com">77430</maintainer>
  <license>MIT</license>

  <buildtool_depend>catkin</buildtool_depend>
  <depend>roscpp</depend>          <!-- depend = build + build_export + exec -->
  <build_depend>message_generation</build_depend>
  <exec_depend>message_runtime</exec_depend>
</package>
```

### 7.3 一个 C++ 节点的 `CMakeLists.txt`

catkin 包的 `CMakeLists.txt` 永远是这个骨架，删掉任何一段都可能有副作用：

```cmake
cmake_minimum_required(VERSION 3.0.2)
project(my_pkg)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES my_pkg
  CATKIN_DEPENDS roscpp std_msgs
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_library(my_pkg src/util.cpp)
add_executable(talker src/talker.cpp)
target_link_libraries(talker ${catkin_LIBRARIES})
```

几个关键点：

- `find_package(catkin REQUIRED COMPONENTS ...)`：**只在这里声明依赖**，写了 `roscpp` 才有 `${catkin_INCLUDE_DIRS}` 和 `${catkin_LIBRARIES}`；
- `catkin_package()`：告诉依赖我的包"我能提供什么"（头文件目录、库、传递依赖），**漏了它下游会找不到头文件**；
- `target_link_libraries(... ${catkin_LIBRARIES})`：这是 catkin 推荐的写法，比逐个写 `-lroscpp` 可靠；
- 自定义消息还要 `add_message_files()` + `generate_messages()` + `add_dependencies(目标 ${${PROJECT_NAME}_EXPORTED_TARGETS})`，否则第一次编译会因为消息头文件还没生成而失败。

### 7.4 catkin 的目录契约

| 目录 | 谁生成 | 里面是什么 | 能否提交到 git |
| :--- | :--- | :--- | :--- |
| `src/` | 你 | 源码包（每个子目录一个包） | ✅ 要提交 |
| `build/` | `catkin_make` | CMake 缓存、中间 `.o` | ❌ 加 `.gitignore` |
| `devel/` | `catkin_make` | 编译产物 + `setup.bash` | ❌ |
| `install/` | `catkin_make install` | 可部署的安装空间 | ❌ |

---

## 8. catkin 工作空间与一次 `catkin_make`

### 8.1 从零建一个工作空间

```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace          # 在 src/ 下建 CMakeLists.txt 软链，指向 catkin 的 toplevel.cmake
catkin_create_pkg my_pkg roscpp std_msgs   # 生成包骨架（CMakeLists.txt + package.xml）
cd ~/catkin_ws
catkin_make                    # 等价于在 build/ 里 cmake .. && make
```

第一次 `catkin_make` 会自动创建 `build/` 和 `devel/`，并在 `devel/` 里生成 `setup.bash`。

### 8.2 `catkin_make` 到底做了什么

它本质是一个**包装脚本**，做了这些事：

1. 检查工作空间根目录（有 `src/CMakeLists.txt` 软链才算）；
2. 在 `build/` 里调用 `cmake`，参数里带上 `-DCATKIN_DEVEL_PREFIX=../devel`；
3. 调用 `make -jN`；
4. 生成 `devel/setup.bash`（以及 `_setup_util.py`、环境钩子）。

而 cmake 配置阶段，`catkin` 会**扫描 `src/` 下所有含 `package.xml` 的目录**，解析依赖关系，做**拓扑排序**，生成一个总的构建顺序——被依赖的包先编。这就是为什么"改了一个底层包，上层包会自动重编"。

### 8.3 `source` 这一步不能省

```bash
source ~/catkin_ws/devel/setup.bash
```

它设置的是：

| 环境变量 | 作用 |
| :--- | :--- |
| `ROS_PACKAGE_PATH` | `rospack find` 能找到你的包 |
| `CMAKE_PREFIX_PATH` | `find_package` 能找到依赖包 |
| `LD_LIBRARY_PATH` | 运行时能找到 `.so` |
| `PYTHONPATH` | Python 节点能 `import` 包 |

**每条新终端都要 source 一次**（或写进 `~/.bashrc`）。忘了 source 的典型症状是：`rospack find my_pkg` 找不到、`rosrun` 说 "package not found"、`find_package` 报 Could not find。

### 8.4 三种构建工具的区别

| 工具 | 构建目录 | 特点 |
| :--- | :--- | :--- |
| `catkin_make` | 单一 `build/`、`devel/` | ROS 官方入门工具，所有包共享一个 CMake 工程，配置快、互相干扰 |
| `catkin_make_isolated` | 每个包独立 `build/`、`devel/` | 隔离性好，生成 `install/` 空间，慢 |
| `catkin build`（catkin_tools） | 每个包独立目录 | 真正并行、增量、可按包构建，大工作空间首选 |

常用的 `catkin_make` 参数：

```bash
catkin_make -j4                       # 并行
catkin_make --pkg my_pkg              # 只编一个包
catkin_make --only-pkg-with-deps my_pkg   # 只编它和它依赖的
catkin_make -DCMAKE_BUILD_TYPE=Release
catkin_make install                   # 生成 install/ 空间
```

### 8.5 常见坑清单

1. **在 `src/` 里跑 `catkin_make`**：会报找不到 `src/CMakeLists.txt`，必须在工作空间根目录跑。
2. **忘了 `catkin_init_workspace`**：`catkin_make` 直接报 "The specified base path ... contains a CMakeLists.txt but ..."。
3. **包名和目录名不一致**：`package.xml` 里的 `<name>` 必须等于目录名，否则 `rosrun` 找不到。
4. **`devel/` 和 `install/` 混用**：两个空间同时 source 会造成库版本混乱，只 source 一个。
5. **`package.xml` 没写依赖但 CMakeLists 用了**：`find_package` 可能碰巧成功（系统装了），换台机器就失败——**依赖必须两边都写全**。
6. **Python 脚本没执行权限**：`chmod +x scripts/*.py`，否则 `rosrun` 报权限错误。
7. **ROS 1 与 ROS 2 混淆**：ROS 2 用 `ament_cmake` + `colcon`，宏名完全不同（`catkin_package` → `ament_package`），网上的教程要看清版本。

> 说明：本机没有 ROS 环境（`/opt/ros` 不存在、`catkin` 命令不存在），这一节是基于文档和 CMake 原理整理的，**没有实际跑通 `catkin_make`**；CMake 部分则是真实实验。两者不混为一谈。

---

## 9. 补充资料学习：The Emperor Has No Clothes

这篇 [The Emperor Has No Clothes](https://www.mihaileric.com/The-Emperor-Has-No-Clothes/)（Mihail Eric，2026-01）讲的是**用 200 行 Python 复刻一个 Claude Code 式的编程 Agent**。放进构建系统的课程里读，我觉得意图很明确：**让你看清"AI 帮你写代码"这件事的机制**。

### 9.1 核心观点：Agent = LLM + 工具循环

文章把整个过程拆成五步，循环往复：

1. 你发一条消息（"帮我建一个 hello 文件"）；
2. LLM 判断需要工具，返回**结构化**的工具调用；
3. 你的程序在本地真正执行这个调用；
4. 执行结果回传给 LLM；
5. LLM 基于新上下文继续，直到任务完成。

关键一句：**LLM 本身从不碰你的文件系统**，它只是"请求"某件事发生，真正动手的是你的代码。所以"AI 删了我的库"这类说法，准确讲是"我的程序照着 AI 的请求执行了危险操作"。

### 9.2 三个工具就够了

| 工具 | 作用 |
| :--- | :--- |
| `read_file` | 让模型看到代码 |
| `list_files` | 让模型浏览工程结构 |
| `edit_file` | 让模型下达修改指令 |

生产级 Agent 还有 `grep`、`bash`、联网搜索等，但文章的实验说明：**读、列、改**三个能力就足以完成真实任务。这解释了我用过的所有编程助手为什么都有"读文件/改文件"这个最小集合。

### 9.3 对学习构建系统的启示

- **工具调用的结果才是事实**：模型说"已生成 CMakeLists.txt"不算数，`ls` 一下、`cmake -S . -B build` 跑一下才算数。这和我上面"必须本机实测"的习惯一致。
- **上下文就是状态**：Agent 不记忆，它每次看到的只有被塞进上下文的那部分。工程里文件一多，`CMakeLists.txt` 的依赖关系很容易被忽略——这也解释了为什么 AI 生成的构建脚本经常"少一个 `target_link_libraries`"。
- **限制工具 = 限制风险**：只给 read/list 的 Agent 不可能改坏东西。反过来，给 AI 的权限越大，你越要自己 review。

---

## 10. 用大模型学构建系统：我的做法和边界

这门课反复强调"负责任使用 AI"，我把这一节写成**可复用的方法**，而不是结论。

### 10.1 确实有效的用法

| 场景 | 具体做法 |
| :--- | :--- |
| 读报错 | 把完整报错 + 相关 `CMakeLists.txt` 片段贴进去，让它解释"哪一行、为什么" |
| 做对照 | 让它把同一功能的"旧写法 / 现代 target 写法"并列，自己判断差异 |
| 出实验 | 让它给最小可复现工程，自己跑一遍验证输出 |
| 查概念 | "PUBLIC 和 INTERFACE 在纯头文件库里分别意味着什么"这类边界问题 |
| 查语法 | 正则、`foreach`、生成器表达式的写法 |

### 10.2 容易踩的坑

1. **版本错配**：模型训练数据里的 CMake 教程大量停留在 2.8/3.0，`cmake_minimum_required(VERSION 2.8)` 在 CMake 4 上直接报错。**版本号永远以本机 `cmake --version` 为准**。
2. **ROS 1/ROS 2 混淆**：问 catkin 的问题，答案里混进 `ament_package()`、`colcon build` 是常事。
3. **"看起来对"的 CMakeLists**：能配置通过，但依赖传递是错的（该 `PUBLIC` 写了 `PRIVATE`），下游一用就炸。
4. **幻觉 API**：`target_link_directories`、`catkin_add_gtest` 这类不常用命令，模型经常编出不存在的参数。

### 10.3 我的三条底线

- **凡是命令，必须本机跑过**；没跑过的输出不写进笔记。
- **凡是"为什么"，先自己想，再问 AI**；AI 的解释只用来校验或补漏。
- **凡是提交进仓库的配置，逐行能讲清楚**；讲不清就说明还没学会，不能算完成作业。

### 10.4 关于这份笔记本身

这份笔记里，CMake 部分的命令、日志、缓存内容都是我在本机（CMake 4.4.4 / GCC 15.2.0）**实际跑出来的**；catkin 部分因为本机没有 ROS 环境，明确标注为"未实测"。写作过程中使用了 AI 助手做结构梳理和文字润色，观点与实验由我自己验证。**课程要求"不应该直接用 AI 生成笔记"，我认为关键在"验证"这一步有没有做**——这一点我在这里如实说明，供助教判断。

---

## 11. 速查表

### 11.1 CMake

```bash
cmake -S . -B build                      # 配置（外部构建）
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4                  # 构建
cmake --build build --target clean       # 清理
cmake --install build --prefix /opt/app  # 安装
ctest --test-dir build --output-on-failure
cmake -LAH -N build                      # 列出所有缓存变量及说明
```

| 需求 | 命令 |
| :--- | :--- |
| 加可执行文件 | `add_executable(app main.cpp)` |
| 加库 | `add_library(lib STATIC a.cpp)` |
| 链接 | `target_link_libraries(app PRIVATE lib)` |
| 头文件 | `target_include_directories(lib PUBLIC include)` |
| C++ 标准 | `set(CMAKE_CXX_STANDARD 17)` + `..._REQUIRED ON` |
| 找依赖 | `find_package(Threads REQUIRED)` → `Threads::Threads` |
| 开关 | `option(FOO "说明" OFF)`，`-DFOO=ON` |
| 子目录 | `add_subdirectory(sub)` |

### 11.2 catkin

```bash
catkin_init_workspace
catkin_create_pkg my_pkg roscpp std_msgs
catkin_make
catkin_make --pkg my_pkg
catkin_make install
source devel/setup.bash
rospack find my_pkg
rosrun my_pkg talker
```

| 需求 | 写法 |
| :--- | :--- |
| 声明依赖 | `find_package(catkin REQUIRED COMPONENTS roscpp)` |
| 导出接口 | `catkin_package(INCLUDE_DIRS include LIBRARIES my_pkg CATKIN_DEPENDS roscpp)` |
| 链接 | `target_link_libraries(talker ${catkin_LIBRARIES})` |
| 消息依赖 | `add_dependencies(talker ${${PROJECT_NAME}_EXPORTED_TARGETS})` |

---

## 12. 参考资料

- CMake 官方文档：<https://cmake.org/documentation/>
- CMake 教程（菜鸟教程）：<https://www.runoob.com/cmake/cmake-tutorial.html>
- catkin 教程（ROS Wiki）：<https://wiki.ros.org/catkin/Tutorials>
- The Emperor Has No Clothes：<https://www.mihaileric.com/The-Emperor-Has-No-Clothes/>
- 知乎：ROS 编译详解（from Makefile to catkin_make）：<https://zhuanlan.zhihu.com/p/476697014>
  （该链接在我这里返回 403，未能读到原文，上文 catkin 部分依据 ROS Wiki 与本地 CMake 实验整理）

---

<sub>本文中 CMake 的命令与输出均来自本机实测记录（2026-10-27，CMake 4.4.4 / GCC 15.2.0），catkin 部分未在本机运行，已单独标注。</sub>
