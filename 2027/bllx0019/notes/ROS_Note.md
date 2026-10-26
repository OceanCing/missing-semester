# ROS_Note

## Something About ROS
ROS（Robot Operating System）是一套用于编写机器人软件的开源框架：它提供进程间通信机制、常用工具和软件包生态，让“感知—决策—控制”这类复杂系统可以拆开开发、独立调试、按需复用
没有 ROS 时，一个机器人程序通常是一个巨大的单体程序：传感器读取、图像处理、路径规划、电机控制全塞在一起，这导致了整个项目的可维护性极低

ROS 主要从四个方向解决这些问题：

| 项目 | How ROS Do |
| --- | --- |
| 进程间通信 | 话题 / 服务 / 动作，统一的消息格式，跨语言（C++、Python） |
| 代码复用 | 以“包（package）”为单位分发，`find_package` / `rosdep` 管理依赖 |
| 观察系统进程 | `rqt_graph`、`rostopic`、`rosbag`、`rviz` 等工具 |
| 不同机器人间共享代码 | 标准消息、TF 坐标变换、launch 约定、REP 规范 |

## ROS结构

ROS Noetic（ROS1家族）的运行模型

```text
                 ┌──────────────────────────────┐
                 │            roscore           │
                 │   master（名字服务/注册中心）  │
                 │   parameter server（参数）    │
                 │   rosout（日志收集）          │
                 └──────────────┬───────────────┘
                                │ 注册/查找
       ┌────────────────────────┼─────────────────────────┐
       │                        │                         │
 ┌─────▼─────┐           ┌──────▼──────┐           ┌─────▼─────┐
 │ 节点 A    │  topic    │  节点 B      │  service  │  节点 C   │
 │ /talker   │─────────▶│  /listener   │◀────────▶│ /server   │
 └───────────┘           └─────────────┘           └───────────┘
```

-master：记录 节点发布的对应话题 与 节点提供的对应服务，节点启动时向master注册；节点间真数据传输**不经过** master
-parameter server：全局的键值存储，节点启动时读取配置
-rosouy：收集所有节点的日志，`rqt_console`可视化
-`roscore`一条命令启动上述三项功能

>[!TIP]
>master只充当通讯录而非交换机的作用；节点之间建立连接后直接通信，所以master挂掉后已有连接仍能传数据，但新节点无法加入。

## ROS的运行
由于煮啵的电脑有点新了（Ultra5-125h），所以本机操作系统是Ubuntu24.04（Linux6.8），运行ROS Noetic则选用了docker容器方案；

docker_file长这样：
```docker

```

为了方便，还写了一个ros1.sh的一键启动脚本，长这样：
```bash

```
但是由于这两样以及ROS Noetic的docker环境都是由ClaudeCode+ds v3配置的，具体聊天记录早已归档且过于久远，已不可寻

## ROS 的核心概念

### 节点与 master
节点（node）是一个独立进程，通常负责执行一件具体的事（e.g.相机,识别,规划,电控）一个机器人系统通常由十几个到上百个节点组成。
```bash
roscore                            #启动master
rosrun turtlesim turtlesim_node    #运行节点：包名+可执行文件名
rosnode list                       #列出正在运行的节点
rosnode info /turtlesim            #查看节点的发布、订阅与服务
rosnode ping /turtlesim            #检查节点是否存活
rosnode kill /turtlesim            #结束节点
```
节点名在 master 里必须唯一。用`rosrun`启动同名节点时，新节点会把旧节点替换掉；调试时可以用`anonymous=True`让节点名自动加随机后缀

### 话题与消息

话题（topic）是发布/订阅的通道；一个节点（发布者）往话题里发消息，所有订阅者都可接收（局域网like），发布者和订阅者互不知道对方存在

```bash
rostopic list                       #列出所有话题
rostopic echo /turtle1/pose         #打印消息内容
rostopic echo -n 1 /turtle1/pose    #只打印一条
rostopic info /turtle1/cmd_vel      #查看消息类型、发布者、订阅者
rostopic hz /turtle1/pose           #查看发布频率
rostopic pub -r 10 /turtle1/cmd_vel geometry_msgs/Twist \
  "{linear: {x: 1.0, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.5}}"
```

消息（message）由 `.msg` 文件定义：
```bash
rosmsg list                  #列出所有消息类型
rosmsg show std_msgs/String  #查看消息字段
```
> [!WARNING]
> 话题的发布和接受是异步的。频率高、数据量大（图像、点云）时，要注意`queue_size`和带宽。

