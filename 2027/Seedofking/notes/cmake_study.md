#### 前言: 
##### 我一直以来有做笔记的习惯, 所以将部分与第二节课相关的笔记整理到了该作业笔记中, 并且在后面加上了一些对AI的看法

## CMake
### 编译过程
1. 预处理：删除注释, 展开宏定义和\#include的代码, 将.h文件复制到.cpp, 处理\#endif等预编译指令 .i
2. 编译: 将预处理后的.i文件生成汇编代码 .s
3. 汇编: 将汇编后的.s文件生成二进制文件 .o
4. 链接: 将多个二进制文件.o链接在一起, 生成可执行文件
###### 在有多个源代码的时候可以实现文件的增量更改, 即单独构建指定的文件, 已构建的其他文件可以不构建

### 静态链接与动态链接

#### 概念:

**静态链接**：编译阶段把库代码（`libfoo.a`）直接复制进可执行文件，运行时不再依赖库文件。
**动态链接**：编译时只记录对共享库（`.so`/`.dll`）的引用，运行时由动态链接器加载并解析符号。

#### 区别

| 维度 | 静态链接 | 动态链接 |
|---|---|---|
| 链接时机 | 编译期 | 运行期 |
| 可执行文件 | 大 | 小 |
| 运行时依赖 | 无 | 需对应共享库 |
| 内存占用 | 每进程一份 | 多进程共享 |
| 库更新 | 需重新编译 | 替换 `.so` 即可 |
| 启动速度 | 较快 | 略慢 |
| 部署 | 单文件即可 | 需保证库版本 |

#### 优劣

**静态链接**
- 优点：部署简单、运行稳定可预测、启动略快、便于跨模块优化。
- 缺点：体积大、更新困难、内存无法共享、GPL 库可能触发传染条款。

**动态链接**
- 优点：省磁盘和内存、库易升级、支持插件机制、可执行文件小。
- 缺点：依赖管理复杂（版本冲突、`DLL` 地狱）、启动有开销、ABI 不兼容会运行失败、库被替换有安全风险。

#### 四、使用场景

**静态链接**：嵌入式、容器镜像、单文件分发工具（如 Go 默认）、要求启动和确定性高的场景。
**动态链接**：常规桌面/服务器程序、系统基础库（glibc）、需热更新或插件化的软件。

#### 总结
静态链接是把库固定死在程序里，换来部署简单和运行确定，代价是体积和更新成本；动态链接是运行时按需拼装，换来空间节省和易升级，代价是依赖管理和兼容性复杂度。实际工程常混合使用：核心稳定部分静态链接，通用/需升级部分动态链接。


### 用指令实现编译
###### 要先进入.cpp文件所在目录

#### 一步到位: `g++ helloworld.cpp -o helloworld`
#### 运行: `./helloworld`

#### 分步编译:
##### 1.预处理:
    g++ -E helloworld.cpp -o helloworld.i

##### 2.编译:
    g++ -S helloworld.i -o helloworld.s

##### 3.汇编:
    g++ -c helloworld.s -o helloworld.o

##### 4.链接:
    g++ helloworld.o -o helloworld

##### 5.运行:
    ./helloworld




### 该笔记由一个C++可执行项目的CMakeList.txt改为md, 既是可运行代码也是笔记

