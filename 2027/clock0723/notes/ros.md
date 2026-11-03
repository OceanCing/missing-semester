# ROS 第一节课笔记

## 1. ROS

ROS 全称 Robot Operating System（机器人操作系统），官方定义是“适用于机器人的开源元操作系统”：集成了大量工具、库、协议，提供类似操作系统的功能，简化对机器人的控制；还提供在多台计算机上获取、构建、运行代码的工具和库。设计者的表述是“ROS = Plumbing + Tools + Capabilities + Ecosystem”，也就是通信机制、工具软件包、机器人高层技能、生态系统四样东西的集合。

拆开看是三块：通信中间件（话题、服务、参数三种对话方式）、工具链（catkin 编译、launch 编排、rviz 和 rqt 可视化、rosbag 录包）、软件生态（OpenCV、PCL、Navigation、MoveIt 和各种硬件驱动）。一句话历史：2007 年斯坦福为 PR2 起头，2010 年 1.0 开源，2020 年 Noetic（本课用的版本），2025 年停止维护。

车上的流水线：相机驱动发 /image_raw → 识别节点发 /track → 解算节点发 /gimbal_cmd、/cmd_chassis、/cmd_vel → 决策节点管 power_limit、heat_limit → 通信中枢走 EtherCAT → 电机收 CAN 帧。记住两个词：节点（谁在干活）和话题（它们之间传什么）。

### 和 Windows、Linux 的区别

Windows、Linux 是操作系统，管的是窗口、驱动、进程、内存和文件，装上就能开机用；ROS 不是操作系统，而是跑在 Linux 之上的元操作系统，属于机器人开发框架，管的是节点之间的通信、工具、库和功能包，不能独立启动，下面必须先有一个 Linux。操作方式上，Windows 以图形界面为主，Linux 常用终端，ROS 则在终端里用 roscore、rosrun、roslaunch 启动；整套环境通常装在 Ubuntu 20.04 的虚拟机里。

### 优点

- **为机器人量身定做**：发布/订阅、坐标变换、参数、录包这些机器人常用能力，一开始就是一等公民
- **生态丰富**：几万个开源包，报错先搜社区，算法包拿来改比从零写快
- **便于团队分工**：rqt_graph 一眼看清谁连谁，每人只碰自己的节点，接口谈好就能并行
- **核心节点用 C++**：roscpp 性能可控，OpenCV、PCL、Eigen 本来就是 C++ 接口
- **可回放**：rosbag 把现场数据录下来，赛场问题回实验室一帧一帧复现
- **可仿真**：Gazebo 里先把逻辑跑通再上实车，不碰真车也能练

队伍的方案是纯上位机：视觉和控制逻辑全跑在上位机 miniPC 的 ROS 节点里，下位机只做 EtherCAT 到 CAN 的帧转换，没有控制逻辑。好处是改控制代码只要 catkin_make，不用烧固件；通信和计算解耦，逻辑越薄越不容易错。

## 2. ROS 的架构

ROS 的架构分三层：硬盘上的**文件系统**（静态）、运行时的**计算图**（动态）和**通信层**（TCPROS/UDPROS，管数据怎么走）。文件系统决定代码怎么放，计算图决定程序跑起来之后节点之间怎么连，通信层决定数据怎么传。排错先分层：找不到包查文件系统，连不上、收不到查计算图，延迟高、丢数据查通信层。

### 文件系统

文件系统是 ROS 源码在硬盘上的组织形式：工作空间包含功能包，功能包包含源码、配置和通信格式文件。

#### 文件结构

```text
WorkSpace --- 自定义的工作空间

    |--- build:编译空间，用于存放CMake和catkin的缓存信息、配置信息和其他中间文件。

    |--- devel:开发空间，用于存放编译后生成的目标文件，包括头文件、动态&静态链接库、可执行文件等。

    |--- src: 源码

        |-- package：功能包(ROS基本单元)包含多个节点、库与配置文件，包名所有字母小写，只能由字母、数字与下划线组成

            |-- CMakeLists.txt 配置编译规则，比如源文件、依赖项、目标文件

            |-- package.xml 包信息，比如:包名、版本、作者、依赖项...(以前版本是 manifest.xml)

            |-- scripts 存储python文件

            |-- src 存储C++源文件

            |-- include 头文件

            |-- msg 消息通信格式文件

            |-- srv 服务通信格式文件

            |-- action 动作格式文件

            |-- launch 可一次性运行多个节点 

            |-- config 配置信息

        |-- CMakeLists.txt: 编译的基本配置

```

