Cmake:
为什么用CMake
   随着项目文件和依赖增加，手写维护Makefile规则变得极其繁琐且不跨平台。CMake是一个跨平台的编译工具，属于过程式语言，通过简单的语句描述所有平台的编译过程。
CMake的工作原理
   CMake本身不直接编译代码，而是通过编写简单的配置文件（CMakeLists.txt），去生成本地标准的建构档（如Makefile或Project文件）。
 常用指令
cmake_minimum_required：指定最低版本。
project：定义项目名称。
add_executable：将源文件编译为可执行文件。
add_library：创建库文件。
target_link_libraries：将库链接到目标文件。
target_include_directories：指定头文件搜索路径。

CMake 的底层逻辑其实是为了生成 Makefile Makefile 就是运行 cmake 命令后自动生成的产物
CMake 是一个跨平台的构建工具。它本身不编译代码，而是通过读取 `CMakeLists.txt` 文件，去生成特定平台下的 Makefile
· 如果在 Linux 上敲 cmake .，它生成的是 Makefile（给 make 命令看）。
· 如果在 Windows 上敲 cmake .，它生成的是一个 .sln 文件（给 Visual Studio 的 MSVC 编译器看）。
· 如果在 苹果 Mac 上敲，它可能会生成 Xcode 工程文件。

Catkin:
catkin 是 ROS 基于 CMake 封装出来的专属构建系统。它专门用来管理复杂的机器人功能包。
catkin与CMake的关系
   catkin是ROS对CMake的扩展封装，本质上就是帮我们把cmake和make指令封装起来，形成了一套适配ROS的工作流规范。
   工作空间结构（Workspace）

src：代码空间，存放源码包（package）。
build：编译空间，存放cmake和catkin的缓存及中间文件（一般不需要看）。
devel：开发空间，存放编译生成的可执行文件和库。

大模型辅助学习:
codex内配置多种不同模型
Pi上运行大模型或品牌自有大模型表现较好

流程相互的关联
用vim编写好cmake list.txt 然后输入cmake 生成一个makefile 然后再输入 Make 生成一个demo
1.写代码：手敲一个 main.cpp（以及 add.cpp 等）。
2.写图纸：手写一个 CMakeLists.txt（告诉机器要编译哪些文件，生成什么名字的程序）。
3.生成排班表：敲命令 cmake .（生成 Makefile）。
4.开始施工：敲命令 make（自动调用 g++ 编译）。
5.运行程序：敲命令 ./demo（看到结果）
写 CMakeLists.txt（写图纸） ➡️ 敲 cmake .（生成 Makefile） ➡️ 敲 make ➡️ 自动调用 g++ ➡️ 生成程序。
(windows下载的诸如Clion VScode内部都配置了编译器)

本质（使用ai生成对其形象的比喻，加深理解）
1.g++：是干活的工人（编译器）它真正把cpp文本变成机器码
2.Makefile：是排班表。它写明了工人（g++）怎么按顺序干活
3.make：是工头。你敲一下 make，工头就会拿着排班表（Makefile）去指挥工人（g++）干活。
4.CMake：是总包工头（生成器）。随着项目跨平台，手写 Makefile 太痛苦了。CMake 让你写一个更简单的 CMakeLists.txt，然后 CMake 会自动帮你生成一份对应系统的 Makefile


CMakeLists.txt package的编译规则(必须)
package.xml package的描述信息(必须)
src/ 源代码文件
include/ C++头文件
scripts/ 可执行脚本
msg/ 自定义消息
srv/ 自定义服务
models/ 3D模型文件
urdf/ urdf文件
launch/ launch文件
include/ c++头文件

LLM
（large language model）
上下文窗口:输入输出的最大文本长度
Agant
大模型使用工具进行自动操作完成任务

ps.合理使用ai
不能过度依赖AI直接生成代码和配置，必须自己搞懂底层的编译逻辑，才能自己排查报错




