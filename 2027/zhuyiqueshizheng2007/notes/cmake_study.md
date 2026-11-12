# **cmake、catkin和大模型学习的学习笔记**

## **cmake基础**
- **背景**
  - gcc/g++命名方式对大量源文件的编译工作显得繁琐，因而开始用Makefile
  - 但在工程体量增大的前提下，Makefile也不能满足需求，因而有了Cmake工具

- **简介**
  - Cmake通过CMakelists.txt配置文件简化了编译构建过程，可管理大型项目，且具有良好的扩展性
  - ROS对Cmake进行拓展，有了Catkin编译系统

> **从源代码到可执行程序的补充:**
> 1. 预处理
> 2. 编译
> 3. 汇编
> 4. 链接

> 更多内容见[详细过程](https://zhuanlan.zhihu.com/p/476697014)

- **基本语法**
  CMakeLists.txt 由一系列CMake指令组成，每个指令的格式为：**命令名(参数列表)**

> *个人批注：语法由于时间原因且有后面回看的条件，暂时将CMake基础语法复制下来，当然，我也会有个人批注在一些我不懂的语法底下*

  - **指定Cmake最低版本**
```bash
# 语法：cmake_minimum_required(VERSION <version>)
# 该指令必须放在 CMakeLists.txt 的最顶部
# 示例：要求 CMake 版本不低于 3.10
cmake_minimum_required(VERSION 3.10)
```
  - **定义项目名称和语言**
```bash
# 语法：project(<项目名> [<语言>...])
# 语言参数可选，常用值：CXX（C++）、C（C 语言）
# 调用 project() 后会自动设置 PROJECT_NAME 等变量
project(MyProject CXX)
```
  - **添加可执行文件**
```bash
# 语法：add_executable(<目标名> <源文件>...)
# 将指定的源文件编译生成一个可执行文件
add_executable(MyApp main.cpp utils.cpp)
```
  - **添加库文件**
```bash
# 语法：add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)
# STATIC：静态库（.a / .lib），编译时直接嵌入可执行文件
# SHARED：动态库（.so / .dll），运行时加载
# 不指定类型时，由 BUILD_SHARED_LIBS 变量决定
add_library(MyLib STATIC library.cpp)
```
> *这里的静态库和动态库区别是否与C++中的静态内存和动态内存有相通之处？*

  - **链接库到目标**
```bash
# 语法：target_link_libraries(<目标> <库>...)
# 将指定的库链接到目标（可执行文件或其他库）
# 可以链接自己项目中的库目标，也可以链接外部库
target_link_libraries(MyApp PRIVATE MyLib)
```
  - **设置变量**
```bash
# 语法：set(<变量名> <值>...)
# 定义一个普通变量，后续通过 ${变量名} 引用
set(CMAKE_CXX_STANDARD 17)

# 同时设置多个值（列表）
set(SOURCES main.cpp utils.cpp helper.cpp)
add_executable(MyApp ${SOURCES})
```
  - **为目标指定头文件路径**
```bash
# 语法：target_include_directories(<目标>
#           [BEFORE | AFTER]
#           [SYSTEM]
#           [PUBLIC | PRIVATE | INTERFACE]
#           <路径>...)
# PUBLIC: 当前目标和依赖它的目标都能使用该路径
# PRIVATE: 只有当前目标能使用
# INTERFACE: 只有依赖当前目标的其他目标能使用
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
```
  - **设置安装规则**
```bash
# 语法：install(TARGETS <目标>...
#           [RUNTIME DESTINATION <可执行文件安装路径>]
#           [LIBRARY DESTINATION <动态库安装路径>]
#           [ARCHIVE DESTINATION <静态库安装路径>]
#           [INCLUDES DESTINATION <头文件安装路径>])
# 定义执行 make install 时各类型文件的安装位置
install(TARGETS MyApp RUNTIME DESTINATION bin)
```
  - **条件语句**
```bash
# 语法：if(<条件>) ... elseif(<条件>) ... else() ... endif()
# CMake 支持的条件表达式包括：比较、逻辑运算、变量判断等
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug 模式下启用调试信息
    message(STATUS "当前为 Debug 构建模式")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
    # Release 模式下启用优化
    message(STATUS "当前为 Release 构建模式")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2") 
endif()
```
  - **自定义命令**
```bash
# 语法：add_custom_command(
#           TARGET <目标>
#           PRE_BUILD | PRE_LINK | POST_BUILD
#           COMMAND <命令> [参数...]
#           [COMMENT <说明>]
#           [VERBATIM])
# PRE_BUILD: 在编译前执行
# PRE_LINK: 在链接前执行
# POST_BUILD: 在构建完成后执行
add_custom_command(
    TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "构建完成！"
    COMMENT "打印构建完成信息"
)
```
  - **构建某个项目典型命令(重点)**
```bash
# 在项目根目录下创建 build 目录（推荐外部构建）
$ mkdir build && cd build

# 生成构建文件
$ cmake ..

# 编译项目
$ cmake --build .

# 安装到系统目录（可选）
$ cmake --install .
```

- **变量与缓存**
  - 类型：**普通变量**和**缓存变量**

  - **普通变量**
    - **作用域：**在CmakeLists.txt中定义，作用域为当前目录及其子目录
    - **生命周期：**单次Cmake运行时，每次Cmake运行时重新计算

  - **缓存变量**
    - **作用域：**存储在CMakeCache.txt文件中，多次Cmake运行中保持值不变
    - **生命周期：**全局持久缓存，多次CMake运行时值不变

> *注意：如果要修改CMakeLists.txt 中缓存变量的默认值，必须删除CMakeCache.txt 或重新运行 cmake，否则修改后的默认值无效*
> *补充：可通过命令行`-D`参数修改缓存变量
```bash
$ cmake .. -DMY_INSTALL_PATH=/opt/runoob -DENABLE_LOGGING=OFF
```

- **头文件搜索路径**
  - **方式：**`include_directories()`和`target_include_directories()`
  - **共性：**都可以为编译器添加头文件搜索路径
  - **区别：**作用范围和控制粒度

  - **作用范围：**无target的作用范围是全局作用域；有target的作用范围是指定目标
  - **控制粒度：**无target的影响范围为全局，易污染其他代码；有target的影响范围为指定目标，可通过PUBLIC、PRIVATE、INTERFACE精准控制

> *补充：public和private与cpp中的两者作用相像，而interface则是必须要将头文件路径告诉别人，别人才可`#include`它*

- **查找库和包**
  - **方式：**`find_package()`
  - **作用：**自动检测和配置外部库

  - 实例：
```bash
# 基本用法：查找 Boost 库（不限定版本）
# REQUIRED 表示如果找不到则中止 CMake 配置
find_package(Boost REQUIRED)

# 指定最低版本：要求 Boost 版本不低于 1.70
find_package(Boost 1.70 REQUIRED)

# 指定搜索路径：在自定义路径下查找 OpenCV
find_package(OpenCV REQUIRED PATHS /path/to/opencv)

# 推荐使用倒入目标方式，如`Boost::Boost`
```

> *Cmake及其相关补充资料的总结：语法部分其实和Cpp有相通之处，储存部分也在cpp部分有所了解，可以类推（大拇指）*
> 更多内容见[CMake基础](https://www.runoob.com/cmake/cmake-basic.html)



## **catkin相关内容**
- **一个车间：**`catkin_workspace`

- **三大核心：**src（源码空间）、build（编译空间）、devel（开发空间）
  - **src：**代码、机器人节点所在的文件夹，需要敲键盘的地方
  - **build：**中间文件、缓存文件所在的文件夹，我们用不着
  - **devel：**编译成功后最终生成的可执行程序和库文件的所在文件夹

- **Package功能包：**
  - **交代：**`src`中代码必须以Package的形式组织，标准包要有*CMakeLists.txt*和*package.xml*两个文件
    - **CMakeLists.txt：**相当于编译规则的说明书。依赖哪些库？编译哪些`.cpp`文件？可执行文件叫什么名？
    - **package.xml：**相当于功能包描述文件。写上包名、版本、作者、依赖项等基本信息

> *总结：我们以后写的代码，要放在src下，经过build编译后，生成文件会跑到devel里。同时Package包中的两个文件也让我开了眼界*

> 补充：实操内容见[Catkin Tutorial](https://wiki.ros.org/catkin/Tutorials)



## **大模型**
- **简介**
  - 类型：对话、Agent
    - 对话：仅能进行文本输出
    - Agent：外设工具，使得文本可转化为生产力

- **用途**
  - 帮助学习（我的Cpp练习、Markdown语法就是通过deepseek进行纠偏、科普的）
  - 帮助开发（在理解自身需求的原理的基础上进行约束性开发）

- **个人想法和观点**
  - 大模型再怎么强大，运用它的依旧是人，它也依旧是工具。应当是人运用工具，而不是让工具反过来奴役人。而人要运用大模型，最基本要求就是自身明白我要做什么、要做的内容的具体原理和过程，以及对使用此工具所带来的结果负起责任。因而，自主学习仍然是最基础、也是最重要的工作，毕竟，手里无剑和有剑不用是两回事
  - 对于要生产出来的文件，我们应进行阅读、组织和编辑工作，以此来引导ai来更好的生产，形成良性循环，详见[The Emperor Has No Clothes](https://www.mihaileric.com/The-Emperor-Has-No-Clothes)
