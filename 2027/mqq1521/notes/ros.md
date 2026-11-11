# ROS1 学习笔记——SMB 小车障碍物距离监测

## 一、本次实践要完成什么

本次实践基于 ROS1 Noetic 和 Gazebo 仿真环境，使用 SMB 小车的激光与点云数据，实现一个简单的障碍物距离监测系统。

主要任务是：

1. 订阅激光话题 `/scan`，计算每帧最近的障碍物距离；
2. 订阅点云话题 `/rslidar_points`，统计点云中的点的数量；
3. 使用自定义消息发布最近距离；
4. 使用另一个节点接收距离并判断是否报警；
5. 支持通过服务修改报警距离；
6. 支持通过 dynamic_reconfigure 在线开关报警；
7. 使用 YAML、launch、RViz 和 rosbag 完成配置、启动、显示与离线回放。

整体数据流如下：

```text
Gazebo 中的雷达
        │
        ▼
/rslidar_points
        │
        ├──► scan_processor：统计点云数量
        │
        ▼
pointcloud_to_laserscan
        │
        ▼
      /scan
        │
        ▼
scan_processor：过滤无效数据并计算最近距离
        │
        ▼
/homework/obstacle
        │
        ▼
obstacle_monitor：显示距离、判断是否报警
```

RViz 同时订阅 `/scan` 和 `/rslidar_points`，用于显示二维激光和三维点云。

---

## 二、ROS 中几个基本概念

### 2.1 节点 Node

节点是 ROS 中一个独立运行的功能模块。

本次实践有两个主要节点：

- `scan_processor`：处理传感器数据并发布最近距离；
- `obstacle_monitor`：接收距离、显示数据并进行报警。

把功能拆成两个节点的好处是职责比较清楚：

- 数据处理节点不需要关心怎样报警；
- 监控节点不需要知道 LaserScan 的具体结构；
- 离线回放 bag 时，可以只运行监控节点。

查看节点：

```bash
rosnode list
rosnode info /scan_processor
```

### 2.2 话题 Topic

话题用于节点之间持续传输数据，采用发布/订阅方式。

例如：

```text
scan_processor 发布 /homework/obstacle
obstacle_monitor 订阅 /homework/obstacle
```

发布者不需要知道谁在订阅，订阅者也不需要知道数据由哪个程序发布，只要话题名和消息类型一致即可通信。

常用命令：

```bash
rostopic list
rostopic info /homework/obstacle
rostopic echo /homework/obstacle
rostopic hz /homework/obstacle
```

### 2.3 服务 Service

服务适合“一次请求、一次响应”的操作。

本次实践使用服务修改报警距离：

```text
客户端：请求把报警距离设置为 0.5 米
监控节点：返回修改成功或拒绝原因
```

服务与话题的区别：

| 对比 | 话题 | 服务 |
|---|---|---|
| 通信方式 | 发布/订阅 | 请求/响应 |
| 数据特点 | 持续发送 | 调用一次、响应一次 |
| 典型用途 | 激光、点云、距离 | 修改阈值、查询或切换状态 |

---

## 三、工作空间与软件包

### 3.1 工作空间

本次实践使用 catkin 工作空间：

```text
catkin_ws/
├── src/     源代码
├── build/   编译中间文件
└── devel/   编译结果和环境配置脚本
```

软件包放在：

```text
/root/catkin_ws/src/smb_highlevel_controller
```

这里的 `/root/catkin_ws` 是 Docker 容器内路径，与宿主机挂载的工作空间对应。

编译与环境加载：

```bash
cd /root/catkin_ws
catkin_make
source devel/setup.bash
```

`source devel/setup.bash` 的作用是告诉当前终端：

- 去哪里寻找自己编译的软件包；
- 去哪里寻找生成的消息、服务和节点。

它只对当前终端有效，新开终端后需要重新执行。

### 3.2 软件包目录

本次软件包的主要结构：

```text
smb_highlevel_controller/
├── CMakeLists.txt
├── package.xml
├── src/
│   ├── scan_processor.cpp
│   └── obstacle_monitor.cpp
├── msg/
│   └── ObstacleDistance.msg
├── srv/
│   └── SetWarningDistance.srv
├── cfg/
│   └── ObstacleMonitor.cfg
├── config/
│   └── params.yaml
├── launch/
│   └── homework.launch
├── rviz/
│   └── smb_view.rviz
└── homework_obstacle.bag
```

