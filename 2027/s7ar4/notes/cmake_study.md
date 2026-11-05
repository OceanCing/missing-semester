# 控制第二课学习随笔

## 1. 从源代码到可执行程序

```text
hello.c ──预处理──> hello.i ──编译──> hello.s ──汇编──> hello.o ──链接──> hello
```

四个阶段具体：创建一个源文件 hello，把代码转换为计算机能读懂的形式，最后变成可执行的程序。

```bash
gcc -E hello.c -o hello.i   # 预处理：展开 #include、替换宏定义、删除注释等
gcc -S hello.i -o hello.s   # 编译
gcc -c hello.s -o hello.o   # 汇编：代码文本转为二进制，但此时还不能跑
gcc hello.o -o hello        # 只链接
```

`gcc hello.c -o hello` 一步到位，中间产物即用即删。

> **疑问：末尾的文件名前面的 `-o` 是什么意思来着？**
> 即 output（输出）的缩写，作用是给输出结果命名。

把第一篇资料（程序的详细编译过程与作业要求）喂给大模型，让它帮我制定学习计划，其中出现了：

**运行并查看：**

```bash
./hello && wc -l hello.c hello.i hello.s && file hello.o hello && nm hello.o
```

| 片段 | 做什么 | 为什么 |
|---|---|---|
| `./hello` | 运行（`./` 表示当前目录） | 确认产物真能跑 |
| `wc -l` | 数行数 | 看出文件内容变化 |
| `file` | 看 ELF 类型 | 区分 `relocatable`（`hello.o`）和 `pie executable`（`hello`）—— 这就是「能不能直接跑」的判据 |
| `nm` | 列符号表 | `T main` = 有定义的代码段符号；`U puts` = **未定义**，等链接器补 |

**最后输出：**

```text
hello, world
    7 hello.c
  845 hello.i
   45 hello.s
hello.o: ELF 64-bit LSB relocatable, x86-64, ...
hello:   ELF 64-bit LSB pie executable, x86-64, ... dynamically linked ...
0000000000000000 T main
                 U puts
```

显然行数能看出几个文件的内容变化：从 .c 到 .i 多了八百多行，就是展开了头文件之类的。

ELF、还有符号表，T、U 什么的又是新概念：

```text
T 来自 Text section（代码段），意思是这个符号是代码，而且大写 = 全局的、本文件里有定义的。
所以 T main 读作：「main 这个函数的代码就在本文件里，地址是 1139，全世界都能调用它。」
而 U = Undefined（未定义）。它只有名字，没有地址。
原因：写 printf("...") 时，只是声明了这个函数长什么样（名字、参数、返回值），
函数体一行都没写——真正的实现在 libc 里，那是个别人编译好的 .so 文件，不在你的 hello.o 里面。
```

（libc 和 .so 文件又是什么？）

```text
于是编译器只能在花名册上留空：U puts   ←「有个叫 puts 的家伙我要用，但是没地址」
也解释了为什么 hello.o 明明已经是二进制了却跑不了——它身上带着一堆没填的地址，一执行就不知道该跳到哪。
```

那必须要填具体地址进去了，所以第四步 `ld` 起作用：

```text
ld 是链接器（linker）。它的工作就是拿着你这本花名册上所有 U 开头的行，去 libc 里找到对应函数的真实地址，然后：
1. 把 libc 里 puts 的代码抄一份进最终文件（静态链接），或者记下「以后运行时去 libc.so.6 里找它」（动态链接，Linux 默认这种）；
2. 把 hello.o 里那个空地址回填成真地址。
填完，U 变成 T，文件类型从 relocatable 变成 pie executable。这时候它才是一个能跑的完整程序。
一句话串起来：编译只保证「语法和名字对得上」，链接才保证「地址对得上」。这也是为什么四个阶段里，只有第 4 步之后才能 ./hello。
```

其实根本看不太懂，大概链接就是给程序找到执行位置？

## 2. CMake 基础

### 2.1 官方定义与流程

**定义：**

> CMake 通过使用简单的配置文件 CMakeLists.txt，自动生成不同平台的构建文件（如 Makefile、Ninja 构建文件、Visual Studio 工程文件等），简化了项目的编译和构建过程。

**构建配置：**

