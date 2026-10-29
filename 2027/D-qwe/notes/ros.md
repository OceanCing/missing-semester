# 27赛季控制视觉招新培训第三课学习笔记：ROS NOTES

## 一.ROS的入门

### 1.ROS Introduction

#### (1).什么是ROS？
1. ROS 是一个适用于机器人的**开源的元操作系统**。它提供了操作系统应有的服务，包括硬件抽象，底层设备控制，常用函数的实现，进程间消息传递，以及包管理。

2. 它也提供用于获取、编译、编写、和跨计算机运行代码所需的工具和库函数。在某些方面ROS相当于一种**机器人框架（robot frameworks）**。

3. ROS设计者将ROS表述为**ROS = Plumbing + Tools + Capabilities + Ecosystem**，即ROS是**通讯机制**、**工具软件包**、**机器人高层技能**以及**机器人生态系统**的**集合体**

#### (2).设计目的
1. ROS 的主要目标是**为机器人研究和开发提供代码复用的支持**。也就是所谓的**代码复用**。

2. ROS是一个**分布式的进程（也就是节点）框架**，这些进程被封装在易于被分享和发布的程序包和功能包集中。

3. ROS中功能模块封装于独立的功能包或元功能包，便于分享，功能包内的模块以**节点**为单位运行，以ROS标准的IO作为接口，开发者不需要关注模块内部实现，**只要了解接口规则就能实现复用**,实现了**模块间点对点的松耦合连接**。

4. ROS 用一种与编程语言无关的中间定义来描述模块间的通信接口，构建时再自动生成各语言的代码，从而让 C++、Python、Java 等不同语言写的模块能够互相通信、自由组合，并支持复杂（嵌套）的消息结构——这就是**语言弱相关**的框架设计。

- 对于这点，我们可以知道：

 （1）. ROS 用一种"谁都不偏向"的格式来描述模块之间传什么数据，构建时再自动翻译成 C++、Python、Java 各自的代码，所以**不同语言写的模块能互相通信**。

 （2）.C++、Python、Java 写的模块都**能混着用**、互相通信。

 5. ROS具有称为rostest的内置单元/集成测试框架，可轻松安装和拆卸测试工具。也就是说：测试模块和正式模块解耦，装上去就能测，**测完卸掉不影响系统**，灵活、干净。

 6. ROS 把一些工具和软件（如 RVIZ 3D 可视化、仿真环境、消息查看工具）做成"组件"，开发者按 ROS 定义的接口接入，就能直接在系统里当**组件**用。

 ### 2，Hello World!ROS入门
 （以C++为例）
 1. 在终端中进入容器：
```bash
sudo docker start 91a4707596b1
sudo docker exec -it 91a4707596b1 bash
```
 2. 创建专属包：
 ```bash
 cd ~/catkin_ws/src
 catkin_create_pkg hello_pkg roscpp rospy std_msgs
```

 3. 进入src，进入src中的写字板
```bash
root@91a4707596b1:~/catkin_ws/src# cd hello_pkg/src
root@91a4707596b1:~/catkin_ws/src/hello_pkg/src# nano hello.cpp
```

4. 在写字板中输入：
```cpp
#include "ros/ros.h"

int main(int argc, char *argv[])
{
    //执行 ros 节点初始化
    ros::init(argc,argv,"hello");
    //创建 ros 节点句柄(非必须)
    ros::NodeHandle n;
    //控制台输出 hello world
    ROS_INFO("hello world!");

    return 0;
}

```
保存好并退出。

5. 先退回根目录，打开nano CMakeLists.txt·
```bash
root@91a4707596b1:~/catkin_ws/src/hello_pkg/src# cd ..
root@91a4707596b1:~/catkin_ws/src/hello_pkg# nano CMakeLists.txt
```

6. 在nano CMakeLists.txt里面加上：
```bash
add_executable(hello_node src/hello.cpp)

target_link_libraries(hello_node
  ${catkin_LIBRARIES}
)
```
保存退出
7. 回到catkin_ws根目录，编译：
```bash
root@91a4707596b1:~/catkin_ws/src/hello_pkg# cd ~/catkin_ws
root@91a4707596b1:~/catkin_ws# catkin_make
```
8. 打开一个新终端并进入容器，输入：
`roscore`
9. 回到上一个终端启动：
```bash
root@91a4707596b1:~/catkin_ws# source devel/setup.bash
root@91a4707596b1:~/catkin_ws# rosrun hello_pkg hello_node
```

**最终输出**
`[INFO] [1793091990.778132567]: hello world!`

### 3.launch文件演示与小乌龟
#### (1).概要
一个程序中可能需要启动多个节点，比如:ROS 内置的小乌龟案例，如果要控制乌龟运动，要启动多个窗口，分别启动 roscore、乌龟界面节点、键盘控制节点。如果每次都调用 rosrun 逐一启动，显然效率低下，如何优化?

- 官方给出的优化策略是使用 **launch 文件**，可以一次性**启动多个 ROS 节点**。

#### (2).演示
1. 进入容器:
```bash
jiecheng@jiecheng-Legion-Y7000-IRX10:~$ sudo docker start 91a4707596b1
sudo docker exec -it 91a4707596b1 bash
```
2. 进入工作空间并加载环境：
```bash
source /opt/ros/noetic/setup.bash
cd ~/catkin_ws/src
```
3. 建一个新包 
```bash
catkin_create_pkg demo_launch_pkg roscpp rospy std_msgs
cd demo_launch_pkg
```
4. 新建文件夹并写文件
```bash
mkdir launch
cd launch
nano demo.launch
```
5. 在弹出来的界面输入：
```cpp
<launch>
    <node pkg="turtlesim" type="turtlesim_node" name="t1"/>
    <node pkg="turtlesim" type="turtle_teleop_key" name="key1" />
</launch>
```
保存退出

6. 启动小乌龟：
```bash
cd ~/catkin_ws
source devel/setup.bash
roslaunch demo_launch_pkg demo.launch
```
#### (3).分析
1. 通过以上操作，我们可以看到，小乌龟是一个独立节点（t1），键盘控制是另一个独立节点（key1），它们在底层通过 ROS Master 互相发消息。我们能控制它，就证明了 ROS 的通信机制是通的。

2. 如果没有**launch 文件**，你要开三个终端：一个 roscore，一个 rosrun turtlesim turtlesim_node，一个 rosrun turtlesim turtle_teleop_key。现在一行命令全搞定，这就是自动化部署的雏形。以后在真实机器人上跑复杂程序，甚至几十个节点，全靠它。

