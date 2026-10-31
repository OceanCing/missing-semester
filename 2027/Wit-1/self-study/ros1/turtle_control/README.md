# ROS 1 Noetic 示例包：turtle_control

这里保存本次自学使用的五个 C++ 示例及自动验证脚本。概念和实验解释见上一级的学习笔记；本文件主要说明怎样编译和运行。

## 环境与编译

使用 Ubuntu 20.04、ROS 1 Noetic 和 catkin_tools。依赖写在 `package.xml` 中，包括 roscpp、geometry_msgs、turtlesim、tf2、tf2_ros 和 tf2_geometry_msgs。Python 验证脚本还使用 rospy、std_srvs 等标准 ROS 包。

本机 Docker 开发环境已将此目录接入 `/home/rosdev/catkin_ws/src/turtle_control`。打开“ROS1 开发终端”后，编译方式是：

```bash
cd ~/catkin_ws
catkin build turtle_control -j2
source devel/setup.bash
```

在其他已有 Noetic、catkin_tools 和上述依赖的机器上，可把本包放进自己的 catkin 工作空间 src 目录。也可创建链接，下面的 `/path/to/missing-semester` 需换成实际克隆位置；目标工作空间中不应已经存在同名包：

```bash
mkdir -p ~/catkin_ws/src
ln -s /path/to/missing-semester/2027/Wit-1/self-study/ros1/turtle_control ~/catkin_ws/src/turtle_control
source /opt/ros/noetic/setup.bash
cd ~/catkin_ws
catkin build turtle_control -j2
source devel/setup.bash
```

## 示例与文件

| 程序 | 功能 | 源码 |
| --- | --- | --- |
| turtle_motion | 发布速度，运行指定时间后停止 | [src/turtle_motion.cpp](src/turtle_motion.cpp) |
| turtle_pose_listener | 订阅 Pose，在回调里打印位置 | [src/turtle_pose_listener.cpp](src/turtle_pose_listener.cpp) |
| turtle_go_to_x | 根据位置误差减速并停车 | [src/turtle_go_to_x.cpp](src/turtle_go_to_x.cpp) |
| point_to_world | 使用 TF2 转换目标点 | [src/point_to_world.cpp](src/point_to_world.cpp) |
| turtle_tf_broadcaster | 将海龟实时位置与朝向发布为动态 TF | [src/turtle_tf_broadcaster.cpp](src/turtle_tf_broadcaster.cpp) |

`launch/` 存放启动文件，`config/` 保存参数配置，`rviz/` 存放可视化配置，`urdf/` 描述模型部件和连接关系，`scripts/` 存放验证脚本。`CMakeLists.txt` 和 `package.xml` 分别管理构建目标和依赖。

## 运行方法

### 启动海龟与第一轮运动

```bash
roslaunch turtle_control lesson.launch
```

它在需要时启动 ROS master，并启动海龟与控制节点。默认前进 3 秒后停止，海龟窗口保留。要复用窗口，让这个 roslaunch 保持运行。

在另一个开发终端中复位并重跑控制：

```bash
rosservice call /reset
rosrun turtle_control turtle_motion _linear:=1 _angular:=1 _duration:=6.3
```

### 通过 YAML 文件集中配置运动

保持 `lesson.launch` 和海龟窗口运行，确保上一轮运动已结束，在另一个容器开发终端执行：

```bash
roslaunch turtle_control configured_motion.launch
```

它复用已有控制程序，将 [motion.yaml](config/motion.yaml) 中的 `linear: 0.5`、`angular: 1.0`、`duration: 6.3` 加载到控制节点的私有参数空间。预期海龟以这些参数画接近一圈的小圆，随后停止。

在原来的 ROS master 仍运行时，可以查询加载后的值：

```bash
rosparam get /turtle_motion
```

YAML 保存配置，launch 负责加载，C++ 程序启动时读取参数并生成速度指令。在当前源码工作空间中，仅修改 YAML 无需重新编译；本程序只启动时读取一次，因此修改配置文件后要重新启动控制节点才能使用新值。

### 接收位置

```bash
rosrun turtle_control turtle_pose_listener
```

按 Ctrl+C 只结束此接收节点。查看第二只海龟时，先结束旧 listener，再按[重映射笔记](../04-remapping-feedback.md)运行。

### 在不同命名空间运行同一个程序

保持 `lesson.launch` 和海龟窗口运行，在另一个开发终端执行：

```bash
roslaunch turtle_control listeners.launch
```

它复用已有的 `turtle_pose_listener` 程序，启动 `/observer_a/pose_listener` 和 `/observer_b/pose_listener` 两个节点。两个节点都通过重映射订阅现有的 `/turtle1/pose`，因此预期打印同一只海龟的位置。新增这个 launch 文件后，在当前源码工作空间中运行无需重新编译。

可在第三个终端检查：

```bash
rosnode list
rostopic info /turtle1/pose
```

节点列表应包含上述两个完整名称，话题的 Subscribers 中也应包含它们。在运行 `listeners.launch` 的终端按 Ctrl+C 可一起结束这两个接收节点。

### 根据位置停车

等上一轮运动结束后运行：

```bash
rosservice call /reset
rosrun turtle_control turtle_go_to_x _target_x:=8 _max_speed:=1
```

复位使海龟面向右方，本例只控制 x。距离目标不超过 0.05 时停车；位置无更新、朝向不适合本课或超时也会停止。max_speed 是速度上限，靠近目标会减速。

### 固定坐标变换

这是独立的固定坐标计算，和海龟当前的实际位置不联动：

```bash
roslaunch turtle_control frames.launch
```

默认将机器人坐标 `(2, 0, 0)` 转成世界坐标 `(5, 5, 0)`。结束本次 frames.launch 后运行 `roslaunch turtle_control frames.launch yaw:=0`，结果应为 `(7, 3, 0)`。