源码和产物分离：src 进 Git，build 和 devel 不进；每开一个终端先执行 **source devel/setup.bash**，或者写进 ~/.bashrc。一个工作空间可以放很多包，但包名必须全局唯一，否则 **rosrun** 会认错。

#### 注意区别

##### 两个 src 的区别

工作空间的 src/ 放功能包，一个子目录就是一个包；包内的 src/ 放这个包的 C++ 源文件。

前者里的 CMakeLists.txt 由 **catkin_init_workspace** 生成，后者由 **catkin_create_pkg** 建出来，两个目录同名，层级差一层。

##### 很多个 CMakeLists 的区别

跑完 HelloWorld 示例，工作空间里有两个 CMakeLists：

| 位置                                      | 主要内容                                                     | 作用                                                         | 是否要修改 |
| ----------------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ---------- |
| ~/catkin_ws/src/CMakeLists.txt            | `cmake_minimum_required(VERSION 3.0.2)`<br>`project(Project)`<br>`set(CATKIN_TOPLEVEL TRUE)`<br>`catkin_workspace()` | 工作空间入口：<br>`project(Project)` 定顶层工程名<br>`set(CATKIN_TOPLEVEL TRUE)` 标记顶层<br>`catkin_workspace()` 扫 src/ 下的包，逐个执行包内 CMakeLists<br>统一配置 build/、devel/，按包间依赖排编译顺序<br>生成 setup.bash 等环境脚本 | 否         |
| ~/catkin_ws/src/demo_hello/CMakeLists.txt | `cmake_minimum_required(VERSION 3.0.2)`<br>`project(demo_hello)`<br>`find_package(catkin REQUIRED COMPONENTS roscpp std_msgs)`<br>`catkin_package()`<br>`include_directories(${catkin_INCLUDE_DIRS})`<br>`add_executable(hello src/hello.cpp)`<br>`target_link_libraries(hello ${catkin_LIBRARIES})` | 包的编译规则：<br>`find_package` 找依赖<br>`include_directories` 加 catkin 头文件目录<br>`add_executable` 把 src/hello.cpp 编成可执行文件 hello<br>`target_link_libraries` 链接 catkin 库 | 是         |

### 计算图层

计算图不是代码或文件，是 ROS 程序跑起来之后的动态结构：节点是各自跑着的进程，话题是节点之间传数据的通道，消息是通道上数据的格式，Master 把它们按名字对上。文件系统层解决“代码怎么组织”，计算图层解决“程序怎么协作”：谁发布、谁订阅。

Master 是计算图的中心：节点启动时来注册名字（谁发布什么、谁提供什么服务），订阅者来查询地址，Master 把双方介绍认识，还管参数服务器和节点存活。数据不经过 Master，所以 Master 挂了已经连上的照跑，新来的连不上。

#### rqt_graph

我们可以使用rqt_graph查看计算图层，它把运行中的节点和话题画成一张图：方框是节点，箭头是话题，方向就是数据流向，一眼看清谁连谁。分工上，**rqt_graph** 看全局，**rosnode info** 看单点，**rostopic echo** 看内容。

##### 命令

打开计算图窗口 **rqt_graph** （先起 **roscore** 和节点，图里才有东西）


#### 节点

一个独立跑着的进程，只干一件小事，类比车间里的一个工人，只管自己那道工序；工序之间的交接靠话题和服务。

节点与包的关系：包是编译和分发的单位，节点是运行的单位；节点写在功能包里，一个包可以有多个节点，

启动节点 **rosrun 包名 节点名** 

##### package

功能包（package）是 ROS 里最小的复用单位，一个文件夹就是一个包，包名必须和文件夹同名，里面放源码、配置和通信格式文件。创建、编译相关的命令上节课的笔记已经说过，这里省略。

###### package 的目录结构

```text
my_pkg/
├── CMakeLists.txt      编译规则：编什么、链什么
├── package.xml         名片：名字、版本、依赖
├── include/my_pkg/     对外暴露的头文件 .h / .hpp
├── src/                C++ 源文件 .cpp
├── scripts/            可执行脚本
├── msg/                自定义消息 .msg
├── srv/                自定义服务 .srv
├── launch/             启动文件 .launch
├── config/             参数文件 .yaml
└── README.md           给人看的：怎么编、怎么跑
```