### 服务

服务（service）是请求/响应模式。客户端发一个请求，服务端处理后返回结果，调用会阻塞直到拿到结果或超时
```bash
rosservice list                 # 列出所有服务
rosservice type /spawn          # 查看服务的 srv 类型
rosservice call /spawn "{x: 2.0, y: 2.0, theta: 0.0, name: 'turtle2'}"
rosservice call /clear "{}"
```
|对比项|话题|服务|
|---|---|---|
|方向|单向发布|双向：请求与响应|
|同步性|异步|同步阻塞|
|接收者数量|多个订阅者|每个服务只有一个服务端|
|典型场景|传感器数据、控制指令流|查询状态、触发一次性动作 |

### 动作

动作（action）用于需要执行一段时间、过程中有进度反馈、可以中途取消的任务，例如导航到目标点、机械臂抓取。一个动作包含三部分：

- goal：目标，例如目标位姿
- feedback：执行过程中的周期反馈
- result：最终结果。

动作基于 actionlib 实现，底层通过一组话题传递 goal、feedback、status、result，客户端与服务端接口由 actionlib 封装，取消等控制也走这套协议

```bash
rosrun actionlib_tools axclient.py /fibonacci   #图形化发送目标、查看反馈
```

### 参数

参数（parameter）是放在参数服务器上的全局键值对，用于配置，不用于高速数据流

```bash
rosparam list
rosparam get /turtlesim/background_b
rosparam set /turtlesim/background_b 120
rosparam dump params.yaml
rosparam load params.yaml
rosparam delete /turtlesim/background_b
```

YAML 文件的写法：

```yaml
turtlesim:
  background_r: 120
  background_g: 80
  background_b: 200
```
>[!CAUTION]
参数用于低频、少量的配置；话题用于高频、流式的数据，两者不要混用

### 命名与重映射

|写法|含义|示例|
|---|---|---|
|`/global`|全局名，从根命名空间开始|`/turtle1/pose`|
|`relative`|相对于当前节点命名空间|`pose`|
|`~private`|相对于当前节点名|`~rate`|

重映射可以把节点内部写死的话题名换成实际使用的话题名：
```bash
rosrun my_pkg talker chatter:=/robot1/chatter
```

launch 文件里的等价写法：
```xml
<node pkg="my_pkg" type="talker" name="talker">
  <remap from="chatter" to="/robot1/chatter"/>
</node>
```

命名空间用于把一组节点和话题放到同一个前缀下：
```xml
<group ns="robot1">
  <node pkg="my_pkg" type="talker" name="talker"/>
</group>
```

### 通信方式对比

|特性|话题|服务|动作|参数|
|---|---|---|---|---|
|模式|发布/订阅|请求/响应|目标/反馈/结果|全局键值|
|同步性|异步|同步阻塞|异步，可取消|同步读写|
|数据方向|单向|双向一次|多次反馈|读写|
|典型场景|传感器流、控制指令|查询、触发|导航、抓取|配置参数|
|命令行工具|`rostopic`|`rosservice`|`rostopic`与 actionlib|`rosparam`|

## 工作空间与包

### catkin 工作空间
```text
catkin_ws/
├── src/          #源码空间，每个子目录是一个包
├── build/        #构建空间，整个工作空间共用一个
├── devel/        #开发空间，构建产物与环境脚本
└── install/      #安装空间，catkin_make install 后生成
```
```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws
catkin_make                    #首次运行会创建 build/、devel/ 和 src/CMakeLists.txt
source devel/setup.bash        #每个新终端都要 source
```
> [!NOTE]
> 一个 catkin 工作空间在构建时会被当作一个大 CMake 工程，`devel/setup.bash`的作用是把当前工作空间叠加到已安装的ROS之上

### 创建包
```bash
cd ~/catkin_ws/src
catkin_create_pkg beginner_tutorials std_msgs rospy roscpp
```

命令格式是`catkin_create_pkg <包名> [依赖1] [依赖2] ...`，使用小写字母、数字和下划线，不能以数字开头
```text
beginner_tutorials/
├── CMakeLists.txt      # 构建规则，必须
├── package.xml         # 包元信息与依赖，必须
├── include/            # C++ 头文件
├── src/                # C++ 与 Python 源码
├── scripts/            # Python 脚本
├── msg/                # 自定义消息
├── srv/                # 自定义服务
├── launch/             # launch 文件
└── config/             # YAML 参数
```

