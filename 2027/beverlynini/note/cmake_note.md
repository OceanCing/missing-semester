# 发展史
单个源代码文件compiled by gcc/g++-->源文件增加 compiled by makefile to 提效-->工程体量增大 用Cmake来处理
cmake:对make 工具的生成器，简化编译过程，可以管理大型的项目，有良好的拓展性
## 源代码到可执行程序
1. 预处理："#"开始的指令就是预处理指令，展开头文件&宏定义，删掉注释。#include //把头文件的所有代码全部copy过来，实现hello.c（源程序）--预处理器cpp--> hello.i（修改了的源程序）
2. 编译：hello.i --编译器ccl--> hello.s（汇编程序），汇编语言是CPU真正能直接读懂并执行的底层纸令，介于cpp代码和机器码之间
3. 汇编：hello.i --汇编器as--> hello.o（可重定位的目标程序）（二进制）
4. 链接：许多目标文件链接成可执行程序，尤其在有多个源代码时可实现文件增量更改
### 举例
- g++ -E main.cpp -o main.ii //-E：预处理；main.cpp：要处理的文件；-o：-output，输出----> 将预处理结果输出文件main.ii中（.ii是预处理后cpp文件的后缀名）
- cat main.ii 就能看到头文件的内容
- g++ -S main.ii -o main.s //-S：编译
- g++ -c main.s -o main.o //-c：Compile/Assemble，将汇编语言完全翻译成机器码
- g++ main.o -o demo 
- ./demo //运行demo这个可执行程序
### 以上四步，是g++ main.cpp -o demo 的拆分
### 对于有多个源代码文件，可以用g++ main.cpp add.cpp -o demo来将二者链接在demo这一个可执行文件里，比如说可以使得在main.cpp里面定义的函数可以运用到add.cpp里面
- 增量链接：此时如果修改其中一个cpp文件比如修改了add.cpp，那么就可以只对add.cpp进行再一次汇编，g++ -c
## Makefile
-  file：一份自动编译的说明书，告诉make工具，哪些文件要编译，编译顺序是什么，需要哪些命令，实现程序构建的自动化
- 操作：vim一个makefile，把上面提及到流程所需要的代码打包放进去，然后运行makefile就好了
```makefile
demo: add.o main.o
        g++ add.o main.o -o demo
add.o: add.cpp
        g++ -c add.cpp -o add.o
main.o: main.cpp
        g++ -c main.cpp -o main.o
clean:
        rm -rf *.o demo /*把带有.o后缀的文件和demo删掉*/
```
(有点像函数的作用，作为一个封装）
## Cmake
- 过程式语言，是一个跨平台(在linux生成makefile，在windows生成project) 的编译工具，可以产生标准的建构档makefile或者project
- 一定要将该文件命名为CMakeLists.txt
- 然后执行 cmake . （.：在当前目录执行该程序）
- 出现makefile后可以像之前一样执行 make ...
### 使用1：最基础
```cmake
cmake_minimum_required(VERSION 3.16) /*最低版本号*/
project(add_demo) /*定义了项目的名称*/
add_executable(demo main.cpp add.cpp) /*添加可执行程序，后面还要加如头文件之类的的话直接加在这个后面就好*/
```
### 使用2：自动扫描
```
cmake_minimum_required((VERSION 3.16) 
project(add_demo) 
add_executable(demo 
    src/main.cpp
    src/add.cpp
)

/*目标头文件目录*/
taget_include_directories(demo PRIVATE
include)
/*在include文件夹里面寻找执行demo项目的所有头文件*/
```
### 一份cmakelist.txt
1. 声明：Cmake和C++版本
  - cmake_minimum_required：至少需要 CMake 3.10。
  - project(...)：项目名是 ros_package_template，后面可以用 ${PROJECT_NAME} 代替。
  - CMAKE_CXX_STANDARD 14：使用 C++14。
  - CMAKE_CXX_STANDARD_REQUIRED ON：编译器必须支持 C++14，不能偷偷退回旧标准。
  - ${PROJECT_NAME} 展开后就是 ros_package_template

2. 编译警告选项

  add_definitions(-Wall -Werror)

  - -Wall：打开常用警告。
  - -Werror：把警告当成错误，有警告就编译失败。

  - 这会让代码更规范。调试时可以临时去掉 -Werror。

  - 这里的设置会影响该文件中的所有目标。现代 CMake 更推荐按目标设置：
     target_compile_options(目标名 PRIVATE -Wall -Werror)
3. 查找依赖包
```
 find_package(catkin REQUIRED
        COMPONENTS
        roscpp
        sensor_msgs
        roslint
)

find_ package(Eigen3 REQUIRED）
find_package(Boost REQUIRED)
```
- catkin：ROS 1 的构建系统。
- roscpp：ROS C++ 客户端库。
- sensor_msgs：传感器消息类型。
- roslint：C++ 代码风格检查工具。
- Eigen3：线性代数库。
- Boost：C++ 工具库。
- REQUIRED：找不到依赖时直接停止配置并报错。
4. 声明ROS包信息
```
 catkin_package(
     INCLUDE_DIRS
     include
     ${EIGEN3_INCLUDE_DIR}
     LIBRARIES
     ${PROJECT_NAME}_core
     CATKIN_DEPENDS
     roscpp
     sensor_msgs
     DEPENDS
)
```
- INCLUDE_DIRS include：对外提供 include 目录中的头文件。
- ${EIGEN3_INCLUDE_DIR}：因为 Eigen 的头文件路径变量名不是 ROS 常见的标准形式，所以手工加入。
- LIBRARIES ${PROJECT_NAME}_core：对外提供 ros_package_template_core 库。
- CATKIN_DEPENDS roscpp sensor_msgs：其他包依赖本包时，也间接需要这两个 ROS 包。
- DEPENDS：写系统依赖的地方。

简单理解：这一段是“打包说明书”，主要给依赖本包的其他项目看。
 
5. 头文件搜索路径

```
  include_directoeries(
      include
      ${catkin_INCLUDE_DIRS}
      ${EIGEN3_INCLUDE_DIR}
      ${Boost_INCLUDE_DIRS}
)
```
编译时到这些目录中查找头文件。
这里的设置对所有目标生效。现代 CMake 更推荐分别设置：

target_include_directories(目标名 PRIVATE include)

6. 创建库

```
  add_library(${PROJECT_NAME}_core
      src/Algorithm.cpp
)
```
这会创建一个库：

ros_package_template_core

它的源码是：

src/Algorithm.cpp

Algorithm.cpp 中的函数可以提供给测试程序或其他程序使用。

这里的 _core 只是命名习惯，表示“核心算法库”。

7. 创建可执行程序

``` 
  add_executable(${PROJECT_NAME}
      src/${PROJECT_NAME}_node.cpp
      src/RosPackageTemplate.cpp
)
```

- 生成一个ROS节点程序：ros_package_template
- 它由两个源文件：src/ros_package_template.cpp和src/RosPackageTemplate.cpp 编译而成
- add_library是做成库；add_executable是做成可以运行的程序

8. c++的标准要求

```
  target_compile_features(${PROJECT_NAME}_core INTERFACE cxx_std_11)
  target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
```
cxx_std_11 表示至少需要 C++11 特性。

不过这里使用 INTERFACE 比较特殊：

- INTERFACE 不会直接作用于目标自己的源文件。
- 它表示“谁链接这个目标，谁就需要 C++11”。
- ros_package_template 是最终可执行文件，通常没有其他目标链接它，因此这一行的实际作用很有限。
- 使用更高级的语言标准不会退回这里的c++11。如果确实要求更高级的语言如c++14,那么就把11改成14就好

9. 等待消息代码生成

```
  add_dependencies(${PROJECT_NAME}_core
      ${catkin_EXPORTED_TARGETS}
)
  add_dependencies(${PROJECT_NAME}
      ${catkin_EXPORTED_TARGETS}
)
```

- ${catkin_EXPORTED_TARGETS} 通常代表：消息、服务或动作生成的自定义构建目标
- add_dependencies 只表示“构建顺序
- 总的来说，流程是：先生成消息相关代码，再编译我的程序和库
- 不等于链接库。

10. 链接库
  
```
  target_link_libraries(${PROJECT_NAME}_core
      ${catkin_LIBRARIES}
)
  target_link_libraries(${PROJECT_NAME}
      ${PROJECT_NAME}_core
      ${catkin_LIBRARIES}
)
```

- ros_package_template_core 链接 catkin 提供的库
- 可执行程序链接自己的核心库和catkin库
- 即，可执行程序--依赖--> ros_package_template_core --依赖--> catkin库
- 库的放置顺序：通常要把被依赖的库放在相对，更后面的位置

11. 安装目标

```

  install(
      TARGET ${PROJECT_NAME} ${PROJECT_NAME}_core
      ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
      LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
      RUNTIME DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)
```

这里规定了安装时，可执行程序和库分别放到什么位置：

- RUNTIME：可执行程序。
- LIBRARY：动态库。
- ARCHIVE：静态库。

CATKIN_PACKAGE_*_DESTINATION 是 catkin 提供的安装路径变量。

12. 安装头文件和文档

```
  install(
      DIRECTORY include/${PROJECT_NAME}/
      DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
      FILES_MATCHING PATTERN "*.hpp"
)
```

安装 include/ros_package_template/ 下面所有 .hpp 头文件，但不安装其他类型的文件。

install(
    DIRECTORY doc
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

把 doc 目录安装到包的共享资源目录。

13. 测试

```
  if (${CATKIN_ENABLE_TESTING})
     set(CMAKE_CXX_FLAGE "${CMAKE_CXX_FLAGES} -pthread"}
     catkin_add_gtest(${PROJECT_NAME}-test
         test/test_ros_package_template.cpp
         test/AlgorithmTest.cpp)
     target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME}_core)
  endif()
```

- CATKIN_ENABLE_TESTING 为真时，才添加测试。
- -pthread：启用多线程支持，GoogleTest 等测试环境经常需要。
- catkin_add_gtest：创建一个基于 gtest 的测试程序。
- 测试程序的源文件是 test/test_ros_package_template.cpp 和 test/AlgorithmTest.cpp。
- 测试程序链接核心库，因此可以测试 Algorithm.cpp 中的功能。

14. roslint 代码检查

```
  if(CATKIN_ENABLE_TESTING) 
    roslint_cpp()
  endif()
```
- roslint_cpp()：把代码检查加入catkin的测试流程
作用是调用 ROS 的 C++ 风格检查工具，一般检查：

- ROS 命名风格
- 缩进和空格
- 头文件保护
- 行长度
- 部分 C++ 代码规范
- ROS 特有的命名和格式要求

### 小结
find_package(...)      # 找依赖
catkin_package(...)    # 声明包对外提供什么
add_library(...)       # 创建库
add_executable(...)    # 创建程序
add_dependencies(...)  # 规定构建先后顺序
target_link_libraries(...) # 链接库
install(...)           # 安装文件
catkin_add_gtest(...)  # 添加测试
roslint_cpp()          # 添加代码风格检查

四个最基础概念
1. 依赖：编译需要哪些库。
2. 目标：要生成一个库还是一个可执行程序。
3. 链接：程序运行时需要调用哪个库。
4. 安装：编译完成后文件放到哪里。

