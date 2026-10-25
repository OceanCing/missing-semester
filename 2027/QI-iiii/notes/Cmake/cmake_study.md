## 概念
CMake 读取 CMakeLists.txt，根据环境生成 Makefile、Ninja 构建文件或 IDE 工程文件，再交给相应工具构建
CMake 管构建配置，make 等工具执行构建规则，gcc / g++ 等编译器处理源代码

## CMakeLists.txt
cmake_minimum_required(VERSION 3.16)  
project(demo LANGUAGES CXX)  
add_executable(app main.cpp add.cpp)
set（CMAKE_C++ STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH "路径/bin")  

cmake_minimum_required：指定项目要求的最低 CMake 版本
project：demo是项目名，启用 C++，C 项目可以写 LANGUAGES C
add_executable：定义一个可执cmake_minimum_required(VERSION 3.16)  
set：指定使用标准

project(demo LANGUAGES C++)  
add_executable(app main.cpp add.cpp)行目标，app 是目标名，后面是源文件（当源文件有多个时，可用空格或者分号隔开）

注释：#
注释块：`#[[]]`

流程eg：
cd ~
mkdir camke01
cd cmake01/
nvim 文件名
......
nvim CMakeLists.txt
......
cmake .
make
./输出结果




## set
set：
1.用于定义变量：
set(SRC main.cpp add.cpp)  
add_executable(app ${SRC})
（set 用来给变量赋值，${变量名} 用来取值，值均为字符串类型）

2.用于指定使用此c++的标准：
set（CMAKE_C++ STANDARD 11)

3.:指定输出路径：
set(EXECUTABLE_OUTPUT_PATH "路径/bin")  
放可执行程序
set(LIBRARY_OUTPUT_PATH "路径/lib")
放库文件
（若路径不存在则自动创建，一般选择绝对路径）

4.通过命令行设置 CMake 缓存变量：  
cmake -S . -B build -DCMAKE_CXX_STANDARD=17  
（这里的 -D 是给 CMake 变量赋值，不是直接给 C++ 添加预处理宏）

## 常用变量：

1.CMAKE_SOURCE_DIR：顶层源目录  
2.CMAKE_BINARY_DIR：顶层构建目录  
3.CMAKE_CURRENT_SOURCE_DIR：当前 CMakeLists.txt 所在源目录  
4.CMAKE_CURRENT_BINARY_DIR：当前目录对应的构建目录  
5.PROJECT_SOURCE_DIR：当前 project() 对应的源目录  
6.PROJECT_BINARY_DIR：当前 project() 对应的构建目录  
7.PROJECT_NAME：项目名  
（只有顶层一个 project() 时，PROJECT_SOURCE_DIR 通常是项目根目录，子目录再调用 project() 时就不能都当成根目录）

其他输出路径变量，通常也写在创建目标之前：  
CMAKE_RUNTIME_OUTPUT_DIRECTORY：可执行文件，Windows 下也包括 DLL  
CMAKE_LIBRARY_OUTPUT_DIRECTORY：Linux 共享库等  
CMAKE_ARCHIVE_OUTPUT_DIRECTORY：静态库，Windows 下也包括导入库

## 搜索文件：

1.搜索指定目录下的源文件：  
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src" SRC)  
（结果存入 SRC，不递归搜索所有子目录）

2.按类型搜索：  
file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/_.cpp")  
add_executable(app ${SRC})  
（GLOB 按路径模式匹配，_.cpp 表示 .cpp 文件）

