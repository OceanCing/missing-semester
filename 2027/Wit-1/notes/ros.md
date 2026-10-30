# ROS 学习笔记：架构与通信

## 1. ROS 解决什么问题

ROS 运行在操作系统之上，提供通信、硬件抽象和开发工具。相机驱动、识别、解算、决策和控制可以拆成模块，通过统一接口协作。`rqt_graph` 看连接，RViz 看空间数据，`rosbag` 录制和回放数据。

## 2. 架构与 Master

| 层次 | 核心内容 |
| --- | --- |
| 文件系统：代码组织 | 工作空间、包、源码、launch、配置 |
| 计算图：运行协作 | 节点、话题、消息、服务、参数 |
| 通信：数据传输 | 发现、连接、序列化、TCPROS / UDPROS |

节点执行任务，普通 roscpp 节点通常对应一个进程；话题是数据通道，消息定义数据结构。

`roscore` 启动 Master、参数服务器和 `rosout` 日志节点。Master 负责名称注册与查询，默认端口为 `11311`。典型话题通信过程：

```text
发布者、订阅者向 Master 注册
    ↓ Master 返回发布者的 XML-RPC 地址
订阅者与发布者协商传输方式，获得数据端口
    ↓ 建立连接并校验消息类型
发布者直接向订阅者传数据（通常使用 TCPROS）
```