1. **CMakeLists.txt 文件**：CMake 的配置文件，用于定义项目的构建规则、依赖关系、编译选项等。每个 CMake 项目通常包含一个或多个 CMakeLists.txt 文件。
2. **构建目录**：为了保持源代码的整洁，CMake 鼓励使用独立的构建目录（Out-of-source 构建）。这样，构建生成的文件与源代码分开存放。

**基本工作流程：**

1. **编写 CMakeLists.txt 文件**：定义项目的构建规则和依赖关系。
2. **生成构建文件**：使用 CMake 生成适合当前平台的构建系统文件（例如 Makefile、Visual Studio 工程文件）。
3. **执行构建**：使用生成的构建系统文件（如 make、ninja、msbuild）来编译项目。

### 2.2 具体流程

| 步骤 | 做什么 | 命令 |
|---|---|---|
| ① 建目录 | 按 .h/.cpp 的分工摆好文件 | `mkdir -p ...` |
| ② 写代码 | .h 写声明，.cpp 写定义 | `cd 目录 && vim 文件` |
| ③ 写配置 | 告诉 CMake 有哪些目标 | `vim CMakeLists.txt` |
| ④ 构建+运行 | 生成 Makefile 再编译 | `cmake -S . -B build && cmake --build build` |

> **注意**
>
> 1. `vim CMakeLists.txt` 这一串文件名是固定的，不能更改。
> 2. 得先有可以执行的代码，才能写 `vim CMakeLists.txt`；也就是说，配置文件里面的目标必须真实存在，最后才能构建成功和运行。

### 2.3 跟着做一遍（菜鸟教程的例子）

如果要初步走一下使用 CMake 的流程，根据菜鸟教程上的实例，我跟着 AI 的步骤走：

目录结构：

```text
~/CMake_pratice/
├── CMakeLists.txt
├── build/              ← cmake 自动生成，可随时 rm -rf 重来
├── include/            ← 对外公开的头文件
│   ├── core.h
│   └── helper.h
└── src/
    ├── main.cpp        ← 唯一有 main() 的文件
    ├── utils.cpp
    ├── utils.h         ← 程序内部头文件，放这里不对外
    └── lib/            ← 库的实现
        ├── core.cpp
        └── helper.cpp
```

再贴一下 `CMakeLists.txt` 中的内容（实际上就是菜鸟教程）：

```cmake
# 文件路径：项目根目录/CMakeLists.txt
# 注意：文件名必须正好是 CMakeLists.txt，大小写敏感，不能叫 cmakelists.txt

# 1. 指定 CMake 最低版本（必须放在最前面）
cmake_minimum_required(VERSION 3.10)

# 2. 定义项目名称、版本和语言
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

# 3. 设置 C++ 标准为 C++17，并强制要求使用该标准
#    （必须写在 project() 之后，写在前面会被 project() 覆盖掉）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 添加静态库目标（产物：build/libMyLib.a）
add_library(MyLib STATIC
    src/lib/core.cpp
    src/lib/helper.cpp
)

# 5. 添加可执行文件目标（产物：build/MyApp）
add_executable(MyApp
    src/main.cpp
    src/utils.cpp
)

# 6. 为库目标指定公开头文件路径（链接 MyLib 的人也能用 include/）
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 7. 为可执行文件指定私有头文件路径（只 MyApp 自己找 src/ 里的 utils.h）
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/src)

# 8. 将 MyLib 链接到 MyApp（等价于 g++ 命令里把 libMyLib.a 排在最后）
target_link_libraries(MyApp PRIVATE MyLib)

# 9. 设置安装规则（cmake --install 时，可执行进 <前缀>/bin，静态库进 <前缀>/lib）
install(TARGETS MyApp RUNTIME DESTINATION bin)
install(TARGETS MyLib ARCHIVE DESTINATION lib)
```

创建并配置好所需文件（头文件、库、程序）后，就是构建与运行：

```bash
cd ~/CMake_pratice && cmake -S . -B build && cmake --build build
cd ~/CMake_pratice && ./build/MyApp
```

最终能成功输出结果，但很多命令还是不能完全明白意思。

### 2.4 不懂的名词与命令

在做这些的时候，我列出了一些不懂的名词与命令，让 AI 给我答复：

