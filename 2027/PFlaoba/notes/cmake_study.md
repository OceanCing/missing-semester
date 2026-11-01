# DX控制组第2课笔记

## 目录

- **一、 CMake**
- **二、 catkin**
- **三、 LLM与Agent**
- **四、 总结**

## 一、 CMake

### 0. 概念

CMake 严格来说不是一门脚本语言，准确来说ta是构建工具，同时提供了一种专门用于构建配置和脚本的语言  

为什么要用 CMake ？  

首先我们需要了解cpp是如何变成可执行程序的：
1. **预处理（Preprocessing）**:处理头文件、宏定义、条件编译等
2. **编译（Compilation）**:将预处理后的 C++ 代码转换成汇编代码
3. **汇编（Assembly）**:将汇编代码转换成目标文件（机器码）
4. **链接（Linking）**:将目标文件和所需的库组合，生成可执行程序

当我们想把.cpp等源文件变成可执行程序时，需要使用 `g++` `cl.exe` 等**编译器驱动**与 `ld` `link.exe` 等**连接器**进行上述等步骤，最终生成可执行程序  
但是 `g++` `cl.exe` 都可以自动调用链接器，因此日常编译时通常只需要运行它们，不用手动调用 `ld` `link.exe`  

当我们做大项目时仅使用 `g++`等工具会有许多痛点，如需要输入长指令，管理、多人协作麻烦，导致效率低下

我们需要一个工具，能够记住构建规则，并自动决定哪些文件需要重新构建

这就是 **Make** 这个构建工具发挥作用的地方

Make 的核心优势：**依赖关系**、**自动构建顺序**、**增量构建**

例如，我们可以写一个名为 Makefile 的文件：
```Makefile
calculator: main.o add.o subtract.o
	g++ main.o add.o subtract.o -o calculator

main.o: main.cpp
	g++ -c main.cpp -o main.o

add.o: add.cpp add.h
	g++ -c add.cpp -o add.o

subtract.o: subtract.cpp
	g++ -c subtract.cpp -o subtract.o
```

表达情况：
- `calculator` 依赖 `main.o`、`add.o` 和 `subtract.o`
- `main.o` 依赖 `main.cpp`
- `add.o` 依赖 `add.cpp` 和 `add.h`
- 每个目标都有对应的生成命令

执行：
```bash
make
```

Make 就会读取 `Makefile`，分析这些依赖关系，并执行必要的命令来生成程序  

假设第一次构建成功后，你只修改了 `add.cpp`

再次执行：
```bash
make
```

在依赖关系正确、文件时间戳正常的情况下，Make 通常只需要：
1. 重新编译 `add.cpp`，生成新的 `add.o`
2. 重新链接目标文件，生成新的 `calculator`
它不必重新编译完全没有变化的 `main.cpp` 和 `subtract.cpp`

这就是增量构建：只重新构建受变化影响的部分  

Make 的工作流程：
读取 Makefile --> 分析目标和依赖关系 --> 判断哪些目标需要更新 --> 执行编译、链接等命令

注意：Make 不负责理解 C++ 语法，也不负责亲自编译代码
它主要负责按照规则调用 g++ 等工具  

既然有 Make 了为什么还需要 CMake ？

其1：项目越大，Makefile 越复杂
大型项目文件成百上千，还有很多第三方库、编译选项和不同的构建目标，人为手动管理这些规则会越来越繁琐

其2：不同平台可能使用不同的构建工具
例如：
| 环境 | 常见构建工具或系统 |
|-|-|
| Linux | GNU Make、Ninja |
| Windows |	MSBuild、Ninja、NMake |
| macOS | Make、Ninja、Xcode 构建系统 |

这些工具的规则格式和使用方式并不完全相同，如果项目要求支持多个平台，开发者就可能需要维护多套构建配置

其3：构建规则和平台细节混在一起
我们想表达或知道的通常是：
- 生成的可执行程序
- 源文件组成
- 依赖库
- 编译选项
而不是每次都关心某个平台具体需要怎样编写构建文件

