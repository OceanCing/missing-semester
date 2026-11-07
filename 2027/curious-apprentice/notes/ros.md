# ROS 基础：架构、通信与工作空间

## 1. ROS 是什么

ROS（Robot Operating System）通常被称作“机器人操作系统”，更准确地说，它是在 Linux 等操作系统之上的**机器人软件框架和工具集**。ROS 本身不替代 Ubuntu，也不自动提供实时控制；它提供一套通用机制，让不同进程中的机器人程序交换数据，并配有驱动、算法、可视化、仿真和构建工具。

可以把 ROS 的用途归为三类：

1. **通信**：用 topic、service、action 等接口连接独立的程序。
2. **集成**：通过软件包复用传感器驱动、坐标变换、导航、控制等功能。
3. **开发调试**：用 catkin 编译，用 launch 编排启动，用 RViz、rqt 查看状态，用 rosbag 录制和回放数据。

例如，一台移动机器人可以把相机驱动、目标识别、运动决策、底盘控制分别做成节点。它们只需约定话题名和消息类型，就能协作；更换相机或识别算法时，通常不必把整个程序重写。

## 2. 三层架构

| 层次 | 关注的问题 | 主要内容 |
| --- | --- | --- |
| 文件系统层 | 代码和配置放在哪里？ | 工作空间、功能包、源码、`package.xml`、`CMakeLists.txt`、launch、配置文件 |
| 计算图层 | 有哪些程序在运行？它们如何协作？ | 节点、话题、消息、服务、参数、动作 |
| 通信层 | 数据怎样从一个进程到另一个进程？ | ROS 1 Master 的发现与注册、节点间传输、消息序列化 |

排查问题时先判断属于哪层：找不到包，检查工作空间与环境；节点连不上，检查名称、类型和网络；数据慢或不稳定，再检查发布频率、队列、带宽和处理耗时。

## 3. 计算图的基本概念

### 3.1 节点 Node

节点是一个运行中的 ROS 进程，通常负责一个相对独立的任务，例如读取相机、识别目标、规划路径或控制电机。节点通过 ROS 接口协作，而不是直接依赖彼此的内部代码。

常见工具：

```bash
rosnode list                 # 当前有哪些节点
rosnode info /node_name      # 查看节点的话题和服务
rosnode ping /node_name      # 检查节点是否响应
```

### 3.2 话题 Topic 与消息 Message

话题是**持续数据流**的命名通道，采用发布/订阅模式。发布者发布消息，订阅者收到后执行回调；发布者和订阅者不需要互相知道对方是谁。一个话题可以有多个发布者和订阅者。

消息定义数据的结构和字段类型。比如 `geometry_msgs/Twist` 用线速度和角速度描述运动指令。双方要连接，话题名称和消息类型必须一致。

```bash
rostopic list                         # 列出话题
rostopic info /cmd_vel                # 查看类型、发布者、订阅者
rostopic echo /cmd_vel                # 打印收到的消息
rostopic hz /camera/image_raw         # 估算发布频率
rostopic type /cmd_vel                # 查询消息类型
rosmsg show geometry_msgs/Twist       # 查看消息字段
```

话题适合传感器数据、里程计、检测结果和连续控制指令。队列长度（`queue_size`）是等待处理消息的缓冲容量；当生产速度持续高于消费速度时，较旧的消息可能被丢弃。实时数据通常应关注“最新值”，而不是积压大量过期数据。

### 3.3 服务 Service

服务是**一次请求和一次应答**。客户端发起调用，服务端处理并返回结果。`.srv` 文件用 `---` 分隔请求字段和应答字段。

服务适合低频、需要明确结果的操作，例如切换模式、清空地图或生成模型。调用通常会等待应答，不适合高频数据流；服务端未启动、网络不可达或回调耗时时，客户端可能等待较久。

```bash
rosservice list
rosservice type /clear
rossrv show turtlesim/Spawn
rosservice call /clear
```

### 3.4 参数 Parameter

ROS 1 参数服务器保存共享的配置键值，例如阈值、相机参数或 PID 参数。参数名有命名空间，节点可以读取或修改。它适合配置，不适合高频传感器流；参数服务器随 `roscore` 生命周期存在，停止后参数不会作为持久数据库保留。

```bash
rosparam list
rosparam get /camera/exposure
rosparam set /camera/exposure 30
rosparam load config/params.yaml /robot
rosparam dump params.yaml /robot
```

常见参数名：绝对名 `/robot/speed` 从根命名空间开始；相对名 `speed` 会受节点或 launch 命名空间影响；私有名 `~gain` 属于当前节点。

### 3.5 Action

Action 用于**耗时任务**，除了最终结果，还能持续反馈进度，并允许取消，例如导航到目标点或执行机械臂轨迹。简单记忆：连续数据用 topic；一次问答用 service；要反馈、可取消的长任务用 action；配置用 parameter。

## 4. ROS 1 Master 与一次话题连接

ROS 1 使用 Master（由 `roscore` 启动）帮助节点发现彼此。典型的 topic 连接流程如下：

1. 启动 `roscore`。
2. 发布者向 Master 注册话题名称、消息类型和自身地址。
3. 订阅者向 Master 查询自己订阅的话题。
4. Master 把发布者地址返回给订阅者。
5. 订阅者与发布者建立连接，之后消息由节点直接传输。