其中：

- `package.xml`：声明软件包依赖；
- `CMakeLists.txt`：说明消息、服务和节点如何编译；
- `msg/`：自定义消息；
- `srv/`：自定义服务；
- `cfg/`：动态参数配置；
- `launch/`：集中启动仿真和节点。

---

## 四、LaserScan 消息与最近距离计算

### 4.1 LaserScan 的主要字段

```cpp
std_msgs/Header header       // 时间戳和坐标系
float32 angle_min            // 起始角度
float32 angle_max            // 结束角度
float32 angle_increment      // 相邻激光束的角度间隔
float32 time_increment       // 相邻测量之间的时间间隔
float32 scan_time            // 扫描周期
float32 range_min            // 传感器允许的最小有效距离
float32 range_max            // 传感器允许的最大有效距离
float32[] ranges             // 每束激光测得的距离
float32[] intensities        // 测量强度
```

本次最近距离计算主要使用：

- `ranges`；
- `range_min`；
- `range_max`。

### 4.2 为什么不能直接找数组最小值

`ranges` 中可能包含：

- `inf`：没有测到有效障碍物；
- `NaN`：测量值不是有效数字；
- 小于 `range_min` 的值；
- 大于 `range_max` 的值。

如果直接求最小值，可能把 0 或其他无效值当成最近障碍物。

因此需要同时判断：

```cpp
std::isfinite(r)
r >= scan->range_min
r <= scan->range_max
```

核心逻辑：

```cpp
float min_dist = INFINITY;
bool valid = false;

for (const float r : scan->ranges) {
    if (std::isfinite(r) &&r >= scan->range_min &&r <=scan->range_max) {
        valid = true;
        if (r < min_dist) {
            min_dist = r;
        }
    }
}

if (!valid) {
    min_dist = 0.0f;
}
```

这里用 `valid` 表示本帧是否存在有效测量。

整帧没有有效数据时：

```text
valid = false
min_distance = 0
```

此时的 0 不表示“障碍物距离为 0 米”，只是一个约定的填充值。监控节点必须先检查 `valid`，不能直接根据距离字段报警。

---

## 五、PointCloud2 与点云数量

点云消息类型为：

```text
sensor_msgs/PointCloud2
```

本次订阅：

```text
/rslidar_points
```

PointCloud2 将点云数据按照二进制格式存储。一个点通常包含：

- `x`；
- `y`；
- `z`；
- 可能还有 `intensity` 等字段。

代码中使用 `PointCloud2ConstIterator` 遍历点云并统计数量：

```cpp
size_t count = 0;

for (sensor_msgs::PointCloud2ConstIterator<float> it(*cloud, "x");
     it != it.end(); ++it) {
    ++count;
}
```

然后使用日志打印：

```cpp
ROS_INFO_THROTTLE(2.0, "PointCloud points: %zu", count);
```

`ROS_INFO_THROTTLE(2.0, ...)` 表示最多每 2 秒打印一次，避免终端被高频日志占满。

PointCloud2 中的 `width * height` 通常可以表示数据元素数量，但数据中仍可能包含无效点。本次任务只要求统计点的数量，不要求进一步过滤 NaN 点。

---

## 六、自定义消息 ObstacleDistance

消息定义：

```text
float32 min_distance
bool valid
```

字段含义：

- `min_distance`：最近有效障碍物的距离，单位为米；
- `valid`：本帧是否存在有效测量。

定义消息后，需要在 `CMakeLists.txt` 中添加：

```cmake
add_message_files(
  FILES
  ObstacleDistance.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  sensor_msgs
)
```

`package.xml` 中还需要区分：

- `message_generation`：生成消息时需要；
- `message_runtime`：运行时使用生成结果。

编译后，C++ 中通过以下头文件使用：

```cpp
#include <smb_highlevel_controller/ObstacleDistance.h>
```

发布话题为：

```text
/homework/obstacle
```

自定义消息的意义是：监控节点不必重复处理原始激光，只需要接收“距离 + 是否有效”这个简洁结果。

---

## 七、自定义服务 SetWarningDistance

服务定义：

```text
float32 distance
---
bool success
string message
```

其中：

- `distance`：希望设置的新报警距离；
- `success`：是否接受修改；
- `message`：成功或失败说明。

调用示例：

```bash
rosservice call /homework/set_warning_distance "distance: 0.5"
```

