# ROS 架构与通信学习笔记

- 学习日期：2026 年 11 月 3 日
- 学习方向：控制组
- GitHub 用户名：wskssg31
- 学习方式：通过交互问答理解概念，在 Docker 中完成 ROS Noetic 实验，再整理自己的理解和遇到的问题。

## 一、ROS 是什么

ROS（Robot Operating System）提供机器人开发需要的通信机制、工具和软件包生态。我目前把它理解为运行在 Linux 上的机器人中间件和工具箱。

机器人中的相机、识别、决策和控制程序可以分开开发，通过约定好的接口交换数据。这样便于团队分工、调试和替换模块。

从不同角度理解 ROS：

| 层次 | 关注的问题 | 例子 |
| --- | --- | --- |
| 文件系统层 | 代码和配置怎样组织 | 工作空间、功能包、package.xml、CMakeLists.txt |
| 计算图层 | 运行中的程序怎样协作 | 节点、话题、消息、服务、参数 |
| 通信层 | 节点之间怎样连接和传输数据 | ROS 1 常用的 TCPROS |

本次重点使用 ROS 1 Noetic，并用 C++ 编写节点。

## 二、开发环境与 Docker

### 1. 本次使用的环境

| 项目 | 实际环境 |
| --- | --- |
| 宿主机 | Ubuntu 26.04.1 |
| Docker | 29.1.3 |
| ROS | Noetic，实验输出中的 ros_comm 版本为 1.15.14 |
| 镜像 | fishros2/ros:noetic-desktop-full |
| 容器名 | ros-noetic |
| 容器网络 | host |
| 工作空间 | /home/wangsikai/ros_learning_ws |

Noetic 对应的官方 Ubuntu 平台是 20.04。按照老师推荐，我使用 Docker 准备匹配的开发环境。

### 2. 我对 Docker 的理解

- Docker：创建和管理容器的工具。
- 镜像：打包好的软件环境模板，包含程序、库和配置等。
- 容器：根据镜像创建、实际运行程序的环境。
- 在 Linux 上，容器拥有自己的软件环境，并共享宿主机内核。

本次使用 host 网络模式，让容器共享宿主机网络。安装工具还将宿主机的个人目录映射进了容器，方便保存练习文件。

进入已有容器：

```bash
sudo docker exec -it ros-noetic bash
```

本次的 ROS 编译、运行命令都在容器内执行。新开宿主机终端后，需要先进入容器。

## 三、节点、话题和消息

### 1. 节点（Node）

节点是运行中负责某项任务的程序，入门时可以理解为一个独立运行的进程。

小乌龟实验中：

- `/teleop_turtle`：键盘控制节点。
- `/turtlesim`：小乌龟仿真节点。
- `/circle_publisher`：自己编译的速度发布节点。
- `/pose_listener`：接收状态反馈的节点。

同一个节点可以同时发布和订阅。例如 `/turtlesim` 订阅速度指令，同时发布位置反馈。

### 2. 话题（Topic）

话题是带名字的数据通道，发布者往其中发送消息，订阅者接收消息。它适合连续变化的数据流。

我的第一次判断是：

- 键盘程序是发布者。
- 小乌龟仿真程序是订阅者。
- `/turtle1/cmd_vel` 是数据通道名。

实际查询结果：

```text
Type: geometry_msgs/Twist
Publishers: /teleop_turtle
Subscribers: /turtlesim
```

### 3. 消息（Message）

消息类型规定数据的结构。话题名和消息类型各有作用，通信双方需要匹配相应的话题和类型。

本次使用：

| 话题 | 消息类型 | 作用 |
| --- | --- | --- |
| /turtle1/cmd_vel | geometry_msgs/Twist | 目标速度指令 |
| /turtle1/pose | turtlesim/Pose | 仿真状态反馈 |

在小乌龟的平面运动实验中，主要关注 Twist 的两个字段：

- `linear.x`：前进、后退的线速度。
- `angular.z`：转向角速度，正值逆时针，负值顺时针。

实际观察到：