### 4.ROS的文件系统

#### (1).ROS文件系统的大致结构：
ROS文件系统级指的是在硬盘上ROS源代码的组织形式，其结构大致可以如下图所示：
```text
WorkSpace --- 自定义的工作空间
    |
    |--- build: 编译空间，用于存放CMake和catkin的缓存信息、配置信息和其他中间文件。
    |
    |--- devel: 开发空间，用于存放编译后生成的目标文件，包括头文件、动态&静态链接库、可执行文件等。
    |
    |--- src: 源码
        |
        |-- package: 功能包(ROS基本单元)包含多个节点、库与配置文件，包名所有字母小写，只能由字母、数字与下划线组成
            |
            |-- CMakeLists.txt 配置编译规则，比如源文件、依赖项、目标文件
            |
            |-- package.xml 包信息，比如:包名、版本、作者、依赖项...(以前版本是 manifest.xml)
            |
            |-- scripts 存储python文件
            |
            |-- src 存储C++源文件
            |
            |-- include 头文件
            |
            |-- msg 消息通信格式文件
            |
            |-- srv 服务通信格式文件
            |
            |-- action 动作格式文件
            |
            |-- launch 可一次性运行多个节点
            |
            |-- config 配置信息
        |
        |-- CMakeLists.txt: 编译的基本配置
```

#### (2).ROS文件系统相关命令
**概述**：
ROS 的文件系统本质上都还是**操作系统文件**，我们可以使用Linux命令来操作这些文件，不过，在ROS中为了更好的用户体验，ROS专门提供了一些**类似于Linux**的命令，这些命令较之于Linux原生命令，更为简介、高效。文件操作，无外乎就是**增删改查与执行**等操作，接下来，我们就从这五个维度，来介绍ROS文件系统的一些常用命令。

1. 增

`catkin_create_pkg 自定义包名 依赖包` === 创建新的ROS功能包

`sudo apt install xxx` === 安装 ROS功能包

2. 删

`sudo apt purge xxx` ==== 删除某个功能包

3. 查

`rospack list` === 列出所有功能包

`rospack find 包名` === 查找某个功能包是否存在，如果存在返回安装路径

`roscd 包名` === 进入某个功能包

`rosls 包名` === 列出某个包下的文件

`apt search xxx` === 搜索某个功能包

4. 改

`rosed 包名 文件名` === 修改功能包文件

需要安装 vim

比如:`rosed turtlesim Color.msg`

5. 执行

**5.1 roscore**

`roscore` === 是 ROS 的系统先决条件节点和程序的集合， 必须运行 roscore 才能使 ROS 节点进行通信。

`roscore` 将启动:

    ros master

    ros 参数服务器

    rosout 日志节点

用法:

`roscore`

或(指定端口号)

`roscore -p xxxx`

**5.2rosrun**

`rosrun 包名 可执行文件名` === 运行指定的ROS节点

比如:`rosrun turtlesim turtlesim_node`

**5.3roslaunch**

`roslaunch 包名 launch文件名` === 执行某个包下的 launch 文件

#### (3).ROS计算图
对于一个已经安装好rqt_graph的ROS系统，我们可以在一个新的终端中用：
`rosrun rqt_graph rqt_graph`
来查看显示当前系统运行情况的动态图形。

比如：[ 节点: /key1 ]  -------- 话题: /turtle1/cmd_vel ------>  [ 节点: /t1 ]

- 这说明了**通信流向**是：从 /key1 指向 /t1。说明 /key1 是发布者（Publisher），它将按方向键产生的速度数据发到通道里；而 /t1 是订阅者（Subscriber），它从通道里拿到数据并驱动乌龟移动。


## 二.ROS通信机制

### 1.概述

在ROS中每一个功能点都是一个单独的进程，每一个进程都是独立运行的，更确切的讲，ROS是进程（也称为Nodes）的分布式框架。为了了解不同的进程的通信，我们就需要学习一下ROS中的通信机制。

ROS 中的基本通信机制主要有如下三种实现策略:

-    **话题通信**(发布订阅模式)

-    **服务通信**(请求响应模式)

-    **参数服务器**(参数共享模式)

我们接下来对于ROS通信机制的学习也是围绕着这三个点来进行。

### 2.话题通信

#### (1).概述
1. 话题通信是**基于发布订阅模式**的，也即:**一个节点发布**消息，**另一个节点订阅**该消息。

2. 一个典型场景就是：机器人在执行导航功能，使用的传感器是激光雷达，机器人会采集激光雷达感知到的信息并计算，然后生成运动控制信息驱动机器人底盘运动。

在上述场景中，就不止一次使用到了话题通信。

  -  以激光雷达信息的采集处理为例，在 ROS 中有一个节点需要时时的**发布当前雷达采集到的数据**，**导航模块中也有节点会订阅并解析雷达数据**。
  
  -  再以运动消息的发布为例，**导航模块**会根据传感器采集的数据时时的计算出**运动控制信息**并发布给底盘，**底盘**也可以有一个节点**订阅运动信息**并最终转换成控制电机的脉冲信号。

以此类推，像雷达、摄像头、GPS.... 等等一些传感器数据的采集，也都是使用了话题通信，换言之，话题通信适用于不断更新的数据传输相关的应用场景。


#### (2).理论模型
话题通信实现模型中涉及到三个角色:

-    **ROS Master** (管理者)
-    **Talker** (发布者)
-    **Listener** (订阅者)

他们大概的工作机制是：

**ROS Master** 负责**保管Talker 和 Listener 注册的信息**，并**匹配话题相同的 Talker 与 Listener**，**帮助 Talker 与 Listener 建立连接**，连接建立后，**Talker 可以发布消息**，且发布的消息**会被 Listener 订阅**。

整个流程由以下步骤实现:

0. **Talker注册**

Talker启动后，会通过RPC在 ROS Master 中注册自身信息，其中包含所发布消息的话题名称。ROS Master 会将节点的注册信息加入到注册表中。

1. **Listener注册**

Listener启动后，也会通过RPC在 ROS Master 中注册自身信息，包含需要订阅消息的话题名。ROS Master 会将节点的注册信息加入到注册表中。

2. **ROS Master实现信息匹配**

ROS Master 会根据注册表中的信息匹配Talker 和 Listener，并通过 RPC 向 Listener 发送 Talker 的 RPC 地址信息。

3. **Listener向Talker发送请求**

Listener 根据接收到的 RPC 地址，通过 RPC 向 Talker 发送连接请求，传输订阅的话题名称、消息类型以及通信协议(TCP/UDP)。