1. **静态库与动态库**：库 = 把一批 .o 文件打包在一起，供别人复用。
   - **静态库（.a）**：整体存在、包含实体文件的库，程序体积大，需要复制。CMake 写法：`add_library(MyLib STATIC ...)`
   - **动态库（.so）**：不复制，程序里留下库的指引（地址？），告诉 xx 运行时去找（libMyLib.so）；也需要库具体存在，但不用重新走。CMake 写法：`add_library(MyLib SHARED ...)`
2. **可执行文件**：能直接被系统加载运行的产物。
3. **公开头文件与私有头文件**：私有 —— 给程序内部自己用；公有 —— 库的调用方也要用。
4. **链接**：把 .o 里空着的调用地址填上，拼成一个完整程序。
5. **目标**：CMake 里对一个要产出的东西的称呼。
6. **依赖**：A 用到了 B，就说 A 依赖 B。
7. **安装规则**：定义 `cmake --install` 时产物复制到哪。

以及暂时没用到的变量：

- **普通变量**：CMakeLists.txt 中定义，每次运行时得重新定义。
- **缓存变量**：存储在 CMakeCache.txt 文件中，在多次 CMake 运行之间保持值不变。

## 3. catkin

### 3.1 AI 给出的介绍

catkin 是 ROS 1 的构建系统，负责把源码编译成可执行程序，并放到正确位置，让 `rosrun` 能找到它。

先用一个类比：

一个 ROS 项目里往往有十几个包，包和包之间互相依赖：A 要用 B 的头文件，B 又要用 C 的库。手动写 Makefile 管理这些依赖会很麻烦。

catkin 的作用相当于一个调度管理器：你只要写清楚**我是谁、依赖谁**，它就会自动按依赖顺序编译、生成、登记，最后统一放到 `devel/` 目录。

它底层是 CMake，只是又包了一层 ROS 专用的规则，并添加了 Python 工具。

### 3.2 catkin 的几个概念

- **catkin workspace（工作区）**：用来存放所有包、作为一次编译的边界的一个目录。
- **package（功能包）**：ROS 的最小复用单位，通常对应一个功能。
- **构建文件**：`package.xml` 和 `CMakeLists.txt`，一个包必须同时有这两个文件。
  - `package.xml`：声明我是谁、version 是什么、依赖哪些包（给 catkin 用于查找和排序）。
  - `CMakeLists.txt`：说明怎么编译（添加哪些源文件、生成什么可执行程序）。

工作区里的三个目录：

| 目录 | 作用 |
|---|---|
| `src` | 存放 package 源代码包 |
| `build` | 开发过程中的一些中间文件、二进制文件与 cmake & catkin 缓存 |
| `devel` | 开发空间，放置开发过程的可执行文件、库以及目标文件 |

### 3.3 操作流程

> **注意：开始工作之前，先进容器，执行（Ubuntu 26.04.1）：**

```bash
xhost +local:root
docker start ros1 && docker exec -it ros1 bash
source /opt/ros/noetic/setup.bash
```

1. **建包**，划出包的边界并声明依赖：`cd ~/ros1_ws/src && catkin_create_pkg 包名 依赖...`
2. **编译空包**（验证链路，选做）：`cd /root/ros1_ws && catkin_make`
3. **写节点的逻辑**：在 `包名/src/` 下写 `xxx.cpp`。
4. **告诉 catkin 怎么把文件编译成程序**：在包目录的 `CMakeLists.txt` 末尾追加下面两行。
5. **编译**，产物写入 `devel/`：`cd ~/ros1_ws && catkin_make`
6. **登记并运行**：`source devel/setup.bash`，然后 `rosrun 包名 xxx`

第 4 步要追加的内容：

```cmake
add_executable(xxx src/xxx.cpp)
target_link_libraries(xxx ${catkin_LIBRARIES})
```

我让 AI 设计了一个基础的运行流程，她最终做到：将两个不认识的程序（没有依赖），在一个频道 `chatter` 上用 `roscore` 中转，让第一个程序每秒的输出结果发出，再在第二个程序上收到并输出。那我后续如果要更改交流的信息，只需修改第一个程序就行了（还没有验证）。

### 3.4 碰到的问题

**1. 在 IDE 上写 cpp 文件时，包含的头文件显示报错。**

是因为包是在容器里创建的，而容器与宿主机的身份权限不一样，得修复（扩大一下权限）：

