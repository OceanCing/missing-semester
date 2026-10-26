#第二次培训笔记：

##1.程序编译的整体流程：
    1.从我们写下的编程语言到电脑能执行的可执行目标文件这个翻译过程分为4个步骤：预处理（Preprocessing）、编译（Compi     lation）、汇编（Assembly）、链接（Linking）
    2.预处理阶段有以下任务（最终会得到.i文件）：
        1.预处理器（cpp）将所有的#define删除，并且展开所有的宏定义。
        2.处理所有的条件预编译指令，比如#if、#ifdef、#elif、#else、#endif等。
        3.处理#include预编译指令，将被包含的文件直接插入到预编译指令的位置。
        4.删除所有的注释。
        5.添加行号和文件标识，以便编译时产生调试用的行号及编译错误警告行号。
        6.保留所有的#pragma编译器指令，因为编译器需要使用它们。
        7.使用gcc -E hello.c -o hello.i命令来进行预处理， 预处理得到的另一个程序通常是以.i作为文件扩展名。
        总结：预处理会将<iostream>这种头文件完整的复制进.i文件......为编译做好准备

    3.编译包含以下内容：
        1.编译器（ccl）将预处理完的文本文件hello.i进行一系列的词法分析、语法分析、语义分析和优化，翻译成文本文件hel          lo.s，它包含一个汇编语言程序。
        2.汇编语言是非常有用的，因为它为不同高级语言的不同编译器提供了通用的输出语言
        3.编译有以下步骤：
            词法分析：扫描器（Scanner）将源代的字符序列分割成一系列的记号（Token）。lex工具可实现词法扫描。
            语法分析：语法分析器将记号（Token）产生语法树（Syntax Tree）。yacc工具可实现语法分析(yacc: Yet Another                       Compiler Compiler)。
            语义分析：静态语义（在编译器可以确定的语义）、动态语义（只能在运行期才能确定的语义）。
            源代码优化：源代码优化器(Source Code Optimizer)，将整个语法书转化为中间代码（Intermediate Code）（中间                         代码是与目标机器和运行环境无关的）。中间代码使得编译器被分为前端和后端。编译器前端负责产生                         机器无关的中间代码；编译器后端将中间代码转化为目标机器代码。
            目标代码生成：代码生成器(Code Generator).
            目标代码优化：目标代码优化器(Target Code Optimizer）。
        4.汇编：
            汇编器（as）将hello.s翻译成机器语言指令，把这些指令打包成一种叫做可重定位目标程序的格式，并将结果保存在            目标文件hello.o中，hello.o是一个二进制文件。
        5.链接：
            链接（linking）是将各种代码和数据片段收集并组合成为一个单一文件的过程，这个文件可被加载（复制）到内存并            执行。链接可以执行于编译时（compile time），也就是在源代码被翻译成机器代码时；也可以执行于加载时（load             time），也就是在程序被加载器（loader）加载到内存并执行时；甚至执行于运行时（run time），也就是由应用程             序来执行。链接是由叫链接器（linker）的程序自动执行的。

##2.CMake:
    1.定义：
        CMake 是一个跨平台的构建系统生成工具，用于管理 C/C++ 项目的编译过程。
        通过编写 CMakeLists.txt 配置文件，你可以定义项目的构建规则、依赖关系和编译选项，CMake 会自动生成对应平台的         构建文件（如 Unix 下的 Makefile 或 Windows 下的 Visual Studio 解决方案）。
    2.个人总结：
        相当于一个帮你管理各个程序编译的工具，提升效率

##3.agent（大模型帮你写程序的工具包）ps:英文文章，理解可能不太准确
    三个agent工具：1.read files 2.list files 3.edit files
    分别用于1.让ai读懂我的代码，2.看懂我的项目，包括目录，3.编辑我的文件
    总之就是赋予ai控制编辑我电脑中各种程序、文件的能力

##4.catkin：
    ai总结：简单来说，Catkin 就是 ROS 的“项目管家”和“构建工具”。它负责帮你把写的机器人代码组织好、编译好，并管理不     同代码模块之间的依赖关系。
    ###创建一个catkin workplace：
        $ mkdir -p ~/catkin_ws/src
        $ cd ~/catkin_ws/
        $ catkin_make
    然后会生成3个文件夹：src/（用来我写代码）、build/（缓存和中间文件）、devel/（编译成功后的文件）补充：devel/文件     夹：编译后你需要 source 这个文件夹里的脚本，才能运行你新写的程序
    ###使用catkin：
        在src/中不是只写单纯的c++程序，而是将代码组织成package，其中包含：
            package.xml（描述这个包的名字、版本、作者，以及它依赖哪些其他的包）ai:身份证
            CMakeLists.txt(告诉 Catkin 如何编译这个包：要编译哪些源文件、生成什么可执行文件、链接哪些库) ai:施工图
        创建工作空间并初始化(同上）：
            mkdir -p ~/catkin_ws/src    # 创建你的工作空间和代码空间
            cd ~/catkin_ws/
            catkin_make                 # 首次运行会完成初始化，并生成 build 和 devel文件夹
        创建功能包：
            cd ~/catkin_ws/src
            catkin_create_pkg my_robot_pkg std_msgs roscpp rospy
            # 这会创建一个名为 my_robot_pkg 的文件夹，并自动生成 package.xml 和 CMakeLists.txt
        编译让环境生效：
            cd ~/catkin_ws/
            catkin_make                 # 编译 src 下的所有包
            source devel/setup.bash     # 让当前终端“认识”你刚编译出来的程序
