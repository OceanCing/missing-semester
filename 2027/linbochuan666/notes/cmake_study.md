# 27赛季DynamicX第二节培训课笔记 
>内容：camke、catkin、大模型、以及四篇补充资料，其中的补充资料直接放到了相关知识点部分
## 0.0简介
- 源代码包需要经过编译才能在系统上运行。Linux下的编译器有gcc、g++
- 源文件增加后，直接用编译器命令的效率太低了，于是开始用Makefile进行编译
- 但工程体量又增大后，Makefile也不能满足需求，于是出现了Cmake工具
- Cmake是对make工具的生成器（自动），它简化了编译构建过程，能够管理大型项目。像ROS这样大体量的平台将采用CMake,而ROS对CMake进行了扩展，于是就有了Catkin编译系统。
- **总的来说：Cmake是底层通用的构建工具；ROS是机器人软件框架；Catkin是基于CMake扩展出来的ROS专用构建系统**。所以实际开发ROS时，通常是**Catkin调用CMake**来编译ROS功能包。

---

## 一、从源代码到可执行程序

相关资料：https://zhuanlan.zhihu.com/p/476697014
- 计算机只看得懂二进制代码
- 编译器转换我们写的语言代码
- 具体转换过程   
  
  1.**预处理**：展开`#include`、宏替换等  

  2.**编译**：把编译语言变成汇编   

  3.**汇编**：把汇编变成机器码目标文件`.o`(*已经得到二进制文件*)   

  4.**链接**：把多个.o文件和库拼成一个可执行的文件(*实现文件的增量构建*)   
  >   *增量构建：只重新编译改动过的部分*
  
```txt
hello.c         hello.i     hello.s     hello.o     hello
-------> 预处理器--------->编译器-------->汇编器-------->链接器------->
 源程序    (cpp)  修改了的  (ccl)  汇编程序 (as)  可重定位  (ld) 可执行
 (文本)           源程序           (文本)        目标程序       目标程序
                                              （二进制）     （二进制）
 ```
 ---
 ## 二、Makefile
- 概念
  - Makefile 是写给Make命令的“**自主编译说明书**”。它告诉 make 工具：哪些文件要编译、编译依赖哪些文件、需要哪些命令
- 作用 
  - 人为手写Makefile **把编译链接规则写下来，让 make 自动执行**。通过编写 Makefile ,在每次编写修改好代码后，只需要敲一次 make 命令就能够得到想要的目标程序。
- 核心格式
  ```makefile
  目标：依赖
    命令
    ```
  - **目标**：要生成什么，比如`auto_aim`、`main.o`
  - **依赖**：生成它需要哪些文件，比如`main.cpp`
  - **命令**：具体怎么生成，注意**前面必须是Tab,不能是空格**
---
  
## 三、CMake基础
相关资料：https://www.runoob.com/cmake/cmake-basic.html
  #### 用CMake的原因：
   1.  随着文件和依赖增加，我们**需要维护的 Makefile 规则就跟着变多**，单纯靠手写 makefile 会不方便。（稍微改个文件路径可能就要改一堆规则）
   2.  **Makefile 不能跨平台**
   
  #### CMake 具体是个啥？
   - 它是一种过程式语言，是一种跨平台的**构建系统生成工具**，用于管理C/C++项目的编译过程。
  #### 工作原理
   - 通过编写**CMakeLists.txt配置文件**，可以定义项目的构建规则、依赖关系和编译选项，CMake会**自动生成对应平台的构建文件**（如Unix下的Makefile或Windows下的Vistual Studio解决方案）。
     >Unix用于Linux、macOS等
  #### CMakeLists.txt文件
   - 是CMake的核心配置文件，每个CMake项目至少要一个CMakeLists.txt文件，CMake通过读取文件中的指令来了解项目的结构、源文件列表和编译要求。
  ##### 基本语法
   - 每个指令的格式：**命令名(参数列表)。**
  >详情见补充资料

  
  ### 下面来结合实际理解一下
  1.先建个空文件夹

  ```bash
  cd ~
  mkdir cmake_practice
  cd cmake_practice
  ```
  2.然后`vim main.cpp`随便写点代码
  ```cpp
  #include <iostream>
    int main() {
      std::cout << "This is origin g++!" << std::endl;
      return 0;
    }
  ```
  先试试原始打法   

  3.纯手搓的话要输入
  ```bash
  g++ main.cpp -o origin_app
  ```
  >⚠️ 相当是用g++编译器，然后给源文件一个output,并且给程序起个名字  

  4.运行
  ```bash
  ./origin_app
  ```
  **注意：当我们修改了源文件后，需要再输`g++ main.cpp -o origin_app`，相当于是更新程序吧，所以文件一多就爆炸麻烦**   

  现在试试CMake  

  5.用vim写一份说明书  

  先输入
  ```bash
  vim CMakeLists.txt
  ```
  再敲入
  ```cmake
  cmake_minimum_required(VERSION 3.10)
  project(my_project)
  add_executable(cmake_app main.cpp)
  ```
  >⚠️ 这里的核心是第三行，相当于是替我敲了`g++ main.cpp -o cmake_app`   

  6.固定的四步编译  

  依次输入
  ```bash
  mkdir build
  cd build
  cmake ..
  make
  ```
  >⚠️..代表上一级目录。cmake .. 即是让CMake去读刚才写的    CMakeLists.txt，它会生成一套复杂的编译规则。
  make则是开始编译（并按CMake的要求）
   
  7.运行CMake生成的应用  

  在build文件夹里输入
  ```bash
  ./cmake_app
  ```
  >这里是在第五步换了个名字，其实就相当于上面的origin_app   

  ⚠️之后如果修改了源代码，就只用进到build文件夹里make就可以了
  >这个build是之前创建的，里面存放了源代码编译后的文件，make相当于是将修改过的源码编译一遍然后放到build。当然，build里面有cmake相关文件，所以make直接自动编译了


  ---

