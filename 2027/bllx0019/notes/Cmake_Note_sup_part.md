# Cmake_Note_supplement

## Cmake Part


### Somthing in Cmake

|文件/目录|作用|协同开发中的提交要求|
|---|---|---|
|`CMakeLists.txt`|项目构建描述，每个目录最多一个|提交|
|`build/`（可改名）|构建目录：缓存、中间文件、产物都在这里|加入`.gitignore`|
|`CMakeCache.txt`|配置阶段产生的缓存变量（`-D` 参数存这里）|不要提交|
|`CMakeFiles/`|CMake 内部规则与中间文件|不要提交|
|`compile_commands.json`|编译数据库，给 clangd/IDE 用（需开启导出）|一般不入库|
|`CMakePresets.json`|预设的配置/构建参数，团队共享|建议提交|
|`cmake/`| 自己写的`*.cmake`模块（如`FindXXX.cmake`） |提交|



### 关于最低版本要求：`cmake_minimum_required`

- `CMakeLists.txt` 的第一条可执行命令必须是：

   ```cmake
   cmake_minimum_required(VERSION 3.20)
   ```

- 它声明“本项目按哪个 CMake 版本来解释”，并让旧版 CMake 直接拒绝配置；
- 新项目通常写 3.20 或更高；
- 也支持区间写法：`cmake_minimum_required(VERSION 3.20...4.4)`，表示`最低要求 3.20，在更新的CMake上按 4.4 的策略执行`，适合需要兼容多版本 CMake 的库
>[!CAUTION] 
>`cmake_minimum_required`写在`project()`之后会导致策略不生效，某些行为会和预期不一致



### 最小项目

目录结构：
```text
hello_world/
├── CMakeLists.txt
└── hello_DX.cpp
```
`CMakeLists.txt`：
```cmake
cmake_minimum_required(VERSION 3.20)            #声明最低版本与策略
project(hello_DX LANGUAGES CXX)              #声明项目名与语言（CXX即C++）
add_executable(hello_DX hello_DX.cpp)     #定义可执行目标hello，源文件hello.cpp
```
在 `hello/` 目录下依次执行：

```bash
cmake -S . -B build                #配置+生成：源码目录`.`，构建目录build
cmake --build build --parallel 8   #构建（-j是并行数，可省略）
./build/hello_DX                #运行
```
>[!TIP]
Cmake项目运行的核心就是`cmake -S . -B build → cmake --build build → 运行产物`三个Part，后面所有内容都是在这三条命令上加参数



### 常用配置参数

|参数|作用|示例|
|----|----|----|
|`-D<变量>=<值>`|设置/覆盖缓存变量|`-DCMAKE_BUILD_TYPE=Release`|
|`-G<生成器>`|指定生成器|`-G Ninja`、`-G "Unix Makefiles"`|
|`-DCMAKE_CXX_COMPILER=...`|指定编译器|`-DCMAKE_CXX_COMPILER=clang++`|
|`-DCMAKE_INSTALL_PREFIX=...`|安装前缀|`-DCMAKE_INSTALL_PREFIX=$HOME/.local`|
|`-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`|生成compile_commands.json|配合 clangd使用|
|`--fresh`|丢弃缓存重新配置（3.24+）|调试“改了没生效”时很有用|
|`--parallel N`|构建时并行（`--build` 的参数）|`cmake --build build --parallel`|

>[!TIP]
> `CMAKE_BUILD_TYPE`只对单配置生成器（Make、Ninja）有效。Visual Studio、Xcode等属于多配置生成器，构建时要写`cmake --build build --config Release`，配置阶段传`-DCMAKE_BUILD_TYPE`不会报错但也不会有什么作用



### What you should attention when you build......

**源外构建（out-of-source）**

```bash
不推荐：在源码目录里直接 cmake .
推荐：单独建 build 目录
```

- 源目录没有生成文件污染，`git status`保持整洁易读易维护
- 可以同时保留多套构建：`build-`
- `debug/`、`build-release/`、`build-clang/`
- 删除build可直接使用`rm -rf build`，不会误删src
>[!CAUTION]
>一旦不小心源内构建（in-source），就要把`CMakeCache.txt`、`CMakeFiles/`、`Makefile`、`cmake_install.cmake`等生成物清理干净，否则后续源外构建构建可能继续用旧缓存



