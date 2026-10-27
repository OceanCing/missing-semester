# 第二课笔记
## 1.程序详细编译过程
### 1.1 整体流程
``` #include<stdio>
int main()
{
    printf("hello, world\n");
    return 0;
}
```
> 为例：每条C语句都必须被其他程序转化为一系列的**低级机器语言指令**，然后这些指令按照一种为**可执行目标程序**的格式打好包，并以**二进制磁盘文件**的形式存放起来。目标程序也称为**可执行目标文件**。

- 由**编译器驱动程序**完成
`linux> gcc -o hello hello.c`
- 分为四个阶段：预处理（Preprocessing）、编译（Compilation）、汇编（Assembly）、链接（Linking）
#### 预处理阶段
- 1.预处理器（cpp）将所有的#define删除，并且展开所有的宏定义。
- 2.处理所有的条件预编译指令，比如#if、#ifdef、#elif、#else、#endif等。
- 3.处理#include预编译指令，将被包含的文件直接插入到预编译指令的位置。
- 4.删除所有的注释。
- 5.添加行号和文件标识，以便编译时产生调试用的行号及编译错误警告行号。
- 6.保留所有的#pragma编译器指令，因为编译器需要使用它们。
- 7.使用gcc -E hello.c -o hello.i命令来进行预处理， 预处理得到的另一个程序通常是以.i作为文件扩展名。
#### 编译阶段
- 编译器（ccl）将预处理完的文本文件hello.i进行一系列的词法分析、语法分析、语义分析和优化，翻译成文本文件hello.s，它包含一个汇编语言程序
-分为6步：**扫描（词法分析）、语法分析、语义分析、源代码优化、代码生成、目标代码优化**。
#### 汇编阶段
- 汇编器（as）将hello.s翻译成机器语言指令，把这些指令打包成一种叫做**可重定位目标程序**的格式，并将结果保存在目标文件hello.o中，hello.o是一个二进制文件。
#### 链接阶段
- hello程序调用了printf函数，它存在于一个名为printf.o的单独的预编译好了的目标文件中，而这个文件必须以某种方式合并到我们的hello.o程序中。连接器（ld）就负责处理这种合并。结果就得到了hello文件，它是一个**可执行目标文件**，可以被加载到内存中，由系统执行
## 2.CMake基础
> CMakeLists.txt 是 CMake 的核心配置文件，每个 CMake 项目至少需要一个 CMakeLists.txt 文件。

