# 27赛季控制视觉招新培训第二课学习笔记：cmake与catkin,大模型使用入门

## 一.CMake基础
（笔记资料来自：https://www.runoob.com/cmake/cmake-basic.html）
总的来说，我们应该理解CMake 是一个跨平台的构建系统生成工具，用于管理 C/C++ 项目的编译过程。

### 1.CMakeLists.txt 

#### (1).基本概念

- **CMakeLists.txt 的本质**：它是 CMake 的核心配置文件，每个 CMake 项目至少需要一个。
  > **🐳 deepseek 解读：** CMakeLists.txt 是 CMake 这个工具的“总指挥部”。没有它，CMake 就不知道该干嘛。

- **严格的命名规则**：哪怕您的项目里只有一个 `.cpp` 文件，您也必须在同一个文件夹里建一个名为 `CMakeLists.txt` 的文件（注意大小写，错一个字母都不行），否则 CMake 直接罢工。

- **CMake 的工作原理**：CMake 通过读取文件中的指令，来了解项目的结构、源文件列表和编译要求。
  > **🐳 deepseek 解读：** CMake 是个“瞎子”，它不看您的代码，只看这个 txt 文件。您在 txt 里写了什么，它就干什么：
  > - **项目结构**：您写了 `project(MyRobot)`，它就知道这个工程叫啥。
  > - **源文件列表**：您写了 `add_executable(app main.cpp test.cpp)`，它就知道要把这两个文件编译在一起。


#### (2).基本语法

- **最低版本要求**(一般情况下，这必须放在最前面)
  - **作用**：指定运行此项目所需的 CMake 最低版本，必须置于文件首行。
  - **语法/示例**：`cmake_minimum_required(VERSION 3.10)`

- **定义项目名**
  - **作用**：定义项目的名称，并可选地指定使用的编程语言（如 CXX 表示 C++，C 表示 C 语言）。
  - **语法/示例**：`project(MyProject CXX)`

- **生成可执行文件**
  - **作用**：将指定的源文件（如 `.cpp` 文件）编译并链接，生成一个可执行文件。
  - **语法/示例**：`add_executable(MyApp main.cpp utils.cpp)`

- **生成库文件**
  - **作用**：将源文件编译成静态库（`.a`）或动态库（`.so`）。
  - **语法/示例**：`add_library(MyLib STATIC library.cpp)`
  - *(注：`STATIC` 为静态库，编译时直接嵌入；`SHARED` 为动态库，运行时加载)*

- **链接库到目标**
  - **作用**：将指定的库链接到您的可执行文件或其他目标中。可以链接自己项目内的库，也可以链接外部库。
  - **语法/示例**：`target_link_libraries(MyApp PRIVATE MyLib)`

- **设置变量**
  - **作用**：定义普通变量，后续通过 `${变量名}` 来引用它。也可以设置包含多个值的列表。
  - **语法/示例**：
    - 单值：`set(CMAKE_CXX_STANDARD 17)`
    - 列表：`set(SOURCES main.cpp utils.cpp)`，后续用 `add_executable(MyApp ${SOURCES})` 引用。

- **为目标指定头文件路径**
  - **作用**：告诉编译器去哪里找 `.h` 头文件。
  - **语法/示例**：`target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)`
  - *(注：`PRIVATE` 表示仅当前目标可用；`PUBLIC` 表示依赖它的目标也能用)*

- **设置安装规则**
  - **作用**：定义执行 `make install` 时，不同类型文件（执行文件、库、头文件）的安装位置。
  - **语法/示例**：`install(TARGETS MyApp RUNTIME DESTINATION bin)`

- **条件语句**
  - **作用**：根据条件执行不同的配置，比如判断是 Debug 还是 Release 模式。
  - **语法/示例**：
    `if(CMAKE_BUILD_TYPE STREQUAL "Debug")`
        `message(STATUS "当前为 Debug 模式")`
    `else()`
        `message(STATUS "当前为 Release 模式")`
    `endif()`

- **自定义命令**
  - **作用**：在构建过程的特定阶段（编译前、链接前、构建后）执行自定义的终端命令。
  - **语法/示例**：
    `add_custom_command(`
        `TARGET MyApp POST_BUILD`
        `COMMAND ${CMAKE_COMMAND} -E echo "构建完成!"`
        `COMMENT "打印构建完成信息")`
    
    **【核心注意点】构建一个项目的典型命令如下：**

  - **在项目根目录下创建 build 目录**
  `mkdir build && cd build`
  (这叫外部构建，建一个文件夹专门用来施工，就算搞砸了也可以直接删除文件夹)
  - **生成构建文件**
  `cmake ..`
  （这里的`..`是让CMake去上一级目录找配置文件，相当于让CMake帮忙生成一个底层的Makefile排班表 ）
  - **编译项目**
  `cmake --build .`
  （开始编译出可执行程序）
  - **安装到系统目录（可选）**
  `cmake --install .`
  （把编译好的可执行文件等复制到系统指定的目录）

#### (3).个人实战

##### 1.从零开始的打开hello_cmake
1. 在掌握了基本语法后，我先是在主文件夹新建一个cmake_demo的文件夹。
2. 然后在VScode里面打开此文件夹，并新建一个CMakeLists.txt文件，并输入以下代码：
```cmake
# 规定项目所需的最低 CMake 版本
cmake_minimum_required(VERSION 3.10)
# 定义项目名称
project(MyRobot)
# 编译并产生可执行程序
add_executable(my_app main.cpp)
```
3. 然后是在出现的main.cpp文件中输入:
```cpp
#include <iostream>
int main() {
    std::cout << "Hello CMake!" << std::endl;
    return 0;
}
```
4. 然后是新打开一个终端，在终端中依次输入：
```bash
# 1. 生成构建文件（指定源码目录为当前，构建目录为 build）
cmake -S . -B build

# 2. 执行编译（指定 build 目录）
cmake --build build

# 3. 运行生成的可执行文件
./build/my_app

**终端输出反馈**

-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/jiecheng/cmake_demo/build
[ 50%] Building CXX object CMakeFiles/my_app.dir/main.cpp.o
[100%] Linking CXX executable my_app
[100%] Built target my_app
Hello CMake!
```

##### 2.静态库的生成与链接检验
1. 在主文件夹新建一个cmake_advanced文件夹。
2. VScode中在此文件夹里面依次新建，

- **library.h**,头文件

- **library.cpp**,源文件

- **main.cpp**,主文件

- **CMakeLists.txt**,图纸
3. 在**library.h**中输入：
```
#pragma once
void print_library_message();
# 告诉别人：“我这里有一个叫 print_library_message 的函数”。主程序 main.cpp 只要 #include "library.h"，就知道这个函数是存在的
```
4. 在**library.cpp**中输入：
```cpp
#include <iostream>
#include "library.h"
void print_library_message() {
    std::cout << "This message is from the static library!" << std::endl;
}
//这是用来真正实现头文件里面所承诺的那个函数
```
5. 在**main.cpp**中输入：
```cpp
#include <iostream>
#include "library.h"
int main() {
    std::cout << "Hello from main app!" << std::endl;
    print_library_message();
    return 0;
}
//最终运行程序
```
6. 在 **CMakeLists.txt**中输入：
```cmake
# 规定项目所需的最低 CMake 版本
cmake_minimum_required(VERSION 3.10)
# 定义项目名称
project(AdvancedDemo)
# 告诉CMake，把library.cpp打包成一个名字叫MyLib的独立工具包
add_library(MyLib STATIC library.cpp)
# 把main.cpp编译成一个最终程序
add_executable(MyApp main.cpp)
# 告诉用户，头文件放在哪个文件夹里面
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR})
# 告诉CMake，最后要把静态库和主程序连接起来
target_link_libraries(MyApp PRIVATE MyLib)
```
7. 然后是新打开一个终端，在终端中依次输入：
```bash
# 1. 生成构建文件（指定源码目录为当前，构建目录为 build）
cmake -S . -B build

# 2. 执行编译（指定 build 目录）
cmake --build build

# 3. 运行生成的可执行文件
./build/MyApp

**终端输出反馈**
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/jiecheng/cmake_advanced/build
[ 25%] Building CXX object CMakeFiles/MyLib.dir/library.cpp.o
[ 50%] Linking CXX static library libMyLib.a
[ 50%] Built target MyLib
[ 75%] Building CXX object CMakeFiles/MyApp.dir/main.cpp.o
[100%] Linking CXX executable MyApp
[100%] Built target MyApp

./build/MyApp
Hello from main app!
This message is from the static library!
```

