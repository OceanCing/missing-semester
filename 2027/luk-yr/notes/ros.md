# ROS 学习笔记

## 1.ROS是什么

ROS 全称叫机器人操作系统。
可以把它理解为运行在 Linux 上的机器人通信中间件和工具箱。

ROS 主要提供：

- 通信机制：话题，服务，参数
- 工具链：catkin编译、launch编排、RViz/rqt可视化
- 软件生态：OpenCV、PCL、相机、雷达

ROS在车上ROS在车上运行过程，相机看到目标到轮子到过程

**1 相机驱动：把相机的数据拿进程序。**
**2 识别／跟踪：从图像中找到目标**
**3 解算：把看到的信息变成需要的数值**
**4 决策：结合当前条件决定动作**
**5 通信模块：把软件指令送到设备**
**6 电机：执行动作**

## 2.为什么使用ROS

### 1.它能替我们省下哪几件事

- ROS 是为机器人量身定做的系统

- 生态丰富，调试工具齐全，拥有庞大的开源社区

- 代码可视化，调试方便，便于团队分工分模块合作

- 核心节点统一用 C++ 写

- 可回放：rosbag 把现场数据录下来

- 可仿真：Gazebo 里先把逻辑跑通

### 2.纯上位机 与 上位机 + 下位机 的区别

### 1.上位机

控制视觉组的方案：纯上位机

上位机：机器人身上的小电脑
运行 Ubuntu、ROS 和控制程序
电脑负责读反馈、算调整，再通过 EtherCAT 把指令发给通信转换设备，转换成 CAN 帧后发送至电机侧设备

特点：算法集中在电脑，修改、调试方便；要关注电脑和通信的时序

### 2.上位机+下位机

上位机＋下位机分担控制：
电脑发送目标，控制板负责读反馈、算调整，再控制电机

特点：控制板在本地持续调整，减少往返通信依赖；需要维护两端程序和接口

### 3.区别

**纯上位机方案**：上位机读反馈、算调整；下位机侧设备主要转发数据，并处理心跳、急停等保护

**上下位机分担方案**：下位机除了通信，还负责读取反馈、计算调整量，完成部分底层闭环

## 3.ROS架构

### 1.文件系统层

代码和配置怎么组织、放在哪里。

#### 1.工作空间 —— 功能包 —— 源码、配置等文件

```
dynamicx@standard:~/catkin_ws$ catkin_ws —— 工作空间（workspace）
catkin_ws/
├── src/            # 你的代码都在这里（进版本库）
│   ├── CMakeLists.txt   ← 软链接到 /opt/ros/...
│   └── my_pkg/          ← 一个功能包
├── build/          # 编译中间文件（可删，别进 git）
├── devel/          # 编译产物：setup.bash、可执行文件
    └── setup.bash  # 每开一个终端都要 source 它

```

#### 2.ROS 包里到底有什么

```
dynamicx@standard:~/catkin_ws$ package —— 功能包（ROS 里最小的复用单位）
my_pkg/
├── CMakeLists.txt      # 编译规则：编什么、链什么
├── package.xml         # 名片：名字、版本、依赖
├── include/my_pkg/     # 对外暴露的头文件 .h / .hpp
├── src/                # C++ 源文件 .cpp
├── scripts/            # 可执行脚本（我们以 C++ 为主，这里放小工具）
├── msg/                # 自定义消息 .msg
├── srv/                # 自定义服务 .srv
├── launch/             # 启动文件 .launch（一次拉起多个节点）
├── config/             # 参数文件 .yaml
└── README.md           # 给人看的：怎么编、怎么跑

```

### 2.计算图层

看程序运行以后，谁在工作、彼此怎么协作

#### 1.ROS 程序的五个基本零件

- 节点	独立跑着的程序，负责具体任务，如遥控节点：读取键盘操作，发出目标速度
- 话题	有名字的数据频道 ，发布：向话题发送数据 订阅：接收这个话题的数据
- 消息	话题上传输的数据及其格式
- 服务	发出请求，处理后返回应答
**话题适合连续发布速度、图像等数据；服务适合清零、查询、切换模式等需要返回结果的操作**
- 参数	供程序读取和使用的配置，如
```
最大速度：0.5
轮子半径：0.08
PID比例系数：2.0
```
#### 2.ROS Master

