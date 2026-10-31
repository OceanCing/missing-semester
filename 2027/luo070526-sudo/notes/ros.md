---
tags:
  - type/lesson
  - topic/ros
  - topic/gazebo
  - topic/vision
  - topic/robomaster
  - topic/reliability
  - status/done
date: 2026-10-29
---

# 第三课：ROS 架构与通信

本文所有 ROS 节点、算法与控制逻辑示例统一使用 C++ 和 roscpp；YAML、XML/Launch、CMake、Catkin、MATLAB 与 Linux 命令只用于配置、构建、分析和运行。

ROS 解决的是“多个机器人程序如何分工并交换数据”。本课用 turtlesim 串起节点、话题、消息、服务、参数和 Catkin。

```text
输入节点产生指令
  → ROS 通信传递数据
  → 处理节点更新状态
  → 状态再次发布给其他节点
```

## 1. ROS 计算图

一台 RoboMaster 机器人可能同时运行相机驱动、视觉识别、状态估计、决策、底盘控制和电机通信。ROS 把这些功能拆成节点，并用统一接口连接。

| 概念 | 含义 | 例子 |
| --- | --- | --- |
| Package | 组织源码、配置和依赖 | `turtlesim` |
| Executable | 编译得到的可执行程序 | `turtlesim_node` |
| Node | 加入 ROS 计算图的运行中进程 | `/turtlesim` |
| Topic | 持续传输数据的通道 | `/turtle1/cmd_vel` |
| Message | 话题中数据的结构 | `geometry_msgs/Twist` |
| Service | 一次请求对应一次响应 | `/spawn` |
| Parameter | 运行时配置 | 背景颜色 |

命令：

```bash
rosrun turtlesim turtlesim_node
```

其中第一个 `turtlesim` 是包名，`turtlesim_node` 是可执行文件名，运行后的默认节点名是 `/turtlesim`。

### 1.1 Master

```bash
roscore
```

节点先向 Master 注册名称、话题、服务和通信地址。订阅者找到发布者后，消息主要在节点之间直接传输。

因此 Master 更像“登记处”，而不是每条消息都必须经过的中转站。

## 2. turtlesim：从按键到运动

需要三个终端：

```bash
# 终端 1
roscore

# 终端 2
rosrun turtlesim turtlesim_node

# 终端 3
rosrun turtlesim turtle_teleop_key
```

把输入焦点放在终端 3，再按方向键。

内部过程：

```text
方向键
  → turtle_teleop_key 生成 Twist
  → 发布到 /turtle1/cmd_vel
  → /turtlesim 接收消息
  → 更新速度、位姿和画面
  → 发布 /turtle1/pose
```

查看消息结构：

```bash
rosmsg show geometry_msgs/Twist
```

`Twist` 中包含 `linear` 和 `angular` 两个三维向量。但 turtlesim 的运动回调主要读取：

- `linear.x`：前进或后退；
- `angular.z`：在二维平面内转向。

消息中存在 `linear.y`，不代表订阅端一定会使用它。理解通信时需要同时看消息定义和接收端源码。

### 2.1 观察节点与数据

```bash
rosnode list
rosnode info /turtlesim
rosnode ping /turtlesim
rostopic list
rostopic info /turtle1/cmd_vel
rostopic type /turtle1/cmd_vel
rostopic echo /turtle1/pose
rqt_graph
```

### 2.2 结束节点

```bash
rosnode kill /turtlesim
```

- `Ctrl+C`：向当前终端的前台进程发送中断信号；
- `q`：`turtle_teleop_key` 自己定义的退出键；
- `rosnode kill`：从其他终端请求指定节点关闭。

## 3. Topic：发布与订阅

Topic 适合速度、图像、雷达和位姿等连续数据。发布者与订阅者正确连接需要：

1. 话题名一致；
2. 消息类型一致；
3. 节点注册到同一个 Master；
4. 双方网络地址可访问。