##### 3.一个综合实操
1. 在主文件夹新建cmake_simple_test文件夹

2. VScode中在此文件夹里面依次新建，

- **utils.cpp**

- **core.cpp**

- **helper.cpp**

- **main.cpp**

- **CMakeLists.txt**

3. 在core.cpp中填入：
```cpp
#include <iostream>
void core_function() { std::cout << "Core function called." << std::endl; }
```

4. 在helper.cpp中填入：
```cpp
#include <iostream>
void helper_function() { std::cout << "Helper function called." << std::endl; }
```

5. 在utils.cpp中填入：
```cpp
#include <iostream>
void utils_function() { std::cout << "Utils function called." << std::endl; }
```
6. 在main.cpp中写入：
```cpp
#include <iostream>
void core_function();
void helper_function();
void utils_function();

int main() {
    std::cout << "Main app started." << std::endl;
    core_function();
    helper_function();
    utils_function();
    return 0;
}
```

7.在CMakeLists.txt中写入：
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 生成静态库，使用当前目录下的源文件
add_library(MyLib STATIC core.cpp helper.cpp)

# 生成可执行文件
add_executable(MyApp main.cpp utils.cpp)

# 因为大家平级，头文件路径就是当前目录
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(MyApp PRIVATE MyLib)

# 安装规则
install(TARGETS MyApp RUNTIME DESTINATION bin)
install(TARGETS MyLib ARCHIVE DESTINATION lib)

```

8. 在终端运行后会得到：
```bash
Main app started.
Core function called.
Helper function called.
Utils function called.
jiecheng@jiecheng-Legion-Y
```

### 2.变量与缓存
CMake 使用变量来存储和传递配置信息，变量可以在 CMakeLists.txt 中定义和使用。

#### （1）.普通变量
普通变量在 CMakeLists.txt 中定义，作用域为当前目录及其子目录。
普通变量在每次 CMake 运行时重新计算，不会持久化。

**deepseek解读：**
1. 普通变量在 CMakeLists.txt 中定义
   👉 必须用 set() 明说。你不敲代码创建它，它就不存在。
2. 作用域为当前目录及其子目录
   👉 只能向下管，不能向上管。父目录定义的变量，子目录能用；子目录定义的，父目录看不见。
3. 运行时重新计算，不会持久化
   👉 每次运行都失忆。敲 cmake 时从头算一遍，上一次的结果不会存下来。所以改了配置，必须重新跑 cmake 命令才能生效。

**使用实例**
#####  定义一个字符串变量（👉 必须用 set(变量名 值) 来创建。就像往盒子里装东西，不装就是空的。）
`set(MY_APP_NAME "runoob-app")`

##### 定义一个列表变量（用分号或空格分隔👉 多个值用空格隔开就行（比如 main.cpp utils.cpp），CMake 会自动把它当成一个列表（List））
`set(SOURCE_FILES main.cpp utils.cpp helper.cpp)`

##### 使用变量：通过 ${变量名} 引用（👉 必须加 ${} 符号。比如想用名字，得写 ${MY_APP_NAME}，直接写名字系统是不认识的。）
`message(STATUS "项目名称: ${MY_APP_NAME}")`

##### 使用列表变量：循环遍历每个源文件）（  👉 这是一个非常好用的语法。它能把列表里的文件（SOURCE_FILES）挨个拿出来，装进 FILE 这个临时变量里，方便批量处理。）
`foreach(FILE ${SOURCE_FILES})`
    `message(STATUS "源文件: ${FILE}")`
`endforeach()`

**比如**：把前文中**3.一个综合实操**中CMakeLists.txt的内容改成：
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

# 1. 定义一个字符串变量（相当于给项目起了个内部小名）
set(MY_APP_NAME "super-app")

# 2. 定义一个列表变量（把咱们那几个源文件打包进一个盒子里）
set(SOURCE_FILES core.cpp helper.cpp utils.cpp)

# 3. 生成可执行文件（直接引用变量！）
# 注意：这里我们不用再一个个敲文件名，而是直接写 ${SOURCE_FILES}
add_executable(${MY_APP_NAME} main.cpp ${SOURCE_FILES})

# 4. 打印信息，验证变量是否生效
message(STATUS "=====================================")
message(STATUS "项目实际生成的名字是: ${MY_APP_NAME}")
message(STATUS "我打包的源文件有: ${SOURCE_FILES}")

# 5. 用 foreach 循环，把盒子里的文件一个个拿出来打印
foreach(FILE ${SOURCE_FILES})
    message(STATUS "正在准备编译文件: ${FILE}")
endforeach()
```
终端运行后可以得到：
```bash
-- =====================================
-- 项目实际生成的名字是: super-app
-- 我打包的源文件有: core.cpp;helper.cpp;utils.cpp
-- 正在准备编译文件: core.cpp
-- 正在准备编译文件: helper.cpp
-- 正在准备编译文件: utils.cpp
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/jiecheng/cmake_simple_test/build
```
**🐳 deepseek 解读：**

1. 从“写死”到“灵活”（解决了牵一发而动全身的问题）

· 之前（没有变量）：您在 add_executable(MyApp main.cpp core.cpp helper.cpp) 里，把文件名一个个敲死了。如果有一天，您的代码多了一个 sensor.cpp，您得手动去括号里加上它。如果项目里有 50 个文件，改起来简直是灾难。
· 现在（使用变量）：您把文件装进了 SOURCE_FILES 这个“大盒子”里。以后加文件，只要在 set 那一行往里扔就行。add_executable 那句代码永远不需要改。这就叫解耦。

2. 彻底避免了“拼写错误”

· 之前：项目名 MyApp 您可能在第 10 行敲对了，在第 50 行写成了 MyAPP，系统就会报错找不到目标。
· 现在：只要定义一次 MY_APP_NAME，后面全用 ${MY_APP_NAME} 引用。哪怕您闭着眼睛复制粘贴，也绝不会拼错。

3. foreach 循环让代码“活”了起来

· 截图里的 foreach 循环，是 C++ 程序员在 CMake 里最爱用的神器。它意味着您不仅能存变量，还能批量处理它们。比如以后您想把所有文件都加上特定的编译选项，用 foreach 遍历一遍变量就行，省去了大量机械劳动。

#### (2).缓存变量
缓存变量存储在 CMakeCache.txt 文件中，在多次 CMake 运行之间保持值不变。
缓存变量通常用于让用户在 CMake 配置阶段自定义构建设置，例如开关功能或指定安装路径。

**deepseek解读：**
1. 它能“记住”自己（持久化）
   👉 普通变量每次运行就失忆；但缓存变量会被保存在 CMakeCache.txt 文件里，下次编译还在。
2. 它是给“用户”用的开关
   👉 比如写了个软件，用缓存变量提供“要不要开启某项功能”或“装在哪个盘”的选项。用户在外面敲命令就能配置，不用改代码。

💡 一句话总结：
普通变量是“程序内部打杂的”，缓存变量是“给用户留的控制面板”。

**使用实例**
 ##### 定义一个字符串类型的缓存变量（用户可修改的安装路径）（👉 `CACHE PATH` 让用户在配置时能修改安装目录，`/usr/local` 是默认值。）
`set(MY_INSTALL_PATH "/usr/local" CACHE PATH "程序的安装路径")`

##### 定义一个布尔类型的缓存变量（功能开关）（👉 `CACHE BOOL` 用于定义开关选项，`ON` 代表默认开启，用户可以在终端随时关掉它。）
`set(ENABLE_LOGGING ON CACHE BOOL "是否启用日志功能")`

