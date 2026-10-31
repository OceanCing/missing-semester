# 05：自身坐标、世界坐标与 TF2

## 学了什么

一个坐标或速度，必须连同“相对于哪个坐标系”一起理解。

在海龟程序里：

| 数据 | 参照 |
| --- | --- |
| `linear.x` | 沿海龟自身前方的速度 |
| `pose.x`、`pose.y` | 在画面固定坐标系中的位置；x 向右、y 向上 |
| `pose.theta` | 海龟相对于固定坐标方向的朝向角，单位弧度 |

所以 `linear.x=1` 不是“移动到世界坐标 x=1”，也不总意味着世界 x 增大。海龟朝上时，自身前方对应世界的正 y 方向。

## 做过的实验：改变朝向，仍然使用同一个前进指令

先确保已经创建 `turtle2`，然后执行：

```bash
rosservice call /turtle2/teleport_absolute "{x: 5.0, y: 3.0, theta: 1.5708}"
rosrun turtle_control turtle_motion _linear:=1 _angular:=0 _duration:=3 turtle1/cmd_vel:=turtle2/cmd_vel
```

用自己的接收程序看它的位置：

```bash
rosrun turtle_control turtle_pose_listener turtle1/pose:=turtle2/pose
```

若旧 listener 还在运行，先在其终端按 Ctrl+C。新实例使用相同节点名，不需要让两个同名实例同时运行。

实验应观察到：海龟朝上前进，世界 x 基本不变，世界 y 增大。前进方向与海龟自身坐标系有关。

## 观察与解释：还要考虑原点的位置

例如：

- 机器人原点在世界坐标 `(5, 3)`。
- 目标在机器人自身正前方 2 个单位，即自身坐标 `(2, 0)`。
- 机器人朝上时，目标的世界坐标是 `(5, 5)`。

先把自身方向旋转到世界方向，再加上机器人原点在世界中的位置。若机器人朝右，目标则在 `(7, 3)`。

平面关系可以写成：

```text
世界坐标 x = 原点世界 x + cos(theta) × 自身 x - sin(theta) × 自身 y
世界坐标 y = 原点世界 y + sin(theta) × 自身 x + cos(theta) × 自身 y
```

本次重点是理解“旋转 + 平移”，没有推导全部三维旋转表示。

## 已准备并自动验证的 TF2 实验

TF2 可以管理坐标系关系并据此转换数据。它并不自己猜出相机安装位置或机器人朝向；这些关系要由模型、标定、定位或其他程序提供。

这是一个独立的固定坐标练习，不跟随海龟画面中的位置：

- `lesson_world`：世界坐标系。
- `lesson_robot`：机器人坐标系，原点在世界 `(5, 3, 0)`。
- 默认朝向：绕 z 轴逆时针 90°。
- 输入点：机器人坐标 `(2, 0, 0)`。

在 ROS1 开发终端运行：

```bash
roslaunch turtle_control frames.launch
```

重点找这两行：

```text
Robot point: frame=lesson_robot x=2.000 y=0.000 z=0.000
World point: frame=lesson_world x=5.000 y=5.000 z=0.000
```

启动文件运行一个固定变换发布程序，以及一个转换点的 C++ 节点。点转换完成后退出，固定关系发布节点继续运行。关系通过 `/tf_static` 提供。

在运行 `frames.launch` 的终端按 Ctrl+C，结束本次 TF 练习，再改变朝向：

```bash
roslaunch turtle_control frames.launch yaw:=0
```

输入点仍是自身坐标 `(2, 0, 0)`，世界结果应变为 `(7, 3, 0)`。`yaw` 的单位是弧度，表示绕 z 轴的朝向角。

保持某次 frames.launch 运行时，还可以在另一个终端换一个输入点：

```bash
rosrun turtle_control point_to_world _x:=0 _y:=1 _z:=0
```

## 关键代码

```cpp
geometry_msgs::PointStamped point;
point.header.frame_id = "lesson_robot";
point.header.stamp = ros::Time(0);
```

`PointStamped` 同时携带坐标、坐标系名称和时间。这里的 frame_id 是坐标系名称，不是节点名，也不是话题名。程序默认通过参数读入点 `(2, 0, 0)`。

```cpp
const auto world_point = buffer.transform(point, "lesson_world", ros::Duration(5.0));
```

输入坐标系来自 point，输出坐标系是 lesson_world；本例获取最新可用变换，最多等待 5 秒。`TransformListener` 把收到的关系放进 buffer，并在内部线程处理 TF 消息，因此本例主线程无需再写 `ros::spin()`。

