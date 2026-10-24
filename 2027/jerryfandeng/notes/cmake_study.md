# 编译与链接流程
1. 预处理（展开头文件、处理 #if #endif，.c -> .i）
2. 编译（.i -> .s）
3. 汇编（.s -> .o）
4. 链接（.o 和库文件 -> 可执行文件）
# 工具不断升级
1. 命令行直接用gcc(编译单个文件好用 但多个文件不好使用)
2. Makefile （自动化编译 只编译改动的 效率快 跨平台兼容性不好 shell解释命令不同） 
3. CMake(CMakeLists.txt指导Makefile的生成 跨平台兼容性好)
   
# catkin
catkin是ros对cmake的扩展
1. catkin工作空间：src build devel三个目录 src放源文件 build(中间文件，二进制文件) devel(开发空间setup.bash 可执行文件和库)
2. ros包结构:package.xml+CMakeLists.txt
3. ros包依赖声明：catkin_create_pkg test_pkg std_msgs roscpp

## ros工程搭建
**最初**
```
|catkin_ws
|--src
```
**给src添加依赖 生成包**
```
|catkin_ws
|--src
 |--包名
  |--package.xml
  |--src
  |--CMakeLists.txt
```
**catkin_ws下catkin build编译工程**
```
|catkin_ws
|--CMakeLists.txt
|--build
|--devel
|--src
 |--包名
  |--package.xml
  |--src
  |--CMakeLists.txt

```
# cmake实战
```
MyProject
├── CMakeLists.txt
├── src
│   ├── CMakeLists.txt
│   ├── include
│   │   └── mylib.h
│   ├── lib
│   │   ├── module1.cpp
│   │   └── module2.cpp
│   └── main.cpp
└── tests
    ├── CMakeLists.txt
    └── test_main.cpp
```
### 顶层cmake
```
# cmake最低版本
cmake_minimum_required(VERSION 3.20)
# project名称
project(MyProject VERSION 1.0)
# 设置c++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 头文件路径
include_directories(src/include)
# 子目录
add_subdirectory(src)
add_subdirectory(tests)
# 可执行文件与库链接
target_link_libraries(MyExec MyLib)
target_link_libraries(MyTest MyLib)
```
### src层cmake
```
add_library(MyLib lib/module1.cpp lib/module2.cpp)
add_executable(MyExec main.cpp )
```
### tests层cmake
```
add_executable(MyTest test_main.cpp )
```

# ai帮助学习的经历
1. 我让ai生成错误的Cmake.txt文件，我根据运行cmake命令报错的信息修改错误的Cmake.txt文件，了解熟悉cmake语法


# 对用ai帮助学习的思考
1. ai快速解释新知识，我能轻易的看懂，但看懂和学会，学会和能用起来有很大的鸿沟，警惕看懂得到的满足感，以为自己真正掌握
2. 在我学习esp32蓝牙ota时，我有完整源码，但我想自己写一遍，去实践，我就让ai阅读源码找到乐鑫官方的一个和源码相关的代码框架，我在这个框架上重新写，ai分解复刻源码的目标，我边改ai找到的代码一边烧录到esp32，了解了gatts服务表 gap,期间很有反馈的学习(每次烧录到esp32,手机的蓝牙调试助手会有响应,很有成就感)
3. 学习新东西，得先向ai说自己对这新东西的理解，再让ai纠正，给出深入方向，如此循环学习

   