### 3.1 发布者：发送 hello

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
    ROS_INFO("publish: %s", msg.data.c_str());
    rate.sleep();
  }
  return 0;
}
```

- `advertise` 声明话题和消息类型；
- `10` 是发送队列长度，不是频率；
- `ros::Rate(1)` 表示循环约为 1 Hz；
- `publish` 发送消息。

### 3.2 订阅者：接收并打印

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

消息到达后执行 `callback`。`ros::spin()` 让节点持续等待并处理回调；删掉它以后，`main` 很快结束。

### 3.3 命令行发布速度

```bash
rostopic pub -r 10 /turtle1/cmd_vel geometry_msgs/Twist "linear:
  x: 2.0
  y: 0.0
  z: 0.0
angular:
  x: 0.0
  y: 0.0
  z: 1.0"
```

`-r 10` 表示以 10 Hz 重复发布。`linear.x = 2.0` 与 `angular.z = 1.0` 让乌龟一边前进一边转向。

若出现：

```text
ERROR: invalid topic type: 10
```

说明 `10` 被放到了消息类型的位置。正确顺序是：

```text
rostopic pub [选项] 话题名 消息类型 消息内容
```

## 4. Service 与 Parameter

### 4.1 Service

Service 适合“一次请求并等待结果”，例如生成新乌龟。

```bash
rosservice list
rosservice type /spawn
rossrv show turtlesim/Spawn
rosservice call /spawn "x: 2.0
y: 3.0
theta: 0.0
name: 'turtle2'"
```

使用服务前先用 `rossrv show` 查看请求和响应字段，不要凭截图猜参数。

### 4.2 Parameter

Parameter 保存阈值、增益和颜色等运行配置，不适合高频数据。

```bash
rosparam list
rosparam get /turtlesim/background_r
rosparam set /turtlesim/background_r 255
rosparam set /turtlesim/background_g 255
rosparam set /turtlesim/background_b 255
rosservice call /clear
```

### 4.3 选择通信方式

| 需求 | 方式 |
| --- | --- |
| 相机持续发布图像 | Topic |
| 持续发送底盘速度 | Topic |
| 生成一只乌龟并获得结果 | Service |
| 设置控制增益 | Parameter |
| 长时间任务，需要进度和取消 | Action |

## 5. C++ 节点怎样被构建

```text
节点源码
  → CMakeLists.txt 规定编译与链接
  → package.xml 声明包和依赖
  → Catkin 构建
  → source 工作空间环境
  → rosrun 或 roslaunch 启动
```

典型目录：

```text
dx_ros_practice/
├── CMakeLists.txt
├── package.xml
└── src/
    ├── talker.cpp
    └── listener.cpp
```

核心 CMake：

```cmake
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

catkin_package()

include_directories(${catkin_INCLUDE_DIRS})

add_executable(talker src/talker.cpp)
target_link_libraries(talker ${catkin_LIBRARIES})

add_executable(listener src/listener.cpp)
target_link_libraries(listener ${catkin_LIBRARIES})
```

构建与运行：

```bash
source /opt/ros/noetic/setup.bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_create_pkg dx_ros_practice roscpp std_msgs
cd ~/catkin_ws
catkin_make
source devel/setup.bash
rosrun dx_ros_practice talker
```

另一个终端：

```bash
source /opt/ros/noetic/setup.bash
source ~/catkin_ws/devel/setup.bash
rosrun dx_ros_practice listener
```

验证：

```bash
rosnode list
rostopic info /chatter
rostopic echo /chatter
rostopic hz /chatter
```

只看到“没有报错”还不够；还要确认节点、连接、消息内容和频率。

## 6. 本次 Docker 与 ROS 实操

### 6.1 宿主机与容器

在 Ubuntu 宿主机查看和进入容器：

```bash
sudo docker ps -a
sudo docker exec -it noetic-gui bash
```

出现 `root@...#` 后已经位于容器内部，不要再次执行 `docker exec`。

容器内检查环境：

```bash
echo "$ROS_DISTRO"
source /opt/ros/noetic/setup.bash
source ~/dx_lesson2_ws/devel/setup.bash
```

宿主机找不到 `/opt/ros/noetic/setup.bash`，不代表容器中的 ROS 没有安装。

### 6.2 已验证的节点与话题

本次已查询到：

```text
/dx_velocity_node
/dx_demo/cmd_vel
```

继续验证应使用：