**话题数据不经过 Master 转发。** 新发布者加入时，Master 仍会通知订阅者；Master 退出后，已有连接可能继续传输，但新连接发现和参数访问会受影响。[注册机制](https://github.com/ros/ros_comm/blob/noetic-devel/tools/rosmaster/src/rosmaster/master_api.py)

## 3. 工作空间与功能包

```text
catkin_ws/
├── src/my_pkg/
│   ├── package.xml       # 包信息与依赖声明
│   ├── CMakeLists.txt    # 构建目标、头文件路径、链接与安装规则
│   ├── src/、include/   # 源码、头文件
│   ├── msg/、srv/       # 自定义消息、服务
│   └── launch/、config/ # 启动编排、YAML 参数
├── build/               # 构建中间文件
├── devel/               # 开发产物与 setup.bash
└── install/             # 执行安装步骤时生成
```

源码和配置进入 Git，构建产物通常忽略。`package.xml` 声明依赖，`CMakeLists.txt` 描述怎样构建；一个包可生成多个程序，一个目标可包含多个 `.cpp`。详见[第二课笔记](cmake_study.md)。

新建包与构建的命令示意，在已加载 Noetic 环境的终端执行：

```bash
cd ~/catkin_ws/src
catkin_create_pkg my_pkg roscpp std_msgs
cd ~/catkin_ws
catkin build my_pkg
source devel/setup.bash
```

`source` 让当前终端找到包和程序。自定义 `.msg` / `.srv` 还要配置 `message_generation`、`message_runtime`、生成规则和目标依赖。[构建说明](https://github.com/ros/catkin/blob/noetic-devel/doc/howto/format2/building_msgs.rst)

## 4. 话题、服务与参数怎么选

| 机制 | 用途 | 例子 | 关键接口 |
| --- | --- | --- | --- |
| Topic | 持续、异步的数据流，可多对多 | 图像、检测结果、速度指令 | `advertise`、`publish`、`subscribe` |
| Service | 一次请求对应一次应答，可有多个客户端 | 生成海龟、清屏、切换模式 | `advertiseService`、`serviceClient`、`call` |
| Parameter | 保存配置，供节点读取 | 阈值、相机参数、PID 参数 | `param`、`getParam`、`setParam` |

**话题重点：**

- 发布、订阅双方解析后的话题名和消息类型必须匹配。
- 先调用 `ros::init`，再创建 `NodeHandle`；用 `ros::spin()` 或循环中的 `ros::spinOnce()` 处理回调。
- `queue_size` 是缓冲深度。回调太慢会积压，订阅队列满时丢旧消息；增大队列也可能增大延迟。[队列实现](https://github.com/ros/ros_comm/blob/noetic-devel/clients/roscpp/src/libros/subscription_queue.cpp)
- 晚启动通常只能收到后续消息；`latch=true` 让仍在运行的发布者向新订阅者补发最后一条。[接口说明](https://github.com/ros/ros_comm/blob/noetic-devel/clients/roscpp/include/ros/node_handle.h)

**服务重点：** `.srv` 用 `---` 分隔请求与应答。`call()` 阻塞调用线程，要检查返回值。`waitForExistence(ros::Duration(2.0))` 只限制等待服务上线的时间，不能给后续 `call()` 设置执行超时。[服务接口](https://github.com/ros/ros_comm/blob/noetic-devel/clients/roscpp/include/ros/service_client.h)

**参数重点：** 修改服务器中的值后，节点要重新读取才会生效；运行时调参需要程序支持。私有参数表示命名范围，不限制其他节点访问。

## 5. YAML 与参数命名空间

YAML 的核心是**缩进表示层级**：用空格、同层对齐，键值写成 `key: value`。常用结构是标量、列表和字典；布尔值统一写 `true` / `false`。

```yaml
camera:
  exposure: 30
  name: "front cam"
debug: false
thresholds: [0.3, 0.6, 0.9]
```

字符串可以含空格；加引号能减少歧义，并避免数字等被解析成其他类型。[YAML 规范](https://yaml.org/spec/1.2.2/)

假设节点全名是 `/vision/detector`，普通 `NodeHandle` 使用 `/vision` 命名空间：

| 写法 | 解析结果 |
| --- | --- |
| `/camera/exposure` | `/camera/exposure`，绝对名 |
| `camera/exposure` | `/vision/camera/exposure`，相对名 |
| `~camera/exposure` | `/vision/detector/camera/exposure`，私有名 |

C++ 读取私有参数时，用 `ros::NodeHandle pnh("~")`，再调用 `pnh.param<int>("camera/exposure", exposure, 30)`；`30` 是默认值。

```bash
rosparam load params.yaml /vision/detector
rosparam get /vision/detector/camera/exposure
rosparam dump saved.yaml /vision/detector
```

launch 用 `<rosparam command="load" file="$(find my_pkg)/config/params.yaml"/>` 加载；放在 `<node>` 内时进入节点的私有命名空间。**文件层级、加载位置、读取路径必须一致。** `dump` 导出指定子树，重新加载时注意命名空间是否重复。[导入导出实现](https://github.com/ros/ros_comm/blob/noetic-devel/tools/rosparam/src/rosparam/__init__.py)

## 6. ros_control 的核心分工

```text
硬件状态 → RobotHW.read()
         → ControllerManager.update()（控制器计算指令）
         → RobotHW.write() → 硬件
```

`RobotHW` 封装硬件状态与命令接口，Controller 计算指令，`ControllerManager` 加载、启动和切换控制器。程序组织周期性的 `read → update → write` 循环。[RobotHW 接口](https://github.com/ros-controls/ros_control/blob/noetic-devel/hardware_interface/include/hardware_interface/robot_hw.h)

课件的队内方案把主要视觉与控制逻辑放在上位机，经 EtherCAT / CAN 传到电机。记住分工：**控制器计算，总线传输，硬件执行。**

## 7. 海龟示例与排查顺序

在三个已加载 ROS 环境的终端分别运行：

```bash
roscore
rosrun turtlesim turtlesim_node
rosrun turtlesim turtle_teleop_key
```

键盘终端保持焦点后，用方向键控制。键盘节点向 `/turtle1/cmd_vel` 发布 `geometry_msgs/Twist`，海龟节点订阅并运动，同时发布 `/turtle1/pose`。

| 要确认什么 | 命令 |
| --- | --- |
| 节点是否启动、接口是什么 | `rosnode list`、`rosnode info /turtlesim` |
| 发布订阅关系、消息和频率 | `rostopic info /turtle1/cmd_vel`、`rostopic echo /turtle1/pose`、`rostopic hz /turtle1/pose` |
| 消息与服务字段 | `rosmsg show geometry_msgs/Twist`、`rossrv show turtlesim/Spawn` |
| 手动调用服务 | `rosservice call /spawn "{x: 2.0, y: 3.0, theta: 0.0, name: 't2'}"` |
| 查看整体连接 | `rqt_graph` |
| 录制与回放指定数据 | `rosbag record -O turtle.bag /turtle1/pose`、`rosbag play turtle.bag` |

改背景色：`rosparam set /turtlesim/background_r 255`，再执行 `rosservice call /clear`。后者重新读取背景参数、更新颜色并清除轨迹。[turtlesim 实现](https://github.com/ros/ros_tutorials/blob/noetic-devel/turtlesim/src/turtle_frame.cpp)

排查顺序：**环境和包 → 节点存活 → 名称与类型 → 内容和频率 → 回调、队列与网络。** 看到话题不等于回调已收到数据；多机还要检查 `ROS_MASTER_URI`、节点地址与网络连通性。
