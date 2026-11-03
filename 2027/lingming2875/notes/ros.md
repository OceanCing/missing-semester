# ROS 架构与通信---（工程代码放哪里？节点之间怎么相互传递信息？）

## 课程信息

- 主题：ROS 架构与通信
- 课程对象：机器人开发入门
- 本课涉及：
  1. ROS 是什么、能做什么
  2. ROS 在机器人系统中的位置,优势（why is it）
  3. ROS 的三层架构
  4. ROS Master 和节点通信过程
  5. 工作空间和功能**包**结构
  6. `package.xml` 与 `CMakeLists.txt`
  7. 话题、服务和参数服务器(三大通信)
  8. YAML 配置文件
  9. 小乌龟、`rosnode` 与常用命令

---

## 1. ROS 是什么

### 1.1 ROS 的定义

ROS 是 **Robot Operating System（机器人操作系统）** 的缩写。

ROS 常被称为机器人领域的“元操作系统”或“中间件”。它运行在 Linux 等操作系统之上，为机器人软件提供一组通用服务和工具，例如：

- 硬件抽象
- 设备驱动
- 进程间通信
- 软件包管理
- 常用算法和开发库
- 程序编译、启动、调试和数据记录工具

可以把 ROS 理解成：

> **运行在 Linux 上的机器人中间件和工具箱。**

它帮助机器人软件中的多个程序进行协作，但不是电脑开机后进入桌面的那种操作系统 (不能单独开机启用，不负责CPU内存)

### 1.2 本课程使用的版本

以 **ROS 1 Noetic** 为例：

- ROS 版本：ROS 1 Noetic
- 操作系统环境：Ubuntu 20.04
- C++ ROS 客户端库：`roscpp`
- 课程示例主要使用 C++ 节点

ROS 1 Noetic 是 ROS 1 的最后一个主要发行版本。实际使用时，应确认项目所用的 ROS 发行版与 Ubuntu、依赖包和构建工具相匹配。

### 1.3 ROS 的三类能力

#### 通信中间件

ROS 提供节点之间的通信方式：

- **话题**：传递连续的数据流---车上相机图像，IMU陀螺仪or底盘数据，
- **服务**：执行一次请求并返回结果---偶尔调用
- **参数服务器**：保存多个节点共享的配置。（存静态配置：PID，相机分辨率）

#### 工具链

ROS 提供机器人软件开发和调试工具：

- `catkin`：构建 ROS 工程。(编译c++代码）
- `launch`：一次启动多个节点并设置参数。（不必一一rosrun）
- `rviz`：可视化机器人模型、点云、坐标系等。
- `rqt`：图形化查看和调试 ROS 系统。
- `rosbag`：录制与回放 ROS 数据。（赛后复盘bug)

#### 软件生态

ROS 社区提供了大量可复用的软件包，例如：
>直接用
- 相机、激光雷达、IMU、串口等驱动
- OpenCV、PCL、Eigen 等常见库。(evision and matrix calculate)
- Navigation、MoveIt 等功能框架。(导航与运动规划)
- 机器人模型、算法和仿真相关的软件包。

### 1.4 ROS 的发展简史

课件列出的大致时间线：

- 2007 年：斯坦福为 PR2 机器人项目启动相关工作。
- 2010 年：ROS 1.0 开源。
- 2020 年：发布 ROS 1 Noetic。
- 2025 年：ROS 1 Noetic 停止维护。

---

## 2. ROS 在机器人中的作用

### 2.1 从传感器到执行机构的流程

以机器人视觉和运动控制为例，数据大致沿下面的流程传递：

```text
相机
  ↓
图像话题 /image_raw
  ↓
识别节点
  ↓
跟踪或目标信息，例如 /track
  ↓
解算节点  /gimbal_cmd等---坐标，角度，底盘移动
  ↓
云台指令 /gimbal_cmd
底盘指令 /cmd_chassis 或 /cmd_vel
  ↓
决策与控制 power_limit / heat_limit---做控制逻辑，输出最终指令
  ↓
通信中枢，例如 EtherCAT
  ↓
电机驱动器与电机
```

### 2.2 ROS 如何帮助接入硬件

常见的机器人硬件通常有现成驱动包，可以减少从寄存器层面编写驱动的工作。

举例：

- `usb_cam`：USB 相机驱动。
- `velodyne_driver`：Velodyne 激光雷达驱动。
- `serial`：串口通信相关包。

硬件驱动通常把传感器数据转换成 ROS 话题，供其他节点订阅。

### 2.3 ROS 如何帮助复用算法库

机器人软件可以复用常用 C++ 库，例如：

- OpenCV：图像处理。
- PCL：点云处理。
- Eigen：矩阵和向量运算。
- Navigation：移动机器人导航。
- MoveIt：机械臂运动规划。
- `tf2`：坐标变换。

函数示例：

- `cv::solvePnP`：根据图像特征求解位姿。
- `pcl::VoxelGrid`：点云体素滤波。
- `tf2`：处理不同坐标系之间的变换。

### 2.4 ROS 如何帮助调试

- 使用 `rviz` 查看点云、坐标系和机器人状态。
- 使用 `rqt` 或 `rqt_graph` 查看节点与话题关系。
- 使用 `rosbag` 录制现场数据，再在实验室回放。
- 使用 PlotJuggler 等工具查看曲线数据。

录制所有话题的示例：

```bash
rosbag record -a
```

### 2.5 ROS 如何帮助仿真

- 可以先在 Gazebo 等仿真环境里验证程序逻辑。
- 仿真能帮助新成员熟悉系统，不必一开始就操作真车。
- rosbag 录制的数据可以反复回放，用来检查程序行为。

### 2.6 两个基础概念

刚开始学习 ROS 时，优先记住：

