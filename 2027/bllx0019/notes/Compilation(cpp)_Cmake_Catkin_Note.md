# Compilation(cpp)_Cmake_Catkin_Note

## CPP编译 Part

### 编译流程：
预处理（Preprocessing）—> 编译（Compilation）—> 汇编（Assembly）—> 链接（Linking）

### 预处理（Preprocessing）：
#### 主要意义
实现头文件复用、宏配置、平台条件编译。形成“翻译单元”：每个 .cpp 加上其包含的头文件，成为一个独立编译单元。
#### 具体任务
    -删除所有#defin，处理#include预编译指令，将被包含的文件直接插入到预编译指令的位置。
    -处理所有的条件预编译指令
    -删除后续编译用不到的内容（如注释等），压缩体积便于后续处理
    -添加行号和文件标识，以便编译时产生调试用的行号及编译错误警告行号
    -预处理得到的另一个程序通常是以.i作为文件扩展名
    -不进行语法、类型检查，不做 C++ 语义分析

### 编译（Compilation）：
#### 主要意义
把预处理后的 C++ 源码转换为低层汇编表示，完成 C++ 语义检查、模板实例化、常量表达式求值和优化，完成类型安全检查与性能优化；每个编译单元独立编译，是分离编译的基础
#### 具体任务
    -进行词法分析，把字符流切分为 token
    - 进行语法分析，构造语法树，检查语句结构、括号、表达式等是否符合语法
    - 进行语义分析，包括类型检查、名字查找、重载解析、访问控制、类型转换、模板实例化、constexpr 求值等
    - 生成中间表示IR，如GCC的GIMPLE、LLVM IR
    - 进行优化，如常量折叠、死代码消除、内联、循环优化等
    - 生成目标平台汇编代码，通常以 .s 作为文件扩展名
    - 处理 C++ 特有信息，如名称修饰 name mangling、异常表信息、RTTI 等
    - 大部分编译错误在此阶段暴露，如语法错误、类型不匹配、未声明标识符、模板实例化错误、重载解析失败等
> [!TIP]
> 汇编语言是非常有用的，因为它为不同高级语言的不同编译器提供了通用的输出语言。

#### 汇编（Assembly）：
#### 主要意义
把汇编代码翻译成目标机器指令的二进制编码，生成可重定位目标文件，为链接阶段提供符号表、重定位表和节区信息；目标文件可独立保存、复用，并可打包成静态库
#### 具体任务
	-把汇编指令翻译成目标机器指令的二进制编码
	-生成可重定位目标文件，通常以 .o 或 .obj 作为文件扩展名，常见格式如 ELF、COFF/PE
	-在目标文件中建立代码段、数据段、BSS 段等节区
	-生成符号表，记录本编译单元定义和引用的符号
	-生成重定位表，标记哪些指令或数据中的地址需要在链接时修正
	-保留调试信息、异常处理信息等辅助信息
	-此时目标文件中的地址通常是相对地址或占位地址，跨文件符号尚未最终解析
	-汇编阶段一般不进行高级优化，主要是翻译汇编代码并生成目标文件结构

### 链接（Linking）：
#### 主要意义
解决跨编译单元的符号引用，把多个目标文件、静态库/动态库和启动文件组合成完整程序或共享库；实现模块化、库复用和分离编译，决定最终内存布局、动态依赖和程序入口
#### 具体任务
	-进行符号解析，把每个目标文件中的未定义符号与其他目标文件或库中的定义匹配
	-进行重定位，根据最终布局修正代码和数据中的地址引用
	-进行段合并与布局，合并各目标文件的 .text、.data、.bss、只读数据、异常表等
	-处理静态库，按需把被引用的目标文件复制进最终产物
	-处理动态库，记录依赖和动态符号，运行时由动态链接器加载解析
	-加入启动代码，如 crt1.o、crti.o、crtbegin.o 等，安排 main 调用、全局构造/析构和程序入口 _start
	-生成可执行文件或共享库，如 ELF 可执行文件、.so、.dll、.exe
	-可选进行链接时优化 LTO，但常规链接不负责 C++ 语义编译
	-常见链接错误在此阶段暴露，如 undefined reference、multiple definition、静态库链接顺序问题、ABI 不兼容等

## Cmake Part
### Why Make&MakeFile？Why Cmake？

#### SOMETHING ABOUT MAKE&MAKEDILE：
 - Makefile如同一份自动编译说明书。它告诉make 工具哪些文件要编译、编译顺序是什么、需要哪些命令。
 - Makefile实现了编译程序、构建程序的自动化提高了人们的效率。通过编写Makefile，在我们每次编写修改好代码后，只需要敲一次make命令便能够得到你想要的目标程序