```bash
sudo chown -R s7ar4:s7ar4 ~/ros1_ws
```

好像每次要在宿主机上修改，都需要修改权限。

**2. 还有修改文件要注意的：**

| 改什么 | 怎么编译 |
|---|---|
| 改 .cpp 里的代码 | 直接 `catkin_make`（增量，很快） |
| 新增/删除源文件、改 CMakeLists.txt | `catkin_make` |
| 改 package.xml 的依赖 | `catkin_make`，并重开终端重新 source 两条 |
| 新建了一个包 | `cd /root/ros1_ws && catkin_make`（不能在包目录里跑） |

**3. 三条必须遵守的规则**

1. 每开一个新终端都要 source 两次：`/opt/ros/noetic/setup.bash`（让 ROS 命令存在）和 `devel/setup.bash`（让你的包存在）。
2. `catkin_make` 必须在工作区根目录执行，不能进入包目录。
3. 频道名两边必须一致，`advertise("chatter")` 和 `subscribe("chatter")` 要一字不差；`add_executable` 里的文件名也必须与源文件的实际名字一致。

## 4. 《The Emperor Has No Clothes: How to Code Claude Code in 200 Lines of Code》

即：皇帝的新衣。

这个标题在知道这篇文章大概讲什么后就变得很有意思，它想教会我们自己做一个 agent。

实话说，自从上大学以来，我特别经常跟大模型、agent 打交道。我知道 agent 可以帮助我们干活、查阅，却从来没有敢想过自己做一个 agent，理解它到底怎样在电脑上做到那么多事情；或者简单想过，却在一开始就认为：这肯定很难，我肯定完全看不懂。这种心理就很像在王权下的大臣们，他们有的可能是看出了皇帝根本没穿上衣服，也有的，可能真的单纯怀疑自己「不够聪明」。这样被「固化」的思维让我有点惭愧。

当然，这篇文章的很多代码我现在确实看不太懂，也勉强做一些笔记吧。

### 4.1 运行逻辑骨架

1. 我发出问题或命令。
2. LLM 要么正常回答，要么发起工具调用。
3. 程序在本机执行那个调用。
4. 结果发回对话里。
5. LLM 拿着新上下文继续 → 回到第 2 步，直到它不再要工具。

### 4.2 一些需要知道的知识基础（AI 给出）

1. **Python 基础**：你写 C++，会撞上的差异就这几个 —— 缩进代替大括号、`def` 定义函数、`dict` 当结构体用、`with open()` 自动关文件、`f"{x}"` 字符串插值。
2. **什么叫「API 调用」**：把文字发给远端模型、拿回文字，本质一次网络请求。
3. **JSON**：`{"key": "value"}` 这种文本格式，用来打包参数。
4. **docstring / 函数签名**：Python 的注释字符串和参数列表（全文最反直觉的一点是，它被当成给 LLM 用的工具说明书）。

### 4.3 搭建

1. **设置脚手架**：import、API 客户端、终端颜色、`resolve_abs_path`（file.py → /abs/path/file.py），产出模具。
2. **实现工具**：创建三个函数分别用来读取、列表、编辑文件，返回结构化的能被 LLM 看懂的值（实现 agent 功能）。
3. **注册 + 生成说明书**：通过函数签名和文档字符串动态生成功能，让 LLM 了解存在哪些工具以及如何调用它们。
4. **解析调用**。
5. **串联主循环**：外层 while 获取用户输入，内层 while 呼叫与调用 LLM → 执行工具 → 塞结果，内层不再要工具才 break。

## 5. 大模型与 agent 使用感受与心得

### 5.1 历程

先记录一下历程：最开始，我想图省事，先把网址丢给 AI，想让她帮我制定一份学习计划，这样我可以跟着操作一步步来。但越进行下去越觉得困难：首先是 AI 给出的各种命令，我完全是一知半解的，又要去重新提问某一段的意思；然后，很多时候 AI 给出的方案并不符合我的要求，或不是我想的那样，自然而然就有冲突，更加无法理解到底做了什么。于是我半脱离 AI 自己进行阅读，不懂的再去询问，这样我获得的信息是间续的，而不是突然一大片未知的信息，这反而消磨了学习的动力。