- **节点（Node）**：谁在执行工作。
- **话题（Topic）**：节点之间传递什么数据。

ROS 系统中的许多功能，都可以从节点如何工作、节点之间如何通信来理解。

---

## 3. 为什么使用 ROS

课件总结了 ROS 的几个优势。

### 3.1 面向机器人开发

ROS 把机器人常用功能作为基础能力提供，例如：

- 发布与订阅。
- 坐标变换。
- 参数配置。
- 数据录制。
- 硬件驱动。
- 机器人可视化。

### 3.2 软件生态丰富

遇到常见问题时，通常可以搜索到社区已有的解决方案。使用现成软件包并根据项目需求修改，往往比从头实现更高效。

### 3.3 便于调试和团队协作

- `rqt_graph` 可以展示节点之间的连接关系。
- 一个团队成员可以负责相机节点，另一个负责识别或控制节点。
- 只要接口约定清楚，各模块可以并行开发。

### 3.4 适合 C++ 工程

课件的核心节点使用 C++ 和 `roscpp`。OpenCV、PCL、Eigen 等常用机器人库也广泛提供 C++ 接口，方便在同一工程中使用。

### 3.5 支持数据录制与回放

赛场或实车运行时的问题可能难以复现。使用 `rosbag` 可以保存当时的话题数据，回到实验室再回放并逐步分析。

### 3.6 支持仿真开发

可以先在 Gazebo 等环境里测试控制逻辑，再部署到实车。仿真适合早期验证和训练，但最终仍需在真实硬件上确认实际表现。

---

## 4. 课件中的机器人方案：纯上位机

### 4.1 系统分工

课件展示的方案将视觉和控制逻辑放在上位机运行：

```text
上位机（NUC / miniPC）  # NUC is short of next unit of computer
├── ROS 节点
│   ├── 目标检测
│   ├── 解算            # 决策
│   ├── 闭环控制
│   └── 状态机
└── EtherCAT 周期帧
        ↓
EtherCAT 与 CAN 之间的帧转换   # 送
        ↓
CAN 帧
        ↓
电机驱动器与电机               # no control logic
```

在这套方案中：

- 上位机负责计算和控制逻辑。
- EtherCAT 负责周期性传输数据。
- 下位机主要负责通信帧转换和必要的安全兜底。
- 控制逻辑没有分散在两块板子上。

```text-photo
相机节点
  ↓ 通过 /image_raw 话题发布图片
识别节点
  ↓ 通过 /track 话题发布目标位置
解算节点
  ↓ 发布云台或底盘控制目标
ros_control
  ├─ Controller：计算控制量
  ├─ Controller Manager：管理控制器
  └─ RobotHW：读写硬件
        ↓
EtherCAT / CAN
        ↓
电机执行
```

### 4.2 方案特点

#### 修改方便

控制代码、状态机和参数都在上位机 C++ 节点中。更新程序通常要重新编译和启动节点，不需要为每次控制逻辑修改都烧录下位机固件。(集中)

#### 计算解耦与通信分离

上位机负责“算什么”，通信链路负责“怎么送到”。分工清楚后，通信层可以保持简单，控制算法也更容易单独测试。EtherCAT保证每一帧按时到---实时性

#### 另一种做法：把闭环放到小板上

把闭环控制放在下位机可以提高实时性，但也会增加系统分布：

- 控制逻辑分散在上位机和下位机。
- 修改控制算法时可能要重新烧录固件。
- 出现问题时需要同时检查两端程序与通信协议。

选择方案时，应结合实时性、硬件能力、通信延迟和维护复杂度评估。
还要保证稳定性及控制周期是否满足要求
---

## 5. ROS 的三层架构

### 5.1 文件系统层（File System）

解决“代码怎么组织”的问题。（管文件怎么放/组织）

包括：

- 工作空间。
- 功能包。
- 源码。
- 配置文件。
- 消息和服务定义。
- 启动文件。

常见文件：

- `package.xml`
- `CMakeLists.txt`
- `msg/`
- `srv/`
- `launch/`
- `config/`

### 5.2 计算图层（Computation Graph）

解决“程序怎么协作”的问题。

包括：

- 节点。node
- 话题。topic
- 服务。service
- 参数。para
- 消息。message

### 5.3 通信层（Communication）

解决“数据怎么传输”的问题。

涉及：

- 节点之间的连接建立。
- 消息序列化与反序列化。
- 发送和接收队列。
- TCPROS 或 UDPROS 等传输方式。
- 网络断开和重新连接。

### 5.4 按层次排查问题

| 现象 | 优先检查 |
|---|---|
| 找不到包 | 工作空间环境是否 `source`、包是否位于 `src` |
| 节点连不上或收不到数据 | 节点名称、话题名称、消息类型、订阅关系是否正确 |
| 数据延迟或丢失 | 队列长度(缓冲的信息数量)、回调处理速度、网络带宽（单位时间内传输多少数据）、系统负载 |
| 节点一启动就退出 | 终端的首个错误和日志 |

---

## 6. ROS 程序的五个基本零件

### 6.1 节点（Node）

节点是执行具体工作的程序单元，通常作为独立进程运行。

类比：一个负责特定工作的工人/APP

常用命令：

```bash
rosnode list
rosnode info /node_name
```

### 6.2 话题（Topic）

话题是具有名称的通信通道。发布者向话题写入数据，订阅者从话题接收数据。

类比：广播电台。

常用命令：

```bash
rostopic list
rostopic echo /topic_name
```

### 6.3 消息（Message）

消息定义话题上传输的数据结构。消息格式通常在 `.msg` 文件中定义。

类比：信封的格式，规定了内容由哪些字段组成。

常用命令：

```bash
rosmsg show <pkg/MessageType>
```

### 6.4 服务（Service）

