# 第二次培训笔记

## 课堂上的笔记

### CMake

管理代码构建的工具，与平台和构建系统无关
根据自己写的配置文件（CMakeListstxt），用于生成不同版本的Makefile
(写一套配置可以在Windows、Linus、Makefile内使用)

不编译代码，只是单纯地读取写的配置文件，让它在不同的操作系统，不同编译器，不同ide上复用
（调用原生工具需要自己操作）

可以在一定程度上简化c语言和cpp第三方的引入和使用流程

- CMakeList.txt:
构建脚本，告诉CMake用哪些源文件，用什么标准，哪些链接

-  .cpp/.h:实际业务代码

例如：
MyProject/
|——————CMakeList.txt  :  用来写CMake配置
|——————main.cpp       :  C++写的源代码
|——————math/
|    |————————add.cpp
|    |————————add.h
|
|————————build/       ：构建目录，cmake生成的文件放在这里

#### 唯一的交汇：

CMake可以用configure_file把配置变量（比如版本号生成到一个  .h的头文件里面 然后在.cpp里面#include 它）
cpp里面从来不写CMake语法

*有些后面有就没有补上*

skill
一组可复用的任务指令和配套资源，指导agent完成特定类型任务
可选资源：脚本/参考文档/模板

AGENTS
类似项目给ai看的说明书

用于ai编程agent提供项目背景/开发规范/工作指引/提升工作

---


## 链接笔记1

**文件名字必须叫做CMakeList.txt**

### 核心指令

- **cmake_minimun_required(VERSION 3.10)**
*指定最低使用Cmake版本*
- **project（MyProject）**
*定义项目名字*
- **add_executable(hello main.cpp)**
*生成可执行文件*

- 第一行:
固定命令名字
cmake_minimute_required()

- 第二行：
project(Myproject)
*外面的project是固定名字，里面的Myproject自己命名*

- 第三行：
add_executable:（hello main.cpp）
固定名字，生成可执行文件
*main.cpp 是个人的文件名字*


### 假设

CMakeList.txt和main.cpp  在同一个目录

#### 创建并进入build目录之中（*推荐外部构建，避免污染目录*）

- mkdir build
- cd build

#### 让CMake读取上级目录的CMakeList.txt，生成Makefile
 
cmake ..

#### 执行编译（或者直接make）

cmake --build .

#### 运行生成之后的可执行文件
./hello

---
编译成功之后，可执行文件hello会出现在build目录里面，   ./hello就可以执行
---

***语言变化不是改变CMake模式，而是切换语言（c，cpp，CUDA，Fortran等语言）

### 进阶配置示例

set（CMAKE_CXX_STANDARD 17）
set (CMAKE_CXX_STANDARD_REQUIRED ON)

- 生成静态库

add_library(mymath STATIC math/add.cpp)

- 可执行文件连接库、指定头文件路径

add_executable(app main.cpp)
target_link_libraries(app PRIVATE mymath)
target_include_directories(add PRIVATE./math)

PRIVATE/PUBLIC  :用来管理依赖的传递性，是现代CMake的最佳实践

---

### 其他语言特性

- 变量引用：$(VAR)  :取值；if语句里面直接写明变量名字
- 作用域：function（）有独立作用域，改变外部变量需要PARENT_ACORE ;
macro() 是文本替换。直接生效
- 流程控制 ：
if（）/elseif（）/else（）/endif（）/foreach（）/while（）

*一个可执行文件可以对应多个 .cpp*
add_executable(hello main.cpp math.cpp utils.cpp)
上面的全部翻译成hello去执行
追加

**.cpp 里面必须有main函数，（通常只有一个），作为程序入口
其他  .cpp 提供函数 /类 给main调用

一个



---


链接笔记2：

## 程序详细编译过程（预处理、编译、汇编、链接）
例子：
'''
#include<stdio>
int main()
{
    printf("hello,world\n");
    return 0；
}
'''
源文件/源程序：写的代码在的地方，实际为1与0组成的位/比特，8个位被放在一组，称为字节（程序中文本字符）
只有ASCII字符构成的文本：文本文件
其他二进制文件

系统运行————c语句需要转化为可执行目标文件（二进制磁盘文件存放）

在Unix系统上，从源文件到目标文件的转换是由*编译器驱动*程序完成的
---
'''
linus> gcc -o hello hello.c
'''
### 四个阶段

