# 第二课：CMake 与 Catkin 基础、大模型使用入门

## 一、课程概述

本课程主要包括两部分：

1. **CMake 与 Catkin 基础**
   - 了解源代码生成可执行程序的过程。
   - 理解 Makefile、CMake 和 Catkin 的作用及关系。
   - 掌握 Catkin 工作空间和功能包的基本操作。
2. **大模型使用入门**
   - 理解大语言模型、Agent、Skill 和 `AGENTS.md` 等概念。
   - 学习如何利用大模型辅助学习与开发。
   - 强调负责任地使用 AI，保留独立思考和真实学习过程。

## 二、从源代码到可执行程序

计算机直接执行的是二进制机器指令，C++ 等高级语言编写的源代码需要经过编译器处理，才能转换为可执行程序。

基本流程如下：

```text
源代码
  ↓
预处理：展开头文件、宏定义等
  ↓
编译：将源代码转换为汇编代码
  ↓
汇编：将汇编代码转换为目标文件
  ↓
链接：合并目标文件及所需库，生成可执行程序
```

## 三、Makefile、CMake 与 Catkin

| 工具 | 主要作用 | 特点 |
|---|---|---|
| GCC/G++ | 编译 C/C++ 源代码 | 文件较多时，手动输入命令效率较低 |
| Makefile | 描述文件依赖、编译顺序和构建命令 | 可以通过 `make` 自动完成构建 |
| CMake | 根据配置文件生成 Makefile 或工程文件 | 跨平台，适合管理大型、复杂项目 |
| Catkin | ROS 使用的构建系统 | 在 CMake 基础上扩展，管理 ROS 工作空间和功能包 |

### 1. Makefile

Makefile 可以理解为一份“自动编译说明书”，用于告诉 `make`：

- 哪些文件需要编译；
- 文件之间存在哪些依赖；
- 按什么顺序编译；
- 需要执行哪些命令。

代码修改后，通常只需执行一次 `make`，即可重新生成目标程序。

### 2. CMake

随着项目文件和依赖增多，手动维护 Makefile 会越来越困难。CMake 通过较简洁的配置描述项目构建过程，并生成适用于当前平台的构建文件。

其主要优势包括：

- 简化大型项目的构建管理；
- 减少手动维护复杂 Makefile 的工作；
- 支持不同操作系统和编译器；
- 提升项目的跨平台能力与可扩展性。

ROS 使用 CMake 管理构建，并在此基础上扩展出 Catkin 构建系统。

## 四、Catkin 工作空间

### 1. 创建工作空间并构建

```bash
# 检查 Catkin 是否可用
catkin --help

# 创建工作空间及源码目录
mkdir -p ~/catkin_ws/src

# 初始化工作空间
cd ~/catkin_ws/src
catkin_init_workspace

# 返回工作空间根目录并构建
cd ~/catkin_ws
catkin build
```

### 2. 工作空间的主要目录

| 目录 | 作用 |
|---|---|
| `src` | 存放功能包源代码 |
| `build` | 存放构建过程中的中间文件和二进制文件，通常不需要直接修改 |
| `devel` | 存放开发过程中生成的可执行文件、库及环境配置文件 |

## 五、Catkin 功能包

功能包（Package）是 ROS 中组织代码和资源的基本单位。

### 1. 创建功能包

以下命令创建 `test_pkg`，并声明对 `std_msgs`、`rospy` 和 `roscpp` 的依赖：

```bash
cd ~/catkin_ws/src
catkin_create_pkg test_pkg std_msgs rospy roscpp
```

### 2. 常见功能包结构

```text
test_pkg/
├── CMakeLists.txt   # 功能包构建规则
├── package.xml      # 功能包信息及依赖声明
├── src/             # C++ 等源代码
├── include/         # C++ 头文件
├── scripts/         # 可执行脚本
├── msg/             # 自定义消息
├── srv/             # 自定义服务
├── models/          # 模型文件
├── urdf/            # URDF 机器人描述文件
└── launch/          # Launch 启动文件
```


## 补充--Docker的ROS_Noetic容器中完成talker-listener话题通信实验---踩坑与要点总结

### 流程
1. docker start ros_noetic --> docker exec -it ros_noetic bash
2. source /opt/ros/noetic/setup.bash
3. mkdir -p ~/catkin_ws/src --> cd ~/catkin_ws/src --> catkin_init_workspacecd ~/catkin_ws --> catkin build
4. cd ~/catkin_ws/src -->  catkin_create_pkg test_pkg roscpp rospy std_msgs
5. cd ~/catkin_ws/src/test_pkg/src --> nano talker.cpp
6. nano ~/catkin_ws/src/test_pkg/CMakeLists.txt
7. 新开一个窗口，剩下同理

### 遇到的问题
1. 在同一个终端先后启动 talker、listener，节点无法通信（节点需要同时在线）
2. 接收端出现 `????`，是终端中文编码乱码，消息本身传输正常
3. 重复启动 talker，触发节点重名警告，旧节点被自动关闭
4. 在容器内部执行 `docker stop`，提示命令不存在（docker 命令属于宿主机）

### 核心要点
- 通信三组件分开三个终端同时运行：`roscore`（ROS主节点）、talker（发布者）、listener（订阅者）
- ROS **节点名称必须唯一**
- 容器只负责编译、运行 ROS 程序；容器启停、查看等 docker 命令，只能在宿主机执行
- `docker stop` 暂停容器后，工作空间代码会保留，不会丢失

