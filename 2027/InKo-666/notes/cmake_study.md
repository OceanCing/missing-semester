## CMake

> Makefile -->自动编译说明书  
> 说明了:要编译的文件，编译顺序，需要的命令 

    ===============================================================================================================
     阶段    输入文件        处理程序(工具)       输出文件              文件说明                 文件类型
    ===============================================================================================================
    预处理  hello.c    ──> 预处理器(cpp)   ──>  hello.i              修改了的源程序             文本
    编译    hello.i    ──> 编译器(ccl)     ──>  hello.s              汇编程序                   文本
    汇编    hello.s    ──> 汇编器(as)      ──>  hello.o              可重定位目标程序           二进制
    链接    hello.o    ──┐
            printf.o  ──┴> 链接器(ld)      ──>  hello                可执行目标程序             二进制
    ===============================================================================================================

## Catkin
    catkin --help             检查catkin否安装
    mkdir catkin_ws 
    cd /catkin_ws &mkdir src
    cd src
    catkin_init_workspace     初始化工作空间
    cd ..
    catkin build

## Package功能包

    - CMakeLists.txt    package的编译规则(必须)
    - package.xml       package的描述信息(必须)
    - src/              源代码文件
    - include/          C++头文件
    - scripts/          可执行脚本
    - msg/              自定义消息
    - srv/              自定义服务
    - models/           3D模型文件
    - urdf/             urdf文件
    - launch/           launch文件

> 创建test_pkg功能包，同时指明依赖std_msgs rospy roscpp
>* catkin_create_pkg test_pkg   
>* std_msgs rospy roscpp

## 知乎上面的教程
https://zhuanlan.zhihu.com/p/476697014
* 源程序实际上就是一个由0和1组成的位（比特）序列，8个位被组织成一组为***字节***

* 像 hello.c这样只由ASCII字符构成的文本称为***文本文件***，其他所有的文件都称为***二进制文件***

C语句 ---> 一系列低级机器语言指令 ---> 以***可执行目标程序***格式打包 ---> 以***二进制磁盘文件***格式存储

#### 预处理
* 1.预处理器（cpp）将所有的#define删除，并且展开所有的宏定义。  

* 2.处理所有的条件预编译指令，比如#if、#ifdef、#elif、#else、#endif等。  
* 3.处理#include预编译指令，将被包含的文件直接插入到预编译指令的位置。  
* 4.删除所有的注释。  
* 5.添加行号和文件标识，以便编译时产生调试用的行号及编译错误警告行号。  
* 6.保留所有的#pragma编译器指令，因为编译器需要使用它们。  
* 7.使用gcc -E hello.c -o hello.i命令来进行预处理， 预处理得到的另一个程序通常是以.i作为文件扩展名。  

#### 编译
> 将处理完的文本文件hello.i进行***词法，语法，语义分析和优化***，翻译成文本文件hello.s

* 编译过程可分为6步：**扫描（词法分析）、语法分析、语义分析、源代码优化、代码生成、目标代码优化**

* 1.词法分析：扫描器（Scanner）将源代的字符序列分割成一系列的记号（Token）。lex工具可实现词法扫描。  
* 2.语法分析：语法分析器将记号（Token）产生语法树（Syntax Tree）。yacc工具可实现语法分析(yacc: Yet Another Compiler Compiler)。  
* 3.语义分析：静态语义（在编译器可以确定的语义）、动态语义（只能在运行期才能确定的语义）。  
* 4.源代码优化：源代码优化器(Source Code Optimizer)，将整个语法书转化为中间代码（Intermediate Code）（中间代码是与目标机器和运行环境无关的）。  中间代码使得编译器被分为前端和后端。编译器前端负责产生机器无关的中间代码；编译器后端将中间代码转化为目标机器代码。  
* 5.目标代码生成：代码生成器(Code Generator).
* 6.目标代码优化：目标代码优化器(Target Code Optimizer)。  
> 好吧其实一点也不懂，先抄下来

