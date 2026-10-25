# ROS Notes

## 1. ROS Overview

ROS 的主要功能：

1. 用节点拆分功能，使感知、决策和控制模块可以独立开发；
2. 用 Topic、Service 和 Parameter 统一模块之间的接口；
3. 提供 RViz、rqt、rosbag 和 Gazebo 等调试、记录与仿真工具；
4. 提供相机、雷达、IMU、导航和机械臂等领域的开源软件包。

eg ： 在机器人系统中，一条典型的数据链：

```text
相机驱动 -> 目标识别 -> 位姿解算 -> 决策 -> 控制器 -> 硬件接口 -> 电机
```

## 2. ROS Architecture

可以从文件系统、计算图和通信机制三个层次理解 ROS1。

### 2.1 File System

文件系统层描述代码在磁盘上如何组织。

```text
catkin_ws/
├── src/                 # 源代码，应纳入 Git
│   └── my_package/
│       ├── CMakeLists.txt
│       ├── package.xml
│       ├── include/my_package/
│       ├── src/
│       ├── msg/
│       ├── srv/
│       ├── launch/
│       └── config/
├── build/               # 构建中间文件，一般不提交
└── devel/               # 开发环境产物，一般不提交
```

工作空间是 Catkin 项目的顶层目录，package 是 ROS 中最小的复用单位。一个工作空间可以包含多个 package。

常见目录的用途：

| 路径 | 用途 |
| --- | --- |
| `src/` | C++ 源文件 |
| `include/<package>/` | 对外公开的头文件 |
| `msg/` | 自定义消息定义 |
| `srv/` | 自定义服务定义 |
| `launch/` | 同时启动、配置多个节点 |
| `config/` | YAML 参数文件 |
| `package.xml` | 包的名称、版本、维护者和依赖 |
| `CMakeLists.txt` | 构建目标、消息生成和链接规则 |

构建并加载工作空间环境：

```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

如果安装了 `catkin_tools`，也可以使用：

```bash
catkin build
```

`catkin_make` 与 `catkin build` 不应在同一个构建目录中随意混用。每次新开终端后都需要 `source devel/setup.bash`，否则 ROS 可能找不到刚编译的软件包。

### 2.2 Computation Graph

计算图描述 ROS 程序运行后各组件之间的关系，主要元素包括：

- Node：独立运行的进程；
- Topic：异步数据通道；
- Message：Topic 上传输的数据结构；
- Service：同步的请求和响应；
- Parameter：由参数服务器保存的配置；
- Master：负责名称注册和连接发现。

可以用下面的命令观察计算图：

```bash
rqt_graph
rosnode list
rosnode info /node_name
```

### 2.3 Communication Layer

ROS1 节点先通过 XML-RPC 与 Master 交换注册和连接信息。Topic 的发布者和订阅者完成协商后，再通过 TCPROS 或 UDPROS 直接传输序列化后的消息。

- TCPROS 基于 TCP，可靠且使用广泛；
- UDPROS 基于 UDP，不保证可靠传输；
- Master 不转发 Topic 数据。

因此，Master 的作用更像“通讯录”，而不是数据中转站。

---

## 3. ROS Master and Topic Connection

以发布者向 `/cmd_vel` 发布 `geometry_msgs/Twist` 为例，通信过程如下：

1. 启动 `roscore`，Master 默认监听 `11311` 端口；
2. 发布者向 Master 注册节点地址、Topic 名称和消息类型；
3. 订阅者向 Master 注册自己要订阅的 Topic；
4. Master 将发布者的 XML-RPC 地址告知订阅者；
5. 订阅者联系发布者并协商传输协议；
6. 发布者把实际数据连接地址返回给订阅者；
7. 双方建立 TCPROS 或 UDPROS 连接并直接传输消息。

```text
Publisher --注册--> Master <--查询-- Subscriber
Publisher ======== 消息直连 ======== Subscriber
```

已有连接通常可以在 Master 停止后继续传输一段时间，但新节点无法完成注册和发现，参数服务器操作也会失败，因此实际系统不能把“Master 挂掉后还能传输”当作容错方案。

`roscore` 通常会启动：

- ROS Master；
- Parameter Server；
- `/rosout` 日志聚合节点。

---

## 4. Three Main Communication Methods

### 4.1 Topic

Topic 使用发布/订阅模型，适合连续变化的数据流，例如图像、激光雷达、里程计和控制指令。

主要特点：

- 异步通信，发布者发送后不等待处理结果；
- 支持一个或多个发布者和订阅者；
- 发布者与订阅者不需要知道彼此的节点名称；
- Topic 名称和消息类型都要一致；
- `queue_size` 决定本地缓冲队列长度，处理过慢时可能丢弃消息。

发布者示例：

```cpp
#include <ros/ros.h>
#include <std_msgs/String.h>