同一目标在不同坐标系里可以有不同数值。相反，只看到 `(2, 0, 0)` 而不知道它属于哪个坐标系，就无法确定目标在世界中的位置。

## 动态 TF：跟随海龟的位置和朝向

前面的 `frames.launch` 使用固定的位置和朝向。新增的 [dynamic_frames.launch](turtle_control/launch/dynamic_frames.launch) 启动 [turtle_tf_broadcaster](turtle_control/src/turtle_tf_broadcaster.cpp)，订阅海龟实时位置，再发布动态变换。

| 坐标系 | 含义 |
| --- | --- |
| `turtle_world` | 海龟画面的固定坐标系 |
| `turtle_body` | 原点和坐标轴跟随海龟移动、旋转的自身坐标系 |

```text
海龟发布 /turtle1/pose
    → turtle_tf_broadcaster 处理位置回调
    → /tf 发布 turtle_world 与 turtle_body 的关系
    → TF 查看工具或其他程序查询变换
```

| 关系 | 本例使用的发布方式 |
| --- | --- |
| 固定的 `lesson_world` 与 `lesson_robot` 关系 | `/tf_static` |
| 跟随海龟位姿更新的 `turtle_world` 与 `turtle_body` 关系 | `/tf` |

### 交互实验的操作与预期结果

示例已完成编译和自动检查。保持海龟及 `lesson.launch` 运行，在另一个开发终端启动：

```bash
roslaunch turtle_control dynamic_frames.launch
```

再开一个开发终端观察：

```bash
rosrun tf tf_echo turtle_world turtle_body
```

这条命令输出海龟自身坐标系在世界中的位姿，也就是把自身坐标表达成世界坐标时使用的变换。重点看：

- `Translation`：海龟原点在世界中的位置。
- `RPY (radian)`：旋转角度；第三项 yaw 对应本例的平面朝向角。

确保控制程序已经停止，在一个空闲开发终端分别设置两个位姿，每执行一条就观察一次输出：

```bash
rosservice call /turtle1/teleport_absolute "{x: 5.0, y: 3.0, theta: 0.0}"
rosservice call /turtle1/teleport_absolute "{x: 5.0, y: 3.0, theta: 1.5708}"
```

| 设置 | 预期 Translation | 预期 RPY（弧度） |
| --- | --- | --- |
| 位于 `(5, 3)`，面向右 | `[5, 3, 0]` | `[0, 0, 0]` |
| 位于 `(5, 3)`，面向上 | `[5, 3, 0]` | `[0, 0, 1.571]` |

原点位置没有变化，所以 Translation 不变；自身坐标轴转了约 90°，所以 yaw 变化。这个表记录的是交互实验的预期值。

第二次瞬移后的实际输出为：

```text
- Translation: [5.000, 3.000, 0.000]
- Rotation: in Quaternion [0.000, 0.000, 0.707, 0.707]
            in RPY (radian) [0.000, -0.000, 1.571]
            in RPY (degree) [0.000, -0.000, 90.000]
```

实际结果与预期相符：海龟原点在世界 `(5, 3, 0)`，绕 z 轴旋转约 90°。Quaternion 和 RPY 是同一朝向的不同表示；输出中的 `-0.000` 可在本例显示精度下视为 0。

### 关键代码与时间标记

位置回调将 `pose.x`、`pose.y` 填入平移，将 `pose.theta` 转成绕 z 轴的旋转，再发布：

```cpp
transform.header.stamp = ros::Time::now();
transform.header.frame_id = "turtle_world";
transform.child_frame_id = "turtle_body";
transform.transform.translation.x = pose->x;
transform.transform.translation.y = pose->y;

tf2::Quaternion rotation;
rotation.setRPY(0.0, 0.0, pose->theta);
transform.transform.rotation = tf2::toMsg(rotation);
broadcaster_.sendTransform(transform);
```

`frame_id` 和 `child_frame_id` 是坐标系名称，`/tf` 是传输变换的话题名，两者用途不同。

`turtlesim/Pose` 没有 Header 和采集时间戳，因此这个实时示例用回调时的当前 ROS 时间标记变换。真实传感器数据应结合采集时间处理；将旧 bag 的位置接入本节点，会按回放时刻标记，不能据此认为恢复了历史 TF 时间。

写入 `ros::Time::now()` 是标记当前这条变换的时间；查询时使用 `ros::Time(0)` 则表示请求最新可用变换。

### 静止时的更新与视觉数据的时间