```bash
rosnode info /dx_velocity_node
rostopic info /dx_demo/cmd_vel
rostopic type /dx_demo/cmd_vel
rostopic echo -n 1 /dx_demo/cmd_vel
```

若 `roscore` 提示已有 Master，不要重复启动，先用 `rosnode list` 与 `rostopic list` 检查现有系统。

### 6.3 图形显示问题

本次 turtlesim 出现过：

```text
Authorization required
could not connect to display :0
Qt xcb error
```

这属于容器图形转发问题，优先检查：

- 宿主机 X11 授权；
- `DISPLAY`；
- `/tmp/.X11-unix` 是否挂载。

它不等于 ROS 安装失败。当前小乌龟图形显示尚未完成修复验证。

## 7. 在 RoboMaster 中的位置

| 课堂例子 | 比赛系统 |
| --- | --- |
| 键盘发布速度 | 决策节点发布底盘目标 |
| `/turtle1/cmd_vel` | 底盘速度目标话题 |
| turtlesim 更新状态 | 控制器计算电机输出 |
| `/turtle1/pose` | 定位或状态估计结果 |

卡尔曼滤波可以作为状态估计节点：订阅编码器、IMU 或视觉测量，发布融合后的机器人状态。`ros_control` 的控制循环可以记为：

1. `read`：读取传感器与电机状态；
2. `update`：计算控制量；
3. `write`：通过 CAN 等接口发送电机命令。

ROS 解决数据如何传递，算法解决收到数据后如何计算。

## 8. 固定排错顺序

| 顺序 | 检查内容 | 命令 |
| --- | --- | --- |
| 1 | Master 是否存在 | `rosnode list` |
| 2 | 节点是否运行 | `rosnode ping /turtlesim` |
| 3 | 发布订阅是否连接 | `rosnode info`、`rqt_graph` |
| 4 | 话题名是否正确 | `rostopic list` |
| 5 | 消息类型是否正确 | `rostopic type`、`rosmsg show` |
| 6 | 是否真的有数据 | `rostopic echo`、`rostopic hz` |
| 7 | 服务字段是否正确 | `rosservice type`、`rossrv show` |
| 8 | 包是否构建并加载 | `catkin_make`、`source devel/setup.bash` |

“乌龟不动”时，按节点、话题、类型、数据、订阅端处理的顺序检查，不要直接重装 ROS。

## 9. 视觉组：从 Gazebo 仿真到图像处理

第三课课件给出的视觉链路是：

```text
相机驱动发布图像 /image_raw
  → 识别节点订阅图像并发布目标 /track
  → 解算节点计算云台指令 /gimbal_cmd
  → 控制节点把指令发送给执行机构
```

图像、检测结果和控制指令都在持续变化，因此主要使用 Topic。相机内参、曝光和识别阈值等配置适合放在 Parameter 或 YAML 中；标定、保存一次图片等偶发操作才适合 Service。

### 9.1 `smb_common_v2` 的三个功能包

这个压缩包是 Super Mega Bot 的 ROS1 仿真工程。根据源码，它把机器人模型、Gazebo 仿真和控制配置拆成三个包：

| 功能包 | 主要作用 | 关键文件 |
| --- | --- | --- |
| `smb_description` | 描述机器人本体、关节和传感器安装位置 | `urdf/`、`sensor_plugins/`、`launch/load.launch` |
| `smb_gazebo` | 加载仿真世界、生成机器人并运行 Gazebo 插件 | `launch/smb_gazebo.launch`、`worlds/` |
| `smb_control` | 加载底盘控制器、速度选择和定位参数 | `config/`、`launch/control.launch` |

源码中的启动关系可以整理为：

```text
smb_gazebo.launch
├── 启动 Gazebo 世界
├── spawn_model 从 robot_description 生成机器人
├── 把 /rslidar_points 转换成 /scan
└── 包含 control.launch
    ├── load.launch 调用 xacro 生成 robot_description
    ├── controller_manager 加载底盘与关节状态控制器
    ├── robot_state_publisher 发布 TF
    ├── twist_mux 选择速度指令来源
    └── ekf_localization 融合里程计与 IMU
```

`robot_description` 是参数服务器中的整棵 URDF 模型。Gazebo 根据它生成机器人，`robot_state_publisher` 再根据关节状态发布各坐标系之间的 TF。