所以，我们需要一种更高层次的方式来描述项目的构建规则，再由工具把这些规则转换成具体构建系统能理解的格式，实现所谓的“大一统”  

CMake 是一个**构建系统生成工具**

例如，前面那份手写 `Makefile` 可以对应成这样的 `CMakeLists.txt`：
```cmake
cmake_minimum_required(VERSION 3.16)

project(Calculator)

add_executable(calculator main.cpp add.cpp subtract.cpp)
```

CMake 会按照对应语言生成相应的构建配置  
至于最终使用 Make、Ninja 还是其他构建系统，则取决于生成器以及开发环境

执行：
```bash
cmake -S . -B build
cmake --build build
```

CMake 运行分三个阶段：
| **配置（Configure）** | **生成（Generate）** | **构建（Build）** |
|-|-|-|
| 读`CMakeLists.txt`<>建`target`图 | 把`target`写成 `Make/ninja` | 真正调用 编译器/链接器 进行编译链接 |
| 在内存里算 | 落到磁盘文件 | 产出 `.o` 可执行文件 |

对应命令：
```bash
cmake -S . -B build        # 配置 + 生成（-S 源码目录，-B 构建目录）
cmake --build build        # 构建
cmake --install build      # （可选）安装
```

### 1. 基础用法

#### 1.1 框架

```cmake
cmake_minimum_required(VERSION 3.20)   # 声明最低 CMake 版本
project(hello VERSION 1.0 LANGUAGES CXX)  # 项目名、版本、语言

add_executable(hello main.cpp)         # 生成一个可执行文件 target
```

#### 1.2 target

要让 `g++` 把一个 `.cpp` 编译成一个 `.o`、再把一堆 `.o` 链接成可执行文件，你需要给编译器以下信息：

1. **源码文件**（编译哪些 .cpp）→ `add_executable`/`add_library` 里的列表
2. **头文件位置**（`-I`）→ `target_include_directories`
3. **编译期宏**（`-D`）→ `target_compile_definitions`
4. **编译选项**（`-Wall -O2`）→ `target_compile_options`
5. **C++ 标准**（`-std=c++20`）→ `target_compile_features`
6. **要链接哪些库**（`-l`）→ `target_link_libraries`

#### 1.3 库

**静态库 STATIC（`.a`）**：编译时把库的机器码直接复制进最终可执行文件，运行时不再需要这个 `.a`
- 特点：可执行文件自包含、稍大，每个链接它的可执行文件里都有一份拷贝

**动态库 SHARED（`so.`）**：链接时只记符号，运行时加载 `.so` ,可执行文件很小，但运行时必须能找到 `.so`
- 注意：动态库默认要求代码位置无关（PIC, -fPIC），CMake 在 SHARED 时默认加

**接口库 INTERFACE（header-only）**：现代 C++ 大量 header-only 库（Eigen、nlohmann/json、很多模板库），没有实现文件，全是头文件
- 特点：只是一堆属性（include 路径、宏、依赖）的容器

```cmake
add_library(pid_header INTERFACE)                    # 没有 .cpp，纯头文件
target_include_directories(pid_header INTERFACE include)
target_compile_features(pid_header INTERFACE cxx_std_20)
```

**对象库 OBJECT（`.o` 集合）**：一组`.o`目标文件
- 用途：同一批 `.cpp` 需要被多个 target 复用，又不想编译两次、也不想打包成库

```cmake
add_library(common OBJECT src/util.cpp)
add_library(robot_core STATIC src/pid.cpp $<TARGET_OBJECTS:common>)
add_executable(app app/main.cpp $<TARGET_OBJECTS:common>)
```

#### 1.4 用`find_package`找包

```cmake
find_package(OpenCV REQUIRED)
target_link_libraries(app PRIVATE opencv_core opencv_imgproc)   # 直接拿到 target
# 或
target_link_libraries(app PRIVATE Eigen3::Eigen)                # 带命名空间 :: 的 imported target
```

### 2. 编译特性与编译选项

用 `target_compile_features` 设 C++ 标准  

```cmake
target_compile_features(app PRIVATE cxx_std_20)
```

用 `target_compile_options` 设警告与编译选项