##### 使用缓存变量和其他变量一样（👉 同样用 `${}` 引用。如果开关是 `ON`，就向编译器添加宏定义 `-DENABLE_LOGGING`。）
`if(ENABLE_LOGGING)`
`    message(STATUS "日志功能已启用")`
`    add_definitions(-DENABLE_LOGGING)`
`endif()`

**【注意】**
 - 用户可以通过命令行 -D 参数修改缓存变量：
   在 cmake 命令中通过 -D 修改缓存变量
   > $ cmake .. -DMY_INSTALL_PATH=/opt/runoob -DENABLE_LOGGING=OFF

**比如**还是把前文中的**3.一个综合实操**中CMakeLists.txt的内容改成：
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==========================================
# 第一部分：普通变量验证
# ==========================================
set(MY_APP_NAME "super-app")
set(SOURCE_FILES core.cpp helper.cpp utils.cpp)

# ==========================================
# 第二部分：缓存变量验证
# ==========================================
# CACHE BOOL 表示这是一个布尔值开关，ON 是默认值
set(ENABLE_DEBUG_MODE ON CACHE BOOL "是否开启调试模式")

# 使用 if 条件语句判断缓存变量
if(ENABLE_DEBUG_MODE)
    message(STATUS "【配置提示】当前为 Debug 调试模式，正在开启优化...")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
    message(STATUS "【配置提示】当前为 Release 模式，性能拉满。")
endif()

# ==========================================
# 第三部分：生成目标与安装规则
# ==========================================
add_library(MyLib STATIC core.cpp helper.cpp)
add_executable(${MY_APP_NAME} main.cpp utils.cpp)

target_include_directories(${MY_APP_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(${MY_APP_NAME} PRIVATE MyLib)

# 打印普通变量，验证 foreach 循环
message(STATUS "----- 变量解析结果 -----")
message(STATUS "项目名称: ${MY_APP_NAME}")
foreach(FILE ${SOURCE_FILES})
    message(STATUS "需要编译的源文件: ${FILE}")
endforeach()

# 安装规则：设置安装路径
install(TARGETS ${MY_APP_NAME} RUNTIME DESTINATION bin)
install(TARGETS MyLib ARCHIVE DESTINATION lib)
```
终端运行后可以得到：
```bash
jiecheng@jiecheng-Legion-Y7000-IRX10:~/cmake_simple_test$ cmake -S .-B build
-- 【配置提示】当前为 Debug 调试模式，正在开启优化...
-- ----- 变量解析结果 -----
-- 项目名称: super-app
-- 需要编译的源文件: core.cpp
-- 需要编译的源文件: helper.cpp
-- 需要编译的源文件: utils.cpp
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/jiecheng/cmake_simple_test/build
jiecheng@jiecheng-Legion-Y7000-IRX10:~/cmake_simple_test$ cmake -S . -B build -DENABLE_DEBUG_MODE=OFF
-- 【配置提示】当前为 Release 模式，性能拉满。
-- ----- 变量解析结果 -----
-- 项目名称: super-app
-- 需要编译的源文件: core.cpp
-- 需要编译的源文件: helper.cpp
-- 需要编译的源文件: utils.cpp
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/jiecheng/cmake_simple_test/build
jiecheng@jiecheng-Legion-Y7000-IRX10:~/cmake_simple_test$ cmake --build build
[ 16%] Building CXX object CMakeFiles/MyLib.dir/core.cpp.o
[ 33%] Building CXX object CMakeFiles/MyLib.dir/helper.cpp.o
[ 50%] Linking CXX static library libMyLib.a
[ 50%] Built target MyLib
[ 66%] Building CXX object CMakeFiles/super-app.dir/main.cpp.o
[ 83%] Building CXX object CMakeFiles/super-app.dir/utils.cpp.o
[100%] Linking CXX executable super-app
[100%] Built target super-app
```
**deepseek解读：**
这次实操的核心成果，可以总结为以下四点，直接切中要害：

1. 成功分离了“内存数据”与“磁盘数据”

· 普通变量（MY_APP_NAME）存在内存里，用完就清空。它的价值在于一次定义，多处引用，最终将可执行文件名成功变成了 super-app。
· 缓存变量（ENABLE_DEBUG_MODE）被写入了 CMakeCache.txt 磁盘文件中，具有持久化特性。

2. 验证了 if 条件判断的“配置分流”作用

· 通过读取缓存变量的值，CMake 自动选择了不同的构建策略（Debug 或 Release），并向编译器传递了不同的编译标志（如 -g）。

3. 验证了缓存变量的“外部控制权”

· 第二次在终端输入 -DENABLE_DEBUG_MODE=OFF 时，没有修改一行代码，直接覆盖了配置文件中的默认值。这证明了用户可以通过命令行参数，直接改变构建配置。

4. 跑通了完整的“生成-编译-链接-安装”流水线

· 通过 foreach 循环遍历了源文件；通过 add_library 生成了 libMyLib.a；通过 target_link_libraries 完成了链接；并最终生成了名为 super-app 的可执行文件。


### 3.头文件搜索路径
在 CMake 中，有两种方式可以指定编译器查找头文件的路径：include_directories() 和 target_include_directories()。两者都可以为编译器添加头文件搜索路径，但作用范围和控制粒度不同。

**deepseek解读：**
在过去，我们常用 `include_directories()`，但现代 CMake 强烈推荐使用 `target_include_directories()`。

#### 1.两种头文件搜索路径的对比
- **(1).作用范围**
  - `include_directories()`：全局作用域，会影响当前目录及子目录中的**所有目标**。
  - `target_include_directories()`：仅作用于**指定的目标**，不会污染其他目标。

- **(2).推荐程度**
  - `include_directories()`：**不推荐**，除非在维护旧版 CMake 项目。
  - `target_include_directories()`：**强烈推荐**，符合现代 CMake 最佳实践。

- **(3).目标关联性**
  - `include_directories()`：不直接关联到特定目标，可能意外影响其他目标。
  - `target_include_directories()`：显式绑定到指定目标，依赖关系一目了然。

- **(4).可维护性**
  - `include_directories()`：较差，容易导致全局路径污染，问题难以排查。
  - `target_include_directories()`：较好，路径与目标绑定，逻辑清晰。

- **(5).可见性控制**
  - `include_directories()`：无法精确控制传播范围。
  - `target_include_directories()`：通过 `PUBLIC`、`PRIVATE`、`INTERFACE` 精确控制。


#### 2.可见性关键字的含义 (`PUBLIC` / `PRIVATE` / `INTERFACE`)
  注：`target_include_directories()` 的最大优势，就是能精确控制头文件路径的“传播”（谁能用，谁不能用）。

| 关键字 | 当前目标可用 | 依赖此目标的其他目标可用 | 典型场景 |
| :--- | :--- | :--- | :--- |
| **`PRIVATE` (私有)** | 是 | 否 | 仅自身实现需要的内部头文件（别人不需要知道）。 |
| **`PUBLIC` (公开)** | 是 | 是 | 库的公开 API 头文件。不仅自己要用，别人调用这个库的时候也必须包含它。 |
| **`INTERFACE` (接口)** | 否 | 是 | 仅头文件的库 (Header-only library)。自己不需要编译，纯给消费者提供头文件。 |

  **🐳 deepseek 解读：**
> 以前用 `include_directories()` 就像在大街上拿喇叭广播（大家都听得到，容易乱）；

现在的 `target_include_directories()` 就像给指定的人发微信，并设置了“仅自己可见”、“公开”或“仅对方可见”。

#### 3.实操
1. 主文件夹中新建文件夹

2. 新建lib.h公共库头文件文件，并输入：
```cpp
#pragma once
void print_public();
```

3. 新建lib.cpp公共库源文件，并输入：
```cpp
 #include "lib.h"
#include <iostream>
void print_public() {std::cout<<"PUBLIC 库被调用了"<<std::endl;}
```

4. 新建main.cpp主程序：
```cpp
#include <iostream>
#include "lib.h"
int main(){
    print_public();
    return 0;
}
```

CMakeLists.txt中输入：
```cmake
cmake_minimum_required(VERSION 3.10)
project(Test)
add_library(MyLib lib.cpp)
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR})
add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE MyLib)
```
终端运行后可以得到：
```bash
jiecheng@jiecheng-Legion-Y7000-IRX10:~/cmake_test$ cmake -S . -B build
cmake --build build
./build/MyApp
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/jiecheng/cmake_test/build
[ 50%] Built target MyLib
[ 75%] Building CXX object CMakeFiles/MyApp.dir/main.cpp.o
[100%] Linking CXX executable MyApp
[100%] Built target MyApp
PUBLIC 库被调用了
```
通过这里我们就可以看到，`target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR})`这行指令能让MyLib库中的源文件能成功找到头文件，并自动把这个路径“传染”给了链接它的主程序 MyApp，因此，main.cpp 中无需额外配置路径。运行后便得到**PUBLIC 库被调用了**这一结果，而这一结果我们是在lib.cpp公共库源文件里面写好了的。

### 4.查找库和包
很多项目依赖外部库（如 Boost、OpenCV），CMake 提供了 find_package() 指令来自动检测和配置这些外部依赖。

#### (1).find_package指令
它会搜索系统中已安装的库，并设置对应的变量（如头文件路径、库文件路径、库目标名称）。

**实例**
 1. 基本用法：查找 Boost 库（不限定版本）
 
 REQUIRED 表示如果找不到，则立即终止 CMake 配置并报错

`find_package(Boost REQUIRED)`

 2. 指定最低版本：要求 Boost 版本不低于 1.70

`find_package(Boost 1.70 REQUIRED)`

 3. 指定搜索路径：在自定义路径下查找 OpenCV

`find_package(OpenCV REQUIRED PATHS /path/to/opencv)`

#### (2).使用查找到的库
find_package() 成功后，通常会提供以下内容供使用：
- **导入目标（现代推荐）**
  - **示例**：`Boost::Boost`
  - **用法**：直接通过 `target_link_libraries` 使用。
  - **说明**：这是现代 CMake 的推荐方式，它会自动处理所有编译和链接选项，大幅减少配置错误。

- **头文件路径变量（旧式）**
  - **示例**：`${Boost_INCLUDE_DIRS}`
  - **用法**：配合 `include_directories()` 使用。
  - **说明**：这是旧式用法，需要手动处理头文件路径。

- **库文件路径变量（旧式）**
  - **示例**：`${Boost_LIBRARY_DIRS}`
  - **用法**：配合 `link_directories()` 使用。
  - **说明**：这也是旧式用法，需要手动处理库文件路径。

> 🐳 **deepseek批注：**
> **强烈推荐使用“导入目标”方式（如 `Boost::Boost`）！** 千万不要再去手动拼接头文件和库路径了。

>   导入目标会把需要的一切都打包好，直接丢给 `target_link_libraries`，优雅且不会出错。


## 2.程序详细编译过程（预处理、编译、汇编、链接）
（笔记资料来自：https://zhuanlan.zhihu.com/p/476697014）

### 1.程序编译的整体流程

**（1）. 文本文件 vs 二进制文件**
- 程序员编写的源程序（如 `hello.c`）本质上是一个由 `0` 和 `1` 组成的位序列。8个位组成一个字节（Byte），每个字节代表一个字符。
- 像 `hello.c` 这样只由 ASCII 字符构成的称为**文本文件**；其他所有文件称为**二进制文件**。

**（2）. 编译系统的作用**
- 计算机无法直接运行 C 语言文本，必须将其转化为一系列低级机器语言指令，并打包成**可执行目标文件**（二进制磁盘文件）。
- 在 Unix/Linux 系统中，这个转化是由**编译器驱动程序**（如 GCC）完成的。

**(3) . GCC编译驱动流程**
GCC 编译器驱动程序读取源程序 `hello.c`，将其翻译为可执行目标文件 `hello`。整个过程分为四个阶段，对应不同的处理程序和文件后缀：

```text
[ hello.c ] (源程序/文本)
    │
    ▼
