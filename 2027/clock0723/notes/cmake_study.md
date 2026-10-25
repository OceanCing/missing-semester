# 第二节课笔记
## 1. gcc
机器只认 0/1，源码要先被编译器翻译成机器指令；从 `.cpp` 到可执行文件，gcc 走预处理、编译、汇编、链接四步
### 四个阶段的产物

| 阶段 | 命令 | 生成文件 | 类型 | 作用 |
| ---- | ---- | ---- | ---- | ---- |
| 预处理 | **gcc -E 源文件 -o 输出.i**（`gcc -E main.cpp -o main.i`） | `main.i` | 文本 | 展开头文件、宏、注释后的源码，交给编译 |
| 编译 | **gcc -S 源文件 -o 输出.s**（`gcc -S main.cpp -o main.s`） | `main.s` | 文本 | 汇编代码，交给汇编器 |
| 汇编 | **gcc -c 源文件 -o 输出.o**（`gcc -c main.cpp -o main.o`） | `main.o` | 二进制 | 可重定位目标文件，交给链接器 |
| 链接 | **gcc 目标文件 -o 程序名**（`gcc main.o add.o -o demo`） | `demo` | 二进制 | 可执行文件，能直接运行 |

为了资料上说的作用，我自己拿一个简单的 hello world 试了一下
- #### 预处理
  - 正常：没有输出，生成 `main.i`（3 万多行文本，开头是 `# 0 "main.cpp"` 这种行标记）；错误：头文件写错
```text
main.i：36944 行文本   | main.cpp:1:10: fatal error: iasadsstream: 没有那个文件或目录
# 0 "main.cpp"         |     1 | #include <iasadsstream>
# 0 "<built-in>"       |       |          ^~~~~~~~~~~~~~
……                     | compilation terminated.
```
说明：预处理就是把 `#` 的内容展开，只看找不找得到

- #### 编译
  - 正常：没有输出，生成 `main.s`（汇编文本）；错误：少写一个分号

```text
main.s：汇编文本    | main.cpp: In function ‘int main()’:
.file  "main.cpp"   | main.cpp:4:25: error: expected ‘;’ before ‘return’
.text               |     4 |     cout << "hi" << endl
.globl main         |       |                         ^
main:               |       |                         ;
                    |     5 |     return 0;
                    |       |     ~~~~~~
```
说明：编译就是按语法把代码翻译成机器语言

- #### 链接

- 正常：`demo` 是 ELF 可执行文件，运行输出 `3`；错误：`add` 没定义
```text
demo：二进制                           | ld.bfd: main.o: in function `main':
file → ELF 64-bit LSB pie executable   | main.cpp:(.text+0x13): undefined reference to `add(int, int)'
nm → T _Z3addii @0x11ac                | collect2: error: ld returned 1 exit status
./demo → 3
```
  - 说明：链接就是把函数的声明和定义对上，把源码并到一起

    #### 静态库和共享库
    - 静态库：**ar rcs 库名 目标文件**（`ar rcs libadd.a add.o`），链接时用 **-L 目录** 指定路径、**-l 库名** 指定库（`gcc main.o -L. -ladd -o demo2`），库代码会被复制进程序：
```text
$ ar rcs libadd.a add.o
$ gcc main.o -L. -ladd -o demo2
$ ./demo2
3
```
  - 共享库：**-shared** 生成共享库、**-fPIC** 生成位置无关代码（`gcc -shared -fPIC -o libadd.so add.cpp`）；编译能过，运行时才找 `.so`：
```text
$ gcc -shared -fPIC -o libadd.so add.cpp
$ gcc main.cpp -L. -ladd -o demo3
$ ldd demo3 | grep add
	libadd.so => not found
$ ./demo3
./demo3: error while loading shared libraries: libadd.so: cannot open shared object file: No such file or directory
$ LD_LIBRARY_PATH=. ./demo3
3
```
  - 发给别人、不想管运行环境用静态库；多个程序共用、想单独升级库用共享库
### 分步骤的作用
1. **错误定位**：报错信息带着阶段特征
2. **增量构建**：只改一个 `.cpp`，就只需要重新 `-c` 这一个文件，再跟旧的 `.o` 链接，不必全部从头编
3. **各管一摊**：预处理管文本拼接，编译管语义和优化，汇编管落到机器指令，链接管跨文件拼装
4. **可检查、可复用**：`-E` 看宏展开成什么，`-S` 看生成的汇编，`.o` 能被多个程序重复链接

