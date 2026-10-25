# 声明最低版本
cmake_minimum_required(VERSION 3.10)
# 声明项目名称
project(hello)

# 找到所有源文件（或者手动列出）这里意思是从src文件夹里找cpp的源代码
file(GLOB SOURCES "src/*.cpp")
# 将收集到的源文件编译成可执行程序（名字对应第一个参数）
add_executable(hello ${SOURCES})
# 添加头文件搜索目录（注意第一个参数要与上面对应）意思是从src文件夹里找头文件
target_include_directories(hello PRIVATE ${CMAKE_SOURCE_DIR}/inc)
