# 为什么用Cmake
- 源代码包只有编译才能在系统上运行，而linux下只有gcc，g++这两个编译器
- 源文件越来越多，用gcc和g++命令的这种方式效率低，于是用Makefile
- 工程体量太大了，Makefile也不好使，于是Cmake工具诞生，便于满足需求
# 从源代码到可执行程序(这里以一个简单的函数为例子)
> 计算机只看得懂二进制代码，我们编写的各种语言要通过编译器转换才能变成机器语言
1. 预处理
- 预处理器（cpp）将所有的#define删除，并且展开所有的宏定义。
- 处理所有的条件预编译指令，比如#if、#ifdef、#elif、#else、#endif等。
- 处理#include预编译指令，将被包含的文件直接插入到预编译指令的位置。
- 删除所有的注释。
- 添加行号和文件标识，以便编译时产生调试用的行号及编译错误警告行号。
- 保留所有的#pragma编译器指令，因为编译器需要使用它们。
- 使用gcc -E hello.c -o hello.i命令来进行预处理， 预处理得到的另一个程序通常是以.i作为文件扩展名。
2. 编译
> 过程：词法分析 → 语法分析 → 语义分析 → 源代码优化 → 目标代码生成 → 目标代码优化
- 词法分析：扫描器（Scanner）将源代的字符序列分割成一系列的记号（Token）。lex工具可实现词法扫描
- 语法分析：语法分析器将记号（Token）产生语法树（Syntax Tree）。yacc工具可实现语法分析(yacc: Yet Another Compiler Compiler)。
- 语义分析：静态语义（在编译器可以确定的语义）、动态语义（只能在运行期才能确定的语义）。
- 源代码优化：源代码优化器(Source Code Optimizer)，将整个语法书转化为中间代码（Intermediate Code）（中间代码是与目标机器和运行环境无关的）。中间代码使得编译器被分为前端和后端。编译器前端负责产生机器无关的中间代码；编译器后端将中间代码转化为目标机器代码。
- 目标代码生成：代码生成器(Code Generator).
- 目标代码优化：目标代码优化器(Target Code Optimizer)
3. 汇编
- 汇编器（as）将hello.s翻译成机器语言指令，把这些指令打包成一种叫做可重定位目标程序的格式，并将结果保存在目标文件hello.o中，hello.o是一个二进制文件。
4. 链接
- hello程序调用了printf函数，它存在于一个名为printf.o的单独的预编译好了的目标文件中，而这个文件必须以某种方式合并到我们的hello.o程序中。连接器（ld）就负责处理这种合并。结果就得到了hello文件，它是一个可执行目标文件（或者称为可执行文件），可以被加载到内存中，由系统执行。（链接程序运行需要的一大堆目标文件，以及所依赖的其它库文件，最后生成可执行文件）。
- 链接（linking）是将各种代码和数据片段收集并组合成为一个单一文件的过程，这个文件可被加载（复制）到内存并执行。链接可以执行于编译时（compile time），也就是在源代码被翻译成机器代码时；也可以执行于加载时（load time），也就是在程序被加载器（loader）加载到内存并执行时；甚至执行于运行时（run time），也就是由应用程序来执行。链接是由叫链接器（linker）的程序自动执行的。
# Makefile
- 如同自动编译说明书，告诉make工具那些文件要编译，编译顺序是什么以及实现哪些命令
- 提高了人们的效率，只需敲一次make命令便能得到你想要的目标程序
# Cmake
> CMake 是一个跨平台的构建系统生成工具，用于管理 C/C++ 项目的编译过程。通过编写 CMakeLists.txt 配置文件，你可以定义项目的构建规则、依赖关系和编译选项，CMake 会自动生成对应平台的构建文件
## Cmake基本语法
1. 指定 CMake 最低版本：cmake_minimum_required(VERSION <version>)
- cmake_minimum_required(VERSION 3.10)
2. 定义项目名称和语言：project(<项目名> [<语言>...])
- project(MyProject CXX)
3. 添加可执行文件：add_executable(<目标名> <源文件>...)
- add_executable(MyApp main.cpp utils.cpp)
4. 添加库文件：add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)
- add_library(MyLib STATIC library.cpp)
5. 链接库到目标：target_link_libraries(<目标> <库>...)
- target_link_libraries(MyApp PRIVATE MyLib)
6. 设置变量：set(<变量名> <值>...)
- set(CMAKE_CXX_STANDARD 17)
- set(SOURCES main.cpp utils.cpp helper.cpp)
 add_executable(MyApp ${SOURCES})
7. 为目标指定头文件路径：target_include_directories(<目标>
            [BEFORE | AFTER]
            [SYSTEM]
            [PUBLIC | PRIVATE | INTERFACE]
            <路径>...)
- PUBLIC：当前目标和依赖它的目标都能使用该路径
- PRIVATE：只有当前目标能使用
- INTERFACE：只有依赖当前目标的其他目标能使用
- target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
8. 设置安装规则：nstall(TARGETS <目标>...
          [RUNTIME DESTINATION <可执行文件安装路径>]
          [LIBRARY DESTINATION <动态库安装路径>]
          [ARCHIVE DESTINATION <静态库安装路径>]
          [INCLUDES DESTINATION <头文件安装路径>])
- install(TARGETS MyApp RUNTIME DESTINATION bin)
9. 条件语句：if(<条件>) ... elseif(<条件>) ... else() ... endif()
- CMake 支持的条件表达式包括：比较、逻辑运算、变量判断等
- if(CMAKE_BUILD_TYPE STREQUAL "Debug")
-   // Debug 模式下启用调试信息
-   message(STATUS "当前为 Debug 构建模式")
-   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
- else()
-   // Release 模式下启用优化
-   message(STATUS "当前为 Release 构建模式")
-   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()
## 变量和缓存
1. 普通变量
- 普通变量在 CMakeLists.txt 中定义，作用域为当前目录及其子目录。
- 普通变量在每次 CMake 运行时重新计算，不会持久化。
2. 缓存变量
- 缓存变量存储在 CMakeCache.txt 文件中，在多次 CMake 运行之间保持值不变。
- 缓存变量通常用于让用户在 CMake 配置阶段自定义构建设置，例如开关功能或指定安装路径。
## 头文件搜索路径
- include_directories() 和 target_include_directories()。 *推荐使用target_include_directories()*
## 查找库和包
- find_package()
- find_package(Boost REQUIRED)//REQUIRED *表示如果找不到则中止 CMake 配置*
- find_package(Boost 1.70 REQUIRED) *指定最低版本：要求 Boost 版本不低于 1.70*
- find_package(OpenCV REQUIRED PATHS /path/to/opencv) *在自定义路径下查找 OpenCV*
## 使用查找到的库
- 导入目标：Boost::Boost *现代 CMake 推荐方式，直接通过 target_link_libraries 使用*
- 头文件路径变量：${Boost_INCLUDE_DIRS} *旧式用法，配合 include_directories 使用*
- 库文件路径变量：${Boost_LIBRARY_DIRS} *旧式用法，配合 link_directories 使用*
# catkin
## 空间结构
- catkin_ws/
- |————src/*package源代码包*
- |————build/*缓存和中间文件*
- |————devel*目标文件*
## 各种catkin工具
- catkin init
- catkin create pkg
- catkin config
- catkin build
- catkin list
- catkin clean