include 是给别人用的头文件，src 是自己编的源文件；msg 和 srv 光建目录没用，要在 CMakeLists 里登记才会生成头文件。

###### package.xml

```xml
<?xml version="1.0"?>
<!--<package format="版本">（1 是旧的，2 是新版）-->
    <package format="2">
        
        <!--<name>包名（必须和文件夹同名）</name> -->
        <name>hello</name>
        
        <!--<version>版本号</version> -->
        <version>0.0.0</version>
        
        <!--<description>描述</description> -->
        <description>The hello package</description>
        
        <!--<maintainer email="作者邮箱">作者名字</maintainer> -->
        <maintainer email="root@todo.todo">root</maintainer>
        
        <!--<license>许可证（TODO 必须换成真实许可证）</license> -->
        <license>TODO</license>
        
        <!--<buildtool_depend>catkin</buildtool_depend> -->
        <buildtool_depend>catkin</buildtool_depend>
        
        <!--<build_depend>编译期依赖</build_depend> -->
        <build_depend>roscpp</build_depend>
        <build_depend>std_msgs</build_depend>
        
        <!--<build_export_depend>导出依赖</build_export_depend> -->
        <build_export_depend>roscpp</build_export_depend>
        <build_export_depend>std_msgs</build_export_depend>
        
        <!--<exec_depend>运行期依赖（漏写运行时会报找不到库）</exec_depend> -->
        <exec_depend>roscpp</exec_depend>
        <exec_depend>std_msgs</exec_depend>
        
        <!--<export>导出信息</export> -->
        <export>
        </export>
        
    </package>
```

##### 节点

ROS 节点的基本框架

```cpp
#include <ros/ros.h>
int main(int argc, char** argv) {
    // ros::init(argc, argv, "节点名")
    // argc、argv来自输入的命令行参数，如 rosrun 包名 节点名，则argc是数量，argv的元素是包和节点的名字
    ros::init(argc, argv, "hello"); 
    // ros::NodeHandle 变量名
    ros::NodeHandle nh; // 类：含 advertise/subscribe/param
    // ros::spin()
    ros::spin(); // 死循环处理回调
    return 0;
}
```

`ros::init` 必须最先调用，之后才能造 `NodeHandle`；节点名不能和正在跑的重名，否则后起的那个会被踢掉。

### 通讯层

节点之间怎么说话：话题、服务、参数三种方式，底层都走 TCPROS/UDPROS。ros_control 不发明新协议，只是把话题和服务用在控制上：controller（PID、轨迹、差速）只算指令，RobotHW 的 read()/write() 和 CAN、EtherCAT 换数据，Controller Manager 管控制器的加载、启动和切换；换电机只改 hardware_interface，换算法只换 controller。

#### 话题通信

话题通讯是 ROS 最常见的通信方式，用于节点之间异步、持续传输消息，一端发布多端订阅，是发布/订阅模式：发布者往话题里写，订阅者从话题里读，双方互不认识，只靠话题名和消息类型对上。

话题通信是异步的，发布完就返回，不等订阅者；一个话题可以有多个发布者和多个订阅者；queue_size 是队列长度，发得比收得快时满了丢旧数据；订阅者晚启动也能自动重连，除非开了 latching。

##### 过程

```text
发布者启动 → 向 Master 注册
订阅者启动 → 向 Master 注册
Master 匹配 → 把发布者的 IP 和端口发给订阅者
订阅者 → 连发布者 → 确认消息类型一致
发布者 → 消息走 TCP 直连 → 订阅者，之后 Master 不再出现
```

##### 话题

一条有名字的广播频道，是节点之间传数据的通道，名字全局唯一；发布者写，订阅者读，同一个话题两端必须同名同类型。

在小乌龟中**/turtle1/cmd_vel** 用 **geometry_msgs/Twist**，键盘节点发布，乌龟节点订阅；**/chatter** 用 **std_msgs/String**，talker 节点发布，listener 节点订阅。

##### message

消息是话题上传输的数据结构，由 .msg 文件定义字段，类比信封的格式；话题名对上、类型也要对上。每个消息类型名称都由 **包名/消息名** 组成，如`std_msgs/String`，`turtlesim/Pose`ROS 提供了标准的消息包。

**std_msgs**：提供 String、Int32、Bool 等基础类型包装及 Header 等通用消息，适合简单数据传递和快速原型。

**sensor_msgs**：统一相机、激光雷达、IMU、GPS 等传感器数据的标准格式。

