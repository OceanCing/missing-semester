##编译的过程
#预处理阶段
- 删掉#define，并且展开宏定义
- 删掉include，展开头文件
- 删除注释
- 处理if，ifdef等指令
#编译阶段
- 词法分析{将关键字(int) 变量名(a,b) 运算符(+-=)等拆分成记号(token)}
- 语法分析(检查语法是否符合规则）
- 语义分析
- 源代码优化(如int a = 2*3,编译器会直接计算出6,让程序更加高效）
- 目标代码生成(生成汇编代码)
- 目标代码优化
#汇编阶段
- 将汇编代码翻译成机器语言指令
#链接阶段
- 将程序中包含的其他函数，变量等符号与源代码链接
- 好处（可以通过链接实现文件的增量更改）

##cmake
#makefile
- 自动编译说明书，可以指出更改过的文件来编译，节约时间，编译输入make
#cmake工具
- 随着文件增加，makefile编写也变复杂，用cmake生成makefile可以减少工作量
- 既能在linux环境下生成makefile也能在windows下生成project可实现两平台互通
- 基本语法
1.指定cmake最低版本
2.定义项目名称和语言（project(add_demo)）
3.添加可执行文件即要编译的文件（add_executable(demo XXX.cpp XXX.cpp)）
4.为目标指定头文件路径（target_include_directories(demo PRIVATE include)）
#普通变量和缓存变量
- 普通变量只在当前配置cmake过程中使用
- 缓存变量会保存到下一次cmake配置过程中
##catkin
- ros系统下的编译工具,同样可以为我们生成cmake文件
