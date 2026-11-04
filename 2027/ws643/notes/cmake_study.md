# Cmake

 ## 从源代码到可执行程序
      hello.c(文本)
### - 预处理
      展开头文件
      g++ -E main.cpp -o main.ii
      hello.i(文本)
### - 编译
      生成汇编语言
      g++ -S main.ii -o main.s
      hello.s(文本)
### - 汇编
      生成机器码
      g++ -c main.s -o main.o
      hello.o
### - 链接
      g++ main.o add.o -o demo
      链接到静态库或者动态库（共享库）
一次性全执行 : ==g++ main.cpp add.cpp -o demo==
      

## CMakeLists.txt 文件
### 格式
###### 命令名(参数列表)
文件名为cmakelist.txt，不可随意改变
每一个Cmake项目至少要有一个cmakelist.txt文件
=>cmake项目 ： 由cmake管理的一个由源代码，配置文件以及其他相关文件组成的整体

#### 命令
  - cmake_minimum_required(VERSION 3.10)  ---指定CMAKE最低版本
  - project(<项目名> [语言]) -----定义项目名称和语言
  - aux_source_directory(. 变量名1) -----取出当前目录下的所有指定语言文件
  - add_executable(<目标名>  <源文件>或 ${变量名1}) ----将指定源文件编译生成一个可执行文件
  ```cmake
  cmake_minimum_required(VERSION 3.10)
  project(project1 cpp)
  add_source_directory(. CMAKRLEARN)
  add_executable(main2 ${CMAKRLEARN})
  target_include_directories(main2 PRIVATE include)
  #局部绑定头文件，只给main2用，PRIVATE表示当前头文件只允许给main2用
  ```
#### 规则

头文件一般放在include文件夹里，源文件一般放在src文件夹里，cmake生成的其他文件放在build文件夹里，生成的目标文件放在bin文件夹里
实现：
```cmake
cmake_minimum_required(VERSION 3.19)
project(project3)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src" SOURCES)
add_executable(demo ${SOURCES})
target_include_directories(demo PRIVATE "${CMAKE_SOURCE_DIR}/include")
```
#### 增量构建
      有时只改了一部分文件，没有全改，不用全编译
      For example:
```shell
      vim main.cpp
      vim add.cpp
      g++ main.cpp add.cpp -o demo
      ./demo
      vim add.cpp
      g++ -c add.cpp
      g++ add.cpp main.cpp -o demo
```
==makefile文件中增量构建由make自动判断，由于add.cpp时间戳晚于add.o，判定被修改，重新编译，然后检查依赖，发现demo，也会重新编译==

# catkin
## Catkin 工作空间（Workspace）

Catkin 是 ROS 1 常用的构建系统，建立在 CMake 之上，用于组织、编译 ROS
软件包（Package）。

### 工作空间的典型目录结构

catkin_ws/
├── src/       # ROS Package 源码包
├── build/     # CMake、Catkin 构建过程中生成的缓存和中间文件
└── devel/     # 开发空间，包含开发过程中生成的可执行文件、库及环境设置文件

• src/：放置 ROS 功能包的源代码。
• build/：放置构建过程中生成的缓存和中间文件，通常不需要手动修改。
• devel/：开发空间，保存构建产生的可执行文件、库及相关环境文件。

### 创建并初始化工作空间

mkdir catkin_ws
cd ~/catkin_ws
mkdir src
cd src
catkin_init_workspace
cd ..
catkin_make

各命令的作用：

mkdir catkin_ws                   创建名为 catkin_ws 的目录

cd ~/catkin_ws                    进入家目录下的 catkin_ws

mkdir src                         创建源码目录

cd src                            进入源码目录

catkin_init_workspace             初始化 Catkin
工作空间的源码目录，通常会创建指向
Catkin CMake 文件的 CMakeLists.txt

cd ..                             返回工作空间根目录

catkin_make                       配置并构建工作空间，生成 build/、devel/ 等目录

> **注意：** `catkin_init_workspace` 应在工作空间的 `src/`
> 目录中执行。初始化后，通常会在 `src/` 中生成
> `CMakeLists.txt`。`catkin_make` 则在工作空间根目录执行。

### catkin --help 与 catkin_make 的区别

• catkin_make：ROS 1 中常用的工作空间构建命令。
• catkin：通常由额外的 catkin_tools 工具包提供，可使用
catkin build 等子命令。
• 找不到 catkin 命令，并不一定意味着 ROS Noetic 没有安装；如果
catkin_make 可用，仍可使用它构建工作空间。
## ROS Package（功能包）

Package 是 ROS 组织代码和功能的基本单位。一个 Package
通常包含程序源码、配置、模型、消息定义等资源。

### 常见 Package 目录结构

test_pkg/
├── CMakeLists.txt   # 构建配置与编译规则（必需）
├── package.xml      # Package 描述信息、依赖等（必需）
├── src/             # 源代码文件
├── include/         # C/C++ 头文件
├── scripts/         # 可执行脚本
├── msg/             # 自定义消息定义
├── srv/             # 自定义服务定义
├── models/          # 模型文件（依具体项目而定）
├── urdf/            # URDF 机器人描述文件（如项目需要）
└── launch/          # launch 启动文件

说明：

• CMakeLists.txt：描述 Package 的构建配置、依赖查找和编译规则。
• package.xml：记录 Package
的名称、版本、维护者、描述及依赖等元信息。
• src/：存放源代码。
• include/：存放 C/C++ 头文件。
• scripts/：存放可执行脚本，例如 Python 节点脚本。
• msg/：定义自定义 ROS 消息类型。
• srv/：定义自定义 ROS 服务类型。
• models/：存放项目使用的模型文件；具体目录是否存在取决于项目。
• urdf/：存放机器人结构描述文件。
• launch/：存放用于启动一个或多个 ROS 节点及相关配置的 launch 文件。

> 以上是常见目录示意，不是每个 Package
> 都必须包含所有目录。课件强调的必需文件是 `CMakeLists.txt` 和
> `package.xml`。

### 创建 Package

课件给出的命令：

cd ~/catkin_ws/src/
catkin_create_pkg test_pkg std_msgs rospy roscpp

含义：

• catkin_create_pkg：创建一个新的 ROS 1 Package。
• test_pkg：新 Package 的名称。
• std_msgs：ROS 标准消息定义包之一。
• rospy：ROS 1 的 Python 客户端库。
• roscpp：ROS 1 的 C++ 客户端库。

这条命令会创建 test_pkg/，并在 Package
的描述和构建配置中声明相关依赖。

### Package 与 Workspace 的关系

• Workspace（工作空间）：用于集中管理和构建多个 ROS Package。
• Package（功能包）：工作空间中的基本功能单元，包含源码、构建配置和依赖信息。

可以把 Workspace 理解为一个项目工作区，把 Package
理解为其中一个个独立的功能模块。

## 和 CMake 的关系

ROS Workspace
    └── Catkin
          └── 基于 CMake 组织构建流程
                ├── CMakeLists.txt：构建规则
                └── catkin_make：配置并执行工作空间构建

• CMakeLists.txt 是构建规则文件，不是要在终端直接执行的命令。
• catkin_make 是终端命令，用于构建 Catkin 工作空间。
• catkin_create_pkg 是创建 Package 的命令。
• catkin_init_workspace 是初始化工作空间源码目录的命令。

