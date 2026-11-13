**我使用ChatGPT辅助我的学习，我将在笔记中同步展示我学习中使用AI辅助的过程（虽然这句话我读起来很像AI生成的，但是真的是我写的)**
# 1.CMake
> 我先自己阅读三个有关CMake和catkin的网址，大概理解他们的内容，然后再发给AI，让其帮我整理出其中的重点信息，提取成一份md文件出来。

## 1.1 程序的详细编译过程（CMake的引入）

- 预处理：展开头文件和宏
- 编译：生成汇编.s
- 汇编：生成目标文件.o（即二进制代码）
- 链接：加入标准库，生成可执行的文件
- 运行：输入后输出计算结果

也就是说，CMake可以控制构建怎么运行整个程序，可称为“通用构建项目管家”。
在CMake中，程序编译过程为：
1.根据CMakeLists.txt(项目说明书，通常由作者或项目模板产生，catkin中由模板生成，再经过作者手动编辑）生成Makefile（告诉Make先编译哪个文件、再链接哪个文件）和Make/Ninja（真正调用编译器和链接器）
2.Make/Ninja调用g++（C++编译器）
3.链接所需的库，生成可执行的文件。

## 1.2 示例

- 先创建CMakeLists.txt
   `cmake_minimum_required(VERSION 3.16)`
   `project(circle_calc LANGUAGES CXX)`
   `add_executable(circle_calc example.cpp)`
- 配置
   `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`

在这一步中，CMake会：检查C++编译器；检查编译环境；读取CMakeLists.txt；在build/中生成Makefile或Ninja文件。*（此时还没真正编译）*

- 然后构建：
   `cmake --build build --verbose`

- 运行：
  `./build/circle_calc`

- 各目录的含义：
example.cpp
CMakeLists.txt
build/
├── CMakeCache.txt
├── Makefile
├── CMakeFiles/
│   └── circle_calc.dir/
│       └── example.cpp.o
└── circle_calc

## 1.3 CMakeLists.txt(在菜鸟教程中学习）

- CMakeLists.txt 是 CMake 的核心配置文件，每个 CMake 项目至少需要一个 CMakeLists.txt 文件
- CMake 通过读取文件中的指令来了解项目的结构、源文件列表和编译要求

下面是CMakeLists.txt的基础语法
> 其由一系列的CMake指令组成，每个指令的格式为：命令名（参数列表）
### 1.3.1 指定CMake最低版本
 `#语法：cmake_minimum_required(VERSION <version>)`
 `#该指令必须放在 CMakeLists.txt 的最顶部`
 `#示例：要求 CMake 版本不低于 3.10`
 `cmake_minimum_required(VERSION 3.10)`
### 1.3.2 定义项目名称和语言
 `#语法：project(<项目名> [<语言>...])`
 `#语言参数可选，常用值：CXX（C++）、C（C 语言）`
 `#调用 project() 后会自动设置 PROJECT_NAME 等变量`
 `project(MyProject CXX)`
### 1.3.3 添加可执行文件
 `#语法：add_executable(<目标名> <源文件>...)`
 `#将指定的源文件编译生成一个可执行文件`
 `add_executable(MyApp main.cpp utils.cpp)`
### 1.3.4 添加库文件
 `#语法：add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)`
 `#STATIC：静态库（.a / .lib），编译时直接嵌入可执行文件`
 `#SHARED：动态库（.so / .dll），运行时加载`
 `#不指定类型时，由 BUILD_SHARED_LIBS 变量决定`
 `add_library(MyLib STATIC library.cpp)`
### 1.3.5 链接库到目标
 `#语法：target_link_libraries(<目标> <库>...)`
 `#将指定的库链接到目标（可执行文件或其他库）`
 `#可以链接自己项目中的库目标，也可以链接外部库`
 `target_link_libraries(MyApp PRIVATE MyLib)`
### 1.3.6 设置变量
 ```
 #语法：set(<变量名> <值>...)
 #定义一个普通变量，后续通过 ${变量名} 引用
 set(CMAKE_CXX_STANDARD 17)

 #同时设置多个值（列表）
 set(SOURCES main.cpp utils.cpp helper.cpp)
 add_executable(MyApp ${SOURCES})
 ```
### 1.3.7 为目标指定头文件路径
 ```
 #语法：target_include_directories(<目标>
            [BEFORE | AFTER]
            [SYSTEM]
            [PUBLIC | PRIVATE | INTERFACE]
            <路径>...)
 #PUBLIC：当前目标和依赖它的目标都能使用该路径
 #PRIVATE：只有当前目标能使用
 #INTERFACE：只有依赖当前目标的其他目标能使用
 target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
 ```
### 1.3.8 设置安装规则
 ```
 #语法：install(TARGETS <目标>...
 #        [RUNTIME DESTINATION <可执行文件安装路径>]
 #        [LIBRARY DESTINATION <动态库安装路径>]
 #        [ARCHIVE DESTINATION <静态库安装路径>]
 #        [INCLUDES DESTINATION <头文件安装路径>])
 #定义执行 make install 时各类型文件的安装位置
 #install(TARGETS MyApp RUNTIME DESTINATION bin)
 ```
### 1.3.9 条件语句
 ```
 #语法：if(<条件>) ... elseif(<条件>) ... else() ... endif()
 #CMake 支持的条件表达式包括：比较、逻辑运算、变量判断等
 if(CMAKE_BUILD_TYPE STREQUAL "Debug")
     #Debug 模式下启用调试信息
     message(STATUS "当前为 Debug 构建模式")
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
 else()
     #Release 模式下启用优化
     message(STATUS "当前为 Release 构建模式")
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
 endif()
 ```
### 1.3.10 自定义命令
 ```
 #语法：add_custom_command(
 #         TARGET <目标>
 #         PRE_BUILD | PRE_LINK | POST_BUILD
 #         COMMAND <命令> [参数...]
 #         [COMMENT <说明>]
 #         [VERBATIM])
 #PRE_BUILD：在编译前执行
 #PRE_LINK：在链接前执行
 #POST_BUILD：在构建完成后执行
 add_custom_command(
     TARGET MyApp POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E echo "构建完成！"
     COMMENT "打印构建完成信息"
 )
 ```

## 1.4 头文件搜索路径
|  | include_directories | target_include_directories |
| --- | --- | --- |
| 作用范围 | 之后的所有目标（全局） | 只有作用于指定的目标 |
| 依赖范围 | 无 | 有（PUBLIC/PRIVATE/INTERFACE）|
| 现代CMake | 不推荐新项目使用 | 推荐 |

## 1.5 总结
总的来说，CMake自己并不翻译C++，只是决定：
- 使用哪个编译器
- 加什么参数
- 哪些源文件属于哪个程序
- 生成哪些.o文件
- 最后将哪些文件链接在一起
**真正的编译和链接工作仍然由g++、ld等工具完成。**

## 1.6 现代CMake的target概念(在交互式问答中增加的知识点，菜鸟教程里面有，一开始没有仔细阅读）：
PUBLIC/PRIVATE/INTERFACE
| 写法 | 当前目标使用 | 链接者继承 |
|---|---:|---:|
| `PRIVATE` | 是 | 否 |
| `PUBLIC` | 是 | 是 |
| `INTERFACE` | 否 | 是 |

# 2.catkin
> 学习catkin的过程，我更多使用交互式问答在与AI交流过程中学习
**catkin是ROS1用来统一管理这些项目的工具，负责找到多个包、读取依赖、安排构建顺序，然后调用CMake和Make**

## 2.1 一个包中需要两份描述文件：
- package.xml: 说明这个包是什么和它需要什么；包名、版本和依赖什么
- CMakeLists.txt：说明怎么把它的源码构建出来；源文件，目标、链接和安装规则

## 2.2 catkin的工作空间：
*catkin工作空间就是一个大文件夹，里面可以放很多包*
catkin_ws/
├── src/
│   ├── circle_demo/
│   │   ├── package.xml
│   │   ├── CMakeLists.txt
│   │   └── src/circle.cpp
│   └── sensor_demo/
├── build/
├── devel/
└── install/
下面解释目录中各个目录的职责
| 目录 | 主要内容 |
|---|---|
| `src/` | 源码、`package.xml`、`CMakeLists.txt` |
| `build/` | Makefile、CMake 缓存、`.o` 文件、链接文件 |
| `devel/` | 编译好的程序、库和 `setup.bash` |
| `install/` | 安装后的最终文件 |

- `src/` 放源码和每个包
- `build/` 放 CMake 配置、.o 文件和链接信息
- `devel/` 放最终生成、开发时可以立即使用的程序
- `install/` 只有执行 catkin_make install 后才真正重要
- `devel/setup.bash` 不是程序本身，而是告诉终端去哪里找刚编译好的 ROS 包

## 2.3 流程

### 2.3.1 创建源码目录
 ```
 mkdir -p ~/catkin_ws/src
 cd ~/catkin_ws/src
 catkin_create_pkg circle_demo roscpp std_msgs
   # 创建包：第一个参数是包名，后面是依赖列表
 cd ~/catkin_ws
 ```
### 2.3.2 构建
   `mkdir -p ~/catkin_ws/src`
   `cd ~/catkin_ws`
catkin_make大概会做这些事情：
- 在`src/`中寻找ROS包
-  读取每个包的 package.xml
-  计算哪些包应该先编译
-  **对整个工作空间只做一次 CMake 配置**（所有包并入同一个 CMake 工程）——不是每个包各调一次，这是它与 catkin build 的核心区别
-  调用 Make 和 g++完成编译和链接
-  把可执行文件放进 devel/lib/包名/
-  生成 devel/setup.bash
### 2.3.3 加载工作空间
   `source devel/setup.bash`  (这一步是修改当前终端的搜索路径）
> source只是修改当前这个终端的环境，不会修改以后新开的终端，因此，新开终端之后应该先加载ROS和工作空间环境
### 2.3.4 运行程序
   `rosrun circle_demo circle_calc`

## 2.4 一些细节（在交互式问答中逐渐完善）

### 2.4.1 catkin的构建
- catkin构建大概会生成两类文件：
`build/`
   `circle.cpp.o`
   `Makefile`
   `CMakeCache.txt`

`devel/`
   `lib/circle_demo/circle_calc`
   `setup.bash`

- 其中：
| 文件 | 作用 | 是否通常能直接运行 |
|---|---|---|
| `circle.cpp.o` | 编译后的目标文件，仍在等待链接 | 不能 |
| `circle_calc` | 链接完成的最终可执行程序 | 能 |
| `setup.bash` | 告诉终端到哪里寻找包和程序 | 不是程序 |

- 因此，构建的流程为：
1.`catkin_make`
2.`build/` 里生成 .o
3.链接成可执行程序
4.复制或链接到 `devel/lib/circle_demo/`
5.`source devel/setup.bash`
6.`rosrun circle_demo circle_calc`

### 2.4.2 catkin的关键目录关系

- 源码      在 src/
- 中间文件  在 build/
- 最终程序  在 devel/lib/包名/
- 运行环境  由 devel/setup.bash 加载

# 3.皇帝的新衣
> 编程智能体本质上就是和一个拥有工具箱的强大LLM对话，不断的循环判断
1.我发一条消息（「新建一个文件，里面写个 hello world 函数」）
2.LLM 判断自己需要某个工具，于是回复一个结构化的工具调用（可能是一次或多次）
3.我的程序在本地执行这个工具调用（真的把文件创建出来）
4.执行结果被发回给 LLM
5.LLM 基于这个结果继续任务或给出回复
因此，我们需要三个工具
- 读文件：让LLM能看到代码
- 列文件：让LLM能浏览项目
- 编辑文件：让LLM下达创建和修改代码的指令

- 于是，我们并不能过度神话依赖Agent，它的原理就只是不断的循环调用上述的三个工具，以及对话而已，但是截至目前为止（10.25）我还没有仔细将文中的代码实现出来，我将在这几天在AI的帮助下搭建出一个比较简易的Agent再提交到github。

# 4.总结

## 4.1 AI通过交互式问答对于我的掌握程度进行判断，以下是AI的生成表格：
| 模块 | 掌握情况 | 判断 |
|---|---|---|
| C++ 编译流程 | 很好 | 能清晰区分预处理、编译、汇编、链接及 `.o` 文件 |
| CMake 配置与构建 | 较好 | 理解 `cmake -S . -B build` 只生成构建文件，真正编译在 build 阶段 |
| CMake target 依赖 | 初步掌握 | 经提示后理解了 `PUBLIC` 和 `PRIVATE` 的传播区别 |
| `package.xml` 与 `CMakeLists.txt` | 需要巩固 | 曾混淆两者职责，现在知道前者管信息与依赖，后者管构建 |
| catkin 目录结构 | 基础薄弱 | 容易把源码目录 `src/`、构建目录 `build/` 和成品目录 `devel/` 混淆 |
| catkin 完整流程 | 正在建立 | 已经能复述“修改源码 → `catkin_make` → `source` → 运行”的顺序 |
| 排错能力 | 需要练习 | 目前能理解讲解，但暂时还不能独立判断环境和路径问题 |

# 5.对大模型辅助学习的一些思考

在这次学习中，我利用大模型总结出相关的一些重要知识点，然后对其进行比较深度的学习，用交互式问答在问答中巩固知识点，对于一些知识点细节补全。但是我觉得前提是自己应当提前阅读文件的原件，不应该完全依赖AI总结出来的知识点，需要拥有自己的思考。在大模型方面，我使用的大模型为ZCode接入DeepSeek以及ChatGPT接入DeepSeek。
