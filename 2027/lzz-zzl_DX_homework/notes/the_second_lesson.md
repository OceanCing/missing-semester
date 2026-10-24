## 编译流程
GCC编译器驱动程序读取源程序文件，并把它翻译成一个可执行目标文件hello。这个翻译过程分为四个阶段：
- 预处理（Preprocessing）、
- 编译（Compilation）、
- 汇编（Assembly）、
- 链接（Linking）。
执行这四个阶段的程序（预处理器、编译器、汇编器、和链接器）一起构成了编译系统。



## cmake
CMake 是一个跨平台的构建系统生成工具，用于管理 C/C++ 项目的编译过程。

通过编写 CMakeLists.txt 配置文件，你可以定义项目的构建规则、依赖关系和编译选项，CMake 会自动生成对应平台的构建文件（如 Unix 下的 Makefile 或 Windows 下的 Visual Studio 解决方案）。

## 功能和对应的语法
#### 指定cmake最低版本
```
# 语法：cmake_minimum_required(VERSION <version>)
# 该指令必须放在 CMakeLists.txt 的最顶部
# 示例：要求 CMake 版本不低于 3.10
cmake_minimum_required(VERSION 3.10)
```
#### 打印
```
message(STATUS "普通信息")
message(WARNING "警告信息")
message(FATAL_ERROR "致命错误，立即终止配置")
```

#### 定义项目
```
project(MyProject CXX)
```
后续用到项目时有参数


#### **添加可执行文件：

```
# 语法：add_executable(<目标名> <源文件>...)  
# 将指定的源文件编译生成一个可执行文件  
add_executable(MyApp main.cpp utils.cpp)
```
可以放不包含main的源文件，但必须有一个main函数
#### 添加文件到 一个库<目标名>，并添加到工程
```
# 语法：add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)  
# STATIC：静态库（.a / .lib），编译时直接嵌入可执行文件  
# SHARED：动态库（.so / .dll），运行时加载  
# 不指定类型时，由 BUILD_SHARED_LIBS 变量决定  
add_library(MyLib STATIC library.cpp)#把文件加入Mylib

#要使用这个库，就链接到项目
target_link_librries(<项目名> PRIVATE <库名>)
```
###### 参数\[STATIC | SHARED | MODULE]
##### STATIC（静态库）
- **作用**：将源文件编译后打包成一个归档文件（如 `.a` 或 `.lib`）。
- **使用方式**：在构建可执行文件时，库的代码会被**直接嵌入**到最终的中
- **特点**：程序运行时不需要该库文件存在，部署简单，但每个使用它的程序都会有一份独立的代码拷贝。
##### SHARED（动态库）

- **作用**：生成动态链接库（如 `.so` 或 `.dll`）。
- **使用方式**：可执行文件在链接时只记录依赖关系，**运行时才加载**库代码。
- **特点**：多个程序可共享同一份库代码，节省内存和磁盘空间。但部署时需要确保目标机器上有对应的库文件。CMake 会自动为这类库开启位置无关代码（PIC）。
##### MODULE（模块库）
- **作用**：生成**插件**（如 `.so` 或 `.dll`）。
- **核心区别**：它**不能被其他目标（如可执行文件或其他库）链接**，专门设计为在运行时通过 `dlopen` 或 `LoadLibrary` 等方式**动态加载**。
- **典型场景**：用于插件系统，或作为 Python、JavaScript 等语言的运行时扩展模块。

#### 设置变量
```
# 语法：set(<变量名> <值>...)  
# 定义一个普通变量，后续通过 ${变量名} 引用  
set(CMAKE_CXX_STANDARD 17)  
  
# 同时设置多个值（列表）  
set(SOURCES main.cpp utils.cpp helper.cpp)  
add_executable(MyApp ${SOURCES})
```
常用的
```
set(CMAKE_CXX_STANDARD 17)   #cpp版本
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # 是否强制使用标准
```
#### 若引用变量
```
${<变量名>}
```


#### 定义头文件搜索路径
```
include_directories()
target_include_directories()
```

### `PRIVATE`、`PUBLIC`、`INTERFACE`

这三个关键字控制的是**某个属性（通常是头文件路径、编译选项、链接库）是否传递给依赖它的其他目标**。你问的 `PRIVATE` 和 `PUBLIC` 是最常用的两个。

CMake 把目标的使用需求分成两类：
- **构建需求（Build requirements）**：编译 B 本身时需要的（如 B 的私有头文件路径）。
- **使用需求（Usage requirements）**：别人使用 B 时需要的（如 B 对外暴露的头文件路径）。
### 因此可以用三个关键字选择

