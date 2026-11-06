# CMake 与 Catkin 学习笔记

> 参考资料：
> - [CMake 基本教程 - 菜鸟教程](https://www.runoob.com/cmake/cmake-basic.html)
> - [ROS catkin 官方教程](https://wiki.ros.org/catkin/Tutorials)
> - [知乎：CMake 入门](https://zhuanlan.zhihu.com/p/476697014)
> - [The Emperor Has No Clothes: How to Code Claude Code in 200 Lines](https://www.mihaileric.com/The-Emperor-Has-No-Clothes/)

---

## 一、CMake 是什么

CMake 是一个**跨平台的构建系统生成工具**，用于管理 C/C++ 项目的编译过程。它本身并不直接编译代码，而是通过读取 `CMakeLists.txt` 配置文件，根据项目的构建规则、依赖关系和编译选项，自动生成对应平台的构建文件（Makefile、Ninja、VS 工程等），从而省去手写 Makefile 的繁琐工作。

- **本质**：一个"元构建系统"（meta-build system），生成别的构建系统的输入。
- **工作方式**：编写 `CMakeLists.txt` → CMake 解析 → 生成 Makefile/工程文件 → 调用编译器完成真正的编译。
- **配置语言**：有自己的脚本语法，但变量、条件、函数等概念和通用编程语言互通，上手门槛不高。

---

## 二、基本语法规则

`CMakeLists.txt` 由一系列指令组成，每条指令的格式为：

```cmake
命令名(参数1 参数2 ...)
```

- 命令名**不区分大小写**（但习惯用小写或驼峰）。
- 参数之间用空格或换行分隔。
- `#` 开头为注释。
- CMake 是**顺序解析脚本**：先出现的指令先执行。
- 但 `target_xxx` 系列命令有硬性规则：**必须先用 `add_executable` / `add_library` 建好目标，才能对该目标做配置**。

---

## 三、核心指令

### 3.1 指定最低版本

```cmake
# 必须放在 CMakeLists.txt 最前面
cmake_minimum_required(VERSION 3.10)
```

### 3.2 声明项目

```cmake
# 语法：project(<项目名> [VERSION x.y.z] [LANGUAGES C CXX])
project(MyProject VERSION 1.0.0 LANGUAGES CXX)
```

声明后会自动生成一些变量，例如：
- `PROJECT_NAME`：项目名
- `PROJECT_SOURCE_DIR`：项目根目录
- `PROJECT_VERSION`：项目版本号

### 3.3 设置 C++ 标准

```cmake
set(CMAKE_CXX_STANDARD 17)        # 使用 C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制要求，不支持则报错
```

### 3.4 添加可执行文件

```cmake
# 将指定源文件编译生成一个可执行文件
add_executable(MyApp main.cpp utils.cpp)
```

### 3.5 添加库文件

```cmake
# 语法：add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)
add_library(MyLib STATIC library.cpp)
```

| 类型 | 生成产物 | 链接方式 | 用途 |
| --- | --- | --- | --- |
| `STATIC` | 静态库（`.a` / `.lib`） | 编译时直接嵌入可执行文件 | 代码保密、减少运行时依赖 |
| `SHARED` | 动态库（`.so` / `.dll`） | 运行时加载 | 多程序共享、便于升级 |
| 不指定 | 由 `BUILD_SHARED_LIBS` 变量决定 | — | — |

> **直观理解**：静态库相当于已经编译成机器码的 `.c` 文件，最终被"复制"进可执行文件；动态库是独立的 `.so` 文件，可执行文件在运行时去"引用"它。

### 3.6 链接库到目标

```cmake
# 语法：target_link_libraries(<目标> <作用域> <库>...)
target_link_libraries(MyApp PRIVATE MyLib)
```

把指定的库链接到目标（可执行文件或其他库），可以链接项目内部的库目标，也可以链接外部库。

### 3.7 指定头文件搜索路径

```cmake
# 语法：target_include_directories(<目标> [PUBLIC|PRIVATE|INTERFACE] <路径>...)
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
```

告诉编译器"去哪里找 `#include` 的头文件"。

### 3.8 查找第三方包

```cmake
# 基本用法：找不到则报错中止
find_package(Boost REQUIRED)

# 指定最低版本
find_package(Boost 1.70 REQUIRED)

# 在自定义路径下查找
find_package(OpenCV REQUIRED PATHS /path/to/opencv)
```

找到后通常会生成对应的"导入目标"（如 `Boost::boost`、`OpenCV::OpenCV`），直接用 `target_link_libraries` 链接即可，**头文件路径和库路径会自动设置好**。

### 3.9 安装规则（可选）

- **不写 install**：编译产物只待在 `build/` 目录里，仅用于本地测试。
- **写 install + `cmake --install`**：把可执行文件、库、头文件部署到系统标准位置，方便全局调用、给其他项目依赖，或打包发布。

```cmake
install(TARGETS MyApp RUNTIME DESTINATION bin)   # 可执行文件 → bin/
install(TARGETS MyLib ARCHIVE DESTINATION lib)   # 静态库 → lib/
```

对应关系：
- `RUNTIME DESTINATION`：可执行文件
- `LIBRARY DESTINATION`：动态库
- `ARCHIVE DESTINATION`：静态库
- `INCLUDES DESTINATION`：头文件

---

## 四、目标作用域三关键字

`target_include_directories` 和 `target_link_libraries` 等命令都要指定作用域，这是 CMake 现代目标（modern target）思想的核心：

| 关键字 | 含义 | 典型使用场景 |
| --- | --- | --- |
| `PRIVATE` | 只有当前目标自己用，**不传递**给依赖它的下游 | 内部实现细节、内部头文件 |
| `PUBLIC` | 当前目标自己用，**同时传递**给下游 | 对外接口的头文件路径（最常用） |
| `INTERFACE` | 当前目标自己不用，**只传递**给下游 | 纯头文件库（header-only library） |

**记忆口诀**：
- `PRIVATE` = 自用，不外传
- `PUBLIC` = 自用 + 传出去
- `INTERFACE` = 自己不用，只传出去

> **最佳实践**：优先使用 `target_xxx` 系列指令（而不是旧的 `include_directories`、`link_libraries` 目录级命令），依赖关系会随目标自动传递。

---

## 五、完整示例

项目结构：

```
MyProject/
├── CMakeLists.txt
├── include/
│   └── mylib/
│       └── core.h
└── src/
    ├── main.cpp
    ├── utils.cpp
    └── lib/
        ├── core.cpp
        └── helper.cpp
```

根目录 `CMakeLists.txt`：

```cmake
# 1. 指定 CMake 最低版本（必须放在最前面）
cmake_minimum_required(VERSION 3.10)

# 2. 定义项目名称、版本和语言
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

# 3. 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 添加静态库目标
add_library(MyLib STATIC
    src/lib/core.cpp      # 核心功能实现
    src/lib/helper.cpp    # 辅助函数实现
)

# 5. 添加可执行文件目标
add_executable(MyApp
    src/main.cpp          # 程序入口
    src/utils.cpp         # 工具函数
)

# 6. 为库目标指定公开头文件路径
#    PUBLIC：使用 MyLib 的目标也能访问该路径
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 7. 为可执行文件指定私有头文件路径
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/src)

# 8. 将 MyLib 链接到 MyApp
target_link_libraries(MyApp PRIVATE MyLib)

# 9. 设置安装规则
install(TARGETS MyApp RUNTIME DESTINATION bin)
install(TARGETS MyLib ARCHIVE DESTINATION lib)
```

**引入第三方库（以 Boost 为例）**：

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyBoostProject CXX)

set(CMAKE_CXX_STANDARD 17)

# 查找 Boost，找不到则报错退出
find_package(Boost REQUIRED)

add_executable(MyApp main.cpp)

# 使用导入目标链接 Boost
# Boost::boost 会自动设置头文件路径和库文件路径
target_link_libraries(MyApp PRIVATE Boost::boost)
```

---

## 六、C/C++ 程序编译四阶段

理解 CMake 在做什么之前，先搞清楚一个 `.c` 文件最终如何变成可执行文件。整个过程分为四步：

| 阶段 | 输入 | 输出 | 做了什么 |
| --- | --- | --- | --- |
| **预处理** Preprocessing | `.c` | `.i` | 处理 `#` 指令：`#include` 展开、`#define` 替换、注释删除、条件编译裁剪 |
| **编译** Compilation | `.i` | `.s`（汇编文本） | 词法分析 → 语法分析 → 语义分析 → 优化，翻译成汇编代码 |
| **汇编** Assembly | `.s` | `.o`（可重定位目标文件） | 汇编器 `as` 把汇编翻译成机器指令，打包成二进制 `.o` |
| **链接** Linking | 多个 `.o` + 库 | 可执行文件 | 链接器 `ld` 把所有目标文件和依赖库合并，解析符号引用，生成可执行文件 |

### `.h` 与 `.c` 的关系

- `.h` 文件 = **说明书（声明）**：告诉编译器"这个函数长什么样、这个结构体有哪些字段"，但不给出具体实现。
- `.c` 文件 = **实物（实现）**：函数体的真正代码。
- `.c` 文件 `#include` 自己的 `.h`：**自检**——让编译器对照声明和实现，防止两者签名不一致。
- `main.c` `#include` 别人的 `.h`：**看懂说明书**——知道怎么调用外部函数。
- **链接阶段**：把"实物"（`.o` 文件、库）拿过来，和主程序拼在一起，形成最终可执行文件。

> CMake 的 `add_executable` / `add_library` 负责告诉编译器要编译哪些源文件；`target_include_directories` 负责告诉编译器去哪找 `.h`；`target_link_libraries` 负责告诉链接器要把哪些 `.o` / 库拼进来。

---

## 七、Catkin —— ROS 的构建系统

Catkin 是 ROS（Robot Operating System）在 catkin 版本之前使用的专用构建系统，它本质上是在 CMake 外层套了一层宏和约定，用来管理 ROS 的功能包（package）依赖、消息生成、环境变量等。

### 7.1 初始化工作空间

```bash
# 1. 先启用系统自带的 ROS 环境（以 Noetic 为例）
source /opt/ros/noetic/setup.bash

# 2. 创建工作空间并进入 src 目录
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src

# 3. 回到工作空间根目录，执行编译
cd ~/catkin_ws
catkin_make
```

编译完成后会生成三个目录：

| 目录 | 作用 |
| --- | --- |
| `src/` | **主要编辑区**：功能包源码都放这里 |
| `build/` | CMake 缓存和中间编译产物 |
| `devel/` | 开发态的可执行文件、库和环境脚本 |

### 7.2 启用工作空间环境

每次新开终端，都要 source 一下当前工作空间，ROS 才能找到你编译出来的包：

```bash
source ~/catkin_ws/devel/setup.bash
```

> 可以把这行加到 `~/.bashrc` 里，避免每次手动输入。

### 7.3 创建功能包

进入工作空间的 `src/` 目录，用 `catkin_create_pkg` 新建包，后面可以跟依赖包列表：

```bash
cd ~/catkin_ws/src
catkin_create_pkg <包名> [依赖1] [依赖2] ...
# 例：catkin_create_pkg my_package roscpp std_msgs
```

创建后会自动生成：
- `CMakeLists.txt`：包的构建脚本（基于 CMake）
- `package.xml`：包的元信息（名称、版本、作者、依赖等），按标签填写说明即可
- `include/`、`src/`：头文件和源文件目录

之后在包内写代码，再回到工作空间根目录执行 `catkin_make` 即可。

---

## 八、学习随想

读完 [The Emperor Has No Clothes](https://www.mihaileric.com/The-Emperor-Has-No-Clothes/) 一文，最大的触动是：**原本以为 Agent 这类技术高深莫测、像皇帝的新衣一样不可直视，结果核心实现只有 200 行代码**。所谓"技术壁垒"并没有想象中那么高。

这给我的启发是：
1. 顺着这个思路，完全可以自己搭一个小项目——比如让 AI 连接本地部署的文生图模型，通过不断发送提示词来生成连续的人物立绘，实现"和二次元角色对话时它会'动起来'"的效果。技术路径上就是：对话状态 → 拼接提示词 → 调用本地生图接口 → 渲染到前端。核心组件都不复。
2. 工具链（CMake、Catkin、ROS）看起来文件多、命令杂，但拆开看就是"告诉编译器去哪找东西、把哪些东西拼到一起"这一件事。
3.ai是一个很好的提问工具，即使不懂提问的艺术，也可以不断的和提问来把握脉
