# 程序编译的整体过程
## 预处理
`gcc -E hello.c -o hello.i`
- 展开 #include，把需要的头文件内容纳入预处理结果。
- 展开 #define 定义的宏。
- 处理 #if、#ifdef 等条件编译指令。
- 去除注释等。
## 编译阶段（hello.s以一种文本格式描述了低级机器语言指令)
- 词法分析:把代码拆分成一个个有意义的基本单位
- 语法分析:检查这些零件的组合方式是否符合语言语法
- 语义分析:检查代码含义是否合理
静态语义(在编译器可以确定的语义)、动态语义(只能在运行期才能确定的语义)
- 源代码优化:在生成目标代码之前，优化程序的表示方式
- 目标代码生成:把 C 语言翻译成更接近计算机底层的语言
- 目标代码优化
## 汇编阶段 (汇编器将hello.s翻译成机器语言指令)
- hello.o(二进制文件,可重定位目标程序的格式)
## 虚拟内存
- 主存中只保存活动区域，并根据需要在磁盘和主存之间来回传送数据
- 为每个进程提供了统一的地址空间
- 为每个进程提供独立的地址空间
## 链接 （得到可执行目标文件，被加载到内存中，由系统执行）
 好处：代码更容易管理、维护和复用。
## 静态链接(可重定位目标文件作为输入）
1. 空间和地址分配:获得所有输入目标文件的段长度，并且将它们合并，连接之后，每个段被分配到相应的虚拟地址
2. 符号解析:将每个符号引用正好和一个符号定义关联起来
3. 重定位:根据最终地址布局修正相关指令或数据
## 动态链接（一个副本可以被不同的正在运行的进程共享）
1. 链接时，并不会把整个共享库的代码和数据直接复制进 prog21
2. 启动动态链接器，处理符号引用和重定位
# CMake
## Makefile告诉make工具哪些文件需要编译、编译顺序、需要什么命令(文件增多不方便）
## Cmake产生Makefile文件(灵活跨平台）
## 认识CMakeList.txt
1. cmake_minimum_required(VERSION 3.10)
要求cmake版本
2. project(MyProject CXX) 
项目名/语言
3. add_executable(MyApp main.cpp utils.cpp) 
目标名/源文件 指定源文件编译成可执行文件
4. add_library(MyLib STATIC library.cpp) 
目标名/静态库/源文件
5. target_link_libraries(MyApp PRIVATE MyLib) 
链接库 
6. set(SOURCES main.cpp utils.cpp helper.cpp) add_executable(MyApp ${SOURCES})
设置变量，多个值
7. target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
PROJECT_SOURCE_DIR 表示的是项目顶层源代码目录，提高可移植性
PUBLIC：当前目标和依赖它的目标都能使用该路径
PRIVATE：只有当前目标能使用
INTERFACE：只有依赖当前目标的其他目标能使用
8. find_package
查找外部库和包后：target_link_libraries(MyApp PRIVATE Boost::Boost)
Boost::Boost 自动设置头文件路径和库文件路径
9. 普通变量：运行时重新计算；缓存变量：运行时重新计算
## 构建项目
1. 配置项目
`cmake -S . -B build`
-S . :指定源代码目录为当前目录
-B build :指定构建目录为 build/
2. 编译和链接
`cmake --build build`
3. 运行项目
`./build/MyApp`
## 常见cmake项目目录
MyProject/
├── CMakeLists.txt          # 项目构建配置文件
├── include/                # 头文件（.h、.hpp）
│   └── utils.h
├── src/                    # 源文件（.cpp）
│   ├── main.cpp
│   └── utils.cpp
├── lib/                    # 第三方库或预编译库
├── tests/                  # 测试代码
├── docs/                   # 项目文档
└── build/                  # 构建目录（生成的文件）
# catkin
## catkin workspace
## src/ (存放 ROS 软件包的源代码)
### 软件包目录结构
robot_control/
├── CMakeLists.txt
├── package.xml
├── include/
│   └── robot_control/
│       └── controller.h
├── src/
│   ├── controller.cpp
│   └── main.cpp
├── launch/
│   └── robot.launch
├── config/
│   └── params.yaml
└── msg/
## build(camke&catkin缓存和中间文件）
## devel（开发空间，放置开发过程中的可执行文件和库）
## 创建工作空间
1. `mkdir -p ~/catkin_ws/src`创建工作空间的源代码目录
2. `cd ~/catkin_ws`
   `catkin_make`             构建工作空间
3. `source devel/setup.bash`	 加载开发空间环境 
#AI大模型
## 大语言模型负责决定做什么，外部程序负责实际执行操作
## 三种能力
- 读取文件，以便LLM可以查看您的代码
- 列出文件，以便它可以浏览您的项目
- 编辑文件，以便能够发出创建和修改代码的指令
## 代理循环
- 外层循环：获取用户输入，添加到对话中
- 内部循环会一直持续，直到 LLM 未请求任何工具为止








   


   






