# 定义

CMake是一种过程式语言，是一个跨平台的编译工具，可以用简单的语句来描述所有平台的编译过程。它能够产生标准的建构档makefile或者project文件

# 流程

![image-20261025155054019](C:\Users\WU\AppData\Roaming\Typora\typora-user-images\image-20261025155054019.png)

![image-20261025155107146](C:\Users\WU\AppData\Roaming\Typora\typora-user-images\image-20261025155107146.png)

# 基本语句

1. catkin --help##检查catkin是否安装
2. mkdir catkin_ws 
3. cd /catkin_ws &mkdir src
4. cd src
5. catkin_init_workspace ##初始化工作空间
6. cd ..
7. catkin build

## 基本创建

cd ~/catkin_ws/src/

创建test_pkg功能包，同时指明依赖std_msgs rospy roscpp

catkin_create_pkg test_pkg std_msgs rospy roscpp

### **常见**

1. CMakeLists.txt：编译规则（必须有）

2. package.xml：包描述/依赖信息（必须有）

3. src/：C++/Python 源代码

4. include/：C++ 头文件

5. scripts/：可执行脚本

6. msg/：自定义消息

7. srv/：自定义服务

8. models/：3D 模型文件

9. urdf/：机器人描述文件

10. launch/：启动文件

    核心： 前两项是强制必需的，其余按需创建。
------


. 预处理（Preprocessing）

    动作：处理所有 # 开头的指令。展开宏定义（纯文本替换，一堆一样的字词换一下）、插入头文件、删掉注释。

    产物：纯 C 代码文件 hello.i。

2. 编译（Compilation）

    动作：经过词法分析、语法分析、语义分析、优化等 6 步，把高级语言翻译成人类还能看懂的汇编语言。

    产物：汇编代码文件 hello.s。

3. 汇编（Assembly）—— 中转

    动作：把汇编指令翻译成机器指令（0和1），打包成 ELF 格式。这时候代码里全是未知地址，等着被重定位。

    产物：可重定位目标文件 hello.o。

4. 链接（Linking）—— 拼装整车

    动作：链接器（ld）把 hello.o 和系统库 printf.o 拼在一起。进行符号解析（找零件）和重定位（分配真实内存地址，填死窟窿）。

    产物：可执行目标文件 hello。

5. 运行（Execution）—— 上路开跑

    动作：操作系统（Loader）把它拉进虚拟内存，分配独立的地址空间。CPU 跳转到入口点（_start -> main），直接执行 0 和 1。

   1.  宏定义 = 文本替换
    编译前无脑替换字面，不管合不合理（所以参数必须加括号）。

   2. 库 = 二进制的字典
    别人写好的、极其复杂的机器码集合。你查字典（调用函数），不需要自己从头造字。

   3. ELF 文件 = 快递柜
    统一的标准格式。不管 .o（半成品）、可执行文件（成品）还是 .so（共享库），底层外壳都是它，只是内部规格不同。

   4. 依赖 = 正确编译、链接、运行所需的全部外部条件
    不限于运行阶段，还包括头文件路径、编译选项、宏、静态/动态库等。

https://chat.deepseek.com/share/5kc7dtitt6w4akoeu7    deepseek交流


------


二    https://www.runoob.com/cmake/cmake-basic.html 
# 一、CMakeLists.txt 是什么

- **格式**：纯文本配置文件，文件名固定为 `CMakeLists.txt`。
- **用法**：放在项目根目录（或子目录），由 CMake 读取。
- **作用**：告诉 CMake 项目的结构、源文件列表、编译要求（C++ 标准、头文件路径、链接库等）。每个 CMake 项目至少需要一个。

# 二、基本语法

- **格式**：`命令名(参数列表)`
- **用法**：每条指令一行或多行书写，参数用空格分隔。
- **作用**：CMake 按顺序执行这些指令完成项目配置。

## 1. cmake_minimum_required

**格式**：

cmake

```
cmake_minimum_required(VERSION <版本号>)
```



**用法**：

cmake

```
cmake_minimum_required(VERSION 3.10)
```



必须放在文件最顶部。