服务提供一次请求和一次响应，客户端发起请求，服务端处理并返回结果。

类比：一次电话问答。

常用命令：

```bash
rosservice call /service_name
```

### 6.5 参数（Parameter）

参数是按名称保存的配置值，多个节点可以读取。

类比：所有人都能查看和修改的配置表。

常用命令：

```bash
rosparam get /param_name
rosparam set /param_name value
```

---

## 7. ROS Master

### 7.1 Master 管理什么

ROS 1 的 Master 主要负责：

1. **名称注册**：记录哪些节点发布了哪些话题、提供了哪些服务。
2. **名称查询**：帮助节点查找其他节点的位置。
3. **参数服务器**：保存全局参数。
4. **节点状态相关信息**：帮助节点发现系统中的其他参与者。（谁还活着谁掉线了）

Master 默认通过 11311 端口提供 XML-RPC 服务。

### 7.2 普通话题数据是否经过 Master

**普通话题的实际数据不经过 Master。**

Master 帮节点互相发现，随后发布者和订阅者直接建立数据连接。Master 不负责逐条转发话题消息。

### 7.3 一次话题通信的完整流程

1. **启动 Master**：运行 `roscore`，Master 开始等待节点注册。
2. **发布者注册**：发布节点向 Master 声明自己发布的话题及消息类型，例如 `/cmd_vel` 和 `geometry_msgs/Twist`。
3. **订阅者注册**：订阅节点向 Master 声明自己需要订阅的话题。
4. **Master 返回发布者信息**：Master 告诉订阅者发布者的地址。
5. **订阅者建立连接**：订阅者主动连接发布者，双方确认通信信息。
6. **直接传递消息**：后续消息通过节点之间的连接传输，不由 Master 转发。

第 4 步给订阅者的是发布者地址等连接信息，不是话题数据本身。

```relative graph
节点 A：我发布 /cmd_vel
             ↓ 注册
          ROS Master
             ↑ 查询
节点 B：我想订阅 /cmd_vel

Master 把 A 的地址告诉 B
              ↓
A 与 B 建立直接连接（TCP）
              ↓
后续消息直接在 A、B 之间传递
```

### 7.4 Master 挂掉时会怎样

- 已经建立的节点间连接可能继续运行。
- 新节点注册或发现发布者可能失败。
- 参数服务器及其他依赖 Master 的操作也会受影响。

所以，Master 暂时不可用时，不能据此认为整个 ROS 系统完全不受影响。

### 7.5 `roscore` 启动了什么

课件概括为：

```text
roscore = Master + rosout（日志）+ 参数服务器
```

一般启动 ROS 1 系统时，先运行：

```bash
roscore
```

---

## 8. ros_control：控制器与硬件分离

### 8.1 `ros_control` 的作用

`ros_control` 将控制器逻辑与具体硬件接口分开，便于复用控制器和替换底层硬件。--**解耦**

它仍然使用 ROS 的话题、服务等机制，没有发明新的 ROS 通信方式。

### 8.2 主要组件

#### Controller

控制器负责根据输入状态和目标计算控制指令。课件列举：

- PID 控制器。
- 轨迹控制器。
- 差速控制器。
- 关节状态控制器。

常见控制器类型或名称包括：

- `joint_state_controller`
- `joint_trajectory_controller`         # 算
- `effort_controllers`
- `diff_drive_controller`

具体可用名称取决于实际安装的 ROS 包。

#### RobotHW / Hardware Interface---硬件接口

硬件接口把具体硬件的读写方式封装起来，向上提供统一接口。

通常需要自行实现：

- `read()`：读取编码器、传感器或其他硬件状态,并回收，传给控制做闭环。
- `write()`：把计算出的控制指令打包成EtherCAT/CAN帧发送到电机或下位机。

以 EtherCAT 或 CAN 收发为例。

```example
ROS 节点之间
话题 / 服务
TCPROS / UDPROS 等
        ↓
RobotHW 或硬件桥接程序            # 搬
        ↓
上位机与底层硬件之间
EtherCAT / CAN / 串口等
        ↓
电机、传感器
```
#### Controller Manager

控制器管理器负责：

- 加载控制器。
- 启动或停止控制器。
- 在运行时切换控制器。
- 管理控制器的生命周期。

课件提到可以使用 `spawner` 启动控制器。

#### 关节限制与传动配置

系统还需要描述：

- 位置、速度和加速度限制。
- 关节到电机之间的传动关系。
- 传动比以及关节与电机之间的映射。

### 8.3 控制循环

常见控制循环可以概括为：

```text
read() → 更新控制器 → write()
```

1. 硬件接口读取当前状态。
2. 控制器根据目标和状态计算输出。
3. 硬件接口把输出发送给执行器。

### 8.4 ros_control 的好处

- 更换电机或通信链路时，通常集中修改硬件接口。
- 更换控制算法时，通常集中修改控制器。
- 同一套控制器逻辑可用于仿真和实车，具体取决于接口与配置。

---

## 9. 工作空间（Workspace）

### 9.1 工作空间结构

使用 `catkin_ws` (Ros代码根目录)作为示例：

```text
catkin_ws/
├── src/                    # 源码与功能包
│   ├── CMakeLists.txt      # 通常链接到 catkin 的顶层文件
│   └── my_pkg/             # 一个功能包
├── build/                  # 编译中间文件
└── devel/                  # 开发空间的编译产物
    └── setup.bash          # 环境设置脚本
```

```bash
catkin_ws 工作空间
└── src
    ├── camera_pkg 功能包
    │   └── 可以编译出相机节点
    ├── detect_pkg 功能包
    │   └── 可以编译出识别节点
    └── control_pkg 功能包
        └── 可以编译出控制相关程序
```

### 9.2 目录用途