以下输入应被拒绝：

- NaN、Inf 等非有限值；
- 0；
- 负数。

拒绝修改时，应保留原来的报警距离。

需要区分两个“成功”：

```cpp
res.success = false;
return true;
```

- `res.success`：本次修改请求是否被接受；
- `return true`：服务回调是否正常执行并返回响应。

因此，即使用户传入了非法距离，服务本身仍然可以正常响应，只是响应中的 `success=false`。

---

## 八、报警逻辑

监控节点订阅：

```text
/homework/obstacle
```

报警条件为：

```cpp
msg->valid &&
msg->min_distance < threshold &&
warning_enabled
```

因此：

- 距离小于阈值：报警；
- 距离等于阈值：不报警；
- 距离大于阈值：不报警；
- 数据无效：不报警；
- `warning_enabled=false`：不报警。

关闭报警时，节点仍然：

- 接收并打印距离；
- 响应修改报警距离的服务；
- 保持 dynamic_reconfigure 可用。

`warning_enabled` 只控制报警开关，不重复管理报警距离。

---

## 九、YAML 与参数服务器

配置文件位于：

```text
config/params.yaml
```

内容示例：

```yaml
scan_processor:
  scan_topic: "/scan"
  cloud_topic: "/rslidar_points"
  queue_size: 10

obstacle_monitor:
  queue_size: 10
  warning_distance: 1.0
```

常用命令：

```bash
rosparam list
rosparam get /obstacle_monitor/warning_distance
rosparam set /obstacle_monitor/warning_distance 0.8
```

### 修改参数后是否立即生效

不一定。

如果节点只在启动时读取一次参数：

```cpp
private_nh.param("warning_distance", threshold, 1.0);
```

那么之后执行 `rosparam set`，正在运行的节点不会自动重新读取，通常需要重启节点。

本次实践中：

- YAML：提供启动时的初始配置；
- 服务：运行时修改报警距离；
- dynamic_reconfigure：运行时修改报警开关。

服务修改阈值不会自动改写 YAML，因此重启后可能恢复为 YAML 中的初始值。

还需要注意：如果 launch 中节点内部又写了同名 `<param>`，可能覆盖前面加载的 YAML 值，使修改 YAML 看起来“不起作用”。

---

## 十、dynamic_reconfigure

动态参数配置位于：

```text
cfg/ObstacleMonitor.cfg
```

本次只添加一个参数：

```python
gen.add(
    "warning_enabled",
    bool_t,
    0,
    "Enable/disable obstacle warning",
    True
)
```

它的作用是允许节点运行时修改参数，并通过回调立即更新程序状态。

打开图形界面：

```bash
rqt_reconfigure
```

也可以使用命令行：

```bash
rosrun dynamic_reconfigure dynparam get /obstacle_monitor

rosrun dynamic_reconfigure dynparam set \
  /obstacle_monitor warning_enabled false

rosrun dynamic_reconfigure dynparam set \
  /obstacle_monitor warning_enabled true
```

如果容器中 rqt 图形界面出现显示问题，可以使用上述命令验证同样的功能。

---

## 十一、Launch 文件

`homework.launch` 主要完成四件事：

1. 包含 `smb_gazebo.launch`；
2. 设置 `laser_enabled=true`；
3. 加载 YAML 并启动两个节点；
4. 启动 RViz 并加载保存的配置。

核心结构：

```xml
<include file="$(find smb_gazebo)/launch/smb_gazebo.launch">
  <arg name="laser_enabled" value="true"/>
</include>

<rosparam command="load"
          file="$(find smb_highlevel_controller)/config/params.yaml"/>

<node pkg="smb_highlevel_controller"
      type="scan_processor"
      name="scan_processor"
      output="screen"/>

<node pkg="smb_highlevel_controller"
      type="obstacle_monitor"
      name="obstacle_monitor"
      output="screen"/>
```

启动：

```bash
roslaunch smb_highlevel_controller homework.launch
```

使用 `$(find 包名)` 可以避免把个人电脑上的绝对路径写死，使软件包更容易迁移。

---

## 十二、RViz 显示

本次 RViz 配置包括：

- Fixed Frame：`odom`；
- LaserScan：`/scan`；
- PointCloud2：`/rslidar_points`。

配置文件：

```text
rviz/smb_view.rviz
```

如果 RViz 中看不到数据，可以检查：

