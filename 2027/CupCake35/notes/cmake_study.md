# 总览：Cmake，catkin，关于补充资料，关于大模型

# Cmake

- 随着文件和依赖增加，我们需要维护的Makefile规则也会越来越多。单纯靠写makefile也会变得不方便。
- CMake是一种过程式语言，是一个跨平台的编译工具，可以用简单的语句来描述所有平台的编译过程。它能够产生标准的建构档makefile或者project文件。
- 单纯靠makefile来维护项目的构建做不到灵活地跨平台，cmake解决了这个问题。
- 我们使用cmake通过编写简单的配置文件去生成本地的Makefile，这个配置文件是独立于运行平台和编译器的，不管在什么平台用什么编译器，cmake都可以生成对应的makefile。

- 流程
1. 你的代码 (.cpp/.hpp)
2. g++ 编译器 ，但 g++ 要一堆参数
3. 可执行文件

- 使用了Cmake
1. CMakeLists.txt
2. CMake 读取
3. Makefile 
4. make 执行
5. 编译器  
6. 程序

# catkin 
- catkin 是站在 CMake 肩膀上的一层封装。 它没有自己的构建引擎，底层照样是 CMake + make，只是额外提供了一堆 CMake 宏（catkin_package()、add_message_files() 等）和 Python 工具（catkin_make、catkin_create_pkg），再配套一套“包 + 工作空间”的约定。
- catkin--help##检查catkin是否安装
- mkdir catkin_ws
- cd /catkin_ws &mkdir src
- cd src
- catkin_init_workspace ##初始化工作空间
- cd ..
- catkin build
- devel是开发空间，放置开发过程中的可执行文件和库
- build是开发过程中的一些中间文件，二进制文件，我们基本上是用不到的

- 开发空间
catkin_ws/                      ← 工作空间根目录（例如名字catkin_ws）
├── src/                        ← 只有这个目录是你手写的
│   ├── CMakeLists.txt          ← 可选，链接到 /opt/ros/noetic/share/catkin/cmake/toplevel.cmake
│   ├── vision_pkg/             ← 一个"包"
│   │   ├── package.xml         ← 包的身份证：名字、版本、依赖
│   │   ├── CMakeLists.txt      ← 包的构建规则
│   │   ├── msg/GimbalCmd.msg   ← 消息定义
│   │   ├── include/            ← 头文件
│   │   ├── src/                ← 源文件
│   │   └── launch/             ← 启动文件
│   └── control_pkg/
├── build/                      ← 自动生成，CMake 中间文件
├── devel/                      ← 自动生成，编译产物 + setup.bash
└── install/                    ← 只有 catkin_make install 才有
- build/ 和 devel/ 都是 catkin_make 自动生成的，你永远不要手动往里面放东西，也不要提交到 git（.gitignore 里加 build/ devel/）。
- CMake 你手动 mkdir build；catkin 里 catkin_make 会自己建好 build/，而且必须在工作空间根目录执行。

# 关于补充资料

## 预处理器（cpp）

1. 将所有的#define删除，并且展开所有的宏定义。
2. 处理所有的条件预编译指令，比如#if、#ifdef、#elif、#else、#endif等。
3. 处理#include预编译指令，将被包含的文件直接插入到预编译指令的位置。
4. 删除所有的注释。
5. 添加行号和文件标识，以便编译时产生调试用的行号及编译错误警告行号。
6. 保留所有的#pragma编译器指令，因为编译器需要使用它们。
7. 使用gcc -E hello.c -o hello.i命令来进行预处理， 预处理得到的另一个程序通常是以.i作为文件扩展名。

## 编译阶段

编译器（ccl）将预处理完的文本文件hello.i进行一系列的词法分析、语法分析、语义分析和优化，翻译成文本文件hello.s，它包含一个汇编语言程序。

## 汇编阶段
- 汇编器（as）将hello.s翻译成机器语言指令，把这些指令打包成一种叫做可重定位目标程序的格式，并将结果保存在目标文件hello.o中，hello.o是一个二进制文件。

## 链接阶段