### 海龟的动态坐标变换

保持 `lesson.launch` 和海龟窗口运行，确保上一轮运动已经结束，然后在另一个开发终端运行：

```bash
roslaunch turtle_control dynamic_frames.launch
```

再开一个开发终端观察：

```bash
rosrun tf tf_echo turtle_world turtle_body
```

`turtle_world` 对应海龟画面的固定坐标系，`turtle_body` 跟随海龟。输出的 Translation 是海龟原点在世界中的位置，RPY 的第三项 yaw 是朝向角。节点订阅 `/turtle1/pose`，收到新位置后通过 `/tf` 发布这两个坐标系的关系。

在另一个终端设置两个已知位姿，观察结果：

```bash
rosservice call /turtle1/teleport_absolute "{x: 5.0, y: 3.0, theta: 0.0}"
rosservice call /turtle1/teleport_absolute "{x: 5.0, y: 3.0, theta: 1.5708}"
```

分别运行、观察后再执行下一条。两次 Translation 均应接近 `[5, 3, 0]`，yaw 从 `0` 变成约 `1.571` 弧度（90°）。在第二种朝向下运行 `rosrun turtle_control turtle_motion _linear:=1 _angular:=0 _duration:=2`，Translation 的 y 应随运动增大。

`turtlesim/Pose` 没有采集时间戳，本示例用回调时的 `ros::Time::now()` 标记实时变换。处理真实传感器时应使用对应的采集时间。把旧 bag 位置接入这个节点时，它会按回放时刻标记，不能据此认为恢复了历史 TF 时间。

### 用 RViz 观察坐标系

保持海龟和 `dynamic_frames.launch` 运行，在另一个容器开发终端执行：

```bash
rosrun rviz rviz -d "$(rospack find turtle_control)/rviz/turtle_frames.rviz"
```

`-d` 加载本包的显示配置。配置使用 `turtle_world` 作为 Fixed Frame，显示网格和 TF 坐标轴，并设置为俯视视角。在当前源码工作空间中，使用这个配置无需重新编译。

坐标轴颜色为 x 红、y 绿、z 蓝。预期能看到固定的 `turtle_world` 和跟随海龟的 `turtle_body`。海龟位于 `(5, 3)`、面向上时，turtle_body 的原点在世界 `(5, 3)`，它的红色 x 轴沿世界正 y 方向。

RViz 订阅坐标变换并绘图；海龟的运动和位置数据仍由 turtlesim 模拟器产生。

### 用 URDF 描述底盘和相机

保持 `lesson.launch`、`dynamic_frames.launch` 运行，关闭前一个仅显示坐标轴的 RViz 窗口，在容器开发终端执行：

```bash
roslaunch turtle_control robot_model.launch
```

这次新增的是 URDF、launch 和显示配置，在当前源码工作空间中使用无需重新编译 C++。启动文件将 [turtle_camera.urdf](urdf/turtle_camera.urdf) 载入 `robot_description` 参数，启动 `robot_state_publisher` 和带 RobotModel 显示的 RViz。

模型包含蓝色底盘 `turtle_body`、橙色相机 `camera_link`，通过 `camera_mount` 固定连接：

```text
turtle_world ──动态 TF──→ turtle_body ──固定关系──→ camera_link
```

动态 TF 由海龟位置广播节点提供，固定关系由 `robot_state_publisher` 根据 URDF 发布。URDF 中 joint 的 `origin xyz="0.4 0 0.3"` 表示相机坐标系原点在底盘坐标系中的位置：向前 0.4 m、向上 0.3 m；`rpy="0 0 0"` 表示两者坐标轴方向一致。这是教学模型的示例安装位置，真实机器人需要根据测量或标定填写。

`link` 描述刚体部件，`joint` 描述父、子部件之间的连接。`fixed` 表示相机相对于底盘的关系不变，底盘运动时，相机在世界中的位置和朝向会随之变化。本例只包含固定关节，因而不需要额外发布关节角度。

默认视角面向世界 `(5, 5)` 附近，与上一轮运动结束的位置对应。预期看到底盘和前上方的相机；在 RViz 左侧展开 TF 的 Tree，也可以查看坐标系树。

## 自动验证

先构建并加载工作空间，在容器或其他满足依赖的 Noetic 开发环境中运行：

```bash
lesson_package="$(rospack find turtle_control)"
python3 "$lesson_package/scripts/check_motion.py"
python3 "$lesson_package/scripts/check_pose_listener.py"
python3 "$lesson_package/scripts/check_go_to_x.py"
python3 "$lesson_package/scripts/check_tf2_points.py"
python3 "$lesson_package/scripts/check_dynamic_tf.py"
```

各脚本会使用独立临时 ROS master，运行真实 ROS 程序进行检查，结束后清理自己的进程。默认无窗口，不更改现有海龟的位置。它们检查三种运动与停止、位置回调、反馈停车、静态 TF2 点转换，以及实时位置到动态 TF 的转换和时间戳更新。具体实现见各脚本。

## 学习记录

- [环境、工作空间与构建](../01-environment-build.md)
- [话题与回调](../02-topics-callbacks.md)
- [参数与服务](../03-parameters-services.md)
- [重映射与反馈控制](../04-remapping-feedback.md)
- [坐标系与 TF2](../05-coordinates-tf2.md)
- [launch、节点命名与命名空间](../06-launch-namespaces.md)
- [rosbag 录制与回放](../07-rosbag.md)
- [RViz 显示与操作](../08-rviz.md)
- [URDF 与 TF 坐标树](../09-urdf-tf-tree.md)
- [话题连接、类型与频率排查](../10-communication-debug.md)
- [YAML 参数配置](../11-yaml-config.md)