1、预处理（hello.c 变成 hello.i）
- 删除#define，展开宏定义
- 处理预编程指令*#if、#ifdef、#dlif、#else、#endif*
- 处理“#include”预编译指令，将被包含的文件插入预编译指令位置
- 删除所有注释
- 添加行号和文件标识（以便编译调试用到的行号以及编译错误警告行号）
- 保留所有#pragma编译器指令，因为编译器需要使用
<!-- 
#pragma指令例子
1、#pragma pack：控制结构体对起
'''
#pragma pack(n)   设置后续结构体的最大对齐边界为n字节

推荐使用pop/push
#pragma pack(pop) :恢复之前的对齐状态
-----------
2、#pragma once ：防止头文件重复包含
-->
- 使用gcc -E hello.c -o hello.i命令来预处理，预处理得到的另外一个程序通常是以 .i 作为文件拓展名


2、编译
main：
 subq   $8,%rap
 movl   $.LCO,%edi
 call   puts
 addq   $8,%rap
 ret
<!-- 低级机器语言指令 -->

编译分为：
- 词法分析（扫描）：扫描器把源代码的字符分割成一系列的记号（lex工具可用）
- 语法分析：产生语法树（yacc工具）
- 语义分析：静态语法（在编译器里面可确定）、动态语法（运行之后才知道）
- 源代码优化：z将整个语法转化为中间代码<!--与目标机器和运行环境无关，使得编译器分为前端和后端-->
<!-- 前端：产生与机器无关的中间代码，后端：编译器把中间代码转化为目标机器代码 -->
- 目标代码生成(Code Generator).
- 目标代码优化(Target Code Optimizer)


3、汇编阶段：
将hello.s翻译成机器语言指令，这些打包成可重定位目标程序
hello.o 二进制文件


4、链接阶段
合并单独处理好的目标文件---得到可执行文件


### 一些专有名词
---
#### 链接：（可以执行于编译中）

各种代码和数据片段收集并组合成为一个单一文件的过程
可被加载（复制）到内存并执行<!-- 链接器负责 -->
*使得分离编译成为可能**不用把大型应用程序组织成 巨大源文件，而是分解成小文件，可独立修改+编译*
**改变其中一个模块不需要重新编译其他文件**
---
#### 虚拟内存（ 虚拟储存器）

建立在主存--辅存物理结构基础上，有附加的**硬件装置**及操作系统存储管理软件组成的一种存储体系
让每个进程都能独占地使用主存。**每个进程看到的内存一致**

提供重要能力
1、主存只保留活动区域，在磁盘和主存之间来回传送数据
2、为每个进程提供了统一的地址空间（以虚拟地址编址），从而简化了存储器管理
3、操作系统会为每个进程提供独立的地址空间，从而保护了每个进程的地址空间不被其他进程破坏

图片：
---
内存虚拟内存      <!-- 用户不可见的代码 -->
---
用户栈            <!--  运行时建立的  -->
---
共享库的内存印射区域     <!-- printf函数 -->
---
运行时堆              <!-- 在运行时候由malloc创建 -->
---
读写数据              
---                   
只读的代码和数据      
---                   
程序开始
---

#### 静态链接

以一组**可重定位目标文件和命令行参数**作为输入，生成一个**完全链接、可以加载和运行的可执行文件**作为输出
输出的可定位目标文件由各种不同的代码和数据节组成，每一节都是一个连续的字节数列<!-- 字节数列：按一定顺序排列的一串字节 -->

Linux的x86-64系统中使用标准的ELF-64可重定位目标文件格式：
- 节
---
ELF头          <!-- 16字节序列开始，描生成文件系统的字的大小、目标文件（其中每个节都有固定条目）的类型（eg 可冲顶文，可执行或者分享）、机器类型、节头部表的文件偏移、节头部表（描述不同节的位置和大小 ）中条目大小和数量、 -->
---
.text                   <!--已经编译的机器代码-->
---
.rodata                 <!-- 只读数据 -->
---
.data                   <!-- 已经初始化的全局变量/静态c变量 -->（局部变量保存栈中）
---
.bss                    <!-- 未初始化的全局和静态变量/所有被初始化为0的的全局或者静态变量 -->
--- 
.symtab                 <!-- 存放定义/引用函数+全局变量的信息 -->
---
.rel.text               <!-- .text/节中位置的列表，任何调用外部函数或者引用全局变量的指令都需要修改 ，*调用本地函数的指令则不需要修改*-->
---
.rel.data               <!-- 被模块引用或定义的所有全局变量的重定位信息 -->
---
.debug                  <!-- 调试符号表（条目是程序定义的局部变量和类型定义/程序中定义和引用全局变量，以及原始的C源文件）-->
---
.line                   <!-- 原始C源程序中的行号和.text节中的机器指令之间的映射 -->
---
.strtab                 <!-- 字符串表，其内容包括:.symtab和.debug节中的符号表/节头部中的节名字。|字符串表:以null结尾的字符串序列 -->
----
节头部表


静态链接过程
1、空间地址分配
2、符号解析和重定位  
- 符号和符号表<!--包含定义和引用的符号信息-->
链接器中三种符号（外部符号）
 由模块m定义并能被其他模块引用的*全局符号*
 全局链接器符号对应*非静态的C函数和全局变量*
 由其他模块定义并被模块m*引用的全局符号*：
特点：
这些符号在m模块任何位置都可见，但是**不能被其它模块引用**
<tip:symtab中的符号表不包含对应于本地非静态程序变量的任何符号，这些符号在运行时在栈中被管理>
**Cstatic属性的本地过程变量是不在栈中管理的**

- 符号解析

将每个符号引用正好和一个符号定义关联起来。如果找不到，则会出现编译时错误
链接器解析符号引用的方法：
1、将每个引用与它输入的可重定位目标文件的符号表中的一个确定的符号定义关联
2、编译器遇到一个不是在当前模块中定义的符号（变量和函数名）时，假设该符号是在其他某个模块中定义的
3、生成一个链接器符号表条目
4、交给链接器处理
<!--链接器在它的任何输入模块中都找不到这个被引用符号的定义，输出一条错误信息并终止-->

链接器解析：
编译时，编译器向汇编器输出每个全局符号，或者是强（strong）或者是弱（weak）
汇编器把这个信息隐含地编码在可重定位目标文件的符号表里。
函数和已初始化的全局变量是强符号，未初始化的全局变量是弱符号。

Linux链接器使用下面的规则来处理多重定义的符号名：
***
不允许有多个同名的强符号。
如果有一个强符号和多个弱符号同名，那么选择强符号。
如果有多个弱符号同名，那么从这些弱符号中任意选择一个***

-重定位 （为程序中每条全局变量和指令生成唯一的地址）
重定位节和符号定义：
链接器将所有的相同类型的节合并成为同一类型的新的聚合节(可执行文件)
链接器将运行时内存地址赋给新的聚合节，赋给输入模块定义的每一个节，以及赋给输入模块定义的每个符号

*链接完成后，链接器就把多个目标文件合并成为了一个可执行的目标文件*
例：
ELF--.rodata   只读内存段（代码段）
段头部段：将连续的文件节印射到运行内存段
.data--.bss    读写内存段（数据段）
.symtab--节头部表    不加载到内存的符号表和调试信息
.strtab--节头部表    描述目标文件的节     

3、加载可执行目标文件
运行可执行的目标文件prog，在Linux shell的命令行中输入它的名字：
linux> ./prog

prog不是一个内置的shell命令，shell会认为prog是一个可执行目标文件，通过调用某个驻留在存储器中称为加载器（loader）的操作系统代码来运行它。
任何Linux程序都可以通过调用execve函数来调用加载器。
加载器将可执行目标文件中的代码和数据从磁盘*复制*到内存中，然后通过跳转到程序的第一条指令或入口点来运行该程序。
/这个将程序复制到内存并运行的过程叫做加载。


#### 动态链接共享库

需要原因：定期的维护和更新、考虑内存和磁盘空间

共享库<!--致力于解决静态库缺陷的一个现代创新产物-->
共享库是一个目标模块，在运行或加载时，可以加载到任意的内存地址，并和一个在内存中的程序链接起来。
这个过程称为动态链接，是由动态链接器来执行的

共享方式：
1、一个库只有一个.so文件，所有引用该库的可执行目标文件共享这个.so文件中的**代码和数据**，而不是像静态库的内容那样被复制和嵌入到引用它们的可执行文件中
2、一个.text节的一个副本可以被**不同的正在运行的进程共享**

调用编译器驱动程序，给编译器和链接器下指令：
'''
linus>gcc -shared -fpic -o libvector.so addvec.c multvec.c

'''
链接到上图的示例程序中
'''
linux> gcc -o prog21 main2.c ./libvector.so
'''
<!--创建了一个可执行目标文件prog21-->
*加载器不会像它通常所作地那样将控制传递给应用，而是加载和运行这个动态链接器*
最后动态链接器将控制传递给应用程序。共享库的位置就固定了，**并且在程序执行的过程中都不会变**

#### 从应用程序中加载和链接共享库
将每个生成动态的内容的函数打包在共享该库中
服务器动态地加载和链接适当地函数，然后直接调用它，而不是使用fork和execve在子进程的上下文中运行函数


---


## Runoob.com---cmake教程阅读笔记

### cmake基础
跨平台的构建系统生成工具，用于管理 C/C++ 项目的编译过程
通过编写 CMakeLists.txt 配置文件，可以定义项目的构建规则、依赖关系和编译选项，CMake 会自动生成对应平台的构建文件

#### CMakeLists.txt文件（核心配置）
通过读取文件中的指令来了解项目结构，源文件列表和编译要求
**每个指令的格式为：命令名(参数列表)**

---
- 指定最低版本
'''
cmake_minimum_required(VERSION <version>)
'''
**该指令必须在CMakeLists的最顶部**

---
- 定义项目名字和语言
'''
project(<project> [<use 语言>...])
'''
语言参数可以选择：CXX、C
eg：
'project(Project CXX)

---
- 添加可执行文件
'''
add_execuable(<a目标名> <a源文件>)
'''
将指定的源文件编译生成一个可执行文件
eg：add_execuable(MyApp main.cpp utils.cpp)

---
- 添加库文件
'''
add_library(<a库文件> [STATIC | SHARED | MODULE] <a源文件>...)
<!--  
STATIC:静态库（.a/ .lib）编译的时候直接嵌入可执行文件
SHARED：动态库（.so /.dll）,运行时加载
不指定类型：由BUILD_SHARED_LIBS变量决定
-->
eg:add_library（MyLib STATIC lirary.cpp）

---
- 链接库到目标
'''
target_link_libraries(<a目标> <a库>...)
'''
将指定的库链接到目标（可执行文件或者其他库）
可以链接到自己项目中的库文件，也可以链接外部库
target_link_librarise(MyApp PRIVATE MyLib)