4. **Talker确认请求**

Talker 接收到 Listener 的请求后，也是通过 RPC 向 Listener 确认连接信息，并发送自身的 TCP 地址信息。

5. **Listener与Talker件里连接**

Listener 根据步骤4 返回的消息使用 TCP 与 Talker 建立网络连接。

6. **Talker向Listener发送消息**

连接建立后，Talker 开始向 Listener 发布消息。

**备注**
1. 上述实现流程中，**前五步**使用的 **RPC协议**，**最后两步**使用的是**TCP 协议**

2. Talker 与 Listener 的**启动无先后顺序要求**

3. Talker 与 Listener 都**可以有多个**

4. Talker 与 Listener **连接建立后**，**不再需要 ROS Master**。也即，即便关闭ROS Master，Talker 与 Listern **照常通信**。

#### (3).话题通信基本操作(C++)实现Talker 与 Listener之间的联系

我们在deepseek指导下的此次实操目的在于：**让Talker 与 Listener之间通过特定频道（话题）传话。**

实操大概流程如下：**建包**-->**分别写两个独立程序**-->**配置 CMakeLists.txt**-->**编译与执行**

1. 进入容器新建一个叫comm_pkg的新包
```bash  
sudo docker exec -it 91a4707596b1 bash
source /opt/ros/noetic/setup.bash
cd ~/catkin_ws/src
catkin_create_pkg comm_pkg roscpp rospy std_msgs
```

2. 写Talker的代码:
先打开一个文件：
`nano Hello_pub.cpp`
后写入保存：
```cpp
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

int main(int argc, char *argv[])
{   
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "talker");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::String>("chatter", 10);

    std_msgs::String msg;
    std::string msg_front = "Hello ！";
    int count = 0;
    ros::Rate r(10); // 10Hz

    ros::Duration(3.0).sleep();

    while (ros::ok())
    {
        std::stringstream ss;
        ss << msg_front << count;
        msg.data = ss.str();
        pub.publish(msg);
        ROS_INFO("Publishing:%s", msg.data.c_str());

        r.sleep();
        count++;
        ros::spinOnce();
    }
    return 0;
}
```
3. 然后当然是Listener啦！
打开一个新的文件：
`nano Hello_sub.cpp`
然后写入：
```cpp
#include "ros/ros.h"
#include "std_msgs/String.h"

void doMsg(const std_msgs::String::ConstPtr& msg_p){
    ROS_INFO("I heard:%s", msg_p->data.c_str());
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "listener");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe<std_msgs::String>("chatter", 10, doMsg);
    ros::spin();
    return 0;
}
```
4. 配置 CMakeLists.txt

先退到包的根目录：
`cd ..`

打开CMakeLists.txt：
`nano CMakeLists.txt`

在文件最底部写入：
```bash
add_executable(Hello_pub
  src/Hello_pub.cpp
)
add_executable(Hello_sub
  src/Hello_sub.cpp
)

target_link_libraries(Hello_pub
  ${catkin_LIBRARIES}
)
target_link_libraries(Hello_sub
  ${catkin_LIBRARIES}
)
```

5. 编译
一键编译成机器能跑的程序：
```bash
cd ~/catkin_ws
catkin_make
```

刷新一下
`source devel/setup.bash`

然后在此终端`roscore`

6. 执行
新打开一个终端，并用`sudo docker exec -it 91a4707596b1 bash`进入容器

启动执行：
`source devel/setup.bash && rosrun comm_pkg Hello_pub`

**执行结果**：
```bash
[INFO] [1793195532.377850885]: Publishing:Hello 0
[INFO] [1793195532.379821586]: Publishing:Hello 1
[INFO] [1793195532.479966620]: Publishing:Hello 2
[INFO] [1793195532.579964602]: Publishing:Hello 3
[INFO] [1793195532.679978893]: Publishing:Hello 4
[INFO] [1793195532.780008525]: Publishing:Hello 5
[INFO] [1793195532.880053762]: Publishing:Hello 6
[INFO] [1793195532.979979076]: Publishing:Hello 7
[INFO] [1793195533.079986237]: Publishing:Hello 8
[INFO] [1793195533.179962138]: Publishing:Hello 9
[INFO] [1793195533.279962435]: Publishing:Hello 10
[INFO] [1793195533.379996323]: Publishing:Hello 11
[INFO] [1793195533.479982283]: Publishing:Hello 12
[INFO] [1793195533.579992710]: Publishing:Hello 13
[INFO] [1793195533.679979571]: Publishing:Hello 14
[INFO] [1793195533.779998151]: Publishing:Hello 15
[INFO] [1793195533.879989170]: Publishing:Hello 16
[INFO] [1793195533.979960257]: Publishing:Hello 17
[INFO] [1793195534.080029255]: Publishing:Hello 18
[INFO] [1793195534.180072451]: Publishing:Hello 19
[INFO] [1793195534.279977737]: Publishing:Hello 20

```
这个呢，就是我们的发布的广播了，与此同时，我们再打开一个终端进入容器，并输入：
`source /opt/ros/noetic/setup.bash && source ~/catkin_ws/devel/setup.bash && rosrun comm_pkg Hello_sub`

然后就会收到：
```bash
[INFO] [1793195836.000839295]: I heard:Hello 0
[INFO] [1793195836.003743692]: I heard:Hello 1
[INFO] [1793195836.103941297]: I heard:Hello 2
[INFO] [1793195836.203805537]: I heard:Hello 3
[INFO] [1793195836.303892170]: I heard:Hello 4
[INFO] [1793195836.403857882]: I heard:Hello 5
[INFO] [1793195836.503907870]: I heard:Hello 6
[INFO] [1793195836.603885417]: I heard:Hello 7
[INFO] [1793195836.703911661]: I heard:Hello 8
[INFO] [1793195836.803901675]: I heard:Hello 9
[INFO] [1793195836.903908543]: I heard:Hello 10
[INFO] [1793195837.003875736]: I heard:Hello 11
[INFO] [1793195837.103914410]: I heard:Hello 12
[INFO] [1793195837.203916635]: I heard:Hello 13
[INFO] [1793195837.303901608]: I heard:Hello 14
[INFO] [1793195837.403876782]: I heard:Hello 15
[INFO] [1793195837.503917552]: I heard:Hello 16
[INFO] [1793195837.604011447]: I heard:Hello 17
[INFO] [1793195837.703854852]: I heard:Hello 18
[INFO] [1793195837.803882830]: I heard:Hello 19
[INFO] [1793195837.903761001]: I heard:Hello 20
```
这样一来，我们就实现了一发一收的**话题通信**。