```cmake
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic)
```

对应 g++ 的参数：`-Wall` 开一批常用警告、`-Wextra` 再多一批、`-Wpedantic` 严格遵循标准

切换**构建类型（build type）**

| 构建类型 | 优化 | 调试符号 | 典型编译参数 | 用途 |
|---------|:---:|:---:|------------|------|
| **Debug** | 关（-O0） | 全（-g） | `-g` | 开发调试，能断点 |
| **Release** | 全（-O3） | 无 | `-O3 -DNDEBUG` | 上场跑，最快 |
| **RelWithDebInfo** | 中（-O2） | 有 | `-O2 -g -DNDEBUG` | 又要快又要能查问题 |
| **MinSizeRel** | 按体积（-Os） | 无 | `-Os -DNDEBUG` | 嵌入式空间紧张 |

单配置生成器（Make/Ninja）下，用 `CMAKE_BUILD_TYPE` 指定：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug      # 调试
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release    # 发布/上场
```

### 3. 报错与调试

| 现象 | 原因 | 怎么查 |
|------|------|--------|
| `Could not find a package configuration file provided by "Foo"` | 库没装，或 CMake 找不到 | `apt install` 或 `-DCMAKE_PREFIX_PATH` |
| `undefined reference to ...` | 链接缺边：某库没 `target_link_libraries` | 检查依赖图 |
| `No rule to make target ...` / 改了 CMakeLists 没反应 | 图纸改了没重新配置 | 重跑 `cmake -S . -B build` |
| 变量取值不对 | 作用域问题 | `message(STATUS "var=${var}")` 打印 |

```bash
# 1) 打印变量/属性
message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
get_target_property(inc robot_core INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "robot_core include = ${inc}")

# 2) 看 CMake 执行
cmake -S . -B build --trace