---
- 设置变量
1、定义一个变量
'''
set(<a变量名> <a值>...)
''' 
定义一个普通变量，后续通过**${变量名}**引用
'''
set(CMAKE_CXX_STANDARD 17)
'''
2、同时设置多个值（列表）
'''
set(SOURCES main.cpp utils.cpp helper.cpp)
add_executable(MyApp $(SOURCES))
'''

- 为目标指定头文件路径：
'''
target_include_directories(<a目标>
    [BEFORE | AFTER]
    [SYSTEM]
    [PUBLIC | PRIVATE | INTERFACE]
    <a路径...>
<PUBLIC :当前目标和依赖它的目标都能使用这个路径>
<PRIVATE：只有当前目标能够使用>
<INTERFACE：只有依赖当前目标的其他对象可以使用>
eg:
target_include_directorise(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)

---
设置安装规则
'''
install(TARGET <a目标>...
    [RUNTIME DESTINATION <a可知性文件安装路径>]
    [LIBRARY DESTINATION <a动态安装路径>]
    [ARCHIVE DESTINATION <a静态库安装>]
    [INCLUDE DESTINATION <a头文件安装路径>]
'''
定义执行make install时各类型文件的安装位置
‘install（TARGET MyApp RUNTIME DESTINATION bin）

---
- 条件语句
'''
if(<a条件>)...
elseif(<a条件>)...
else()...
endif()
'''
Cmake支持的表达类型包括：比较、逻辑运算、变量判断
'''
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    <!-- debug模式下启用调试信息 -->
    message（STATUS "当前为debug构建模式"）
    set(CMAKE_CXX_FLAGS “${CMAKE_CXX_FLACE} -g”)
else()
    <!-- Release 模式下的启用优化 -->
    message（STATUS ”当前为release构建模式“）
    set(CMAKE_CXX_FLAGS "${CMAKe_CXX_FLAGS} -O2")
endif()
'''
<!--
-g:告诉编译器在可执行文件中加入调试符号（只有加入之后才能后面用GDB进行调试）
-O2：大写O，让编译器对代码进行优化，编译器尝试重排指令/省略无用代码/优化循环/内联函数等，提高与运行速度，降低体积
-->
*运行快的后果是编译时间变长，但是经过优化的代码会与原来有些不一样，不建议开启优化进行单步调试*
debug常用**-O0**
release模式常用**-O2  /-O3**

---
- 自定义语句：
'''
add_custom_command(
    TARGET <a目标>
    PRE_BUILD | PRE_LINK | POST_BUILD
    COMMAND <a命令> [参数...]
    [COMMENT <说明>]
    [VERBATIM])
'''
<!--
PRE-BUILD :在编译之前执行
PRE-LINK ：在链接之前执行
POST-BUILD ：在构建完成之后执行
-->
例子：
'''
add_custom_command(
    TARGET MyAdd POST-BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "构建完成！"
    [COMMENT "打印构建完整信息"]
'''

完整例子
---
'''
# 1. 指定 CMake 最低版本（必须放在最前面）
cmake_minimum_required(VERSION 3.10)

# 2、定义项目名称、版本和语言
project（MyProject VERSION 1.0.0 LANGUAGES CXX）

# 3、设置 C++ 标准为 C++17，并强制要求使用该标准
set（CMAKE_CXX_STANDARD 17）
set (CMAKE_CXX_STANDARD REQUIRED ON)

# 4、添加静态库目标
add_library(MyLib STATIC
   src/lib/core.cpp   <!-- 核心功能实现 -->
   scr/lib/helper.cpp   <!-- 辅助函数实现 -->
)

# 5、添加可执行文件目标
add_executable(MyApp
    scr/main.cpp      <!-- 程序入口 -->
    scr/utils.cpp      <!-- 工具函数 -->
)

# 6、为库函数指定公开文件路径
*PIBLIC 表示使用MyLib的目标也能访问该路径*、
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 7、为可执行文件指定私有头文件路径
target_include_directories(MyApp PRIVATE${PROJECT_SOURCE_DIR}/src)

# 8、将MyLib链接到MyApp
target_Link_libraries(MyApp PRIVATE MyLib)

---
构建项目命令：
'''
mkdir build && cd build    <a在根目录下创建build目录>
cmake ..
cmake --build .
cmake --install . <a安装到系统目录>

---
#### 变量和缓存

- 普通变量
- <a定义一个字符串变量> 
set(MY_APP_NAME "runoob-app")
- <a定义一个列表变量（用分号或者空格隔开）>
set(SOURCE_FILES main.cpp utils.cpp helper.cpp)
- <a使用变量：同过${变量名} 引用>
message（STATUS “项目名称：${MY_APP_NAME}”）
- <a使用列表变量：循环便利每个源文件>
foreach（FILE ${SOURCE_FILE}）
    message(STATUS “源文件：${FILE}”)
endforeach()

---
- 缓存变量
存储在 CMakeCache.txt 文件中，在多次 CMake 运行之间**保持值不变**
用于让用户在 CMake 配置阶段自定义构建设置，例如开关功能或指定安装路径
实例：
'''
set(<a变量名> <a默认值> CACHE <a类型> <a描述>)
*类型可以选择：STRING 、BOOL、PATH、FILEPATH*
如果CMakeLists.txt已经有该变量，**set不会覆盖已有值**

<a定义一个字符串类型的缓存变量（用户可以修改的安装路径）>
set(MY_INSTALL_PATH "/usr/local" CACHE PATH "程序的安装路径")

<a定义一个布尔类型的缓存变量>
set(ENABLE_LOGGING ON CACHE BOOL "是否启用日志功能")

<a使用缓存变量和其他变量一样>
if（ENABLE_LOGGING）
    message(STATUS "日志功能已启用")
    add_definitions(-DENABLE_LOGGING)
endif()
'''
用户可以通过命令行 —D 参数修改缓存变量：
$ cmake .. -DMY_INSTALL_PATH=/opt/runoob -DENABLE_LOGGING=OFF

---
#### 头文件搜索路径
指定编译器查找头文件的路径：
include_directories()
target_include_directories()
- 作用范围：
   include-directories()
作用范围：全局作用域，影响当前目录以及子目录中所有目标
推荐程度：不推荐，除非维护旧版CMake项目
目标关联性：不直接关联到特定目标，可能意外影响其他目标
可维护性：差，容易导致全局路径污染，问题难以排查
可见性控制：无法精确控制传播范围
  target_include_directories()
仅作用于指定的目标，不污染其他目标
推荐**优先**使用，符合现代CMake最佳实践
显式绑定到指定目标，依赖关系一目了然
较好，路径与目标绑定，逻辑清晰
通过PUBLIC、PRIVATE、INTERFACE精确控制

PUBLIC:当前目标可用/依赖此目标的其他目标可用————库的公开 API 头文件，自身和消费者都需要
PRIVATE：当前目标可用/依赖此目标的其他目标不可用—————仅自身实现需要的内部头文件
INTERFACE：当前目标不可用/依赖此目标的其他目标可用————仅头文件的库

#### 查找库和包
find_package()指令
会搜索系统中已安装的库，并设置对应的变量（如头文件路径、库文件路径、库目标名称）

'''
<a基本用法：查找Boost库（不限定版本）>
REQUIRED 表示如果找不到则终止CMAke配置

find_package(Boost REQUIRED)    
find_package(Boost 1.70 REQUIRED)    <a指定最低版本>
find_package(OpenCV REQUIRED PATHS /path/to/opencv)

find成功之后：
- 提供内容|示例|说明
导入目标 |	Boost::Boost |	现代 CMake 推荐方式，直接通过 target_link_libraries 使用<!--推荐，会自动理解所有编译和链接选项，减少配置错误-->
头文件路径变量 |	${Boost_INCLUDE_DIRS} |	旧式用法，配合 include_directories 使用
库文件路径变量 |	${Boost_LIBRARY_DIRS} |	旧式用法，配合 link_directories 使用

使用第三方库示例：
- 文件路径：项目根目录/CMakeLists.txt
'''
cmake_minimum_required(VERSION 3.10)
project(MyBoostProject CXX)
'''

- 设置 C++ 标准
'''
set(CMAKE_CXX_STANDARD 17)
'''

- 查找 Boost 库，如果找不到则报错退出
'''
find_package(Boost REQUIRED)
'''

- 添加可执行文件
'''
add_executable(MyApp main.cpp)
'''

- 使用导入目标链接 Boost
Boost::Boost 会自动设置头文件路径和库文件路径
'''
target_link_libraries(MyApp PRIVATE Boost::Boost)
'''

*注意*
- 优先使用 target_xxx 系列指令
include_directories() 和 link_directories() 是全局指令，**容易导致路径污染**。在大型项目中应始终使用 target_include_directories() 和 target_link_libraries()，它们让依赖关系更清晰、更易维护

-始终指定 C++ 标准
如果不设置 CMAKE_CXX_STANDARD，编译器会使用默认标准（通常是 C++98 或 C++14，取决于编译器版本）。**建议同时设置 CMAKE_CXX_STANDARD_REQUIRED 为 ON**，确保编译器不支持指定标准时直接报错

- 理解 PUBLIC / PRIVATE / INTERFACE 的区别

- 缓存变量的默认值不会自动更新
修改 CMakeLists.txt 中 set(... CACHE ...) 的默认值后，**已有的 CMakeCache.txt 不会被覆盖**。需要删除 build 目录中的 CMakeCache.txt 或整个 build 目录后**重新运行 cmake**

---
### Cmake构建流程
1、创建构建目录 	mkdir build && cd build 	    使用 Out-of-source 方式，保持源码目录整洁
2、生成构建文件 	cmake .. 	                    读取 CMakeLists.txt，生成 Makefile / Ninja / .sln 等
3、编译和构建 	    cmake --build . 	            执行实际的编译和链接过程
**4、清理构建文件 	cmake --build . --target clean 	删除编译产生的中间文件和目标文件**
**5、重新配置和构建 	cmake .. && cmake --build . 	CMakeLists.txt 变更后重新生成并编译**

#### 详解：
1、创建构建目录
CMake 推荐使用 **Out-of-source（源码外）**构建方式，即**将构建文件放在源代码目录之外的独立目录中**。
这种方式可以保持源代码目录的整洁，同时支持同一份源码配置多个不同的构建方案（如 Debug 和 Release 并存）
*直接在源码目录中运行 cmake 会污染项目结构，生成的中间文件难以清理，也不便于切换构建类型*
---
2、进入
cd build
3、生成构建文件
cmake ..
4、指定生成器
cmake -G “Ninja” ..
5、指定构建类型
使用 -DCMAKE_BUILD_TYPE 参数设置编译优化级别和调试信息
'''
cmake -DCMAKE_BUILE_TYPE=Release ..
'''
*四种常见的构建类型：Debug / Release / RelWithDebInfo / MinSizeRel*

6、检查配置结果
CMake 会输出配置过程中的详细信息，包括检测到的编译器、找到的库、定义的选项等。
如果没有错误，构建系统文件将被生成到当前构建目录中。
如果配置过程中出现错误（如找不到指定的库），CMake 会中止并输出错误信息。解决错误后，重新运行 cmake 命令即可继续。

7、编译和构建
- 使用 CMake 通用构建命令   **推荐**
cmake --build .
- 构建特定目标
cmake --build . --target MyExecutable
- 使用 Makefile 构建系统
make
make -j $ (nproc)
make MyExecutable
- 使用 Ninja 构建系统
ninja
ninja MyExecutable
- 使用 Visual Studio
如果生成了 Visual Studio 工程文件，可以打开 .sln 解决方案文件，在 IDE 中选择生成解决方案
msbuild MyExecutable.sln /p:Configuration=Release

8、清理构建文件
构建过程中生成的中间文件（.o、.obj 等）和目标文件可以通过清理操作删除，释放磁盘空间
- 使用 CMake 通用清理命令
cmake --build . --target clean
- 使用Makefile
make clean
- 使用Ninja
ninja clean
- 手动清理
rm -f build <!-- 删除构建目录中的所有文件，保留源代码目录不变 -->
*最简单彻底的清理方式是直接删除整个 build 目录，然后重新创建并运行 cmake。这比单独执行 clean 更可靠，因为 clean 目标可能未正确定义*


**9、重新配置和构建：**
修改了 CMakeLists.txt 文件或调整了项目设置后，**需要重新运行 CMake 配置并重新编译**
- 重新配置：
cmake ..
- 重新编译：
cmake --build .
*大多数情况下，修改 CMakeLists.txt 后直接运行 cmake --build . 即可。构建系统会自动检测到 CMakeLists.txt 的变更并重新运行 cmake 配置步骤*

#### 构建系统对比
构建系统 	    适用平台 	        并行编译 	    特点
Unix Makefiles 	Linux / macOS 	    需手动指定 -j 	兼容性最广，几乎所有 Unix 系统自带
Ninja 	        跨平台 	            自动并行        速度更快，特别适合增量编译和大型项目
Visual Studio 	Windows 	        IDE 自动管理 	与 VS IDE 深度集成，支持图形化调试和配置

注意事项
- 始终使用 Out-of-source 构建。将 build 目录添加到 .gitignore 中是最佳实践
- 优先使用 cmake --build . 命令。*这个命令是跨平台的通用构建入口*，无论底层是 Make、Ninja 还是 MSBuild 都能正确工作。在脚本和 CI 环境中尤其推荐使用
- 构建类型影响性能和调试体验。
Debug 模式包含调试符号且不优化，适合开发；
Release 模式启用优化且去除调试信息，适合生产部署。
**不要用 Debug 模式测试性能，也不要用 Release 模式调试代码**
- 遇到奇怪的构建错误时，删除 build 目录重来。
CMake 的缓存有时会保留过期的配置信息，导致难以排查的错误。删除整个 build 目录并重新 cmake 是最快的排查方式

### cmake构建示例

#### 项目准备

##### 项目结构
主程序源文件+库源文件+库头文件
'''
MyProject/
├── CMakeLists.txt          # CMake 配置文件
├── src/
│   ├── main.cpp            # 主程序源文件
│   └── mylib.cpp           # 库源文件
└── include/
    └── mylib.h             # 库头文件
'''
文件 	            作用 	        说明
CMakeLists.txt  	CMake 配置文件 	定义项目的构建规则、目标和依赖关系
src/main.cpp    	主程序源文件 	包含 main() 函数，程序的入口点
src/mylib.cpp    	库源文件 	    自定义库的功能实现
include/mylib.h 	库头文件 	    库的公开*接口声明*，供主程序包含使用

---
##### CmakeLists.txt配置文件
略，上面有
*每个 add_executable 和 add_library 都定义了一个构建目标（Target）。目标的概念是 CMake 的核心——后续的target_include_directories、target_link_libraries 等指令都是围绕目标来组织的*
1、创建CMakeList.txt
2、创建目录并进入
3、配置项目
CMake 会输出配置结果，显示检测到的编译器和配置状态。如果一切正常，会看到类似以下输出
'''
-- The CXX compiler identification is GNU 11.4.0
-- Detecting CXX compiler ABI info - done
-- Configuring done
-- Generating done
-- Build files have been written to: /path/to/MyProject/build
'''
如果配置过程中出现错误（如缺少编译器或 CMake 版本过低），CMake 会中止并输出具体错误原因。解决对应问题后*重新运行 cmake .. *即可

4、编译项目        <cmake --build .>
5、运行可执行文件  <./MyExecutable>
6、清理构建文件

注意：
- 确保源文件路径正确
add_executable 和 add_library 中的源文件路径是**相对于 CMakeLists.txt 所在目录的**。如果路径写错，CMake 配置阶段不会报错，但编译时会提示**找不到文件**。
- 目标名称区分大小写。MyLib 和 mylib 是不同的目标。target_link_libraries 中引用的目标名**必须与 add_library 定义的目标名完全一致**
- **修改 CMakeLists.txt 后需要重新配置**。
如果只修改了源文件，直接 make 即可；
但如果修改了 CMakeLists.txt（如添加新文件、更改编译选项），需要重新运行 cmake .. 或直接运行 cmake --build .（它会自动检测配置变更）。
- 初次使用建议从简单项目开始。先用单个可执行文件跑通整个构建流程，再逐步添加库、外部依赖和自定义编译选项。一次加太多东西会让排查错误变得困难

### cmake 高级特性
主题 	            核心能力 	                典型应用场景
自定义模块和脚本 	封装可复用的 CMake 逻辑 	统一编译器选项、自定义查找逻辑、团队共享构建规则
构建配置和目标 	    管理多配置和多目标 	        Debug/Release 切换、构建多种输出产物
高级查找和配置 	    灵活定位外部依赖 	        查找特定组件、通过配置文件传递构建信息
自定义构建步骤 	    扩展构建流程 	            代码生成、资源处理、构建后自动化
跨平台和交叉编译 	面向多平台构建 	            嵌入式开发、为不同操作系统编译
目标属性和配置 	    精细控制编译链接        	针对特定目标设置警告级别、链接路径

详细解释：
#### 自定义从CMake模块和脚本

当多个 CMakeLists.txt 中需要重复同样的逻辑时，将这些逻辑封装成自定义模块可以大幅减少代码冗余。自定义模块和脚本的本质是将常用的 CMake 函数、宏和配置提取到独立文件中，供多个项目复用
1、创建cmake/ 目录
在项目更目录下创建，用于集中存放自定义模板
2、创建模板文件
例如：cmake/MyModule.cmake,编写自定义函数
3、扩展模块探索路径
在CMakeLists.txt中通过CMAKE_MODULE_PATH注册该目录
4、加载模块
使用include（）引入模块并调节其中的函数

*自定义模块示例——MyModule.cmake*
'''
<a文件路径:>     cmake/MyModule.cmake
<a自定义模块:>   提供项目通用的辅助函数
*函数：为所有目标统一设置编译器警告选项
参数 ARG_TARGET：需要设置警告的目标名称*
function(runoob_set_warnings ARG_TARGET)         <!-- 根据编译器类型设置不同警告标志 -->
    if(MSVC)
        target_compile_options(${ARG_TAGRET} PRIVATE /w4)        <!-- MSVC编译器：启用w4级别 -->
    else（）
        target_compile_options(${ARG_TARGET} PRIVATE -Wall -Wextr -Wpedentic)          <!-- GCC/CLANG编译器：启用常用警告-->
    endif()
endunction

- 函数：打印目标的基本构建信息
function(runoob_print_target_info ARG_TARGET)
    message(STATUS "目标名称：${ARG_TARGET}")
    message(STATUS "源文件列表：$<TARGET_PROPERTY:${ARG_TARGET},SOURCES>")
endfunction()

*在 CMakeLists.txt 中加载并使用模块：*
'''
cmake_minimum_required(VERSION 3.10)
project(MyProject CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")       <!-- 将 cmake/ 目录添加到模块搜索路径 -->
include(MyModule)           <!-- 加载自定义模块 -->
add_executable(MyApp main.cpp)        
runoob_set_warnings(MyApp)          <!-- 调用模块中的自定义函数，为 MyApp 设置警告选项 -->
runoob_print_target_info(MyApp)          <!-- 调用模块中的自定义函数，打印目标信息 -->
'''

CMAKE_MODULE_PATH 是 CMake 查找模块文件时的搜索路径列表。*CMake 会先在内置模块路径中查找，找不到时再到 CMAKE_MODULE_PATH 指定的路径中查找*

#### 使用自定义 CMake 脚本

通常用于执行配置操作而非定义可复用的函数
脚本可以直接用 include() 加载，CMake 会按顺序执行其中的每一条指令

##### 创建脚本文件 config.cmake         集中管理项目的配置选项

'''
config.cmake           <!--路径-->
if(NOT CMAKE_BUILD_TYPE)                      <!-- 默认构建类型为Release -->
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "构建类型" FORCE)
    message(STATUS "未指定构建类型，默认使用 Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")         <!--根据构建类型设置不同的编译选项-->
    message(STATUS "启用 Debug 模式：关闭优化，开启调试符号")
else()
    message(STATUS "启用 Release 模式：开启 O2 优化")
endif()
'''

在 CMakeLists.txt 中调用脚本：
使用绝对路径加载脚本
'''
include(${CMAKE_SOURCE_DIR}/config.cmake)
'''

#### 构建配置和目标

*CMake 支持在同一个项目中管理多种构建配置（如 Debug 和 Release），并定义多个构建目标*
##### 多配置生成器
允许在同一个构建目录中切换 Debug/Release(vs/xcode),而 Unix Makefiles 等单配置生成器**需要在配置阶段就确定构建类型**

生成器类型 	典型生成器   	        配置指定方式
单配置 	    Unix Makefiles、Ninja 	配置时通过 -DCMAKE_BUILD_TYPE=Release 指定
多配置 	    Visual Studio、Xcode 	构建时通过 --config Release 切换

- 在 CMakeLists.txt 中设置默认配置：
<!--仅对单配置生成器生效，多配置生成器会忽略此变量-->
'''
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type")
'''

**CMAKE_BUILD_TYPE 只对单配置生成器有效**
如果使用 Visual Studio，请通过 cmake --build . --config Release 或在 IDE 中切换配置。

- 构建目标
一个 CMake 项目中可以定义多个构建目标，每个目标独立配置编译选项和链接依赖。
可以在一次构建中产出多个可执行文件或多个库

'''
add_executable(MyApp src/main.cpp)                                             <!--定义两个可执行文件目标，各自有不同的源文件-->
add_executable(MyTool src/tool.cpp)

set_target_properties(MyApp PROPERTIES COMPILE_DEFINITIONS "APP_MODE")         <!-- 为不同目标设置不同的编译宏，控制条件编译--> 
set_target_properties(MyTool PROPERTIES COMPILE_DEFINITIONS "TOOL_MODE")

target_link_libraries(MyApp PRIVATE MyLib)                                     <!-- 为不同目标链接不同的库 -->
target_link_libraries(MyTool PRIVATE MyLib CLI11::CLI11)
'''
#### 高级查找和配置

除了基本的 find_package 用法外，CMake 还支持指定组件、精确控制版本和通过配置文件模板传递构建信息

##### find_package 高级用法

COMPONENTS 关键字允许你只查找库中的特定子模块，而非整个包。
对于像 Boost 这样的大型库非常重要——**按需引入可以避免链接不需要的组件**

'''
<!--只查找 Boost 的 filesystem 和 system 两个组件（而非整个 Boost）
# filesystem 内部依赖 system，所以两个都要声明-->
find_package(Boost REQUIRED COMPONENTS filesystem system)
<!--检查是否找到了所需组件-->
if(Boost_FOUND)
    message(STATUS "Boost 版本：${Boost_VERSION}")
    message(STATUS "Boost 头文件路径：${Boost_INCLUDE_DIRS}")
<!--分别链接每个需要的组件-->
    target_link_libraries(MyApp PRIVATE
        Boost::filesystem
        Boost::system
    )
endif()
'''

- 指定搜索路径（适用于非标准安装位置）：
方式一：通过变量指定根目录
cmake .. -DBOOST_ROOT=/path/to/custom/boost

方式二：在 CMakeLists.txt 中预先设置
set(BOOST_ROOT "/path/to/custom/boost")
find_package(Boost REQUIRED COMPONENTS filesystem)

##### 配置文件与构建选项

configure_file() 指令可以将 CMake 变量的值注入到代码文件中，实现在编译时传递配置信息

'''
<!--文件路径：config.h.in     CMake 配置模板文件，通过 configure_file() 生成 config.h-->
define RUNOOB_VERSION "@PROJECT_VERSION@"              <!--项目版本号（由 CMake 变量 @PROJECT_VERSION@ 替换）-->
#define RUNOOB_BUILD_TYPE "@CMAKE_BUILD_TYPE@"         <!--构建类型（Debug 或 Release）-->
define RUNOOB_BUILD_TIMESTAMP "@BUILD_TIMESTAMP@"      <!--编译时间戳-->
'''
##### 在 CMakeLists.txt 中配置并生成头文件：
'''
string(TIMESTAMP BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")
configure_file(config.h.in config.h)    <!--将 config.h.in 中的 @变量@ 替换为 CMake 变量的值     输出到 build 目录下的 config.h-->
include_directories(${CMAKE_BINARY_DIR}) <!--将 build 目录添加到头文件搜索路径，使源文件能找到 config.h-->
'''

在源文件中使用生成的配置：
'''
#include "config.h"
std::cout << "RUNOOB Version: " << RUNOOB_VERSION << std::endl;
std::cout << "Build Type: " << RUNOOB_BUILD_TYPE << std::endl;
'''

#### 生成自定义构建步骤

add_custom_command() 
依赖驱动/只有当 OUTPUT 文件不存在或 DEPENDS 文件更新时，命令才会执行

'''
<!--仅当 generated_file.cpp 不存在或 input_template.txt 更新时才执行-->
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/generated_file.cpp        <!--输出文件（CMake 通过检查此文件决定是否重新执行-->
    COMMAND ${CMAKE_COMMAND}                             <!--要执行的命令（此处用 cmake -E 执行内置的文件生成操作）-->
        -E echo "// 自动生成的源文件" > ${CMAKE_BINARY_DIR}/generated_file.cpp
    DEPENDS ${CMAKE_SOURCE_DIR}/input_template.txt       <!--依赖文件（此文件变更时命令重新执行） -->
    COMMENT "正在从模板生成源文件..."                    <!-- 执行时输出的提示信息 -->
'''

构建阶段触发命令——在链接完成后执行：
'''<!--在 MyApp 构建完成后，复制可执行文件到部署目录  -->
add_custom_command(
    TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:MyApp>
        ${CMAKE_SOURCE_DIR}/deploy/
    COMMENT "将 MyApp 复制到部署目录"
)
'''

##### 自定义目标

add_custom_target() 定义一个不产生输出文件的构建目标，通常用于将多个自定义命令组织在一起。
除非使用 ALL 关键字，否则自定义目标不会在默认构建中执行，需要显式指定目标名

'''
add_custom_target(generate_code ALL      <!--关联上面的自定义命令-->
DEPENDS ${CMAKE_BINARY_DIR}/generated_file.cpp     <!--ALL 表示在默认构建（make）时自动执行-->
add_custom_target(deploy
    COMMAND ${CMAKE_COMMAND} -E echo "部署到服务器..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_BINARY_DIR}/bin
        /opt/runoob/deploy/
    COMMENT "执行部署操作"
)
'''
<!--在构建时单独触发此目标
$ cmake --build . --target deploy-->

add_custom_command 和 add_custom_target 的区别：
前者定义一个「如何生成文件」的规则，**由依赖关系触发**；后者定义一个「做什么事」的动作目标，需要**显式调用或在 ALL 时自动执行**

#### 跨平台和交叉编译

- 跨平台构建
变量                	作用         	示例值
CMAKE_SYSTEM_NAME   	目标操作系统 	Linux、Windows、Darwin、Android
CMAKE_SYSTEM_PROCESSOR 	目标处理器架构 	x86_64、arm、aarch64
CMAKE_C_COMPILER    	C 编译器路径 	/usr/bin/arm-linux-gnueabihf-gcc
CMAKE_CXX_COMPILER   	C++ 编译器路径 	/usr/bin/arm-linux-gnueabihf-g++

-交叉编译
指在当前平台（如 x86_64 Linux）上编译出运行在另一种架构（如 ARM）上的程序
CMake 通过工具链文件（Toolchain File）来指定交叉编译所需的编译器、系统信息和链接器配置

创建工具链文件 toolchain.cmake：
'''
set(CMAKE_SYSTEM_NAME Linux)                                    <!--目标系统为 Linux-->

set(CMAKE_SYSTEM_PROCESSOR arm)                                 <!--目标处理器架构为 ARM-->

set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc)          <!--指定交叉编译器（必填：路径需根据实际工具链位置调整）-->
set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)       

set(CMAKE_FIND_ROOT_PATH /path/to/arm-sysroot)                   <!--指定目标系统的根文件系统路径（可选，用于查找库和头文件）-->
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)                     <!--查找程序时只在目标系统路径中搜索（不在主机路径搜索）-->
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)                     <!--查找库时只在目标系统路径中搜索-->
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)                     <!--查找头文件时只在目标系统路径中搜索-->
'''

使用工具链文件进行交叉编译：
'''
- 配置时通过 -DCMAKE_TOOLCHAIN_FILE 指定工具链文件
mkdir build_arm && cd build_arm
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..
- 使用生成的 Makefile 编译（将调用 ARM 交叉编译器）
make
'''

工具链文件必须在第一次运行 cmake 时指定。如果已有 CMakeCache.txt，需要删除 build 目录后重新配置才能切换工具链

#### 目标属性和配置

##### 目标属性

set_target_properties() 用于设置目标级别的属性，影响编译器和链接器的行为

属性                 	作用        	示例
COMPILE_OPTIONS     	设置编译选项 	-Wall、-O2
COMPILE_DEFINITIONS 	设置预处理器宏 	DEBUG、VERSION=2
LINK_FLAGS 	            设置链接器标志 	-L/path/to/lib
OUTPUT_NAME 	        修改输出文件名 	目标名是 MyApp，输出为 my_app

'''
add_executable(MyApp main.cpp)
set_target_properties(MyApp PROPERTIES          <!--为目标设置编译选项和链接标志-->
    COMPILE_OPTIONS "-Wall;-Wextra"
    COMPILE_DEFINITIONS "RUNOOB_DEBUG"
    LINK_FLAGS "-L/usr/local/lib"
    OUTPUT_NAME "my_app"
)
'''

上面的 set_target_properties 等同于以下三条指令的效果：
'''
target_compile_options(MyApp PRIVATE -Wall -Wextra)
target_compile_definitions(MyApp PRIVATE RUNOOB_DEBUG)
target_link_options(MyApp PRIVATE -L/usr/local/lib)
'''

推荐使用 target_compile_options()、target_compile_definitions() 和 target_link_options() 替代 set_target_properties()，因为它们支持 PUBLIC/PRIVATE/INTERFACE 可见性控制，依赖关系更明确

##### 自定义编译和链接选项

'''
target_compile_options(MyApp PRIVATE -Wall -Wextra -Wpedantic)
<!--为 MyApp 设置编译选项
PRIVATE：只有 MyApp 自身编译时使用这些选项-->