**geometry_msgs**：描述点、向量、四元数、位姿、速度等空间几何与运动信息。

**nav_msgs**：承载里程计、路径、占据栅格地图等导航相关数据。其他常见包还有 diagnostic_msgs（诊断信息）、trajectory_msgs（轨迹）、visualization_msgs（可视化标记）。

###### **自定义message**

系统的massage有时不能够满足实际需求，因此我们要使用自定义massage

使用步骤

1. 在包文件夹新建 **消息类型名.msg文件**

  2. 写入消息格式（类似结构体，一行一行写）

  3. 修改package.xml,增加

  ```xml
     <build_depend>message_generation</build_depend>
     <exec_depend>message_runtime</exec_depend>
  ```

​	以及Cmakelist

  ```cmake
 find_package(增加 message_generation)
 add_message_files(之前新建的msg文件)
 generate_messages(msg文件的依赖，即里面用到的类型的依赖)
 catkin_package(增加message_runtime)
  ```

之后会在**/include**生成对应的库，代码包含库头文件后就可以使用自定义message类型

##### publisher

发布者，往话题里写数据的一端，用 `advertise` 创建，创建时定话题名、消息类型和队列长度。

```cpp
#include <ros/ros.h>
//使用到的message相关的库
#include <std_msgs/String.h>
int main(int argc, char** argv) {
    //设置编码，每则中文输出乱码
    setlocale(LC_ALL,"");  
    ros::init(argc, argv, "talker");
    ros::NodeHandle nh;
    // ros::Publisher 变量名 = 句柄.advertise<消息类型>("话题名", 队列长度)
    ros::Publisher pub = nh.advertise<std_msgs::String>("/chatter", 10); 
    // ros::Rate 变量名(频率)
    ros::Rate rate(1); 
    //设置休眠，保证subscriber注册，防止丢失信息
    ros::Duration(3.0).sleep()
    // while (ros::ok()) 设置循环
    while (ros::ok()) { 
        // 消息类型 消息名 （实例化后赋值）
        std_msgs::String msg; 
        msg.data = "hello";
        // 发布者.publish(消息变量)
        pub.publish(msg);
        //打印到屏幕上（语法类似printf）
        ROS_INFO("Published: %s", msg.data.c_str());
        //有回调函数建议使用
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}
```

##### subscriber

订阅者，从话题里读数据的一端，用 `subscribe` 创建，注册一个回调函数，消息到了就在回调里处理。

```c++
#include <ros/ros.h>
#include <std_msgs/String.h>
// void 回调函数名(const 消息类型::ConstPtr& 消息名)，函数内容是对变量的处理，constptr本质只读智能指针
void chatterCallback(const std_msgs::String::ConstPtr& msg) {
    ROS_INFO("I heard: %s", msg->data.c_str());
}
int main(int argc, char** argv) {
    ros::init(argc, argv, "listener");
  	ros::NodeHandle nh;
    // ros::Subscriber 变量名 = 句柄.subscribe("话题名", 队列长度, 回调函数名)
    ros::Subscriber sub =
        nh.subscribe("/chatter", 10, chatterCallback); 
    // ros::spin()
    ros::spin(); // 死循环收消息
    return 0;
}    
```

##### 命令

列出所有活跃话题 **rostopic list**

把话题上的内容打到终端 **rostopic echo 话题名**

看谁在发布、谁在订阅、类型是什么 **rostopic info 话题名**

测发布频率 **rostopic hz 话题名**

按固定频率手动发一条 **rostopic pub -r 频率 话题名 消息类型** （频率和消息内容按需要填，调发布者不用写代码）

#### 服务通讯

服务通信是基于“请求/应答”的同步通信方式：客户端发一次请求，服务端处理后返回一次应答；一个服务名只对应一个服务端实现，但可以有多个客户端调用。用于偶然的、对实时性有要求、有一定逻辑处理需求的数据传输场景，多用于发布**控制命令**和**查询操作**。

##### 过程

```text
服务端启动 → 向 Master 注册服务名
客户端启动 → 向 Master 注册并查询服务名
Master 匹配 → 把服务端的 TCP 地址发给客户端
客户端 → 连服务端 → 发送请求
服务端 → 处理请求 → 返回应答 → 客户端
```

##### srv文件

srv 文件定义服务通信的请求和应答数据结构，放在功能包根目录下的 srv/ 文件夹里