实际观察到，海龟保持静止时，`tf_echo` 的 `At time` 仍在增大。海龟持续发布位置，广播节点每次处理位置回调都会生成带新时间戳的变换，因此位置和朝向的数值可以保持不变，而对应的时间继续更新。

时间戳用于说明数据对应哪个时刻。图像消息 `sensor_msgs/Image` 的 `header.stamp` 应记录图像采集时间；处理完成的时间可能更晚。

例如，机器人原点保持在世界 `(5, 3)`，拍摄时面向右，处理图像时已经转为面向上。假设从该图像解算出的目标点，在拍摄时的机器人自身坐标系中为 `(2, 0)`：

| 使用的变换 | 得到的世界坐标 | 含义 |
| --- | --- | --- |
| 拍摄时面向右的变换 | `(7, 3)` | 正确对应这帧图像 |
| 误用处理时面向上的变换 | `(5, 5)` | 将旧数据配上了新朝向，结果错误 |

因此，转换这帧图像解算出的点时，应使用与图像采集时刻匹配的 TF。`ros::Time(0)` 请求最新可用变换，适合当前示例中的最新位姿观察；对于带采集时间的传感器数据，需要按数据时间查询。

## 按时间查询与常见报错

视觉节点收到图像后，可以根据图像消息携带的坐标系和采集时间，查询对应的变换。下面是接口用法示意，不是完整的可运行节点：

```cpp
const auto transform = buffer.lookupTransform(
    "turtle_world",           // 目标坐标系
    image->header.frame_id,    // 图像所属的相机坐标系
    image->header.stamp,       // 图像采集时刻
    ros::Duration(0.2));       // 等待所需 TF 的超时时长
```

这段代码查询的是：在图像采集时刻，将相机坐标表达成世界坐标所需的变换。实际程序还要建立 TF listener，并处理查询失败的异常。等待参数不会将图像时间改为当前时间。

动态 TF 的 buffer 会保存一段时间的历史变换。查询时间可以落在已有样本之间，由 TF2 在关系允许时插值；不要求每张图像都恰好匹配一条相同时间戳的 TF。

假设某条动态关系在 buffer 中有覆盖 `10.0` 至 `10.5` 秒的有效记录：

| 查询 | 含义 |
| --- | --- |
| `ros::Time(0)` | 请求最新可用变换；不是查询零秒的历史记录 |
| `10.2` 秒 | 位于已有时间范围内，可以在关系连续且数据有效时查询 |
| `10.8` 秒 | 晚于最新记录，可能报 `extrapolation into the future` |
| `9.8` 秒 | 早于最早记录，可能报 `extrapolation into the past` |

报错中的“未来”和“过去”是相对于 buffer 的可用时间范围。图像时间比最新 TF 晚时，可能是 TF 传输或计算还没跟上；图像时间比最早 TF 更早时，可能是历史记录已被清理、listener 启动较晚，或两边使用的时间来源不一致。

| 常见异常 | 排查重点 |
| --- | --- |
| `LookupException` | 坐标系名称是否正确，listener 是否已经收到这个坐标系 |
| `ConnectivityException` | 两个坐标系之间是否缺少连接，整条 TF 链是否完整 |
| `ExtrapolationException` | 请求时间与报错中的最新、最早时间，以及各节点是否使用同一时间轴 |

有完整的 TF 链，也可能缺少所需时刻的动态变换。对于带采集时间的图像，不能只为消除报错就改成 `ros::Time(0)`；机器人移动时，这会改变坐标转换所依据的时刻。静态安装关系则不按动态历史范围处理。

## 参考资料

ROS 1 Noetic 官方实现参考：

- [固定变换发布工具](https://github.com/ros/geometry2/blob/noetic-devel/tf2_ros/src/static_transform_broadcaster_program.cpp)
- [TF2 Buffer 变换接口](https://github.com/ros/geometry2/blob/noetic-devel/tf2_ros/include/tf2_ros/buffer_interface.h)
- [动态变换发布接口](https://github.com/ros/geometry2/blob/noetic-devel/tf2_ros/include/tf2_ros/transform_broadcaster.h)
- [TF 查看工具](https://github.com/ros/geometry/blob/noetic-devel/tf/src/tf_echo.cpp)
- [图像消息及其采集时间定义](https://github.com/ros/common_msgs/blob/noetic-devel/sensor_msgs/msg/Image.msg)
- [按指定时间查询变换的接口](https://github.com/ros/geometry2/blob/noetic-devel/tf2_ros/include/tf2_ros/buffer.h)
- [动态 TF 时间缓存与外推报错](https://github.com/ros/geometry2/blob/noetic-devel/tf2/src/cache.cpp)