target_compile_definitions(MyApp PRIVATE RUNOOB_VERSION="1.0")          
<!--为 MyApp 设置预处理器定义
等同于在源文件中写 #define RUNOOB_VERSION "1.0"-->

target_link_options(MyApp PRIVATE -L/usr/local/lib)

target_compile_features(MyLib PUBLIC cxx_std_17)
<!--如果是库，使用 PUBLIC 可以让链接此库的目标也继承这些选项
例如：使用 MyLib 的任何目标都会自动启用 C++17-->

注意：
- CMAKE_BUILD_TYPE 仅对单配置生成器有效
- 优先使用 target_xxx 系列指令
- 工具链文件必须在首次 cmake 时指定
交叉编译时，如果在已有 CMakeCache.txt 的目录中重新指定工具链，可能会产生不完整的配置。始终在干净的 build 目录中配置交叉编译
- 自定义模块中的**函数名建议加上前缀**
避免与 CMake 内置命令或其他模块产生命名冲突。例如使用项目名作为前缀：myproject_set_warnings()。



---



## creating a catkin workspace tutorial笔记

### catkin工作空间

分为三个目录：
- src/  : 源空间，存放catkin软件包源代码
- build/   ：构建空间，在这里调用cmake，为源空间中的catkin包生成缓存文件和中间文件
- devel/   ：开发空间，构建产物在被系统使用之前先在这里