### 9.2 Gazebo 传感器插件

URDF/Xacro 描述“传感器装在哪里”，Gazebo 插件负责“在仿真中产生数据并发布成 ROS 话题”。

| 传感器 | 源码中的主要设置 | 发布内容 |
| --- | --- | --- |
| 单目相机 | `640×480`、`30 Hz`、视场角约 `80°` | `/mono/cam0/image_raw` |
| Blackfly 相机 | `720×540`，帧率由宏参数传入 | `/<name>/color/image_raw`、`/<name>/color/camera_info` |
| 双目相机 | 左右各 `720×540`，基线 `0.09 m` | 左右图像与相机信息，最终名称受命名空间影响 |
| 深度相机 | `1280×720`，裁剪范围 `0.05～11 m` | 彩色图、深度图、深度相机信息和点云 |
| Velodyne 激光雷达 | `10 Hz` | `/rslidar_points`，随后可转换为 `/scan` |
| IMU | `400 Hz` | `/imu0` |

图像话题通常使用 `sensor_msgs/Image`。`camera_info` 保存相机标定信息，做去畸变、三维恢复或 PnP 解算时不能只订阅图像而忽略它。`frameName` 则说明数据属于哪个坐标系，后续通过 TF 转换到云台、底盘或世界坐标系。

### 9.3 文件存在不等于插件已经运行

压缩包中虽然有单目、双目和深度相机插件，但默认的 `smb_standalone.urdf.xacro` 只实际接入了 IMU 和激光雷达。相机文件目前更像可复用的宏定义。

要让虚拟相机真正发布数据，至少要完成：

1. 在主 Xacro 中 `include` 相机插件文件；
2. 创建相机对应的 `link`、固定 `joint` 和 optical frame；
3. 实例化相机宏，并保证 `<gazebo reference="...">` 指向真实存在的 link；
4. 启动后用 `rostopic list` 确认图像话题确实出现。

因此不能仅凭“目录里有相机文件”就写成“相机仿真已经运行成功”。

### 9.4 视觉节点的典型订阅代码

视觉节点接收 ROS 图像时，常用 `image_transport` 订阅，再用 `cv_bridge` 转成 OpenCV 图像：

```cpp
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/core.hpp>

void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
  try {
    const cv_bridge::CvImageConstPtr cvPtr =
        cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::BGR8);

    const cv::Mat& image = cvPtr->image;
    ROS_INFO_THROTTLE(1.0, "image: %d x %d, frame: %s",
                      image.cols, image.rows,
                      msg->header.frame_id.c_str());

    // 在这里进行颜色转换、目标检测或特征提取。
  } catch (const cv_bridge::Exception& e) {
    ROS_ERROR("cv_bridge error: %s", e.what());
  }
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "vision_subscriber");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);

  image_transport::Subscriber sub =
      it.subscribe("/mono/cam0/image_raw", 1, imageCallback);

  ros::spin();
  return 0;
}
```

图像数据量大，订阅队列通常先设为 `1`，让处理节点优先拿到最新帧，避免算法速度跟不上时积压大量旧图像。回调中若要进行耗时推理，还应考虑独立线程或把处理拆到其他节点。

对应的 CMake 核心配置：

```cmake
find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  image_transport
  cv_bridge
)
find_package(OpenCV REQUIRED)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(vision_subscriber src/vision_subscriber.cpp)
target_link_libraries(
  vision_subscriber
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
)
```

`package.xml` 中也要声明 `roscpp`、`sensor_msgs`、`image_transport` 和 `cv_bridge`，否则换一台电脑或重新安装依赖时可能构建失败。

### 9.5 仿真数据的验证方法

启动仿真前先加载环境：

```bash
source /opt/ros/noetic/setup.bash
source ~/catkin_ws/devel/setup.bash
```

如果工程及依赖已经放入工作空间并成功构建，源码给出的入口是：

```bash
roslaunch smb_gazebo smb_gazebo.launch
```

本次只完成了源码阅读，没有在本机完成上述工程的依赖安装、构建和运行，因此下面是运行后的验证顺序，不应当写成已经得到的实测结果：