### package.xml与CMakeLists

`package.xml`的最小示例：

```xml
<?xml version="1.0"?>
<package format="2">
  <name>beginner_tutorials</name>
  <version>0.1.0</version>
  <description>ROS 1 练习package</description>
  <maintainer email="you@example.com">Your Name</maintainer>
  <license>BSD</license>

  <buildtool_depend>catkin</buildtool_depend>

  <build_depend>roscpp</build_depend>
  <build_depend>rospy</build_depend>
  <build_depend>std_msgs</build_depend>

  <exec_depend>roscpp</exec_depend>
  <exec_depend>rospy</exec_depend>
  <exec_depend>std_msgs</exec_depend>
</package>
```
`CMakeLists.txt`的最小实现

```cmake
cmake_minimum_required(VERSION 3.0.2)
project(beginner_tutorials)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs
)

include_directories(${catkin_INCLUDE_DIRS})
```
两边的对应关系：

| `package.xml` | `CMakeLists.txt` | 说明 |
| --- | --- | --- |
| `<buildtool_depend>catkin</buildtool_depend>` | `find_package(catkin REQUIRED)` | 构建工具依赖 |
| `<build_depend>roscpp</build_depend>` | `find_package(catkin REQUIRED COMPONENTS roscpp)` | 编译期依赖 |
| `<exec_depend>rospy</exec_depend>` | `catkin_package(CATKIN_DEPENDS rospy)` | 运行期依赖，会传给下游 |
| `<depend>std_msgs</depend>` | 同时表达 build 与 exec | format 2 的简写 |
| `<build_depend>message_generation</build_depend>` | `generate_messages(...)` | 自定义 msg / srv 时需要 |
| `<exec_depend>message_runtime</exec_depend>` | `catkin_package(CATKIN_DEPENDS message_runtime)` | 消息运行时 |
> [!WARNING]
> 只改一边是最常见的错误。`CMakeLists.txt` 加了 `find_package` 但 `package.xml` 没加依赖，`rosdep` 安装和发布时会失败；只改 `package.xml` 不改 CMake，编译时找不到头文件

### 构建与 source
```bash
cd ~/catkin_ws
catkin_make                       # 增量构建全部包
catkin_make -DCMAKE_BUILD_TYPE=Release
catkin_make --force-cmake         #强制重新配置
catkin_make install               #生成 install 空间
catkin_make run_tests             #运行测试
source devel/setup.bash           #构建后必须 source
```
```bash
rm -rf build devel install        #清理构建产物
```
> [!WARNING]
> 如果`catkin_make`报找不到`src/CMakeLists.txt`，通常是工作空间没有初始化，或者`src/`为空

### 包与文件查找
```bash
rospack find beginner_tutorials      #包所在目录
rospack depends1 beginner_tutorials  #直接依赖
rospack depends beginner_tutorials   #递归依赖
roscd beginner_tutorials             #进入包目录
rosls beginner_tutorials             #列出包内文件
rosed beginner_tutorials package.xml #编辑包内文件
```
`roscd`找不到新包时，先确认已经执行`source devel/setup.bash`，再执行`rospack profile`刷新缓存

## 常用工具

### 命令行工具

| 工具 | 典型用法 | 作用 |
| --- | --- | --- |
| `roscore` | `roscore` | 启动 master、参数服务器、rosout |
| `rosrun` | `rosrun pkg node` | 运行单个节点 |
| `roslaunch` | `roslaunch pkg file.launch` | 按 launch 文件批量启动 |
| `rosnode` | `rosnode list / info / ping / kill` | 查看和管理节点 |
| `rostopic` | `rostopic list / echo / info / hz / pub` | 查看和调试话题 |
| `rosmsg` | `rosmsg list / show pkg/Msg` | 查看消息类型 |
| `rosservice` | `rosservice list / call / type` | 查看和调用服务 |
| `rossrv` | `rossrv list / show pkg/Srv` | 查看服务类型 |
| `rosparam` | `rosparam list / get / set / load / dump` | 读写参数服务器 |
| `rosbag` | `rosbag record / play / info` | 录制和回放数据 |
| `rospack` | `rospack find / depends / depends1` | 包查找与依赖 |
| `roscd`、`rosls`、`rosed` | `roscd pkg` | 进入、列出、编辑包内文件 |
| `rosdep` | `rosdep install ...` | 安装系统依赖 |
| `roswtf` | `roswtf` | 检查 ROS 环境与计算图 |
| `rqt_graph` | `rqt_graph` | 可视化节点与话题连接 |
| `rviz` | `rosrun rviz rviz` | 三维可视化 |