#### (4).话题通信自定义msg
**msg 的核心意义**：

1. 突破单一数据的限制
std_msgs 只能传一个文本或数字。要想传雷达数据（距离+角度+强度）、视觉识别结果（坐标+置信度），必须自定义结构体。

2. 统一通信协议
发送方和接收方必须基于同一个 .msg 文件。

3. 跨语言无缝互通
写一次 .msg 文件，编译时 ROS 会自动生成 C++ 和 Python 两套代码。这意味着，C++ 的雷达节点发出的消息，Python 节点直接就能接，这是开发大项目的基石。

**实践目的**

接下来的实践我们要传输一个复杂对象（包含姓名，年龄，身高），让ROS能够打包，发送并解包这个自定义结构体，从而验证：

1. 验证系统能不能认识你发明的数据结构：看看编译后，`devel/include/msg_pkg/Person.h` 这个文件到底有没有成功生成。

2. 验证 C++ 代码能不能调用它：验证写代码时，能不能直接写出 p.name = "sunwukong" 这种面向对象的赋值。

3. 验证网络传输：验证组合数据（文字+数字）在网络中是否会被拆包或乱码。

**具体操作**
1. 进入容器建专属包：
```bash
source /opt/ros/noetic/setup.bash
cd ~/catkin_ws/src
catkin_create_pkg msg_pkg roscpp rospy std_msgs message_generation message_runtime
cd msg_pkg
```

2. 创建和编写msg文件：
先输入：
```bash
mkdir msg
nano msg/Person.msg
```
在打开的文件里面输入：
```bash
string name
uint16 age
float64 height
```

3. 修改CMakeLists.txt
打开CMakeLists.txt文件，

第一，把
```cpp
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)
```
改成：
```cpp
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)
```
加上message_generation这个工具能把.msg图纸变成C++代码

第二，把
```cpp
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )
```
改成：
```cpp
add_message_files(
  FILES
  Person.msg
)
```
让系统能去msg文件夹中找到Persion.msg文件来当图纸
第三，把
```cpp
# generate_messages(
#   DEPENDENCIES
#   std_msgs
# )
```
中的#去掉让它生效。

第四，把：
```cpp
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES msg_pkg
  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)
```
改成：
```cpp
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES msg_pkg
  CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
#  DEPENDS system_lib
)
```
加上message_runtime的支持

4. 让系统读取修改的配置和生成Persion.msg文件
```bash
cd ~/catkin_ws
catkin_make
```
5. 创建并编辑talker代码：
打开编辑器：
```bash
cd ~/catkin_ws/src/msg_pkg/src
nano person_talker.cpp
```
输入：
```cpp
#include "ros/ros.h"
#include "msg_pkg/Person.h"

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "talker_person");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<msg_pkg::Person>("chatter_person", 1000);

    msg_pkg::Person p;
    p.name = "sunwukong";
    p.age = 2000;
    p.height = 1.45;

    ros::Rate r(10);
    ros::Duration(3.0).sleep();

    while (ros::ok())
    {
        pub.publish(p);
        p.age += 1;
        
        ROS_INFO("My name is: %s, age: %d, height: %.2f", p.name.c_str(), p.age, p.height);

        r.sleep();
        ros::spinOnce();
    }
    return 0;
}
```
6. 创建并编辑Listener的代码
`nano person_listener.cpp`后输入：
```cpp
#include "ros/ros.h"
#include "msg_pkg/Person.h"

void doPerson(const msg_pkg::Person::ConstPtr& person_p){
    ROS_INFO("Received: %s, %d, %.2f", person_p->name.c_str(), person_p->age, person_p->height);
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "listener_person");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe<msg_pkg::Person>("chatter_person", 10, doPerson);

    ros::spin();
    
    return 0;
}
```
7. 打开并配置CMakeLists.txt
`nano ~/catkin_ws/src/msg_pkg/CMakeLists.txt`打开后，在指令行的最末端写入：
```cpp
add_executable(person_talker src/person_talker.cpp)
add_executable(person_listener src/person_listener.cpp)

add_dependencies(person_talker ${PROJECT_NAME}_generate_messages_cpp)
add_dependencies(person_listener ${PROJECT_NAME}_generate_messages_cpp)

target_link_libraries(person_talker
  ${catkin_LIBRARIES}
)
target_link_libraries(person_listener
  ${catkin_LIBRARIES}
)
```
8. 生成可执行程序：
`cd ~/catkin_ws && catkin_make`

9. 后台支持：
新建一个终端进入容器，并启动`roscore`

10. 在第一个终端输入：
`source devel/setup.bash && rosrun msg_pkg person_talker`
启动发布

运行后得到：
```bash
[INFO] [1793202830.115735605]: My name is: sunwukong, age: 2001, height: 1.45
[INFO] [1793202830.118282913]: My name is: sunwukong, age: 2002, height: 1.45
[INFO] [1793202830.218465947]: My name is: sunwukong, age: 2003, height: 1.45
[INFO] [1793202830.318401494]: My name is: sunwukong, age: 2004, height: 1.45
[INFO] [1793202830.418415730]: My name is: sunwukong, age: 2005, height: 1.45
[INFO] [1793202830.518379584]: My name is: sunwukong, age: 2006, height: 1.45
[INFO] [1793202830.618395031]: My name is: sunwukong, age: 2007, height: 1.45
[INFO] [1793202830.718378629]: My name is: sunwukong, age: 2008, height: 1.45
[INFO] [1793202830.818410238]: My name is: sunwukong, age: 2009, height: 1.45
[INFO] [1793202830.918369538]: My name is: sunwukong, age: 2010, height: 1.45
[INFO] [1793202831.018359112]: My name is: sunwukong, age: 2011, height: 1.45
[INFO] [1793202831.118378947]: My name is: sunwukong, age: 2012, height: 1.45
[INFO] [1793202831.218388734]: My name is: sunwukong, age: 2013, height: 1.45
[INFO] [1793202831.318420667]: My name is: sunwukong, age: 2014, height: 1.45
[INFO] [1793202831.418399121]: My name is: sunwukong, age: 2015, height: 1.45
```

11. 在一个新终端的容器内输入：
`source /opt/ros/noetic/setup.bash && source ~/catkin_ws/devel/setup.bash && rosrun msg_pkg person_listener`
启动接收