```bash
rosnode list
rostopic list | grep -E 'image|camera_info|points|scan|imu|odom|cmd_vel'
rostopic type /rslidar_points
rostopic hz /rslidar_points
rostopic type /imu0
rostopic hz /imu0
rqt_graph
```

接入名为 `front` 的 Blackfly 相机后，可以继续检查：

```bash
rostopic type /front/color/image_raw
rostopic hz /front/color/image_raw
rostopic echo -n 1 /front/color/camera_info
rqt_image_view /front/color/image_raw
rosrun tf tf_echo base_link front_optical_link
```

不要用 `rostopic echo` 长时间打印原始图像，像素数组会迅速刷满终端。图像内容用 `rqt_image_view` 看，频率用 `rostopic hz` 看，连接关系用 `rostopic info` 或 `rqt_graph` 看。

需要离线复现视觉问题时，可以只录关键话题：

```bash
rosbag record -O vision_test.bag /front/color/image_raw /front/color/camera_info /imu0
# 录制结束时按 Ctrl+C
rosbag info vision_test.bag
rosbag play vision_test.bag
```

`-O vision_test.bag` 固定了输出文件名，便于后续查看和回放。

### 9.6 这份工程暴露出的接口问题

源码阅读中可以发现几处必须核对的接口：

- IMU 插件发布 `/imu0`，但 `localization.yaml` 订阅 `imu/data`；若没有额外 remap，EKF 收不到 IMU；
- `gazebo_interface.yaml` 写的是 `husky_velocity_controller/odom`，实际控制器配置名是 `smb_velocity_controller`；
- `gazebo_interface.yaml` 中有一处 `servce` 拼写错误；
- 相机插件只有宏定义，默认模型没有实例化。

这说明 ROS 工程排错不能只看某一个文件。必须沿着“发布者实际名称—话题名—消息类型—订阅者配置—remap”逐项核对。

### 9.7 视觉仿真的核心意义

Gazebo 中的虚拟相机和实车相机可以向视觉节点提供相同类型的 ROS 图像。只要话题名、消息类型、标定信息和 TF 接口保持一致，识别算法就能在仿真数据、rosbag 回放和真实相机之间切换。

```text
Gazebo 虚拟传感器 ─┐
rosbag 回放数据 ───┼→ 同一个视觉节点 → 检测结果 → 解算与控制
真实相机驱动 ──────┘
```

因此仿真的主要价值不是让画面看起来像游戏，而是在不上真车的情况下检查接口、坐标系、数据频率和算法流程。

## 10. RoboMaster 中的一条完整数据链

课堂上的 `talker → /chatter → listener` 只有一个字符串；比赛系统中的一条控制链还包含时间、坐标系、有效性和安全限制。

```text
相机驱动
  → 图像与采集时间
  → 视觉检测与目标坐标
  → 弹道/姿态解算
  → 云台或底盘目标
  → 控制器
  → EtherCAT/CAN
  → 电机执行
  → 编码器和 IMU 反馈
  → 状态估计
```

裁判系统数据还会给出比赛状态、机器人状态、功率或热量等约束。它们不能只显示在界面上，而应进入控制逻辑，决定当前允许的输出范围和失效策略。

### 10.1 消息除了数值，还要描述数值

一个简化的视觉目标消息可以包含：

```text
std_msgs/Header header
bool tracking
float32 yaw_error_rad
float32 pitch_error_rad
float32 distance_m
float32 confidence
```

- `header.stamp`：数据产生时间，不是回调收到时间；
- `header.frame_id`：目标量属于哪个坐标系；
- `tracking`：当前是否真的有可用目标；
- 单位写进字段名或接口文档；
- `confidence` 不能代替完整的有效性和超时判断。

如果消息没有时间和坐标系，下游即使收到数值，也无法判断它是否过期、能否与 IMU 或云台状态对齐。

### 10.2 接口要隔离赛季变化

RoboMaster 的规则、机器人制作规范和通信协议可能在赛季内更新。解析裁判系统数据时，应把协议帧解析放在独立模块中，并记录：

- 协议版本与发布日期；
- 帧头、命令字、长度和 CRC 校验；
- 字段单位、有效范围与更新频率；
- 断帧、校验失败和旧数据的处理；
- 新旧协议切换时的回归测试。