#### SOMETHING ABOUT CMAKE：
 - CMake是对make工具的生成器，是更高层的工具，它简化了编译构建过程，**能够管理大型项目**，具有良好的扩展性。对于ROS这样大体量的平台来说，就采用的是CMake

 - CMake 的核心抽象是target（目标）：可执行文件、库都是目标；依赖、头文件路径、编译选项都应该“挂在目标上”（现代 CMake）

 - 我们使用cmake通过编写简单的配置文件去生成本地的Makefile，这个配置文件是独立于运行平台和编译器的，不管在什么平台用什么编译器，cmake都可以生成对应的makefile
> [!TIP]
>CMake不编译代码，它读取 `CMakeLists.txt`，为 Make、Ninja、Visual Studio 等生成构建文件，再调构建工具完成编译。



### Cmake与编译器的关系

|角色| 代表工具|职责|
|-------| ---------- | ----------------- | 
|编译器 |`gcc` / `g++`、`clang++`、`cl.exe`|把 `.cpp` 编译成 `.o`，把 `.o`链接成可执行文件或库|
|构建工具|`make`、`ninja`、`MSBuild`|记录“谁依赖谁”，只重编改过的文件，按依赖顺序调用编译器|
|构建系统生成器 |CMake|读取`CMakeLists.txt`，检查编译器/依赖，生成上面两种工具的输入文件|
> [!TIP]
>可以把Cmake的功能视作开发者写一次构建描述，生成各平台各工具的原生构建文件



### 构建流程

CMakeLists.txt
     │  configure（配置）：执行 CMake 语言、寻找找编译器&依赖、写缓存
     ▼
CMakeCache.txt + CMakeFiles/ + 各 target 的规则
     │  generate（生成）：把规则翻译成 Makefile / build.ninja / *.sln
     ▼
Makefile / build.ninja / *.vcxproj
     │  build（构建）：由 **make** / ninja / MSBuild 真正编译、链接
     ▼
可执行文件、静态库、动态库



### Cmake常用操作

|常用操作|对应功能|
|------|-------|
|`cmake -S <源码目录> -B <构建目录>`|完成configure + generate|
|`cmake --build <构建目录>`|等价于进入构建目录执行 `make`指令，并且跨生成器统一了写法|
> [!TIP]
>构建产物和中间文件都在 build 目录里，保证源码目录整洁易读。



### When you write CMake...

```cmake
# 注释用 #

# 命令(参数...)，参数用空格或分号分隔
set(SRCS main.cpp util.cpp)      # 列表：main.cpp;util.cpp

# 引用变量要加 ${}
add_executable(app ${SRCS})

# 字符串：含空格的参数必须加引号
set(GREET "hello world")

# 条件
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build")
endif()

# 循环
foreach(src ${SRCS})
    message(STATUS "source: ${src}")
endforeach()
```

| 规则         | 说明                                                         |
| ------------ | ------------------------------------------------------------ |
| 命令名大小写 | **不敏感**（`ADD_EXECUTABLE` 等价 `add_executable`），但约定全小写 |
| 变量名大小写 | **敏感**（`SRCS` 与 `srcs` 是两个变量）                      |
| 参数分隔     | 空格或分号；列表本质上就是分号分隔的字符串                   |
| 注释         | `#` 到行尾；`#[[ ... ]]` 为块注释                            |
| 缩进         | 无语法意义，统一 2 或 4 空格即可                             |


###  CMake的六条原则

1. 围着 target ：`add_executable` / `add_library` 之后，用 `target_*` 配置它。
2. 依赖用目标而非路径：`target_link_libraries(app PRIVATE other::lib)`，不要手工拼 `-L` / `-l`。
3. 头文件路径挂目标：用 `target_include_directories`，不用全局 `include_directories`。
4. 编译选项/宏挂目标：用 `target_compile_options` / `target_compile_definitions`。
5. 用 `target_compile_features` 声明 C++ 标准：让标准随依赖传播，而不是改全局 `CMAKE_CXX_FLAGS`。
6. 留接口：需要被 `find_package` 使用时就写 `install(TARGETS ... EXPORT ...)` + `install(EXPORT ...)`，并区分 `BUILD_INTERFACE` 与 `INSTALL_INTERFACE`。