int main(int argc, char** argv) {
  ros::init(argc, argv, "talker");
  ros::NodeHandle nh;

  ros::Publisher pub =
      nh.advertise<std_msgs::String>("/chatter", 10);
  ros::Rate rate(1);

  while (ros::ok()) {
    std_msgs::String msg;
    msg.data = "hello";
    pub.publish(msg);
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}
```

订阅者示例：

```cpp
#include <ros/ros.h>
#include <std_msgs/String.h>

void callback(const std_msgs::String::ConstPtr& msg) {
  ROS_INFO("I heard: %s", msg->data.c_str());
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "listener");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/chatter", 10, callback);
  ros::spin();
  return 0;
}
```

回调函数应尽量短。耗时计算会阻塞同一回调队列中的后续消息，可以考虑拆分节点、使用工作线程或使用异步 spinner。

常用命令：

```bash
rostopic list
rostopic info /chatter
rostopic echo /chatter
rostopic hz /chatter
rostopic pub /chatter std_msgs/String "data: 'hello'"
```

普通订阅者晚启动后仍可以接收之后发布的新消息。如果发布者启用了 latching，新的订阅者还会立即收到发布者保留的最后一条消息。

### 4.2 Service

Service 使用请求/响应模型，适合低频、需要明确返回结果的操作，例如切换模式、生成对象或修改一次配置。

`.srv` 文件以 `---` 分隔请求和响应：

```text
float32 distance
---
bool success
string message
```

Service 调用通常是同步阻塞的。服务器没有启动、回调耗时过长或调用未设置合理的等待策略，都可能让客户端停在调用位置。因此，高频传感器数据不适合通过 Service 传输。

常用命令：

```bash
rosservice list
rosservice type /service_name
rossrv show package_name/ServiceType
rosservice call /service_name "distance: 1.5"
```

### 4.3 Parameter Server

参数服务器是 Master 中的全局 key-value 存储，适合保存阈值、PID 参数、Topic 名称和相机内参等配置。

参数支持层级命名：

```text
/robot/camera/exposure
```

常见名称形式：

| 名称 | 含义 |
| --- | --- |
| `/robot/speed` | 绝对名称，从根命名空间开始 |
| `speed` | 相对名称，拼接到当前命名空间 |
| `~speed` | 私有名称，属于当前节点 |

C++ 读取参数并提供默认值：

```cpp
ros::NodeHandle pnh("~");
double warning_distance = 1.0;
pnh.param<double>("warning_distance", warning_distance, 1.0);
```

常用命令：

```bash
rosparam list
rosparam get /robot/speed
rosparam set /robot/speed 1.0
rosparam load config/params.yaml
rosparam dump params_backup.yaml
rosparam delete /robot
```

参数服务器不适合高频数据和大型二进制数据。参数发生变化后，节点也不一定自动改变行为。只有节点再次读取参数、轮询参数或使用 dynamic_reconfigure 等机制时，新值才会真正影响节点。

### 4.4 Communication Choice

| 需求 | 选择 |
| --- | --- |
| 连续、高频数据流 | Topic |
| 偶发操作并需要返回结果 | Service |
| 启动配置或低频配置 | Parameter |
| 长时间任务，需要反馈进度或取消 | Action |

---

## 5. Custom Messages and Services

自定义消息示例：

```text
# msg/ObstacleDistance.msg
float32 min_distance
bool valid
```

自定义服务示例：

```text
# srv/SetWarningDistance.srv
float32 distance
---
bool success
string message
```

`package.xml` 需要声明生成期和运行期依赖：

```xml
<buildtool_depend>catkin</buildtool_depend>
<depend>roscpp</depend>
<depend>sensor_msgs</depend>
<build_depend>message_generation</build_depend>
<exec_depend>message_runtime</exec_depend>
```

`CMakeLists.txt` 中需要配置生成规则：

```cmake
find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  message_generation
)