运行后得到：
```bash
[INFO] [1793202874.719002391]: Received: sunwukong, 2447, 1.45
[INFO] [1793202874.818854165]: Received: sunwukong, 2448, 1.45
[INFO] [1793202874.918819770]: Received: sunwukong, 2449, 1.45
[INFO] [1793202875.018808404]: Received: sunwukong, 2450, 1.45
[INFO] [1793202875.118810282]: Received: sunwukong, 2451, 1.45
[INFO] [1793202875.218851255]: Received: sunwukong, 2452, 1.45
[INFO] [1793202875.318826305]: Received: sunwukong, 2453, 1.45
[INFO] [1793202875.418734569]: Received: sunwukong, 2454, 1.45
[INFO] [1793202875.518728915]: Received: sunwukong, 2455, 1.45
[INFO] [1793202875.618743687]: Received: sunwukong, 2456, 1.45
[INFO] [1793202875.718779963]: Received: sunwukong, 2457, 1.45
[INFO] [1793202875.818887761]: Received: sunwukong, 2458, 1.45
[INFO] [1793202875.918675758]: Received: sunwukong, 2459, 1.45
[INFO] [1793202876.018743547]: Received: sunwukong, 2460, 1.45
[INFO] [1793202876.118743284]: Received: sunwukong, 2461, 1.45
[INFO] [1793202876.218833837]: Received: sunwukong, 2462, 1.45
[INFO] [1793202876.318764313]: Received: sunwukong, 2463, 1.45
[INFO] [1793202876.418855061]: Received: sunwukong, 2464, 1.45
[INFO] [1793202876.518770023]: Received: sunwukong, 2465, 1.45
```
**总结**
我们通过这次实操，来看看我们一开始要验证的三点有没有达到目的：
1. 验证系统能不能认识你发明的数据结构？

- 终端刷出 `[ 78%] Built target msg_pkg_generate_messages` ，代表 CMake 成功读取了 Person.msg，并在底层生成了 C++ 可以调用的 Person.h 头文件。

2. 验证 C++ 代码能不能直接调用它？

- 发布者代码里，成功使用了 `msg_pkg::Person p`; 并直接给 `p.name、p.age` 赋值。如果系统没生成那个头文件，这里在编译阶段就会直接红字报错（找不到类型），但编译干净利落，说明面向对象的数据结构调用成功。

3. 验证复杂的组合数据在网络传输中会不会丢包或乱码？

- 订阅者收到了 `Received: sunwukong, 2580, 1.45`，发布者发的是 `My name is: sunwukong, age: 2580, height: 1.45`。
 
 由此可见实操很成功。

### 3.服务通信

#### (1).概述
服务通信也是ROS中一种极其常用的通信模式，服务通信是基于请求**响应模式**的，是一种**应答机制**。也即: 一个节点A向另一个节点B发送请求，B接收处理请求并产生响应结果返回给A。
 
比如如下场景:

机器人巡逻过程中，控制系统分析传感器数据发现可疑物体或人... 此时需要拍摄照片并留存。

一个节点需要向相机节点发送拍照请求，相机节点处理请求，并返回处理结果

与上述应用类似的，服务通信更适用于对**时时性**有要求、具有**一定逻辑处理**的应用场景。

#### (2).理论模型
理论模型中涉及到三个角色:

-    **ROS master**(管理者)
-    **Server**(服务端)
-    **Client**(客户端)

**ROS Master** 负责**保管 Server 和 Client 注册的信息**，并**匹配话题相同的 Server 与 Client** ，帮助 Server 与 Client **建立连接**，连接建立后，**Client 发送请求信息**，**Server 返回响应信息**。

整个流程由以下步骤实现:

0. Server注册

Server 启动后，会通过**RPC**在 ROS Master 中注册自身信息，其中包含提供的服务的名称。ROS Master 会将节点的注册信息加入到注册表中。

1. Client注册

Client 启动后，也会通过**RPC**在 ROS Master 中注册自身信息，包含需要请求的服务的名称。ROS Master 会将节点的注册信息加入到注册表中。

2. ROS Master实现信息匹配

ROS Master 会**根据注册表**中的信息匹配Server和 Client，并通过 RPC 向 Client 发送 Server 的 TCP 地址信息。

3. Client发送请求

Client 根据步骤2 响应的信息，使用 TCP 与 Server 建立网络连接，并发送请求数据。

4. Server发送响应

Server 接收、解析请求的数据，并**产生响应结果**返回给 Client。

#### (3).服务通信自定义srv

**srv 的核心意义**：

1. **突破单向传输的限制**
   `msg` 只能让发布者单向广播，而 `srv` 支持**双向问答**（客户端发 Request，服务端给 Response）。要想实现“请求底盘状态”、“下发控制指令”等需要明确结果反馈的功能，必须用 `srv`。
2. **统一双向通信协议**
   通过 `---` 分割线，强制客户端和服务端基于同一个 `.srv` 文件。上面是请求格式，下面是响应格式，确保两边收发数据完全一致。
3. **跨语言无缝互通**
   写一次 `.srv` 文件，编译时 ROS 会自动生成 C++ 和 Python 两套代码。C++ 写的服务端，Python 写的客户端可以直接调用，这是大型机器人系统混合编程的基石。

**实践目的**：

接下来的实践我们要实现一个加法求和功能，客户端提交两个整数，服务端计算后返回结果。我们要让 ROS 打包、发送并解包这个双向数据载体，从而验证：

1. **验证系统能不能认识你发明的服务结构**：看看编译后，`devel/include/srv_pkg/AddInts.h` 这个文件到底有没有成功生成。

2. **验证 C++ 代码能不能正确调用请求与响应**：验证写代码时，能不能直接调用 `req.num1`、`req.num2` 发送请求，并通过 `res.sum` 读取响应。

3. **验证双向网络传输**：验证数据在客户端发出、服务端接收并计算、最后把结果发回客户端的过程中，数值是否准确，是否会丢包。

**具体实操**
1. 进入容器并建一个名为srv_pkg的包
```bash
sudo docker start 91a4707596b1
source /opt/ros/noetic/setup.bash
cd ~/catkin_ws/src
catkin_create_pkg srv_pkg roscpp rospy std_msgs message_generation message_runtime
cd srv_pkg
mkdir srv
```