### 常用内置变量
|变量|含义|
|----|----|
|`CMAKE_SOURCE_DIR`|顶层`CMakeLists.txt`所在目录（`project()` 不会改变它）|
|`PROJECT_SOURCE_DIR`|最近一次 `project()` 所在目录|
|`CMAKE_CURRENT_SOURCE_DIR`|当前处理的 `CMakeLists.txt` 所在目录（`add_subdirectory` 会改变它）|
|`CMAKE_BINARY_DIR` / `PROJECT_BINARY_DIR` / `CMAKE_CURRENT_BINARY_DIR`|对应的构建目录|
|`PROJECT_NAME`/`PROJECT_VERSION`|`project()` 定义的项目名/版本|
|`CMAKE_CXX_COMPILER_ID`|编译器标识，如 `GNU`、`Clang`、`MSVC`|
|`CMAKE_SYSTEM_NAME`|目标系统，如 `Linux`、`Windows`、`Darwin`|
|`CMAKE_BUILD_TYPE`|单配置生成器的构建类型（Debug/Release/...）|
|`CMAKE_INSTALL_PREFIX`|安装前缀，Unix 默认 `/usr/local`|
|`CMAKE_PREFIX_PATH`|`find_package` 搜索前缀列表|
|`BUILD_SHARED_LIBS`|为 `ON` 时 `add_library` 不写类型默认生成动态库|
|`CMAKE_EXPORT_COMPILE_COMMANDS`|为 `ON` 时导出 `compile_commands.jso`|

## catkin Part

### Somthing in catkin_workspace
```catkin
catkin_ws/
├── src/          # 源码空间：放各个 catkin 包；catkin_make 首次会生成 src/CMakeLists.txt（软链）
├── build/        # 构建空间：整个工作空间共用一个，中间文件 + CMakeCache
├── devel/        # 开发空间：构建产物 + setup.bash 等环境脚本（开发时用）
└── install/      # 安装空间：catkin_make install 后生成（发布时用，与 devel 二选一）
```
### catkin 与 CMake 的关系
- catkin 是 ROS 1 的构建系统，构建在 CMake 之上：它提供一系列 CMake 宏/模块（如 `catkin_package()`、`generate_messages()`），并规定包必须有 `package.xml` 和 `CMakeLists.txt`、每个包独立目录、不能嵌套。
- 一个 catkin 工作空间在构建时被当作一个大 CMake 工程（From ROS Wiki ：*“it will work as if it were one large CMake project”*），所有包共享一个 `build/` 空间。
- `catkin_make` 只是便捷封装：第一次运行时在 `src/` 下创建指向 catkin 顶层 `toplevel.cmake` 的 `CMakeLists.txt` 链接，然后在 `build/` 里执行 `cmake ../src` 和 `make`。

### catkin编译流程
```bash
catkin --help  ##检查catkin是否安装
mkdir catkin_ws 
cd /catkin_ws &mkdir src
cd src
catkin_init_workspace  ##初始化工作空间
cd ..
catkin build
```
### 创建一个package（`CreatingPackage`）
```bash
cd ~/catkin_ws/src/  # 创建test_pkg功能包，同时指明依赖std_msgs rospy roscpp
catkin_create_pkg test_pkg std_msgs rospy roscpp
```
### Somthing in package
```ROS
├── CMakeLists.txt      #package的编译规则(**必须**)
├── package.xml         #package的描述信息(**必须**)
├── src/                #源代码文件
├── include/            #C++头文件
├── scripts/            #可执行脚本
├── msg/                #自定义消息
├── srv/                #自定义服务
├── models/             #3D模型文件
├── urdf/               #urdf文件
└── launch/             #launch文件
```

>[!TIP] 
>注意 catkin 的 CMakeLists 风格并不是“现代 CMake”：它仍大量使用 `include_directories(${catkin_INCLUDE_DIRS})` 和 `${catkin_LIBRARIES}`，这是因为 catkin 的设计早于 target-based 现代写法。

###  `package.xml` 与 `CMakeLists.txt` 的对应关系

|`package.xml`标签| 对应 CMake 位置 | 说明 |
| --- | --- | --- |
|`<buildtool_depend>catkin</buildtool_depend>`|`find_package(catkin REQUIRED)`|构建工具依赖，每个包都有|
|`<build_depend>roscpp</build_depend>`|`find_package(catkin REQUIRED COMPONENTS roscpp)`|编译期需要|
|`<exec_depend>rospy</exec_depend>`|`catkin_package(CATKIN_DEPENDS rospy)`|运行期需要，并传给下游|
|`<depend>xxx</depend>`|同时提供 build + exec 依赖|format 2 的简写，多数情况可直接用|
|`<test_depend>gtest</test_depend>`|`catkin_add_gtest(...)`|测试需要|
|`<build_depend>message_generation</build_depend>`| `generate_messages(...)`|消息生成器|
|`<exec_depend>message_runtime</exec_depend>`|`catkin_package(CATKIN_DEPENDS message_runtime)`|消息运行时|

>[!CAUTION] 
>只改 `CMakeLists.txt` 的 `find_package` 而漏改 `package.xml`，本机能编译，但别人用 `rosdep` 安装依赖或发布包时会失败；反之只改 `package.xml` 漏改 CMake，则找不到头文件；两边要同步