所以 **gcc main.cpp -o app** 只是把四步一口气跑完；**gcc -E / -S / -c** 则是把四步拆开，每步留下一个中间文件
## 2. Makefile
gcc 适合个人用，代码多又复杂时就用 Makefile，相当于 gcc 脚本
### Makefile 规则
- 课堂上学长手写的，规则就三句：
  - **目标: 依赖**：可执行文件依赖两个 `.o`，下一行写链接命令，`demo: main.o add.o`
  - **目标.o: 源文件 头文件**：一条规则管一个 `.o`，用到哪些头文件也写进依赖，`main.o: main.cpp add.h`
  - **make**：比较时间戳，哪个 `.cpp` 或 `.h` 比 `.o` 新就重跑对应命令，所有 `.o` 就绪后才链接
- 依赖得人写：`add.h` 漏写进规则，`touch add.h` 后 make 只说“已是最新”；缩进必须 tab；跨平台要各写一份
## 3. CMake
Makefile 要手写，依旧很麻烦，而平台一换就得改
### 先认识 CMake 和 catkin
- CMake：用 `CMakeLists.txt` 描述工程（有哪些源码、生成什么、依赖谁），由它生成对应平台的 Makefile / 工程文件；它自己不编译
- catkin：ROS 1 在 CMake 外面套的一层构建约定，专门管“包”和“工作空间”
### 配置和构建
- 生成构建目录和 build/Makefile **cmake -S 源码目录 -B 构建目录**
- 按生成的 Makefile 编译 **cmake --build 构建目录**（cmake --build build）
### 两个小坑
- 缓存变量改了默认值不会自动更新 **set(... CACHE ...)**（旧 build/CMakeCache.txt 还在，要删 build/ 重来）
- 加了源文件忘了重新 **cmake**，build/ 里还是旧的
### 三种写法对比

| 环节 | 纯 gcc 命令 | 手写 Makefile | CMakeLists.txt |
| ---- | ---- | ---- | ---- |
| 基础编译与链接 | `gcc -c main.cpp`、`gcc -c add.cpp`，再 `gcc main.o add.o -o demo` | `demo: main.o add.o`，下面写链接命令；每个 `.o` 再写一条规则 | `add_executable(demo main.cpp add.cpp)` 一行 |
| 头文件依赖 | 改 `.h` 后手动重敲 | 规则后加 `main.o: main.cpp add.h` | 自动扫描 `#include`，改 `.h` 自动重编 |
| 场景 A：子目录 | 加 -I ./include，如 gcc -I ./include -c src/add.cpp | 路径写死在规则里，如 add.o: src/add.cpp | target_include_directories(demo PRIVATE include) |