**作用**：声明项目要求的最低 CMake 版本，使 CMake 启用对应的兼容策略，避免旧版本因不支持新指令而报错。

## 2. project

**格式**：

cmake

```
project(<项目名> [VERSION <版本>] [LANGUAGES <语言>...])
```



**用法**：

cmake

```
project(MyProject CXX)
project(MyProject VERSION 1.0.0 LANGUAGES CXX)
```



语言常用 `CXX`（C++）、`C`（C 语言），可省略（默认 C 和 CXX）。

**作用**：定义项目名称和语言，并自动设置 `PROJECT_NAME`、`PROJECT_SOURCE_DIR`、`PROJECT_VERSION` 等变量。

## 3. add_executable

**格式**：

cmake

```
add_executable(<目标名> <源文件>...)
```



**用法**：

cmake

```
add_executable(MyApp main.cpp utils.cpp)
```



也可以用set把所有源文件名打包成一个变量再来add_executable



**作用**：把列出的源文件编译（先）、（后）链接成一个可执行文件叫目标标；第一个参数是目标名，供后续 `target_*`、`install` 等指令引用。

**源文件 → 各自编译成 .o → 链接成一个整体 → 这个整体就是"目标"，第一个参数给它命名。**

## 4. add_library

**格式**：

cmake

```
add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)
```

​									库类型（可选）                                                                      (库是代码包，无main（）可执行文件才有)

| 类型     | 产物                              | 何时用                             | 链接方式                 |
| -------- | --------------------------------- | ---------------------------------- | ------------------------ |
| `STATIC` | `.a`（Linux）/ `.lib`（Windows）  | 想随程序一起分发、无运行时依赖     | 编译时**嵌入**可执行文件 |
| `SHARED` | `.so`（Linux）/ `.dll`（Windows） | 多程序共享、可单独更新             | **运行时**加载           |
| `MODULE` | `.so`/`.dll`                      | 插件，运行时用 `dlopen` 等动态加载 | 通常**不直接链接**       |

**"部署要带库文件"= 动态库程序运行时才加载 `.so`/`.dll`，所以拷贝程序到别的机器时，必须把这些库文件一起带上，否则启动报"找不到共享库"。静态库已嵌进可执行文件，则不用带。**

**用法**：

cmake

```
add_library(MyLib STATIC library.cpp)
```



- `STATIC`：静态库（`.a` / `.lib`），编译时嵌入可执行文件。
- `SHARED`：动态库（`.so` / `.dll`），运行时加载。
- `MODULE`：插件式模块，通常不直接链接。
- 不写类型时由 `BUILD_SHARED_LIBS` 决定。

**作用**：生成一个库目标，供其他目标链接。

## 5. target_link_libraries

**格式**：

cmake

```
target_link_libraries(<目标> <PUBLIC|PRIVATE|INTERFACE> <库>...)
```

**给这个目标加上它要用的库，并按可见性决定要不要把这个依赖继续传给下游。**

**用法**：

cmake

```
target_link_libraries(MyApp PRIVATE MyLib)
```



库可以是本项目目标、外部导入目标（如 `Boost::Boost`）或系统库名（如 `pthread`）。

**作用**：把指定库链接到目标上；可见性关键字决定该依赖是否传播给依赖此目标的其他目标。

## 6. set（设置变量）

**格式**：

cmake

```
set(<变量名> <值>...)
```



**用法**：

cmake

```
set(CMAKE_CXX_STANDARD 17)
set(SOURCES main.cpp utils.cpp helper.cpp)
add_executable(MyApp ${SOURCES})
```



引用变量用 `${变量名}`；多值即列表变量（空格或分号分隔）。

**作用** :**定义/赋值变量**，供后续指令用 `${变量名}` 引用。能设两类：

1. **自定义变量**：如 `SOURCES`、`MY_APP_NAME`，纯拿来自己用。**当前目录及子目录**有效，配置过程结束就没了，不持久化。
2. **CMake 内置变量**：如 `CMAKE_CXX_STANDARD`、`CMAKE_BUILD_TYPE`，设了会改变 CMake 行为。每次重新运行 `cmake` 都重来
3. 用的时候要${a}

## 7. target_include_directories

**格式**：