2. 创建并编写srv文件
`nano srv/AddInts.srv`
打开文件后写入：
```bash
int32 num1
int32 num2
---
int32 sum
```
3. 修改CMakeLists.txt
`nano CMakeLists.txt`
**第一，注册srv文件**
打开后找到：
```cpp
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )
```
把它修改成：
```cpp
add_service_files(
  FILES
  AddInts.srv
)
```
**第二，引入服务生成工具**
找到
```cpp
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)
```
加上： `message_generation`
**第三，生成服务信息：**
找到
```cpp
# generate_messages(
#   DEPENDENCIES
#   std_msgs
# )
```
把#去掉
**第四，配置包的运行时依赖**
找到
`# CATKIN_DEPENDS message_generation message_runtime roscpp rospy std_msgs`
改成：
`CATKIN_DEPENDS message_runtime roscpp rospy std_msgs`

4. 在package.xml中补全构建依赖
`nano ~/catkin_ws/src/srv_pkg/package.xml`
打开后找到<build_depend>的地方加上：
`<build_depend>message_runtime</build_depend>`

5. 开始编译
`cd ~/catkin_ws && catkin_make`

6. 创建服务端代码
```bash
cd ~/catkin_ws/src/srv_pkg/src
nano server.cpp
```
进入后写入：
```cpp
#include "ros/ros.h"
#include "srv_pkg/AddInts.h"

bool doReq(srv_pkg::AddInts::Request& req, srv_pkg::AddInts::Response& res)
{
    res.sum = req.num1 + req.num2;
    ROS_INFO("Server received: num1=%d, num2=%d. Server response: sum=%d", req.num1, req.num2, res.sum);
    return true;
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "add_server");
    ros::NodeHandle nh;

    ros::ServiceServer server = nh.advertiseService("add_ints", doReq);

    ros::spin();

    return 0;
}
```
7. 创建客户端代码
`nano client.cpp`
进入后写入：
```cpp
#include "ros/ros.h"
#include "srv_pkg/AddInts.h"

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "add_client");
    ros::NodeHandle nh;

    ros::ServiceClient client = nh.serviceClient<srv_pkg::AddInts>("add_ints");

    srv_pkg::AddInts srv;
    srv.request.num1 = 10;
    srv.request.num2 = 20;

    client.waitForExistence();

    if (client.call(srv))
    {
        ROS_INFO("Client received response: sum=%d", srv.response.sum);
    }
    else
    {
        ROS_ERROR("Failed to call service add_ints");
    }

    return 0;
}
```
8. 修改CMakeLists.txt，配置支持环境
`nano ~/catkin_ws/src/srv_pkg/CMakeLists.txt`
进入后在最下面加上：
```cpp
add_executable(server src/server.cpp)
add_executable(client src/client.cpp)

add_dependencies(server ${PROJECT_NAME}_generate_messages_cpp)
add_dependencies(client ${PROJECT_NAME}_generate_messages_cpp)

target_link_libraries(server ${catkin_LIBRARIES})
target_link_libraries(client ${catkin_LIBRARIES})
```
之后保存退出把新代码编译成可执行文件：
`cd ~/catkin_ws && catkin_make`

9. 启动管理者
新开一个终端进入容器，启动roscore

10. 启动服务端
新开一个终端进入容器，并输入：
`source /opt/ros/noetic/setup.bash && source ~/catkin_ws/devel/setup.bash && rosrun srv_pkg server`

11. 启动客户端
新开一个终端进入容器，并输入：
`source /opt/ros/noetic/setup.bash && source ~/catkin_ws/devel/setup.bash && rosrun srv_pkg client`
这一步之后会看到客户端的终端弹出来：
`[INFO] [1793243710.939060317]: Client received response: sum=30`
相应的服务端会反馈：
`[INFO] [1793243710.937134479]: Server received: num1=10, num2=20. Server response: sum=30`
这说明两边的联动起来了！服务通信已经成功。

**总结**
1. 验证系统能不能认识我们发明的服务结构？

- 编译时没有任何“找不到头文件”的报错，并且 server.cpp 和 client.cpp 里都能成功 `#include "srv_pkg/AddInts.h"`。这说明 CMake 成功读取了 .srv 文件，并生成了底层的 C++ 结构体。

2. 验证 C++ 代码能不能正确调用请求与响应？

- 在客户端代码里，我们成功写出了 `srv.request.num1 = 10 和 srv.request.num2 = 20`；在服务端代码里，我们成功写出了 `res.sum = req.num1 + req.num2`。这说明系统已经把这些数据当成了标准的面向对象结构体来调用。

3. 验证双向网络传输（数值是否准确、是否丢包）？

- 服务端清晰地打印了 `Server received: num1=10, num2=20`，客户端准确地收到了 `sum=30`。没有乱码，没有延迟，数据一来一回，严丝合缝。

### 4.参数服务器

#### (1).概述

参数服务器在ROS中主要用于**实现不同节点之间的数据共享**。参数服务器相当于是独立于所有节点的一个**公共容器**，可以将数据存储在该容器中，被不同的节点调用，当然不同的节点也可以往其中存储数据.

一个典型的应用场景是：导航实现时，会进行路径规划，比如: 全局路径规划，设计一个从出发点到目标点的大致路径。本地路径规划，会根据当前路况生成时时的行进路径
上述场景中，全局路径规划和本地路径规划时，就会使用到参数服务器：

- 路径规划时，需要参考小车的尺寸，我们可以将这些尺寸信息存储到参数服务器，全局路径规划节点与本地路径规划节点都可以从参数服务器中调用这些参数

#### (2).参数服务器理论模型
参数服务器实现的模型中涉及到三个角色:

-    **ROS Master** (管理者)
-    **Talker** (参数设置者)
-    **Listener** (参数调用者)

ROS Master 作为一个**公共容器**保存参数，**Talker**可以向容器中**设置参数**，**Listener** 可以**获取参数**。

整个流程由以下步骤实现:

1. Talker 设置参数

Talker 通过 RPC 向参数服务器发送参数(包括参数名与参数值)，ROS Master 将参数保存到参数列表中。

2. Listener 获取参数

Listener 通过 RPC 向参数服务器发送参数查找请求，请求中包含要查找的参数名。

3. ROS Master 向 Listener 发送参数值

ROS Master 根据步骤2请求提供的参数名查找参数值，并将查询结果通过 RPC 发送给 Listener。

#### (3).参数操作(C++)

**目的**：
1. 验证两套 API 是否等效：验证 ros::NodeHandle 和 ros::param 这两种写法，是不是都能成功往同一个地方存/取数据。

2. 验证复杂数据类型的支持：验证除了基础的整数，ROS 到底能不能把数组（Vector）和字典（Map）直接存在参数服务器里。

3. 验证全局可见性：验证我们在 C++ 代码里修改了参数后，能不能在终端通过敲 rosparam list 和 rosparam get 直接在系统层面看到（这是验证参数服务器机制最硬核的手段）。

