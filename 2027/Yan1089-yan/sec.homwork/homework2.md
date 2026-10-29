# 第二次笔记
## agent理解
### agent**运行原理** = 大模型 + 本地工具调用 
    large language model大量参数模型，字符串预测
    agent是能够让大模型与外界环境交互操作并执行任务的工具，，安装agent并接入大模型。大模型相当与操作机甲的机甲师，agent就是机甲。
1.发出指令-大模型阅读
2.大模型判断需不需要工具，需要哪些工具，发出命令要求调用（通过函数签名和文档字符串动态生成，提供给大模型可利用的工具）
3.本地程序调用工具，列出目录给大模型
4.使用编辑和生成代码权限，创建和生成所需文件，源代码
5.本地程序执行代码文件。
6.返回大模型根据上下文继续判断需求，如此循环往复（内外循环，外循环获取用户需求和信息，内循环完成调用直至上下文需求已完成）

### 大模型所需工具：①读文件 ②列出并导航寻找文件 ③编辑文件
更高级的辅助生产级别agent有更流畅的使用体验，更智能的上下文管理和更多的可调用的智能工具
### 合理使用ai工具
- 用大模型帮助学习 收集资料设计课程，作自己输出让ai检查，让出练习题
- 大模型帮助开发框架组织提示词，
    目标-上下文-重要约束
    比如用git返回ai生成的shi-规定验证标准-允许ahent自主探索-避免过度规定执行步骤可能会降低效率
- skill是可复用的任务指令（提示词和工具的组合），AGENTS.md约束ai的开发行为，
    规范的是写到AGENT.md里让他们去vdu读文件，而不是全部放到跟目录里面
- 让agent装就不用自己在终端里面装
- 负责任地使用ai，自己去读懂技术细节，不要太依赖棒棒糖，利用ai去学习，提升自己的本领
---
---


## 源代码到可执行程序
- 源代码到可执行程序：预处理（cpp），编译器，汇编，链接
1. 编译器会先把代码翻译成电脑能懂的文件形式（.o 文件）
2. 静态链接：链接器把多个目标文件拼在一起，给各个代码段和数据段分配虚拟地址，然后重定位代码。（强弱符号选强，同弱任选）链接可以实现多个增量更改
3. 生成可执行文件：拼装完成，生成类似 prog21 的可执行文件。文件内部划分为不同的段（比如 .text 放代码，.data 放初始化数据，.symtab 放符号清单等）

- 虚拟内存：给每个进程可以全部调用真实所以内存的假象，实际只占有编写代码的那段内存空间，正式开始运行程序时才能够按照进程搭建，使用内存
特点：可以各自独立，保护其他进程不会被破坏

- 静态链接和动态链接区别
    静态链接：把用到的所有库代码全部复制进最终的程序里。缺点是占内存和硬盘，库更新了程序也要重新编译，优点是方便，打包即走
    动态链接（共享库）：程序里不复制库的实体代码，要用的时候随时调用，避免重复运行占内存（.so 文件）

- 运行程序
1. 启动程序：你在终端运行程序，操作系统的加载器把可执行文件读进内存。
2. 启动动态链接器，把需要的库加载到内存中 ，修改main里的标记，把函数调用的位置指向内存中真实的库地址（重定位）。
3. 程序运行
---
---

## cmake介绍

- makefile编译说明书，可以用一条简单的指令省去多个步骤，提高编译效率
- cmake，当东西变多手写规则makefile会变得不方便，，而且makefile在其他系统可能用不了。cmake可以实现跨平台，生成标准的makefile文件和project文件，用简单的命令描述编译过程，是一个跨平台的构建系统生成工具，管理 C/C++ 项目的编译过程。通过编写 CMakeLists.txt 配置文件，可以定义项目的构建规则、依赖关系和编译选项，CMake 会自动生成对应平台的构建文件

#### 构建 编写CMakeLists.txt文件（就像总指挥，核心作用）