+-------------------+
|  预处理器 (cpp)   |  ──> 生成 [ hello.i ] (修改后的源程序/文本)
+-------------------+
    │
    ▼
+-------------------+
|   编译器 (ccl)    |  ──> 生成 [ hello.s ] (汇编程序/文本)
+-------------------+
    │
    ▼
+-------------------+
|   汇编器 (as)     |  ──> 生成 [ hello.o ] (可重定位目标程序/二进制)
+-------------------+
    │
    ▼
+-------------------+
|   链接器 (ld)     |  <── 合并外部库 (如 printf.o)
+-------------------+
    │
    ▼
[ hello ] (可执行目标程序/二进制)
```

### 2.预处理
预处理器（cpp）在正式编译前，主要完成以下“清理与组装”工作：

1. **宏替换**：将所有的 `#define` 删除，并且展开所有的宏定义。
2. **条件编译**：处理所有的条件预编译指令（如 `#if`、`#ifdef`、`#elif`、`#else`、`#endif` 等）。
3. **头文件展开**：处理 `#include` 预编译指令，将被包含的文件直接插入到该指令的位置。
4. **删除注释**：把代码里所有的注释（`//` 和 `/* */`）清理干净。
5. **添加标识**：添加行号和文件标识，以便编译时产生调试用的行号及编译错误警告行号。
6. **保留特定指令**：保留所有的 `#pragma` 编译器指令，因为后续编译器需要使用它们。

> **deepseek提供的实操命令：**
> 使用 `gcc -E hello.c -o hello.i` 命令来进行预处理。
> 预处理得到的文件通常以 `.i` 作为扩展名（比如 `hello.i`）。

### 3.编译阶段
编译过程可分为6步：扫描（词法分析）、语法分析、语义分析、源代码优化、代码生成、目标代码优化。

1. **词法分析**：扫描器（Scanner）将源代的字符序列分割成一系列的记号（Token）。lex工具可实现词法扫描。
2. **语法分析**：语法分析器将记号（Token）产生语法树（Syntax Tree）。yacc工具可实现语法分析。
3. **语义分析**：静态语义（在编译器可以确定的语义）、动态语义（只能在运行期才能确定的语义）。
4. **源代码优化**：将语法树转化为**与目标机器无关**的中间代码。这使得编译器分为**前端**（负责产生中间代码）与**后端**（负责将其转化为目标机器码）。
5. **目标代码生成**：代码生成器(Code Generator).
6. **目标代码优化**：目标代码优化器(Target Code Optimizer)。

### 4.汇编阶段
汇编器（as）将hello.s翻译成机器语言指令，把这些指令打包成一种叫做可重定位目标程序的格式，并将结果保存在目标文件hello.o中，hello.o是一个二进制文件。

### 5.链接阶段：
`hello` 程序调用了 `printf` 函数，但这个函数并不在您的代码里，而是存在于一个预先编译好的独立目标文件 `printf.o` 中。链接器（`ld`）负责将这些零散的文件合并起来。

- **核心工作**：把 `printf.o` 合并到我们的 `hello.o` 程序中。
- **最终产物**：生成 `hello` 文件（即可执行目标文件）。
- **执行方式**：该文件可以被加载到内存中，由系统直接执行。

**结合deepseek的理解**
> 如果把编译比作造车，`hello.o` 就像是自己造好的车身，但发现缺个方向盘。`printf.o` 就是那个现成的方向盘。
> **链接器（ld）** 就是总装车间的机械臂，它把方向盘（`printf.o`）安到车身上（`hello.o`），最终拼成了一辆能直接开上路的完整汽车（`hello` 可执行文件）。
> 链接器不仅要拼装您自己的零件，还要去系统库里找齐所有依赖的零件（库文件），最后打包生成可执行程序。

### 6.链接：
1. **链接（linking）**是将各种代码和数据片段收集并组合成为一个单一文件的过程，这个文件可被加载（复制）到内存并执行。

