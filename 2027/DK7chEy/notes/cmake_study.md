## 1.程序编译流程

* 源代码（高级语言）——编译系统——可执行程序（计算机语言，二进制代码）

* 具体转换过程：

  §1.预处理

  §2.编译

  §3.汇编

  §4.链接

<img src="Cmake_study.assets/image-20261025194747981.png" alt="image-20261025194747981" style="zoom:80%;" />

[详细介绍](https://zhuanlan.zhihu.com/p/476697014)

## 2.CMake

### 1.场景

* 源文件增加，直接用编译器gcc/g++命令的方式（单次手动）显得效率低下，人们开始用**Makefile**来进行编译。**make** 通过将繁琐的手动编译过程抽象为可重复执行的规则，极大地提升了开发效率。随着工程体量的增大，Makefile也不能满足需求，于是便出现了Cmake工具。

---



* 我的疑问（**ai**解答）：我在使用ide编辑器（如vs code）中可以直接编译并运行 还需要这种make或cmake工具吗？

* 答：在VSCode这类编辑器中，“一键编译运行”的便捷性，和你是否需要 `make`/`cmake`，其实是**开发阶段**与**工程化阶段**的区别。你目前享受的便利，本质上是IDE或插件在后台替你完成了一次简单的编译命令；但一旦项目稍微复杂，这种“便利”就会迅速失效。这意味着，一旦你的项目包含多个 `.cpp` 文件、需要链接第三方库，或者有复杂的头文件搜索路径，Code Runner 就会直接“罢工”，因为它**根本不扫描、也不处理文件之间的依赖关系**。

* 当然，你可以手动编写 VSCode 的 `tasks.json` 来告诉它“编译 `src/` 下的所有文件”。但随着文件增多、配置变复杂（如区分 Debug/Release、添加链接库），手写并维护这些规则会迅速失控。**这正是构建工具要解决的问题域**。

* **可重复、可移植、可自动化**

  ---

> Makefile如同一份自动编译说明书。它告诉make 工具:哪些文件要编译、编译顺序是什么、需要哪些命令。
>
> make(命令解释器)执行makefile（规则文件）定义的源文件之间的依赖关系和构建步骤，实现自动化地完成程序的编译、链接等复杂过程

### 2.简介

CMake 是个一个开源的跨平台自动化建构系统，用来管理软件建置的程序，并不依赖于某特定编译器，并可支持多层目录、多个应用程序与多个函数库。

CMake 通过使用简单的配置文件 CMakeLists.txt，自动生成不同平台的构建文件（如 Makefile、Ninja 构建文件、Visual Studio 工程文件等），简化了项目的编译和构建过程。

CMake 本身不是构建工具，而是生成构建系统的工具，它生成的构建系统可以使用不同的编译器和工具链。

**优势**：我们使用cmake通过编写简单的配置文件去生成本地的Makefile，这个配置文件是独立于运行平台和编译器的，不管在什么平台用什么编译器，cmake都可以生成对应的makefile

### 3.构建配置

**CMakeLists.txt 文件：** CMake 的配置文件，用于定义项目的构建规则、依赖关系、编译选项等。每个 CMake 项目通常包含一个或多个 CMakeLists.txt 文件。

**构建目录：** 为了保持源代码的整洁，CMake 鼓励使用独立的构建目录（Out-of-source 构建）。这样，构建生成的文件与源代码分开存放。

### 4.基本工作流程

1. **编写 CMakeLists.txt 文件：** 定义项目的构建规则和依赖关系。
2. **生成构建文件：** 使用 CMake 生成适合当前平台的构建系统文件（例如 Makefile、Visual Studio 工程文件）。
3. **执行构建：** 使用生成的构建系统文件（如 `make`、`ninja`、`msbuild`）来编译项目。

[详细教程](https://www.runoob.com/cmake/cmake-tutorial.html)

## 3.catkin

### 1.简介

Catkin是ROS为了取代旧构建系统`rosbuild`而创建的官方构建系统。它的核心是**一组CMake宏、变量和Python脚本的集合**，其目的是在CMake的标准工作流程之上，提供ROS特定的功能。

**CMake是引擎，Catkin是专为ROS这辆“车”定制的控制系统**。Catkin利用CMake的强大能力，并添加了管理ROS包生态系统（如依赖解析、统一工作空间）所需的功能，让构建复杂的机器人软件变得有序和高效。

### 2.基础指令

catkin --help##检查catkin是否安装

mkdir catkin_ws 

cd /catkin_ws &mkdir src

cd src##src–**package源代码包**

catkin_init_workspace ##初始化工作空间

cd ..

catkin build

* **package功能包**

cd ~/catkin_ws/src/ #创建test_pkg功能包，同时指明依赖std_msgs rospy roscpp

catkin_create_pkg test_pkg 

std_msgs rospy roscpp

![image-20261025215939632](Cmake_study.assets/image-20261025215939632.png)

[高阶详细教程](https://wiki.ros.org/catkin/Tutorials)



## 4.使用deepseek harness安装codex 指使codex删除deepseek harness🤣🤣🤣

- [ ] 未完待续：cmake和catkin具体应用命令和案例
- [ ] https://www.mihaileric.com/The-Emperor-Has-No-Clothes/ 暂时没弄清这篇在说什么东西qwq