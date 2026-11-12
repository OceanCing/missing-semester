# CMake 与相关工具学习笔记

## 1. C/C++ 编译流程

源代码通常经过四步生成可执行文件：

- 预处理：展开头文件和宏，处理条件编译，删除注释。
- 编译：检查语法和语义、优化代码，生成汇编代码。
- 汇编：把汇编代码转换为机器指令，生成 `.o` 目标文件。
- 链接：合并目标文件与所需库，解析函数和变量引用，生成可执行文件。

目标文件通常不能独立运行，需要经过链接。

静态链接把所需库代码放入可执行文件；动态链接在加载或运行时使用共享库，减少重复存储，但运行环境需要提供兼容的库。

## 2. CMake 基础

CMake 根据 `CMakeLists.txt` 描述的源文件、依赖和编译选项，生成供 Make、Ninja 等工具使用的构建文件，再由构建工具调用编译器。

假设项目根目录已有 `main.cpp`，最小配置如下：

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app main.cpp)
```

在项目根目录执行：

```bash
cmake -S . -B build
cmake --build build
```

第一条命令配置项目并生成构建文件；第二条执行构建。构建产物放在 `build/` 中，便于与源代码分开管理。

常用指令：`add_library()` 创建库，`target_include_directories()` 设置头文件路径，`target_link_libraries()` 链接依赖，`find_package()` 查找外部包。

目标的使用要求通过 `PRIVATE`（仅自身）、`PUBLIC`（自身和使用者）、`INTERFACE`（仅使用者）控制传播。

## 3. AI 编程代理

基本流程：用户提出需求 → 模型选择工具 → 本地程序执行 → 返回结果 → 模型继续处理。

三个基础工具是读取文件、列出目录、编辑文件。模型负责决定下一步，实际文件操作由程序完成。约 200 行代码可以演示核心循环；完整产品还需要错误处理、上下文管理、命令执行和权限控制。

## 4. ROS 1 的 catkin

catkin 基于 CMake，负责组织和构建 ROS 1 软件包；ROS 2 通常使用 colcon 配合 ament。

安装并加载对应 ROS 1 环境后，在同一终端执行：

```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

`src/` 存放软件包，`build/` 存放构建中间文件，`devel/` 提供开发环境。`source` 让当前终端找到工作空间里的包和程序；修改代码后需要重新构建。

软件包用 `package.xml` 声明包信息与依赖，用 `CMakeLists.txt` 描述构建规则。

## 5. 工具之间的关系

编译器负责转换源代码；CMake 负责生成构建规则；catkin 在 CMake 基础上管理 ROS 1 软件包与工作空间。AI 编程代理通过工具协助读写代码，也可以扩展工具来执行构建。

## 参考资料

- [程序详细编译过程](https://zhuanlan.zhihu.com/p/476697014)
- [CMake 基础](https://www.runoob.com/cmake/cmake-basic.html)
- [约 200 行代码实现编程代理](https://www.mihaileric.com/The-Emperor-Has-No-Clothes/)
- [catkin 教程索引](https://wiki.ros.org/catkin/Tutorials)