add_message_files(FILES ObstacleDistance.msg)
add_service_files(FILES SetWarningDistance.srv)
generate_messages(DEPENDENCIES sensor_msgs)

catkin_package(CATKIN_DEPENDS
  roscpp
  sensor_msgs
  message_runtime
)

include_directories(${catkin_INCLUDE_DIRS})

add_executable(scan_processor src/scan_processor.cpp)
add_dependencies(scan_processor
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)
target_link_libraries(scan_processor ${catkin_LIBRARIES})
```

`add_dependencies` 可以保证自定义消息头文件先生成，再编译引用它的节点。修改 `.msg` 或 `.srv` 后要重新构建，并重新加载工作空间环境。

---

## 6. Launch and YAML

### 6.1 YAML Rules

YAML 用缩进表达层级，常用于 ROS 参数文件。

基本规则：

- 只使用空格缩进，不使用 Tab；
- 同一层级必须对齐；
- 冒号后必须有空格；
- 类型需要明确，`30`、`3.0` 和 `"30"` 的类型不同；
- `#` 后的内容是注释。

示例：

```yaml
scan_processor:
  laser_topic: /scan
  pointcloud_topic: /rslidar_points
  queue_size: 10

obstacle_monitor:
  warning_distance: 1.0
  warning_enabled: true
```

### 6.2 Launch File

Launch 文件负责组织多个节点，并加载参数：

```xml
<launch>
  <rosparam
      command="load"
      file="$(find smb_highlevel_controller)/config/obstacle.yaml"/>

  <node pkg="smb_highlevel_controller"
        type="scan_processor"
        name="scan_processor"
        output="screen"/>

  <node pkg="smb_highlevel_controller"
        type="obstacle_monitor"
        name="obstacle_monitor"
        output="screen"/>
</launch>
```

`$(find package_name)` 会在运行时查找 package 路径，因此 launch 文件不依赖某台电脑上的绝对目录。

---

## 7. ros_control

`ros_control` 的目标是把控制算法和具体硬件隔离。它没有创造新的通信机制，而是在 ROS 的 Topic、Service 和插件体系上组织控制器。

```text
控制指令
   |
Controller Manager
   |
Controller: PID、轨迹、差速控制
   |
RobotHW / hardware_interface
   |
EtherCAT 或 CAN
   |
电机和编码器
```

主要组成：

- Controller：实现 PID、轨迹或底盘控制逻辑；
- Controller Manager：加载、启动、停止和切换控制器；
- RobotHW：把统一的关节接口映射到真实硬件；
- `read()`：从编码器或总线读取状态；
- `write()`：把控制器输出转换为硬件指令；
- Joint Limits：约束位置、速度和加速度；
- Transmission：描述关节与执行器之间的传动关系。

这样做的好处是：更换电机或总线时，主要修改硬件接口；更换控制算法时，主要替换 Controller；仿真和实车也可以尽量复用相同控制器。

对于课程中的纯上位机方案，闭环控制和状态机运行在上位机节点中，EtherCAT 与 CAN 通道主要负责按周期传输数据。优点是修改控制逻辑后只需重新编译上位机代码，不必反复烧写下位机固件。缺点是控制实时性更依赖上位机操作系统、总线周期和进程调度。

---

## 8. Homework Data Flow

课件引用的 `ROS_Class` 作业把本节知识组合成一条完整链路：

```text
Gazebo / SMB sensors
  |-- /scan (sensor_msgs/LaserScan)
  `-- /rslidar_points (sensor_msgs/PointCloud2)
             |
      scan_processor
             |
  /homework/obstacle (ObstacleDistance)
             |
      obstacle_monitor
        |          |
     日志报警     SetWarningDistance Service
        |
 dynamic_reconfigure: warning_enabled