**操作部分**
1. 进入容器在src下新建一个包并且建好src目录：
```bash
sudo docker exec -it 91a4707596b1 bash

source /opt/ros/noetic/setup.bash

cd ~/catkin_ws/src
catkin_create_pkg param_pkg roscpp rospy std_msgs

cd param_pkg
mkdir src
cd src
```
2. 写一个c++程序负责往参数服务器里面塞数据和修数据
`nano param_set.cpp`打开后写入：
```cpp
#include "ros/ros.h"
#include <vector>
#include <map>

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "set_update_param");

    std::vector<std::string> stus;
    stus.push_back("zhangsan");
    stus.push_back("lisi");
    
    std::map<std::string, std::string> friends;
    friends["guo"] = "huang";
    friends["yuang"] = "xiao";

    // ========== API 1: ros::NodeHandle ==========
    ros::NodeHandle nh;
    nh.setParam("nh_int", 10);
    nh.setParam("nh_double", 3.14);
    nh.setParam("nh_bool", true);
    nh.setParam("nh_string", "hello NodeHandle");
    nh.setParam("nh_vector", stus);
    nh.setParam("nh_map", friends);
    
    nh.setParam("nh_int", 10000);

    // ========== API 2: ros::param ==========
    ros::param::set("param_int", 20);
    ros::param::set("param_double", 3.14);
    ros::param::set("param_bool", false);
    ros::param::set("param_string", "Hello Param");
    ros::param::set("param_vector", stus);
    ros::param::set("param_map", friends);
    
    ros::param::set("param_int", 20000);

    ROS_INFO("Parameters have been set and updated.");
    
    return 0;
}
```
3. 写第二个程序用来查看数据
`nano param_get.cpp`打开后写入：
```cpp
#include "ros/ros.h"
#include <vector>
#include <map>

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "get_param");

    ros::NodeHandle nh;

    // 1. 使用 param 函数获取（键存在返回真实值，不存在返回默认值）
    int res1 = nh.param("nh_int", 100); 
    int res2 = nh.param("nh_int_nonexistent", 100); 
    ROS_INFO("param result: %d, %d", res1, res2);

    // 2. 使用 getParam 函数获取（存在返回 true 并赋值，不存在返回 false）
    int nh_int_value;
    std::string nh_string_value;
    std::vector<std::string> stus;
    std::map<std::string, std::string> friends;

    nh.getParam("nh_int", nh_int_value);
    nh.getParam("nh_string", nh_string_value);
    nh.getParam("nh_vector", stus);
    nh.getParam("nh_map", friends);

    ROS_INFO("getParam result: int=%d, string=%s", nh_int_value, nh_string_value.c_str());
    
    for (auto &&stu : stus)
    {
        ROS_INFO("vector element: %s", stu.c_str());        
    }

    for (auto &&f : friends)
    {
        ROS_INFO("map element: %s = %s", f.first.c_str(), f.second.c_str());
    }

    // 3. 使用 ros::param API 获取
    int res3 = ros::param::param("param_int", 20); 
    ROS_INFO("ros::param result: %d", res3);

    return 0;
}
```
4. 写第三个程序来自动删除无用数据
`nano param_del.cpp`打开后写入：
```cpp
#include "ros/ros.h"

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "delete_param");

    ros::NodeHandle nh;

    // 1. 使用 NodeHandle API 删除
    bool r1 = nh.deleteParam("nh_int");
    ROS_INFO("Delete nh_int result (1 for true): %d", r1);

    // 2. 使用 ros::param API 删除
    bool r2 = ros::param::del("param_int");
    ROS_INFO("Delete param_int result (1 for true): %d", r2);

    // 尝试删除一个不存在的键，看会返回什么
    bool r3 = ros::param::del("non_existent_key");
    ROS_INFO("Delete non-existent key result: %d", r3);

    return 0;
}
```
5. 编辑CMakeLists.txt文件，配置好环境
```bash
cd ~/catkin_ws/src/param_pkg
nano CMakeLists.txt
```
打开后在最下面写入：
```bash
add_executable(param_set src/param_set.cpp)
add_executable(param_get src/param_get.cpp)
add_executable(param_del src/param_del.cpp)

target_link_libraries(param_set ${catkin_LIBRARIES})
target_link_libraries(param_get ${catkin_LIBRARIES})
target_link_libraries(param_del ${catkin_LIBRARIES})
```
6. 编译并启动管理者
`cd ~/catkin_ws && catkin_make`后
新开一个终端进入容器，启动roscore

7. 将数据写入管理者的内存
新开一个终端进入容器，输入：
`source /opt/ros/noetic/setup.bash && source ~/catkin_ws/devel/setup.bash && rosrun param_pkg param_set`
之后系统会反馈`[INFO] [1793251033.626510897]: Parameters have been set and updated.`
代表数据已经成功写入。

8. 查看数据
(1).看看管理者现在手里都有哪些参数键名
      `rosparam list`
    之后会得到：
```bash
/nh_bool
/nh_double
/nh_int
/nh_map/guo
/nh_map/yuang
/nh_string
/nh_vector
/param_bool
/param_double
/param_int
/param_map/guo
/param_map/yuang
/param_string
/param_vector
/rosdistro
/roslaunch/uris/host_91a4707596b1__34535
/rosversion
/run_id
```

(2).查一下被改过的整数
`rosparam get /nh_int`
得到：10000

(3).查一下字符串
`rosparam get /nh_string`
得到：`hello NodeHandle`

(4).查看Vector
`rosparam get /nh_vector`
得到：
- zhangsan
- lisi

9. 运行读取节点，用c++来查看数据。
`rosrun param_pkg param_get`
之后终端会反馈：
```bash
[INFO] [1793251320.427600172]: param result: 10000, 100
[INFO] [1793251320.429621608]: getParam result: int=10000, string=hello NodeHandle
[INFO] [1793251320.429651613]: vector element: zhangsan
[INFO] [1793251320.429667203]: vector element: lisi
[INFO] [1793251320.429681840]: map element: guo = huang
[INFO] [1793251320.429695391]: map element: yuang = xiao
[INFO] [1793251320.430261156]: ros::param result: 20000
 
```
证明c++读取功能正常。

10. 最后运行删除节点，把参数删除干净
```bash
root@91a4707596b1:/# rosrun param_pkg param_del
[INFO] [1793257072.549147503]: Delete nh_int result (1 for true): 1
[INFO] [1793257072.550682610]: Delete param_int result (1 for true): 1
[INFO] [1793257072.551019866]: Delete non-existent key result: 0
root@91a4707596b1:/# rosparam get /nh_int
ERROR: Parameter [/nh_int] is not set
```
**总结**
1. 验证两套 API 是否等效？