``` cmake

cmake_minimum_required(VERSION 3.28)#指定CMake可用的最低版本，比该版本低会报错

project(BigBuild)#指定要构建项目的名字，随便写

set(CMAKE_CXX_STANDARD 20)#指定C++语言的标准版本

set(CMAKE_EXE_LINKER_FLAGS "-static")
#设置链接方式为静态链接，(cmake默认是动态链接)，动态链接会让.exe可执行文件每次运行时会寻找其依赖文件，将.exe移出文件夹其就会失效
#而静态链接方式生成的.exe文件不会有外部依赖，放在各种地方都可以运行

#单文件夹单工程多文件编译
#add_executable(Class_1 Class_1_main.cpp Class_1_App.cpp)
#executable是可执行的意思
#Class_1是指定生成的可执行文件(.exe)的名字，会生成在cmake-build-debug里面
#工程名加一个空格 后面可以跟一个或多个.cpp文件，.h文件可不写，可以用空格间隔，可以用; 间隔

#[[
若想避免手写，则可以使用file来收集目录下所有的文件，存在一个名字里
file(GLOB Class_1_Files(名字自取）  ————如果只想取该目录下一层的文件，则用GLOB，如果想取该目录下全部子文件里的文件，则用GLOB_RECURSE
        "${PROJECT_SOURCE_DIR}/tests/*.h"
        "${PROJECT_SOURCE_DIR}/tests/*.h"
    )
add_executable(Class_1 ${Class_1_Files})
]]

#单文件夹多工程编译
add_executable(Function_Test Function_Test_App.cpp main.cpp)    #多文件
add_executable(Class_2 Class_2.cpp)   #单文件
add_executable(Class_3 Class_3.cpp)
add_executable(Class_4 Class_4.cpp)
add_executable(Class_Test CLass_Test.cpp)
add_executable(First_Time First_Time.cpp)
add_executable(IO_Test IO_Test.cpp)
add_executable(Memory_Test Memory_Test.cpp)
add_executable(Pointer_Test Pointer_Test.cpp)
add_executable(Reference Reference.cpp)
#一个个写即可

#target_include_directories(Class_1 PRIVATE ${PROJECT_SOURCE_DIR})

set(A ON) #定义A为ON，ON属于关键字
if (A)   #A是ON，if会被判定为true
    #message可以将信息打印出来，点击重新加载cmake项目时可在cmake日志查看，是--开头的，比如-- A:D:/3-Clion/C++/Class_1
    #message(STATUS "   ")可显示冒号内的内容
    #$(   )的作用是查看其内容的值
    message(STATUS "A:${PROJECT_NAME}") #显示工程的名称
    message(STATUS "A:${PROJECT_SOURCE_DIR}")   #显示工程的文件夹源文件目录，即包含CMakeLists.txt文件的目录
    message(STATUS "A:${PROJECT_BINARY_DIR}")   #显示当前CMake运行的二进制文件目录
endif ()


#[[工具链设置：
0.如果改编译配置先把原来存在的编译文件删掉
1.要在设置里把工具链配好，有stm32和mingGW
2.要在调试那里把Debug的工具链选为mingGW，Generator也会自动跟着工具链选好
3.设置运行时可能需要执行的操作和参数
]]

#[[
CMAKE_CXX_FLAGS 指定编译C++代码时使用的编译选项
CMAKE_C_FLAGS   指定编译C代码时使用的编译选项
CMAKE_EXE_LINKER_FLAGS  指定链接可执行文件时的链接选项
CMAKE_SYSTEM_NAME   指定当前操作系统名称(Windows, Linux)
CMAKE_SYSTEM_PROCESSOR  指定当前处理器类型(x86, x86_64等)
CMAKE_CXX_COMPILER_ID   指定当前使用的C++编译器，同理可得编译器对应的名字

include_directories(${如PROJECT_SOURCE_DIR}/include)   也是暴露一个目录，用于在编译阶段暴露头文件
link_directories( ${如PROJECT_SOURCE_DIR}/lib)将一个目录的文件都暴露给整个Cmake工程，{}里面是一个路径，主要在链接阶段暴露库文件(.lib .a .so)
add_library(自己取的库名称，文件如main.cpp) 将后面的文件生成一个库，供其他程序使用
target_link_libraries(工程名PROJECT_NAME，库名称)  给工程链接一个库，使用之前要先用link_directories暴露库路径

]]
```

## Catkin的CMakeLists.txt