这些节点怎么找到彼此？就会用到Master
ROS Master帮助节点登记信息、找到彼此的服务程序

Master 到底管什么

1. 名字注册
   谁发布了什么话题、谁提供了什么服务
   2. 名字查询
      按名字把双方介绍给对方
      3. 参数服务器
         全局参数的存放地（后面的 YAML 就住这儿）
         4. 状态监控
            谁还活着、谁掉线了

Master 只做注册和查询

## 4.建立ROS包

**1.package.xml：这张「名片」决定谁能找到你**

```
<?xml version="1.0"?>
<package format="2">
  <name>my_pkg</name> //包名，必须和文件夹同名；改了文件夹不会自动改这里//
  
  <version>0.1.0</version>
  
  <description>我的第一个 ROS 包</description>
  
  <maintainer email="you@example.com">Your Name</maintainer>
  
  <license>MIT</license>
  
  <buildtool_depend>catkin</buildtool_depend>
  
  <build_depend>std_msgs</build_depend>
  //编译这个包时需要的东西：头文件、库、消息包//
  
  <exec_depend>message_runtime</exec_depend>
  //运行时需要的东西：动态库、消息运行库、launch里调用的包//
  
  <depend>roscpp</depend>
  //depend= build + build_export + exec，一般写这个最省事//
  //自定义msg/srv：要额外加message_generation（编译期）和message_runtime（运行期）//
</package>
```

**2.CMake基础用法见：[CMake与catkin笔记](cmake_study.md)。**

## 5.三大通信

*分别解决三件事：**持续传数据、请求办事并拿到结果、保存配置***

  ### 1.话题通信 Topic

  有名字的数据频道，发布者往里面发数据，订阅者接收数据。

  键盘节点 —— 发布目标速度到 /turtle1/cmd_vel —— 小乌龟节点接收并移动

  **1.组成：四个角色**

- **发布者 Publisher** 往话题里写数据的节点
- **订阅者 Subscriber** 从话题里读数据的节点
- **消息 Message** 数据的结构，由 .msg 定义（Twist、Image…）
- **名字 + 类型** 同名 + 同类型才能对上，类型不一致直接连不上

  **2.特点**：为什么适合数据流

- 异步：发完就返回，不等任何人

- 多对多：一个话题可以有很多发布者和订阅者

- 有队列：queue_size 是缓冲深度，满了会丢旧数据

- 断线自动重连：订阅者晚启动也能接上（除非 latching）

**3.发布端：往话题里发消息**

```
// talker.cpp —— 发布端

#include <ros/ros.h>
#include <std_msgs/String.h>  // 使用 String 消息类型

int main(int argc, char** argv) {
  ros::init(argc, argv, "talker");  // 初始化 ROS，节点名为 talker
  ros::NodeHandle nh;              // 通过 nh 创建发布者、订阅者等

  ros::Publisher pub =
      nh.advertise<std_msgs::String>("/chatter", 10);
  // 创建发布者：话题名 /chatter，类型 String，队列长度 10

  ros::Rate rate(1);  // 期望每秒循环 1 次，即 1 Hz

  while (ros::ok()) {       // ROS 正常运行时继续循环
    std_msgs::String msg;  // 创建一条消息
    msg.data = "hello";    // 填入消息内容
    pub.publish(msg);      // 发布消息
    rate.sleep();          // 等待，控制循环频率
  }
}
```

**4.订阅端：收到消息后进行处理**

```
// listener.cpp —— 订阅端

#include <ros/ros.h>
#include <std_msgs/String.h>

void cb(const std_msgs::String::ConstPtr& msg) {
  ROS_INFO("I heard: %s", msg->data.c_str());
}
// cb：回调函数，收到消息后调用它
// msg：指向收到消息的只读指针
// ROS_INFO：输出日志

int main(int argc, char** argv) {
  ros::init(argc, argv, "listener");
  ros::NodeHandle nh;

  ros::Subscriber sub =
      nh.subscribe("/chatter", 10, cb);
  // 订阅 /chatter，队列长度 10，收到消息交给 cb 处理

  ros::spin();  // 持续等待并处理回调，直到节点退出
}
```

