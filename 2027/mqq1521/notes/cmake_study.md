# 第二课笔记
### 简介
> - 对于源代码包，我们只有编译才能在系统上运行。linux下的编译器有gcc、g++。
> - 随着源文件的增加，直接用gcc/g++命令的方式显得效率低下，人们开始用**Makefile**来进行编译.
> - 然而随着工程体量的增大，Makefile也不能满足需求，于是便出现了**Cmake**工具。
> - **CMake**是对**make**工具的生成器，它简化了编译构建过程，具有良好的扩展性。对于ROS来说，就采用的是CMake。
### 从源代码到可执行程序
 - 计算机只看得懂二进制代码（机器语言），所以实际由计算机执行的程序都是二进制构成的
 - 编译器转换我们写的高级语言代码成机器语言，充当翻译官的角色
https://zhuanlan.zhihu.com/p/476697014
##### 具体转换的过程分为以下几步：
1.预处理
2.编译
3.汇编
4.链接
## Makefile
> - **Makefile**如同一份自动编译说明书。它告诉make工具：哪些文件要编译、编译顺序是什么、需要哪些命令。
> - **Makefile**实现了编译程序、构建程序的自动化提高了人们的效率。通过编写Makefile，在我们每次编写修改好代码后，只需要敲一次make命令便能够得到你想要的目标程序。
## Cmake
> - 随着文件和依赖增加，我们需要维护的Makefile规则也会越来越多。单纯靠写makefile也会变得不方便。
> - Cmake是一种过程式语言，是一个跨平台的编译工具，可以用简单的语句来描述所有平台的编译过程。它能够产生标准的建构档makefile或者譬如哦金额词条文件。
> - 单纯靠makefile来维护项目的构建做不到灵活地跨平台，cmake解决了这个问题
> - 我们使用cmake通过编写简单的配置文件去生成本地的Makefile，这个配置文件是独立于运行平台和编译器的，不管在什么平台用什么编译器，cmake都可以生成对应的makefile
> - 语法参考https://www.runoob.com/cmake/cmake-basic.html
## catkin
catkin --helo## 检查catkin是否安装
mkdir catkin_ws
cd/catkin_ws &mkdir src
cd src
catkin_init_workspace ## 初始化工作空间
cd ..
catkin build
## package功能包
[zhihu](https://zhuanlan.zhihu.com/p/621598025)
## 大模型（LLM）
大语言模型（large language models）是一种有大量参数的人工智能模型
LLM可以被看作是给定提示字符串（输入）的补全字符串（输出）概率分布进行建模。LLM推理（即向对话聊天应用提交查询时发生的事情）会从这个概率分布中采样。
LLMLLM具有固定的上下文窗口，即输入和输出字符串的最大长度。
## Agent软件
Agent软件就是一套让大模型能够使用工具、与外部环境交互，并持续执行任务的系统。
当你在自己的电脑上面装了一个agent软件并接入了大模型，大模型就可以利用agent软件里提供的工具和你的电脑交互（删改读写文件），帮助你做事情。
### 用大模型帮助开发
可用**skill**和**AGENTS. md**
可参考
1.清楚说明目标
2.提供必要上下文
3.明确重要约束
4.规定验证标准
5.允许Agent自主探索
6.避免过度规定执行步骤
### skill
Skill是一组可复用的任务指令和配套资源，用于指导Agent完成特定类型的任务。Agent可以在需要时加载这些内容，按照其中的流程开展工作。
可选资源：脚本、参考文档、模板等。
### AGENTS. md
类似于项目中给人看得README. me,AGENTS.md就是给ai看的项目说明书
用于向AI编程Agent提供项目背景、开发规范和工作指引，指导它在这个项目中开展工作
### 构建harness
https://www.mihaileric.com/The-Emperor-Has-No-Clothes/