## 四、catkin基础
#### 为什么有了CMake还要有catkin？
- ROS太过庞大，功能包太多，包与包之间的依赖关系太过复杂
- catkin就是帮我们自动梳理这些包之间的依赖关系，然后自动去调用底层的CMake和make,完成整个工作空间的编译。
- catkin是ROS下的编译工具，CMake更高层次的抽象

#### 运行流程
  1.catkin运行：扫描src文件夹，读取每个包的package.xml和CMakeLists.txt,理清包与包之间的依赖。  

  2.catkin调用CMake  

  3.CMake生成Makefile  

  4.make执行编译   


#### catkin强制规定的工作空间结构 
  catkin workspace
  - src/**（源码空间）**：重点，我们所有的源代码包（Package）必须放在这里
  -  build/**（编译空间）**：存放CMake和catkin产生的中间文件、缓存文件
  -  devel/**（开发空间）**：编译生成的最终可执行文件、库文件，以及环境变量脚本，全放在这里
#### 一些命令


  - **catkin --help  ## 检查 catkin 是否安装成功，并列出所有可用的命令**
  - mkdir catkin_ws
  - cd catkin_ws   
  - mkdir src      # 创建 src 目录存放代码
  - cd src
  - **catkin_init_workspace  ## 初始化工作空间（会在src下生成一个CMakeLists.txt链接文件）**
  - cd ..
  - **catkin build   # 使用 catkin build 命令对整个工作空间进行编译**
  - **catkin_make    # 编译整个工作空间**

#### package功能包
 1. 先进入工作空间的 src 目录，包要建在这里  
   
    cd ~/catkin_ws/src/  

 1. 创建 test_pkg 功能包，并指明依赖  
   
    格式：catkin_create_pkg 包名 依赖包1 依赖包2 ...  

    catkin_create_pkg test_pkg std_msgs rospy roscpp

```txt
 ├── CMakeLists.txt  # package的编译规则 (必须)
 ├── package.xml     # package的描述信息 (必须)
 ├── src/            # 存放源代码文件 (如 .cpp 文件)
 ├── include/        # 存放C++头文件 (如 .h 或 .hpp 文件)
 ├── scripts/        # 存放可执行脚本 (如 .py 文件)
 ├── msg/            # 存放自定义消息类型 (自定义数据结构)
 ├── srv/            # 存放自定义服务类型 (服务通信)
 ├── models/         # 存放3D模型文件 (如 .dae, .stl)
 ├── urdf/           # 存放机器人的urdf模型描述文件
 └── launch/         # 存放launch文件 (用于一键启动多个节点)
 ```
### 接下来跟着补充资料和ai简单尝试了一下
https://wiki.ros.org/catkin/Tutorials

为catkin创建一个工作区
```bash
# 1.激活 ROS 基础环境
source /opt/ros/noetic/setup.bash

# 2.创建src目录
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src

# 3.初始化工作空间
catkin_init_workspace

# 4.返回工作空间根目录执行编译
cd ~/catkin_ws
catkin_make

# 5.编译成功后，激活自己的局部环境(让系统找到自己编译的程序)
source devel/setup.bash

# 6.为确保自己的工作空间被安装脚本正确覆盖，要确保ROS_PACKAGE_PATH环境变量包含你所在的目录。（没看懂）
echo $ROS_PACKAGE_PATH
eg：/home/youruser/catkin_ws/src:/opt/ros/kinetic/share
```
资料里还有很多扩展内容不过没时间尝试了


 ---

## 五、大模型LLM

### 概念
####  什么是大模型
- 是一种有大量参数的人工智能模型
- 可以被看作是对给定提示字符串（输入）的补全字符串（输出）**概率分布进行建模**。LLM的推理会从这个概率分布中采样。
- LLM具有固定的上下文窗口，即输入和输出字符串的最大长度。

#### 什么是Agent软件
- 定义：能自主思考并调用云端工具
- Agent软件就是一套让大模型能够**使用工具、与外部环境交互，并持续执行任务**的系统
- 当你在电脑上装了一个agent软件并接入了大模型，大模型就可以利用agent软件里提供的工具和电脑交互（删改读写文件），帮助你做事情。


### 使用
#### 用大模型帮助学习
- 1.收集资料
  - “去网上搜索关于xx的最佳实践/优秀参考，给出信息来源”
- 2.告诉AI你已经学过什么，设计针对性的课程
  - “我学过c++，请你通过交互式提问我的知识水平，并设计一套python学习课程”
- 3.学习一个课题时，自己讲一遍让AI检查
- 4.让AI出练习题

#### 用大模型帮助开发
- 可参考来组织提示词的框架：
  - 清楚说明目标
  - 提供必要上下文
  - 明确重要约束
  - 规定验证标准
  - 允许Agent自主探索
  - 避免过度规定执行步骤
- 可以使用skill和AGENTS.md来帮助完成第二第三点

##### skill
- Skill是一组可复用的**任务指令和配套资源**，用于指导Agent完成特定类型的任务。Agent可以在需要时加载这些内容，按照其中的流程开展工作。
- 可选资源：脚本、参考文档、模板等。

##### AGENTS.md`
- 类似与项目中给人看的 README.md`，AGENTS.md就是给ai看的**项目说明书**
- 用于向AI编程Agent提供**项目背景**、**开发规范**和**工作指引**，指导它在这个项目中开展工作
- 需要一直更新，应当放在多个分目录，需要时让ai去查看

#### 部署agent
-   1.安装一个Node.js（运行环境）和pnpm（包管理器）
  2.安装一个框架，如：从GitHub克隆DeepSeek Harness的源码到本地，然后安装依赖并构建
  3.启动
  4.接入大模型（配置API）

---

### 思考
- Agent是一个完整的系统
  - **Agent = modle + harness**
  - 但是Agent不一定是本地的，比如ChatGPT是云端Agent
- 一些概念的整理
  - 大模型：“脑子”。推理思考，无法行动
  - API：“电话线”。电脑通过API跟云端的脑子联系
  - Harness（如dsh）：“身体的外壳”。一个空架子，里面提供了接口
  - Agent：把“脑子”（大模型）通过“电话线”（API）塞进“外壳”（Harness）里，在加上写的代码，形成一个完整的Agent
- codex、ChatGPT、豆包都是一个Agent
- Deepseek Harness是一个本地端部署的框架，还需要接入云端大模型，dsh可以接入许多的插件(依靠Cordis底层基座)


---


### 扩展资料：皇帝新衣：怎样用200行代码跑完Agent核心逻辑
https://www.mihaileric.com/The-Emperor-Has-No-Clothes/
#### 一、 核心心智模型

Agent 本质上是个带工具箱的大模型（LLM）。
它的工作是一个循环：
你发消息 --> LLM 决定用哪个工具 --> 本地代码执行工具 --> 把结果还给 LLM --> LLM 继续思考下一步。
**重点：大模型本身连不到你的文件系统，是本地代码在执行。**

#### 二、 Agent底层只需要三个基础工具

1. **读文件**：让大模型看到代码。
2. **列目录**：让大模型知道项目长什么样。
3. **改文件**：让大模型能写入新代码。

#### 三、 教 AI 用工具

写入一定的文件，让AI自己**调用**并学习
#### 四、 和工业级产品差在哪？

除了这 200 行核心逻辑外，像 Codex 这种商业产品，增加了许多代码工程细节：比如更好的报错处理、更聪明的内容管理、更多工具等等。