业务节点应订阅解析后的统一 ROS 消息，而不是到处直接读取原始字节。这样协议变化时只修改一处。

## 11. 延迟、队列与数据新鲜度

机器人失控不一定是“没有消息”，也可能是收到的消息已经太旧。端到端延迟可以粗略写成：

```text
总延迟 = 采集 + 传输 + 排队 + 算法 + 控制 + 执行
```

### 11.1 检查消息年龄

```cpp
void targetCallback(const dx_msgs::Target::ConstPtr& msg)
{
  const ros::Duration age = ros::Time::now() - msg->header.stamp;

  if (age.toSec() < 0.0 || age.toSec() > 0.08 || !msg->tracking)
  {
    targetValid = false;
    return;
  }

  latestTarget = *msg;
  targetValid = true;
}
```

`0.08 s` 只是示例，真实阈值要根据相机帧率、算法耗时和控制需求测试。出现负时间通常说明多机时钟不同步，或仿真时间和系统时间混用了。

### 11.2 队列不是越大越好

| 数据 | 常见考虑 |
| --- | --- |
| 相机图像 | 更关心最新帧，队列通常较小 |
| 目标检测结果 | 旧目标可能导致错误追踪，需要超时 |
| 电机状态 | 频率高，要避免回调阻塞 |
| 低频配置或状态切换 | 可以保留，但必须明确顺序 |

队列过小可能在瞬时抖动时丢数据；队列过大则会让节点处理历史消息。选择队列长度前要先回答：这个模块需要“每一条都处理”，还是“只需要最新状态”。

### 11.3 看门狗与零输出

控制节点应保存各关键输入的最后更新时间。视觉、遥控、裁判系统或电机反馈任一关键输入超时，都要进入预先定义的降级状态，而不是继续使用最后一次指令。

```text
输入正常 → 按当前模式控制
视觉超时 → 退出自瞄，保留人工控制
遥控超时 → 底盘和云台进入安全输出
电机反馈超时 → 停止闭环输出并报告故障
裁判数据超时 → 使用保守限制，不能默认无限功率
```

具体动作由整车安全设计决定，但“超时后怎么办”必须在上车前写清楚并测试。

## 12. TF、坐标系与单位

视觉给出的目标可能在相机坐标系，IMU 姿态在机体坐标系，控制器需要的是云台或世界坐标系。数值正确但坐标系错误，结果仍然会完全错误。

检查顺序：

```bash
rostopic echo -n 1 /front/color/camera_info/header
rosrun tf tf_echo base_link front_optical_link
rosrun tf tf_echo base_link gimbal_yaw_link
rosrun tf view_frames
```

需要确认：

1. `frame_id` 与 TF 树中的名称完全一致；
2. 固定安装外参的平移和旋转方向正确；
3. 变换时间覆盖消息时间；
4. 角度使用弧度还是度；
5. 位置使用米还是毫米；
6. 正方向符合控制器定义。

不要通过在代码里临时加负号来掩盖坐标系问题。应先画出父子坐标系和旋转方向，再修正 URDF、标定或接口定义。

## 13. 多机 ROS 网络与时间同步

当相机算法和控制程序运行在不同电脑时，节点能否互相发现取决于 Master 地址、各自公布的 IP 和网络可达性。

假设控制上位机为 `192.168.10.10`，视觉电脑为 `192.168.10.11`：

```bash
# 两台电脑都指向控制上位机上的 Master
export ROS_MASTER_URI=http://192.168.10.10:11311

# 控制上位机
export ROS_IP=192.168.10.10

# 视觉电脑
export ROS_IP=192.168.10.11
```

排查顺序：

```bash
hostname -I
ping -c 3 192.168.10.10
ping -c 3 192.168.10.11
echo "$ROS_MASTER_URI"
echo "$ROS_IP"
rosnode list
rosnode info /vision_node
```

`rosnode list` 能看到节点但 Topic 连不上时，要检查节点公布的地址能否从另一台电脑访问。不要把 `ROS_IP` 设置成 `127.0.0.1` 或不可达的无线网卡地址。

多机融合还要求时钟接近一致。检查：