```yaml
linear:
  x: 2.0
  y: 0.0
  z: 0.0
angular:
  x: 0.0
  y: 0.0
  z: 0.0
```

这条指令要求向前直走。另一次消息中 `linear.x = 0`、`angular.z = -2`，要求原地顺时针转动。

我判断 `linear.x = 1`、`angular.z = 1` 会边前进边转弯，实际运行后观察到了绕圈运动。

## 四、ROS Master 的作用

Master 负责节点和通信资源的注册、查找，帮助节点发现彼此。

一次话题连接可以简化理解为：

1. 发布者向 Master 注册话题信息。
2. 订阅者向 Master 注册订阅，获得发布者的联系信息。
3. 节点之间协商并建立连接。
4. 消息通过节点之间的连接传输。

发现信息在运行中也可能更新，例如有新的发布者加入。Master 不承担普通话题消息的数据转发。

在问答中，我判断：Master 退出后，如果双方节点和已有连接仍正常，已有话题连接可以继续传输；新节点的注册和发现会受到影响。本次没有专门关闭 Master 做故障实验。

启动核心服务：

```bash
roscore
```

本次看到了以下启动信息，确认核心服务正常启动：

```text
/rosdistro: noetic
started core service [/rosout]
```

`roscore` 会启动 Master、参数服务器和日志节点等核心服务。

## 五、话题、服务和参数怎样选择

| 需求 | 适合的方式 | 理由 |
| --- | --- | --- |
| 持续发送目标速度、接收实际状态 | 话题 | 连续的数据流 |
| 请求里程计清零，并取得执行结果 | 服务 | 一次请求，对应一次应答 |
| 保存轮子半径、最大车速或 PID 配置 | 参数 | 程序需要读取的配置 |

我的记忆方法：话题传数据，服务办一次事，参数存配置。

### 1. 服务（Service）

服务包含请求和响应。

一开始，我把“里程计清零并返回结果”误选成了广播方式。后来通过拆解过程，理解了服务的特点：

```text
提出清零请求 → 底盘执行 → 返回执行结果
```

“收到请求”还不足以确认“已经执行成功”，需要结合服务定义和返回结果判断。

本次实际调用了生成乌龟的服务：

```bash
rosservice call /spawn "{x: 2.0, y: 2.0, theta: 0.0, name: 'turtle2'}"
```

终端返回：

```text
name: "turtle2"
```

窗口中出现了第二只乌龟，完成了一次请求和应答。服务的响应字段由对应的 `.srv` 定义，并不一定都包含“成功/失败”字段。

### 2. 参数（Parameter）

参数服务器保存配置。程序需要主动读取并使用这些值。

我做了背景颜色实验：

```bash
rosparam get /turtlesim/background_r
rosparam set /turtlesim/background_r 255
rosparam get /turtlesim/background_r
```

两次读取结果分别是 `69` 和 `255`，但背景没有立即变色。之后调用：

```bash
rosservice call /clear
```

仿真程序在处理清屏时重新读取背景配置，清除了轨迹，并显示新的背景色。

我的理解是：`rosparam set` 把新配置存入参数服务器，程序何时采用它，取决于读取和更新配置的实现。

同样，如果控制器只在启动时读取一次 `kp`，运行中仅修改参数服务器里的值，控制效果不会立即改变。

## 六、功能包和工作空间

### 1. 功能包与节点的区别

我最初认为下面两条命令涉及两个包、两个节点：

```bash
rosrun turtlesim turtlesim_node
rosrun turtlesim turtle_teleop_key
```

后来纠正为：一个功能包，两个可执行程序，分别运行时形成两个节点。

```text
turtlesim（功能包）
├── turtlesim_node
└── turtle_teleop_key
```

`rosrun` 的格式是：

```text
rosrun 功能包名 可执行程序名
```

查询包的位置：

```bash
rospack find turtlesim
```

实际返回：

```text
/opt/ros/noetic/share/turtlesim
```

另外，`turtle1`、`turtle2` 是仿真对象的名字，两只乌龟都由同一个 `/turtlesim` 节点管理。

### 2. 创建自己的功能包

