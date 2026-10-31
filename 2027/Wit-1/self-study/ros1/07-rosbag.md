# 07：rosbag 录制与回放

## 学了什么

rosbag 可以将 ROS 话题消息保存到 `.bag` 文件中，之后按录制时的时间间隔重新发布。文件保存话题名、消息类型、内容和记录时间。

视觉项目可以录下一段相机图像，再用同一段数据反复验证算法。本次先用海龟的位置和速度指令练习。

| 操作 | rosbag 的作用 |
| --- | --- |
| `rosbag record` | 订阅话题，将收到的消息写入文件 |
| `rosbag info` | 查看文件中的话题、类型、消息数量和时长 |
| `rosbag play` | 读取文件，将记录的消息重新发布 |

## 做过的实验：录制位置和速度指令

所有命令在容器开发终端运行。保持 `lesson.launch` 和海龟窗口运行，先结束上一轮尚未停止的运动控制程序。

在一个开发终端开始录制：

```bash
mkdir -p ~/catkin_ws/records
cd ~/catkin_ws/records
rosbag record -O turtle_demo.bag /turtle1/pose /turtle1/cmd_vel
```

`-O` 指定输出文件名。看到 `Recording to ...` 后，在另一个开发终端复位并让海龟画圆：

```bash
rosservice call /reset
rosrun turtle_control turtle_motion _linear:=1 _angular:=1 _duration:=6.3
```

等运动结束后，在录制终端按 Ctrl+C，等待文件保存完成，再查看：

```bash
rosbag info turtle_demo.bag
```

文件在容器中的路径为：

```text
/home/rosdev/catkin_ws/records/turtle_demo.bag
```

对应宿主机的 `DynamicX/catkin_ws/records/turtle_demo.bag`。录制数据保存在本地开发目录，学习笔记中保留命令和结果。

## 实际录制结果

| 项目 | 本次结果 |
| --- | --- |
| 文件格式版本 | 2.0 |
| 时长 | 54.2 秒 |
| 大小 | 276.9 KB |
| 消息总数 | 3504 |
| 压缩 | 无 |

`version: 2.0` 是 bag 文件格式版本，本次使用的开发环境仍是 ROS 1 Noetic。

| 话题 | 消息数量 | 消息类型 |
| --- | --- | --- |
| `/turtle1/cmd_vel` | 130 | `geometry_msgs/Twist` |
| `/turtle1/pose` | 3374 | `turtlesim/Pose` |

54.2 秒是整个录制时长，包括运动前后的静止时间。位置消息在海龟停着时也持续发布；速度指令主要由控制程序在运动期间和结束停车时发送，所以两种消息的数量不同。

## 做过的实验：用原来的 listener 接收回放数据

保持 `lesson.launch` 运行，使当前 ROS master 可用。在另一个开发终端启动专门接收回放位置的实例：

```bash
rosrun turtle_control turtle_pose_listener \
  __name:=replay_listener \
  turtle1/pose:=/replay/turtle1/pose
```

`__name:=replay_listener` 设置这次运行的节点名；后面的重映射让它订阅 `/replay/turtle1/pose`。回放开始前，它只显示正在监听并等待消息。

在另一个开发终端回放：

```bash
rosbag play ~/catkin_ws/records/turtle_demo.bag \
  /turtle1/pose:=/replay/turtle1/pose \
  --topics /turtle1/pose
```

`--topics /turtle1/pose` 选择文件中的原始位置话题；重映射将发布目标改为 `/replay/turtle1/pose`。本次已确认原来的 listener 能重新输出历史位置。

```text
turtle_demo.bag
    → rosbag 回放节点读取并发布
    → /replay/turtle1/pose
    → replay_listener 接收并打印
```

## 回放速度、暂停和继续

控制回放时，可先启动前面的 `replay_listener`，再使用：

```bash
rosbag play --pause -r 2 ~/catkin_ws/records/turtle_demo.bag \
  /turtle1/pose:=/replay/turtle1/pose \
  --topics /turtle1/pose
```