### 收尾流程
`Ctrl + C` 依次停止 ROS 节点 → `exit` 退出容器终端 → 回到宿主机按需执行 `docker stop ros_noetic` 暂停容器。





## 六、大语言模型基础

大语言模型（Large Language Model，LLM）是一种具有大量参数的人工智能模型。(Codex cli可以让大模型读取本地文件的原因)

从概率角度看，LLM 对“给定输入后可能出现什么输出”进行建模。推理时，模型根据输入生成的概率分布逐步产生输出。

需要注意：

- 大模型具有有限的**上下文窗口**，即一次能够处理的输入和输出长度存在上限；
- 网页对话通常需要用户手动复制代码或结果；
- 接入 Agent 软件后，模型可以借助工具操作本地文件和开发环境。(有权限，agents给LLM上了一整套工具)

## 七、Agent、Skill 与 AGENTS.md

### 1. Agent 软件

Agent 软件是一套让大模型能够：

- 调用工具；
- 与外部环境交互；
- 持续执行任务的系统。

例如，在本地安装 Agent 软件并接入大模型后，模型可以在授权范围内读取、创建和修改项目文件，从而减少手动复制粘贴，提高开发效率。

### 2. Skill

Skill（技能）是一组可复用的任务指令及配套资源，用于指导 Agent 完成特定类型的任务。

可选资源包括：

- 脚本；
- 参考文档；
- 模板；
- 特定任务的操作流程。

Agent 可以在需要时加载相应 Skill，并按照其中定义的流程工作。

### 3. AGENTS.md
- 和skill配套使用
- `AGENTS.md` 可以理解为“写给 AI 编程 Agent 的项目说明书”。

它通常用于说明：

- 项目背景与结构；
- 开发规范；
- 构建和测试方法；
- 工作指引与注意事项。

**交代该怎么干活，哪些不能碰，遵循什么规范**

它与面向人类开发者的 `README.md` 类似，但主要服务于 AI 编程 Agent。

## 八、使用大模型辅助学习

可以采用以下方法：

1. **收集资料**
   - 让 AI 搜索某个主题的最佳实践或优秀参考，并要求给出来源。
2. **制定个性化学习计划**
   - 告诉 AI 自己已经掌握的知识；
   - 让 AI 通过提问评估当前水平；
   - 根据评估结果设计针对性课程。
3. **用自己的话复述知识**
   - 学完一个课题后，自己先讲一遍，再让 AI 检查遗漏或错误。
4. **生成练习**
   - 让 AI 根据学习内容出练习题，通过练习检验理解程度。

## 九、使用大模型辅助开发

编写提示词时，不必堆砌过多复杂指令，否则可能造成上下文膨胀。可以重点组织以下内容：

- 清楚说明目标；
- 提供必要上下文；
- 明确重要约束；
- 规定验证标准；
- 允许 Agent 自主探索；
- 避免过度规定具体执行步骤。

其中，Skill 和 `AGENTS.md` 可以帮助沉淀项目背景、开发规范和 reusable 工作流程。

## 十、负责任地使用 AI

使用 AI 时，应当对团队和自己负责：

- 不盲目接受 AI 生成的代码
- 对关键内容进行验证和测试
- 理解自己提交或使用的代码
- 保留独立思考，不用 AI 代替真实学习过程

## 十一、课后任务