''' 
workspace_folder/                         # 工作空间
    src/                                  # 源空间
        CMakeLists.txt                    # 顶层Cmake文件，由catkin创建
        package_1/
            CMakeLists.txt                # package_1的CMakeLists.txt
            package.xml                   # package_1的包清单（manifest）
        package_2/
            CMakeLists.txt
            package.xml
    build/                                # 构建空间，由catkin_make 生成
    devel/                                # 开发空间，由catkin_make生成
'''

## 创建工作空间

'''
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/
catkin_make
'''

*catkin_make是一个便捷工具，会在build目录中运行cmake，再运行make。首次执行之后，build/和devel/会自动生成，同时在src/下面创建一个顶层CMakeLists.txt*

## source(加载)生成的工作空间

使用刚生成的工作空间，需要source它的setup文件

'''
source devel/setup.bash                   
'''
工作空间就会被正确叠加在ros环境之上

---

通过查看ROS_PACKAGE_PATH  来验证

'''
echo $ROS_PACKAGE_PATH
'''

输出中应当包含工作空间路径：
/home/<你的用户名名字>/catkin_WS/src:/opt/ros/<ROS 发行版本>/share
（工作空间排在最前面，优先用于安装系统的ros）

## 如何判断自己是否下载了catkin

1、commond -v catkin_make
'有输出路径一般存在，反之不存在'