```bash
mkdir -p /home/wangsikai/ros_learning_ws/src
cd /home/wangsikai/ros_learning_ws/src
catkin_create_pkg ros_learning roscpp geometry_msgs
```

后来为了使用 `turtlesim/Pose`，又添加了 `turtlesim` 依赖。

本次练习的主要结构：

```text
ros_learning_ws/
├── src/
│   └── ros_learning/
│       ├── package.xml
│       ├── CMakeLists.txt
│       ├── src/
│       │   ├── circle_publisher.cpp
│       │   ├── pose_listener.cpp
│       │   └── heading_controller.cpp
│       ├── config/
│       │   ├── motion.yaml
│       │   └── heading.yaml
│       └── launch/
│           ├── demo.launch
│           └── heading.launch
├── build/
└── devel/
```

外层 `src` 存放功能包，包内的 `src` 存放 C++ 源码。`build` 是构建中间文件，`devel` 保存开发环境相关的构建产物。

### 3. 两个关键文件

- `package.xml`：包的名称、版本、依赖等信息。
- `CMakeLists.txt`：查找依赖，指定编译目标和链接规则。

例如：

```cmake
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  turtlesim
)

add_executable(circle_publisher src/circle_publisher.cpp)
target_link_libraries(circle_publisher ${catkin_LIBRARIES})
```

`add_executable` 指定源码和生成的可执行程序，`target_link_libraries` 链接所需的库。

### 4. 构建与加载环境

```bash
cd /home/wangsikai/ros_learning_ws
catkin_make
source devel/setup.bash
rospack find ros_learning
```

本次构建成功后，查询到了：

```text
/home/wangsikai/ros_learning_ws/src/ros_learning
```

`catkin_make` 在工作空间根目录执行。`source devel/setup.bash` 将工作空间环境加载到当前终端；新终端也需要加载相应环境。

## 七、自己编写发布者和订阅者

### 1. 发布节点

核心过程：

```cpp
ros::init(argc, argv, "circle_publisher");
ros::NodeHandle nh;

ros::Publisher pub =
    nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);

ros::Rate rate(10);

while (ros::ok())
{
    geometry_msgs::Twist cmd;
    cmd.linear.x = 1.0;
    cmd.angular.z = 1.0;
    pub.publish(cmd);
    rate.sleep();
}
```

我理解的几个接口：

- `ros::init`：初始化 ROS，设置节点名。
- `advertise`：创建发布者，指定消息类型、话题和发送队列容量。
- `publish`：发送一条消息。
- `ros::Rate` 与 `rate.sleep()`：安排循环频率。

`advertise` 最后的 `10` 是队列容量，`ros::Rate(10)` 中的 `10` 表示每秒约 10 次循环。

### 2. 话题重映射

第一次运行自己编译的程序时，只有第一只乌龟绕圈，因为源码指定了 `/turtle1/cmd_vel`。

如果修改源码，完整话题名应改为 `/turtle2/cmd_vel`。我还实际使用了启动时的重映射：

```bash
rosrun ros_learning circle_publisher /turtle1/cmd_vel:=/turtle2/cmd_vel
```

第二只乌龟开始绕圈。`:=` 左侧是程序原来使用的话题，右侧是本次运行要使用的话题。

### 3. 订阅节点与回调函数

查询反馈：

```bash
rostopic info /turtle2/pose
rostopic echo -n 1 /turtle2/pose
```

`-n 1` 表示接收并显示一条消息后退出。实际看到的消息类型为 `turtlesim/Pose`，发布者为 `/turtlesim`。

反馈中包含：

- `x、y`：位置。
- `theta`：朝向，单位为弧度。
- `linear_velocity、angular_velocity`：仿真中的运动速度。

订阅节点的核心接口：

```cpp
ros::Subscriber sub = nh.subscribe("/turtle2/pose", 10, poseCallback);
ros::spin();
```

`subscribe` 注册订阅和回调函数，`spin` 持续处理回调任务。回调函数通过 `msg->x` 等字段读取消息内容。

我判断：如果登记完订阅就直接 `return 0`，程序会结束，无法持续处理反馈。