- `src/`：存放源码与功能包，一般提交到版本库。
- `build/`：存放编译中间文件，通常可删除并重新生成。
- `devel/`：存放开发环境中的可执行文件、环境脚本等。
- `install/`：使用 install 空间构建时可能生成，用于安装后的文件。

- 关系图
```bash
src 中的源代码
     ↓ 编译
build 中生成中间文件
     ↓
devel 中生成可执行程序和环境配置
```

### 9.3 编译和加载环境

在工作空间根目录编译：

```bash
catkin_make
```

然后加载开发环境：

```bash
source devel/setup.bash
```

也可以将 `source` 命令写入 shell 启动文件，减少每次开终端时手动输入的步骤。

### 9.4 工作空间部署

课件给出的部署思路：

```text
在上位机获取源码
→ 编译
→ 加载工作空间环境
→ 运行节点
```

例如通过 SSH 在没有图形界面的上位机中编译和启动 ROS 程序。

### 9.5 包名

提醒，一个工作空间中包名应保持唯一，避免工具按包名查找时产生歧义。包名应符合 ROS 包命名规则，通常使用小写英文字母、数字和下划线。

---

## 10. ROS 功能包（Package）

### 10.1 功能包的概念

功能包是 ROS 中常用的代码组织和**复用**单位。一个包可以包含程序、消息定义、配置文件和启动文件。

### 10.2 常见目录与文件

```text
my_pkg/
├── CMakeLists.txt       # 编译规则
├── package.xml          # 包信息与依赖
├── include/my_pkg/      # 对外提供的头文件
├── src/                 # C++ 源文件
├── scripts/             # 可执行脚本
├── msg/                 # 自定义消息定义
├── srv/                 # 自定义服务定义
├── launch/              # launch 启动文件
├── config/              # YAML 配置文件
└── README.md            # 给开发者阅读的说明
```

这些目录按项目需要创建，并不是每个包都必须含有全部目录。

### 10.3 `include/` 与 `src/`

- `include/my_pkg/`：通常放需要供其他源码包含的头文件。
- `src/`：通常放 C++ 实现文件。

简单程序可以只有 `src/`。程序规模扩大或需要复用代码时，再清楚地区分头文件和实现文件。

### 10.4 消息和服务目录

- 自定义消息放在 `msg/`。
- 自定义服务放在 `srv/`。
- 目录名要使用 `srv/`，而不是 `service/`。
- 仅仅创建目录和 `.msg` 或 `.srv` 文件还不够，还要配置构建文件生成相应代码。

### 10.5 launch 和 config

- `launch/` 通常放 `.launch` 文件，用于统一启动节点和加载配置。
- `config/` 通常放 `.yaml` 文件，用于保存参数。
- 它们不一定参与 C++ 编译，但会影响程序如何启动和运行。

---

## 11. `package.xml`：包的身份和依赖---身份名片

用途：包叫什么，谁维护的，依赖哪些包，用什么许可证

### 11.1 示例

```xml
<?xml version="1.0"?>
<package format="2">
  <name>my_pkg</name>
  <version>0.1.0</version>
  <description>我的第一个 ROS 包</description>

  <maintainer email="you@example.com">Your Name</maintainer>
  <license>MIT</license>

  <buildtool_depend>catkin</buildtool_depend>
  <build_depend>std_msgs</build_depend>
  <exec_depend>message_runtime</exec_depend>
  <depend>roscpp</depend>
</package>
```

### 11.2 常见字段

- `<name>`：包名，**应与包目录名称一致**。
- `<version>`：包版本。
- `<description>`：包用途说明。
- `<maintainer>`：维护者信息。
- `<license>`：许可证。
- `<buildtool_depend>`：构建工具依赖，例如 `catkin`。
- `<build_depend>`：编译时需要的依赖。
- `<exec_depend>`：运行时需要的依赖。
- `<depend>`：同时声明构建(编译)、导出和运行依赖，= build + build_export + exec 

### 11.3 自定义消息与服务依赖

自定义 `.msg` 或 `.srv` 时，常见依赖包括：

- `message_generation`：构建时生成消息或服务代码。
- `message_runtime`：运行时提供消息或服务支持。

### 11.4 常见问题

- 包名和文件夹名称不一致。
- 依赖只写在 `CMakeLists.txt` 或 `package.xml` 的一处。
- 编译依赖与运行依赖混淆。
- 修改包结构后没有重新构建。
- 包名使用空格、中文或不符合命名规则的字符。

---

## 12. `CMakeLists.txt`：编译规则

### 12.1 常见结构

```cmake
cmake_minimum_required(VERSION 3.0.2)
project(my_pkg)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
)

add_message_files(
  FILES
  MyMsg.msg
)

add_service_files(
  FILES
  MySrv.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS
  roscpp
  std_msgs
  message_runtime
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(talker src/talker.cpp)
target_link_libraries(talker ${catkin_LIBRARIES})

add_dependencies(talker ${${PROJECT_NAME}_EXPORTED_TARGETS})
```

### 12.2 常见步骤

1. **项目声明**：指定最低 CMake 版本和项目名称。
2. **查找依赖**：使用 `find_package(catkin ... COMPONENTS ...)`。
3. **生成消息和服务**：只有定义了自定义 `.msg` 或 `.srv` 才需要配置。
4. **声明导出的包信息**：使用 `catkin_package()`。
5. **添加头文件路径**：让编译器找到依赖和项目头文件。
6. **定义可执行目标**：通过 `add_executable()` 声明要生成的程序。
7. **链接依赖库**：通过 `target_link_libraries()` 指定要链接的库。
8. **声明消息生成依赖**：在依赖自定义消息的目标中正确设置构建依赖。

### 12.3 常见编译错误