3.递归搜索：  
file(GLOB_RECURSE SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")  
（包括子目录，别把多个带 main 的测试文件都放进同一个可执行目标）

4.新增文件后：重新运行 CMake，再构建  
（普通 GLOB 和 aux_source_directory 不保证只运行 make 就能发现新文件，也可以直接在配置里列出源文件）

## 指定头文件路径：

1.目录级设置：  
include_directories("${PROJECT_SOURCE_DIR}/include")  
（告诉编译器去哪里找头文件，影响当前目录的目标及之后添加的子目录）

2.只给指定目标设置：  
target_include_directories(app PRIVATE "${PROJECT_SOURCE_DIR}/include")  
（app 要先定义，指定头文件路径不等于链接了库）

## add_library：

1.生成静态库：  
add_library(calc STATIC add.cpp sub.cpp)  
（Linux 下通常生成 libcalc.a，链接时把需要的代码纳入最终程序）

2.生成动态库：  
add_library(calc SHARED add.cpp sub.cpp)  
（Linux 下通常生成 libcalc.so，程序运行时还要能找到它）

3.库名：只写 calc，不自己补 lib 和 .a / .so  
（上面两种写法二选一，同一项目不能创建两个同名 calc 目标；Windows 下文件后缀和导出规则不同）

## 链接库：

1.指定库搜索目录：  
link_directories("${PROJECT_SOURCE_DIR}/lib")  
（解决链接时去哪里找库，对之后创建的目标生效）

2.目录级添加链接库：  
link_libraries(calc)  
add_executable(app main.cpp)  
（影响当前目录及子目录中之后创建的目标，静态库和动态库都可以）

3.给指定目标链接库：  
add_executable(app main.cpp)  
target_link_libraries(app PRIVATE calc)  
（app 必须先定义，后面可以写库目标名、库名或库文件完整路径，静态库和动态库都能链接）

4.链接项目里自己创建的库：  
add_library(calc STATIC add.cpp)  
add_executable(app main.cpp)  
target_link_libraries(app PRIVATE calc)  
（CMake 知道 calc 是项目目标，会安排它先构建，一般不必再写 link_directories）

5.链接已经编译好的外部库：  
target_link_libraries(app PRIVATE "${PROJECT_SOURCE_DIR}/lib/libcalc.a")  
（完整路径可以直接指定库文件，头文件路径仍要单独设置）

6.系统线程库：  
find_package(Threads REQUIRED)  
target_link_libraries(app PRIVATE Threads::Threads)  
（使用 pthread 举例，这种写法由 CMake 检测需要的线程编译或链接选项）

## PRIVATE、PUBLIC、INTERFACE：

1.PRIVATE：目标自己使用，不把依赖的使用要求作为公开接口  
2.PUBLIC：目标自己使用，也把使用要求传给依赖它的目标  
3.INTERFACE：只给依赖它的目标使用，自身不使用这项设置

target_link_libraries(A PUBLIC B)  
target_link_libraries(app PRIVATE A)  
（app 链接 A 时，也能得到 A 公开的 B 依赖要求，这不是 C++ 类的访问权限，也不等于控制函数符号是否导出）

## 运行时找不到动态库：

1.头文件路径：编译时找声明  
2.链接配置：链接时找函数等实现  
3.运行时路径：程序启动或加载共享库时找 .so 文件

error while loading shared libraries  
（这类错误要检查运行时搜索路径，link_directories 不是通用解决办法）

Linux 下可检查 RPATH、LD_LIBRARY_PATH  
（CMake 通常会为构建目录中的程序设置所需的运行时路径，移动程序或库后可能需要重新配置）

## message：

1.打印变量或路径：  
message(STATUS "src = ${CMAKE_CURRENT_SOURCE_DIR}")  
（在 CMake 配置时输出，不是 C++ 程序运行时输出）

2.日志类型：  
STATUS：普通状态信息  
WARNING：警告，继续处理  
AUTHOR_WARNING：面向开发者的警告  
SEND_ERROR：记录错误，继续处理，但跳过生成  
FATAL_ERROR：报错并立即停止

## 字符串与列表：

1.创建列表：  
set(SRC main.cpp add.cpp)  
（也可以用分号隔开，set(NAME "a b c") 则只有一个带空格的项）

2.通过 set 追加：  
set(SRC ${SRC} sub.cpp)  
（先取原来的值，再和新内容一起赋值回去）

3.通过 list 追加：  
list(APPEND SRC sub.cpp)  
（保留原有内容，不要再把 ${SRC} 作为追加内容写一遍，否则会重复）

4.移除指定文件：  
file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")  
list(REMOVE_ITEM SRC "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")  
（上面搜索结果是绝对路径，移除时也要用相同路径）

5.打印列表：  
message("${SRC}")  
（保留分号，message(${SRC}) 会把展开后的多个参数直接拼接显示）

## list：

（L 是列表变量名，n、item、pos、text 是用于保存结果的变量名）

1.获取长度：  
list(LENGTH L n)

2.获取元素：  
list(GET L 0 item)  
list(GET L -1 item)  
（0 是第一个，-1 是最后一个，取元素时越界会报错）

3.查找元素：  
list(FIND L value pos)  
（找到则返回索引，没找到返回 -1）

4.连接成字符串：  
list(JOIN L "," text)  
（用逗号连接各项）

5.末尾追加：  
list(APPEND L value)

6.开头追加：  
list(PREPEND L value)

7.指定位置插入：  
list(INSERT L 1 value)

8.移除末尾或开头元素：  
list(POP_BACK L)  
list(POP_FRONT L)

9.按值或索引移除：  
list(REMOVE_ITEM L value)  
list(REMOVE_AT L 0)

10.去重：  
list(REMOVE_DUPLICATES L)

11.反转：  
list(REVERSE L)

12.排序：  
list(SORT L)  
list(SORT L COMPARE NATURAL CASE INSENSITIVE ORDER DESCENDING)  
（COMPARE：STRING 字符串顺序、FILE_BASENAME 按文件名部分、NATURAL 自然顺序）  
（CASE：SENSITIVE 区分大小写、INSENSITIVE 不区分）  
（ORDER：ASCENDING 升序、DESCENDING 降序）

## 添加编译宏：

1.目录级添加：  
add_definitions(-DDEBUG)  
（类似 gcc / g++ 的 -DDEBUG，代码里的 `#ifdef DEBUG `就能成立）

2.只给指定目标添加：  
target_compile_definitions(app PRIVATE DEBUG)  
（app 先定义好，这里不用再加 -D）

3.set(DEBUG ON)：只设置 CMake 变量，不会自动给 C++ 定义 DEBUG  
（视频的“自定义宏”讲的是 C/C++ 预处理宏，不是 CMake 的 macro() 命令）

## 嵌套 CMake：

1.按模块分别写 CMakeLists.txt，顶层添加子目录：  
add_subdirectory(calc)  
add_subdirectory(sort)  
add_subdirectory(test1)  
add_subdirectory(test2)  
（子目录里要有 CMakeLists.txt，执行到 add_subdirectory 时先处理子目录，再继续处理父目录后面的内容）

2.顶层先设置公共变量，再添加子目录：  
父目录的普通变量可以向子目录传递  
子目录自己设置的普通变量，默认不会修改父目录，也不会自动传给兄弟目录

3.模块生成库：  
add_library(calc STATIC add.cpp sub.cpp)  
target_include_directories(calc PUBLIC "${PROJECT_SOURCE_DIR}/include")

4.测试程序使用库：  
add_executable(test_calc main.cpp)  
target_link_libraries(test_calc PRIVATE calc)  
（calc 公开了头文件路径，test_calc 使用它时也能得到这个路径）

5.顶层统一写 project 和 cmake_minimum_required 即可  
（简单子目录不必重复，单纯存头文件的目录也不一定需要单独写 CMakeLists.txt）

## 静态库依赖其他库：

1.静态库 A 使用静态库 B：  
add_library(B STATIC b.cpp)  
add_library(A STATIC a.cpp)  
target_link_libraries(A PRIVATE B)

2.程序使用 A：  
add_executable(app main.cpp)  
target_link_libraries(app PRIVATE A)  
（CMake 会记录依赖，在最终链接程序时处理 B，不是把 libB.a 自动合并进 libA.a）

3.如果只把 libA.a 文件交给别人，仍要提供它依赖的 B 和对应配置

4.把 B 改成 SHARED，就是静态库依赖动态库  
（程序运行时还需要找到 B 的动态库文件）

5.静态库的 PRIVATE 实现依赖，仍可能作为最终链接需要的依赖传递  
（如果 A 的公开头文件也要求使用者获取 B 的头文件等使用要求，就考虑 PUBLIC）

## 综合项目：

1.视频示例：HTTP 服务器，分成 common、http、reactor、tcp、thread 等模块

2.每个模块：收集自己的源文件、设置头文件路径、生成库

3.顶层：通过 add_subdirectory 添加模块，再定义 server 并链接需要的库

4.找不到 EventLoop.h：检查对应目标的头文件路径  
（库生成了不代表程序已经链接它，头文件配置和库依赖都要写清楚）

5.模块相互依赖时：检查模块划分和文件放置，不只靠调整 add_subdirectory 顺序

## 查错：

1.配置失败：检查 CMake 命令、路径、目标名、版本和编译器环境  
2.找不到头文件：检查 include 路径、文件名大小写  
3.cannot find -lxxx：检查链接时的库名和库文件位置  
4.undefined reference：检查实现有没有加入目标、库有没有链接、声明和定义是否一致  
5.运行时找不到 .so：检查动态库搜索路径








# Catkin

## 基本概念：

1.Catkin：ROS 1 使用的构建系统，基于 CMake，组织多个软件包的构建和依赖关系  
（底层仍会调用编译器，包内也要写 CMakeLists.txt）

2.软件包：按功能组织代码的单位，例如摄像头包、识别包、控制包

3.工作空间：集中存放和构建这些软件包的目录

## 常见文件和目录：

1.CMakeLists.txt：写软件包怎样构建  
2.package.xml：写包名、版本、作者和依赖等信息  
3.src：存放各个软件包的源代码  
4.build：存放构建过程中的中间文件  
5.devel：存放开发时使用的构建结果和环境脚本

## 创建工作空间：

1.加载 ROS 环境：  
source /opt/ros/noetic/setup.bash  
（以已经安装的 ROS Noetic 为例，路径中的 noetic 要对应实际安装的发行版）

2.创建目录并初始化：  
mkdir -p ~/catkin_ws/src  
cd ~/catkin_ws  
catkin init  
（下面使用 catkin_tools 的流程，需要已经安装 ROS 和 catkin_tools）

## 创建软件包：

1.进入工作空间的 src：  
cd ~/catkin_ws/src

2.创建包并指定依赖：  
catkin_create_pkg test_pkg roscpp std_msgs  
（test_pkg 是包名，roscpp 提供 ROS 的 C++ 支持，std_msgs 提供标准消息类型）

## 构建和使用：

1.构建工作空间中的包：  
cd ~/catkin_ws  
catkin build  
（catkin build 是 catkin_tools 提供的命令）

2.加载工作空间环境：  
source ~/catkin_ws/devel/setup.bash  
（让当前终端找到工作空间中的包，换新终端时需要重新加载）




## 大模型辅助学习的思考
利：
1.可以及时解惑
2.制定学习规划，复习指导
3.补充拓展知识点
4.解决无意义且麻烦的事情（安装软件，配置 环境等等）
弊：
记忆更浅，许多疑惑点由ai即时解决而可能导致思考不足