cmake

```
target_include_directories(<目标>
  [BEFORE|AFTER] [SYSTEM]
  [PUBLIC|PRIVATE|INTERFACE]
  <路径>...)
```

---

| 参数                           | 含义                                                 |
| ------------------------------ | ---------------------------------------------------- |
| `<目标>`                       | 给哪个目标加头文件搜索路径                           |
| `BEFORE`/`AFTER`               | 路径加在搜索列表的前面还是后面（控制搜索顺序），可选 |
| `SYSTEM`                       | 标记为系统头文件，抑制其警告，可选                   |
| `PUBLIC`/`PRIVATE`/`INTERFACE` | 可见性，决定路径是否传播给消费者                     |
| `<路径>...`                    | 头文件目录，可多个                                   |

**用法**：

cmake

```
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
```

**`target_include_directories(目标 可见性 路径...)` = 给这个目标加头文件搜索路径（相当于 `-I`），并按可见性决定是否把该路径传播给依赖它的目标。**

**头文件搜索目录 = 编译时编译器按顺序去查找 `#include` 头文件的那些文件夹；CMake 里用 `target_include_directories` 添加，最终变成编译器的 `-I` 参数。**

`target_include_directories` = **定义"我创建的头文件应该去哪个目录里被搜索"**，即把该目录加入编译器的头文件搜索路径（`-I`）。

------

- `PUBLIC`：自身和消费者都可用。
- `PRIVATE`：仅自身可用。
- `INTERFACE`：仅消费者可用。
- `SYSTEM`：标记为系统头文件，抑制警告。
- `BEFORE`/`AFTER`：控制搜索顺序。

**作用**：为指定目标添加头文件搜索路径（相当于编译器 `-I`），并通过可见性关键字精确控制传播范围。

## 8. install

**格式**：

cmake

```
install(TARGETS <目标>...
  [RUNTIME DESTINATION <路径>]
  [LIBRARY DESTINATION <路径>]
  [ARCHIVE DESTINATION <路径>]
  [INCLUDES DESTINATION <路径>])
```



**用法**：

cmake

```
install(TARGETS MyApp RUNTIME DESTINATION bin)
install(TARGETS MyLib ARCHIVE DESTINATION lib)
```

**`RUNTIME`**：对应**可执行文件**（以及 Windows 下的动态库 `.dll`）。默认安装到 `bin` 目录。

**`LIBRARY`**：对应**非 Windows 平台的动态库**（`.so`/`.dylib`），以及模块库。默认安装到 `lib` 目录。

**`ARCHIVE`**：对应**静态库**（`.a`/`.lib`），以及 Windows 下动态库配套的导入库。默认安装到 `lib` 目录。

**`INCLUDES`**：不是用来“装头文件”的，而是配合 `install(EXPORT)` 使用，用来告诉 CMake：“当别人通过导出配置使用我的库时，应该去哪里找头文件”。

**作用**：定义执行 `cmake --install .`（或 `make install`）时各类文件的安装位置，路径相对于安装前缀（默认 `/usr/local`）。

## 9. if / elseif / else / endif

**格式**：

cmake

```
if(<条件>)
  ...
elseif(<条件>)
  ...
else()
  ...
endif()
```

**`if/elseif/else/endif` = 按条件执行不同配置块，用于区分构建类型、开关功能、按平台加选项等；条件支持比较、逻辑运算、变量真假、文件存在等判断，必须以 `endif()` 结束。**

**用法**：

cmake

```
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "当前为 Debug 构建模式")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
  message(STATUS "当前为 Release 构建模式")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()
```



支持比较（`STREQUAL`、`LESS`、`GREATER`）、逻辑运算（`AND`/`OR`/`NOT`）、变量真假判断（`if(ENABLE_LOGGING)`）。

**作用**：按条件执行不同配置，如区分 Debug/Release 构建、开关功能。

## 10. add_custom_command

**格式**：

cmake

```
add_custom_command(
  TARGET <目标>
  PRE_BUILD | PRE_LINK | POST_BUILD
  COMMAND <命令> [参数...]
  [COMMENT <说明>]
  [VERBATIM])
```