- `package.xml` 与 `CMakeLists.txt` 声明的依赖不一致。
- 自定义消息或服务没有加入 CMake 配置。
- 消息修改后没有重新构建。
- 目标依赖生成消息的步骤未配置，造成找不到生成的消息头文件。
- 新增源文件后忘记把它加入目标。

---

## 13. 三种主要通信方式

### 13.1 选择方式

| 需求 | 适合的机制 |
|---|---|
| 连续、异步传输数据 | 话题 |
| 发起一次请求并获取明确结果 | 服务 |
| 保存共享配置 | 参数服务器 |
| 执行较长任务，需要反馈、取消或查询进度 | Action |

课件主要讲前三种；Action 和其他机制留作后续学习。

---

## 14. 话题通信（Topic）---广播电台

### 14.1 话题的组成

- **Publisher（发布者）**：向话题发布数据。
- **Subscriber（订阅者）**：订阅话题并接收数据。
- **Topic（话题）**：发布者和订阅者共同使用的名称。
- **Message（消息）**：话题中数据的结构和类型。

例如：

```text
话题：/turtle1/cmd_vel
类型：geometry_msgs/Twist
发布者：键盘控制节点
订阅者：小乌龟节点
```

### 14.2 话题的特点

#### 异步

发布者发布消息后通常立即返回，不必等待订阅者处理完。

#### 多对多

一个话题可以有多个发布者和多个订阅者。具体的数据组合和处理语义由系统设计决定。

#### 队列

发布者和订阅者可以设置队列长度。若消息生产速度高于消费速度，队列可能满，部分消息可能被丢弃。

#### 连接与重连

订阅者启动时可以发现发布者并连接。连接中断时，ROS 通常会尝试重新建立连接。

默认情况下，新订阅者接收连接建立后的新消息。若发布者使用 `latch`，新订阅者可以收到发布者保留的最后一条消息。

### 14.3 C++ 发布者

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
    rate.sleep();
  }

  return 0;
}
```

说明：

- `ros::init()` 初始化 ROS 节点。
- `NodeHandle` 用于创建发布者、订阅者和服务等对象。
- `advertise()` 声明发布话题、消息类型和队列长度。
- `publish()` 发送消息。
- `ros::Rate` 用于控制循环频率，这里是 1 Hz。

### 14.4 C++ 订阅者

```cpp
#include <ros/ros.h>
#include <std_msgs/String.h>

void callback(const std_msgs::String::ConstPtr& msg) {
  ROS_INFO("I heard: %s", msg->data.c_str());
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "listener");
  ros::NodeHandle nh;

  ros::Subscriber sub =
      nh.subscribe("/chatter", 10, callback);

  ros::spin();
  return 0;
}
```

说明：

- `subscribe()` 创建订阅者并注册回调函数。
- 收到消息时，ROS 调用回调处理数据。
- `ros::spin()` 等待并处理回调。

### 14.5 回调函数注意事项

在单线程 `ros::spin()` 的情况下，如果回调函数长时间不返回，后续回调可能无法及时执行。因此：

- 回调中避免耗时计算。
- 大量计算可以交给其他线程或节点处理。
- 需要并行回调时，了解并使用适当的 spinner 和线程安全机制。

### 14.6 话题命令

```bash
rostopic list
```

列出当前可见的话题。

```bash
rostopic echo /topic_name
```

显示话题内容。

```bash
rostopic info /topic_name
```

查看话题类型、发布者和订阅者。

```bash
rostopic hz /topic_name
```

估算话题发布频率。

```bash
rostopic pub -r 10 /topic_name <message_type> '<message>'
```

按指定频率发布消息。实际使用时应填写正确的消息类型和消息字段。

---

## 15. 服务通信（Service）---打电话

### 15.1 服务的组成

- **Server（服务端）**：提供服务并处理请求。
- **Client（客户端）**：主动发起请求。
- **Request（请求）**：客户端发送的数据。
- **Response（响应）**：服务端返回的结果。
- **`.srv` 文件**：定义请求和响应的数据字段。

### 15.2 `.srv` 文件格式

上下两部分用 `---` 分隔：

```srv
# 请求部分---告诉
float32 x
float32 y
float32 theta
string name
---
# 响应部分---回答
string name
```

分隔线以上是请求字段，以下是响应字段。

### 15.3 服务适用场景

例如：

- 切换工作模式。
- 查询或生成某项结果。
- 清空轨迹。
- 生成一只新的小乌龟。
- 执行不频繁、具有明确完成结果的操作。

### 15.4 服务的特性与注意事项

- 普通服务调用是同步的，客户端会等待结果。
- 服务端没有启动时，客户端无法完成调用。
- 可使用 `waitForService()` 等待服务可用。
- 服务调用失败时要检查返回值。
- 对等待时间敏感的系统应设计**超时或异步处理**。
- 长时间任务、有反馈或取消需求时，考虑使用 Action。
- 高频调用是否合适，需要根据调用开销和实时性要求判断。

### 15.5 C++ 服务客户端

```cpp
ros::ServiceClient client =
    nh.serviceClient<turtlesim::Spawn>("/spawn");

client.waitForExistence();

turtlesim::Spawn srv;
srv.request.x = 2.0;
srv.request.y = 3.0;
srv.request.theta = 0.0;
srv.request.name = "t2";

if (client.call(srv)) {
  ROS_INFO(
      "new turtle: %s",
      srv.response.name.c_str()
  );
}
```

### 15.6 命令行服务命令

列出服务：

```bash
rosservice list
```

查询服务类型：

```bash
rosservice type /spawn
```

调用服务：

```bash
rosservice call /spawn "x: 2.0 y: 3.0 theta: 0.0 name: 't2'"
```

查看服务字段：

```bash
rossrv show turtlesim/Spawn
```

### 15.7 自定义服务的构建配置

在 `CMakeLists.txt` 中加入服务文件和消息生成配置：

```cmake
add_service_files(
  FILES                       ## 申明服务
  Spawn.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)