- hello程序调用了printf函数，它存在于一个名为printf.o的单独的预编译好了的目标文件中，而这个文件必须以某种方式合并到我们的hello.o程序中。连接器（ld）就负责处理这种合并。结果就得到了hello文件，它是一个可执行目标文件（或者称为可执行文件），可以被加载到内存中，由系统执行。（链接程序运行需要的一大堆目标文件，以及所依赖的其它库文件，最后生成可执行文件）。

## 一个典型的ELF可重定位目标文件包含下面几个节

- .text：已编译程序的机器代码。

- .rodata：只读数据，比如printf语句中的格式串和开关语句的跳转表。

- .data：已初始化的的全局和静态C变量。局部C变量在运行时被保存在栈中，即不出现在.data节中，又不出现在.bss节中。

- .bss：未初始化的全局和静态C变量，以及所有被初始化为0的全局或静态变量。在目标文件中这个节不占据实际的空间，它仅仅是一个占位符。目标文件格式区分已初始化和未初始化变量是为了空间效率：在目标文件中，未初始化变量不需要占据任何实际的磁盘空间。运行时，在内存中分配这些变量，初始值为0。

- .symtab：一个符号表，它存放着在程序中定义和引用的函数和全局变量的信息。

- .rel.text：一个.text节中位置的列表，当链接器把这个目标文件和其他文件组合时，需要修改这些位置。一般而言，任何调用外部函数或者引用全局变量的指令都需要修改。另一方面，调用本地函数的指令则不需要修改。

- .rel.data：被模块引用或定义的所有全局变量的重定位信息。一般而言，任何已初始化的全局变量，如果它的初始值是一个全局变量地址或者外部定义函数的地址，都需要被修改。

- .debug：一个调试符号表，其条目是程序定义的局部变量和类型定义，程序中定义和引用全局变量，以及原始的C源文件。

- .line：原始C源程序中的行号和.text节中的机器指令之间的映射。

- .strtab：一个字符串表，其内容包括.symtab和.debug节中的符号表，以及节头部中的节名字。字符串表就是以null结尾的字符串序列。

## cmake

语法：cmake_minimum_required(VERSION <version>)
该指令必须放在 CMakeLists.txt 的最顶部
示例：要求 CMake 版本不低于 3.10
cmake_minimum_required(VERSION 3.10)
指定 CMake 最低版本：

实例
语法：cmake_minimum_required(VERSION <version>)
该指令必须放在 CMakeLists.txt 的最顶部
示例：要求 CMake 版本不低于 3.10
cmake_minimum_required(VERSION 3.10)
定义项目名称和语言：

实例
语法：project(<项目名> [<语言>...])
语言参数可选，常用值：CXX（C++）、C（C 语言）
调用 project() 后会自动设置 PROJECT_NAME 等变量
project(MyProject CXX)
添加可执行文件：

实例
语法：add_executable(<目标名> <源文件>...)
将指定的源文件编译生成一个可执行文件
add_executable(MyApp main.cpp utils.cpp)
添加库文件：

实例
语法：add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)
STATIC：静态库（.a / .lib），编译时直接嵌入可执行文件
SHARED：动态库（.so / .dll），运行时加载
不指定类型时，由 BUILD_SHARED_LIBS 变量决定
add_library(MyLib STATIC library.cpp)
链接库到目标：

实例
语法：target_link_libraries(<目标> <库>...)
将指定的库链接到目标（可执行文件或其他库）
可以链接自己项目中的库目标，也可以链接外部库
target_link_libraries(MyApp PRIVATE MyLib)
设置变量：

实例
语法：set(<变量名> <值>...)
定义一个普通变量，后续通过 ${变量名} 引用
set(CMAKE_CXX_STANDARD 17)

同时设置多个值（列表）
set(SOURCES main.cpp utils.cpp helper.cpp)
add_executable(MyApp ${SOURCES})
为目标指定头文件路径：

实例
```语法：target_include_directories(<目标>
           [BEFORE | AFTER]
          [SYSTEM]
           [PUBLIC | PRIVATE | INTERFACE]
           <路径>...)
 PUBLIC：当前目标和依赖它的目标都能使用该路径
 PRIVATE：只有当前目标能使用
 INTERFACE：只有依赖当前目标的其他目标能使用
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
```
设置安装规则：