> CMake 通过读取文件中的指令来了解项目的结构、源文件列表和编译要求。
### 2.1 CMakeLists.txt文件
> CMakeLists.txt 由一系列 CMake 指令组成，每个指令的格式为：命令名(参数列表)。
#### **基本语法**
- 指定CMake最低版本
```# 语法：cmake_minimum_required(VERSION <version>)
# 该指令必须放在 CMakeLists.txt 的最顶部
# 示例：要求 CMake 版本不低于 3.10
cmake_minimum_required(VERSION 3.10)
```
- 定义项目名称和语言
```# 语法：project(<项目名> [<语言>...])
# 语言参数可选，常用值：CXX（C++）、C（C 语言）
# 调用 project() 后会自动设置 PROJECT_NAME 等变量
project(MyProject CXX)
```
- 添加可执行文件
``` # 语法：add_executable(<目标名> <源文件>...)
# 将指定的源文件编译生成一个可执行文件
add_executable(MyApp main.cpp utils.cpp)
```
- 添加库文件
```# 语法：add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)
# STATIC：静态库（.a / .lib），编译时直接嵌入可执行文件
# SHARED：动态库（.so / .dll），运行时加载
# 不指定类型时，由 BUILD_SHARED_LIBS 变量决定
add_library(MyLib STATIC library.cpp)
```
- 链接库到目标
```# 语法：target_link_libraries(<目标> <库>...)
# 将指定的库链接到目标（可执行文件或其他库）
# 可以链接自己项目中的库目标，也可以链接外部库
target_link_libraries(MyApp PRIVATE MyLib)
```
- 设置变量
```# 语法：set(<变量名> <值>...)
# 定义一个普通变量，后续通过 ${变量名} 引用
set(CMAKE_CXX_STANDARD 17)

# 同时设置多个值（列表）
set(SOURCES main.cpp utils.cpp helper.cpp)
add_executable(MyApp ${SOURCES})
```
- 为目标指定文件路径
```# 语法：target_include_directories(<目标>
#           [BEFORE | AFTER]
#           [SYSTEM]
#           [PUBLIC | PRIVATE | INTERFACE]
#           <路径>...)
# PUBLIC：当前目标和依赖它的目标都能使用该路径
# PRIVATE：只有当前目标能使用
# INTERFACE：只有依赖当前目标的其他目标能使用
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
```
- 设置安装规则
```# 语法：install(TARGETS <目标>...
#         [RUNTIME DESTINATION <可执行文件安装路径>]
#         [LIBRARY DESTINATION <动态库安装路径>]
#         [ARCHIVE DESTINATION <静态库安装路径>]
#         [INCLUDES DESTINATION <头文件安装路径>])
# 定义执行 make install 时各类型文件的安装位置
install(TARGETS MyApp RUNTIME DESTINATION bin)
```
-条件语句
```
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
- 自定义命令
```# 语法：add_custom_command(
#          TARGET <目标>
#          PRE_BUILD | PRE_LINK | POST_BUILD
#          COMMAND <命令> [参数...]
#          [COMMENT <说明>]
#          [VERBATIM])
# PRE_BUILD：在编译前执行
# PRE_LINK：在链接前执行
# POST_BUILD：在构建完成后执行
add_custom_command(
    TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "构建完成！"
    COMMENT "打印构建完成信息"
)
```
## 3.catkin
> ROS 1 的构建系统。把工作空间里所有包当成**一个大的 CMake 工程**一次编完，产物落进 devel/，source 一下就"装上"，不往 /usr 写东西。

### 3.1 三层空间
```
catkin_ws/                  # WORKSPACE
├── src/                    # 源码空间，所有包放这里
│   ├── CMakeLists.txt      # 顶层 CMake，首次 catkin_make 时在 src 里建的链接
│   └── package_a/
│       ├── CMakeLists.txt
│       └── package.xml
├── build/                  # 中间产物，一个工作空间只有一个
├── devel/                  # 开发用产物 + setup.*sh
└── install/                # 发布用产物 + setup.*sh
```
- 一个工作空间只有一个 build/，CMake 眼里整个 src 就是一个工程
- devel/ 只放生成物，源码靠**重定向**指回 src/，所以改 Python 不用重编
- 环境靠 `source devel/setup.bash` 叠加（PATH、PYTHONPATH、CMAKE_PREFIX_PATH）
- devel 和 install **不要同时 source**

### 3.2 最小工作流
```bash
source /opt/ros/noetic/setup.bash          # 底层环境
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws && catkin_make              # 首次会在 src 里生成顶层 CMakeLists.txt 链接
source devel/setup.bash                    # 每个新终端都要做一次
echo $ROS_PACKAGE_PATH                     # 自检，应能看到自己的 src

cd ~/catkin_ws/src
catkin_create_pkg beginner_tutorials std_msgs rospy roscpp   # 包名 + 依赖
cd ~/catkin_ws && catkin_make              # 回根部编译
rospack depends1 beginner_tutorials        # 看直接依赖
```
> Python 3：Noetic 默认就是 py3。只有 Melodic 及更早、从源码编译且要 py3 时，才需要首编加 `-DPYTHON_EXECUTABLE=/usr/bin/python3`，之后正常 catkin_make。

### 3.3 包的结构
> 成为 catkin 包必须满足三条：有 package.xml；有使用 catkin 的 CMakeLists.txt；**一个包一个目录**（不能嵌套、不能共用目录）。

#### package.xml
```xml
<package format="2">
  <name>beginner_tutorials</name>
  <version>0.1.0</version>
  <description>The beginner_tutorials package</description>
  <maintainer email="you@yourdomain.tld">Your Name</maintainer>   <!-- 必填 -->
  <license>BSD</license>                                          <!-- 必填 -->

  <buildtool_depend>catkin</buildtool_depend>   <!-- 构建工具，永远是 catkin -->
  <build_depend>roscpp</build_depend>           <!-- 编译期需要 -->
  <exec_depend>roscpp</exec_depend>             <!-- 运行期需要 -->