2、一行命令直接给出结论
command -v catkin_make >/dev/null 2>&1 && echo "catkin 已安装: $(command -v catkin_make)" || echo "catkin 未安装"

3、完整的if判断脚本（可以写进shell脚本）
'''
if command -v catkin_make >/dev/null 2>&1; then
    echo "catkin 已安装，路径：$(which catkin_make)"
else
    echo "catkin 未安装，请先安装 ROS"
fi
'''

4、其他判断方式：
' catkin --version '
- 检查cmake——tools（cmake build命令，与catkin_make是两套工具）

' ls /opt/ros/ '
- 查看ros安装在哪个目录里面（catkin和ROS一起安装）

' echo $ROS_DISTRO '
- 查看当前ROS的发行版环境变量

' dpkg - l | grep catkin '
- 从apt安装记录里面查

' python3 -c "import catkin_pkg; print(catkin_pkg.__version__)" '
- 检查python端的catkin_pkg 库

docker pull 是幂等的，就算重复执行，如果远端没更新，他也只是会提示Image is up to date，不会重复下载。所以重复拉取没有坏处，但没必要。

### 直接运行即可：

- 用之前定制好的镜像（推荐，如果里面已经装了要的东西）
docker run -it --rm -v ~/catkin_ws:/root/catkin_ws my_ros_ready bash
- 或者用官方基础镜像
docker run -it --rm -v ~/catkin_ws:/root/catkin_ws ros:noetic bash