实例
```语法：install(TARGETS <目标>...
         [RUNTIME DESTINATION <可执行文件安装路径>]
         [LIBRARY DESTINATION <动态库安装路径>]
         [ARCHIVE DESTINATION <静态库安装路径>]
         [INCLUDES DESTINATION <头文件安装路径>])
 定义执行 make install 时各类型文件的安装位置
install(TARGETS MyApp RUNTIME DESTINATION bin)
```
条件语句：

实例
 语法：if(<条件>) ... elseif(<条件>) ... else() ... endif()
 CMake 支持的条件表达式包括：比较、逻辑运算、变量判断等
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug 模式下启用调试信息
    message(STATUS "当前为 Debug 构建模式")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
    # Release 模式下启用优化
    message(STATUS "当前为 Release 构建模式")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()
自定义命令：

实例
``` 语法：add_custom_command(
          TARGET <目标>
          PRE_BUILD | PRE_LINK | POST_BUILD
          COMMAND <命令> [参数...]
          [COMMENT <说明>]
          [VERBATIM])
        PRE_BUILD：在编译前执行
        PRE_LINK：在链接前执行
         POST_BUILD：在构建完成后执行
add_custom_command(
    TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "构建完成！"
    COMMENT "打印构建完成信息"
)
```
完整实例
下面是一个完整的 CMakeLists.txt 示例，综合使用了上述多个指令。

实例
文件路径：项目根目录/CMakeLists.txt

 1. 指定 CMake 最低版本（必须放在最前面）
cmake_minimum_required(VERSION 3.10)

 2. 定义项目名称、版本和语言
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

 3. 设置 C++ 标准为 C++17，并强制要求使用该标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

 4. 添加静态库目标
add_library(MyLib STATIC
    src/lib/core.cpp      # 核心功能实现
    src/lib/helper.cpp    # 辅助函数实现
)

 5. 添加可执行文件目标
add_executable(MyApp
    src/main.cpp           # 程序入口
    src/utils.cpp          # 工具函数
)

 6. 为库目标指定公开头文件路径
    PUBLIC 表示使用 MyLib 的目标也能访问该路径
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

 7. 为可执行文件指定私有头文件路径
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/src)

 8. 将 MyLib 链接到 MyApp
target_link_libraries(MyApp PRIVATE MyLib)

 9. 设置安装规则
install(TARGETS MyApp RUNTIME DESTINATION bin)
install(TARGETS MyLib ARCHIVE DESTINATION lib)
构建这个项目的典型命令如下：

 在项目根目录下创建 build 目录（推荐外部构建）
$ mkdir build && cd build

 生成构建文件
$ cmake ..

 编译项目
$ cmake --build .

 安装到系统目录（可选）
$ cmake --install .

## 变量和缓存
CMake 使用变量来存储和传递配置信息，变量可以在 CMakeLists.txt 中定义和使用。

变量分为两类：普通变量和缓存变量，它们的作用域和生命周期不同。

## 普通变量
普通变量在 CMakeLists.txt 中定义，作用域为当前目录及其子目录。

普通变量在每次 CMake 运行时重新计算，不会持久化。

## 缓存变量
缓存变量存储在 CMakeCache.txt 文件中，在多次 CMake 运行之间保持值不变。

缓存变量通常用于让用户在 CMake 配置阶段自定义构建设置，例如开关功能或指定安装路径。

缓存变量的值会持久化。如果你修改了 CMakeLists.txt 中缓存变量的默认值，需要删除 CMakeCache.txt 或重新运行 cmake 才能让新默认值生效。

## 头文件搜索路径
在 CMake 中，有两种方式可以指定编译器查找头文件的路径：include_directories() 和 target_include_directories()。

## 查找库和包
很多项目依赖外部库（如 Boost、OpenCV），CMake 提供了 find_package() 指令来自动检测和配置这些外部依赖。

find_package 指令
find_package() 会搜索系统中已安装的库，并设置对应的变量（如头文件路径、库文件路径、库目标名称）。

## 关于大模型

deepseek harness从这两样工具的作用开始讲起，涉及到它们的优势等内容。
当我询问一些指令，ai会给出，例如cmake_minimum_required(VERSION 3.16)等，同时附上作用，以及注意事项和常见的错误




 