# 3) 看最终生成的编译命令
cmake --build build --verbose   # 或 cmake --build build -v
```

### 4. 附录：链接器与程序加载原理

1. **符号表（Symbol Table）**：程序记录“某个函数、变量量名、位置”的信息表

2. **重定位（Relocation）**：把程序中暂时无法确定的地址，修正为最终可以使用的地址（只声明并调用函数找不到定义却可以通过编译的原因）

3. **ELF（Executable and Linkable Format）**：是 Linux 下可执行文件、目标文件和共享库常用的文件格式  
ELF 文件中包含机器指令、数据、符号表以及其他供链接和加载使用的信息

4. **虚拟地址空间（Virtual Address Space）**：是操作系统为进程提供的地址视图
程序运行时，使用的不是简单地直接操作物理内存地址，而是通过虚拟地址访问内存

5. **动态链接器（Dynamic Linker）**：负责在程序启动（需要）时，帮助程序找到并加载所需的共享库，并完成必要的符号解析和重定位


## 二、 catkin

### 0. 概念

catkin 是 ROS1 的官方“工程构建系统”

catkin 相比 CMake的 变化：
1. 增加“包（Package）级”的自动排序：只要你的代码在 src 里，并且有 package.xml，Catkin 就可自动扫描所有的包并推算依赖关系

2. 实现代码翻译（消息自动生成）：，Catkin 会在编译时自动把自定义消息文件（比如 carry.msg）翻译成 C++ 的 .h 头文件和 Python 代码

3. 环境变量自动配置（一键环境挂载）：catkin编译完会生成`devel/setup.bash`，`source`后就能认识所有包、可执行文件及环境变量，不用手动配任何路径

4. devel 和 build（统一仓库管理）：catkin强行规定了“工作空间（Workspace）”的目录结构。所有的编译产物集中放在 build，所有的最终代码集中放在 devel，所有源码放 src

### 1. 基础

#### 1.1 ROS软件包与catkin工作空间

##### 1.1.1 生成
`package.xml`：存储软件包名称、版本、描述、维护者、许可证、依赖等元信息

catkin的**工作空间**:
catkin_ws/
├── src/       # 源代码空间
├── build/     # 构建过程中产生的文件
├── devel/     # 开发空间，包含构建后的开发产物
└── install/   # 安装空间，使用安装目标后生成

```bash
cd ~/catkin_ws/src
catkin_create_pkg new_pkg std_msgs rospy roscpp
```

> 这条命令会创建软件包目录及初始配置文件，并记录所声明的依赖

含义：
- new_pkg：新软件包的名称
- std_msgs：常见的 ROS 消息定义包
- rospy：ROS 的 Python 客户端库
- roscpp：ROS 的 C++ 客户端库

##### 1.1.2 构建与加载环境

```bash
cd ~/catkin_ws
catkin_make
source ~/catkin_ws/devel/setup.bash
```

`source ~/catkin_ws/devel/setup.bash`加载生成的环境配置，使**当前终端**能够找到该工作空间中的软件包和构建产物

##### 1.1.3 软件包依赖

```bash
rospack depends1 new_pkg
rospack depends new_pkg
```

- depends1：查看直接依赖
- depends：查看递归依赖，即这些依赖进一步依赖的软件包

`package.xml` 中常见的依赖标签包括：
| 标签 || 教程中的含义 |
|-|-|
|buildtool_depend |	构建工具依赖，如 catkin|
|build_depend || 构建时需要的依赖 |
| exec_depend |	运行时需要的依赖 |
| test_depend || 测试时需要的依赖 |

#### 1.2 catkin工作空间覆盖（Overlay）

- **Underlay工作空间**：已有的 ROS 安装，例如 /opt/ros/noetic
- **Overlay工作空间**：自己正在开发、修改的软件包

可以通过更新加载环境覆盖原有工作空间，但覆盖会有概率发生一些error情况

## 三、 LLM与Agent

### 0. 概念

**LLM（Large Language Model，大语言模型）**是一种通过大量数据训练、能够根据上下文生成文本等内容的模型

- **模型**：根据输入和上下文生成输出
- **Token**：模型处理文本时使用的基本单位，不完全等同于汉字或英文单词
- **上下文窗口**：模型一次处理对话或其他输入时能够容纳的上下文范围
- **Prompt**：提供给模型的指令、背景信息和任务要求
- **API**：Application Programming Interface（应用程序编程接口），程序与模型服务进行通信的一种接口

### 1. 常用术语

1. **`README.md`**：给开发者，也可给ai阅读的项目说明（介绍项目、使用方法和基本信息等等）

2. **`AGENTS.md`**：给编程 Agent 阅读的项目说明，内容包括启动方式（启动命令、环境变量...）、测试方式（测试命令、覆盖率...）、代码风格规范（格式、命名、Lint...）、权限（规定不能改动的文件/目录）、提交信息规范（格式、前缀、粒度...）、架构说明、注意事项等。`AGENTS.md`也可分级（总规则与子规则）

3. **Skill**：给 Agent 的工具栏，一个 Skill（文件夹）通常包含`Skill.md`、脚本、模板/素材等资源。例如 CMake Skill 可以专门为 Agent 提供 CMake 相关工作流程和知识

4. **Memory**：Agent 的长期记忆库，该系统建议项目必须遵守的规则放在项目说明文件。存储用户长期偏好或需要跨会话使用的经验，可以交给合适的 Memory 系统

5.  **MCP**：Model Context Protocol（模型上下文协议），它是一种开放协议，用于连接 AI 应用与外部工具、数据源和上下文（统一接口）。官方架构采用 Host、Client、Server 等角色。MCP 可提供 Tools（工具）：可由 AI 应用请求调用的操作；Resources（资源）：可供应用读取的上下文数据；Prompts（提示模板）：服务端提供的可复用提示模板

6.  **A2A**：解决不同 Agent 如何协作的问题。官方 A2A 文档明确将 MCP 与 A2A 定位为互补协议

7.  **Subagents**：子 Agent，指的是主 Agent 将部分工作交给其他专门的 Agent 执行，再根据返回结果继续工作。具体机制取决于用户使用的编程助手或 Agent 框架，并不是所有平台都有完全相同的子 Agent 功能

### 2.其他

#### 2.1 dsh-ops 插件原理

1. 有界紧凑输出（Bounded, compact output）
- 每个工具结果有硬上限：Native 文本 64 KiB、worker 响应 96/128 KiB、文件/发现 schema ≤400 token
- 读文件用**行号正文**（`N|内容`）+ 可逆 JSON 转义，防伪造行号/文件名；共用 `Base` 目录省去重复路径
- **我的应用**：读大文件只读需要的行（用 offset/limit），不全量 dump；输出尽量紧凑、不重复粘贴已知内容

2. 渐进式披露（Progressive disclosure）
- 只常驻最小工具面（`read_files` + `discover_tools`，仅 242 token），高级工具（grep/glob/编辑/PDF…）**按需发现**，一次最多选 3 个
- **我的应用**：先给「索引/总览/课程地图」，细节等用到再展开，不一次性灌满上下文

3. 上下文读取去重（Context-aware read dedup）
- 同一文件同版本已成功读进上下文后，再次读只返回「未改变」引用，不重复返回全文。
- **我的应用**：不重读刚改过的文件（Edit/Write 已保证状态）；不重复读已知内容

4. 批量操作（Batching）
- 一次读 1–32 个文件、一次编 1–100 段修改，减少进程启动。
- **我的应用**：并行读多个相关文件（一条消息多个工具调用）；批量处理 77 集时逐条下载/抽帧

5. 诚实反馈 + CAS 乐观锁
- 工具失败作为**返回值**（status/error.code/effectState），不抛异常；编辑用 `expectedRevision`(SHA-256) 做乐观锁，版本过期/歧义/重叠**全部拒绝**
- **绝不把「部分完成/提前停止」报成「完成」**；必须检查两层 complete（顶层 + 每文件）
- **我的应用**：不假装完成某项进程

6. 结果预算 / 生命周期
- 单次执行 30s 上限；FIFO 队列（2 并发 + 8 等待）；溢出返回 CALL_LIMIT 且无副作用；引用绑定会话/权限/根目录，会话销毁即过期
- **我的应用**：控制单次规模，分批推进；长时间任务放后台、分轮做

7. 最小侵入 + 权限保护
- 只加 `ops_*` 工具，不改宿主逻辑、不重写共享工具注册表；每次调用检查权限与取消；拒绝符号链接；只读调用不产生审计/缓存文件
- **我的应用**：做项目/脚本不破坏现有结构；下载物与脚本分目录；不越权

8. 工程硬规则
- 单文件 ≤1000 行；新行为必须有「改回去会失败的断言」；不编辑生成物；新依赖要理由 + 版本已发布 7 天

#### 2.2 mattpocock skills 的方法论

| 技能 | 作用 |
|-|-|
| **grilling / grill-me** | 动手前对计划/决定**持续追问**，直到问题被彻底想清（压力测试自己的思路） |
| **domain-modeling** | 先**建领域模型**：统一术语、写 GLOSSARY.md、记 ADR（架构决策） |
| **to-spec / to-tickets** | 把讨论沉淀成 spec，再拆成带阻塞依赖的 ticket 列表 |
| **tdd** | 测试驱动开发：红→绿→重构 |
| **code-review** | 沿「是否符合编码标准」+「是否符合 spec」两条轴审查，并行子代理 |
| **diagnosing-bugs** | 棘手脚 bug 的诊断循环 |
| **research** | 对照高可信一手来源调研，结论存 Markdown |
| **wayfinder** | 把单个会话装不下的大活，规划成 issue 上的决策 ticket 地图，逐个解决 |
| **wizard** | 只有「人」能做的步骤（配密钥/CI 等）才生成交互向导，agent 自己能做的别用 |

## 四、 总结

初步进入 ROS ，基本了解 CMake、catkin、LLM与Agent使用
Agent的进阶使用需要一定的语言基础，任重道远

参考文献：  
[程序详细编译过程](https://zhuanlan.zhihu.com/p/476697014)  
[cmake基础](https://www.runoob.com/cmake/cmake-basic.html)  
[200行代码编写 Claude Code](https://www.mihaileric.com/The-Emperor-Has-No-Clothes/)  
[catkin教程](https://wiki.ros.org/catkin/Tutorials)