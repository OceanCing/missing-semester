# CMake_study

## 传统手动编译

![](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-24 10-44-04.png)

| 阶段   | 命令（示例）                               | 产出文件   | 作用说明                                                     |
| ------ | ------------------------------------------ | ---------- | ------------------------------------------------------------ |
| 预处理 | `g++ -E -I include src/main.cpp -o main.i` | `.i`       | 展开 `#include` 头文件，删除注释，替换宏定义                 |
| 编译   | `g++ -S -I include src/main.cpp -o main.s` | `.s`       | 将 C++ 代码翻译成底层的**汇编语言**                          |
| 汇编   | `g++ -c -I include src/main.cpp -o main.o` | `.o`       | 将汇编代码翻译成计算机能懂的**机器码**（二进制）             |
| 链接   | `g++ main.o calc.o -o my_calc`             | 可执行文件 | 将多个 `.o` 文件与系统库（如 `iostream`）合并打包，生成最终可执行的程序。 |

本人尝试过有个注意点：手敲 g++ 时，如果代码里有 `#include "xxx.h"`，但头文件不在当前目录下，**必须加 `-I <目录名>`**（如 `-I include`），否则会报 `fatal error: xxx.h: No such file or directory`。



## CMake 构建

#### 目录结构

```` text
MyProject/
├── build/          # 专门用来放编译产物（自动生成）
├── include/        # 存放头文件（.h）
├── src/            # 存放源文件（.cpp）
└── CMakeLists.txt  # CMake 构建脚本
````

#### CMakeLists.txt 核心语法

```` cmake
cmake_minimum_required(VERSION 3.16) # 规定 CMake 的最低版本要求
project(MyProject)                   # 定义项目名称

# 自动扫描 src 目录下所有的 .cpp 文件，存入变量 SOURCES
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src" SOURCES)

# 用收集到的源文件，编译生成名为 demo 的可执行程序
add_executable(demo ${SOURCES})

# 告诉编译器去 include 目录下找头文件
target_include_directories(demo PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
````

#### 标准走命令

```` bash
cmake ..  # 读取上级目录的 CMakeLists.txt，生成 Makefile

cmake --build .  # 执行编译（自动调用 g++ 完成预处理、编译、汇编、链接）

./demo # 运行程序
````

#### 实现过程

我做了一个关于两个数字加减乘除的简单程序进行实现

**/src**

![](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-24 11-16-37.png)

![](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-24 11-17-18.png)

**/include**

![](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-24 11-34-40.png)

**最后的实现**

![](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-24 12-00-30.png)

![](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-24 12-10-08.png)

**实现时的踩坑**

自己写了一个 `int div(int, int)`，与 C++ 标准库 `<cstdlib>` 自带的 `div` 函数冲突。后来就改名为 `divide`；还有一个就是头文件找不到：直接 `g++ main.cpp` 编译，找不到 `#include "calc.h"`。后来直接加上 `-I include` 参数（CMake 里对应 `target_include_directories`）



# Catkin

## 结构

```` text
catkin_ws/                 
├── build/                 <-- 自动生成：编译中间文件
├── devel/                 <-- 自动生成：存放编译好的程序和环境变量脚本
└── src/                  
    ├── CMakeLists.txt     <-- 整个工作空间的顶层 CMake（由 catkin_init_workspace 自动生成）
    └── my_package/        <-- 【ROS 包】 (相当于一个独立的 MyProject)
        ├── CMakeLists.txt <-- 包的构建脚本
        ├── package.xml    <-- 声明依赖了什么库
        ├── include/
        └── src/
````

## 创建流程(docker版)

#### 启动和退出容器

```` bash
docker start -ai ros1_docker
exit

ros1_docker            # 没跑就启动，然后进入容器里的 shell（已 source ROS 环境）
ros1_docker -d         # 只启动，不进 shell
ros1_docker -s         # 停止
ros1_docker -r         # 重启
ros1_docker --status   # 看状态
ros1_docker roscore    # 直接在容器里执行命令
````

#### 新建一个容器挂载在目录上

```` bash
docker run -it --name ros1_docker \
  -v ~/catkin_ws/src:/root/catkin_ws/src \
  osrf/ros:noetic-desktop-full
````

#### 配置环境变量

```` bash
source /opt/ros/noetic/setup.bash
````

#### 初始化工作环境

```` bash
# 进入工作空间的 src 目录
cd /root/catkin_ws/src
catkin_init_workspace
````

#### 创建包

```` bash
catkin_create_pkg my_test_pkg roscpp std_msgs
````

#### 编译

```` bash
# 退回工作空间根目录
cd ..
catkin_make
````

**成功结果**

![](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-24 14-03-09.png)

# **关于DeepSeek-Harness**

## 安装

https://nodejs.org/zh-cn/download

https://github.com/deepseek-ai/deepseek-harness/blob/master/README.zh.md

## 启动后台

```` bash
npx @deepseek-ai/dsh web
````

## 心得	

我刚开始使用这个的时候为了图方便，让harness帮忙把他自己修改成开机自启后台的状态，接着我每次开机的时候只要在终端输入ds就可以启动harness，体验感还是有点新鲜的。AI对于我们学习的话还是便捷太多了吧，很难想象之前没有AI的时候遇到问题就只能百度和博客的无力感，确实省了很多查资料的功夫。但也说的对，不能不经过思考的去相信AI,之前我做了一个人工智能原理的实验，老师讲到了AI有时会给出欺骗式的代码，因为AI 本质上是一个“概率文字接龙机器”，它为了把话说圆，有时会一本正经地胡说八道，甚至凭空捏造不存在的函数和库。所以我们作为人还是要会理解过程原理和含义吧！

**交互记录**

![](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-24 14-09-42.png)

![](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-24 14-09-51.png)