```bash
timedatectl status
chronyc tracking
chronyc sources
```

若系统没有安装 Chrony，后两条命令会提示不存在，应先按队内统一方案配置时间同步。赛场无线环境可能与实验室不同，关键控制链应优先使用经过验证且符合比赛要求的通信方式，并准备断网降级策略。

## 14. 参数、命名空间与多机器人配置

同一个节点可能用于步兵、英雄或哨兵，但每台机器人的轮距、相机外参和控制增益不同。不要在源码中到处写死参数。

```yaml
robot_name: infantry_3
camera:
  exposure: 3000
  gain: 8.0
chassis:
  max_vx: 1.5
  cmd_timeout: 0.10
gimbal:
  yaw_kp: 12.0
```

需要区分：

- 通用默认值：放在包内版本管理；
- 机器人专属标定：按机器人编号保存；
- 比赛现场临时调整：必须留下修改记录；
- 密钥和账号信息：不能提交到仓库。

ROS1 参数修改后不会自动通知所有节点。有的程序只在启动时读取一次，因此执行 `rosparam set` 后必须确认节点是否会重新读取，不能看到参数服务器变了就认为控制器已经生效。

## 15. 启动、重启与命令仲裁

整车上可能同时存在遥控、导航、视觉和调试工具，它们不能直接无序地向同一个执行端发送命令。需要明确优先级和唯一输出口。

```text
急停/安全锁
  > 遥控人工接管
  > 自动控制
  > 调试指令
```

`twist_mux` 一类工具可以按优先级选择速度来源，但它只执行配置好的规则，不能替代安全设计。启动时还要考虑：

- 硬件未就绪时控制器是否会误发指令；
- 节点启动顺序不同是否仍能恢复连接；
- `respawn` 是否造成崩溃循环和日志刷屏；
- 同名节点是否互相顶掉；
- 重启后旧参数和旧 latched 消息是否仍然有效。

launch 中可以为关键节点启用重启，但必须设置合理延时并保留退出原因。一个持续崩溃的节点反复自动重启，并不等于系统恢复正常。

## 16. 现场记录与 rosbag 策略

全量录制方便，但高分辨率图像和点云会快速占满磁盘并抢占 I/O。更稳妥的做法是先定义关键证据话题：

```bash
mkdir -p ~/bags
cd ~/bags
rosbag record --split --size=1024 -O match_debug \
  /front/color/image_raw \
  /front/color/camera_info \
  /target \
  /gimbal_cmd \
  /imu0 \
  /smb_velocity_controller/odom
```

`--split --size=1024` 按约 1024 MB 分包，避免一个文件无限增大。录制期间继续监控：

```bash
df -h ~/bags
rostopic hz /target
rostopic delay /target
top
```

bag 必须配合 Git Commit、参数快照、固件和协议版本才能复现现场。只有 bag 而不知道当时运行哪版代码，复盘价值会大幅下降。

## 17. 赛前整车检查清单

### 软件与版本

- 当前 Git Commit 或标签已经记录；
- 工作空间来自预期路径，没有运行旧节点；
- 参数、相机标定、下位机固件和协议版本一致；
- 所有节点能正常退出，异常时有回退版本。

### 通信与数据

- 相机、IMU、电机和裁判系统数据频率正常；
- 消息时间戳没有明显落后或跳变；
- TF 树连通，外参与正方向已做小范围动作验证；
- 多机网络和时间同步在目标网络环境中测试过。

### 安全与限制

- 遥控、视觉、裁判系统和电机反馈超时均已测试；
- 急停、人工接管和命令优先级明确；
- 速度、加速度、功率与热量限制生效；
- 断网、节点崩溃和设备重插后行为可预测。

### 记录与资源

- 日志和关键 bag 能正常保存；
- 磁盘、CPU、内存和温度有余量；
- 长时间运行不会持续涨内存或堆积旧消息；
- 赛后能根据时间、代码、参数和数据复现问题。

RoboMaster 实战中最危险的不是某一条命令不会，而是系统在异常情况下没有明确行为。ROS 学习的最终目标，是让每条数据从哪里来、是否可信、到哪里去、失效后怎么办，都可以被检查和解释。