```

自定义服务还需要在 `package.xml` 和 `CMakeLists.txt` 中声明对应的生成与运行依赖。

---

## 16. 参数服务器（Parameter Server）

### 16.1 参数服务器是什么

参数服务器为 ROS 节点保存共享的 key-value 配置。

参数名称具有层级结构，例如：

```text
/robot/camera/exposure
/robot/gimbal/pid/p
```

参数常用于：

- 相机内参和曝光值。
- 识别阈值。
- PID 参数。
- 串口波特率。
- 运行模式和功能开关。

### 16.2 参数的数据类型

课件列举的类型包括：  ---是**键值对**

- 整数。
- 浮点数。
- 布尔值。
- 字符串。
- 列表。
- 字典。

读写时要注意值的类型：

```text
30       # 整数
"30"     # 字符串
3.0      # 浮点数
true     # 布尔值
```

### 16.3 参数的生命周期

参数服务器随 ROS Master 运行。Master 或 `roscore` 停止后，参数服务器中的运行时数据也随之消失。

因此，重要参数应保存到 YAML 等文件，方便下次启动时重新加载。

### 16.4 参数修改与节点行为

参数服务器保存的是共享配置值。某个节点是否会自动采用新值，取决于该节点的实现：

- 节点只在启动时读取一次，修改参数后节点可能仍使用旧值。
- 节点定期重新读取，或实现了动态参数更新，修改后才可能影响运行状态。
- ROS 1 的参数更新不会自动替所有节点执行回调逻辑。

因此，修改参数后要确认节点是否重新读取。

### 16.5 参数 API

常见 C++ 参数接口：

```cpp
nh.getParam("camera/exposure", exposure);
```

读取参数。

```cpp
nh.param<int>("camera/exposure", exposure, 30);
```

读取参数；若参数不存在则使用默认值 `30`。

```cpp
nh.setParam("camera/exposure", 40);
```

写入参数。

```cpp
nh.hasParam("camera/exposure");
```

检查参数是否存在。

```cpp
nh.deleteParam("camera/exposure");
```

删除参数。

对于较频繁的读取，课件提到可以考虑 `get_param_cached`。是否使用缓存要结合参数变更方式和性能需求。

### 16.6 参数命名空间

#### 绝对名称

```text
/robot/speed
```

从根命名空间开始，与节点当前命名空间无关。

#### 相对名称

```text
speed
```

相对于当前命名空间解析，launch 中设置命名空间后，解析结果也会变化。

#### 私有名称

```text
~speed
```

属于当前节点的私有命名空间，常用于只由一个节点使用的参数。

### 16.7 参数服务器命令

```bash
rosparam list
```

列出参数。

```bash
rosparam get /cam/exposure
```

读取参数。

```bash
rosparam set /cam/exposure 30
```

设置参数。

```bash
rosparam dump my_params.yaml /cam
```

将指定命名空间下的参数导出到 YAML 文件。

```bash
rosparam load my_params.yaml /cam
```

将 YAML 参数加载到指定命名空间。

```bash
rosparam delete /cam
```

删除指定参数或命名空间下的一组参数。

示例：

```bash
rosparam list | grep cam
```

筛选名称中包含 `cam` 的参数。

### 16.8 参数加载中的命名空间问题

参数文件中的层级和加载时使用的命名空间会共同决定最终参数路径。

假设 YAML 文件中已经有：

```yaml
cam:
  exposure: 30
```

加载时又指定 `/cam`，最终路径可能包含两层 `cam`。因此加载前后应检查参数的实际完整路径：

```bash
rosparam list
rosparam get /cam/cam/exposure
```

---

## 17. YAML 配置文件

### 17.1 YAML 的作用

YAML 是一种适合人阅读和编辑的数据格式。ROS 常用 YAML 文件批量配置参数，再通过命令行或 launch 加载到参数服务器。

### 17.2 YAML 的基本语法

#### 缩进表达层级

```yaml
camera:
  width: 640
  height: 480
```

`width` 和 `height` 属于 `camera`。

#### 只使用空格缩进

- 不要使用 Tab。
- 同一层级的缩进要保持一致。
- 常见习惯是每一级缩进两个空格。

#### 键值之间的冒号

常见写法：

```yaml
width: 640
```

冒号后通常留一个空格，格式更清楚，也能避免解析歧义。

#### 注释

```yaml
# 整行注释
width: 640  # 行尾注释
```

如果字符串本身包含容易被当作注释的字符，可用引号明确标记字符串。

#### 大小写和布尔值

YAML 对大小写敏感。为了可读性和跨工具一致性，建议使用：

```yaml
debug: true
enabled: false
```

### 17.3 三种基本数据结构

#### 标量（Scalar）

单个值：

```yaml
speed: 1.5
name: "sentry"
debug: true
empty: null
```

#### 列表（List）

```yaml
ports:
  - 8080
  - 8081
```

也可以写成行内形式：

```yaml
ids: [1, 2, 3]
```

#### 字典（Dictionary）

```yaml
camera:
  width: 640
  height: 480
```

也可以使用行内形式：

```yaml
point: {x: 1, y: 2}
```

### 17.4 复杂结构示例

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
    - name: "hero"
      id: 1
    - name: "sentry"
      id: 7
```

阅读时注意：

- 缩进决定层级。
- 冒号左侧是键名。
- 冒号右侧是值。
- `-` 表示列表中的一项。

### 17.5 YAML 常见问题

- 使用了 Tab 缩进。
- 同一层级缩进没有对齐。
- 冒号和空格写法不清楚。
- 字符串、数字和布尔值的类型不符合预期。
- 把注释符号误写到字符串内容中。
- YAML 文件层级与加载命名空间重复。
- YAML 语法正确，但节点读取的参数路径不匹配。