进入容器后就可以照着教程执行 mkdir -p ~/catkin_ws/src && cd ~/catkin_ws && catkin_make

### tip：

1、之前用 --name 参数创建过容器，里面可能还留着上次的工作，可以用 'docker ps -a' 查看、用 'docker start -ai 容器名' 重新进入，而不必新建；
2、如果需要 rviz 这类图形界面，就选 desktop-full 那个镜像

### catkin以后的使用方法：

1、开vc写代码
*打开本地的 ～/catkin_ws文件*

- 在src目录下创建你的功能包，然后写cpp和python

2、启动docker容器
'''
docker start -ai my_ros_env
'''
(回到上次敲exit后的地方)

3、在容器里面编译
*外面改代码，容器里面是同步的*
'''
cd ~/catkin_ws
catkin_make
'''
*如果有报错，就看报错改代码，如果看到’【100%】Build target‘,证明代码没问题*

4、容器内运行：
'''
cource devel/setup.bash             # 激活
rosrun my_pkg my_code               # 运行ROS节点
roslaunch my_pkg my_luanch.luanch   # 启动launch文件

### 避坑

Q：关掉终端怎么进入容器
A: 终端后台停止，打开新终端
'''
docker start -ai my_ros_env
'''
敲完就能复活

---
Q：容器怎么安装新软件
A：
'''
apt-get update
apt-get install
'''
*没带  --rm,下载能够永久保存*

Q：在容器里面改代码会怎么样
A:非用nano的话也可以，双向一起改变

---
Q：怎么删掉重来
A：
'''
exit
docker rm -f my_ros_env
'''
然后重新建立容器



---


## 皇帝的新衣-笔记

从零开始构建一个能真正干活的编程智能体（coding agent）<!--python-->

- 心智模型:

agent:本质上就是一个带着工具箱的强大 LLM 的对话
LLM 从不真正接触你的文件系统——它只是“请求”某些事情发生，而由代码让它们发生

三个工具:
- 读取文件——让 LLM 能看到你的代码
- 列出文件——让它能在你的项目中导航
- 编辑文件——让它能下达创建和修改代码的指令

- 导入和 API 客户端
(示例代码实际使用的是 Anthropic 客户端；该写法适用于任何 LLM 提供商)
'''
import inspect
import json
import os

import anthropic
from dotenv import load_dotenv
from pathlib import Path
from typing import Any, Dict, List, Tuple

load_dotenv()

claude_client = anthropic.Anthropic(api_key=os.environ["ANTHROPIC_API_KEY"])
'''

- 一些让输出更易读的终端颜色:
'''
YOU_COLOR = "\u001b[94m"
ASSISTANT_COLOR = "\u001b[93m"
RESET_COLOR = "\u001b[0m"
'''

- 解析文件路径的工具函数（把 file.py 变成 /Users/you/project/file.py）：
'''
def resolve_abs_path(path_str: str) -> Path:
    """
    file.py -> /Users/you/project/file.py
    """
    path = Path(path_str).expanduser()
    if not path.is_absolute():
        path = (Path.cwd() / path).resolve()
    return path