**`add_custom_command(TARGET 目标 阶段 COMMAND 命令...)` = 在指定目标的构建过程（编译前/链接前/构建后）执行自定义命令，常用于拷贝资源、生成文件、打印提示；用 `${CMAKE_COMMAND} -E` 保证跨平台。**

`add_custom_command(TARGET 目标 阶段 COMMAND 命令)` = **把一条自定义命令挂到某个目标上，在这个目标构建过程的指定阶段自动执行。**

**用法**：

cmake

```
add_custom_command(
  TARGET MyApp POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "构建完成！"
  COMMENT "打印构建完成信息"
)
```



- `PRE_BUILD`：编译前
- `PRE_LINK`：链接前
- `POST_BUILD`：构建完成后

**作用**：在构建过程的特定阶段执行自定义命令；`${CMAKE_COMMAND} -E echo` 是跨平台 echo，比系统命令更可移植。

# 三、完整实例

**格式**：一个综合使用上述指令的完整 `CMakeLists.txt`。

**用法**：

cmake

```
cmake_minimum_required(VERSION 3.10)              # 1. 版本
project(MyProject VERSION 1.0.0 LANGUAGES CXX)    # 2. 项目

set(CMAKE_CXX_STANDARD 17)                        # 3. C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)               #    强制要求

add_library(MyLib STATIC                          # 4. 静态库
  src/lib/core.cpp
  src/lib/helper.cpp
)

add_executable(MyApp                              # 5. 可执行文件
  src/main.cpp
  src/utils.cpp
)

target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)  # 6. 库公开头文件
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/src)     # 7. 可执行文件私有头文件

target_link_libraries(MyApp PRIVATE MyLib)        # 8. 链接

install(TARGETS MyApp RUNTIME DESTINATION bin)    # 9. 安装规则
install(TARGETS MyLib ARCHIVE DESTINATION lib)
```



**构建命令**：

bash

```
mkdir build && cd build
cmake ..
cmake --build .
cmake --install .
```



**作用**：演示一个库 + 一个可执行文件的标准项目结构；`MyLib` 用 `PUBLIC` 暴露 `include/`，`MyApp` 链接 `MyLib` 后自动继承该路径，无需重复声明；外部构建避免产物污染源码目录。

# 四、变量和缓存

## 1. 普通变量

**格式**：

cmake

```
set(<变量名> <值>...)
```



**用法**：

cmake

```
set(MY_APP_NAME "runoob-app")
set(SOURCE_FILES main.cpp utils.cpp helper.cpp)
message(STATUS "项目名称：${MY_APP_NAME}")
foreach(FILE ${SOURCE_FILES})
  message(STATUS "源文件：${FILE}")
endforeach()
```



**作用**：存储配置信息；作用域为当前目录及子目录，每次运行 CMake 重新计算，不持久化。

## 2. 缓存变量

**格式**：

cmake

```
set(<变量名> <默认值> CACHE <类型> <描述>)
```



**用法**：

cmake

```
set(MY_INSTALL_PATH "/usr/local" CACHE PATH "程序的安装路径")
set(ENABLE_LOGGING ON CACHE BOOL "是否启用日志功能")

if(ENABLE_LOGGING)
  message(STATUS "日志功能已启用")
  add_definitions(-DENABLE_LOGGING)
endif()
```

**若 `CMakeCache.txt` 中已存在该变量，`set(... CACHE ...)` 不会覆盖已有值。**或用 `FORCE` 强制覆盖

**类型**：`STRING`、`BOOL`、`PATH`、`FILEPATH`。用户可通过命令行覆盖：

**缓存变量 = 存进 `CMakeCache.txt`、跨次运行保留、用户可用 `-D` 覆盖的变量；`set(... CACHE 类型 描述)` 定义它，但已有缓存值时不会覆盖，要改得删缓存或加 `FORCE`。**

bash

```
cmake .. -DMY_INSTALL_PATH=/opt/runoob -DENABLE_LOGGING=OFF
```



**作用**：存储在 `CMakeCache.txt` 中，多次运行之间保持值不变，供用户在配置阶段自定义构建设置。