### 2.服务通信 Service

发出一次请求，对方处理后返回应答。

客户端请求清零 —— 服务端执行清零 —— 返回是否成功

**1.组成**

- 服务端 Server：提供能力，注册一个回调处理请求
- 客户端 Client：主动发起调用，等结果
- Request / Response：请求和应答两段数据结构
- .srv 文件：上下两段用 --- 隔开，上面请求、下面应答

**2.特点**

- 同步阻塞：调用期间客户端线程停住
- 一对一：一个服务对应一个实现
- 服务端没起来就报错，要 wait_for_service
- 高频场景不要用：每次调用都要建连、序列化

**3.服务的数据格式**

以 turtlesim/Spawn 为例：请求生成一只乌龟，返回新乌龟的名字。

```
# Spawn.srv

float32 x       # 请求：横坐标
float32 y       # 请求：纵坐标
float32 theta   # 请求：朝向，单位是弧度
string name     # 请求：希望使用的名字
---
string name     # 应答：生成的乌龟名字
```

**--- 上面是请求，下面是应答。**

如果是自己定义的 .srv，需要在 CMakeLists.txt 中配置服务生成，并配好依赖：

```
add_service_files(FILES Spawn.srv)          # 指定服务定义文件
generate_messages(DEPENDENCIES std_msgs)   # 生成代码所需的头文件
```

**4.命令行调用**

```
rosservice list
# 列出当前所有服务

rosservice type /spawn
# 查看 /spawn 的服务类型，本例是 turtlesim/Spawn

rosservice call /spawn "{x: 2.0, y: 3.0, theta: 0.0, name: 't2'}"
# 发出请求：在（2，3）的位置生成朝向为 0、名字叫 t2 的小乌龟

name: "t2"
# 返回应答：生成的小乌龟名字
```

**5.C++ 客户端调用**

```
ros::ServiceClient cli =
    nh.serviceClient<turtlesim::Spawn>("/spawn");
// 创建客户端 cli，指定服务名 /spawn、服务类型 turtlesim::Spawn

turtlesim::Spawn srv;
// 创建服务数据对象 srv，里面包含请求 request 和应答 response

srv.request.x = 2.0;       // 填入请求：横坐标为 2
srv.request.name = "t2";   // 填入请求：希望名字为 t2
// 这里没有设置 y 和 theta，使用默认值 0

if (cli.call(srv))         // 发出请求，等待应答；调用成功后执行下面一行
  ROS_INFO("new: %s", srv.response.name.c_str());
  // 打印应答中的名字，如 new: t2
```

**request：调用前填写要发出的数据；response：调用成功后读取返回的数据。**

### 3.参数服务器 Parameter Server

集中保存配置，节点按参数名读取和修改。

参数服务器存着轮子半径 —— 控制节点读取 —— 用这个数值进行计算

**1.组成**

- 全局 key-value：存在 Master 里，所有节点都能访问
- 树形命名空间：/robot/camera/exposure 这样一层层往下
- 六种类型：int、double、bool、string、list、dict
- 生命周期：随 roscore 存在，roscore 一关就没了

**2.特点**

- 以静态配置为主：启动时读一次
- 谁都能 set，改完所有节点下次 get 都能看到
- ROS 1 里改参数不会触发回调（要自己轮询）
- 改参数不用重启节点 —— 这就是它最大的价值

**修改服务器上的参数后，程序要再次读取，才会使用新值。只在启动时读一次的程序，不会自动跟着改变。**

```
# 单个读写
$ rosparam set /cam/exposure 30
$ rosparam get /cam/exposure
30

# 看全部，再过滤
$ rosparam list | grep cam

# 导出成文件 / 从文件灌入
$ rosparam dump my_params.yaml /cam
$ rosparam load my_params.yaml /cam

```