在我结束高中、步入大学，开启新一段学习的阶段，正处在 AI 高速发展的时候。于是很容易就能在网络平台上看到 AI 的各种事情：大模型、agent、vibecoding……能够帮你做事，听起来很吸引人。以及迅速迭代的大模型，不断增强的 AI 的能力。我刚开始看到一些新奇的东西就想着去做做，最后的结果却不尽人意，也耗费了时间。这算一种跟风吗？我在平台上碰到过一个评论，大致是这个意思：现在很多人只是想去用 AI，因为它流行、新奇，做了很多却不知道做出来的东西到底有什么用，是不是自己真正需要的，只是为了潮流。就我自己来看，我确实有时候就像这条评论说的那样。希望我能找到对于自己 AI 的意义，和自己真正需要和想做的东西。

在我使用 AI 的路上，国产模型用过很多，agent 也试过几个。目前大部分情况用的是 ds 接入 Hermes + DSH。秉着全局的思维，当然是哪个性价比好用哪个，哪个能做事用哪个。我比较看重 Hermes 优良的记忆与技能，她能记好我很多要求，存成 skill，使用起来不烧心。而 DSH 可塑性与动手能力也都不错。至于国外的还没有过多了解。

### 5.2 要求（限定词）是最重要的一环

在使用 AI 学习或者获取信息、工作的场景中，要求（或者说是限定词）是非常重要的一环，直接决定了你的目标产出质量与实际时间成本（毕竟得不断调试）。正如现在流行的非常优秀的 AI 短剧，可以说是有海量（可达到百万级）的限定词才能产出一部好的作品。下附一些限定词（不一定正确）：

1. 以瞎猜接口为耻，以认真查询为荣。
2. 以模糊执行为耻，以寻求确认为荣。
3. 以臆想业务为耻，以人类确认为荣。
4. 以创造接口为耻，以复用现有为荣。
5. 以跳过验证为耻，以主动测试为荣。
6. 以破坏架构为耻，以遵循规范为荣。
7. 以假装理解为耻，以诚实无知为荣。
8. 以盲目修改为耻，以谨慎重构为荣。

这套准则倡导严谨求证、遵守规范的工作态度。

### 5.3 用过的几种「人设限定词」

```text
我是一名智力低下的研一学生，毫无基础，帮我解析一下这篇文献，让我达到彻底看懂这篇文献的效果，注意，我不懂任何专业术语，你要详细解释文献中的每一个图以及图中的横纵坐标以及每个线和点。

我是一位100岁的太奶，这东西看的我头晕眼花，这些知识点我都看不懂，不过我宝刀未老，依旧怀有着一颗浓厚的热烈的爱学习新知识的心，我的学习热情一点不减20岁那年，好孩子，劳驾你帮奶奶把这个 PDF 理一理，给奶奶说道说道，让奶奶彻底理解，彻底看懂，一定要给我讲明白哈，你可千万别蒙奶奶，奶奶最后的希望就是你啦！重新开始讲！

你是这个领域的权威专家，而我是一位差生，而且患有 ADHD，容易走神，理解能力很差，有轻微阅读障碍，更糟糕的是我现在是你的学生，请详细阅读 PDF 并掌握内容，请您帮我用通俗易懂的语言和方式讲解，讲解时注重细节性理解并加以解释，谢谢老师。
```

### 5.4 第二课培训补充：用大模型帮助学习

1. **收集资料**：「去网上搜索关于 xx 的最佳实践/优秀参考，给出信息来源」。
2. **告诉 AI 你已经学过什么，设计针对性的课程**：「我学过 c++，请你通过交互式提问我的知识水平，并设计一套 python 学习课程」。
3. **学习一个课题时，自己讲一遍让 AI 检查**。
4. **让 AI 出练习题**。

### 5.5 组织提示词的框架

1. 清楚说明目标。
2. 提供必要上下文。
3. 明确重要约束。
4. 规定验证标准。
5. 允许 agent 自主探索。
6. 避免过度规定执行步骤。

一个采访问答 skill，帮助 AI 更好地理解我的诉求、进行决策：
<https://github.com/mattpocock/skills/blob/main/skills/productivity/grilling/SKILL.md>
直接让 agent 帮助装一下就行。

> 在进行使用 AI 学习的过程中，也给自己加上限定词：不要麻木地跟着 AI 操作，而是去理解 AI 每一步的用意与作用。