``` cmake
cmake_minimum_required(VERSION 3.0.2)
project(dm1)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msg
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a exec_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a exec_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a exec_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES dm1
#  CATKIN_DEPENDS roscpp rospy std_msg
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/dm1.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

```
```

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
# add_executable(${PROJECT_NAME}_node src/dm1_node.cpp)

```
### ↑添加可执行文件:
#### 这里的 `add_executable(${PROJECT_NAME}_node src/dm1_node.cpp)`可以复制放在文件末尾
#### 就可以给这个包的工程取名和添加可执行文件

```

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

```
```

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )

```
### ↑添加需要编译的库:
    # target_link_libraries(${PROJECT_NAME}_node
    #   ${catkin_LIBRARIES}
    # )
#### 复制到文件尾就可以让include的库被加入编译

```

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# catkin_install_python(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_executables.html
# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark libraries for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_libraries.html
# install(TARGETS ${PROJECT_NAME}
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_dm1.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)

---------------------------------------------------------------

add_executable(${PROJECT_NAME}_node src/dm1_node.cpp) # 默认工程名
add_executable(AAAAADM1_node src/dm1_node.cpp) # 自己起名

target_link_libraries(${PROJECT_NAME}_node
${catkin_LIBRARIES}
)

```

## Catkin