</package>
```
- 四个依赖标签：`buildtool_depend`（构建工具）、`build_depend`（编译期）、`exec_depend`（运行期）、`test_depend`（仅测试）
- catkin_create_pkg **只生成 build_depend**，运行时还要用的必须自己补 exec_depend，漏了就是"编译过、运行找不到"

#### CMakeLists.txt 四段式
```cmake
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs)   # 找依赖
catkin_package()                                                 # 导出本包给下游
add_executable(my_node src/my_node.cpp)                          # 目标
target_link_libraries(my_node ${catkin_LIBRARIES})               # 链接
```

### 3.4 catkin_make 常用
```bash
catkin_make                                # 在工作空间根部执行，增量编译
catkin_make -DCMAKE_BUILD_TYPE=Release     # 优化构建（另有 RelWithDebInfo、Debug）
catkin_make install                        # 等价于 cd build && make install
catkin_make --force-cmake                  # 新加了包却没被识别时强制重跑 cmake
catkin_make -DCMAKE_VERBOSE_MAKEFILE=ON    # 看实际调用的编译/链接命令
```
> **短板**：CMake 和 make 用的是调用时的同一个环境，所以 rostest 依赖刚生成的 Python 代码时会找不到路径。
> 绕法：catkin_make → source devel/setup.bash → catkin_make run_tests；或用 catkin_make_isolated。

#### 等价的手工步骤
```bash
mkdir -p ~/catkin_ws/build && cd ~/catkin_ws/build
cmake ../src -DCMAKE_INSTALL_PREFIX=../install -DCATKIN_DEVEL_PREFIX=../devel
make          # 产物进 devel
make install  # 产物进 install
```
- 何时要重跑 configure：增删包、改了某个包的构建文件、改构建系统设置
- CMAKE_INSTALL_PREFIX 默认是 `/usr/local`，而且 catkin **没有 uninstall 命令**，共享机器别用默认位置
- "卸载"就是删生成目录：`rm -rf build devel install`

### 3.5 overlay（叠加）
> 在已经装好的版本之上，用源码构建同名包，让你自己的版本"覆盖"掉它。
```bash
source /opt/ros/noetic/setup.bash          # 先有 underlay
mkdir -p ~/overlay_ws/src && cd ~/overlay_ws/src
git clone https://github.com/ros/ros_tutorials.git
cd ~/overlay_ws && catkin_make
source ~/overlay_ws/devel/setup.bash
roscd rospy_tutorials && pwd               # 从 /opt/ros/... 变成 ~/overlay_ws/src/...
```
#### 三个坑
- 只覆盖**同名**构建产物。上层删掉的头文件，下层那份仍然可用，不会被隐藏
- 上层依赖的包**不会重编**，用的还是下层产物 → 把依赖包也放进上层，或软链过去
- roscd 走**位置缓存**，新加的包要 `catkin_make` 之后再 `rospack profile` 才能找到

#### 链式工作空间
- 多个工作空间可以串起来，配置时输出会打印链条：`-- This workspace overlays: /home/<user>/overlay_ws/devel;/opt/ros/noetic`
- 配置时 underlay 必须处于已 source 状态；删了 build/ 重新配置就要重新 source

### 3.6 速查
- `catkin_create_pkg <包名> [依赖...]` 建包骨架
- `catkin_make` 在工作空间**根部**编译
- `rospack depends1 <包>` / `rospack depends <包>` 看直接 / 递归依赖
- `roscd <包>` 跳到包目录（走缓存）
- `rospack profile` 刷新 roscd、rospack 的查找缓存
> ROS 2 已改用 colcon + ament，catkin 只在维护 ROS 1 项目时用；Noetic 已于 2025-05-31 EOL。
