# cmake与catkin、AI
## 1.源文件到可执行程序
```mermaid
graph LR
A([.c源文件]) --"预处理"-->B([.i修改文本]) --"编译"-->C([.s汇编程序]) --"汇编"-->D([.o可重定向目标程序]) --"链接"-->E([可执行文件])
```
### (1)Makelife、、Cmake、catkin、package
- makelife：告诉make工具编译顺序、编译文件、命令。实现编译、构建程序自动化。
- Cmake：过程式语言，可以用简单的语句来描述所以平台的编译过程。能够产生标准的建构档makelife或project文件。
- catkin：Cmake上的封装工具，用来管理ROS软件包的编译、依赖和信息生成。
- package：ROS中代码组织、构建和分发的最小单元。至少包含两个核心文件：package.xm1、CMakeLists.txt
  
### (2)catkin流程和package创建
#### catkin
- catkin --help   ##检查catkin是否安装
- mkdir catkin——ws ##创建文件夹
- cd /catkin_ws &mkdir src  ##进入工作空间并创建src
- cd src    ##进入src目录
- catkin_init_workspace ##初始化工作空间
- cd .. ##返回上一级
- catkin build  ##编译整个工作空间
- source devel/setup.bash ##刷新环境变量
#### package
- cd ~/catkin_ws/src/  ##切换到工作空间的src目录
- catkin_create_pkg test_pkg std_msgs rospy roscpp    ##创建test_pkg功能包，同时指明依赖std_msgs rospy roscpp



## 2.AI
- 大模型（Large Language Model,LLM）:可以看作对给定输入的输出概率分布进行建模。
- Agent：一套让LLM能够使用工具、与外部环境交互、并执行任务的系统
- 好的使用习惯：1.清楚说明目标，2.提供必要上下文，3.明确重要约束，4.规定检验标准，5.允许Agent自主探索，6.避免过度规定执行步骤。
- skill：一组可以复用的任务指令和配套资源，用于Agent完成指定任务。
- AGENTS.md:给ai看的项目说明书，README.md：给人看的说明书。
  

## 补充资料
- 暂无补充
