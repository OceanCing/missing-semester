# 环境
- ROS Noetic (Ubuntu 20.04)
- C++11 / Python3
- 仿真：Gazebo

# 依赖
catkin,geometry_msgs,nav_msgs,roscpp,sensor_msgs,message_generation,std_msgs

# 构建
```
source opt/ros/noetic/setup.bash 
cd 工作空间路径
catkin build
source devel/setup.bash
```
# 启动
1. 启动仿真和节点
```
roslaunch smb_highlevel_controller smb_highlevel_controller.launch
```
2. 打开动态调参面板

```
rosrun rqt_reconfigure rqt_reconfigure
```
3. 录制 bag（至少 10s，录制时移动障碍物改变距离）

```
rosbag record -O obstacle.bag /scan /homework/obstacle
```
4. 查看节点与话题图
```
rqt_graph
```
5. 回放 bag（关闭 Gazebo 与原发布节点，仅保留监控节点）
```
rosbag play obstacle.bag
```

## 完成情况
- 已创建与构建软件包
- 可输出激光最小距离与点云数量，可处理无效数据处理结果
- 已自定义消息与发布订阅，可正常通信
- `warning_enabled` 仅控制报警输出，报警距离阈值由独立 service 管理，可处理法与非法请求的调用结果
- 实现 `dynamic_reconfigure`，bool 参数 `warning_enabled`，rqt_reconfigure 在线开关报警
- 支持 rosbag 录制与回放，可复现距离输出与报警逻辑

## 问题简答
1. 参数服务器的值改变后，节点行为是否一定立即改变。
    不一定，分情况
    - 只在启动读取一次，后面即使修改也不会变，就是我这种写法,读取只执行一次，没有循环，想要生效只能重启节点(我的代码)
    - 另一种将nh.param写在节点循环里面，不断读取，这样节点行为就会改变,或者是写自定义ros服务接口文件（srv）

2. 数据流（rqt_graph说明：
Gazebo 仿真节点发布 `/scan`（pointcloud_to_laserscan）和`/rslidar_points`→scan_processor节点订阅 /scan和rslidar_points，计算最小距离和云中点数量；节点发布 `/homework/obstacle`→obstacle_monltor节点订阅/homework/obstacle，得到距离和有效标志，并判断报不报警； rqt_reconfigure 通过自动生成服务 `/smb_highlevel_controller/warning_config` 修改参数；自定义服务 `/homework/set_warning_distance` 修改报警阈值。
3. pointcloud_to_laserscan 节点
    - 作用: 把3D 点云（sensor_msgs/PointCloud2），在指定高度区间（`min_height` ~ `max_height`）切片投影，生成2D 激光扫描数据（sensor_msgs/LaserScan）。
    - 输入话题：/tf； 输出话题：/scan

4. rosbag 内容：
bag 仅保存录制的话题消息（`/scan`、`/homework/obstacle`，带时间戳）；不保存服务调用、dynamic_reconfigure 参数修改操作，因为 service 不是话题流，rosbag 只订阅录制话题。

5. nodelet
Node（普通 ROS 节点：
Node 是独立操作系统进程，拥有独立 PID。节点间通过 ROS 网络通信，消息需要序列化、传输、反序列化。优点是进程隔离，单个节点崩溃不影响其他节点；缺点是大数据（点云、图像）传输拷贝开销大、延迟较高。使用`rosrun`直接启动。
Nodelet：
Nodelet 本身不是独立进程，是编译生成的动态库（`.so`）插件，不能单独启动，必须由 nodelet_manager 加载运行。加载到同一个 manager 中的 nodelet 之间采用进程内共享指针通信，不需要序列化、不走网络，实现零拷贝，适合点云、图像等大数据处理。
Nodelet Manager：
Manager 本身是一个标准 ROS Node，属于独立进程，作为 nodelet 的宿主容器。它负责接收加载 / 卸载服务，动态载入多个 nodelet 插件，统一管理容器内所有 nodelet。
问题：两个 nodelet 插件是否属于同一个进程:
两个 nodelet 插件加载到同一个 manager 时，属于同一个进程；如果分别加载到两个不同的 manager，则分属两个独立进程，节点间通信走 ROS 网络。