##### 基本语法

1. 指定最低版本（必须放在最顶部）
    示例：`cmake_minimum_required(VERSION 3.10)`
2. 定义项目名字和使用的编程语言
   示例：`project(名称 语言)`  `project(MyProject CXX)`
3. 添加可执行文件
   示例：`add_executable(目标文件名不用加后缀 源文件)` 源文件可以有多个
   `add_executable(MyApp main.cpp utils.cpp)`
4. 添加库文件
   示例：`add_library(目标文件名 [STATIC | SHARED | MODULE]类型 <源文件>...)`
5. 链接库到目标（可以链接外部库）
   `target_link_libraries(目标文件名 <库>...)`  `target_link_libraries(MyApp PRIVATE MyLib)`
6. 设置变量
   `set(<变量名> <值>...)`  后续通过 `${变量名}` 引用
7. 为目标指定头文件路径
   `target_include_directories（目标文件名 可见性 路径）`

   `target_include_directories(MyApp PRIVATE include)`

   - 可见性有三种：
        ①PUBLIC：当前目标和依赖它的目标都能使用该路径
        ②PRIVATE：只有当前目标能使用
        ③INTERFACE：只有依赖当前目标的其他目标能使用
8. 设置安装规则
   `install(TARGETS 目标文件 安装路径）`

   ```
   [RUNTIME DESTINATION <可执行文件安装路径>]
   [LIBRARY DESTINATION <动态库安装路径>]
   [ARCHIVE DESTINATION <静态库安装路径>]
   [INCLUDES DESTINATION <头文件安装路径>])
   ```

##### 变量--储存和传递信息

|  | 普通变量 | 缓存变量 |
| --- | --- | --- |
| 存哪 | 内存，随配置运行 | CMakeCache.txt |
| 是否持久 | 否 | 是 |
| 作用域 | 当前目录及子目录 | 全局 |
| 能不能改 | 不能 | 能，-D 参数或 ccmake |

##### 头文件搜索路径

在 CMake 中，有两种方式可以指定编译器查找头文件的路径：include_directories() 和 target_include_directories()。
- 推荐使用`target_include_directories()`，有针对和指向性，逻辑清晰可精准控制
- PUBLIC / PRIVATE / INTERFACE 的区别
| 关键字 | 当前目标可用 | 依赖此目标的其他目标可用 | 典型场景 |
| --- | --- | --- | --- |
| PRIVATE | 是 | 否 | 仅自身实现需要的内部头文件 |
| PUBLIC | 是 | 是 | 库的公开 API 头文件，自身和消费者都需要 |
| INTERFACE | 否 | 是 | 仅头文件的库（Header-only library） |

##### 查找库和包

很多项目依赖外部库和外部工具，find_package 可自动检测和调用，配置这些外部工具
示例：`find_package(库 REQUIRED)`

另外推荐直接通过 target_link_libraries 将库导入，更加符合现代 cmake
示例：`target_link_libraries(MyApp PRIVATE Boost::Boost)`
---
---

## catkin使用

### 构建工作空间

1. 加载环境

   ```
   source /opt/ros/noetic/setup.bash
   ```

2. 创建工作空间
   示例：

   ```
   mkdir -p ~/catkin_ws/src
   cd ~/catkin_ws/
   catkin_make
   ```

   （第一次在工作区运行 catkin_make 会在 src 下生成一个 CMakeLists.txt 链接，在根目录可以找到 build 和 devel 两个文件夹）

3. 加载当前工作空间的环境
   示例：

   ```
   source devel/setup.bash
   ```

4. 确保工作区被设置脚本正确覆盖，确保环境变量 ROS_PACKAGE_PATH 包含所在的目录。
   示例：输入：

   ```
   echo $ROS_PACKAGE_PATH
   ```
   正常输出：

   ```
   /home/用户名/catkin_ws/src:/opt/ros/<版本>/share
   ```