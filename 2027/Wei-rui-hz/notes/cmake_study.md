# Lesson 2 Notes: CMake、Catkin 与大模型使用他入门（草稿，过段时间继续补齐）

## 一、从源代码到可执行程序

## 1. 编译的本质

计算机只能执行二进制机器码，我们写的 C++ 等高级语言代码必须经过**编译器**（Linux 下常用`gcc`/`g++`）翻译成低级机器语言才能运行

GCC编译器驱动程序读取源程序文件hello.c，并把它翻译成一个可执行目标文件hello。这个翻译过程分为四个阶段：**预处理（Preprocessing）**、**编译（Compilation）**、**汇编（Assembly）**、**链接（Linking）**。执行这四个阶段的程序**（预处理器、编译器、汇编器、和链接器）**一起构成了编译系统。

完整的转换过程分为四步：

1. **预处理**：处理`#include`、`#define`等宏，生成 `.i` 文件
2. **编译**：把预处理后的代码经过**扫描（词法分析）**、**语法分析**、**语义分析**、**源代码优化**、**代码生成**、**目标代码优化**这6个步骤后翻译成汇编代码`.s`
3. **汇编**：汇编器（as）将汇编代码`.s`翻译成机器语言指令，把这些指令打包成一种叫做**可重定位目标程序**的格式，并将结果保存在目标文件`.o`中
4. **链接**：把多个目标文件和库由连接器（ld）链接成最终的**可执行目标文件**（也称为**可执行程序**）

```bash
g++ main.cpp -o main    #预处理 → 编译 → 汇编 → 链接
```

### 2. 构建工具的演进

| 工具 | 解决的问题 | 局限 |
|------|-----------|------|
| `gcc/g++` | 单文件直接编译 | 文件较多时手动写命令费时费力 |
| `Makefile` | 自动管理编译顺序和依赖 | 跨平台能力差，规则维护成本高 |
| `Cmake` | 用简单配置描述构建，跨平台生成Makefile | 需要额外学一门脚本语法 |

**Makefile** 就像一份自动编译说明书，告诉 `make` 哪些文件要编译、顺序和命令。改完代码敲一次 `make` 即可，更加便捷。

**Cmake** 是更高层的构建工具，它本身不编译，而是生成平台相关的 Makefile 或工程文件。ROS 基于 Cmake 扩展出了 Catkin 编译系统。

## 二、Cmake

### 1. Cmake 的作用

CMake **不负责编译**，而是读取 `CMakeLists.txt`，生成当前平台对应的构建文件（Linux 下是 Makefile）。

```text
CMakeLists.txt ──cmake──▶ Makefile ──make──▶ 可执行文件
```

### 2. 最小项目

```text
hello/
├── CMakeLists.txt
└── main.cpp
```

```cmake
# CmakeLists.txt
cmake_minimum_required(VERSION x.y.z)
project(hello)

add_executable(hello main.cpp)
```

```cpp
// main.cpp
#include<iostream>
int main(){
    std::cout << "Hello World" << std::endl;
    return 0;
}
```

### 3. 构建流程

```bash
mkdir build && cd build   # 独立的构建目录
cmake ..                  # 生成 Makefile
make                      # 编译
./hello                   # 运行
```

好处：`build/` 可随时删除重来，不会污染源码目录。

#### 4. 常用命令

| 指令 | 作用 |
|------|------|
| `cmake_minimum_required(VERSION x.y)` | 声明最低 CMake 版本 |
| `project(名称)` | 定义项目 |
| `add_executable(目标 源文件...)` | 生成可执行文件 |
| `add_library(库名 源文件...)` | 生成库（静态/动态） |
| `target_link_libraries(目标 库...)` | 链接依赖 |
| `find_package(包 REQUIRED)` | 查找外部依赖 |
| `include_directories(路径)` | 添加头文件路径 |
| `add_subdirectory(目录)` | 引入子目录 |