'''
**把工具函数的 docstring 写得尽可能详细，因为 LLM 会借助它们来推理对话中该调用哪些工具**

### 工具

#### 工具一：读取文件

接收文件名，返回其内容

'''
def read_file_tool(filename: str) -> Dict[str, Any]:
    """
    Gets the full content of a file provided by the user.
    :param filename: The name of the file to read.
    :return: The full content of the file.
    """
    full_path = resolve_abs_path(filename)
    print(full_path)
    with open(str(full_path), "r") as f:
        content = f.read()
    return {
        "file_path": str(full_path),
        "content": content
    }
'''

**返回一个字典，因为 LLM 需要关于“发生了什么”的结构化上下文**

#### 工具二：列出文件

通过列出目录内容来浏览项目
'''
def list_files_tool(path: str) -> Dict[str, Any]:
    """
    Lists the files in a directory provided by the user.
    :param path: The path to a directory to list files from.
    :return: A list of files in the directory.
    """
    full_path = resolve_abs_path(path)
    all_files = []
    for item in full_path.iterdir():
        all_files.append({
            "filename": item.name,
            "type": "file" if item.is_file() else "dir"
        })
    return {
        "path": str(full_path),
        "files": all_files
    }
'''

工具三：编辑文件

处理两种情况：
- 创建新文件——当 old_str 为空时
- 替换文本——找到 old_str 并替换为 new_str

'''
def edit_file_tool(path: str, old_str: str, new_str: str) -> Dict[str, Any]:
    """
    Replaces first occurrence of old_str with new_str in file. If old_str is empty,
    create/overwrite file with new_str.
    :param path: The path to the file to edit.
    :param old_str: The string to replace.
    :param new_str: The string to replace with.
    :return: A dictionary with the path to the file and the action taken.
    """
    full_path = resolve_abs_path(path)
    if old_str == "":
        full_path.write_text(new_str, encoding="utf-8")
        return {
            "path": str(full_path),
            "action": "created_file"
        }
    original = full_path.read_text(encoding="utf-8")
    if original.find(old_str) == -1:
        return {
            "path": str(full_path),
            "action": "old_str not found"
        }
    edited = original.replace(old_str, new_str, 1)
    full_path.write_text(edited, encoding="utf-8")
    return {
        "path": str(full_path),
        "action": "edited"
    }
'''
old_str 为空表示“创建这个文件”，否则就执行查找替换
真实的 IDE 在找不到字符串时会加入复杂的回退行为，但这样已经够用了

### 工具注册表

#### 按名称查找工具的方法：

'''
TOOL_REGISTRY = {
    "read_file": read_file_tool,
    "list_files": list_files_tool,
    "edit_file": edit_file_tool
}
'''

#### 让 LLM 了解我们的工具

LLM 需要知道有哪些工具、如何调用它们
从函数签名和 docstring 动态生成这些信息
'''
def get_tool_str_representation(tool_name: str) -> str:
    tool = TOOL_REGISTRY[tool_name]
    return f"""
    Name: {tool_name}
    Description: {tool.__doc__}
    Signature: {inspect.signature(tool)}
    """

def get_full_system_prompt():
    tool_str_repr = ""
    for tool_name in TOOL_REGISTRY:
        tool_str_repr += "TOOL\n===" + get_tool_str_representation(tool_name)
        tool_str_repr += f"\n{'='*15}\n"
    return SYSTEM_PROMPT.format(tool_list_repr=tool_str_repr)
'''

然后是系统提示词本身：
'''
SYSTEM_PROMPT = """
You are a coding assistant whose goal it is to help us solve coding tasks. 
You have access to a series of tools you can execute. Here are the tools you can execute:

{tool_list_repr}

When you want to use a tool, reply with exactly one line in the format: 'tool: TOOL_NAME({{JSON_ARGS}})' and nothing else.
Use compact single-line JSON with double quotes. After receiving a tool_result(...) message, continue the task.
If no tool is needed, respond normally.
"""
'''
*只是在告诉 LLM“这些是你的工具，这是调用它们的格式”*

#### 解析工具调用

'''
def extract_tool_invocations(text: str) -> List[Tuple[str, Dict[str, Any]]]:
    """
    Return list of (tool_name, args) requested in 'tool: name({...})' lines.
    The parser expects single-line, compact JSON in parentheses.
    """
    invocations = []
    for raw_line in text.splitlines():
        line = raw_line.strip()
        if not line.startswith("tool:"):
            continue
        try:
            after = line[len("tool:"):].strip()
            name, rest = after.split("(", 1)
            name = name.strip()
            if not rest.endswith(")"):
                continue
            json_str = rest[:-1].strip()
            args = json.loads(json_str)
            invocations.append((name, args))
        except Exception:
            continue
    return invocations
'''
简单的文本解析：查找以 tool: 开头的行，提取函数名和 JSON 参数

#### LLM 调用

'''
def execute_llm_call(conversation: List[Dict[str, str]]):
    system_content = ""
    messages = []
    
    for msg in conversation:
        if msg["role"] == "system":
            system_content = msg["content"]
        else:
            messages.append(msg)
    
    response = claude_client.messages.create(
        model="claude-sonnet-4-20250514",
        max_tokens=2000,
        system=system_content,
        messages=messages
    )
    return response.content[0].text
'''

### 智能体主循环

'''
全部放在一起
'''

整体结构：
1、外层循环：获取用户输入，加入对话
2、内层循环：调用 LLM，检查是否有工具调用
- 不需要工具 → 打印回复，跳出内层循环
- 需要工具 → 执行它们，把结果加入对话，继续循环
内层循环会一直持续，直到 LLM 的回复不再请求任何工具。这使得智能体能够串联多次工具调用（先读文件，再编辑，然后确认编辑结果）。


#### 运行

'''
if __name__ == "__main__":
    run_coding_agent_loop()
'''

### 与生产级工具（如 Claude Code）额外增加了：

- 更好的错误处理和回退行为
- 流式响应带来更好的用户体验
- 更智能的上下文管理（比如对长文件做摘要等）
- 更多工具（运行命令、搜索代码库等）
- 针对破坏性操作的审批流程



## 关于大模型的一些思考

在整个过程中用过Zcode，deepseek（手机两种
可能因为刚开始，还用不太习惯agent，还是比较多地在手机查找一些具体步骤/某些词语的意思，比如“-O2”等等

感觉在学习过程中懵逼的地方，及时地对大模型发问，理解回复，记入笔记，这样记忆得还挺快，稍稍也偏牢固
<!--
比如在cmake里面看到以前相关代码中没出现，也当下无文字解析的时候，我直接问了deepseek“  ’02‘  这个是什么”，并且直接将语境（上下文）直接拍给它
后面它回答问题的时候还一起指出，是 “-O2,大写字母O” 。

感觉有的时候不仅是知识的获得，还有一些比较微小/难以注意到的 缺点也被指出来了

关于200行python写大模型还有那篇文章纯英其实看得半知半懂（其实也和不懂差不多），浏览器翻译失败后，直接丢给agent让它翻译，做完再细看内容，发现除了翻译之外

还有一些提示，像是知识点更好的用法（？）

或者是别的链接更深的打算教你（？）
-->
在之后熟练之后也许会get agent真好用。但是也有偶尔出错，发问时歪解我的问题和意图进而开始固执地坚守一个观点死磕
<belike 之前问过关于下载ubuntu，但速度慢，，于是他提出了一堆命令行指令，但一直在循环的确认/做某件事，比如6.1g的东西下了，失败了，重新来，还是那些文体>

感觉在学习中更侧重还是自己觉悟+实践运用那个代码，无脑copy还不如不学，学了也没记住太多