订阅节点运行后，位置和朝向持续更新。代码限制了日志的打印频率，打印频率不能直接当作消息接收频率。

## 八、YAML 参数文件

本次创建的 `config/motion.yaml`：

```yaml
circle_publisher:
  linear_speed: 1.0
  angular_speed: 0.5
  publish_rate: 10.0
```

YAML 用缩进表达层级，常用 `key: value` 表示配置。缩进使用空格，键值之间的冒号后留空格。

按这份结构加载后，我写出的参数路径是：

```text
/circle_publisher/angular_speed = 0.5
```

实际加载和查询：

```bash
rosparam load /home/wangsikai/ros_learning_ws/src/ros_learning/config/motion.yaml
rosparam get /circle_publisher
```

终端正确显示了三个参数。

随后，发布者改为在启动时读取配置：

```cpp
ros::NodeHandle pnh("~");
pnh.param("angular_speed", angular_speed, 1.0);
```

`~` 表示当前节点的私有参数空间。节点名是 `circle_publisher`，所以这里读取 `/circle_publisher/angular_speed`。找不到参数时使用默认值 `1.0`。

重新编译、启动后，日志中出现：

```text
Parameters: linear=1.00 angular=0.50 rate=10.00 Hz
```

实际观察到绕圈变大，说明新的配置被程序采用。这版程序只在启动时读取一次参数，之后使用读取到的数值。

## 九、launch 统一启动

我用 `demo.launch` 完成了三件事：

1. 加载 `motion.yaml`。
2. 启动小乌龟仿真和速度发布节点。
3. 启动状态订阅节点，并把反馈话题映射到第一只乌龟。

关键写法：

```xml
<rosparam command="load"
          file="$(find ros_learning)/config/motion.yaml"/>

<node pkg="ros_learning" type="circle_publisher"
      name="circle_publisher" output="screen"/>
```

- `pkg`：功能包名。
- `type`：可执行程序名。
- `name`：运行时节点名。
- `output="screen"`：把节点输出显示在终端。
- `$(find ros_learning)`：查找包的位置，使文件路径能随包移动。

订阅者使用了：

```xml
<remap from="/turtle2/pose" to="/turtle1/pose"/>
```

启动命令：

```bash
roslaunch ros_learning demo.launch
```

实际看到新窗口中的乌龟绕圈，终端持续打印其位置。创建或修改 launch、YAML 文件后，启动时会直接读取；修改 C++ 源码后需要重新构建。

## 十、计算图与开环、闭环

运行：

```bash
rqt_graph
```

实际看到的数据流：

```text
/circle_publisher
    │ /turtle1/cmd_vel
    ▼
/turtlesim
    │ /turtle1/pose
    ▼
/pose_listener
```

图中的椭圆是节点，箭头旁标注通信话题。

我判断这套程序还没有形成反馈闭环，理由是：

- 发布者一直发送启动时读到的固定速度。
- 订阅者只是打印状态。
- 状态反馈没有参与下一次控制输出的计算。

因此，“能看到反馈”与“用反馈进行控制”需要区分。闭环控制的关键是用实际状态与目标比较，再调整输出。

## 十一、平面运动与比例控制实验

### 1. 线速度、角速度与转弯半径

在平面匀速转弯条件下：

```text
R = v / |ω|
```

| 线速度 v | 角速度 ω | 理论半径 R |
| ---: | ---: | ---: |
| 1.0 | 1.0 | 1.0 |
| 1.0 | 0.5 | 2.0 |
| 1.0 | 2.0 | 0.5 |

我实际观察到了角速度从 1.0 降到 0.5 后，绕圈半径变大；问答中也算出了最后一种情况的半径是 0.5。

这是理解底盘运动学的一个入口：线速度和角速度共同决定运动轨迹。

### 2. P 比例控制的计算

本次任务是让小乌龟原地转到目标朝向后停下。

基本关系：

```text
误差 e = 目标朝向 − 当前朝向
角速度输出 = kp × e
```

目标为 `1.0` 弧度，`kp = 2.0` 时：