### 17.6 验证 YAML

可以通过 ROS 命令尝试加载并查看参数：

```bash
rosparam load params.yaml /test
rosparam get /test
```

除了确认文件能否加载，也要检查参数名称和数据类型是否符合节点代码的预期。

### 17.7 YAML 在 ROS 中的三种使用方式

#### 命令行加载

适合临时调试：

```bash
rosparam load config/params.yaml /my_ns
```

#### launch 文件加载

适合随节点一起启动：

```xml
<launch>
  <rosparam
    file="$(find my_pkg)/config/params.yaml"
    command="load"
    ns="my_ns"/>
</launch>
```

#### C++ 节点读取

推荐提供默认值：

```cpp
ros::NodeHandle pnh("~");

int exposure;
pnh.param<int>(
    "camera/exposure",
    exposure,
    30
);
```

---

## 18. 小乌龟：一个最小 ROS 系统

### 18.1 启动系统

打开三个终端。

终端 1：启动 ROS Master 和相关基础服务。

```bash
roscore
```

终端 2：启动小乌龟仿真节点。

```bash
rosrun turtlesim turtlesim_node
```

终端 3：启动键盘控制节点。

```bash
rosrun turtlesim turtle_teleop_key
```

在键盘控制终端中按方向键，可以控制小乌龟移动。

### 18.2 小乌龟中的话题

#### 控制指令

```text
/turtle1/cmd_vel
```

类型：

```text
geometry_msgs/Twist
```

典型关系：

```text
键盘控制节点发布
→ 小乌龟节点订阅
```

#### 位姿数据

```text
/turtle1/pose
```

小乌龟节点发布自己的位置和姿态，其他节点可以订阅。

### 18.3 小乌龟中的服务

课件列出的服务包括：

- `/spawn`：生成乌龟。
- `/kill`：删除乌龟。
- `/clear`：清除轨迹。
- `/turtle1/set_pen`：设置画笔。

### 18.4 小乌龟中的参数

参数示例包括：

```text
/turtlesim/background_r
/turtlesim/background_g
/turtlesim/background_b
```

可修改背景颜色相关参数，再调用 `/clear` 让界面刷新：

```bash
rosparam set /turtlesim/background_r 255
rosservice call /clear
```

### 18.5 查看节点关系

```bash
rqt_graph
```

它可以显示当前 ROS 计算图中的节点及其话题连接关系。

---

## 19. rosnode：查看和管理节点

### 19.1 列出节点

```bash
rosnode list
```

课件示例输出：

```text
/turtle_teleop_key
/turtlesim
```

### 19.2 查看节点详情

```bash
rosnode info /turtlesim
```

通常可以查看：

- Publications：该节点发布的话题。
- Subscriptions：该节点订阅的话题。
- Services：该节点提供的服务。
- 节点连接及其他状态信息。

收不到消息时，可以先用 `rosnode info` 检查节点实际上是否发布或订阅了对应接口。

### 19.3 检查节点和停止节点

```bash
rosnode ping /turtlesim
```

检查节点接口是否响应。

```bash
rosnode kill /turtlesim
```

通过 ROS 机制请求关闭节点。

课件建议排查时先执行 `rosnode list`，再执行 `rosnode info`。`list` 只能看到节点名称，`info` 才能看到节点实际连接的接口。

### 19.4 三个常见观察角度

- `rqt_graph`：看整体节点连接。
- `rosnode info`：看一个节点发布和订阅了什么。
- `rostopic echo`：看话题中实际传输了什么。

---

## 20. 常用 ROS 命令速查

### 20.1 包与工作空间

```bash
rospack find <pkg>                         
roscd <pkg>                                
rosls <pkg>    
catkin_create_pkg <name> <dependencies>    
catkin build
```

用途：

- `rospack find`：查找包路径。
- `roscd`：切换到指定包目录。
- `rosls`：列出包目录内容。
- `catkin_create_pkg`：创建新包并声明依赖。

### 20.2 编译

```bash
catkin_make
```

或者在安装了 catkin_tools 的工程中使用：

```bash
catkin build
```

编译后加载环境：

```bash
source devel/setup.bash
```

同一个工程应按项目选择的构建方式进行构建。

### 20.3 启动

```bash
roscore
rosrun <pkg> <node>
roslaunch <pkg> <file.launch>
```
- `roscore`:启动核心
- `rosrun`：启动一个包中的节点。
- `roslaunch`：根据 launch 文件启动多个节点并加载配置。

### 20.4 节点

```bash
rosnode list        ---查看节点
rosnode info <node>
rosnode ping <node>
rosnode kill <node>
```

### 20.5 话题

```bash
rostopic list          ---列出所有话题
rostopic echo <topic>  ---打印话题内容
rostopic info <topic>  ---看话题类型
rostopic hz <topic>    ---测发布频率
rostopic pub -r 10 <topic> <type> '<message>'  ---手动发布一条话题
```

### 20.6 服务与消息

```bash
rosservice list                          ---引出所有服务
rosservice type <service>
rosservice call <service> '<request>'
rosservice find <service_type>

rosmsg show <pkg/MessageType>            ---查看消息结构
rossrv show <pkg/ServiceType>            ---查看服务结构
```

### 20.7 参数

```bash
rosparam list                      ---列出所有参数
rosparam get <name>   
rosparam set <name> <value>
rosparam load <file> [namespace]   ---从YML加载,导出成YAML
rosparam dump <file> [namespace]
rosparam delete <namespace>        ---删除参数,命名空间
```

### 20.8 调试与数据记录

```bash
rviz                 ---3D可视化,看点云,TF图像
rqt_graph
rosbag record -a     ---录包
rosbag info <bag>    ---回放录包
rosbag play <bag>
```

