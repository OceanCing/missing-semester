# 关于cmake和catkin的学习笔记
##CMake
###定义项目名称和语言
**语法**：`project(<项目名>[<语言>...])`
语言参数:CXX（C++),C(c语言)
###添加可执行文件
**语法**：`add_executable(<目标名><源文件>...)`
将指定的源文件编译生成一个可执行文件
###添加库文件
**语法**：`add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)`
STATIC：静态库（.a / .lib），编译时直接嵌入可执行文件
SHARED：动态库（.so / .dll），运行时加载
不指定类型时，由 BUILD_SHARED_LIBS 变量决定
add_library(MyLib STATIC library.cpp)
###链接库到目标
**语法**：`target_link_libraries(<目标> <库>...)`
将指定的库链接到目标（可执行文件或其他库）
可以链接自己项目中的库目标，也可以链接外部库
###设置变量
语法：`set(<变量名> <值>...)`
定义一个普通变量，后续通过 ${变量名} 引用
同时设置多个值（列表）
`set(SOURCES main.cpp utils.cpp helper.cpp)`
`add_executable(MyApp ${SOURCES})`
###为目标指定头文件路径
语法：`target_include_directories(<目标>
           [BEFORE | AFTER]
           [SYSTEM]
           [PUBLIC | PRIVATE | INTERFACE]
           <路径>...)`
 PUBLIC：当前目标和依赖它的目标都能使用该路径
 PRIVATE：只有当前目标能使用
 INTERFACE：只有依赖当前目标的其他目标能使用
eg:`target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)`
###增量编译
```makefile
demo: main.o add.o
    g++ main.o add.o -o demo
main.o: main.cpp add.h
    g++ -c main.cpp
add.o: add.cpp add.h
    g++ -c add.cpp
```        
##Catkin
`src/` :存放码源包
`build/` :存放CMake和catkin的缓存，中间文件
`devel/` :存放开发过程中生成的可执行文件和库，以及环境变量脚本
编译工作空间：`catkin_make`
刷新环境变量（每次打开新终端都要做）：`source devel/setup.bash`