| 当前朝向 | 误差 | 比例输出 |
| ---: | ---: | ---: |
| 0.2 | 0.8 | 1.6 |
| 0.9 | 0.1 | 0.2 |
| 1.0 | 0 | 0 |
| 1.2 | -0.2 | -0.4 |

我回答最后一种情况输出为 `-0.4`。负号表示超过目标后向反方向修正。

### 3. 实际闭环节点

`heading_controller` 同时订阅 `/turtle1/pose`、发布 `/turtle1/cmd_vel`，每轮使用最新反馈计算角速度。

```text
当前朝向 → 与目标比较 → P 控制器计算 → 发送角速度
    ↑                                      │
    └────────── 小乌龟运行后的状态反馈 ────────┘
```

配置文件：

```yaml
heading_controller:
  target_theta: 1.0
  kp: 2.0
  max_angular_speed: 1.0
  angle_tolerance: 0.01
  control_rate: 30.0
  pose_timeout: 0.5
```

启动：

```bash
roslaunch ros_learning heading.launch
```

代码的主要处理包括：

- 用 `ros::spinOnce()` 在控制循环中处理反馈，再计算输出。
- 计算最短角度误差，处理朝向在 ±π 附近的环绕。
- 将角速度限制在允许范围内。
- 误差进入容许范围时输出零速度。
- 没有新鲜反馈时输出零速度。

本次主要验证了正常运行和发布者冲突排查，没有逐项测试所有异常保护分支。

最初日志中的误差一直在约 0.02～0.04 波动，窗口还出现了平移轨迹。排查发现旧的 `circle_publisher` 仍然注册为速度发布者。清理旧实验、确保速度话题由闭环控制器单独发布后，实验达到了预期：小乌龟接近目标朝向后停下。

本次实现的是普通 ROS 节点中的 P 控制，没有加入 PID 的 I、D 项。

## 十二、对 ros_control 的初步理解

控制组要求重点理解 `ros_control`。通过本次学习，我先把它的分工记清楚：

| 部分 | 作用 |
| --- | --- |
| Controller | 根据目标和实际状态计算控制输出 |
| RobotHW / 硬件接口 | 读取硬件状态、向硬件发送指令 |
| Controller Manager | 加载、启动、停止和切换控制器，并调用运行中的控制器更新 |

典型控制循环：

```text
read() → Controller Manager 调用控制器 update() → write()
```

- `read()`：读取编码器等反馈状态。
- `update()`：控制器根据反馈和目标计算输出。
- `write()`：把指令发送到实际硬件。

我在问答中的理解：

1. 换电机后，通信数据格式发生变化，应优先修改硬件接口中的读取和发送代码。
2. 控制器已经加载但状态为 `stopped`，需要由控制器管理器启动它。
3. 发出驱动指令不能证明电机已经达到目标转速，需要实际反馈。
4. 参数服务器里的 PID 参数改变后，是否生效取决于控制器的读取和更新机制。

`ros_control` 是控制器与硬件接口的组织框架。控制循环中的状态和指令也可以通过接口共享，不能把每次 `read/update/write` 都理解为一次 ROS 话题通信。

本次小乌龟 P 实验帮助我理解了反馈控制，但还没有编写 RobotHW 硬件类或 ros_control 控制器插件。这部分属于后续需要继续学习的内容。

## 十三、调试过程中的问题与收获

### 1. 下载工具时的代理问题

最初 `wget` 提示无法解析 `[::1]`。使用 `--no-proxy` 后，安装工具下载成功。随后清理当前终端的代理环境变量，继续运行安装流程。

这让我认识到，下载安装失败时要先看错误发生在域名解析、连接还是软件包安装环节。

### 2. Docker 安装包找不到

小鱼工具安装 Docker 时出现 `docker-ce-cli` 无候选、找不到 `containerd.io` 等错误，之后又出现：

```text
sudo: docker: command not found
```

尽管脚本继续向下执行，Docker 实际没有安装成功。我退出流程，使用宿主机 Ubuntu 软件源中的 `docker.io` 完成安装，再用：

```bash
sudo docker version
```