2. 链接是由叫**链接器（linker）**的程序自动执行的。

3. 链接使得**分离编译（separate compilation）**成为可能：
- **化整为零**：不用把大型程序写成一个巨无霸文件，而是拆分成多个便于管理的小模块，独立编译。
- **增量更新**：修改某个模块后，**只需重新编译该模块并重新链接**，无需把其他文件全部重编一遍，极大提升了开发效率。

### 7.虚拟内存的三大核心能力

- **1. 高效利用物理内存（按需加载）**
  - 我的理解是，系统把主存当成磁盘的高速缓存。只把当前正在用的部分装进内存，没用的丢在磁盘（交换空间）里，需要时再换进来。这让小内存也能跑大程序。

- **2. 统一地址空间（简化管理）**
  - 我想，给每个进程提供一套统一的“虚拟地址”。程序员不需要去管数据到底存在哪块物理内存上，这样一来找内存变得极其简单。

- **3. 进程安全隔离（互相保护）**
  - 也就是操作系统给每个进程分配**独立**的虚拟地址空间。哪怕 A 程序崩溃了，也绝对破坏不到 B 程序的数据，大家互不干扰。

### 8.静态链接与 ELF 文件格式

静态链接器（如 Linux 的 `ld`）把一组可重定位目标文件（`.o`）和命令行参数作为输入，拼装、合并成最终的可执行文件。这一切都依赖于 ELF（可执行可链接格式）文件的结构。

**ELF 文件的内部结构（重点速查）**
一个典型的 ELF 目标文件由 ELF 头、节头部表以及各种“节（Section）”组成。最核心的节如下：
- **`.text`（代码区）**：存放已编译程序的机器代码。
- **`.rodata`（只读数据区）**：存放只读数据，比如 `printf` 里的格式串。
- **`.data`（数据区）**：存放**已初始化**的全局和静态 C 变量。
- **`.bss`（未初始化数据区）**：存放**未初始化**或初始化为 0 的全局/静态变量。
- **`.symtab`（符号表）**：记录程序中定义和引用的函数、全局变量信息。
- **`.rel.text` / `.rel.data`（重定位节）**：记录那些“地址还没确定，链接时需要修改”的位置（比如调用了外部函数）。
- **`.debug` / `.line`（调试信息）**：给 GDB 调试用的，记录了变量类型和代码行号的映射。

> ** deepseek的大白话比喻：**
> 如果把 `.o` 目标文件看作一个“行李箱”：
> `.text` 是放衣服的（代码）、`.data` 是装洗漱用品（已经装好的变量）、`.symtab` 是行李箱里的物品清单（符号表）。
> 至于 `.bss`，它其实是**“占位符”**！因为它里面装的都是未初始化的变量（全是 0），为了节省空间，它不占行李箱的实际重量，到了内存里才真正分配空间。

### 9.静态链接过程：
静态链接器（`ld`）把多个目标文件（`.o`）合并成一个可执行文件，主要经历三个阶段：**空间分配、符号解析、重定位**。

#### (1). 空间与地址分配（合并拼图）
链接器扫描所有输入文件，计算各个段的长度，并将它们合并。

> ** deepseek：**
> 链接器就像一个总包工头，先量一下所有的砖头（`.text`）有多少，所有的水泥（`.data`）有多少，然后把它们整合在一起，规划好在最终大楼（可执行文件）里，每部分该放在哪个位置。
> **疑惑解答**：链接时程序还没运行，哪来的地址？其实是通过虚拟存储器，给每个段提前分好虚拟地址（链接前都是 0，链接后各自有了归属）。

#### (2). 符号解析（找亲戚）
每一个 `.o` 文件里都有一个符号表（`.symtab`），记录了它定义了什么函数/变量，又引用了别人的什么函数/变量。

- **符号分类**：全局符号（自己造，别人也能用）、外部符号（别人造，自己要用）、局部符号（带 `static`，只给自己用）。
- **多重定义规则（强/弱符号）**：函数和已初始化的全局变量是**强符号**；未初始化的全局变量是**弱符号**。
  - 规则一：不能有两个同名的强符号（直接报错）。
  - 规则二：一强多弱，选强。
  - 规则三：多个弱符号，随便挑一个。

> **deepseek：**
> 符号解析就是**“全球认亲大会”**。如果 A 文件调用了 `printf`，链接器就要去所有的文件里找“谁定义了 `printf`”。
> 如果没人定义，直接报错（找不到亲戚）；如果有好几个同名大佬（多个强符号），直接打架报错；如果只有一个大佬带几个小弟（一强多弱），那就听大佬的。

#### (3). 重定位（贴标签 + 修正地址）
一旦符号都找到了，链接器就开始**分配真实的运行内存地址**，并修改代码里的引用。

- **重定位节**：把相同类型的节合并（比如所有的 `.data` 合并成一个大 `.data`），并赋予真实的虚拟内存地址。
- **重定位符号引用**：修改代码里的调用指令，让它们跳转到正确的地址。

> **deepseek：**
> 认完亲戚后，大家得坐到一个教室里（合并节）。链接器给每个人分配了一个具体的座位号（分配运行时地址），然后拿着小本本，把每个人“找亲戚”的纸条上原本空着的地址，填上那个亲戚现在的真实座位号。这样程序跑起来，一找一个准。

#### 10.动态链接与共享库 (Dynamic Linking)

既然有了静态链接，为什么还要搞动态链接？主要是为了解决静态库的两个致命缺陷：

- **痛点 1：更新极其困难**。静态库更新后，程序员必须重新找到新库，并重新链接自己的程序。
- **痛点 2：空间极大浪费**。如果两个程序用了同一个静态库模块，那么磁盘和内存里都会保存两份一模一样的副本。

为了解决这些问题，**共享库（.so 文件）**和**动态链接**诞生了：

- **机制**：程序在运行或加载时，才由“动态链接器”把共享库加载到内存中，并链接起来。
- **优势 1（省磁盘）**：整个系统里，一个库只有一个 `.so` 文件，所有程序共享调用，不复制。
- **优势 2（省内存）**：当多个进程运行时，内存里只需要保留一份共享库的 `.text`（代码段）副本，大家共用。

> **deepseek：**
> **静态链接**就像每个人买电脑，都得自己买一份正版 Windows 系统装硬盘里（占空间，系统更新了还得各自去重装）。
> **动态链接**就像公司的云桌面，大家电脑里都没系统，全靠一根网线连到主机上。主机上的系统（共享库）更新了，大家一刷新就全都用上新的了，既省硬盘又省内存。

**下图概括了程序的动态链接过程**
```text
 [ main2.c ]  [ vector.h ]
      │             │
      ▼             ▼
+-----------------------------+
|       翻 译 器 (cpp, ccl, as) | <──────────────┐
+-----------------------------+                  │
      │                                          │
      ▼                                          │
[ 可重定位目标文件 main2.o ]                     │  libc.so
      │                                          │  libvector.so
      ▼                                          │ (重定位和符号表信息)
+-----------------------------+                  │
|        链 接 器 (ld)          | <──────────────┘
+-----------------------------+
      │
      ▼
[ 部分链接的可执行目标文件 prog21 ]
      │
      ▼
+-----------------------------+
|       加 载 器 (execve)       |
+-----------------------------+
      │
      ▼                                          │  libc.so
+-----------------------------+                  │  libvector.so
| 动态链接器 (ld-linux.so)      | <──────────────┘
+-----------------------------+     (代码和数据)
      │
      ▼
[ 内存中完全链接的可执行文件 ]
```

