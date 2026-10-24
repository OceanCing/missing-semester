# CMake 与 catkin 学习笔记

> 第二课学习记录 | kuailemeiyitian663 | 2026-10

## 一、为什么需要编译工具链

在 Linux 下的编译器 gcc/g++ 因为源文件增加而效率低下，就有了 Makefile，但工程体量增大满足不了需求就有了 CMake，而 ROS 对 CMake 扩展就有了 catkin。

## 二、从源码到可执行程序

```text
hello.c ──▶ 预处理器(cpp) ──▶ hello.i ──▶ 编译器(ccl) ──▶ hello.s ──▶ 汇编器(as) ──▶ hello.o ──▶ 链接器(ld) ──▶ hello
源程序        修改后的源程序              汇编程序               可重定位目标程序          可执行目标程序
(文本)          (文本)                   (文本)                 (二进制)                (二进制)
```

## 三、Makefile 是什么

Makefile 如同一份自动编译说明书。它告诉 make 工具：哪些文件要编译、编译顺序是什么、需要哪些命令。通过编写 Makefile，在我们每次编写修改好代码后，只需要敲一次 make 命令便能够得到你想要的目标程序。

## 四、CMake 基础

### 4.1 CMakeLists.txt 的最小结构

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

### 4.2 六个核心指令

```cmake
# 语法：cmake_minimum_required(VERSION <version>)
# 该指令必须放在 CMakeLists.txt 的最顶部
cmake_minimum_required(VERSION 3.10)

# 语法：project(<项目名> [<语言>...])
# 语言参数可选，常用值：CXX（C++）、C（C 语言）
project(MyProject CXX)

# 语法：add_executable(<目标名> <源文件>...)
add_executable(MyApp main.cpp utils.cpp)

# 语法：add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)
# STATIC：静态库（.a / .lib），编译时直接嵌入可执行文件
# SHARED：动态库（.so / .dll），运行时加载
add_library(MyLib STATIC library.cpp)

# 语法：target_link_libraries(<目标> <库>...)
target_link_libraries(MyApp PRIVATE MyLib)

# 语法：target_include_directories(<目标> [PUBLIC | PRIVATE | INTERFACE] <路径>...)
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
```

### 4.3 PUBLIC / PRIVATE / INTERFACE

```cmake
# 场景：MyLib 的头文件在 include/ 目录下
# PUBLIC：自身和消费者都能访问该路径
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# PRIVATE：只有 MyApp 自己能访问（内部工具函数头文件）
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/src/internal)

# INTERFACE：MyLib 是纯头文件库，自身无需编译，只传递路径给消费者
add_library(HeaderOnlyLib INTERFACE)
target_include_directories(HeaderOnlyLib INTERFACE ${PROJECT_SOURCE_DIR}/header_only)
```

### 4.4 外部库：find_package

```cmake
# 基本用法：查找 Boost 库（不限定版本），REQUIRED 表示找不到就中止
find_package(Boost REQUIRED)

# 指定最低版本：要求 Boost 版本不低于 1.70
find_package(Boost 1.70 REQUIRED)

# 指定搜索路径：在自定义路径下查找 OpenCV
find_package(OpenCV REQUIRED PATHS /path/to/opencv)
```

### 4.5 构建流程

```bash
# 1. 创建构建目录
mkdir build
cd build

# 2. 生成构建文件（.. 指向包含 CMakeLists.txt 的源代码目录）
cmake ..
cmake -G "Ninja" ..                    # 用 Ninja，比 Make 快
cmake -DCMAKE_BUILD_TYPE=Release ..    # Debug / Release / RelWithDebInfo / MinSizeRel

# 3. 编译和构建
cmake --build .                        # 不依赖底层构建系统
cmake --build . --target MyExecutable  # 只编指定目标
make
make -j$(nproc)                        # 多核并行
make MyExecutable
ninja                                  # Ninja 自动并行
ninja MyExecutable
msbuild MyProject.sln /p:Configuration=Release

# 4. 清理构建文件
cmake --build . --target clean
make clean
ninja clean
rm -rf build/*

# 5. 重新配置和构建（只重编变更部分）
cmake ..
cmake --build .
```