##### srv的使用 (和自定义message的很像)

1. 创建 **服务文件名.srv**
  格式   (使用**---**分隔请求和响应)

   客户端请求时发送的数据										如：客户端请求时发送两个数字				

​	**数据类型 变量名1**												int32 num1
​	**数据类型 变量名2**												int32 num2
 	**---**															---	
​    服务器响应发送的数据 											服务器响应发送的一个数字			
​	**数据类型 变量名3**												int32 sum
​	**数据类型 变量名4** 																							


2. 修改package.xml和CmakeLists

   修改package.xml和自定义message一样

   CMakeLists 里只是自定义message的把msg 文件换成 srv文件：

   ```cmake
   find_package(增加 message_generation)
   add_service_files(之前新建的srv文件)
   generate_messages(msg文件的依赖，即里面用到的类型的依赖)
   catkin_package(增加message_runtime)
   ```
 ##### 服务端代码
```cpp
// #include "包名/srv名.h"
#include "add_node/Add.h"

//bool 回调函数名（包名::srv名::Request& 请求结构体名，包名::srv名::Response& 响应结构体名）
bool doReq(add_node::Add::Request& req, add_node::Add::Response& resp) {
    // 对请求数据进行处理，给响应变量赋值
    resp.sum = req.num1 + req.num2;
    return true;
}
int main(int argc, char** argv) {
    ros::init(argc, argv, "add_server");
    ros::NodeHandle nh;
    // ros::ServiceServer 变量名 = 句柄.advertiseService("服务话题名", 回调函数名)
    ros::ServiceServer server = nh.advertiseService("add", doReq);
    ros::spin();
    return 0;
}  
```

##### 客户端代码

```cpp
#include "add_node/Add.h"

// 函数返回类型 函数名（参数类型 参数名，参数类型 参数名）
int main(int argc, char** argv) {
   
    ros::init(argc, argv, "add_client");
    ros::NodeHandle nh;
    // ros::ServiceClient 变量名 = 句柄.serviceClient<包名::srv名>("服务名")
    ros::ServiceClient client = nh.serviceClient<add_node::Add>("add");
    // ros::service::waitForService("服务名")
    ros::service::waitForService("add");
    // 包名::srv名 变量名
    add_node::Add add_srv;
    // 给请求结构体的变量赋值
    add_srv.request.num1 = 1;
    add_srv.request.num2 = 2;
    // if (变量名.call(请求响应对象)) {  // 条件判断
    if (client.call(add_srv)) {
        // ROS_INFO("格式串", 变量名.response.字段名)
        ROS_INFO("结果:%d", add_srv.response.sum);
    }
    // return 0
    return 0;
}
```

#####  命令
列出所有服务  **rosservice list** 
看类型  **rosservice type 服务名**，
手工调一次  **rosservice call 服务名 参数**
看 .srv 的字段定义  **rossrv show 包名/服务类型**

#### 参数服务器

参数服务器是 Master 里的一个公共容器：Talker 把参数名和值存进去，Listener 按参数名取出来，双方不直接通信。它不传数据，只是一份全局配置，随 roscore 存在，roscore 一关就没了。能存的数据类型有 32 位整数、bool、string、double、ISO8601 日期、list、字典和 base64 二进制；它不是为高性能设计的，适合静态的、非二进制的简单数据。特点是以静态配置为主，节点启动时读一次；谁都能 set，改完所有节点下次 get 都能看到；ROS 1 里改参数不会触发回调，要自己轮询；改参数不用重启节点。用处是阈值、相机内参、PID 参数、串口波特率这类启动配置。

##### 过程

```text
Talker → RPC 传参数名和值 → Master 存进参数列表
Listener → RPC 按参数名查询 → Master 查参数列表
Master → RPC 返回参数值 → Listener
```

三个角色：ROS Master 是管理者，Talker 是参数设置者，Listener 是参数调用者。

C++ 有两套 API，ros::NodeHandle 和 ros::param，功能一一对应。

参数名有三种写法：/robot/speed 是绝对名，从根开始，换命名空间也指同一个地方；~speed 是私有名，等价于 /节点名/speed，只属于这个节点；speed 是相对名，拼在当前命名空间后面，随 launch 的 ns 变。

##### 新增和修改

参数服务器本质是键值存储：键在**同一命名空间下唯一**，一个键对应一个值，用相同键再次设置会覆盖旧值而不是新增；不同命名空间下可以存在同名键