### rqt 家族

| 工具 | 作用 |
| --- | --- |
| `rqt_graph` | 节点与话题连接图 |
| `rqt_plot` | 把话题里的数值字段画成曲线 |
| `rqt_console` | 汇总所有节点的日志，按级别过滤 |
| `rqt_logger_level` | 动态调整节点日志级别 |
| `rqt_reconfigure` | 动态调参 |
| `rqt_image_view` | 查看图像话题 |
| `rqt_bag` | 图形化查看和回放 bag 文件 |
| `rqt_publisher` | 手动向话题发布消息 |
| `rqt_service_caller` | 手动调用服务 |

### rviz 与 Gazebo

rviz 把话题里的数据绘制出来，包括点云、图像、机器人模型、TF、路径和地图......

Gazebo 是物理仿真器，可以模拟重力、碰撞和传感器噪声，常与`gazebo_ros`配合发布和订阅话题。

```bash
rosrun rviz rviz
rosrun rviz rviz -d my_config.rviz
```

rviz 使用要点：

1. 先选择Fixed Frame，通常是`map`或`odom`
2. 用 Add 按话题类型添加显示项，比如LaserScan、PointCloud2、Image、RobotModel、TF
3. 把配置保存成 `.rviz` 文件，放进包的 `rviz/` 目录

### TF2 坐标变换

机器人系统里有大量坐标系：`map`、`odom`、`base_link`、`laser` 等。TF维护它们之间的变换关系，任何时刻都可以查询一个坐标系下的点在另一个坐标系下的坐标。

```bash
#发布静态变换：map → base_link，无旋转无平移
rosrun tf2_ros static_transform_publisher 0 0 0 0 0 0 map base_link

#查询两个坐标系之间的变换
rosrun tf2_ros tf2_echo map base_link

#生成当前 TF 树的结构图 frames.pdf
rosrun tf2_tools view_frames.py
```

约定的常见坐标系链：
```text
map → odom → base_link → {laser, camera, imu, ...}
```
> [!TIP]
> TF 是一棵坐标系树。每个节点广播自己到父坐标系的变换，其它节点查询即可`static_transform_publisher` 用于固定不变的变换，比如雷达安装位置；动态变换由 `tf2_ros::TransformBroadcaster` 广播

### rosbag
rosbag 把话题数据录成 `.bag` 文件，是调试、复现、离线分析的常用工具。
```bash
rosbag record -a -O all.bag          #录制所有话题
rosbag record /scan /tf /odom -O nav.bag
rosbag info nav.bag                  #查看话题、消息数、时长
rosbag play nav.bag                  #按原时间轴回放
rosbag play -r 0.5 nav.bag           #半速回放
rosbag play nav.bag --topics /scan   #只回放某个话题
```
>[!TIP]
>调试时遇到bug可通过录制机器人行为异常时的bag并用`rqt_plot`或`rostopic echo`进行回放分析

### 日志与诊断

- 日志级别：`ROS_DEBUG`、`ROS_INFO`、`ROS_WARN`、`ROS_ERROR`、`ROS_FATAL`，Python 用 `rospy.loginfo` 等。
- `rqt_console` 汇总日志，`rqt_logger_level` 可以在运行时切换级别。
- `roslaunch --screen` 让所有输出可见。
- `roswtf` 检查环境变量、`ROS_PACKAGE_PATH` 和计算图。
- `rosnode info` 查看节点连接，`rostopic hz` 检查频率，`rostopic delay` 检查延迟（消息需要带 header 时间戳）。

> [!WARNING]
> 高频数据（图像、点云）传输慢时，先排查是否是因为`queue_size` 太小导致丢帧、带宽不足、订阅端处理慢，而不是 ROS 本身

## 排查流程

```text
1. roscore在运行吗         rosnode list
2. 环境source了吗          echo $ROS_PACKAGE_PATH
3. 节点和话题存在吗         rostopic list、rqt_graph
4. 数据内容正确吗           rostopic echo、rostopic hz、rosbag
5. 日志说了什么             roslaunch --screen、rqt_console、~/.ros/log
```