## 4. catkin
catkin 是 ROS 1 的构建系统：它没重写 CMake，而是在 CMake 外面套了一层 ROS 专用的约定，让一堆包能一起管理
### 它比 CMake 多了什么
- 包有了边界：一个包一个目录，用 package.xml 说清自己叫什么、依赖谁
- 依赖不用自己一个个找：catkin 把 ROS 包汇总给 find_package，再统一链接
- 消息、服务这类自动生成的代码，catkin 会排进构建顺序
- 工作空间统一管理：源码在 src/，产物进 build/、devel/
- 编完生成 setup.bash
### 包目录里有什么
- 最少两个文件：怎么编 **CMakeLists.txt**、叫什么和依赖谁 **package.xml**
- 习惯上源码放 **src/**、头文件放 **include/**；catkin_create_pkg 会顺手建好
- 一个目录只当一个包用，别把包塞进另一个包的目录
### 工作空间
- 两条约定：
  - 包都放在**工作空间**的 src/ 下，统一编译
  - 编完 source devel/setup.bash
- 结构：catkin_ws/ 下是 src/（每个包一个目录，还有一个顶层 `CMakeLists.txt`）、build/、devel/（可执行文件在 devel/lib/<包名>/）
- devel/ 是开发空间，放可执行文件和库；build/ 里是中间文件，平时基本不用看；两个目录都不进版本库
- 建工作空间：
  - 先 **source /opt/ros/noetic/setup.bash**
  - **mkdir catkin_ws && cd catkin_ws**，再 **mkdir src && cd src**
  - **catkin_init_workspace** 生成 `CMakeLists.txt`
  - 回到工作空间根目录 **catkin build**，生成 build/、devel/
- #### 两条命令
  - 初始化工作空间，往 src/ 放一个顶层 `CMakeLists.txt` **catkin_init_workspace**
  - 编译整个工作空间，建出 build/ 和 devel/ **catkin build**
### 建包
- 在 src/ 下执行 **catkin_create_pkg 包名 依赖...**，`catkin_create_pkg test_pkg std_msgs rospy roscpp`
- 生成的就是上面“包目录里有什么”里的那几个文件；`catkin_create_pkg` 会顺手建好 include/test_pkg/ 和 src/
### CMakeLists.txt
- `catkin_create_pkg` 给的模板大部分是注释，解开后要写的就下面这些
- #### 项目与目标
  - 指定最低版本 **cmake_minimum_required(VERSION 版本号)**（必须放最前面，`cmake_minimum_required(VERSION 3.16)`）
  - 定义工程名和语言 **project(名字 语言)**（`project(demo LANGUAGES CXX)`）
  - 生成可执行文件 **add_executable(程序名 源码...)**（`add_executable(demo main.cpp add.cpp)`）
  - 生成库文件 **add_library(库名 STATIC/SHARED 源码...)**（`add_library(add STATIC add.cpp)`）
  - 把库链接到目标 **target_link_libraries(目标 库名)**（`target_link_libraries(demo PRIVATE add)`）
- #### 路径与安装
  - 加头文件搜索路径 **target_include_directories(目标 PUBLIC/PRIVATE 目录)**（target_include_directories(demo PRIVATE include)）
  - 定义安装规则 **install(TARGETS 目标 RUNTIME DESTINATION 目录)**（install(TARGETS demo RUNTIME DESTINATION bin)）
- #### 变量与源文件
  - 定义变量 **set(变量名 值)**（`set(SOURCES main.cpp add.cpp)`），取值写 `${变量名}`
  - 设置 C++ 标准 **set(CMAKE_CXX_STANDARD 版本)**（`set(CMAKE_CXX_STANDARD 17)`），再加 `set(CMAKE_CXX_STANDARD_REQUIRED ON)` 强制要求
  - 把目录里的源文件收集进变量 **aux_source_directory(目录 变量)**（`aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}" SOURCES)`）
- #### catkin 专用
  - 找 ROS 依赖 **find_package(catkin REQUIRED COMPONENTS 组件...)**（`find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs)`）
  - 声明本包导出什么 **catkin_package()**（不写别的包找不到它）
  - 加 ROS 头文件路径 **include_directories(${catkin_INCLUDE_DIRS})**
  - 消息/服务代码先编 **add_dependencies(目标 ...)**（`add_dependencies(test_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})`，纯 C++ 节点可以不写）
- #### 最小框架
  - 一个能跑的 catkin 包，最少写这六行（基础框架）：
```cmake
cmake_minimum_required(VERSION 3.0.2)
project(test_pkg)
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs)
catkin_package()
add_executable(test_node src/main.cpp)
target_link_libraries(test_node ${catkin_LIBRARIES})
```
### 编译与运行
- 命令：**catkin build**，再 **source devel/setup.bash**，最后 **rosrun test_pkg test_node**
- #### 四个注意
  - 在工作空间根目录编译 **catkin build**（并行参数照旧能用 `catkin build -j8`）
  - 只编某个包：`catkin build test_pkg` 会连依赖一起编，只编它自己用 `catkin build test_pkg --no-deps`
  - 产物不在 src/：可执行文件在 devel/lib/test_pkg/，生成的头文件在 devel/include/
  - 把 devel 空间加进环境变量 **source 环境脚本**（source devel/setup.bash；不 source，`rosrun` 会说找不到包，新终端要重新 source）
- 改坏了想重来 **catkin clean**（或直接删 build/、devel/，比手工删文件干净）
### launch
- 节点多了，每次 `rosrun` 带参数很麻烦，写个 launch 文件 test_pkg/launch/test.launch，内容就一行 `<launch><node pkg="test_pkg" type="test_node" name="test_node" output="screen"/></launch>`
- 启动：**roslaunch test_pkg test.launch**
- 按包名找到 launch/ 下的文件，需要时先拉起 `roscore` **roslaunch 包名 文件名**（一个文件里能写多个 `<node>`，还能带参数和命名空间）
- #### 三个字段
  - 包名 **pkg**
  - 可执行文件名，必须和 `add_executable` 的名字一致 **type**（在 devel/lib/test_pkg/ 下）
  - 节点在 ROS 图里的名字，可以和 `type` 不同 **name**
## 5. 大模型使用入门
### 概念
- #### LLM（相当于大脑）
  - 参数量很大的模型 **大语言模型（LLM）**（可以看成“给定提示（输入），对补全内容（输出）的概率分布建模”，推理时从分布里采样）
  - 有固定的上下文窗口（输入 + 输出的最大长度），装不下的内容等于没说，提示词要精炼
- #### Agent 软件（相当于手脚）
  - 让大模型能用工具、和外部环境交互、持续执行任务的系统 **Agent 软件**（装好 agent 并接入模型后，模型就能读写你的文件、跑命令，比如把网页聊天换成 codex cli，省掉复制粘贴）
  - 要控制授权范围，别让它碰不该碰的东西

- #### 插件 (相当于工具)

  - 通过插件可以提升模型的工作能力，特别是dsh（万物皆插件）


### 帮助学习

1. 收集资料：“去网上搜索关于 xx 的最佳实践 / 优秀参考，给出信息来源”
2. 告诉它你学过什么，让它设计针对性课程：“我学过 C++，请你通过交互式提问判断我的水平，并设计一套 Python 学习课程”
3. 学一个课题时，自己先讲一遍，再让它检查
4. 让它出练习题

共同点是让它出题、挑错、给反馈，而不是直接要答案——知识得自己讲一遍才算会
### 帮助开发
- 新的研究强调：过去写大量复杂指令去引导模型，可能已经没必要，甚至会造成上下文膨胀
- #### 提示词框架
  - 清楚说明目标
  - 提供必要上下文（比如给权限读代码）
  - 明确重要约束（比如必须用 git 提交）
  - 规定验证标准
  - 允许 Agent 自主探索
  - 避免过度规定执行步骤
- #### 两个工具
  - 一组可复用的任务指令和配套资源 **Skill**（脚本、参考文档、模板；Agent 需要时加载）
  - 给 AI 看的项目说明书，类似给人看的 `README.md` **AGENTS.md**（写项目背景、开发规范和工作指引）

负责任地使用 AI：对团队负责，对自己负责；工具越能干，越要清楚它在做什么、结果对不对
## 6.用大模型帮助学习的思考
第一，大模型适合干脏活累活，但不能因此放弃自己的思考。 整理笔记时，排版、合并重复、把零散命令归成表格、检查哪里前后不一致，或者电脑出了写奇奇怪怪的为题，这些机械又耗时的活交给它很合适。但是也要知道下次遇到这种情况证明解决，也要学会判断。所以我的做法是：脏活可以外包，判断和学习不能外包。

第二，大模型除了当老师，还要当挑错的人。 让它讲概念、解释，是“当老师”；但更关键的是让它挑错：挑我笔记里的结构问题、挑我讲得不准确的地方、挑我自以为懂了其实没懂的表述。这里有个坑：我自己问问题时如果已经钻了牛角尖，提问方式就会把 AI 往同一个方向带，它顺着我的错误前提往下答，看起来很有道理，其实一起跑偏了。所以不能只让我问它，也要让它反过来问我——让它就某个概念向我提问，或者让它先复述它理解的前提，看我是不是那个前提就错了。它当老师时我容易被动接受，它当挑错者和提问者时，我才被迫重新想一遍。

第三，要学会写提示词，因为“觉得不对但说不出来”是常态。 经常出现的情况是：我知道它给的答案不对劲，但我说不清哪里不对，也就不知道该怎么让它改。这不是它笨，是我没把目标、上下文、约束和验证标准讲清楚。我的体会是，提示词不是一次写好的，而是边追问边收敛：先说我学过什么、现在卡在哪、要它做到什么程度；它答偏了，不要只说“不对”，而是尽量指出“哪一句不对、我希望它往哪个方向改”；实在表达不出来，就让它先给几个不同版本，我再从中指认哪个接近我想要的方向。写提示词的过程，其实也是把自己模糊的直觉逼成明确要求的过程。