| 关键字         | 含义          | 对“自己”生效 | 对“使用者”生效 |
| ----------- | ----------- | ------- | -------- |
| `PRIVATE`   | 仅自己用        | 是       | x        |
| `PUBLIC`    | 自己和用自己的人都用  | 是       | 是        |
| `INTERFACE` | 仅使用者用（自己不用） | x       | 是        |


## 查找库和包

很多项目依赖外部库（如 Boost、OpenCV），CMake 提供了 find_package() 指令来自动检测和配置这些外部依赖。

### find_package 指令

find_package() 会搜索系统中已安装的库，并设置对应的变量（如头文件路径、库文件路径、库目标名称）。

##### 实例

```
# 基本用法：查找 Boost 库（不限定版本）  
# REQUIRED 表示如果找不到则中止 CMake 配置  
find_package(Boost REQUIRED)  
  
# 指定最低版本：要求 Boost 版本不低于 1.70  
find_package(Boost 1.70 REQUIRED)  
  
# 指定搜索路径：在自定义路径下查找 OpenCV  
find_package(OpenCV REQUIRED PATHS /path/to/opencv)  
```

### 使用查找到的库

find_package() 成功后，通常会提供以下内容供使用：

|提供的内容|示例|说明|
|---|---|---|
|导入目标|`Boost::Boost`|现代 CMake 推荐方式，直接通过 target_link_libraries 使用|
|头文件路径变量|`${Boost_INCLUDE_DIRS}`|旧式用法，配合 include_directories 使用|
|库文件路径变量|`${Boost_LIBRARY_DIRS}`|旧式用法，配合 link_directories 使用|

## 进阶
#### project()
**设置内置变量**：CMake 会自动生成一系列以项目名命名的变量，方便后续引用。
```
project(cpp_learn)   #cpp_learn为函数名
#将生成
 用内置变量指定头文件目录
# ${cpp_learn_SOURCE_DIR} 会自动替换为 /home/xxx/cpp_learn
target_include_directories(app PRIVATE ${cpp_learn_SOURCE_DIR}/include)
# 用内置变量指定源文件路径
add_executable(app ${cpp_learn_SOURCE_DIR}/src/main.cpp)
```


#### 判断
```
if(<键> <值>)
	message(STATUS "A")
	
elseif(<键> <值>)

else()
endif()
```
#### 拆分路径

| `get_filename_component(<var> <path> NAME)`      | 获取文件名（含扩展名）    |
| ------------------------------------------------ | -------------- |
| `get_filename_component(<var> <path> NAME_WE)`   | 获取文件名（不含扩展名）   |
| `get_filename_component(<var> <path> EXT)`       | 获取文件扩展名        |
| `get_filename_component(<var> <path> DIRECTORY)` | 获取所在目录路径       |
| `get_filename_component(<var> <path> ABSOLUTE)`  | 获取绝对路径         |
| `get_filename_component(<var> <path> REALPATH)`  | 获取真实路径（解析符号链接） |
|                                                  |                |


#### 列表遍历
```angular2html xml
foreach(HEADER ${HEADERS})
get_filename_component(HEADER_DIR ${HEADER} DIRECTORY)
list(APPEND INCLUDE_DIRS ${HEADER_DIR})
endforeach()

list(REMOVE_DUPLICATES INCLUDE_DIRS)
```





## 在学cmake时使用AI
先提前了解了大体概念和作用
在链接库时提前遇到了private,public,INTERFACE,提前使用AI询问用法

在发现add_executable，target_link_librries有同样作用后（实则不然）问AI区分用途

更详细询问AI一些使用细节（具体挺多的

## 对使用AI学习的理解
我比较激进的认为，从头到尾一步步学肯定不是效率最高的学习方法，
特别是在一些概念相互交叉的知识学习上，了解大概后学会舒服很多，而且不容易被消磨学习热情
而AI可以一定程度上让我们可以不用按照顺序更，并且快速获得知识间的联系

然后AI就是对学习资源获取肯定会有提升

#### 举个例子
在学现代控制理论学理论学累了想仿真试一试lqr，此时需要其他知识：
- matlab，simulink ,simspace使用
- 理论力学

我可以不用看几十个小时的matlab使用,用AI快速知道目前要的知识（simspace的大致使用）然后让AI帮我写一份.m脚本
遇到不理解的就接着问

效果就是：我快速加深对理论的理解，并且了解了进一步学习需要补充的知识

在学习ROS时也有差不多的感受，因为ROS的概念还是挺多的，刚开始打开一个文件夹全是不认识的东西，但是通过AI就能知道各文件夹和文件类型的作用，
学的时候也会对进度更清晰，而且按照理解学明白一个东西会比被教程牵着走更有成就感