## CMake 语法

CMake 语言可以视作**一条命令 + 一串参数**；真正需要理解的是变量存活、作用域传染和target“可见性”的传递

### 相关语法

```cmake
# 注释用#

# 命令(参数...)，参数用空格或分号分隔
set(SRCS main.cpp util.cpp)      #列表：main.cpp;util.cpp

# 引用变量要加${}
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
|规则|说明|
|---|---|
|命令名大小写|不敏感（`ADD_EXECUTABLE` 等价 `add_executable`），但约定全小写|
|变量名大小写|敏感（`SRCS` 与 `srcs` 是两个变量）|
|参数分隔|空格或分号；列表本质上就是分号分隔的字符串|
|注释|`#`到行尾；`#[[ ... ]]` 为块注释|
|缩进|无语法意义，统一2或4空格或tab即可|

### 变量名与作用域
```cmake
set(APP_NAME demo)                              #普通变量：当前目录及子目录可见
set(SRCS main.cpp util.cpp)                     #列表变量
set(MY_PREFIX "/opt/demo" CACHE PATH "安装前缀")  #缓存变量：写入 CMakeCache.txt
option(ENABLE_TESTS "是否构建测试" ON)            #等价于BOOL类型缓存变量
unset(APP_NAME)                                 #删除普通变量

message(STATUS "APP_NAME=${APP_NAME}")
message(STATUS "HOME=$ENV{HOME}")               #环境变量
```
|类型|定义方式|生命周期|覆盖方式|
|:-----|:--|:--|:--|
|普通变量|`set(VAR value)`|本次配置；当前目录+子目录|在子目录中`set`只影响子作用域；`PARENT_SCOPE`可回写父作用域|
|缓存变量|`set(VAR value CACHE STRING "doc")`|跨多次配置，存在 `CMakeCache.txt`|命令行 `-DVAR=value`；`option()`定义的BOOL变量同理|
|环境变量|`$ENV{VAR}`|进程环境|`set(ENV{VAR} value)`（只影响当前 CMake 进程）|

- 普通变量是目录作用域：子目录能用父目录的变量，父目录看不到子目录新设的变量
- 函数内部是独立作用域，想改外部变量要用`set(VAR value PARENT_SCOPE)`或 `return(PROPAGATE VAR)`（3.25+）
- 缓存变量的默认值不会自动更新：改了`set(... CACHE ...)`的默认值后，必须删除 `CMakeCache.txt`（或 `cmake -U VAR`、`cmake --fresh`）才生效

> 普通变量像“当前目录的局部变量”，缓存变量像“项目级的全局配置”，`-D` 改的是缓存变量。

### 条件与循环

```cmake
if(WIN32)
    message(STATUS "Windows")
elseif(APPLE)
    message(STATUS "macOS")
else()
    message(STATUS "Linux/其他")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

foreach(opt IN ITEMS a b c)
    message(STATUS "opt=${opt}")
endforeach()

while(NOT done)
    # ...
    set(done TRUE)
endwhile()
```

常用判断：
|写法|含义|
|---|---|
|`if(EXISTS path)`|路径是否存在|
|`if(DEFINED VAR)`|变量是否已定义|
|`if(TARGET tgt)`|目标是否已存在|
|`if(VAR STREQUAL "value")`|字符串相等（大小写敏感）|
|`if(VERSION_LESS 3.20)`|版本比较：`VERSION_LESS`/`VERSION_GREATER`/`VERSION_EQUAL`|
|`if(CMAKE_SYSTEM_NAME STREQUAL "Linux")`|系统名判断|
|`if(BUILD_TESTING)`|布尔变量判断|

> 如果变量 `VAR`未定义，`if(VAR)`会把`VAR`当成非空字符串，结果恒为真。判断“有没有定义”用`if(DEFINED VAR)`，判断“值是否为真”先`set(VAR OFF)`给默认值。