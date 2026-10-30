# 该笔记记录做smb小车仿真遇到的问题和解决

## 1.Gazebo GUI 图形界面崩溃
简单来说，docker 容器默认**没有 X11 图形转发**，Gazebo 的 GUI（gzclient）要渲染窗口，找不到显示器，直接崩溃。已经运行的容器无法添加，只能重新创建容器并配置 X11 转发
```
docker run -it \
  --name ros-noetic-x11 \
  --env DISPLAY=$DISPLAY \
  --env QT_X11_NO_MITSHM=1 \
  --volume /tmp/.X11-unix:/tmp/.X11-unix \
  --volume ~/dockerFileShare:/root/download \
  --network host \
  osrf/ros:noetic-desktop-full
```

当需要用x11时，在终端机先放行 X 访问
```
xhost +local:root
```

用完记得回收，也可以不用
```
xhost -local:root
```

## 2.Gazebo仿真为什么可以和ros通信
靠的是**gazebo_ros**包---他编译时把ros的库链接进gazebo，gazebo本身变成一个**ROS节点**（名为gazebo），所以可以进行通信

## 3.需要什么依赖包
```
geometry_msgs
sensor_msgs
nav_msgs
```
msgs意味着信息

## 3. sensor_msgs/LaserScan.msg
数组代表激光束，从左到右

## 4.无效测量
`sensor_msgs/LaserScan` 里的规则：

- `inf`（无穷大）：激光打出去，没有碰到障碍物，超出雷达最大测距；
- `nan`（Not a Number）：传感器出错、读数非法；
- `value < msg->range_min`：离雷达太近，超出雷达最小测量下限；
- `value > msg->range_max`：超过雷达最大量程。

# 5.无法找到包，无法启用环境
```
cd ~/catkin_smb
rm -rf build devel
source /opt/ros/noetic/setup.bash
catkin_make
source devel/setup.bash
```
## 6.没有scan
缺少 `pointcloud_to_laserscan` 这个包

## 7.ros::spin()
只是循环回调函数，main只执行一次

##8. ros::Subscriber sub1 = nh.subscribe<sensor_msgs::LaserScan>("/scan",10,boost::bind(scanCallback,_1,boost::ref(pub)));
用bind必须表明类型

## 9.launch文件，<node>包裹<rosparam>
这使参数变为这个节点的私有变量，引用要加上这个节点的名字name/

## 10.有关launch语法
/是闭合，类似于{} 

## 11.关于rviz
rviz是ros的可视化工具，能将话题的信息可视化，可以理解为传感器的视野

## 12.一旦 `source devel/setup.bash`，PATH 被污染，ros 基础命令直接消失
老版本bug，改用 catkin build
