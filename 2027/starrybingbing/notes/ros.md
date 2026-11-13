

# 参考：

- [ROS Wiki](https://wiki.ros.org/)
- [赵虚左 ROS1 讲义](https://www.autolabor.com.cn/book/ROSTutorials/)
- [课程仓库](https://github.com/OceanCing/missing-semester)

---

# 1. 认识 ROS

## 1.1 一句话定义

官方原话是这样的：

> ROS 是一个开源的元操作系统：它提供操作系统该提供的那些服务 —— 硬件抽象、底层设备控制、常用功能的实现、进程间消息传递、软件包管理。

「元操作系统」（meta-operating system）这个词第一次看很唬人，其实就是说：**它不是操作系统，但它干了操作系统该干的一部分活**，然后跑在 Linux 上面。开机不会进 ROS 桌面，你还是在 Ubuntu 里敲命令。

所以更准确的说法是：**跑在 Linux 之上的机器人中间件 + 工具箱**。

## 1.2 三件东西

**① 通信中间件** —— 节点之间唯一的对话方式

- 话题：连续的数据流
- 服务：一次问答
- 参数：全局配置

**② 工具链** —— 编译、启动、看、录

- catkin 编译 / launch 编排
- rviz、rqt 可视化
- rosbag 录包回放

**③ 软件生态** —— 几万个开源包，C++ 接口居多

- OpenCV / PCL / Eigen
- Navigation / MoveIt
- 相机、雷达、IMU 的驱动

## 1.3 版本

我们用 **ROS 1 Noetic**，跑在 **Ubuntu 20.04** 上，节点统一用 C++（roscpp）。

一句话历史：2007 年斯坦福为 PR2 起头 → 2010 年 1.0 开源 → 2020 年 Noetic（ROS 1 的最后一个版本）→ 2025 年停止维护。

> 停维护不等于不能用。队里的车跑的就是 Noetic，生态最全、资料最多，出问题一搜就有答案。ROS 2 现在还没有到「什么包都有」的程度。

## 1.4 在我们的车上：一条从相机到轮子的流水线

抽象讲完，看具体。我们车上的一条链路是这样的：

```
相机 /image_raw  →  识别 /track  →  解算 /gimbal_cmd · /cmd_chassis · /cmd_vel
                 →  决策 power_limit · heat_limit  →  EtherCAT  →  CAN 帧  →  电机
```

一条链路上，每一段都是一个或几个**节点**，节点之间用**话题**连着。这条链路里，ROS 帮我们省掉的是四件事：

- **驱动与硬件抽象** —— 相机、激光、IMU、串口 / CAN 都有现成的驱动包，不用从寄存器写起。例：`usb_cam`、`velodyne_driver`、`serial`。
- **算法库** —— OpenCV、PCL、Eigen、Navigation、MoveIt —— 全是 C++ 接口，拿来就用。例：`cv::solvePnP`、`pcl::VoxelGrid`、`tf2`。
- **调试与可视化** —— rviz 看点云和 TF，rqt 调试，rosbag 把现场录下来回放，plotjuggler 看曲线。例：`rosbag record -a`，回放时一帧一帧看。
- **仿真与回放** —— Gazebo 里先把逻辑跑通再上实车；录下来的包可以反复重放。例：先在虚拟场地撞够，再上真车。

>    **节点**（谁在干活）+ **话题**（它们之间传什么）。剩下的全是这两件事的组合。

---

# 2. 为什么用 ROS

## 2.1 六条理由

1. **它是为机器人量身定做的** —— 发布 / 订阅、坐标变换、参数、录包……机器人要的东西，它一开始就当成一等公民。你写 web 后端不会用到 tf，但写底盘一定会。
2. **生态丰富，调试工具齐全，社区庞大** —— 遇到报错先搜一遍，通常已经有人踩过；算法包拿来改，比从零写快一个数量级。
3. **代码可视化，便于团队分工** —— `rqt_graph` 一眼看清谁连谁；每人只碰自己那个节点，接口谈好就能并行。这条对我们队最重要 —— 视觉、控制、电控三拨人同时改代码，靠的就是「话题名不变，实现随便换」。
4. **核心节点统一用 C++ 写（roscpp）** —— 性能可控；OpenCV、PCL、Eigen 本来就是 C++ 接口，不用为调库再套一层。
5. **可回放：rosbag 把现场数据录下来** —— 赛场上出的问题，回实验室能一帧一帧重放，而不是「当时就那样，复现不了」。这条是真的救命。
6. **可仿真：Gazebo 里先把逻辑跑通** —— 新队员不碰真车也能练；控制律先在虚拟场地里撞够，再上实车。

## 2.2 我们的方案：纯上位机

这一点和「为什么用 ROS」是连着的 —— 因为 ROS 的通信足够便宜，我们才敢把所有逻辑都堆在上位机。

```
上位机（NUC）
    ROS 节点：检测 · 解算 · 闭环 · 状态机 —— 所有控制和视觉程序都在这里跑
    └─ EtherCAT 周期帧 ─→  EtherCAT ⇄ CAN（只做帧转换传输）
            └─ CAN 帧 ─→  电机 × N（力矩 / 位置指令）
```
**我们的下位机里没有控制逻辑。**

- **① 控制代码修改便捷** —— 闭环、状态机、参数全在上位机的 C++ 节点里。跑新代码只需要 `catkin build` 编译，不用烧固件，也不用和另一块板子对协议。
- **② 通信和计算解耦** —— EtherCAT 只保证「这一帧按时到」；从站只做帧转换和心跳 / 急停兜底。逻辑越薄越不容易错，也少一份要长期维护的代码。
- **③ 另一种做法：把闭环放到小板上** —— 实时性更强，但逻辑分两处：改一个 PID 要烧固件，出问题得两端一起看。

> 这就是我这次写底盘控制器的背景：控制器跑在上位机的 ROS 节点里，通过 `ros_control` 的 RobotHW 接口把指令打包丢给 EtherCAT。

---

# 3. ROS 的架构

## 3.1 三层架构
ROS 的所有东西都能归到三层里，**排错也按这三层走**。

- **文件系统层（File System）**（管代码文件放哪、怎么打包、怎么编译） —— 工作空间 → 功能包 → 源码 / 配置。解决「代码怎么组织」：`package.xml`、`CMakeLists`、`msg/srv`、`launch`、`config`。
- **计算图层（Computation Graph）**（管各个节点怎么配合、谁发消息，谁收消息） —— 节点 · 话题 · 服务 · 参数 · 消息。解决「程序怎么协作」：谁发布、谁订阅、谁提供什么服务。
- **通信层（Communication）**（管数据在电脑上怎么从A程序传到B程序）—— TCPROS / UDPROS：节点之间建一条 TCP。解决「数据怎么走」：连接建立、序列化、队列、断线重连。

排错口诀，我觉得比很多教程都实在：

| 现象 | 该查哪一层 | 具体查什么 |
|---|---|---|
| 找不到包 | 文件系统层 | source 了吗？包在 src 里吗？ |
| 连不上 / 收不到 | 计算图层 | 名字对不对？类型对不对？ |
| 延迟高 / 丢数据 | 通信层 | 队列太浅？带宽不够？ |
| 节点一启动就退 | —— | 先看终端里第一行红色 FATAL |

- **关系理解**：
- 把ROS想象成快递系统：
- **文件系统层** = 仓库、货架，规定包裹（代码）放哪里
- **计算图层** = 快递单（话题名），规定谁寄、谁收
- **通信层** = 运输的货车和道路，负责把包裹送过去
- 找不到包＝仓库找不到货；
- 有快递单但是收不到货＝快递单写错（计算图层）；
- 货能送到但是很慢、经常丢件＝路上堵车（通信层）。

## 3.2 五个基本零件

计算图上的东西，一共就五个：

- **节点 Node** —— 一个独立跑着的进程，干一件小事。类比：一个工人。命令 `rosnode list / info`。
- **话题 Topic** —— 一条有名字的广播频道，发布者写、订阅者读。类比：广播电台。命令 `rostopic list / echo`。
- **消息 Message** —— 话题上传输的数据结构，`.msg` 定义字段。类比：信封的格式。命令 `rosmsg show`。
- **服务 Service** —— 一问一答的远程调用，有请求也有应答。类比：打一次电话。命令 `rosservice call`。
- **参数 Parameter** —— 全局共享的配置表，key-value。类比：贴在墙上的配置表。命令 `rosparam get / set`。

## 3.3 ROS Master

**这是 ROS 1 最容易被误解的一点：数据根本不经过 Master。**

先记住三句话：

- Master 是**通讯录**，不是中转站
- 发布者和订阅者之间**直接建 TCP**
- Master 只在「让你们认识」的那一刻出现

> 我一开始以为所有消息都要先发给 Master 再转发，所以一直不理解「为什么 roscore 挂了小乌龟还能动」。后来知道是直连，才想通。

Master 到底管什么，就四件事：

- **① 名字注册** —— 谁发布了什么话题、谁提供了什么服务
- **② 名字查询** —— 按名字把双方介绍给对方
- **③ 参数服务器** —— 全局参数的存放地（后面的 YAML 就住这儿）
- **④ 状态监控** —— 谁还活着、谁掉线了

`roscore` = Master + rosout（日志）+ 参数服务器，一条命令全起。

## 3.4 一次话题通信，到底走了几步

把 Master 放回它该在的位置：**只在前两步出现**。

- **① 启动 Master** —— `roscore` 起在 11311 端口，等别人来注册。
- **② 发布者报到** —— 节点启动即向 Master 注册：我发布 `/cmd_vel`，类型 `Twist`。
- **③ 订阅者报到** —— 订阅者注册：我要订阅 `/cmd_vel`。
- **④ Master 回地址** —— 把发布者的 IP 和端口告诉订阅者（**只此一次**）。
- **⑤ 订阅者去连接** —— 订阅者主动连发布者，握手确认消息类型一致。
- **⑥ 直连传数据** —— 之后每一条消息都走这条 TCP 连接，Master 不再出现。

> 第 4 步返回时，订阅者拿到的是**发布者的地址**，不是数据本身。所以：**Master 挂了，已连上的照跑；新来的连不上。**

这个结论我实测过：`roscore` 被杀掉之后，已经在跑的小乌龟照样能被键盘控制，但新起的节点全废。理解了这一条，「为什么必须开 roscore」就不再是死记硬背。

## 3.5 ros_control：把「控制器」和「硬件」拆开

ros_control **不发明新的通信方式**，只是把话题、服务用在了控制上：

```
话题 /cmd_vel（controller 的输入）
    └─→  Controller（PID · 轨迹 · 差速）
            └─→  RobotHW 接口：read() / write()
                    └─→  下位机 → CAN → 电机
```

中间那层 **RobotHW** 是唯一要你自己写的东西：`write()` 把指令打包下去，`read()` 把编码器读数收上来。

- **Controller Manager** —— 加载、启动、切换控制器，管理控制器的生命周期。`spawner` 一行起一个。
- **常用 Controller** —— `joint_state` / `joint_trajectory` / `effort`（PID）/ `diff_drive` —— 直接用，不用自己写。
- **RobotHW（自己写）** —— 写一个硬件类，把 EtherCAT 或 CAN 的收发包成 `read()` / `write()`，对上位机就是透明的。队里一般自己写一个。
- **joint_limits 与传动** —— 限位、最大速度、加速度；传动比与「关节 ↔ 电机」的映射也在这里。云台 / 底盘的限位都在这。

好处一句话：**换电机只改 hardware_interface，换算法只换 controller；仿真和实车共用同一套 controller。**

> 我这次的底盘控制器就是按这个套路写的：一个 `HeroChassisController` 继承 `controller_interface::Controller`，在 `update()` 里对四个轮子各跑一个 PID，算完的力矩通过 `hardware_interface::EffortJointInterface` 往下发。所以「力矩限幅 ±20」限的是 effort，单位是 **N·m**，不是 rad/s。

---
# 4. 建立 ROS 包

## 4.1 工作空间：那四个目录

`catkin_ws` 里每个目录只干一件事：

```
catkin_ws —— 工作空间（workspace）
catkin_ws/
├── src/            # 你的代码都在这里（进版本库）
│   ├── CMakeLists.txt   ← 软链接到 /opt/ros/...
│   └── my_pkg/          ← 一个功能包
├── build/          # 编译中间文件（可删，别进 git）
├── devel/          # 编译产物：setup.bash、可执行文件
    └── setup.bash  # 每开一个终端都要 source 它
```

- **源码和产物分离** —— `src` 进 Git，`build` / `devel` 不进。别人 clone 下来重新编译即可。
- **每次开终端先 source** —— `source devel/setup.bash`，或者写进 `~/.bashrc` 一劳永逸。
- **一个空间可以放很多包** —— 包名必须全局唯一，否则 `rosrun` 会认错。

上车部署的一条龙：`git clone` 到上位机 → `catkin_make` → `source devel/setup.bash`。车上没有图形界面，编译和运行都在 SSH 里做。

## 4.2 一个包里到底有什么

八样东西，各有各的位置；**放错地方，编译就是不过**。

```
my_pkg/ —— 功能包（ROS 里最小的复用单位）
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

- **include vs src** —— `include` 是「给别人用的」，`src` 是「给自己编的」；只写 `src` 也能跑，但别把 `.h` 乱塞。
- **msg / srv 要配合编译** —— 光建目录没用，`CMakeLists` 里要 `add_message_files` 才会生成头文件。
- **launch / config 不进编译** —— 但它们决定了「怎么跑」，是别人接手你的包时**最先看的东西**。

> 坑：服务目录在 ROS 里叫 `srv/`，别建成 `service/`，编译时会找不到。
>
> 另一个坑：`include/my_pkg/` 这一层目录名要跟包名一致，否则 `#include <my_pkg/xxx.h>` 找不到。

## 4.3 package.xml：这张「名片」决定谁能找到你

```xml
<?xml version="1.0"?>
<package format="2">
  <name>my_pkg</name>              <!-- 必须与文件夹同名 -->
  <version>0.1.0</version>
  <description>我的第一个 ROS 包</description>
  <maintainer email="you@example.com">Your Name</maintainer>
  <license>MIT</license>

  <buildtool_depend>catkin</buildtool_depend>
  <build_depend>std_msgs</build_depend>     <!-- 编译期需要 -->
  <exec_depend>message_runtime</exec_depend> <!-- 运行期需要 -->
  <depend>roscpp</depend>                   <!-- 三合一 -->
</package>
```

- **`<name>`** —— 包名，必须和文件夹同名；改了文件夹不会自动改这里。
- **`<build_depend>`** —— 编译这个包时需要的东西：头文件、库、消息包。
- **`<exec_depend>`** —— 运行时需要的东西：动态库、消息运行库、launch 里调用的包。
- **`<depend>`** —— = build + build_export + exec，一般写这个最省事。
- **自定义 msg/srv 要额外加** —— `message_generation`（编译期）和 `message_runtime`（运行期）。

> 坑：包名不能有中文、空格和大写歧义；新建包后先编译一次再写代码。
>
> 依赖写错时，编译器报的错通常和真正的原因差十万八千里 —— 这句话是原话，我信。曾经漏了 `<depend>sensor_msgs</depend>`，报的错是「找不到 boost 的某个头文件」。

## 4.4 CMakeLists.txt：告诉编译器「编什么、链什么」

```cmake
# ① 项目声明
cmake_minimum_required(VERSION 3.0.2)
project(my_pkg)

# ② 找依赖（包名要和 package.xml 对得上）
find_package(catkin REQUIRED COMPONENTS
  roscpp std_msgs message_generation)

# ③ 自定义消息 / 服务（用到才写）
add_message_files(FILES MyMsg.msg)
add_service_files(FILES MySrv.srv)
generate_messages(DEPENDENCIES std_msgs)
catkin_package(CATKIN_DEPENDS roscpp std_msgs message_runtime)

# ④ 可执行目标
include_directories(${catkin_INCLUDE_DIRS})
add_executable(talker src/talker.cpp)
target_link_libraries(talker ${catkin_LIBRARIES})
add_dependencies(talker ${${PROJECT_NAME}_EXPORTED_TARGETS})
```

- **① 声明** —— 项目名、CMake 最低版本，一行都别省。
- **② 找依赖** —— `COMPONENTS` 后面写包名；`package.xml` 里有的，这里也要有。
- **③ 生成消息** —— 只有自定义 `.msg/.srv` 才需要；三个函数要配套出现。
- **④ 目标与链接** —— 每加一个 `.cpp` 就加一条 `add_executable`，否则它不会被编译。

> 坑：忘了 `add_dependencies` → 报「找不到 MyMsg.h」；改了 `.msg` 不重新 `catkin_make` → 用的是旧消息。
>
> 我的做法是：只要动了 `msg/` 或 `srv/`，直接 `catkin build --force-cmake` 重来一遍，别赌缓存。

---

# 5. 三大通信

## 5.1 总览：广播、打电话、贴公告

| | 话题 Topic | 服务 Service | 参数 Parameter |
|---|---|---|---|
| 类比 | 广播电台 | 打一次电话 | 贴在墙上的配置表 |
| 关系 | 发布者只管喊，订阅者爱听不听；谁都不认识谁 | 你问一句，他答一句，答完就挂 | 谁都能看，改一次所有人都看到 |
| 场景 | 传感器数据、里程计、检测结果、控制指令 | 切换模式、取一次地图、清空轨迹、生成一只乌龟 | 阈值、相机内参、PID 参数、串口波特率 |
| 性质 | 连续变化的数据流，多对多 | 需要一个明确结果，一对一，调用不频繁 | 是配置不是数据，启动时读一次就够 |

选择口诀：**连续的数据流 → 话题；要一次结果 → 服务；配置项 → 参数；跑得久、要进度、能取消 → action。**

（action、nodelet 培训课暂不涉及，可自行学习。action 其实就是「带反馈、能取消的服务」。）

## 5.2 话题通信（Topic）：发布 / 订阅

一对多、多对多、异步通信 —— **发出去就不管了，谁在听是别人的事**。

### 5.2.1 四个角色

- **发布者 Publisher** —— 往话题里写数据的节点
- **订阅者 Subscriber** —— 从话题里读数据的节点
- **消息 Message** —— 数据的结构，由 `.msg` 定义（Twist、Image…）
- **名字 + 类型** —— 同名 + 同类型才能对上，类型不一致直接连不上

### 5.2.2 为什么它适合数据流

- **异步** —— 发完就返回，不等任何人
- **多对多** —— 一个话题可以有很多发布者和订阅者
- **有队列** —— `queue_size` 是缓冲深度，满了会丢旧数据
- **断线自动重连** —— 订阅者晚启动也能接上（除非 latching）

### 5.2.3 代码：两个进程

发布端：

```cpp
// talker.cpp —— 发布端
#include <ros/ros.h>
#include <std_msgs/String.h>

int main(int argc, char** argv) {
  ros::init(argc, argv, "talker");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<std_msgs::String>(
      "/chatter", 10);
  ros::Rate rate(1);            // 1 Hz
  while (ros::ok()) {
    std_msgs::String msg;
    msg.data = "hello";
    pub.publish(msg);
    rate.sleep();
  }
}
```

订阅端：

```cpp
// listener.cpp —— 订阅端
#include <ros/ros.h>
#include <std_msgs/String.h>

void cb(const std_msgs::String::ConstPtr& msg) {
  ROS_INFO("I heard: %s", msg->data.c_str());
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "listener");
  ros::NodeHandle nh;
  ros::Subscriber sub =
      nh.subscribe("/chatter", 10, cb);
  ros::spin();                  // 卡住，等回调
}
```

三行核心 API：

- `advertise()` 创建发布者，指定话题名、类型、队列长度
- `publish()` 发一条消息，立刻返回
- `subscribe()` 创建订阅者，注册一个回调函数

> **`ros::init` 必须最先。** init 之后才能造 NodeHandle；漏了它，节点名和参数都认不出来。
>
> **第二个参数是队列长度。** `advertise / subscribe` 的 10 是缓冲深度，发送比接收快就会丢旧数据。
>
> **回调别做耗时操作。** 回调跑在 `spin` 的线程里，卡住会堵住后面的消息；重活丢给别的节点。
>
> 我个人最容易犯的错是写完 `subscribe` 忘了 `ros::spin()`，然后疑惑「为什么回调一次都不进」。

### 5.2.4 命令行：不写代码也能看

```bash
rostopic list              # 列出所有活跃话题
rostopic echo /chatter     # 把话题上的内容打到终端
rostopic info /chatter     # 谁在发布、谁在订阅、类型是什么
rostopic hz /chatter       # 测频率
rostopic pub -r 10 /chatter std_msgs/String "data: 'hi'"   # 手动发
```

小乌龟里的例子：`/turtle1/cmd_vel`（`geometry_msgs/Twist`）—— 键盘节点发布，乌龟节点订阅。

> `rostopic pub` 默认发一条就退出，加 `-r 10` 才会持续发。这个 `-r` 就是「rate」。
>
> 更要命的坑：**`rostopic pub` 发的是「设定值」，不是「动作」**。你发一条 `x: 0.3`，控制器每个周期都会重复执行最后收到的这条指令，车就会一直跑。想停必须再发一条全零：
>
> ```bash
> rostopic pub -1 /cmd_vel geometry_msgs/Twist '{}'
> ```
>
> 用 `timeout 4 rostopic pub ...` 只是让**发布**停下来，**车不会停**。我在这上面栽过一次，车直接撞墙。

## 5.3 服务通信（Service）：请求 / 应答

同步阻塞：**发出去之后，代码就停在那儿，直到拿到结果或超时**。

### 5.3.1 两个角色 + 一份契约

- **服务端 Server** —— 提供能力，注册一个回调处理请求
- **客户端 Client** —— 主动发起调用，等结果
- **Request / Response** —— 请求和应答两段数据结构
- **`.srv` 文件** —— 上下两段用 `---` 隔开，上面请求、下面应答

### 5.3.2 什么时候不该用它

- **同步阻塞** —— 调用期间客户端线程停住
- **一对一** —— 一个服务对应一个实现
- **服务端没起来就报错** —— 要 `wait_for_service` / `waitForExistence`
- **高频场景不要用** —— 每次调用都要建连、序列化

### 5.3.3 先定义，再调用

```text
# srv/Spawn.srv —— 上段请求，下段应答
float32 x
float32 y
float32 theta
string name
---
string name      # 返回：新乌龟的名字
```

```cmake
# CMakeLists.txt 里加两行
add_service_files(FILES Spawn.srv)
generate_messages(DEPENDENCIES std_msgs)
```

```bash
# 命令行调用（最快的验证方式）
$ rosservice list
$ rosservice type /spawn
$ rosservice call /spawn "x: 2.0 y: 3.0 theta: 0.0 name: 't2'"
name: "t2"
```

```cpp
// C++ 客户端
ros::ServiceClient cli =
    nh.serviceClient<turtlesim::Spawn>("/spawn");
turtlesim::Spawn srv;
srv.request.x = 2.0;  srv.request.name = "t2";
if (cli.call(srv))
  ROS_INFO("new: %s", srv.response.name.c_str());
```

核心 API：

- `serviceClient<T>()` 创建客户端，指定服务名和 srv 类型
- `ServiceServer` 注册服务名、类型、处理函数
- `waitForExistence()` 等对方上线，避免直接报错
- `cli.call(srv)` 阻塞，直到返回 response

命令行：

- `rosservice list` 列出当前所有服务
- `rosservice type` 看这个服务的类型
- `rosservice call` 手工填参数调一次，最快的验证方式
- `rossrv show` 看 `.srv` 的字段定义

小乌龟里的例子：`/spawn`（`turtlesim/Spawn`）—— 传 x、y、theta，返回新乌龟的名字。

> 坑三条：
>
> **服务名写错会一直等。** 现象是「程序卡住没反应」，先用 `rosservice list` 核对名字。
>
> **高频调用会拖垮节点。** 每次调用都有建连和序列化开销；10 Hz 以上考虑用话题。
>
> **超时要自己加。** `call()` 默认一直等，比赛里必须自己包一层超时，否则一次卡死全队。

记住：话题是「持续广播」，服务是「一次问答」。**频率高、要求实时 → 话题；偶尔一次、要结果 → 服务。**

## 5.4 参数服务器（Parameter Server）：全局的「配置墙」

**它不在节点之间传数据，它只是让大家读同一份配置。**

### 5.4.1 它到底是什么

- **全局 key-value** —— 存在 Master 里，所有节点都能访问
- **树形命名空间** —— `/robot/camera/exposure` 这样一层层往下
- **六种类型** —— int、double、bool、string、list、dict
- **生命周期** —— 随 roscore 存在，roscore 一关就没了

### 5.4.2 别把它当数据库

- **以静态配置为主** —— 启动时读一次
- **谁都能 set** —— 改完所有节点下次 get 都能看到
- **ROS 1 里改参数不会触发回调** —— 要自己轮询
- **改参数不用重启节点** —— 这就是它最大的价值

### 5.4.3 读写与默认值

```cpp
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

核心 API：

- `get_param(name, default)` 读，给默认值最稳
- `set_param(name, value)` 写
- `has_param` / `delete_param` 判断 / 删除
- `get_param_cached` 带缓存，高频读用它

### 5.4.4 命令行：四个就够用

```bash
rosparam list                 # 列出所有参数（配合 grep 过滤）
rosparam get / set            # 读一个 / 改一个
rosparam load / dump          # 从 YAML 灌入 / 导出成 YAML
rosparam delete               # 删掉一棵子树
```

### 5.4.5 三种命名写法，作用域完全不同

| 写法 | 名字 | 含义 |
|---|---|---|
| `/robot/speed` | 绝对名 | 从根开始，换命名空间也指同一个地方 |
| `~speed` | 私有名 | 等价于 `/节点名/speed`，只属于这个节点 |
| `speed` | 相对名 | 拼在当前命名空间后面，随 launch 的 `ns` 变 |

> **类型提醒：** `30` 是 int，`"30"` 是 string，`3.0` 是 double，`true` 是 bool。
>
> **读参数时一定给默认值**，参数文件漏写一项不该让节点崩掉。
>
> 坑：`rosparam dump` 出来的文件**带命名空间前缀**，`load` 时再加一次会叠成 `/cam/cam/exposure`。我第一次 dump 完再 load，参数全串了，找了一晚上。

---
# 6. 配置文件：YAML

参数一多，就该写进 YAML 文件批量灌进去，别在 launch 里手写几十个 `<param>`。

## 6.1 五条硬规矩

报错 `Error parsing YAML` 的时候，九成是这五条里的一条。

1. **只用空格缩进，绝不用 Tab** —— 两个空格算一级；同一层必须严格对齐。
2. **冒号后面必须有空格** —— `key: value` 对；`key:value` 错 —— 它会被当成一个字符串。
3. **大小写敏感** —— `True` 和 `true` 不是一回事；ROS 里按 bool 解析的是 `true` / `false`，不要写 `yes` / `no`，编译器是不认的。
4. **`#` 是注释** —— 可以整行，也可以在行尾；值里要用 `#` 就加引号。
5. **`---` 可以分隔多个文档** —— 空文件也是合法的，等于一个空字典。

对的做法：

```yaml
# 对的做法
camera:
  width: 640
  height: 480
  name: "front"   # 注释
  debug: true
```

错的做法：

```yaml
# 错的做法
camera:
  width:640        # 冒号后没空格
  name: front cam  # 有空格没引号
```

> Tab 缩进肉眼几乎看不出来，但 YAML 直接报错。上面几种写法都会让加载失败。

三个立刻能用的检查：
1. 编辑器里把 Tab 显示出来（VS Code 右下角）
2. 看行号旁的缩进参考线有没有对齐
3. 用 `rosparam load` 试灌一遍：

```bash
rosparam load p.yaml /t
rosparam get /t
```

能灌进去、类型也对，语法就过了。

一句话：**YAML 用缩进表达层级，所以「对齐」就是它的语法本身。**

## 6.2 只有三种结构

所有复杂的东西，都是这三样嵌套出来的。

标量（scalar）：

```yaml
speed: 1.5          # 数字
name: "sentry"      # 字符串
debug: true         # 布尔
empty: null         # 空值
```

列表（list）：

```yaml
ports:
  - 8080
  - 8081
# 行内写法
ids: [1, 2, 3]
```

字典（dict）：

```yaml
camera:
  width: 640
  height: 480
# 行内写法
point: {x: 1, y: 2}
```

真实的样子，一份 ROS 参数文件：

```yaml
turtlesim:
  background_r: 69
  background_g: 86
  background_b: 255
  holonomic: false

my_node:
  thresholds: [0.3, 0.6, 0.9]
  camera:
    exposure: 30
    gain: 1.2
  targets:
    - {name: "hero", id: 1}
    - {name: "sentry", id: 7}
```

读法：缩进决定层级，冒号左边是名字，右边是值；`-` 表示列表的一项。

## 6.3 在 ROS 里怎么用：三种加载方式

文件写对了，还得「加载对地方」。

```bash
# ① 命令行：临时调试最方便
rosparam load config/params.yaml /my_ns
```

```xml
<!-- ② launch 文件里（最常用） -->
<rosparam file="$(find my_pkg)/config/params.yaml"
          command="load" ns="my_ns"/>
```

```cpp
// ③ 代码里读（带默认值最稳）
ros::NodeHandle pnh("~");
int exposure;
pnh.param<int>("camera/exposure", exposure, 30);
```

> 我的控制器就是这样配的：`config/controllers.yaml` 里写四个轮子的 PID 参数和 `wheel_radius` / `wheel_base`，launch 里一行 `<rosparam file="$(find hero_chassis_controller)/config/controllers.yaml" command="load"/>` 灌进去，`update()` 里 `pnh.param()` 读出来。改参数不用重新编译，`roslaunch` 重起一次就行 —— 调 PID 的时候这个太重要了。

---

# 7. 跑起来

## 7.1 小乌龟：三行命令，一个完整的 ROS 系统

它麻雀虽小，话题、服务、参数一样不少。

```bash
# 终端 1
roscore
# → 启动 Master 与参数服务器

# 终端 2
rosrun turtlesim turtlesim_node
# → 仿真窗口：一只乌龟在爬

# 终端 3
rosrun turtlesim turtle_teleop_key
# → 键盘控制：方向键移动
```

它身上的东西：

| 类别 | 内容 |
|---|---|
| 话题 | `/turtle1/cmd_vel`（`geometry_msgs/Twist`）键盘节点发布 → 乌龟订阅 |
| 话题 | `/turtle1/pose`（`turtlesim/Pose`）乌龟发布 → 你订阅 |
| 服务 | `/spawn` `/kill` `/clear` `/turtle1/set_pen` |
| 参数 | `/turtlesim/background_r` 等，背景色、速度上限 |

动手摸一下：

```bash
rosparam set /turtlesim/background_r 255     # 背景变红
rosservice call /clear                       # 清屏，新颜色生效
rqt_graph                                    # 一张图看清谁连谁
```

> 这三个命令刚好把三大通信各摸了一遍：`rosparam set` 改配置、`rosservice call` 调服务、`rqt_graph` 看计算图。

## 7.2 rosnode：谁在跑、跑得好不好

排查问题永远从这一条命令开始。

```bash
# 谁在跑
$ rosnode list
/turtle_teleop_key
/turtlesim

# 它在和谁打交道 —— 收不到消息时先看这里
$ rosnode info /turtlesim
Publications: /turtle1/pose, /rosout
Subscriptions: /turtle1/cmd_vel
Services: /turtle1/set_pen, /clear ...

# 还活着吗 / 干掉它
$ rosnode ping /turtlesim
$ rosnode kill /turtlesim
```

- **先 list，再 info** —— 「我以为它订阅了」是最常见的误判；`info` 会把真相打出来。
- **ping 看延迟与存活** —— 节点假死时 `list` 里还在，`ping` 才看得出来。
- **kill 会先注销** —— 比 `kill -9` 干净；挂了之后靠 `respawn` 复活。

一句话：**`rqt_graph` 看全局，`rosnode info` 看单点，`rostopic echo` 看内容。**

## 7.3 小结（课上那五条）

- **01 ROS 是中间件 + 工具箱** —— 跑在 Linux 之上：通信 / 工具链 / 生态，三件东西。
- **02 视觉和控制共用一套骨架** —— 只是控制多一条链路：上位机 → EtherCAT → 电机。
- **03 架构分三层** —— 文件系统层 / 计算图层 / 通信层，排错先分层。
- **04 三大通信各有分工** —— 话题 = 数据流，服务 = 一次问答，参数 = 配置。
- **05 YAML 是参数的写法** —— 缩进即语法，写完用 `rosparam load` 验一遍。

课后学习建议：roswiki、B 站 ros 教程、赵虚左 ros1 讲义。

---
# 附录 A：命令速查表

**包与编译**
```bash
rospack find <pkg>
roscd <pkg> / rosls <pkg>
catkin_create_pkg <name> <deps>
catkin build
```

**运行与节点**
```bash
roscore
rosrun <pkg> <node> <参数>
roslaunch <pkg> <file.launch>
rosnode list | info | ping | kill
```

**话题**
```bash
rostopic list
rostopic echo <topic>
rostopic info <topic>
rostopic hz <topic>
rostopic pub -r 10 <topic> <type>
```

**服务与消息**
```bash
rosservice list | type | call
rosservice find <srv_type>
rosmsg show <pkg/Type>
rossrv show <pkg/Type>
```

**参数**
```bash
rosparam list | get | set
rosparam load <file> [ns]
rosparam dump <file> [ns]
rosparam delete <ns>
```

**调试与录制**
```bash
rviz
rosbag record -a
rosbag info <bag>
rosbag play <bag>
# 或在 plotjuggler 上播放 bag
```

---

# 附录 B：名词对照

| 名词 | 一句话 |
|---|---|
| 元操作系统 | ROS 的自我定位：不是 OS，但干了 OS 的一部分活 |
| 节点 Node | 一个进程，干一件小事 |
| 话题 Topic | 有名字的广播频道 |
| 消息 Message | 频道里传的数据格式，`.msg` 定义 |
| 服务 Service | 一问一答，`.srv` 定义，`---` 分两段 |
| 参数 Parameter | 全局 key-value，存在 Master 里 |
| Master | 只管名字注册和查询，不碰数据 |
| roscore | Master + rosout + 参数服务器 |
| 计算图 | 节点、话题、服务、参数组成的网络 |
| 命名空间 ns | 名字的前缀，用来隔离同名话题 |
| 队列 queue_size | 缓冲深度，满了丢旧数据 |
| latching | 发布者保留最后一条，新订阅者一上线就收到 |
| TCPROS | 节点间传数据的协议，底层是 TCP |
| rosout | 全局日志话题，所有 `ROS_INFO` 都往这扔 |
| 回调 callback | 消息到了自动执行的函数，跑在 spin 线程里 |
| spin | 让程序卡住，等回调进来 |
| `catkin_ws` | 工作空间，`src` 进 git，`build`/`devel` 不进 |
| `devel/setup.bash` | 编译产物的环境变量，每开终端都要 source |
| `package.xml` | 包的名片：名字、版本、依赖 |
| `CMakeLists.txt` | 编译规则：编什么、链什么 |
| transmission | URDF 里「电机 ↔ 关节」的机械关系 |
| RobotHW | `ros_control` 里唯一要自己写的硬件接口层 |
| Controller Manager | 加载、启动、切换控制器 |
| joint_limits | 限位、最大速度、加速度，云台底盘的限位都在这 |

---

# 附录 C：踩坑清单

按「会不会让车跑飞」排序。

1. **`rostopic pub` 之后车一直跑** —— 发的是设定值不是动作，控制器每周期都重复执行最后一条。`timeout` 只停发布不停车，必须再补一条全零：

    ```bash
    rostopic pub -1 /cmd_vel geometry_msgs/Twist '{}'
    ```

2. **力矩限幅单位搞错** —— `effort` 接口限的是**力矩**，单位 N·m；`velocity` 接口限的才是 rad/s。写 `±20` 之前先看接口类型。
3. **服务目录建成 `service/`** —— ROS 里叫 `srv/`，写成 `service/` 编译时找不到。
4. **忘了 `add_dependencies`** —— 报「找不到 MyMsg.h」，实际是消息还没生成就先编了 cpp。
5. **改了 `.msg` 没重新编译** —— 用的是旧消息，现象是字段莫名其妙少一个。直接 `catkin build --force-cmake`。
6. **YAML 里混了 Tab** —— 报 `Error parsing YAML`，肉眼看不出来。编辑器打开「显示空白字符」。
7. **`rosparam dump` 完再 `load` 叠了前缀** —— 变成 `/cam/cam/exposure`。dump 出来的文件本身带前缀，load 时 ns 要留空。
8. **写完 `subscribe` 忘了 `ros::spin()`** —— 回调一次都不进，程序直接退出。
9. **回调里做耗时操作** —— 堵住 spin 线程，后面所有消息都积压。重活丢给另一个节点。
10. **节点名重复** —— 两个同名节点一起跑，后起的会把先起的顶掉（ROS 1 的行为），现象是「另一个节点莫名其妙没了」。