## 五、catkin

### 5.1 catkin 和 CMake 的关系

catkin = CMake 的扩展 + 一批约定。

### 5.2 建工作空间的完整命令

```bash
catkin --help          # 检查 catkin 是否安装
mkdir catkin_ws
cd catkin_ws && mkdir src
cd src
catkin_init_workspace  # 初始化工作空间
cd ..
catkin build           # 需先装 catkin_tools；ROS1 原生命令是 catkin_make
```
照着课件敲 cd /catkin_ws &mkdir src 时报错了。查下来两个问题：mkdir catkin_ws 建在家目录，路径应是 cd catkin_ws；& 是后台运行，&& 才是"前一条成功才继续"。

### 5.3 devel / build / src 各放什么

- `devel`：放可执行文件和库
- `build`：放中间文件
- `src`：放源代码

### 5.4 创建功能包

```bash
cd ~/catkin_ws/src/
# 创建 test_pkg 功能包，同时指明依赖 std_msgs rospy roscpp
catkin_create_pkg test_pkg std_msgs rospy roscpp
```

## 六、补充资料阅读笔记

### 6.1 catkin 官方 Tutorials

```bash
cd ~/catkin_ws/src/beginner_tutorials/src
# Add/Edit source files

cd ~/catkin_ws/src/beginner_tutorials
# Update CMakeLists.txt to reflect any changes to your sources

cd ~/catkin_ws
catkin_make -DCMAKE_BUILD_TYPE=Release
```
官方教程的顺序是 create workspace → create package → build，和 PPT 第 7、8 页的顺序一致，区别是官方用 catkin_make 而不是 catkin build。

### 6.2 知乎那篇

GCC 编译器驱动程序读取源程序文件 hello.c，并把它翻译成一个可执行目标文件 hello。这个翻译过程分为四个阶段：预处理（Preprocessing）、编译（Compilation）、汇编（Assembly）、链接（Linking）。执行这四个阶段的程序（预处理器、编译器、汇编器和链接器）一起构成了编译系统。

### 6.3 菜鸟教程 CMake 基础

这篇告诉我完整的 CMakeLists.txt该如何创建，以及学会了关于Cmake的指令，比如变量和缓存等等

### 6.4 The Emperor Has No Clothes

心智模型：在编写任何代码之前，先理解使用编码智能体时实际发生的流程。它本质上就是一场对话，对话对象是一个自带工具箱的强大大语言模型。

1.你发送一条消息（比如：“新建一个文件，里面写一个 hello world 函数”）
2.大语言模型判断需要调用工具，并返回结构化的工具调用请求
3.你的程序在本地执行这个工具调用（真正完成文件创建）
4.执行结果回传给大语言模型
5.大语言模型结合这份上下文继续推进任务，或是给出回复

这就是完整循环。大语言模型本身并不会直接访问你的文件系统；它只发起操作请求，由你的代码来落地执行这些操作。

编码智能体从根本上需要三类工具：

- 读取文件：让大语言模型查看你的代码
- 列出文件：让它浏览、导航整个项目
- 编辑文件：让它新建和修改代码

## 七、我用大模型学这两个工具的记录

有一次终端执行 cd /catkin_ws & 报错，我截图问ai为啥，AI 说，`&` 是后台运行符号，顺序执行多条命令要用 `&&`。
我先试 cd /catkin_ws，报“没有那个文件或目录”，才意识到是路径问题：mkdir catkin_ws 建在家目录，应该用 cd catkin_ws。另外 & 也不对，它是后台运行，要顺序执行得用 &&。两个问题叠在一起，改完路径和连接符才通.
感谢ai，什么不知道问他就对了

## 八、我对"用大模型帮助学习"的思考

大模型用好了可以帮助我们快速有效率地完成任务和学习，但用得太过依赖会使得自己变懒，不想思考。