> **关键规则**：若 `CMakeCache.txt` 中已存在该变量，`set(... CACHE ...)` 不会覆盖已有值；修改默认值后需删除 `CMakeCache.txt` 或整个 `build` 目录再重新运行 `cmake`。

# 五、头文件搜索路径

## 1. include_directories 与 target_include_directories 对比

**格式**：

cmake

```
include_directories(<路径>...)
target_include_directories(<目标> <PUBLIC|PRIVATE|INTERFACE> <路径>...)
```



**用法**：

cmake

```
include_directories(${PROJECT_SOURCE_DIR}/include)                 # 全局，不推荐
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)  # 目标级，推荐
```



**作用对比**：

| 特性       | include_directories()              | target_include_directories()       |
| ---------- | ---------------------------------- | ---------------------------------- |
| 作用范围   | 全局，影响当前目录及子目录所有目标 | 仅作用于指定目标                   |
| 推荐程度   | 不推荐（旧式）                     | 推荐（现代最佳实践）               |
| 目标关联性 | 不绑定目标，可能误伤其他目标       | 显式绑定                           |
| 可维护性   | 差，易造成路径污染                 | 好，逻辑清晰                       |
| 可见性控制 | 无法精确控制                       | 通过 PUBLIC/PRIVATE/INTERFACE 控制 |

## 2. PUBLIC / PRIVATE / INTERFACE

**格式**：作为 `target_include_directories`、`target_link_libraries` 的可见性关键字。

**用法**：

cmake

```
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/src/internal)

add_library(HeaderOnlyLib INTERFACE)
target_include_directories(HeaderOnlyLib INTERFACE ${PROJECT_SOURCE_DIR}/header_only)
```



**作用**：

| 关键字    | 当前目标可用 | 依赖此目标的其他目标可用 | 典型场景                   |
| --------- | ------------ | ------------------------ | -------------------------- |
| PRIVATE   | 是           | 否                       | 仅自身实现需要的内部头文件 |
| PUBLIC    | 是           | 是                       | 库的公开 API 头文件        |
| INTERFACE | 否           | 是                       | 纯头文件库（自身不编译）   |

> **记忆口诀**：自身需要且消费者需要 → PUBLIC；只有自身需要 → PRIVATE；只有消费者需要 → INTERFACE。

# 六、查找库和包

## 1. find_package

**格式**：

cmake

```
find_package(<包名> [<版本>] [REQUIRED] [PATHS <路径>...])
```



**用法**：

cmake

```
find_package(Boost REQUIRED)                # 不限定版本
find_package(Boost 1.70 REQUIRED)           # 要求 ≥ 1.70
find_package(OpenCV REQUIRED PATHS /path/to/opencv)  # 自定义搜索路径
```



**作用**：搜索系统中已安装的库，并设置对应的导入目标或变量；`REQUIRED` 表示找不到就中止配置并报错。

## 2. 使用查找到的库

**格式**：`find_package` 成功后提供三类内容。

**用法**：

| 提供的内容     | 示例                    | 说明                                       |
| -------------- | ----------------------- | ------------------------------------------ |
| 导入目标       | `Boost::Boost`          | 现代推荐，直接用于 `target_link_libraries` |
| 头文件路径变量 | `${Boost_INCLUDE_DIRS}` | 旧式，配合 `include_directories`           |
| 库文件路径变量 | `${Boost_LIBRARY_DIRS}` | 旧式，配合 `link_directories`              |

**作用**：导入目标自动带上头文件路径、编译选项、链接选项，避免手动拼路径出错，依赖关系更清晰。

## 3. 使用第三方库完整示例

**格式**：

cmake

```
cmake_minimum_required(VERSION 3.10)
project(MyBoostProject CXX)
set(CMAKE_CXX_STANDARD 17)
find_package(Boost REQUIRED)
add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE Boost::Boost)
```



**用法**：`find_package` 找到 Boost 后，直接链接导入目标 `Boost::Boost`。

**作用**：一行 `target_link_libraries` 即可完成 Boost 的头文件路径和库链接，无需手动设置任何路径。

# 七、注意事项（四条最佳实践）

## 1. 优先使用 target_xxx 系列指令

