# cmake && catkin 

## 编译原理

**将单个`.cpp`文件编译成一个汇编程序**

![img](https://pic1.zhimg.com/v2-b9e8babf753b1691039458592667121a_1440w.jpg)

1. 预处理preprocessing：将`#`后面的内容对应替换，除了\#pragma编译器指令，因为编译器需要使用它们。预处理得到的另一个程序通常是以.i作为文件扩展名

```bash
g++ -E hello.c -o hello.i
```

2. 编译compilation:将预处理完的文本文件hello.i翻译成文本文件hello.s，它包含一个汇编语言程序（**过程：扫描（词法分析）、语法分析、语义分析、源代码优化、代码生成、目标代码优化**）

   ```bash
   g++ -S hello.i -o hello.s
   ```

   

3. 汇编assembly:将hello.s翻译成机器语言指令，把这些指令打包成一种叫做**可重定位目标程序**的格式，并将结果保存在目标文件hello.o中，hello.o是一个二进制文件

   ```bash
   g++ -c hello.s -o hello.o
   ```

**将多个`.o`文件链接成一个文件**

4. 链接linking：把目标文件与c++标准库(libstdc++）启动代码链接，得到了`hello`文件，它是一个**可执行目标文件**

```bash
g++ hello.o -o hello
```

> 链接使得**分离编译（separate compilation）**成为可能，我们不用将一个大型的应用程序组织成一个巨大的源文件，而是可以把它分解成为更小、更好管理的模块，可以独立的修改和编译这些模块。当我们改变这些模块中的一个时，只需要简单的重新编译它，并重新链接应用，而不必重新编译其他文件。

四个步骤一步到位：`g++ hello.cpp -o hello`

**运行**

```bash
./hello
```

> **加载可执行目标文件**
> 当要运行可执行的目标文件hello，我们可以在Linux shell的命令行中输入它的名字：
> `linux> ./hello`
> 因为hello不是一个内置的shell命令，所以shell会认为hello是一个可执行目标文件，通过调用某个驻留在存储器中称为**加载器（loader）**的操作系统代码来运行它。任何Linux程序都可以通过调用execve函数来调用加载器。加载器将可执行目标文件中的代码和数据从磁盘复制到内存中，然后通过跳转到程序的第一条指令或入口点来运行该程序。这个将程序复制到内存并运行的过程叫做加载。

## 编译命令的层层封装

+ makefile:

`makefile` = `g++ 命令` *n

+ CMake:是一个跨平台的构建系统生成工具，用于管理 C/C++ 项目的编译过程。https://www.runoob.com/cmake/cmake-basic.html

  + CMakeLists.txt文件

  + **基本语法**

CMakeLists.txt 由一系列 CMake 指令组成，每个指令的格式为：**命令名(参数列表)**

**完整实例**

```cmake
# 文件路径：项目根目录/CMakeLists.txt

# 1. 指定 CMake 最低版本（必须放在最前面）
cmake_minimum_required(VERSION 3.10)

# 2. 定义项目名称、版本和语言
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

# 3. 设置 C++ 标准为 C++17，并强制要求使用该标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 添加静态库目标
add_library(MyLib STATIC
    src/lib/core.cpp      # 核心功能实现
    src/lib/helper.cpp    # 辅助函数实现
)

# 5. 添加可执行文件目标
add_executable(MyApp
    src/main.cpp           # 程序入口
    src/utils.cpp          # 工具函数
)

# 6. 为库目标指定公开头文件路径
#    PUBLIC 表示使用 MyLib 的目标也能访问该路径
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 7. 为可执行文件指定私有头文件路径
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/src)

# 8. 将 MyLib 链接到 MyApp
target_link_libraries(MyApp PRIVATE MyLib)

# 9. 设置安装规则
install(TARGETS MyApp RUNTIME DESTINATION bin)
install(TARGETS MyLib ARCHIVE DESTINATION lib)
```

> ros的catkin_make工具能够便捷地生成CMakeLists.txt模板

+ catkin工具[教程][https://wiki.ros.org/catkin/Tutorials]

### 0. 安装：

[安装教程](https://wiki.ros.org/catkin#Installing_catkin)

```bash
sudo apt-get install ros-noetic-catkin
```

### 1. 创建工作空间

```bash
 mkdir -p ~/catkin_ws/src
 cd ~/catkin_ws/
 catkin_make
```

`catkin_make`命令是用于处理`catkin workspace`的便捷工具。在工作空间中首次运行，它将在的`src`文件夹中创建一个CMakeLists.txt

此外，当前目录下，有一个 `build`和`devel`文件夹。在 `devel` 文件夹中，有几个 `setup.*sh` 文件。加载(`source`)这些文件都会将此工作空间叠加到你的环境中

```bash
source devel/setup.bash
```

 查看`ROS_PACKAGE_PATH` 环境变量 确保工作空间被`setup.bash`正确加载

```bash
echo $ROS_PACKAGE_PATH
>/home/youruser/catkin_ws/src:/opt/ros/noetic/share
```

### 2.使用catkin_make构建工作空间

**必须在catkin工作空间的顶层调用它**

```bash
cd ~/catkin_ws/src/beginner_tutorials/src
# 添加/编辑源文件
cd ~/catkin_ws/src/beginner_tutorials
#更新CMakeFiles.txt同步源代码的更改
cd ~/catkin_ws
catkin_make
```

可以将传递给[catkin_make](https://wiki.ros.org/catkin/commands/catkin_make)一些参数：

+ 指定处理某个包

```
catkin_make package_a
```

+ 可以告诉它构建安装目标：

```bash
cd ~/catkin_ws
catkin_make install
```

> 这相当于调用 '`cd ~/catkin_ws/build && make install`'。工作空间目录下会有一个[安装空间](https://wiki.ros.org/catkin/workspaces#Install_Space)（`~/catkin_ws/install`），其中包含其自身的`setup.*sh`文件。`source`这些`setup.*sh`文件可以把安装空间加载到你的环境中。

**不要同时使用两种catkin命令**

### 3. 清空工作空间

```
cd ~/catkin_ws
rm -rf build devel install
```



# LLM&&Agent

**一点废话**

AI是一个好用的工具，可以完成相当规模的任务。但是使用者的知识必须相当扎实，对AI生成的东西必须有90%理解，才能视为自己的产出。这个理解不是了解知道即可，而是自己讲得清楚：这个概念**是什么**、**为什么**要用到它、**怎么样**使用它、优化它（有没有更好的方案）真要你手搓也能搓个成品出来。

只有达到这种程度，才能说AI是个工具而不是“枪手”。当然shi上雕花的事还是要用的。

## skill与插件

**skill是一段告诉模型“应该怎么做”的指令。**

**插件是供大模型调用的工具kit**

![img](https://pic4.zhimg.com/v2-dc6e44c767dd61b520a3dbfcd4b324ab_1440w.jpg)

