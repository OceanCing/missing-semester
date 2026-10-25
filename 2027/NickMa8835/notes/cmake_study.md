# CMake和catkin 学习笔记

一个C/C++源文件变成可执行文件前需要经历四个阶段

1. **预处理（Preprocessing）**：处理以 `#` 开头的指令，如 `#include`（把头文件内容展开进来）、`#define`（宏替换）、条件编译 `#ifdef` 等，生成扩展后的 `.i` 文件。对应命令 `gcc -E main.c -o main.i`。
2. **编译（Compilation）**：把预处理后的代码翻译成汇编语言，生成 `.s` 文件。对应命令 `gcc -S main.i -o main.s`。这一步才真正开始进行语法、语义分析。
3. **汇编（Assembly）**：把汇编代码翻译成机器码，生成目标文件 `.o`。对应命令 `gcc -c main.s -o main.o`。
4. **链接（Linking）**：把一个或多个目标文件以及用到的库（静态库 `.a`、动态库 `.so`）合并成最终的可执行文件。对应命令 `gcc main.o -o main` 或直接 `ld`。

当只有一两个文件需要处理时，手动写gcc还可以接受，但当项目i多起来时，手动写就变得不显示了。Make和CMake都是为了解决这个问题而生的，它们可以负责“怎么编译、编译谁、链接谁”这套规则的描述

## CMake学习笔记

### 1.CMake是什么

CMake是一个跨平台的构建系统生成工具，它本身并不直接编译代码，而是读取我们用CMakelists.txt 描述的构建规则，生成对应的构建文件（在Linux系统下生成makefile）

### 2. 常用指令

| 指令                                                 | 作用                                               |
| ---------------------------------------------------- | -------------------------------------------------- |
| `cmake_minimum_required(VERSION 3.10)`               | 指定 CMake 最低版本，必须放在文件最顶部            |
| `project(MyProject CXX)`                             | 定义项目名和语言，并自动设置 `PROJECT_NAME` 等变量 |
| `add_executable(MyApp main.cpp utils.cpp)`           | 把源文件编译成可执行文件                           |
| `add_library(MyLib STATIC library.cpp)`              | 生成静态库（`STATIC`）或动态库（`SHARED`）         |
| `target_link_libraries(MyApp PRIVATE MyLib)`         | 把库链接到目标                                     |
| `set(CMAKE_CXX_STANDARD 17)`                         | 设置变量（如 C++ 标准）                            |
| `target_include_directories(MyApp PRIVATE include/)` | 为目标指定头文件搜索路径                           |
| `install(TARGETS MyApp RUNTIME DESTINATION bin)`     | 定义 `make install` 时的安装位置                   |
| `find_package(Boost REQUIRED)`                       | 查找外部依赖库                                     |
| `if(...) ... elseif(...) ... else() ... endif()`     | 条件分支                                           |
| `add_subdirectory(dir)`                              | 引入子目录的构建                                   |



## catkin学习笔记

### 1.catkin是什么

是ROS官方定制的一套构建系统，在CMake上做了拓展，专门解决机器人项目"包数量多、包之间依赖复杂、需要统一工作空间管理"的问题。对使用者来说，日常命令是 `catkin_make` 而不是直接调 `cmake`，但其底层仍是 CMake，语法也是 CMake 语法，配置文件同样叫 `CMakeLists.txt`。

### 2 catkin 工作空间结构

一个典型的 catkin 工作空间包含四个目录：

| 目录       | 名称                      | 作用                                    |
| ---------- | ------------------------- | --------------------------------------- |
| `src/`     | 源空间（source space）    | 存放包的源代码                          |
| `build/`   | 构建空间（build space）   | CMake 生成中间文件、实际编译的地方      |
| `devel/`   | 开发空间（devel space）   | 编译产物与 `setup.bash`，供开发调试使用 |
| `install/` | 安装空间（install space） | 可选，执行安装后的产物                  |

### 3 创建工作空间

```bash
source /opt/ros/$ROS_DISTRO/setup.bash   # 先加载 ROS 环境
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace                    # 初始化工作空间
cd ~/catkin_ws
catkin_make                              # 首次构建
source devel/setup.bash                  # 让 ROS 能找到本工作空间
```

以后每一个项目是一个package，只要把package丢到工作空间中就可以，不用每次都重建

### 4 创建一个 catkin 包

一个合法的 catkin 包必须包含两个文件：

- `package.xml`：描述包的基本信息（名称、版本、维护者、依赖等），格式符合 catkin 规范；
- `CMakeLists.txt`：使用 catkin 的 CMake 构建配置。

用 `catkin_create_pkg` 可以自动生成带模板的包：

```bash
cd ~/catkin_ws/src
catkin_create_pkg 项目名 std_msgs rospy roscpp
```

这条命令会创建项目名文件夹，并根据你给出的依赖（`std_msgs`、`rospy`、`roscpp`）**部分填充**好 `package.xml` 和 `CMakeLists.txt`，之后我们再往里面补充自己的源码。

### 5 构建与使用包

```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

- `catkin_make` 会构建 `src/` 下的所有包，在 `build/` 中编译，产物放到 `devel/`。
- `devel/` 里会生成 `setup.bash` 等环境文件，`source` 后会把本工作空间"叠加"到 ROS 环境之上，这样 `rospack`、`rosrun`、`roslaunch` 才能找到你新加的包。
- 开发调试时一般用 `devel` 空间；发布部署时才用 `install` 空间，二者不要同时混用。

## 补充资料学习心得

## The Emperor Has No Clothes

大模型的能力来自阅读资料 + 调用工具 + 循环反馈这个朴素机制，总共其实就是200行的python代码，没有想象中的高级玄妙。

一方面：大模型确实能帮我快速读文档、解释概念、给示例，降低阅读门槛、加速试错

但另一方面：它给出的内容需要经过真实实验验证，学习时要多问为什么，而不是让大模型牵着鼻子走



在本次学习过程中，我也用了大模型辅助我学习，我把完整问题和四篇文章的链接发给了codex，它只用了几分钟就完成了爬取文章，汇总概要，生成它的学习笔记，但在我一节节读它的笔记并用自己的话重新写我的笔记时确发现需多地方它仅仅机械地给出解读，对于一个刚开始学习本章内容的人来说根本看不懂，所以我卡了很久，每次遇到问题就重新思考并向它发问。

这个过程让我看清了大模型的边界和局限，只能把它当作辅助查阅整理资料、检查代码的工具，但实际的操作还是需要自己一遍遍的试错纠正