1. 对应话题是否正在发布；
2. Fixed Frame 是否正确；
3. 消息的 `frame_id` 是否能通过 TF 转换到 `odom`；
4. 点的大小是否过小；
5. 仿真是否已经启动或处于暂停状态。

---

## 十三、pointcloud_to_laserscan 的作用

该节点用于把三维点云转换为二维 LaserScan。

简单理解：

> 从三维点云中选取一定高度范围内的点，再投影到二维平面，形成类似单线激光雷达的数据。

本次数据关系为：

```text
输入：/rslidar_points
类型：sensor_msgs/PointCloud2

输出：/scan
类型：sensor_msgs/LaserScan
```

它并不会把三维信息完整保留下来，而是提取适合二维处理的部分，降低后续算法的处理复杂度。

该节点已经由仿真 launch 提供，因此自己的 `homework.launch` 不应重复启动，否则可能出现同一个话题存在多个发布者的问题。

---

## 十四、rosbag 录制与回放

### 14.1 录制

```bash
rosbag record -O homework_obstacle.bag /homework/obstacle
```

本次 bag 中保存了：

- 一个话题：`/homework/obstacle`；
- 消息类型：`smb_highlevel_controller/ObstacleDistance`；
- 206 条消息；
- 约 20.5 秒数据。

录制过程中移动障碍物，使距离发生变化，并包含小于报警阈值的情况。

### 14.2 查看 bag 信息

```bash
rosbag info homework_obstacle.bag
```

### 14.3 离线回放

先停止 Gazebo 和原来的 `scan_processor`，避免原节点继续发布同一个话题。

然后：

```bash
roscore
rosrun smb_highlevel_controller obstacle_monitor
```

另开终端：

```bash
rosparam set use_sim_time true
rosbag play homework_obstacle.bag --clock
```

应当先启动监控节点，再播放 bag，避免错过开始部分的数据。

回放时，监控节点能够重新显示距离并在满足条件时报警，说明自定义消息和监控逻辑不依赖实时仿真也能工作。

### 14.4 bag 中没有什么

rosbag 只记录指定的话题消息，不自动记录：

- `rosservice call` 服务调用；
- `rqt_reconfigure` 参数修改操作；
- 节点的内部变量；
- RViz 中的显示配置。

因此回放时，报警距离和报警开关使用监控节点本次启动后的状态，不会自动恢复成录制过程中的每一次修改。

---

## 十五、常用命令总结

```bash
# 编译
cd /root/catkin_ws
catkin_make
source devel/setup.bash

# 启动
roslaunch smb_highlevel_controller homework.launch

# 查看节点和话题
rosnode list
rostopic list
rostopic echo /homework/obstacle
rostopic hz /homework/obstacle

# 查看参数
rosparam get /obstacle_monitor/warning_distance

# 修改报警距离
rosservice call /homework/set_warning_distance "distance: 0.5"

# 动态开关报警
rosrun dynamic_reconfigure dynparam set \
  /obstacle_monitor warning_enabled false

rosrun dynamic_reconfigure dynparam set \
  /obstacle_monitor warning_enabled true

# 查看通信关系
rqt_graph

# 录制与回放
rosbag record -O homework_obstacle.bag /homework/obstacle
rosbag info homework_obstacle.bag
rosbag play homework_obstacle.bag --clock
```

---

## 十六、学习收获

通过本次实践，我对 ROS 的理解从“会运行几条命令”变成了“能够按数据流分析程序”。

主要收获包括：

1. 理解了节点、话题、服务和参数服务器分别适合什么场景；
2. 学会处理 LaserScan 中的无效测量，而不是直接求数组最小值；
3. 理解了自定义消息生成后，还需要正确配置 CMake 和依赖；
4. 学会了将传感器处理与监控报警拆成两个节点；
5. 理解了 YAML 初始参数、运行时服务和动态参数之间的区别；
6. 掌握了使用 rosbag 将实时问题转化为可重复回放的问题；
7. 认识到排查 ROS 问题时，除了检查代码，还要检查环境变量、话题名称、参数覆盖、TF 和仿真状态。

以后遇到类似问题，我会按照下面的顺序排查：

```text
节点是否运行
→ 话题是否存在
→ 消息是否正常发布
→ 回调是否执行
→ 参数是否真正生效
→ 输出是否符合预期
```

这种按数据流逐层检查的方法，比盲目修改代码更有效。