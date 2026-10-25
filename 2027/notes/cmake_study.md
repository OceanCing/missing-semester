# 学习笔记
<small>课内课外一起了</small>

### 程序编译

- 源文件->目标文件：预处理(.c/.cpp->.i),编译(.i->.s),汇编(.s-.o),链接(.o文件整合)
- 发展历史:makefile->cmake->catkin
- 编译时会进行优化，用(volvate)去禁止优化部分函数或者代码
- 虚拟内存(虚拟储存器):给进程提供虚拟地址，隔离了操作和内存之间的直接映射，防止恶意修改或者覆盖其他进程，
- 静态链接:将一组文件生成为一个可执行的目标文件
- 静态链接过程:
    1. 空间内存和地址分配(这里主要是将所有东西进行预处理，建立映射关系)
    2. 符号解析和重定位:符号(即本地的.c里面的全局变量或者函数、外部可被引用的全局变量或者函数、本地静态变量或者本地函数[只能在本地使用])
    3. 链接器操作.o文件，将文件里面的函数和变量用符号代替，同时将多个文件打包为一个文件.o文件
    4. 要执行文件时，输入 ./'文件名'
- 动态链接共享库可以减少静态库重复浪费内存的情况，在链接重复文件时给链接器目标，然后让链接器生成部分的.o文件(不重复的部分)，然后再用动态链接器执行文件

---
### cmake

创建CmakeList.txt文件
- 指令

```bash
#指定cmake版本
cmake_minimum_required(VERSION 3.10)
#定义项目名称和语言
project(MyProject CXX)
#添加可执行文件
add_executable(MyApp main.cpp utils.cpp)
#添加库文件
add_library(MyLib STATIC library.cpp)
#链接库到目标
target_link_libraries(MyApp PRIVATE MyLib)
#设置变量
    # 定义一个普通变量，后续通过 ${变量名} 引用
    set(CMAKE_CXX_STANDARD 17)
    # 同时设置多个值（列表）
    set(SOURCES main.cpp utils.cpp helper.cpp)
    add_executable(MyApp ${SOURCES})
#为目标指定头文件路径
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/include)
#设置安装规则
install(TARGETS MyApp RUNTIME DESTINATION bin)
```
---
### 大模型LLM

- 在Emerpor的文件中，里面说提供ai三个工具，让ai使用三个工具(即读取、列出、编辑文件),让ai通过这三个工具，能过得到你所需要的文件、获取你的问题、给出你所需要的答案或者已编辑完成的文件。
- 类似于给一个接口，上层ai调用、得到、获得、给出文件并思考，下层程序调用、提供原文件给ai，从ai中得到修改后的文件，这样可以在程序里面隔离ai去触碰其他的文件
- agent软件也就是三个工具一样，与外界交互。
- skill可以让ai在特定的学术地方或者区域内搜索、思考，AGENTS.md是给ai看的说明书
- '你老是依赖棒棒糖，迟早会吃亏的'


---

### catkin


# ROS 1 Noetic 入门：从容器到 catkin 工作空间

本文使用 Docker 搭建 ROS 1 Noetic 开发环境，并完成 catkin 工作空间和软件包的创建。

## 1. 启动 ROS 容器

在宿主机终端中运行：

```bash
sudo docker run -it --name ros1 osrf/ros:noetic-desktop-full bash
```

该命令会创建名为 `ros1` 的容器，并进入容器内的 Bash。

## 2. 启动 ROS 核心

在容器终端中运行：

```bash
roscore
```

`roscore` 需要保持运行。请勿关闭这个终端，也暂时不要在该终端执行其他命令。

## 3. 打开另一个容器终端

在宿主机上新开一个终端，然后进入同一个容器：

```bash
sudo docker exec -it ros1 bash
```

> **注意：** 如果在第一个终端中执行了 `exit`，容器通常会停止，此时 `docker exec` 将无法进入容器。应先启动容器，再执行上面的命令：
>
> ```bash
> sudo docker start ros1
> sudo docker exec -it ros1 bash
> ```
>
> 如果 `roscore` 也在容器停止时退出了，需要重新进入容器并再次运行 `roscore`。

## 4. 创建并构建 catkin 工作空间

在第二个容器终端中加载 ROS 1 Noetic 环境：

```bash
source /opt/ros/noetic/setup.bash
```

创建 catkin 工作空间的目录结构：

```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/
```

构建工作空间：

```bash
catkin_make
```

加载新生成的环境设置文件：

```bash
source devel/setup.bash
```

检查 `ROS_PACKAGE_PATH`：

```bash
echo $ROS_PACKAGE_PATH
```

确认输出中包含当前工作空间的 `src` 目录，例如：

```text
/home/<用户名>/catkin_ws/src:/opt/ros/noetic/share
```

这样可确认 `setup.bash` 已正确把工作空间加入 ROS 的软件包搜索路径。

## 5. 创建 catkin 软件包

进入工作空间的 `src` 目录：

```bash
cd ~/catkin_ws/src
```

创建名为 `beginner_tutorials` 的软件包，并声明对 `std_msgs`、`rospy` 和 `roscpp` 的依赖：

```bash
catkin_create_pkg beginner_tutorials std_msgs rospy roscpp
```

## 6. 重新构建并加载环境

返回工作空间根目录：

```bash
cd ~/catkin_ws
```

重新构建工作空间：

```bash
catkin_make
```

加载更新后的环境设置文件：

```bash
source ~/catkin_ws/devel/setup.bash
```

## 7. 检查软件包依赖

查看 `beginner_tutorials` 的一级依赖：

```bash
rospack depends1 beginner_tutorials
```

`rospack` 列出的依赖应与运行 `catkin_create_pkg` 时传入的依赖一致。这些依赖同时记录在软件包的 `package.xml` 文件中。

进入软件包并查看 `package.xml`：

```bash
roscd beginner_tutorials
cat package.xml
```

查看 `rospy` 自身的一级依赖：

```bash
rospack depends1 rospy
```

递归查看 `beginner_tutorials` 的全部依赖：

```bash
rospack depends beginner_tutorials
```

`rospack depends1` 只显示直接依赖，而 `rospack depends` 会继续展开这些依赖所依赖的软件包。

## 关键命令速查

| 操作 | 命令 |
| --- | --- |
| 创建并进入 ROS 容器 | `sudo docker run -it --name ros1 osrf/ros:noetic-desktop-full bash` |
| 启动 ROS 核心 | `roscore` |
| 进入已有容器 | `sudo docker exec -it ros1 bash` |
| 加载 ROS 环境 | `source /opt/ros/noetic/setup.bash` |
| 构建工作空间 | `catkin_make` |
| 加载工作空间环境 | `source ~/catkin_ws/devel/setup.bash` |
| 创建软件包 | `catkin_create_pkg beginner_tutorials std_msgs rospy roscpp` |
| 查看直接依赖 | `rospack depends1 beginner_tutorials` |
| 查看全部依赖 | `rospack depends beginner_tutorials` |

(其实catkin这个东西还是看不懂，带着ai也还是消化不良，只能把他的指令搬上来了)
(╥﹏╥)
---