1. 补充资料：
   - [Catkin Tutorials](https://wiki.ros.org/catkin/Tutorials)
   - [知乎补充资料](https://zhuanlan.zhihu.com/p/476697014)
   - [CMake 基础教程](https://www.runoob.com/cmake/cmake-basic.html)
   - [The Emperor Has No Clothes](https://www.mihaileric.com/The-Emperor-Has-No-Clothes/)
2. 在 `notes` 文件夹中创建 `cmake_study.md`
3. 使用规范的 Markdown 语法记录 CMake、Catkin 及补充资料的学习笔记
4. 可以记录使用大模型辅助理解工具的交互过程、思路，以及对 AI 辅助学习的思考
5. 不应直接让 AI 代写笔记，应保留自己的理解与真实学习过程



# 11.1-程序详细编译过程-学习总结与思考(Have understood)

> 资料来源：[程序详细编译过程（预处理、编译、汇编、链接）](https://zhuanlan.zhihu.com/p/476697014)

## 一、程序编译的整体流程

计算机只能直接执行机器语言指令，因此 C/C++ 等高级语言编写的源代码，需要经过编译系统转换，才能成为可执行程序。

以 `hello.c` 为例：

```c
#include <stdio.h>

int main()
{
    printf("hello, world\n");
    return 0;
}
```

使用 GCC 编译：

```
bash
gcc -o hello hello.c

```

GCC 会自动完成四个阶段：

```text
hello.c
   ↓ 预处理
hello.i
   ↓ 编译
hello.s
   ↓ 汇编
hello.o
   ↓ 链接
hello
```

| 阶段 | 执行程序 | 主要工作 | 典型文件 |
|---|---|---|---|
| 预处理 | 预处理器 | 展开宏、处理头文件和条件编译 | `.i` |
| 编译 | 编译器 | 分析、优化并生成汇编代码 | `.s` |
| 汇编 | 汇编器 | 将汇编代码转换为机器代码 | `.o` |
| 链接 | 链接器 | 合并目标文件和库，生成可执行文件 | 可执行文件 |

## 二、预处理阶段

预处理器主要完成以下工作：

1. 展开 `#define` 定义的宏。
2. 处理 `#if`、`#ifdef`、`#elif`、`#else`、`#endif` 等条件编译指令。
3. 处理 `#include`，将头文件内容插入对应位置。
4. 删除源代码中的注释。
5. 添加行号和文件标识，便于编译器报告错误位置。
6. 保留 `#pragma` 等供编译器使用的指令。

只执行预处理：

```
bash
gcc -E hello.c -o hello.i

```

预处理后的文件仍然是文本文件，但头文件和宏已经被展开，内容通常比原始源代码多。

## 三、编译阶段

编译器对预处理后的文件进行：

- 词法分析
- 语法分析
- 语义分析
- 代码优化
- 汇编代码生成

执行命令：

```
bash
gcc -S hello.i -o hello.s

```

生成的 `hello.s` 是汇编语言文件,但仍然是文本文件，但表达方式已经接近底层机器指令

## 四、汇编阶段

汇编器将汇编代码转换为机器语言，生成可重定位目标文件：

```
bash
gcc -c hello.s -o hello.o

```

目标文件已经包含机器代码，但通常不能直接运行，因为：

- 程序可能由多个源文件组成
- 当前文件可能调用其他文件中的函数
- 程序可能依赖静态库或动态库
- 部分符号的最终地址尚未确定

这些问题需要在 **链接**阶段处理

## 五、链接阶段

链接器以可重定位目标文件和库为输入，生成可加载、可运行的可执行目标文件。

```
bash
gcc hello.o -o hello

```

链接的核心作用包括：

1. 合并不同目标文件中的代码和数据。
2. 将符号引用与符号定义对应起来。
3. 为代码和数据分配合适的地址。
4. 修正需要重定位的指令和数据。
5. 支持分离编译和模块化开发。

## 六、可重定位目标文件

以 Linux x86-64 系统的 ELF-64 格式为例，介绍了目标文件中的常见节

| 节 | 主要内容 |
|---|---|
| `.text` | 已编译程序的机器代码 |
| `.rodata` | 只读数据，例如格式字符串 |
| `.data` | 已初始化的全局变量和静态变量 |
| `.bss` | 未初始化或初始化为零的全局、静态变量 |
| `.symtab` | 函数和全局变量的符号表 |
| `.rel.text` | `.text` 中需要重定位的位置 |
| `.rel.data` | 全局数据相关的重定位信息 |
| `.debug` | 调试符号信息 |
| `.line` | 源代码行号与机器指令的对应关系 |
| `.strtab` | 符号表和调试信息使用的字符串 |

`.bss` 在目标文件中通常只占位，不保存大量实际的零数据；程序运行时，系统会为这些变量分配内存并初始化。

## 七、静态链接

静态链接在生成可执行文件时完成主要链接工作，所需库代码会被复制到可执行文件中。

静态链接的主要过程包括：

### 1. 空间和地址分配

链接器扫描输入目标文件，收集各段的长度、属性和位置，并合并相同类型的段

同时，链接器收集符号定义和符号引用，建立全局符号表，为输出文件中的段和符号分配地址

### 2. 符号解析

符号解析的目标是把每个符号引用与对应的符号定义关联起来。

三类符号：

- 当前模块定义、可被其他模块引用----全局符号；
- 其他模块定义、被当前模块引用----外部符号；
- 只在当前模块内部使用----局部符号

如果引用的符号找不到定义，链接器就会报告未定义引用等错误

### 3. 重定位

合并段并确定地址后，链接器需要修改目标文件中的相关指令和数据，使其使用正确的运行时地址

## 八、动态链接与共享库

动态链接不会在生成可执行文件时把所有共享库代码复制进去，而是在程序**加载**或**运行**时完成部分链接工作

创建共享库的示例：

```
bash
gcc -shared -fpic -o libvector.so addvec.c multvec.c

```

其中：

- `-shared`：生成共享目标文件
- `-fpic`：生成位置无关代码

使用共享库构建程序：

```
bash
gcc -o prog21 main2.c ./libvector.so

```

程序加载时，动态链接器会：

1. 将所需共享库的代码和数据重定位到内存中
2. 修正程序对这些库中函数和数据的引用
3. 完成必要的运行时链接
4. 将控制权交给应用程序

## 九、静态链接与动态链接对比

| 对比项 | 静态链接 | 动态链接 |
|---|---|---|
| 主要链接时机 | 构建程序时 | 加载或运行时 |
| 库代码是否复制进程序 | 是 | 通常否 |
| 可执行文件体积 | 通常较大 | 通常较小 |
| 库共享 | 不便于多个程序共享同一份代码 | 多个程序可共享库 |
| 库更新 | 通常需要重新链接 | 接口兼容时可替换共享库 |
| 运行依赖 | 对静态库文件的运行依赖较少 | 需要正确找到共享库 |

## 十、总结

预处理、编译、汇编和链接并不是彼此孤立的步骤，而是共同完成从高级语言到可执行程序的转换

其中：

- 预处理负责处理源代码文本
- 编译负责分析并生成汇编代码
- 汇编负责生成目标文件
- 链接负责解决模块之间的引用和地址问题

## 十二、重难点--虚拟内存与Linux进程虚拟地址空间
> B站

### 一、基础概念区分
- **虚拟内存（虚拟存储器）**：一套由硬件 + 操作系统共同实现的**内存管理体系**，不是真实的内存条
  作用：给每个程序制造假象——程序仿佛独占整块内存

- **虚拟地址空间**：虚拟内存机制给**每一个进程单独分配的一张内存布局图纸**，进程看到的地址都是虚拟地址，不是物理内存真实地址。

> 类比：
> 虚拟内存 = 图书馆借阅管理制度
> 虚拟地址空间 = 分配给你个人的阅览清单

### 二、虚拟内存的3个核心作用
1. **高效利用物理内存（缓存机制）**
把磁盘当成内存的后备。只有程序当前正在使用的数据放在物理内存；暂时不用的数据换到磁盘交换空间，需要的时候再调入内存。突破物理内存大小限制。

2. **统一地址，简化程序开发**
所有进程的地址布局格式一致，编译器、链接器不需要关心物理内存实际排布。

3. **进程隔离、内存保护**
每个进程拥有独立虚拟地址空间，进程之间不能随便访问对方内存。一个程序崩溃，不会连带其他程序一起崩溃。

### 三、Linux 进程虚拟地址空间（从低地址 → 高地址）
> 重点：这只是进程**自己以为**的内存布局，不等于真实物理内存排布

1. **只读代码和常量段（.text）**
存放编译好的程序指令、字符串常量。**只读不可修改**，来自可执行文件

2. **读写数据段（.data / .bss）**
存放全局变量、静态变量，程序启动加载时就分配，运行全程可读可写

3. **堆（heap）**
`malloc()` / `new` 动态申请的内存在这里；**向高地址方向增长**，需要程序员手动申请、手动释放

4. **共享库映射区**
系统动态库（比如 `libc`，`printf` 就在里面）映射到这里。多个进程可以共享同一份库，节约内存

5. **用户栈（stack）**
局部变量、函数参数、函数返回地址保存在栈；**向低地址方向增长**。函数调用自动入栈，函数结束自动回收；递归太深容易栈溢出

6. **内核虚拟内存区（最高地址）**
用户代码**不能直接访问**，属于操作系统内核。只能通过系统调用让内核协助完成操作，是安全边界

## 四、一句话总结
- 操作系统依靠虚拟内存这套机制，给每个进程分配独立的虚拟地址空间
- 程序操作虚拟地址，硬件+OS自动翻译成物理地址
- 同时支持内存换**页**、进程隔离保护

## 五、简单记忆
- 虚拟内存：软硬件配合的存储管理体系，实现内存虚拟化
- 虚拟地址空间：单个进程独立的虚拟内存布局
- 分段：只读代码段、数据段、堆（malloc，向上增长）、共享库、栈（局部变量，向下增长）、内核区（用户不可访问）
- 三大功能：页面调度充分利用内存、统一地址简化编译、进程内存隔离保护



# 11.2-CMake 基础学习笔记(AI:person = 3:7)

> 资料来源：[CMake 基础——菜鸟教程](https://www.runoob.com/cmake/cmake-basic.html)

## 一、CMake 是什么

CMake 是一个跨平台的构建系统生成工具，主要用于管理 C/C++ 项目的配置和构建过程。

CMake 本身不是编译器。它读取 `CMakeLists.txt`，生成适用于当前平台的构建文件，例如：

- Linux 下的 Makefile；
- Ninja 构建文件；
- Windows 下的 Visual Studio 工程；
- 其他 IDE 或构建工具所需的工程文件。

基本工作流程：

```text
编写 CMakeLists.txt
        ↓
运行 CMake 配置项目
        ↓
生成平台相关构建文件
        ↓
调用编译器和构建工具完成编译
```

## 二、CMakeLists.txt

`CMakeLists.txt` 是 CMake 项目的核心配置文件，用于描述：

- 项目名称和语言；
- 源文件；
- 构建目标；
- 头文件路径；
- 外部依赖；
- 编译选项；
- 安装规则。

CMake 指令的基本形式：

```cmake
命令名(参数列表)
```

## 三、常用指令

### 1. 指定最低版本

```cmake
cmake_minimum_required(VERSION 3.10)
```

该指令通常放在 `CMakeLists.txt` 开头，用于指定项目要求的最低 CMake 版本。

### 2. 定义项目

```cmake
project(MyProject VERSION 1.0.0 LANGUAGES CXX)
```

常见语言参数：

- `C`：C 语言；
- `CXX`：C++。

### 3. 设置 C++ 标准

```
cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

```

`CMAKE_CXX_STANDARD_REQUIRED ON` 表示编译器必须支持指定标准，不能静默改用较低标准。

### 4. 添加可执行文件

```
cmake
add_executable(MyApp main.cpp utils.cpp)

```

该指令定义一个名为 `MyApp` 的可执行文件目标。

### 5. 添加库

```cmake
add_library(MyLib STATIC library.cpp)
```

常见库类型：

| 类型 | 含义 |
|---|---|
| `STATIC` | 静态库 |
| `SHARED` | 动态共享库 |
| `MODULE` | 可由程序动态加载的模块 |
| `INTERFACE` | 不产生实体库文件，用于传递使用要求 |

### 6. 链接库

```cmake
target_link_libraries(MyApp PRIVATE MyLib)
```

该指令表示 `MyApp` 依赖 `MyLib`，构建时需要链接该库。

### 7. 设置头文件路径

```cmake
target_include_directories(
    MyLib
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
```

相比全局设置路径，将路径绑定到目标更有利于表达依赖关系。

### 8. 设置安装规则

```
cmake
install(TARGETS MyApp RUNTIME DESTINATION bin)
install(TARGETS MyLib ARCHIVE DESTINATION lib)

```

这些规则决定执行安装时，可执行文件和库分别被安装到什么位置。

## 四、完整示例

假设项目结构如下：

```text
MyProject/
├── CMakeLists.txt
├── include/
│   └── core.h
└── src/
    ├── main.cpp
    └── core.cpp
```

`CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.10)

project(MyProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(MyLib STATIC
    src/core.cpp
)

target_include_directories(MyLib
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_executable(MyApp
    src/main.cpp
)

target_link_libraries(MyApp
    PRIVATE MyLib
)

install(TARGETS MyApp RUNTIME DESTINATION bin)
install(TARGETS MyLib ARCHIVE DESTINATION lib)
```

## 五、配置、构建和安装

推荐使用外部构建，将生成文件放入独立的 `build` 目录：

```
bash
mkdir build
cd build

```

配置项目：

```bash
cmake ..
```

构建项目：

```
bash
cmake --build .
```

安装项目：

```bash
cmake --install .
```

外部构建的优点：

- 不污染源码目录；
- 中间文件集中管理；
- 可以同时保留多套构建配置；
- 需要重新配置时，可以删除整个 `build` 目录。

## 六、变量和列表

定义普通变量：

```
cmake
set(MY_APP_NAME "cmake-demo")

```

引用变量：

```
cmake
message(STATUS "项目名称：${MY_APP_NAME}")

```

定义源文件列表：

```
cmake
set(SOURCE_FILES
    main.cpp
    utils.cpp
    helper.cpp
)

add_executable(MyApp ${SOURCE_FILES})

```

遍历列表：

```
cmake
foreach(FILE ${SOURCE_FILES})
    message(STATUS "源文件：${FILE}")
endforeach()

```

## 七、缓存变量

缓存变量保存在 `CMakeCache.txt` 中，可以在多次 CMake 配置之间保留。

```
cmake
set(
    ENABLE_LOGGING
    ON
    CACHE BOOL
    "是否启用日志功能"
)

```

常见缓存类型：

- `BOOL`；
- `STRING`；
- `PATH`；
- `FILEPATH`。

通过命令行修改缓存变量：

```
bash
cmake .. -DENABLE_LOGGING=OFF

```

也可以设置安装路径：

```
bash
cmake .. -DCMAKE_INSTALL_PREFIX=/opt/my_app

```

需要注意：如果缓存中已经存在某个变量，再次修改 `CMakeLists.txt` 中的默认值，不一定会覆盖原来的缓存值。配置异常时，应检查 `CMakeCache.txt`，必要时清理构建目录后重新配置。

## 八、条件语句

CMake 可以根据构建类型或功能开关选择不同配置：

```
cmake
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "当前为 Debug 构建")
else()
    message(STATUS "当前不是 Debug 构建")
endif()

```

使用功能开关：

```
cmake
if(ENABLE_LOGGING)
    message(STATUS "日志功能已启用")
endif()

```

## 九、PUBLIC、PRIVATE 和 INTERFACE

这三个关键字用于控制目标的使用要求如何向依赖者传播。

| 关键字 | 当前目标使用 | 依赖者使用 | 典型场景 |
|---|---:|---:|---|
| `PRIVATE` | 是 | 否 | 仅当前目标内部需要 |
| `PUBLIC` | 是 | 是 | 当前目标和调用者都需要 |
| `INTERFACE` | 否 | 是 | 只向调用者传递要求 |

简单记忆：

```
text
自己需要，别人不需要 → PRIVATE
自己需要，别人也需要 → PUBLIC
自己不需要，别人需要 → INTERFACE

```

纯头文件库示例：

```
cmake
add_library(HeaderOnlyLib INTERFACE)

target_include_directories(
    HeaderOnlyLib
    INTERFACE ${PROJECT_SOURCE_DIR}/include
)

```

## 十、目标级指令与全局指令

推荐优先使用 `target_xxx` 系列指令

例如：

```
cmake
target_include_directories(MyApp PRIVATE include)
target_link_libraries(MyApp PRIVATE MyLib)

```

相比大量使用全局的：

```
cmake
include_directories(...)
link_directories(...)

```

目标级指令的优点是：

- 作用范围明确；
- 依赖关系清晰；
- 不容易影响无关目标；
- 更适合大型项目；
- 便于维护依赖的传播规则。

## 十一、查找外部依赖

使用 `find_package()` 查找外部库：

```
cmake
find_package(Boost REQUIRED)

```

`REQUIRED` 表示找不到依赖时停止配置。

也可以指定最低版本：

```
cmake
find_package(Boost 1.70 REQUIRED)

```

使用导入目标：

```
cmake
add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE Boost::Boost)

```

现代 CMake 推荐使用导入目标，因为它可以自动传递头文件路径、库文件和其他使用要求，减少手动拼接路径造成的错误。

## 十二、自定义命令

可以在目标构建完成后执行额外命令：

```
cmake
add_custom_command(
    TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "构建完成"
    COMMENT "输出构建完成信息"
)

```

`POST_BUILD` 表示在目标构建完成后执行

## 十三、学习重点

1. CMake 负责生成构建文件，不直接替代编译器
2. `CMakeLists.txt` 的核心是描述目标及其依赖
3. 推荐使用外部构建
4. 优先使用目标级指令
5. 理解 `PUBLIC`、`PRIVATE` 和 `INTERFACE` 的传播规则
6. 注意缓存变量不会简单地随默认值修改而自动更新
7. 使用外部库时，优先选择 `find_package()` 和导入目标



# 11.3-《The Emperor Has No Clothes》文章核心讲解(Have understood)

> 资料来源：[The Emperor Has No Clothes: How to Code Claude Code in 200 Lines of Code](https://www.mihaileric.com/The-Emperor-Has-No-Clothes/)

## 一、文章要解决的问题

AI 编程助手能够读取文件、修改代码并连续完成任务，看起来十分复杂。文章通过一个约 200 行的 Python 示例说明：Coding Agent 的核心架构其实可以拆分为几个简单的组成部分。

这些部分包括：

- 大语言模型；
- 对话上下文；
- 工具定义；
- 工具调用解析；
- 本地工具执行；
- 循环控制。

## 二、核心心智模型

Coding Agent 可以理解为：

```text
一个大语言模型
+
一套可调用的工具
+
一个持续运行的交互循环
```

其工作过程如下：

1. 用户提出任务(例如：新建一个 hello.py 文件）
2. LLM 根据任务和上下文决定是否要使用工具
3. LLM 输出结构化的工具调用请求
4. **本地 Python 代码**执行工具指令，真正完成磁盘上的文件读写操作
5. 将执行结果（文件内容、目录列表等）返回给大模型
6. LLM 基于结果继续思考，要么继续调用工具，要么给出最终回答

流程图：

```text
用户输入
   ↓
调用 LLM
   ↓
是否请求工具？
   ├─ 否 → 输出回答
   └─ 是 → 执行本地工具
              ↓
          返回工具结果
              ↓
          再次调用 LLM
```

最重要的认识是：**真正操作文件的是本地代码，LLM 只负责“决策何时调用、调用哪个工具**

## 三、最小工具集合

文章实现了三个基础工具。

### 1. 读取文件

```text
read_file(filename)
```

作用：读取文件内容，让 LLM 查看现有代码

有了读取能力，模型才能根据项目中的真实代码作出判断。

### 2. 列出文件

```text
list_files(path)
```

作用：列出文件夹结构，让 LLM 理解项目目录

### 3. 编辑文件

```text
edit_file(path, old_str, new_str)
```

文章中的编辑工具支持两种情况：

- 当 `old_str` 为空时，创建或覆盖文件；
- 当 `old_str` 非空时，查找旧文本并替换为新文本。

虽然这种实现比较简单，但已经能够完成创建文件和局部修改代码的任务。

## 四、工具注册表

文章使用注册表保存工具名称与函数之间的对应关系：

```python
TOOL_REGISTRY = {
    "read_file": read_file_tool,
    "list_files": list_files_tool,
    "edit_file": edit_file_tool
}
```

注册表的作用是：一个字典，建立“工具名称 → Python 函数”的映射，LLM 指定工具名后，程序从字典中匹配并执行对应函数

## 五、向模型描述工具

模型必须知道：

- 有哪些工具；
- 每个工具有什么作用；
- 参数名称和类型是什么；
- 应该使用什么格式发起调用。

文章根据函数签名和文档字符串生成工具说明，并将这些说明放入系统提示词。

因此，工具文档字符串不仅是写给开发者看的，也会影响模型如何选择和使用工具。

良好的工具描述应该：

- 明确工具用途；
- 明确参数含义；
- 说明输入和输出；
- 避免含糊或相互冲突；
- 对重要限制作出说明。

## 六、工具调用格式

文章约定模型使用单行文本请求工具：

```text
tool: TOOL_NAME({"argument": "value"})
```

宿主程序需要解析：

1. 工具名称；
2. JSON 参数；
3. 参数是否合法；
4. 工具是否存在。

解析成功后，宿主程序调用工具注册表中的对应函数。

工具结果再以结构化形式加入对话，例如：

```text
tool_result({"path": "hello.py", "action": "created_file"})
```

模型根据该结果决定下一步操作。

## 七、双层 Agent 循环

`run_coding_agent_loop`（核心灵魂),Agent 包含两层循环

### 外层循环

负责：

- 接收用户输入；
- 保存对话历史；

### 内层循环

负责：

1. 调用 LLM
2. 检查回答中是否包含工具调用
3. 如果没有工具调用，输出回答并结束本轮
4. 如果存在工具调用，执行工具
5. 将工具结果加入上下文
6. 再次调用 LLM

内层循环使 Agent 能够连续完成多个步骤，例如：

```text
读取 hello.py
    ↓
理解现有代码
    ↓
编辑 hello.py
    ↓
确认修改结果
    ↓
向用户报告
```

## 八、简单 Agent 与生产级 Agent 的区别

文章中的示例用于解释核心原理，生产级agent更NB：

| 能力 | 作用 |
|---|---|
| 更完善的错误处理 | 处理文件不存在、参数错误和工具异常 |
| 流式输出 | 改善长时间任务中的交互体验 |
| 上下文管理 | 处理长文件和长对话 |
| 代码搜索 | 快速定位相关定义和引用 |
| Shell 工具 | 执行构建、测试和其他开发命令 |
| 网络搜索 | 获取外部资料 |
| 审批机制 | 控制删除、覆盖等高风险操作 |
| 更可靠的编辑方式 | 处理复杂修改和匹配失败 |

这些功能会提高实用性，但并没有改变“模型请求工具—宿主执行—结果返回模型”的基本架构。

## 九、安全性思考

简化示例没有完整处理生产环境中的安全问题。实际使用 Coding Agent 时，需要考虑：

- 限制 Agent 可访问的目录；
- 限制可调用的工具；
- 对删除和覆盖操作进行确认；
- 避免执行未经审查的高风险命令；
- 不把密钥等敏感信息写入提示词；
- 检查代码修改差异；
- 使用测试和构建验证修改；
- 防止工具结果被误认为任务已经成功完成。

工具调用成功只说明某个操作被执行，不代表整个任务一定正确。

## 十、总结

文章把 Coding Agent 从“magin”拆解成可以理解的程序结构。

其本质是：

```

LLM 是负责大脑，本地代码是手脚，通过工具调用循环，让 AI 间接操作本地项目文件

```



# 11.4-Catkin 官方教程学习笔记(AI:persion = 6:4)

> 资料来源：[ROS Wiki：Catkin Tutorials](https://wiki.ros.org/catkin/Tutorials)

## 一、Catkin 是什么

Catkin 是 ROS 1 使用的构建系统，以 CMake 为基础，用于组织、构建和管理 ROS 功能包

Catkin 主要涉及：

- 工作空间；
- 功能包；
- 构建依赖；
- 运行依赖；
- 环境设置；
- 工作空间覆盖。

官方教程目录主要包括：

1. 创建 Catkin 工作空间；
2. 创建 ROS 功能包；
3. 构建和使用工作空间中的功能包；
4. 使用工作空间进行环境覆盖。

## 二、创建工作空间

首先加载 ROS 环境。以下以 Noetic 为例，实际使用时应替换为本机 ROS 发行版：

```bash
source /opt/ros/noetic/setup.bash
```

创建工作空间：

```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws
catkin_make
```

首次运行 `catkin_make` 后：

- `src` 中会出现顶层 `CMakeLists.txt`；
- 工作空间中会出现 `build` 目录；
- 工作空间中会出现 `devel` 目录。

加载当前工作空间的环境：

```bash
source ~/catkin_ws/devel/setup.bash
```

检查环境是否生效：

```bash
echo $ROS_PACKAGE_PATH
```

如果输出包含当前工作空间的 `src` 路径，说明该工作空间已经加入 ROS 的包搜索环境。

## 三、工作空间目录

典型结构：

```text
catkin_ws/
├── src/
├── build/
├── devel/
└── install/
```

| 目录 | 名称 | 作用 |
|---|---|---|
| `src` | Source Space | 存放功能包源代码 |
| `build` | Build Space | 存放 CMake 配置和编译中间文件 |
| `devel` | Development Space | 存放开发时生成的程序、库和环境脚本 |
| `install` | Install Space | 存放安装后的文件，执行安装目标后生成 |

通常主要修改 `src` 中的内容，不应随意修改 `build` 中自动生成的文件。

## 四、Catkin 功能包的组成

一个基本 Catkin 功能包至少需要：

```text
my_package/
├── CMakeLists.txt
└── package.xml
```

要求包括：

1. 包含符合 Catkin 要求的 `package.xml`。
2. 包含使用 Catkin 的 `CMakeLists.txt`。
3. 每个功能包拥有自己的独立目录。
4. 不应把多个功能包放在同一个目录中。

两个核心文件的分工：

| 文件 | 作用 |
|---|---|
| `package.xml` | 记录包名、版本、描述、维护者、许可证和依赖 |
| `CMakeLists.txt` | 描述源代码、构建目标、消息生成和链接规则 |

## 五、功能包在工作空间中的位置

推荐将功能包放入工作空间的 `src` 目录：

```text
catkin_ws/
└── src/
    ├── CMakeLists.txt
    ├── package_1/
    │   ├── CMakeLists.txt
    │   └── package.xml
    └── package_2/
        ├── CMakeLists.txt
        └── package.xml
```

构建时，Catkin 会发现源码空间中的功能包，并统一组织构建。

## 六、创建功能包

进入源码空间：

```bash
cd ~/catkin_ws/src
```

创建功能包：

```bash
catkin_create_pkg beginner_tutorials std_msgs rospy roscpp
```

其中：

- `beginner_tutorials` 是功能包名称；
- `std_msgs`、`rospy`、`roscpp` 是创建时声明的依赖。

创建完成后，返回工作空间根目录构建：

```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

## 七、package.xml 的主要内容

### 1. 描述

```xml
<description>The beginner_tutorials package</description>
```

描述应简明说明功能包的用途。

### 2. 维护者

```xml
<maintainer email="user@example.com">User Name</maintainer>
```

至少需要一个维护者，方便其他人联系功能包负责人。

### 3. 许可证

```xml
<license>BSD</license>
```

常见开源许可证包括 BSD、MIT、GPL、LGPL 等，应根据项目实际情况填写。

### 4. 依赖

常见依赖标签：

| 标签 | 作用 |
|---|---|
| `build_depend` | 编译当前功能包所需的依赖 |
| `buildtool_depend` | 构建工具依赖 |
| `exec_depend` | 运行时依赖 |
| `test_depend` | 测试时所需依赖 |

正确区分构建依赖和运行依赖，有助于保证功能包在其他环境中能够正常编译和运行。

## 八、查看功能包依赖

查看直接依赖：

```bash
rospack depends1 beginner_tutorials
```

查看递归依赖：

```bash
rospack depends beginner_tutorials
```

依赖分为：

- **直接依赖**：当前功能包直接声明的依赖；
- **间接依赖**：直接依赖继续引入的其他依赖。

递归依赖可能很多，但构建系统会负责处理这些依赖关系。

## 九、构建工作空间

官方教程中的典型工作流：

```bash
cd ~/catkin_ws/src/beginner_tutorials
# 修改源代码或 CMakeLists.txt

cd ~/catkin_ws
catkin_make -DCMAKE_BUILD_TYPE=Release
```

常见构建类型：

- `Debug`：保留调试信息，便于排查问题；
- `Release`：以运行性能为重点；
- `RelWithDebInfo`：同时进行优化并保留调试信息。

构建过程中：

- 源文件仍保留在 `src`；
- 中间文件放入 `build`；
- 生成的可执行文件、库和生成代码放入 `devel`；
- 环境设置脚本也会生成在 `devel` 中。

## 十、安装功能包

可以执行安装目标：

```bash
cd ~/catkin_ws
catkin_make install
```

执行后会生成 `install` 空间，其中同样包含环境设置脚本。

加载安装空间环境：

```bash
source ~/catkin_ws/install/setup.bash
```

## 十一、不使用 catkin_make 的构建过程

官方教程还展示了底层过程，以说明 `catkin_make` 实际做了什么。

### 1. 创建构建目录

```bash
mkdir ~/catkin_ws/build
cd ~/catkin_ws/build
```

### 2. 配置项目

```bash
cmake ../src
```

在以下情况下通常需要重新配置：

- 添加或删除功能包；
- 修改构建文件；
- 修改 CMake 或 Catkin 构建设置。

### 3. 构建

```bash
make
```

Catkin 会把工作空间中的多个功能包组织起来，像一个较大的 CMake 项目一样构建。

### 4. 指定目录

可以指定开发空间：

```bash
cmake ../src -DCATKIN_DEVEL_PREFIX=../devel
```

也可以指定安装目录：

```bash
cmake ../src -DCMAKE_INSTALL_PREFIX=../install
```

## 十二、catkin_make 的局限

官方教程指出，`catkin_make` 使用同一环境执行相关构建命令，在某些测试场景中可能出现环境路径尚未准备好的情况。

教程给出的处理方式之一是先构建并加载环境，再运行测试：

```bash
catkin_make
source devel/setup.bash
catkin_make run_tests
```

对于需要按功能包隔离构建的场景，教程还介绍了 `catkin_make_isolated`。

这说明构建工具并不是完全屏蔽环境问题的，出现测试或依赖异常时，还需要检查当前 Shell 的环境设置。

## 十三、工作空间覆盖

Overlay 指在已有 ROS 环境之上构建和使用本地源码版本，使本地功能包优先于系统安装版本。

例如：

```text
本地 catkin_ws
      ↓ 覆盖
/opt/ros/noetic
```

基本流程：

```bash
source /opt/ros/noetic/setup.bash

cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

其用途包括：

- 修改已有功能包的源代码；
- 测试本地版本；
- 避免直接修改系统安装目录；
- 在多个工作空间之间组织不同项目。

## 十四、环境覆盖的注意事项

1. `source` 只对当前 Shell 及其后续环境生效。
2. 新开终端后，可能需要重新加载环境。
3. 多个工作空间叠加时，加载顺序会影响优先使用的功能包。
4. 可以通过 `ROS_PACKAGE_PATH` 等环境变量检查当前配置。
5. 上层工作空间中的依赖包不一定会因为底层变化而全部自动重建。
6. 出现异常时，应检查工作空间顺序，必要时清理并重新构建。

## 十五、完整操作流程

```bash
# 1. 加载 ROS 环境
source /opt/ros/noetic/setup.bash

# 2. 创建工作空间
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws
catkin_make

# 3. 加载工作空间环境
source devel/setup.bash

# 4. 创建功能包
cd ~/catkin_ws/src
catkin_create_pkg beginner_tutorials std_msgs rospy roscpp

# 5. 检查并修改 package.xml 和 CMakeLists.txt

# 6. 构建
cd ~/catkin_ws
catkin_make

# 7. 加载新生成的环境
source devel/setup.bash

# 8. 检查包路径
echo $ROS_PACKAGE_PATH
```

## 十六、总结

Catkin 学习的关键是分清几个概念：
- 工作空间负责组织多个功能包；
- 功能包是 ROS 代码和资源的基本组织单位；
- `package.xml` 负责描述功能包及依赖；
- `CMakeLists.txt` 负责描述构建规则；
- `src`、`build`、`devel` 和 `install` 分别承担不同职责；
- 构建后必须正确加载环境，ROS 才能找到新生成的功能包；
- Overlay 可以在不修改系统安装文件的情况下测试本地版本。