```cpp
// #include <ros/ros.h>
#include <ros/ros.h>

// 函数返回类型 函数名（参数类型 参数名，参数类型 参数名）
int main(int argc, char** argv) {
    ros::init(argc, argv, "set_update_param");
    ros::NodeHandle nh;
    // nh.setParam("键", 值)
    nh.setParam("nh_int", 10);
    nh.setParam("nh_double", 3.14);
    nh.setParam("nh_bool", true);
    nh.setParam("nh_string", "hello NodeHandle");
    // 键相同、值不同就是修改
    nh.setParam("nh_int", 10000);
    // ros::param::set("键", 值)
    ros::param::set("param_int", 20);
    ros::param::set("param_int", 20000); // 修改
    return 0;
}
```

list 和字典也能存，分别用 std::vector 和 std::map 传进去。

##### 查询

```cpp
// #include <ros/ros.h>
#include <ros/ros.h>

// 函数返回类型 函数名（参数类型 参数名，参数类型 参数名）
int main(int argc, char** argv) {
    ros::init(argc, argv, "get_param");
    ros::NodeHandle nh;
    // nh.param("键", 默认值)：存在返回真实值，不存在返回默认值
    int res1 = nh.param("nh_int", 100);
    int res2 = nh.param("nh_int2", 100);
    // nh.getParam("键", 变量)：存在返回 true 并赋值
    int value = 0;
    bool ok = nh.getParam("nh_int", value);
    // nh.getParamCached("键", 变量)：带缓存，高频读用它
    nh.getParamCached("nh_int", value);
    // nh.hasParam("键")：判断键在不在
    bool exist = nh.hasParam("nh_int");
    // nh.searchParam("键", 结果变量)：搜索键
    std::string key;
    nh.searchParam("nh_int", key);
    // nh.getParamNames(结果变量)：拿到所有键
    std::vector<std::string> names;
    nh.getParamNames(names);
    // ros::param::param("键", 默认值)、ros::param::get("键", 变量)、ros::param::has("键")
    int res3 = ros::param::param("param_int", 20);
    return 0;
}
```

##### 删除

```cpp
// #include <ros/ros.h>
#include <ros/ros.h>

// 函数返回类型 函数名（参数类型 参数名，参数类型 参数名）
int main(int argc, char** argv) {
    ros::init(argc, argv, "del_param");
    ros::NodeHandle nh;
    // nh.deleteParam("键")：删除成功返回 true，键不存在返回 false
    nh.deleteParam("nh_int");
    // ros::param::del("键")
    ros::param::del("param_int");
    return 0;
}
```

读参数时类型要和 set 时一致，类型对不上会抛异常。

##### YAML 文件

参数一多就写进 .yaml 文件，用 **rosparam load 文件名 命名空间** 一次灌入。缩进只用空格，一级两个空格；冒号后必须有空格；大小写敏感；# 是注释；--- 分隔多个文档。标量直接写值，列表每行用 - 或写成 [值1, 值2]，字典靠缩进或写成 {键1: 值1, 键2: 值2}。

```yaml
# 键:（下面缩进子键，一层两个空格）
turtlesim:
  background_r: 69
  background_b: 255
  holonomic: false

# 键:（下面缩进子键，一层两个空格）
my_node:
  # 键: [值1, 值2, 值3]（列表行内写法）
  thresholds: [0.3, 0.6, 0.9]
  camera:
    # 子键: 值（数值）
    exposure: 30
```

也能在 launch 里加载：

```xml
<!--<launch></launch>-->
    <launch>
        
        <!--<node pkg="包名" type="可执行文件名" name="节点名"/> -->
        <node pkg="hello" type="talker" name="talker"/>
        
        <!--<rosparam file="$(find 包名)/config/文件名.yaml" command="命令（load、dump、delete）" ns="命名空间"/> -->
        <rosparam file="$(find hello)/config/params.yaml" command="load" ns="my_node"/>
        
    </launch>
```

一个 launch 里能写多个 rosparam 标签

##### 命令

列出所有参数 **rosparam list** （参数多时配合 **grep** 过滤）

读一个参数 **rosparam get 参数名**

改一个参数 **rosparam set 参数名 值** （改完不用重启节点）

删掉一棵参数子树 **rosparam delete 命名空间**

把参数导出成 YAML **rosparam dump 文件名 命名空间**

从 YAML 文件批量灌入 **rosparam load 文件名 命名空间**