| 操作 | 含义 |
| --- | --- |
| `--pause` | 启动后先暂停，准备好接收节点再开始 |
| 在回放终端按空格 | 在暂停和继续之间切换 |
| `-r 2` | 两倍速回放，消息之间的播放间隔约减半 |
| `-r 0.5` | 半速回放，方便观察较慢的处理过程 |
| Ctrl+C | 结束本次回放 |

按空格需要将键盘焦点放在运行 rosbag play 的终端。暂停期间，回放停止发布后续消息，listener 不会持续收到新的位置。

54.2 秒的记录采用两倍速时，连续播放约需 27 秒，不包含手动暂停时间。改变播放速度不会修改 bag 文件中的原始消息内容。

## 做过的实验：观察回放时钟

在回放终端结束上一轮播放，再运行：

```bash
rosbag play --clock --hz 1 --pause -r 2 \
  ~/catkin_ws/records/turtle_demo.bag \
  /turtle1/pose:=/replay/turtle1/pose \
  --topics /turtle1/pose
```

另一个开发终端结束 listener，改为查看时钟话题：

```bash
rostopic echo /clock
```

`--clock` 让 rosbag 发布当前回放时间；`--hz 1` 将时钟话题的发布频率设为每秒一次，便于查看，不改变位置话题的播放速度。时钟消息中的 `secs` 是秒，`nsecs` 是不足一秒的纳秒部分。

本次已观察到：播放时 `secs` 前进，暂停时数值停止前进。暂停期间 rosbag 仍可重复发布相同的时钟值；时钟消息持续出现，与时间数值是否前进是两回事。

## 程序采用哪一种时间

ROS C++ 程序通常通过 `ros::Time::now()` 获取 ROS 时间。节点启动时的 `/use_sim_time` 配置决定它使用哪种时间：

| `/use_sim_time` | `ros::Time::now()` 的时间来源 |
| --- | --- |
| 未设置或为 `false` | 电脑的系统时间 |
| 为 `true` | `/clock` 提供的仿真或回放时间 |

`rosbag play --clock` 负责提供时钟；它本身不会替接收节点启用仿真时间。ROS C++ 节点在启动时读取这个参数，切换实验时应先配置参数，再启动相关节点。

启用方式为：

```bash
rosparam set /use_sim_time true
```

先设置参数，再启动需要采用仿真时间的 C++ 节点，同时用 `rosbag play --clock` 提供时钟。这里说明的是配置方法，本次交互实验确认的是 `/clock` 的变化，尚未记录节点切换时间来源后的实测结果。

以使用 `ros::Time::now()` 计算时间间隔的节点为例，假设暂停 bag，现实中等待 10 秒再继续：

| 节点采用的时间 | 暂停期间的计时 |
| --- | --- |
| 系统时间 | 继续前进，会把现实等待的这 10 秒计入间隔 |
| `/clock` 提供的回放时间 | 随回放暂停，不会因现实等待额外增加这 10 秒 |

有 `/clock` 话题，与节点采用它作为 ROS 时间来源，是两个条件。回放与 TF 配合使用时，相关节点需要使用一致的时间轴，并按数据的采集时间查询坐标变换。

## 观察与解释

listener 的 C++ 代码没有变化。它仍然按 `turtlesim/Pose` 类型处理消息，只是通过重映射更换了消息来源。回放节点负责发布，listener 负责接收。

真实海龟的位置继续使用 `/turtle1/pose`，回放数据使用 `/replay/turtle1/pose`，两种来源因此可以分别观察。

本次只回放位置数据，海龟窗口保持当前状态，listener 输出记录中的历史坐标。要驱动海龟运动，需要向它订阅的速度话题发送指令；位置消息本身不会控制运动。

默认播放时长约 54 秒，静止时段也按记录回放，因此有些阶段会连续打印相同坐标。回放结束后，listener 仍保持运行，继续等待新消息。