**格式**：用 `target_include_directories()`、`target_link_libraries()` 替代 `include_directories()`、`link_directories()`。

**用法**：

cmake

```
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(MyApp PRIVATE MyLib)
```



**作用**：避免全局路径污染，依赖关系显式、可传播、易维护，适合大型项目。

## 2. 始终指定 C++ 标准

**格式**：

cmake

```
set(CMAKE_CXX_STANDARD <标准号>)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```



**用法**：

cmake

```
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```



**作用**：不设置时编译器用默认标准（可能是 C++98 或 C++14）；`CMAKE_CXX_STANDARD_REQUIRED ON` 保证编译器不支持指定标准时直接报错，而非静默降级。

## 3. 理解 PUBLIC / PRIVATE / INTERFACE

**格式**：作为 `target_*` 指令的可见性关键字。

**用法**：

cmake

```
target_link_libraries(MyApp PRIVATE MyLib)   # 仅 MyApp 自己用
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)  # 自身和消费者都用
```



**作用**：这是初学者最常见的坑；正确使用可精确控制依赖的传播范围，避免过度暴露或遗漏路径。

## 4. 缓存变量的默认值不会自动更新

**格式**：`set(<变量名> <默认值> CACHE <类型> <描述>)`

**用法**：修改默认值后，需删除 `CMakeCache.txt` 或整个 `build` 目录，再重新运行 `cmake`：

bash

```
rm -rf build && mkdir build && cd build && cmake ..
```



**作用**：避免"改了 `CMakeLists.txt` 却没生效"的困惑——这是缓存机制导致的常见问题

project/                        ← 源码目录（你维护）
├── CMakeLists.txt              ← 你手写的配置源文件，不是缓存
├── src/
└── build/                      ← 构建目录（cmake 生成）
    ├── CMakeCache.txt          ← 缓存变量存这里 ✅
    ├── CMakeFiles/
    └── Makefile / MyApp ...

**`build/` 就是"构建目录"，专门存放构建过程中生成的东西**，不污染源码目录。
https://chat.deepseek.com/share/2fuwaircmb67oa658f     deepseek交流
------



https://www.mihaileric.com/The-Emperor-Has-No-Clothes/    皇帝的新衣
## 一、智能体

> **智能体 = LLM 的"决策" + 你程序的"执行" + 一个把两者接起来的循环。**
>
> LLM 不会碰的文件系统，它只会"说话"。是外面那层程序替它干活。

------

## 二、基础

### 1. LLM 是什么

**LLM = Large Language Model（大语言模型）**，给它一段文字，它预测"下一个最可能是什么"，一个词一个词往外蹦。

- **不是数据库**，不"查"答案，而是**现场生成**答案
- **没有记忆**——每次对话，程序把整段历史重新发给他
- **不能上网/读文件**——这些是外面 agent 帮它做的

| 缩写 | 全称                    | 关系          |
| ---- | ----------------------- | ------------- |
| AI   | Artificial Intelligence | 最大范畴      |
| ML   | Machine Learning        | AI 的一种方法 |
| DL   | Deep Learning           | ML 用神经网络 |
| LLM  | Large Language Model    | DL 的一种     |

**AI ⊃ ML ⊃ DL ⊃ LLM**

**调用工具是 LLM 预测出了一行特定格式的文字（如 `tool: edit_file(...)`）。照格式输出，是因为 system prompt 和训练数据都指向这个格式。**

------

### 2. JSON 是什么

**JSON**，一种用纯文本表示结构化数据的格式，所有语言都认。

**类型**

| 类型   | 写法   | 例子                |
| ------ | ------ | ------------------- |
| 对象   | `{ }`  | `{"name": "Alice"}` |
| 数组   | `[ ]`  | `[1, 2, 3]`         |
| 字符串 | 双引号 | `"hello"`           |
| 数字   | 直接写 | `42`、`3.14`        |
| 布尔   | 小写   | `true` / `false`    |
| 空值   | 小写   | `null`              |

**JSON ↔ Python 对照：**

| JSON             | Python           |
| ---------------- | ---------------- |
| `{ }`            | `dict`           |
| `[ ]`            | `list`           |
| `"str"`          | `str`            |
| `42`             | `int`            |
| `true` / `false` | `True` / `False` |
| `null`           | `None`           |