**消息数据通常不经过 Master。** Master 主要负责名称注册与发现；节点间的实际传输通常使用 TCPROS。Master 不可用时，已经建立的连接可能继续传输，但新节点无法正常发现和建立连接。机器跨主机通信时，除 `ROS_MASTER_URI` 外，还要确保节点地址可达、防火墙放行，并正确设置 `ROS_IP` 或 `ROS_HOSTNAME`。

```bash
roscore
# 通常等价于启动 Master、rosout 和参数服务器
```

## 5. 工作空间与功能包

工作空间是源码包的集合。catkin 工作空间常见结构如下：

```text
catkin_ws/
├── src/          # 版本控制中的源码和功能包
├── build/        # 编译中间产物
└── devel/        # 开发空间：生成的程序和环境脚本
```

功能包（package）是 ROS 里组织和复用代码的基本单元。典型目录：

```text
my_pkg/
├── package.xml       # 包名、版本、依赖
├── CMakeLists.txt    # 编译和链接规则
├── include/my_pkg/   # 对外头文件
├── src/              # C++ 源文件
├── msg/              # 自定义消息（需要时）
├── srv/              # 自定义服务（需要时）
├── launch/           # 启动配置
├── config/           # YAML 等参数配置
└── README.md
```

`package.xml` 声明包的身份和依赖；`CMakeLists.txt` 告诉 catkin/CMake 如何找到依赖、生成消息和编译目标。自定义消息或服务还要在构建配置中声明生成步骤，并在 `package.xml` 中声明相应依赖。

### 创建和编译（catkin_tools 示例）

使用 `catkin build` 时，可以按下面方式建立工作空间。若项目明确使用 `catkin_make`，则应遵循项目要求，不要在同一工作空间中混用两套构建工具。

```bash
mkdir -p ~/demo_ws/src
cd ~/demo_ws
catkin init
cd src
catkin_create_pkg hello_world roscpp rospy std_msgs
cd ..
catkin build
source devel/setup.bash
```

每个新终端都要加载工作空间环境；也可以在 shell 配置中自动 source。使用 `rospack find hello_world` 可确认 ROS 是否能找到包。

```bash
rospack find hello_world
rosrun hello_world <node_name>
roslaunch hello_world <file.launch>
```

## 6. 启动文件与 YAML 参数

`.launch` 文件可以一次启动多个节点、设置命名空间、传入参数并加载配置文件。这样可以把一次运行所需的节点组合成可复现的启动入口。

YAML 用缩进表示层级，只使用空格；冒号后要有空格。下面是一个参数文件示例：

```yaml
camera:
  exposure: 30
  frame_id: "camera_link"
  enabled: true
thresholds: [0.3, 0.6, 0.9]
```

在 launch 文件中加载：

```xml
<launch>
  <rosparam file="$(find hello_world)/config/params.yaml"
            command="load" ns="robot" />
  <node pkg="hello_world" type="camera_node" name="camera" output="screen" />
</launch>
```

用 `rosparam get /robot/camera/exposure` 检查实际加载的参数路径和类型。参数路径拼错或命名空间重复，是配置看似加载成功但节点读不到值的常见原因。

## 7. 常用工具速查

| 目的 | 命令 |
| --- | --- |
| 启动 ROS 1 核心 | `roscore` |
| 查找包路径 | `rospack find <pkg>` |
| 编译工作空间 | `catkin build` 或项目指定的构建命令 |
| 运行单个节点 | `rosrun <pkg> <node>` |
| 启动一组节点 | `roslaunch <pkg> <file.launch>` |
| 查看节点关系图 | `rqt_graph` |
| 可视化机器人、坐标系和传感器 | `rviz` |
| 录制所有话题 | `rosbag record -a` |
| 查看/回放 bag | `rosbag info <file.bag>` / `rosbag play <file.bag>` |
| 查看话题数据 | `rostopic echo <topic>` |

一种排查顺序：

1. `rosnode list`：确认节点是否启动。
2. `rostopic list` 和 `rosnode info`：确认接口是否存在、是否连对。
3. `rostopic echo`、`rostopic hz`：确认数据内容和频率。
4. `rosparam get`：核对参数值与命名空间。
5. 回到启动终端查看最早出现的错误；必要时用 `rqt_graph` 和 rosbag 缩小问题范围。

## 8. ROS 1 与 ROS 2 的区别

ROS 1 常见工具是 `roscore`、`rosrun`、`roslaunch` 和 catkin；ROS 2 常见工具是 `ros2 run`、`ros2 launch` 和 colcon，通信架构也不同。ROS 1 的 Master 机制不能套用到 ROS 2。

ROS Noetic 是 ROS 1 的最后一个发行版，已于 **2025-05-31** 结束官方支持。

## 9. 继续学习

- [ROS Wiki：ROS 概念](http://wiki.ros.org/ROS/Concepts)
- [ROS Wiki：Topics](http://wiki.ros.org/Topics)
- [ROS Wiki：Services](http://wiki.ros.org/Services)
- [ROS Wiki：roslaunch](http://wiki.ros.org/roslaunch)
- 后续可接着学习 URDF 与 Xacro、TF 坐标变换、ros_control、Gazebo 和导航。