## 3.Catkin Tutorials 
(笔记资料来自：https://wiki.ros.org/catkin/Tutorials)

### (1).creating a workspace for catkin(创建工作空间)

1. ROS 装好之后，它的所有命令（比如 roscore、catkin_make）其实都藏在 `/opt/ros/noetic` 这个系统深层的文件夹里。Linux 默认是找不到它们的。

这时候就需要`source /opt/ros/noetic/setup.bash`这句命令来找到。

- 可以说，每次新开一个终端，必须 source 一下系统环境，否则终端连 catkin_make 是什么都不知道。

2. 之后建一个叫 catkin_ws 的工作空间（Workspace）。

其中所有的 ROS 功能包源码以后都必须放在这个 src 里面。

`mkdir -p ~/catkin_ws/src`

3. 想走进这个工作空间的“大门”。必须站在这个大门里（也就是 catkin_ws 根目录）

`cd ~/catkin_ws/`

4. 一键建目录、配置并编译整个工作空间,也就是只要在工作空间里敲下它，就能自动帮你建好文件夹、生成配置、并编译所有代码。

`catkin_make`

**这一句后的系统会反馈一大串东西**

5. 在catkin_make编译完了，产生了一个 devel 文件夹，刚才写的代码包（比如 my_first_pkg）就藏在里面。

这时候需要source一下它，告诉系统要把代码包加入到环境变量中。

`source devel/setup.bash`

6. 确认刚写的代码包路径有没有被成功收录进系统的环境变量里。

`echo $ROS_PACKAGE_PATH`

**这一句后的系统会反馈：**`/root/catkin_ws/src:/opt/ros/noetic/share`

**完整终端流程：**
```bash
jiecheng@jiecheng-Legion-Y7000-IRX10:~$ sudo docker start 91a4707596b1
sudo docker exec -it 91a4707596b1 bash
[sudo: authenticate] 密码：          
91a4707596b1
root@91a4707596b1:/# source /opt/ros/noetic/setup.bash
root@91a4707596b1:/# mkdir -p ~/catkin_ws/src
root@91a4707596b1:/# cd ~/catkin_ws/
root@91a4707596b1:~/catkin_ws# catkin_make
Base path: /root/catkin_ws
Source space: /root/catkin_ws/src
Build space: /root/catkin_ws/build
Devel space: /root/catkin_ws/devel
Install space: /root/catkin_ws/install
####
#### Running command: "make cmake_check_build_system" in "/root/catkin_ws/build"
####
####
#### Running command: "make -j24 -l24" in "/root/catkin_ws/build"
####
root@91a4707596b1:~/catkin_ws# source devel/setup.bash
root@91a4707596b1:~/catkin_ws# echo $ROS_PACKAGE_PATH
/root/catkin_ws/src:/opt/ros/noetic/share
root@91a4707596b1:~/catkin_ws# 
```

### (2).Creating a ROS Package(创建一个ROS包)

1. 首先，切换到创建的 catkin 工作空间的源代码（source）空间目录：

`cd ~/catkin_ws/src`

2. 然后使用脚本创建一个新包：

`catkin_create_pkg beginner_tutorials std_msgs rospy roscpp`

3. 接下来是构建catkin工作空间中的包：

 `cd ~/catkin_ws`
 
 `catkin_make`

4. 要将工作空间添加到ROS环境中，需要加载生成的文件：

`. ~/catkin_ws/devel/setup.bash`

5. 可以使用工具查看一阶依赖：

`rospack depends1 beginner_tutorials`

6. 还可以查看间接依赖：

`rospack depends1 rospy`

7. rospack可以递归确定所有嵌套的依赖项：

`rospack depends beginner_tutorials`

**注释**
`rospack depends1 beginner_tutorials`之后会得到：
```
roscpp
rosgraph
rosgraph_msgs
roslib
std_msgs
```


### (3).Building and using catkin packages in a workspace(在工作空间中构建和使用catkin包)

1. 在catkin工作空间的底层调用catkin_make

`cd ~/catkin_ws/src/beginner_tutorials/src`

`cd ~/catkin_ws/src/beginner_tutorials`

`cd ~/catkin_ws`

`catkin_make -DCMAKE_BUILD_TYPE=Release`

2. 将平时传递给make的任何参数传递给catkin_make:

`cd ~/catkin_ws`

`catkin_make install`

3. 使用install目标将其安装到系统中：

`cd ~/catkin_ws`

`catkin_make install -DCMAKE_INSTALL_PREFIX=/opt/ros/groovy  # might need sudo`

4. 告诉catkin将新包添加到已经编译好的二进制文件：

`catkin_make --force-cmake`

5. 查看命令行：

`catkin_make -DCMAKE_VERBOSE_MAKEFILE=ON`

**终端最后输出：**(作节选)
```bash
Base path: /root/catkin_ws
Source space: /root/catkin_ws/src
Build space: /root/catkin_ws/build
Devel space: /root/catkin_ws/devel
Install space: /root/catkin_ws/install
####
#### Running command: "cmake /root/catkin_ws/src -DCMAKE_VERBOSE_MAKEFILE=ON -DCATKIN_DEVEL_PREFIX=/root/catkin_ws/devel -DCMAKE_INSTALL_PREFIX=/root/catkin_ws/install -G Unix Makefiles" in "/root/catkin_ws/build"
####
-- Using CATKIN_DEVEL_PREFIX: /root/catkin_ws/devel
-- Using CMAKE_PREFIX_PATH: /root/catkin_ws/devel;/opt/ros/noetic
-- This workspace overlays: /root/catkin_ws/devel;/opt/ros/noetic
-- Found PythonInterp: /usr/bin/python3 (found suitable version "3.8.10", minimum required is "3") 
-- Using PYTHON_EXECUTABLE: /usr/bin/python3
-- Using Debian Python package layout
-- Using empy: /usr/lib/python3/dist-packages/em.py
-- Using CATKIN_ENABLE_TESTING: ON
-- Call enable_testing()
-- Using CATKIN_TEST_RESULTS_DIR: /root/catkin_ws/build/test_results
-- Forcing gtest/gmock from source, though one was otherwise available.
-- Found gtest sources under '/usr/src/googletest': gtests will be built
-- Found gmock sources under '/usr/src/googletest': gmock will be built
-- Found PythonInterp: /usr/bin/python3 (found version "3.8.10") 
-- Using Python nosetests: /usr/bin/nosetests3
-- catkin 0.8.12
-- BUILD_SHARED_LIBS is on
-- BUILD_SHARED_LIBS is on
-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- ~~  traversing 2 packages in topological order:
-- ~~  - beginner_tutorials
-- ~~  - my_first_pkg
-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- +++ processing catkin package: 'beginner_tutorials'
-- ==> add_subdirectory(beginner_tutorials)
-- +++ processing catkin package: 'my_first_pkg'
-- ==> add_subdirectory(my_first_pkg)
-- Configuring done
-- Generating done
-- Build files have been written to: /root/catkin_ws/build
####
#### Running command: "make -j24 -l24" in "/root/catkin_ws/build"
####
/usr/bin/cmake -S/root/catkin_ws/src -B/root/catkin_ws/build --check-build-system CMakeFiles/Makefile.cmake 0
/usr/bin/cmake -E cmake_progress_start /root/catkin_ws/build/CMakeFiles /root/catkin_ws/build/CMakeFiles/progress.marks
make -f CMakeFiles/Makefile2 all
make[1]: Entering directory '/root/catkin_ws/build'
make[1]: Nothing to be done for 'all'.
make[1]: Leaving directory '/root/catkin_ws/build'
/usr/bin/cmake -E cmake_progress_start /root/catkin_ws/build/CMakeFiles 0
```

###  (4).Overlaying with catkin workspaces(使用catkin工作空间进行叠加)
**deepseek解读**
这部分是 Overlay（工作空间叠加），属于 ROS 高阶操作。它的核心目的是：在系统原有的 ROS 之上，再叠加一层您自己写的代码层。 当您调用某个包时，系统会优先使用您叠加层里的版本。

1. 安装ROS包（此处省略）

2. 首先要创建一个文件夹来存放新的工作空间：

`mkdir -p ~/overlay_ws/src`

`cd ~/overlay_ws`

3. 然后把ROS的源码添加到src文件：

`cd ~/overlay_ws/src`

`git clone https://github.com/ros/ros_tutorials.git`

4. 确保环境支持构建：

`source /opt/ros/noetic/setup.bash`

5. 再次构建工作空间：
`cd ~/overlay_ws/`

`catkin_make`

6. 叠加安装：

`source ~/overlay_ws/devel/setup.bash`

7. 测试：

`roscd rospy_tutorials`

`pwd`
 
8. 创建第二个工作空间：

`mkdir -p ~/overlay_ws_overlay/src`

`cd ~/overlay_ws_overlay/src`

`catkin_init_workspace`

`cd ~/overlay_ws_overlay`

`source ~/overlay_ws/devel/setup.bash`

`catkin_make`

9. 打开一个新的终端测试叠加：

`source ~/overlay_ws_overlay/devel/setup.bash`

`roscd rospy_tutorials`

`pwd`

10. 如果没有生效，要更新roscd：

`cd ~/overlay_ws_overlay`

`catkin_make`

`rospack profile`

`roscd rospy_tutorials`

`pwd`

**终端交互流程：**
```bash
root@91a4707596b1:~/catkin_ws# mkdir -p ~/overlay_ws/src
root@91a4707596b1:~/catkin_ws# cd ~/overlay_ws/src
root@91a4707596b1:~/overlay_ws/src# git clone https://github.com/ros/ros_tutorials.git
Cloning into 'ros_tutorials'...
remote: Enumerating objects: 3506, done.
remote: Counting objects: 100% (204/204), done.
remote: Compressing objects: 100% (78/78), done.
remote: Total 3506 (delta 162), reused 126 (delta 126), pack-reused 3302 (from 3)
Receiving objects: 100% (3506/3506), 802.84 KiB | 486.00 KiB/s, done.
Resolving deltas: 100% (2110/2110), done.
root@91a4707596b1:~/overlay_ws/src# cd ~/overlay_ws/
root@91a4707596b1:~/overlay_ws# catkin_make
Base path: /root/overlay_ws
Source space: /root/overlay_ws/src
Build space: /root/overlay_ws/build
Devel space: /root/overlay_ws/devel
Install space: /root/overlay_ws/install
Creating symlink "/root/overlay_ws/src/CMakeLists.txt" pointing to "/opt/ros/noetic/share/catkin/cmake/toplevel.cmake"
####
#### Running command: "cmake /root/overlay_ws/src -DCATKIN_DEVEL_PREFIX=/root/overlay_ws/devel -DCMAKE_INSTALL_PREFIX=/root/overlay_ws/install -G Unix Makefiles" in "/root/overlay_ws/build"
####
-- The C compiler identification is GNU 9.4.0
-- The CXX compiler identification is GNU 9.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Using CATKIN_DEVEL_PREFIX: /root/overlay_ws/devel
-- Using CMAKE_PREFIX_PATH: /root/catkin_ws/devel;/opt/ros/noetic
-- This workspace overlays: /root/catkin_ws/devel;/opt/ros/noetic
-- Found PythonInterp: /usr/bin/python3 (found suitable version "3.8.10", minimum required is "3") 
-- Using PYTHON_EXECUTABLE: /usr/bin/python3
-- Using Debian Python package layout
-- Found PY_em: /usr/lib/python3/dist-packages/em.py  
-- Using empy: /usr/lib/python3/dist-packages/em.py
-- Using CATKIN_ENABLE_TESTING: ON
-- Call enable_testing()
-- Using CATKIN_TEST_RESULTS_DIR: /root/overlay_ws/build/test_results
-- Forcing gtest/gmock from source, though one was otherwise available.
-- Found gtest sources under '/usr/src/googletest': gtests will be built
-- Found gmock sources under '/usr/src/googletest': gmock will be built
-- Found PythonInterp: /usr/bin/python3 (found version "3.8.10") 
-- Found Threads: TRUE  
-- Using Python nosetests: /usr/bin/nosetests3
-- catkin 0.8.12
-- BUILD_SHARED_LIBS is on
-- BUILD_SHARED_LIBS is on
-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- ~~  traversing 2 packages in topological order:
-- ~~  - turtlesim_msgs (plain cmake)
-- ~~  - turtlesim (plain cmake)
-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CMake Error at /opt/ros/noetic/share/catkin/cmake/catkin_workspace.cmake:100 (message):
  This workspace contains non-catkin packages in it, and catkin cannot build
  a non-homogeneous workspace without isolation.  Try the
  'catkin_make_isolated' command instead.
Call Stack (most recent call first):
  CMakeLists.txt:69 (catkin_workspace)


-- Configuring incomplete, errors occurred!
See also "/root/overlay_ws/build/CMakeFiles/CMakeOutput.log".
See also "/root/overlay_ws/build/CMakeFiles/CMakeError.log".
Invoking "cmake" failed
root@91a4707596b1:~/overlay_ws# rm -rf ~/overlay_ws/src/ros_tutorials
root@91a4707596b1:~/overlay_ws# cp -r ~/catkin_ws/src/beginner_tutorials ~/overlay_ws/src/
root@91a4707596b1:~/overlay_ws# cd ~/overlay_ws
root@91a4707596b1:~/overlay_ws# catkin_make
Base path: /root/overlay_ws
Source space: /root/overlay_ws/src
Build space: /root/overlay_ws/build
Devel space: /root/overlay_ws/devel
Install space: /root/overlay_ws/install
####
#### Running command: "cmake /root/overlay_ws/src -DCATKIN_DEVEL_PREFIX=/root/overlay_ws/devel -DCMAKE_INSTALL_PREFIX=/root/overlay_ws/install -G Unix Makefiles" in "/root/overlay_ws/build"
####
-- Using CATKIN_DEVEL_PREFIX: /root/overlay_ws/devel
-- Using CMAKE_PREFIX_PATH: /root/catkin_ws/devel;/opt/ros/noetic
-- This workspace overlays: /root/catkin_ws/devel;/opt/ros/noetic
-- Found PythonInterp: /usr/bin/python3 (found suitable version "3.8.10", minimum required is "3") 
-- Using PYTHON_EXECUTABLE: /usr/bin/python3
-- Using Debian Python package layout
-- Using empy: /usr/lib/python3/dist-packages/em.py
-- Using CATKIN_ENABLE_TESTING: ON
-- Call enable_testing()
-- Using CATKIN_TEST_RESULTS_DIR: /root/overlay_ws/build/test_results
-- Forcing gtest/gmock from source, though one was otherwise available.
-- Found gtest sources under '/usr/src/googletest': gtests will be built
-- Found gmock sources under '/usr/src/googletest': gmock will be built
-- Found PythonInterp: /usr/bin/python3 (found version "3.8.10") 
-- Using Python nosetests: /usr/bin/nosetests3
-- catkin 0.8.12
-- BUILD_SHARED_LIBS is on
-- BUILD_SHARED_LIBS is on
-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- ~~  traversing 1 packages in topological order:
-- ~~  - beginner_tutorials
-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- +++ processing catkin package: 'beginner_tutorials'
-- ==> add_subdirectory(beginner_tutorials)
-- Configuring done
-- Generating done
-- Build files have been written to: /root/overlay_ws/build
####
#### Running command: "make -j24 -l24" in "/root/overlay_ws/build"
####
root@91a4707596b1:~/overlay_ws# source devel/setup.bash
root@91a4707596b1:~/overlay_ws# roscd beginner_tutorials
root@91a4707596b1:~/overlay_ws/src/beginner_tutorials# pwd
/root/overlay_ws/src/beginner_tutorials
```
**注释**：其中，因为我们所拉取的ros_tutorials源码库，里面包含了一些早期 ROS 1 时代遗留的纯 CMake 包（如 turtlesim_msgs）。catkin_make 不允许混合编译纯 CMake 包和 Catkin 包，所以它直接报错。为此，通过询问deepseek，通过建设自己的纯净包来替代完成测试。

## 4.如何用200行代码编码克劳德密码
（笔记资料来自：https://www.mihaileric.com/The-Emperor-Has-No-Clothes/，这确实是一篇很好的文章，我还得多读几遍）

人工智能编程助手会读取文件、编辑项目并编写函数式代码。，能支持这种能力，大概需要200行简单的Python。

### (1).理解使用编码代理时的实际流程
1. YOU发送消息(“创建一个具有 hello world 函数的新文件”)
2. The LLMLLM 决定需要一个工具,并通过结构化工具调用(或多个工具调用)进行响应
3. Your program您的程序在本地执行该工具调用(实际上创建文件)
4. The result结果会被退回到LLM
5. The LLM 会利用该上下文来继续或回应

### (2).需要的三种工具
1. 读取文件,以便 LLM 能够查看您的代码
2. 列出文件,以便其能够浏览您的项目
3. 编辑文件,以便其提供创建和修改代码的指令

- （三种工具足以完成令人惊叹的事情！）

### (3).个人思考
从中可见，AI助手=LLM+工具箱+循环；
其运转机制：“请求-执行-反馈”的完美闭环（The Loop）

AI 帮我们写代码，其实就是一个 5 步的循环：
1. 下达指令（比如：“帮我建个 hello.py”）。
2. 大模型思考后说：“我要用 edit_file 这个工具，参数是XXX”（它只负责动嘴）。
3. 写的 Python 程序接收到指令，在本地真正动手创建文件（它负责动手）。
4. 程序把执行结果（成功或报错）返回给大模型。
5. 大模型看到结果后，决定是继续下一步，还是直接告诉我们“搞定了”。

其中AI最核心，最基础有三个能力：

· **读（Read files**：让 AI 能看到我们现有的代码。

· **看（List files**：让 AI 知道我们的项目里都有啥，能自己找路。

· **写(Edit files)**：让 AI 能把想法变成真正的代码写进文件里。

> 其中文章提到一个非常关键的洞察（Key Insight）：

> 我们其实没有教 AI 怎么思考，我们只是在“告诉它有哪些工具，以及调用的格式是什么”。

> 通过把 Python 函数的注释（docstrings）提取出来拼成提示词（Prompt），大模型自己就能看懂这些工具是干嘛用的，并在什么时候该用它们。

> 大模型的理解能力，就是整个系统的灵魂。（灵魂！！！）

### (4).在终端上的实操
deepseek的指导下完成了以下实操：
```bash
root@91a4707596b1:~# cat << 'EOF' > full_agent.py
> import json
> import os
> # 读
> def read_file_tool(filename):
>     try:
>         with open(filename, 'r', encoding='utf-8') as f:
>             return {"file_path": filename, "content": f.read()}
>     except Exception as e:
>         return {"error": str(e)}
> # 列
> def list_files_tool(path="."):
>     try:
>         files = [{"filename": f, "type": "file" if os.path.isfile(os.path.join(path, f)) else "dir"} for f in os.listdir(path)]
>         return {"path": path, "files": files}
>     except Exception as e:
>         return {"error": str(e)}
> # 按名称查找工具的方法:
> TOOL_REGISTRY = {
>     "read_file": read_file_tool,
>     "list_files": list_files_tool
> }
> 
> #  真正严谨的模拟大模型逻辑
> def mock_llm_call(conversation):
>     # 把历史对话全拼成一个大字符串，方便查找
>     history_text = " ".join([msg["content"] for msg in conversation])
>     
>     # 如果历史记录里既没有 list_files，也没有 file_path，那就是刚开始
>     if "list_files" not in history_text and "file_path" not in history_text:
>         return 'tool: list_files({"path": "."})'
>     
>     # 如果有了 list_files，但没有 file_path，说明看完了目录，要去读文件
>     elif "list_files" in history_text and "file_path" not in history_text:
>         return 'tool: read_file({"filename": "test.txt"})'
>     
>     # 如果 file_path 都出来了，说明彻底读完了
>     else:
>         return "我已经帮您读取完 test.txt 了，任务结束！"
> 
> def extract_tool_invocations(text):
>     invocations = []
>     for line in text.splitlines():
>         line = line.strip()
>         if not line.startswith("tool:"): continue
>         try:
>             after = line[len("tool:"):].strip()
>             name, rest = after.split("(", 1)
>             args = json.loads(rest[:-1])
>             invocations.append((name.strip(), args))
>         except Exception:
>             pass
>     return invocations
> 
> def run_agent_loop():
>     print("--- 启动本地 Agent ---")
>     with open("test.txt", "w", encoding="utf-8") as f:
>         f.write("Hello, Agent! 这是真实的文件内容。")
>     
>     conversation = [{"role": "user", "content": "请先查看当前目录的内容，然后帮我读取 test.txt"}]
>     print(f"用户: {conversation[0]['content']}\n")
>     
>     step = 1
>     while True:
>         response = mock_llm_call(conversation)
>         print(f"[第 {step} 轮] Assistant 决定: {response}")
>         
>         tool_invocations = extract_tool_invocations(response)
>         if not tool_invocations:
>             print("--- 任务结束，大模型未请求更多工具 ---")
>             break
>             
>         for name, args in tool_invocations:
>             print(f"  [本地执行] 调用工具 {name}")
>             tool_result = TOOL_REGISTRY[name](**args)
>             print(f"  [工具结果] 成功获取数据")
>             
>             # 必须把 AI 的原始回复和工具的执行结果追加到历史中
>             conversation.append({"role": "assistant", "content": response})
>             conversation.append({"role": "user", "content": f"tool_result({json.dumps(tool_result, ensure_ascii=False)})"})
>         print("-" * 40)
>         step += 1
> 
> if __name__ == "__main__":
>     run_agent_loop()
> EOF
root@91a4707596b1:~# python3 full_agent.py
--- 启动本地 Agent ---
用户: 请先查看当前目录的内容，然后帮我读取 test.txt

[第 1 轮] Assistant 决定: tool: list_files({"path": "."})
  [本地执行] 调用工具 list_files
  [工具结果] 成功获取数据
----------------------------------------
[第 2 轮] Assistant 决定: tool: read_file({"filename": "test.txt"})
  [本地执行] 调用工具 read_file
  [工具结果] 成功获取数据
----------------------------------------
[第 3 轮] Assistant 决定: 我已经帮您读取完 test.txt 了，任务结束！
--- 任务结束，大模型未请求更多工具 ---

```
**注**：根据deepseek的说法，对于这个简易的agent,可以通过以下步骤实现真正的agent功能：
1. **决策层**：从硬编码的 `if-else` 判断，升级为真实的 LLM API 调用，赋予了系统真正的自然语言理解能力。
2. **解析层**：废弃了容易出 Bug 的字符串切割，改用大模型原生的 `tools` 参数和结构化 JSON 返回（`tool_calls`），实现了更稳定的工具调度。
3. **交互层**：从单次固定任务，升级为带记忆的多轮对话循环。AI 能够在真实的对话中，根据需要自主决定调用哪个本地工具。
写在最后，记住：
**这基本上只是与一个拥有工具箱的强大LLM的对话。**

## 5.个人使用大模型的思考

现在 AI 发展迅速，这是一个属于人工智能的时代。大模型已经成为我们不可或缺的强大工具，我们在工作和学习中都或多或少会使用到 DeepSeek、GPT 等 Agent。

就拿我这次的学习来说，由于 CMake 和 Catkin 的相关资料繁多、阅读难度大、思维繁杂，我也多次使用大模型为我辅助，比如翻译外文资料、理解生涩的术语和概念，甚至在电脑终端上实操遇到报错时，也是大模型帮我排错解决的。

基本上我的学习模式就是，先学习文献知识，然后根据文献知识结合大模型的指导在电脑上完成实操，对于实操结果再结合大模型的解读进行理解和学习，最后整理成为笔记。

可以说，在本次学习中大模型起到了至关重要的作用。正是它强大的工具能力，让我能更深入地理解和吸收文献知识，也让我对 CMake 和 Catkin 建立了一个初步的认识。

不过，大模型终究也只是工具，归根到底还是得自己去学习、吸收和实践。就比如在阅读《如何用200行代码编码克劳德密码》这篇文章时，还是得自己去思考其中所传递的核心思想以及应用所在。**最终，依然需要我手动进行笔记的整理、整合与排版。**