**两个核心函数（带 `s` = 处理字符串）：**

python

```
import json

data = json.loads('{"path": "a.py"}')      # 字符串 → 字典（load string）
text = json.dumps({"path": "a.py"})        # 字典 → 字符串（dump string）
```



**常见坑**

- 只能用双引号，不能用单引号
- 不能有注释
- 末尾不能多逗号
- `json.loads` 格式不对会抛 `JSONDecodeError`
- 中文默认被转成 `\uXXXX`，需 `ensure_ascii=False`

------

## 三、智能体的心智模型

一次交互的完整流程：

text

```
我: "创建 hello.py，打印 hello world"
        ↓
LLM 看到: "我有 read_file / list_files / edit_file 三个工具"
LLM 回复: tool: edit_file({"path":"hello.py","old_str":"","new_str":"print('hi')"})
        ↓
我的程序: 解析这行文本 → 真的调用 edit_file_tool → 文件被创建
        ↓
我的程序把结果发回: tool_result({"path":"...","action":"created_file"})
        ↓
LLM: "完成！已创建 hello.py"
```



**五个步骤：**

1. 用户发消息
2. LLM 判断需要工具 → 返回结构化工具调用
3. 我的程序在本地执行
4. 结果送回给 LLM
5. LLM 用上下文继续或回应

------

## 四、三个工具

| 工具         | 用途   | 功能                       |
| ------------ | ------ | -------------------------- |
| `read_file`  | 看内容 | 返回文件内容               |
| `list_files` | 看结构 | 返回目录列表               |
| `edit_file`  | 写内容 | 替换 `old_str` → `new_str` |

**`edit_file` 的两种模式：**

python

```
if old_str == "":
    # 模式1：创建/覆盖整个文件
    full_path.write_text(new_str)
else:
    # 模式2：找 old_str → 替换 new_str（只替换第一次出现）
    edited = original.replace(old_str, new_str, 1)
```



**用"查找替换"不是"按行号改"**
因为 LLM 最容易犯"行号对不上"的错。"找原文→替换"是**基于内容**的定位，更稳健。

------

## 五、让 LLM 知道有哪些工具

### 工具描述是自动生成的

python

```
def get_tool_str_representation(tool_name: str) -> str:
    tool = TOOL_REGISTRY[tool_name]
    return f"""
    Name: {tool_name}
    Description: {tool.__doc__}          # ← 读 docstring
    Signature: {inspect.signature(tool)} # ← 读参数签名
    """
```



用 inspect 反射出**参数名**和**文档字符串**，拼进 system prompt。

> **反直觉的事实：** 你写的 `"""..."""` 注释不是给人看的，**是给 LLM 看的**。LLM 靠 docstring 判断"该不该用这个工具、参数怎么填"。

### system prompt 三件事

python

```
SYSTEM_PROMPT = """
You are a coding assistant...              ← 1. 你是谁
{tool_list_repr}                           ← 2. 你有什么工具（自动生成）
When you want to use a tool, reply with exactly one line:
'tool: TOOL_NAME({{JSON_ARGS}})' and nothing else.   ← 3. 怎么调用
"""
```



**prompt engineering 的本质：** 用自然语言和 LLM 约定一个协议。

------

## 六、解析，文本里出工具调用

LLM 回复是自由文本，要解析成 `(工具名, 参数字典)` 列表。

### 完整代码

python

```
def extract_tool_invocations(text):
    invocations = []
    for raw_line in text.splitlines():
        line = raw_line.strip()
        if not line.startswith("tool:"):       # ① 只认 tool: 开头
            continue
        try:
            after = line[len("tool:"):].strip() # ② 去掉前缀
            name, rest = after.split("(", 1)    # ③ 按第一个 ( 切
            name = name.strip()
            if not rest.endswith(")"):          # ④ 确认以 ) 结尾
                continue
            json_str = rest[:-1].strip()        # ⑤ 去掉尾部 )
            args = json.loads(json_str)         # ⑥ 解析 JSON
            invocations.append((name, args))
        except Exception:
            continue                            # ⑦ 出错就跳过
    return invocations
```