- 在 param_set.cpp 中，nh.setParam 和 ros::param::set 都成功向服务器写入了数据。在 param_get.cpp 中，前者读出了 nh_int = 10000，后者读出了 param_int = 20000，证明它们操作的是同一个底层黑板，只是语法不同。

2. 验证复杂数据类型的支持？

- 终端成功打印出了 `vector element: zhangsan` 和 lisi，以及 `map element: guo = huang、yuang = xiao。`证明 ROS 参数服务器不仅能存数字，也能完美解析并还原数组和字典。

3. 验证全局可见性（终端与 C++ 互通）？
。
- C++ 程序跑完后（节点已退出），在终端敲 `rosparam get /nh_int`，成功拿到了 10000。随后跑了` param_del` 节点删除，再次在终端敲 `rosparam get /nh_int` 时，系统直接报红字 `Parameter [/nh_int] is not set`。这证明终端命令和 C++ 代码操作的是同一块内存，数据被真正抹除了。

### 5.ROS中的常用命令

#### (1). rosnode:获取节点信息
1. `rosnode ping`: 测试到节点的连接状态

2. `rosnode list`:列出活动节点

3. `rosnode info`:打印节点信息

4. `rosnode machine`:列出指定设备上的节点

5. `rosnode kill`:杀死某个节点

6. `rosnode cleanup`:清除无用节点，启动乌龟节点，然后 ctrl + c 关闭，该节点并没被彻底清除，可以使用 cleanup 清除节点

#### (2).rostopic : 操作话题
1. `rostopic list(-v)`:直接调用即可，控制台将打印当前运行状态下的主题名称

2. `rostopic list -v` : 获取话题详情(比如列出：发布者和订阅者个数...)

3. `rostopic pub`:可以直接调用命令向订阅者发布消息

比如：为roboware 自动生成的 发布/订阅 模型案例中的 订阅者 发布一条字符串
```bash
rostopic pub /主题名称 消息类型 消息内容
rostopic pub /chatter std_msgs gagaxixi
```

或者为 小乌龟案例的 订阅者 发布一条运动信息
```bash
rostopic pub /turtle1/cmd_vel geometry_msgs/Twist
 "linear:
  x: 1.0
  y: 0.0
  z: 0.0
angular:
  x: 0.0
  y: 0.0
  z: 2.0"
//只发布一次运动信息

rostopic pub -r 10 /turtle1/cmd_vel geometry_msgs/Twist
 "linear:
  x: 1.0
  y: 0.0
  z: 0.0
angular:
  x: 0.0
  y: 0.0
  z: 2.0"
// 以 10HZ 的频率循环发送运动信息
```

4. `rostpic echo`:获取指定话题当前发布的消息

5. `rostopic info`:获取当前话题的小关信息,消息类型,发布者信息,订阅者信息

6. `rostopic type`:列出话题的消息类型

7. `rostopic find` :根据消息类型查找话题

8. `rostopic delay`:列出消息头信息

9. `rostopic hz`:列出消息发布频率

10. `rostopic bw`:列出消息发布带宽

#### (3).rosmsg : 操作msg消息
1. `rosmsg list`:会列出当前 ROS 中的所有 msg

2. `rosmsg packages`:列出包含消息的所有包

3. `rosmsg package`:列出某个包下的所有msg
比如：
```bash
rosmsg package 包名 
rosmsg package turtlesim
```

4. `rosmsg show`:显示消息描述
比如：
```bash
//rosmsg show 消息名称
rosmsg show turtlesim/Pose
结果:
float32 x
float32 y
float32 theta
float32 linear_velocity
float32 angular_velocity
Copy
```
5. `rosmsg info`:作用与 rosmsg show 一样

#### (4).rosservice : 操作服务
1. `rosservice list`:列出所有活动的 service
比如：
```bash
~ rosservice list
/clear
/kill
/listener/get_loggers
/listener/set_logger_level
/reset
/rosout/get_loggers
/rosout/set_logger_level
/rostopic_4985_1578723066421/get_loggers
/rostopic_4985_1578723066421/set_logger_level
/rostopic_5582_1578724343069/get_loggers
/rostopic_5582_1578724343069/set_logger_level
/spawn
/turtle1/set_pen
/turtle1/teleport_absolute
/turtle1/teleport_relative
/turtlesim/get_loggers
/turtlesim/set_logger_level
```

2. `rosservice args`:打印服务参数
比如：
```bash
rosservice args /spawn
x y theta name
```

3. `rosservice call`:调用服务
比如：
```bash
为小乌龟的案例生成一只新的乌龟

rosservice call /spawn "x: 1.0
y: 2.0
theta: 0.0
name: 'xxx'"
name: "xxx"

//生成一只叫 xxx 的乌龟
```

4. `rosservice find`:根据消息类型获取话题

5. `rosservice info`:获取服务话题详情

6. `rosservice type`:获取消息类型

7. `rosservice uri`:获取服务器 uri

#### (5).rossrv : 操作srv消息

1. `rossrv list`:会列出当前 ROS 中的所有 srv 消息

2. `rossrv packages`:列出包含服务消息的所有包

3. `rossrv package`:列出某个包下的所有msg
比如：
```bash
//rossrv package 包名 
rossrv package turtlesim
```

4. `rossrv show`:显示消息描述
比如：
```bash
//rossrv show 消息名称
rossrv show turtlesim/Spawn
结果:
float32 x
float32 y
float32 theta
string name
---
string name
```

5. `rossrv info`:作用与 rossrv show 一致

6. `rossrv md5`:对 service 数据使用 md5 校验(加密)

#### (6).rosparam : 操作参数
1. `rosparam list`:列出所有参数
会得到：
```bash
/rosdistro
/roslaunch/uris/host_helloros_virtual_machine__42911
/rosversion
/run_id

```
2. `rosparam set`:设置参数
比如：
```bash
rosparam set name huluwa

//再次调用 rosparam list 结果
/name
/rosdistro
/roslaunch/uris/host_helloros_virtual_machine__42911
/rosversion
/run_id
```

3. `rosparam get`:获取参数
比如：
```bash
rosparam get name

//结果
huluwa
```

4. `rosparam delete`:删除参数
比如：
```bash
rosparam delete name

//结果
//去除了name
```

5. `rosparam load(先准备 yaml 文件)`:从外部文件加载参数

6. `rosparam dump`:将参数写出到外部文件