#### 构建过程
![构建过程](https://bee-reg-ab.imagency.cn/mr/202610/23/6adb11f2f10c7.png)

#### catkin工作空间层级

catkin_ws/
├── .catkin_tools/              # catkin init 生成的隐藏配置目录 (保存构建配置)
│   ├── CATKIN_IGNORE
│   └── profiles/
│       └── default/
│           └── config.yaml
├── **CMakeLists.txt**/ # 顶层CMakeLists.txt负责管理包, 无需自己修改
├── **build**/                      # 编译中间文件 (catkin build 自动生成)
│   └── my_first_pkg/           # 每个包一个独立的编译目录
│       └── ...
├── **devel**/                      # 开发空间 (存放环境变量、可执行文件等)
│   ├── setup.bash              # source 这个文件来配置环境
│   ├── setup.sh
│   ├── env.sh
│   ├── local_setup.bash
│   └── ...
├── **logs**/                       # 构建日志 (包含编译记录)
│   └── my_first_pkg/
│       └── build.catkin-tools.<timestamp>.log
└── **src**/                        # 源码空间 (你主要工作的地方，存放所有功能包)
    └── my_first_pkg/           # 你的具体功能包 (Package)
        ├── CMakeLists.txt      # package的编译规则, 用于管理包内代码编译 (必须)
        ├── package.xml         # package的描述信息 (必须)
        ├── src/                # 源代码文件 (如 .cpp 节点代码)
        ├── include/            # C++ 头文件 (如 .h, .hpp)
        ├── scripts/            # 可执行脚本 (如 .py 节点代码)
        ├── msg/                # 自定义消息 (自定义的 .msg 文件)
        ├── srv/                # 自定义服务 (自定义的 .srv 文件)
        ├── models/             # 3D模型文件 (如 URDF 引用的 .dae, .stl)
        ├── urdf/               # urdf文件 (机器人模型描述)
        └── launch/             # launch文件 (用于一次性启动多个节点)



#### package层级
![package层级](https://bee-reg-ab.imagency.cn/mr/202610/24/6adc8b51a2cc3.png)


### 初始化catkin工作空间:

#### 1.创建工作空间文件夹:
    先创建要作为工作空间的文件夹, 常命名为 工程名_ws, 而该文件夹内的文件夹名须采用默认名如src

#### 2.初始化工作空间:
    catkin_init_workspace

#### 3.在工作空间创建src和功能包:
    cd catkin_ws
    mkdir src
    cd src

    catkin_create_pkg 包名 依赖列表
    catkin_create_pkg test_pkg rospy roscpp std_msgs


#### 4.编译工作空间:
    cd catkin_ws
    catkin build
###### 若编译失败, 显示找不到src, 有可能是catkin_ws以上的目录存在.catkin_tools, 需要删除

###### 至此, catkin_ws下便有了CMakeLists.txt, build, cmake-build-debug, devel, logs, src

#### 5.加载ros系统和工作空间环境变量:
    source /opt/ros/noetic/setup.bash
    source /root/catkin_ws/devel/setup.bash
###### 这两个命令可以放在容器系统里的 .bashrc, 每次启动自动加载

#### (查看是否成功加载:)
    printenv | grep ROS
###### 若看见类似 ROS_DISTRO=noetic, ROS_VERSION=1等变量则成功加载ros系统环境变量
    echo $ROS_PACKAGE_PATH
###### 若输出包含catkin_ws/src路径, 例如/rose_ws/src/rostest_pkg:/opt/ros/noetic/share
###### 则说明成功加载, 若只有/opt/ros/noetic/share则没有成功source /devel/setup.bash


#### 在Clion添加环境变量
在 设置/构建、执行、部署/CMake 右侧选项栏中的 CMake选项 点+号插入宏, 选择项目根路径插入 
![插入宏](https://s41.ax1x.com/2026/10/25/pnD7wkQ.png)



添加ros系统环境变量(加一个空格与前面分隔):
 -DCMAKE_PREFIX_PATH=/opt/ros/noetic


![添加ros变量](https://s41.ax1x.com/2026/10/25/pnD70Yj.png)


### 代码开发:
在catkin_ws/src/test_pkg/src下, 可以开始使用clion开发代码
catkin_ws下的src是用于存放依赖包的, 而包文件夹下的src是用于存放这个包的代码的
写代码时先在包的CMakeLists.txt里面添加.cpp文件, 否则无法识别到外部库

## 我使用AI的经历和对AI的一些看法

#### 用AI的过往经历
我第一次接触vibe coding是在去年26年4月, 当时师兄让我尝试学习ai, 让我完全用ai做一次控制考核题练手
于是在此之后, 直至参加DynamicX的考核之前, 我竟然再也没有手写过一次代码, 也没有自己做过一个实验报告或文档工作
当时各比赛对于stm32的掌握程度与代码水平是超出我能力的, 而我又拿不出哪怕一天的时间去学习, 也学不完
于是无奈依赖codex  

我从gpt5.4, 一路用到5.5, 5.6, ai迭代速度飞快, 能做的事越来越多
但用了那么久, 我对于它们, 无疑是失望的
再强大的ai, 也要依靠于人的强大; ai再强大的能力, 也会在一次又一次掉链子的时候毁掉我的比赛
ai总喜欢加诸多的校验与保护措施, 有各种无比酷炫的优化措施, 但我想要的仅仅是一个能发数据包控制电机的程序而已
我尽力地要求代码尽可能简单, 而聪明如gpt, 终于也都会把代码变成几百上千行无从找bug的一坨
而且这些问题往往处在ai自己增添的"保护与校验"上
在比赛最后的一周, 72小时, 比赛前8个小时的凌晨, 我只能睁着眼看着ai在那缓慢而无效地干活, 就跟个无能的丈夫一样
在这种时候, 我只是多么渴望我能写代码, 写出一个只有简陋直接的底层控制的小车, 而不是有一大坨代码但主程序都瘫痪的废物
但我会改变

#### 我对于AI的看法
在上学期比赛结束后我就没有继续给中转站续钱了, 以至于大家在讨论gpt6 astra有多无敌的时候, 我都没有体验过
不过鉴于deepseek在处理诸如pdf, ppt, 整理数据生成图片之类的劣势, 我还是给大厦将倾的中转站送了点钱, 
我保留了用gpt和claude的能力, 而我再也没用它们在ubuntu里干过什么活, 现在的任务都交给deepseek了
我拿deepseek的网页版学习ros, 补充知识, 接入claude code处理涉及复杂环境配置的问题 

现在的我不再想让ai碰我的代码了, 我自己没招时就让ai找问题, 然后我自己改, 记在笔记里面, 下次就能自己解决了
我现在每一次使用ai都只有一个目的: 那就是提升我这个人, 而不是用ai做一件事
我会亲手写下这次考核的每一行代码, 只有我自己有能力, 才有供ai放大我能力的资格