```

### 8.1 `scan_processor`

主要任务：

1. 从参数服务器读取 Topic 名称和队列长度；
2. 订阅 `LaserScan` 并寻找最近的有效距离；
3. 过滤 NaN、无穷值以及超出传感器范围的测量；
4. 订阅 `PointCloud2` 并统计点数量；
5. 发布自定义的 `ObstacleDistance` 消息。

如果一帧中没有有效距离，应发布：

```text
valid: false
min_distance: 0.0
```

### 8.2 `obstacle_monitor`

主要任务：

1. 订阅 `/homework/obstacle`；
2. 读取报警距离，默认值为 `1.0 m`；
3. 仅在数据有效、距离严格小于阈值且报警开关开启时报警；
4. 通过 Service 修改运行中的阈值；
5. 拒绝 NaN、无穷值、零和负数；
6. 通过 dynamic_reconfigure 在线开关报警功能。

Service 修改的是内存中的运行值，不会自动修改 YAML 文件。节点重启后，仍会从 YAML 或默认值重新加载。

### 8.3 rosbag

`rosbag` 可以记录 Topic 消息并在离线环境中回放：

```bash
rosbag record -O obstacle.bag /homework/obstacle
rosbag info obstacle.bag
rosbag play obstacle.bag
```

Bag 记录的是指定 Topic 上的消息。普通 Service 调用和 dynamic_reconfigure 操作不会因为“录了某个 Topic”而自动完整保存，需要根据实际要复现的信息额外记录相关 Topic 或日志。

### 8.4 RViz and rqt

- RViz：显示 `LaserScan`、`PointCloud2`、TF 等空间数据；
- `rqt_graph`：观察节点和 Topic 的连接；
- `rqt_reconfigure`：在线修改 dynamic_reconfigure 参数；
- PlotJuggler：观察时间序列曲线；
- rosbag：记录与回放 Topic 数据。

---

## 9. Debugging Workflow

排查 ROS 问题时，先判断问题属于哪一层。

### 9.1 Package Not Found

```bash
rospack find package_name
echo "$ROS_PACKAGE_PATH"
source ~/catkin_ws/devel/setup.bash
```

检查 package 是否位于工作空间的 `src` 中，名称是否与 `package.xml` 一致，以及当前终端是否加载了工作空间环境。

### 9.2 Node Not Running

```bash
rosnode list
rosnode info /node_name
rosnode ping /node_name
```

先看节点启动终端中的第一条错误，再检查可执行目标是否正确构建、依赖是否声明完整。

### 9.3 Topic Has No Data

```bash
rostopic list
rostopic info /topic_name
rostopic type /topic_name
rostopic echo /topic_name
rostopic hz /topic_name
```

依次检查 Topic 名称、消息类型、发布者数量、订阅者数量和实际频率。节点连接正常但仍丢数据时，再检查队列长度、回调耗时和网络带宽。

### 9.4 Service Call Hangs or Fails

```bash
rosservice list
rosservice type /service_name
rossrv show package_name/ServiceType
```

检查服务器是否已经启动、服务名称是否正确、请求字段是否完整，以及服务回调是否被阻塞。

### 9.5 Parameter Has No Effect

```bash
rosparam get /parameter/name
rosparam set /parameter/name value
```

参数服务器中的值已经变化，并不代表节点会自动重新读取。需要检查程序是只在启动时读取，还是持续轮询或使用 dynamic_reconfigure。

---

## 10. Common Commands

```bash
# Package and build
rospack find <package>
roscd <package>
catkin_create_pkg <package> roscpp std_msgs
catkin_make

# Run
roscore
rosrun <package> <node>
roslaunch <package> <file.launch>

# Node
rosnode list
rosnode info <node>
rosnode ping <node>
rosnode kill <node>

# Topic
rostopic list
rostopic info <topic>
rostopic echo <topic>
rostopic hz <topic>

# Message and service
rosmsg show <package/Message>
rossrv show <package/Service>
rosservice list
rosservice call <service> <request>

# Parameter
rosparam list
rosparam get <name>
rosparam set <name> <value>
rosparam load <file> [namespace]
rosparam dump <file> [namespace]

# Debug and record
rqt_graph
rviz
rosbag record -a
rosbag info <file.bag>
rosbag play <file.bag>
```