| 步骤 | 作用     | 关键点                                                   |
| ---- | -------- | -------------------------------------------------------- |
| ①    | 过滤     | 只处理 `tool:` 开头的行                                  |
| ②    | 去前缀   | `line[5:]` 切掉 `"tool:"`                                |
| ③    | 切名字   | `split("(", 1)` —— **只切一次**，防止 JSON 里的 `(` 破坏 |
| ④    | 防御     | 末尾不是 `)` 就跳过（防截断）                            |
| ⑤    | 去尾括号 | `rest[:-1]` 去掉最后的 `)`                               |
| ⑥    | 解析     | `json.loads` 字符串 → 字典                               |
| ⑦    | 容错     | 吞掉异常，跳过坏行，不崩程序                             |

1. **JSON 里有字面换行** → `splitlines()` 切碎 → 失败
2. **多行工具调用** → 每行不完整 → 漏掉
3. **正文里出现 `tool:`** → 误报
4. **静默吞异常** → 格式错的调用被丢弃，LLM 收不到反馈可能卡住

> 生产工具用 **function calling / tool use API**，由模型端保证返回合法 JSON，不用自己解析文本。

------

## 七、Agent 循环：两层循环

python

```
while True:                          # ── 外层：对话循环
    user_input = input("You:")
    conversation.append(user_input)

    while True:                      # ── 内层：推理/工具循环
        response = execute_llm_call(conversation)
        tool_calls = extract_tool_invocations(response)

        if not tool_calls:           # ① 不需要工具了
            print(response)
            break                    #    跳出内层，等下一句话

        for name, args in tool_calls: # ② 要工具
            result = 执行工具(name, args)
            conversation.append({
                "role": "user",
                "content": f"tool_result({json.dumps(result)})"
            })
            # 回到内层开头，让 LLM 看到结果再继续
```



**一次任务常需多步。**

例：

```
第1圈: LLM → tool: read_file({"filename":"hello.py"})
       程序读文件，结果塞回去
第2圈: LLM 看到内容 → tool: edit_file({"path":"hello.py", ...})
       程序改文件，结果塞回去
第3圈: LLM → "已添加乘法函数"（无 tool: 行）
       → 跳出内层
```



**"智能"就体现在：** 内层循环让 LLM 自己决定还需要几步、下一步调什么，直到它认为完成（不再请求工具）才交回控制权。

> `tool_result(...)` 以 `role: "user"` 塞回去——在 API 眼里，"工具结果"就是发给模型的又一条输入。

------

## 八、`execute_llm_call` 里的一个细节

python

```
def execute_llm_call(conversation):
    system_content = ""
    messages = []
    for msg in conversation:
        if msg["role"] == "system":
            system_content = msg["content"]   # 单独拎出来
        else:
            messages.append(msg)

    response = claude_client.messages.create(
        model="...",
        system=system_content,                # ← 作为独立参数
        messages=messages
    )
    return response.content[0].text
```



**为什么单独拎 system？** Anthropic API **不允许** `messages` 里出现 `role: "system"`——必须作为顶层 `system=` 参数。OpenAI 则允许放 messages 里。这是适配不同 provider 最容易被坑的点。

---

每轮工具调用都往 `conversation` 里塞内容，长任务会爆 token。生产工具做**上下文压缩**（摘要旧消息、只保留相关片段）。

---

`old_str` 找不到就失败

python

```
if original.find(old_str) == -1:
    return {"action": "old_str not found"}   # 直接失败
```

LLM 常因空格/缩进对不上而失败。生产工具会：归一化空白 → 模糊匹配 → 实在不行报错重试。

---

 没有安全护栏

`edit_file` 可覆盖任何文件，`path` 可传 `../../etc/passwd`。生产工具做**路径白名单、破坏性操作审批**。

---

## **三个工具（读/列/改）+ 一段自动生成的工具清单 + 两层循环 ≈ 200 行。剩下的 99% 工程量，都是在健壮性（错误处理、上下文管理、安全、流式）上打磨，而不是换架构。**
https://chat.deepseek.com/share/7wkwobrchfhrpp9l8z    deepseek交流
