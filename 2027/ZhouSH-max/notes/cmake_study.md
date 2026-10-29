# Cmake 与 Catkin学习笔记

## 背景
- 计算机只能看懂二进制代码，所以我们写的高级语言就需要编译器转化成机器语言。Makefile就像一本自动编译说明书，告诉make工具那些文件要编译、编译的顺序是什么、需要哪些命令。这样，在我们每一次编写修改好代码之后只需敲一次make命令就能得到目标程序。
## 一、 为什么需要 CMake
- **CMake 的诞生**：当工程体量的增大，Makefile规则也越来越多，不能满足需求，于是出现了Cmake。CMake 是一个**更高层、跨平台的编译工具**。我们只需要写简单的配置文件，CMake 就能自动帮我们生成各个平台对应的 Makefile。

## 二、 Catkin：ROS 的专属编译系统
- **什么是 Catkin**：ROS 对 CMake 进行了扩展，形成了 Catkin 编译系统，专门用来管理 ROS 的庞大工程。
- **工作空间（Workspace）结构**：
  - `src`：存放源代码。
  - `build`：编译过程中的中间文件。
  - `devel`：开发空间，存放最终生成的可执行文件和环境变量。
- **功能包（Package）**：
  - 包含 `CMakeLists.txt`（编译规则）和 `package.xml`（描述信息）。
  - 创建包的命令（同时指明依赖）：`catkin_create_pkg test_pkg std_msgs rospy roscpp`。

## 三、 关于使用大模型辅助学习的思考
- **“你老是依赖棒棒糖，迟早会吃亏的。”**
- AI 是非常厉害的老师，它可以帮我解释报错、梳理概念。但如果我直接让 AI 帮我写完整的代码和笔记,自己却连读下去的耐心都没有，那就是失去了思考的能力对自己不会有任何进步。所以必须保持自己的独立性，把 AI 当工具。

## 四、 Catkin 工作空间实操流程
在 ROS 中，所有的代码都必须放在特定的工作空间中进行编译。

```bash
# 1. 检查系统是否安装了 catkin
catkin --help

# 2. 创建主文件夹，并进入
mkdir catkin_ws
cd catkin_ws

# 3. 创建存放源代码的 src 文件夹
mkdir src

# 4. 进入 src 文件夹并初始化工作空间
cd src
catkin_init_workspace

# 5. 退回工作空间根目录
cd ..

# 6. 编译整个工作空间
catkin build 
```

# 补充材料笔记

## C++ 程序是怎么变成机器码的？
### 1. 编译的步骤
我们写的 C++ 代码（`.cpp`）本质上就是一堆文本，计算机看不懂。要把它变成能运行的程序，中间要经历四个阶段：
1. **预处理**：把 `#include` 头文件展开，处理宏定义，删掉注释。（生成 `.i` 文件）
2. **编译**：把高级语言代码翻译成汇编语言（比如 `mov`, `push`）。（生成 `.s` 文件）
3. **汇编**：把汇编语言翻译成机器指令（全是 0 和 1）。（生成 `.o` 目标文件）
4. **链接**：把多个 `.o` 文件打包在一起，变成最终的可执行文件。

### 2. 什么是 ELF 文件？
在 Linux 下，`.o` 文件和最终的可执行文件，都叫 **ELF 文件**。它就像个标准化的压缩包，里面分门别类地存放了各种东西：
- `.text`：代码段（存放真正的机器指令）。
- `.data`：已初始化的全局变量。
- `.bss`：未初始化的全局变量。
- `.rodata`：只读数据（比如 `printf` 里的固定字符串）。

### 3. 静态链接和动态链接
为什么最后一步需要“链接”？因为我们的程序可能调用了别的库（比如 `printf` 函数）。
- **静态链接**：把别人写好的库代码，直接复制粘贴到我的程序里。缺点：如果 100 个程序都用了这个库，电脑里就要存 100 份，极其浪费空间。
- **动态链接（.so 文件）**：把库代码单独放在电脑里一份（就像快捷方式一样）。程序运行的时候，顺着指针去找这个库。这样既省空间，又方便升级库。

### 4. 这和CMake 有什么关系？
看了这个底层流程，我深刻理解了CMake 的作用。
如果不用 CMake，我需要手动敲 `gcc -o hello hello.cpp`，遇到链接多文件或者找库的时候，命令会非常复杂。而 CMake 就是帮我管理这些文件的依赖关系，自动指挥 gcc 去完成这复杂的四步操作。
## CMake 进阶语法与实战

### 1. 基础指令
- `cmake_minimum_required(VERSION 3.10)`：指定最低版本，必须在第一行。
- `project(MyProject CXX)`：定义项目名称和支持的语言（CXX 代表 C++）。
- `add_executable(MyApp main.cpp utils.cpp)`：将源文件编译成可执行文件。
- `add_library(MyLib STATIC library.cpp)`：生成库文件。`STATIC` 是静态库（.a/.lib），`SHARED` 是动态库（.so/.dll）。

### 2. 变量的使用
- **普通变量**：`set(MY_APP_NAME "demo")`，只在当前目录及子目录生效，每次运行重新计算。
- **缓存变量**：`set(MY_INSTALL_PATH "/usr/local" CACHE PATH "描述")`。会存入 `CMakeCache.txt` 文件，多次运行不失效。
- 使用变量时，必须加 `${}`，如 `${MY_APP_NAME}`。

### 3. 头文件与库的链接
- **添加头文件路径**：
  - 不推荐：`include_directories()`（全局污染，容易冲突）。
  - **推荐**：`target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)`。
- **链接库**：
  - `target_link_libraries(MyApp PRIVATE MyLib)`：将指定的库链接到目标（可执行文件）上。

> **理解 `PUBLIC / PRIVATE / INTERFACE`**：
> - `PRIVATE`：只给自己用（比如自己内部的私有头文件，不暴露给别人）。
> - `PUBLIC`：自己和依赖自己的人都能用（比如库的公开 API）。
> - `INTERFACE`：自己不写代码，只给依赖自己的人用（纯头文件库常用）。

### 4. 查找与使用第三方库
- 使用 `find_package(Boost REQUIRED)` 命令，CMake 会自动去系统里找这个库。
- 现代 CMake 推荐使用“导入目标”的方式，直接链接库名即可：`target_link_libraries(MyApp PRIVATE Boost::Boost)`。这会自动处理头文件和库路径，不用再手动写了。

### 5. 条件判断与自定义命令
- **条件语句**：`if(条件) ... else() ... endif()`，常用于判断构建类型（Debug 还是 Release）。
- **自定义命令**：`add_custom_command(TARGET MyApp POST_BUILD COMMAND ...)`，可以在编译完成后自动执行一些操作（比如拷贝文件、打印提示信息）。

---

## 八、 完整 CMakeLists.txt 模板

```cmake
# 1. 最低版本
cmake_minimum_required(VERSION 3.10)
# 2. 项目名与语言
project(MyProject VERSION 1.0.0 LANGUAGES CXX)
# 3. 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 添加静态库目标
add_library(MyLib STATIC src/lib/core.cpp src/lib/helper.cpp)

# 5. 添加可执行文件目标
add_executable(MyApp src/main.cpp src/utils.cpp)

# 6. 指定头文件路径
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/src)

# 7. 链接库
target_link_libraries(MyApp PRIVATE MyLib)

# 8. 安装规则（make install 执行）
install(TARGETS MyApp RUNTIME DESTINATION bin)
install(TARGETS MyLib ARCHIVE DESTINATION lib)