```
dynamicx@standard:~/catkin_ws$ cat src/gimbal_node.cpp
// ① 读一个值，带默认值最稳
int exposure;
nh.param<int>("camera/exposure", exposure, 30);

// ② 私有参数：只属于这个节点
ros::NodeHandle pnh("~");
double gain = 1.0;
pnh.param<double>("gain", gain, 1.0);

// ③ 读整棵子树（类型任意）
XmlRpc::XmlRpcValue tree;
nh.getParam("camera", tree);

```



### 4.三种方式的区别

**话题：告诉你现在的数据。**
例如：现在要求机器人以 0.5 m/s 前进。

**服务：请你做一件事，并告诉我结果。**
例如：把里程计清零，返回是否成功。

**参数：告诉程序按照什么配置工作。**
例如：最大速度设为 0.8 m/s，轮子半径设为 0.08 m。

一个节点可以同时发布话题、订阅话题、提供服务和读取参数。

## 6.配置文件 YAML

YAML：把配置写进文件，再加载到参数服务器。

### 1.YAML 的五条硬规矩

- 只用空格缩进，不用 Tab；通常两个空格一级，同一层对齐
- 冒号后面留空格，如 key: value
- 参数名大小写敏感，布尔值统一写 true / false
- \# 是注释，可以整行，也可以在行尾；值里需要 # 时加引号
- --- 可以分隔多个文档

检查：编辑器里显示 Tab，看缩进是否对齐，再用命令加载、查看。

### 2.三种结构：标量、列表、字典

**1.标量：单个值**

```text
speed: 1.5      #数字
name: "sentry"  #字符串
debug: true     #布尔值
empty: null     #YAML空值，ROS 1常规参数不直接使用null
```

**2.列表：按顺序放多个值**

```text
ports:
  - 8080
  - 8081

ids: [1, 2, 3]  #行内写法
```

**3.字典：名字和值对应**

```text
camera:
  width: 640
  height: 480

point: {x: 1, y: 2}  #行内写法
```


### 3.在 ROS 里怎么用 YAML

```
# ① 命令行：临时调试最方便
$ rosparam load config/params.yaml /my_ns

# ② launch 文件里（最常用）
<rosparam file="$(find my_pkg)/config/params.yaml"
          command="load" ns="my_ns"/>

# ③ 代码里读（带默认值最稳）
ros::NodeHandle pnh("~");  /
int exposure;
pnh.param<int>("camera/exposure", exposure, 30);

```



## 7.常用命令

```text
#包与编译
rospack find <pkg>                 #查找包的位置
roscd <pkg>                        #进入包目录
rosls <pkg>                        #查看包里的文件
catkin_create_pkg <name> <deps>     #创建包，指定包名和依赖
catkin build                      #编译

#运行与节点
roscore                           #启动ROS基础服务
rosrun <pkg> <node> <参数>          #运行包里的程序
roslaunch <pkg> <file.launch>      #按launch文件启动

#话题
rostopic list                     #列出话题
rostopic echo <topic>             #查看话题内容
rostopic info <topic>             #查看发布者、订阅者和消息类型
rostopic hz <topic>               #测频率
rostopic pub -r 10 <topic> <type> <消息内容>
#手动发布，-r 10表示每秒10次

#服务与消息
rosservice list                   #列出服务
rosservice type <服务名>           #查看服务类型
rosservice call <服务名> <请求内容> #调用服务
rosservice find <srv_type>         #按类型查找服务
rosmsg show <pkg/Type>             #查看消息字段
rossrv show <pkg/Type>             #查看请求和应答字段

#参数
rosparam list                     #列出参数
rosparam get <参数名>             #读取参数
rosparam set <参数名> <值>        #设置参数
rosparam load <file> [ns]          #从YAML加载，ns是可选命名空间
rosparam dump <file> [ns]          #导出成YAML
rosparam delete <ns>               #删除这个名字下的参数

#调试与录制
rviz                              #查看模型、点云、坐标系等
rosbag record -a                   #录制所有话题
rosbag info <bag>                  #查看录制文件的信息
rosbag play <bag>                  #回放数据
```