#### 汇编
> 将hello.s翻译成机器语言指令，把这些指令打包成一种叫做**可重定位目标程序**的格式，并将结果保存在目标文件hello.o中，hello.o是一个***二进制文件***

#### 链接
> * ***处理目标文件与程序的合并***  
>* 将各种代码和数据片段收集并组合成为一个单一文件

    比如:程序调用了printf函数，这个函数是存在于printf.o里面的，连接器把它合并到程序里，才能让printf发挥作用

* 处理后，得到可执行文件，可以被加载到内存中，由系统执行

*链接可以执行于编译时（compile time），也就是在源代码被翻译成机器代码时；  
可以执行于加载时（load time），也就是在程序被加载器（loader）加载到内存并执行时；  
可以执行于运行时（run time），也就是由应用程序来执行*

* 这个可以让其中单个文件编译再链接，而·不用使得别的文件再编译。

### 虚拟内存
* 1. 让主存主要保存活动区域，高效利用主存
* 2. 为每个进程提供统一的地址空间，简化存储器管理
* 3. 为每个进程提供独立的地址空间，保护每个进程的地址空间不被其他进程破坏
 > 虚拟存储器是假想的存储器，而虚拟存储空间是假想的内存

后面的都不会了，待续

### 菜鸟教程CMake

指定 CMake 最低版本：
```cpp
cmake_minimum_required(VERSION 3.10)
# 版本不低于3.10
```
定义项目名称和语言：
```cpp
project(<项目名> [<语言>...])

如： project(MyProject CXX)
```
添加可执行文件：
```cpp
add_executable(<目标名> <源文件>...)

如： add_executable(MyApp main.cpp utils.cpp)
```
添加库文件：
```cpp
add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)

# STATIC：静态库（.a / .lib），编译时直接嵌入可执行文件
# SHARED：动态库（.so / .dll），运行时加载
# 不指定类型时，由 BUILD_SHARED_LIBS 变量决定

如： add_library(MyLib STATIC library.cpp)
```
链接库到目标：
```cpp
target_link_libraries(<目标> <库>...)

# 将指定的库链接到目标（可执行文件或其他库）
# 可以链接自己项目中的库目标，也可以链接外部库

如： target_link_libraries(MyApp PRIVATE MyLib)
```
设置变量：
```cpp
set(<变量名> <值>...)

如： set(SOURCES main.cpp utils.cpp helper.cpp)
引用时用 ${SOURCES}
```
为目标指定头文件路径：
```cpp
target_include_directories(<目标>[BEFORE | AFTER]
           [SYSTEM]
           [PUBLIC | PRIVATE | INTERFACE]
           <路径>...)
# PUBLIC：当前目标和依赖它的目标都能使用该路径
# PRIVATE：只有当前目标能使用
# INTERFACE：只有依赖当前目标的其他目标能使用

如： target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
```
设置安装规则：
```cpp
install(TARGETS <目标>...
         [RUNTIME DESTINATION <可执行文件安装路径>]
         [LIBRARY DESTINATION <动态库安装路径>]
         [ARCHIVE DESTINATION <静态库安装路径>]
         [INCLUDES DESTINATION <头文件安装路径>])
 # 定义执行 make install 时各类型文件的安装位置

 如： install(TARGETS MyApp RUNTIME DESTINATION bin)
 ```
 条件语句：
 ```cpp
 if(<条件>) ... elseif(<条件>) ... else() ... endif()

 如： if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug 模式下启用调试信息
    message(STATUS "当前为 Debug 构建模式")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
    # Release 模式下启用优化
    message(STATUS "当前为 Release 构建模式")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()
```
自定义命令：
```cpp
add_custom_command(
          TARGET <目标>
          PRE_BUILD | PRE_LINK | POST_BUILD
          COMMAND <命令> [参数...]
          [COMMENT <说明>]
          [VERBATIM])
# PRE_BUILD：在编译前执行
# PRE_LINK：在链接前执行
# POST_BUILD：在构建完成后执行

如： add_custom_command(
    TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "构建完成！"
    COMMENT "打印构建完成信息"
)
```

#### 完整示例
```cpp
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