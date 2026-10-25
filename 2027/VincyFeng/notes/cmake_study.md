# CMake、catkin学习笔记

## CMake

1. CMake是跨平台构建生成工具，它本身不直接编译代码，读取 CMakeLists.txt 配置文件，生成对应平台的底层构建脚本（Linux生成Makefile，Windows生成VS工程），再交由make、ninja等工具完成编译链接。

2. CMake语法骨架

   1. `cmake_minimum_required(VERSION 3.10)` ***必须置顶***

   2. `project(MyProject CXX)`定义项目名、编程语言；自动生成 PROJECT_NAME 、 PROJECT_SOURCE_DIR 等内置变量

   3. 添加静态库目标`add_library(MyLib STATIC src/lib/core.cpp#核心功能实现 src/lib/helper.cpp#辅助函数实现)` 添加可执行文件目标`add_executable(MyApp main.cpp#程序入口 utils.cpp#工具函数)`

   4. 给指定目标设置头文件路径`target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)`，支持 `PRIVATE/PUBLIC/INTERFACE` 控制依赖传播，不会全局污染。为目标链接库`target_link_libraries(MyApp PRIVATE MyLib)`，同样有三个可见性修饰符。

      | 关键字    | 当前目标可用 | 依赖此目标的其他目标可用 | 典型场景                                |
      | --------- | ------------ | ------------------------ | --------------------------------------- |
      | PRIVATE   | 是           | 否                       | 仅自身实现需要的内部头文件              |
      | PUBLIC    | 是           | 是                       | 库的公开 API 头文件，自身和消费者都需要 |
      | INTERFACE | 否           | 是                       | 仅头文件的库（Header-only library）     |

   5. 典型构建命令：

      在项目根目录下创建 build 目录（推荐外部构建）`mkdir build && cd build`

      生成构建文件 `cmake .`

      编译项目 `cmake --build`

      安装到系统目录（可选） `cmake --install`

## catkin

1. Catkin不是独立编译工具，是ROS1在CMake之上封装的构建系统，用来管理ROS功能包工作空间，对原生CMake做了一层规范与扩展，底层依旧调用CMake+make完成编译。
2. catkin 项目的 CMakeLists.txt 完全符合标准 CMake 格式，只是**多了几个 catkin 专属指令**，整体结构大致是：
   1. `cmake_minimum_required()` #CMake 最低版本
   2. `project()` #包名
   3. `find_package(catkin REQUIRED COMPONENTS ...)`  #找依赖
   4. `add_message_files()` / `add_service_files()`  #ROS 特有：声明 msg/srv
   5. `generate_messages()`  #ROS 特有：生成消息头文件
   6. `catkin_package()`   #ROS 特有：导出包信息给下游
   7. `add_executable()` / `add_library()` / `target_link_libraries()`
   8. `install()`
3. ***ROS 1*** 的最后一个发行版 Noetic 已于 2025 年 5 月 31 日正式停止维护，catkin 仓库***也已归档***。学 catkin 依然是理解构建系统思想的好素材，但新项目应转向 ***ROS 2*** 的 ***ament/colcon***。

## 使用大模型辅助学习CMake/Catkin：模仿编码Agent的交互思路

1. 阅读Mihail Eric文章之后理解：AI编码Agent本质不是“魔法”，核心就是一套循环：用户提问 → LLM生成工具调用 → 本地执行读文件/修改文件 → 将结果返回大模型 → LLM继续思考输出，只需要200行左右Python就可以实现最简版本的编码Agent，工具主要就是读文件、列目录、编辑文件三个基础能力。
2. 把自己当作Agent循环里的“本地环境”，大模型是LLM大脑，自己执行“工具调用”（读文档、写文件、运行编译、看报错），再把执行结果丢回给大模型，迭代加深理解。可能的交互流程如下：
   1. 概念提问（对应Agent获取任务）：用通俗语言解释CMake和catkin的关系，区分哪些是CMake原生命令，哪些是catkin新增宏；不要直接贴完整代码，解释每个指令的作用。拿到概念解释后，自己对照教程文档核对，标记不懂的点。
   2. 小Demo分步生成（对应Agent调用edit_file工具）：不要一次性生成完整大工程，要求大模型分步输出片段，每输出一小段就自己手写进 CMakeLists.txt ，而不是直接复制全部。分步写一个最小catkin功能包CMakeLists，只写第一步到find_package，解释每一行；不要输出完整成品。
   3. 本地执行构建，收集报错（对应Agent执行工具拿到tool_result）：手动执行 catkin_make 或者cmake编译，把完整报错日志复制给大模型，这一步是关键。
   4. 对比辨析，深挖坑点（模拟多次工具迭代）：主动向大模型提出对比问题，理清易错点，例如：`include_directories` 和 `target_include_directories` 在catkin包里面分别会产生什么后果？`catkin_package()`放在`add_executable`前面还是后面，为什么？`PUBLIC PRIVATE INTERFACE`在ROS catkin包里面分别适合什么场景？
   5. 自我验证，做修改实验：拿到解释之后，手动修改配置，故意制造错误，观察编译行为，验证自己是否真懂。比如故意把PRIVATE改成PUBLIC，看下游包能不能访问到头文件。
3. 警惕大模型的坑
   1. 限制大模型访问与目标项目无关的文件，以防止重要文件泄露或者个人隐私泄露，造成重大损失。
   2. 大模型生成的代码文件，个人需要学习理解、并且调试没有问题才上传，否则出问题都是个人承担责任，而大模型无需承担责任。