---

## 21. 从零运行一个 ROS 工程的基本流程

### 21.1 创建工作空间和功能包

```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src

catkin_create_pkg my_pkg roscpp std_msgs
```

### 21.2 编写源码和构建配置

- 在 `src/` 中写 C++ 源文件。
- 检查 `package.xml` 的依赖。
- 检查 `CMakeLists.txt` 的目标和链接配置。
- 需要自定义消息或服务时，添加对应定义和生成配置。

### 21.3 编译并加载工作空间

```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

### 21.4 启动 ROS 系统

一个典型流程：

1. 运行 `roscore`。
2. 运行需要的节点，或使用 `roslaunch` 一次启动。
3. 用 `rosnode list` 确认节点已运行。
4. 用 `rostopic list` 和 `rosnode info` 检查接口。
5. 用 `rostopic echo` 检查实际数据。
6. 需要时用 `rqt_graph`、RViz 或 rosbag 辅助排查。

---

## 22. 易混淆概念汇总

### 22.1 Master 与话题数据

- Master 帮节点发现彼此。
- 话题数据通常由节点之间直接传输。
- Master 不负责转发每一条普通话题消息。

### 22.2 话题与服务

| 比较项 | 话题 | 服务 |
|---|---|---|
| 通信形式 | 发布与订阅 | 请求与响应 |
| 是否异步 | 通常异步 | 普通调用通常阻塞等待 |
| 典型数据 | 连续数据流 | 一次操作或查询 |
| 例子 | 相机图像、速度命令 | 生成乌龟、清空轨迹 |

### 22.3 参数与话题

- 参数保存配置值。
- 话题传递运行时数据流。
- 修改参数服务器中的值，不保证节点会自动重新读取或更新自身状态。

### 22.4 文件系统与计算图

- 文件系统层描述包、源码和配置文件如何组织。
- 计算图层描述运行时的节点如何交互。
- 一个功能包可以包含多个节点；包和节点不是同一个概念。

### 22.5 `.msg` 与 `.srv`

- `.msg` 定义话题消息的数据结构。
- `.srv` 定义请求和响应的数据结构。
- 两者都需要在构建配置中声明并生成对应代码。

---

## 23. 课件内容回顾

1. **ROS 是运行在 Linux 上的机器人中间件与工具生态。**
2. **机器人程序通常由多个节点组成，节点通过话题、服务和参数协作。**
3. **Master 帮节点发现彼此，普通话题数据由节点之间直接传输。**
4. **ROS 架构可分为文件系统层、计算图层和通信层。**
5. **工作空间组织多个包，功能包通过 `package.xml` 和 `CMakeLists.txt` 描述依赖与编译方式。**
6. **话题适合连续数据流，服务适合一次请求与响应，参数适合保存配置。**
7. **YAML 用缩进表达层级，常用于批量加载 ROS 参数。**
8. **ros_control 通过硬件接口和控制器分离硬件读写与控制逻辑。**
9. **小乌龟程序展示了节点、话题、服务和参数如何组合成一个 ROS 系统。**
10. **排查问题时，先确认环境、节点、接口名称和消息类型，再检查数据、参数、日志与通信情况。**

---
- 补充ros关系框架

```bash
Ubuntu：学校所在的土地和房子
└── ROS 1：机器人学校
    ├── 文件系统：课本、书包、教室怎么安排
    │   └── catkin 工作空间
    │       └── 功能包 package
    │           ├── 源代码
    │           ├── 消息与服务定义
    │           ├── launch 启动文件
    │           └── YAML 配置文件
    │
    ├── 计算图：同学们怎样分工合作
    │   ├── 节点 Node
    │   ├── 话题 Topic
    │   ├── 消息 Message
    │   ├── 服务 Service
    │   └── 参数 Parameter
    │
    ├── 通信机制：消息实际怎样送达
    │   ├── Master：帮助双方找到彼此
    │   └── TCPROS / UDPROS：节点间传数据
    │
    ├── 控制框架：专门管机器人怎么动
    │   └── ros_control
    │       ├── Controller
    │       ├── Controller Manager
    │       └── RobotHW
    │
    └── 工具与生态：观察、调试、录制、仿真
        ├── rosnode / rostopic / rosservice
        ├── rosbag / RViz / rqt
        └── Gazebo / OpenCV / PCL 等
```


## 24. 课后复习清单

- [ ] 能用自己的话说明 ROS 是什么。
- [ ] 能解释节点、话题、消息、服务和参数服务器。
- [ ] 能描述发布者与订阅者建立一次话题通信的过程。
- [ ] 能说明 ROS Master 管什么、不负责什么。
- [ ] 能区分文件系统层、计算图层和通信层。
- [ ] 能看懂 `catkin_ws` 的基本目录。
- [ ] 能解释 `package.xml` 和 `CMakeLists.txt` 的作用。
- [ ] 能编写简单的 Publisher 和 Subscriber。
- [ ] 能定义并调用一个 `.srv` 服务。
- [ ] 能设置、读取和加载参数。
- [ ] 能写出含字典、列表和标量的 YAML 配置。
- [ ] 能启动小乌龟并使用 `rqt_graph` 查看节点关系。
- [ ] 能使用 `rosnode`、`rostopic`、`rosservice` 和 `rosparam` 排查问题。
- [ ] 能说明 ros_control 中 `read()`、控制器更新和 `write()` 的关系。

## 25. 后续学习方向

课件建议继续学习：
>https://www.autolabor.com.cn/book/ROSTutorials/
- TF 坐标变换。
- 控制器的实现。
- PID 参数调节。
- 底盘运动学解算。
- `ros_control` 的使用与硬件接口实现。
- 课件附带的 ROS 课程仓库和教程资料。