确认 Client 和 Server 都显示版本信息后，重新进行 Noetic 容器安装。

这是本次环境中的处理记录，不表示所有系统都应采用相同安装步骤。

### 3. 同名节点冲突

出现过：

```text
new node registered with same name
```

我理解到，同一 ROS 系统中的完整节点名应唯一。多个终端重复启动同名程序，可能导致之前的同名节点被要求退出。

处理时先结束旧节点，再单独启动需要的实例。

### 4. 多个速度发布者干扰控制

闭环实验中，要确认速度指令的来源：

```bash
rostopic info /turtle1/cmd_vel
```

本次独立验证 P 控制器时，Publishers 应只有 `/heading_controller`。检查发布者解决了旧固定速度程序与闭环控制器之间的干扰问题。

我的收获是：控制效果异常时，既要检查控制算法，也要检查是否有重复节点、错误话题和多路指令来源。

## 十四、常用命令整理

以下 ROS 命令在已加载环境的容器终端中使用：

| 命令 | 作用 |
| --- | --- |
| rosnode list | 查看注册的节点 |
| rosnode info 节点名 | 查看节点通信信息 |
| rosnode kill 节点名 | 请求关闭节点 |
| rostopic list | 查看话题 |
| rostopic info 话题名 | 查看消息类型、发布者、订阅者 |
| rostopic echo 话题名 | 显示消息内容 |
| rostopic echo -n 1 话题名 | 只显示一条消息 |
| rostopic pub | 从命令行发布消息 |
| rosservice call 服务名 | 调用服务 |
| rosparam get 参数名 | 读取参数 |
| rosparam set 参数名 值 | 设置参数 |
| rosparam load 文件 | 加载 YAML 参数 |
| rospack find 包名 | 查找包目录 |
| rosrun 包名 程序名 | 运行程序 |
| roslaunch 包名 启动文件 | 按 launch 文件启动 |
| catkin_make | 构建工作空间 |
| rqt_graph | 查看节点通信图 |

## 十五、学习反思与后续预习

本次学习中，我从分辨发布者、订阅者开始，逐步完成了环境安装、消息观察、功能包创建、C++ 节点构建、参数配置、统一启动和 P 闭环控制实验。

我纠正了几个容易混淆的认识：

- 同一个功能包可以包含多个程序，包的数量与节点数量不能直接画等号。
- 生成一只新的仿真乌龟，并不意味着增加一个新的 ROS 节点。
- 服务要结合一次请求和响应理解。
- 参数写入服务器后，程序需要读取并采用它。
- 能打印反馈，并不代表反馈已经参与控制。
- 控制器的计算与硬件接口的收发各有分工。

后续按照控制组要求继续预习：

1. tf/tf2：坐标系、坐标变换与坐标系树。
2. PID：继续学习积分项、微分项及实际调参。
3. 底盘运动学：理解具体底盘结构与轮速、车体速度之间的关系。
4. ros_control：进一步阅读硬件接口、控制器实现及 Controller Manager 的使用。

本次使用 Codex 辅助解释概念、提供代码示例和排查问题。发布节点由我按步骤创建和编译，订阅节点、参数化版本和 P 控制示例由 AI 辅助准备，我在自己的环境中复制、构建、运行并观察结果。以上笔记依据实际问答和实验整理，后续仍需继续阅读源码、独立修改和验证。

## 参考资料

- 课程课件：《ROS架构与通信(2)》。
- [小鱼的一键安装系列](https://fishros.org.cn/forum/topic/20/%E5%B0%8F%E9%B1%BC%E7%9A%84%E4%B8%80%E9%94%AE%E5%AE%89%E8%A3%85%E7%B3%BB%E5%88%97)
- [Docker 官方文档：容器](https://docs.docker.com/get-started/docker-concepts/the-basics/what-is-a-container/)
- [Docker 官方文档：镜像](https://docs.docker.com/get-started/docker-concepts/the-basics/what-is-an-image/)
- [ROS REP 3：各发行版的目标平台](https://github.com/ros-infrastructure/rep/blob/master/rep-0003.rst)
