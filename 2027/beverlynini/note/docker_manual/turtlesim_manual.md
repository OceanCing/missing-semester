# Turtlesim 小乌龟参数与玩法说明
# 注意，每开一个不同的节点，都要先开一个新终端进入容器再做
--- 
## 1. 启动前准备

先启动 ROS 主节点：

source /opt/ros/noetic/setup.bash
roscore

- roscore启动了Parameter Server（参数服务器，用来储存全局配置，比如储存了小乌龟颜色，速度，坐标这些参数）和rosout（日志记录）两个基础服务。
- roscore里面运行着master（主节点），相当于一个通讯录，为ros中各个作为独立节点（node）的程序建立联系
- 启动小乌龟时，对应节点会向master注册“我叫turtlesim，我在这里”

再开一个容器终端，启动小乌龟：

source /opt/ros/noetic/setup.bash
rosrun turtlesim turtlesim_node

———

## 2. 参数速查

| 项目 | 是否支持 | 修改方式 |
|---|---|---|
| 背景颜色 | 支持 | background_r/g/b 参数 |
| 画笔颜色 | 支持 | /turtle1/set_pen 服务 |
| 画笔粗细 | 支持 | /turtle1/set_pen 服务 |
| 移动速度 | 支持 | scale_linear、scale_angular 或 cmd_vel |
| 乌龟数量 | 支持 | /spawn 服务 |
| 乌龟位置 | 支持 | /teleport_absolute、/teleport_relative |
| 乌龟大小 | 不支持 | 没有运行时参数 |
| 乌龟本体颜色 | 不支持直接改 | 需要更换图片或修改源码 |
| 乌龟外形 | 只能随机 | 内置多种图片，不能指定 |

———

## 3. 修改背景颜色

RGB 取值范围是 0 到 255：

rosparam set /turtlesim/background_r 30
rosparam set /turtlesim/background_g 30
rosparam set /turtlesim/background_b 60

- rosparam set：ROS修改参数的固定指令
- /turtlesim/background_r：程序启动时在系统默认注册好的参数名称，三原色（r:red ;b:blue ;g:green ）

改完参数后，需要调用清除服务刷新背景：

rosservice call /clear

例如改成浅蓝色：

rosparam set /turtlesim/background_r 180
rosparam set /turtlesim/background_g 220
rosparam set /turtlesim/background_b 255
rosservice call /clear

———

## 4. 修改画笔颜色和粗细

rosservice call /turtle1/set_pen "{r: 255, g: 0, b: 0, width: 5, off: 0}"

参数说明：

| 字段 | 含义 |
|---|---|
| r | 红色，0 到 255 |
| g | 绿色，0 到 255 |
| b | 蓝色，0 到 255 |
| width | 线条宽度 |
| off | 0 表示画线，1 表示关闭画线 |

关闭画线：

rosservice call /turtle1/set_pen "{r: 255, g: 0, b: 0, width: 5, off: 1}"

重新开启画线：

rosservice call /turtle1/set_pen "{r: 255, g: 0, b: 0, width: 5, off: 0}"

注意：set_pen 修改的是乌龟爬行留下的轨迹，不是乌龟本体颜色。

———

## 5. 修改移动速度

默认遥控速度倍率是 2.0。

先设置速度参数，再启动遥控：

rosparam set /scale_linear 1.0
rosparam set /scale_angular 0.5
rosrun turtlesim turtle_teleop_key

参数说明：

| 参数 | 含义 |
|---|---|
| /scale_linear | 前进和后退速度倍率 |
| /scale_angular | 转向速度倍率 |

如果遥控程序已经在运行，需要先按 Ctrl+C 结束，再重新启动。

也可以直接发布速度指令：

rostopic pub -r 10 /turtle1/cmd_vel geometry_msgs/Twist \
"{linear: {x: 1.0}, angular: {z: 0.5}}"

乌龟会持续转圈。按 Ctrl+C 停止发布。

### 直行

rostopic pub -r 10 /turtle1/cmd_vel geometry_msgs/Twist \
"{linear: {x: 2.0}, angular: {z: 0.0}}"

### 原地旋转

rostopic pub -r 10 /turtle1/cmd_vel geometry_msgs/Twist \
"{linear: {x: 0.0}, angular: {z: 1.5}}"

### 停止乌龟

rostopic pub -1 /turtle1/cmd_vel geometry_msgs/Twist \
"{linear: {x: 0.0}, angular: {z: 0.0}}"

———

## 6. 生成新的乌龟

rosservice call /spawn "{x: 3.0, y: 3.0, theta: 0.0, name: 'turtle2'}"

参数说明：

| 字段 | 含义 |
|---|---|
| x | 横坐标 |
| y | 纵坐标 |
| theta | 朝向，单位弧度 |
| name | 乌龟名称 |

生成后可以用新名字控制它：

rostopic pub -r 10 /turtle2/cmd_vel geometry_msgs/Twist \
"{linear: {x: 1.0}, angular: {z: 0.0}}"

删除乌龟：

rosservice call /kill "turtle2"

———

## 7. 移动乌龟到指定位置

### 绝对位置
- 打开一个有别于key和node的终端，进入容器，然后输入以下命令
rosservice call /turtle1/teleport_absolute "{x: 5.0, y: 5.0, theta: 1.57}"

| 字段 | 含义 |
|---|---|
| x | 目标横坐标 |
| y | 目标纵坐标 |
| theta | 目标朝向，单位弧度 |

### 相对移动

rosservice call /turtle1/teleport_relative "{linear: 1.0, angular: 0.0}"

向前移动 1 个单位：

rosservice call /turtle1/teleport_relative "{linear: 1.0, angular: 0.0}"

原地旋转约 90 度：

rosservice call /turtle1/teleport_relative "{linear: 0.0, angular: 1.57}"

———

## 8. 清除轨迹和重置

清除画面和轨迹：

rosservice call /clear

重置 Turtlesim：

rosservice call /reset

———

## 9. 关于外形、大小和本体颜色

### 乌龟大小

turtlesim 没有乌龟大小参数，不能通过 rosparam 直接放大或缩小。

### 乌龟本体颜色

乌龟本体的颜色由内置图片决定，set_pen 只能修改爬行轨迹的颜色。

### 乌龟外形

Turtlesim 内置了多种乌龟图片，启动时会随机选择一种外形。

想换外形，最简单的办法是：

1. 关闭 turtlesim_node。
2. 重新运行 rosrun turtlesim turtlesim_node。
3. 多试几次，就会看到不同的乌龟造型。

也可以通过 /spawn 服务生成多只乌龟，每只都会随机选择外形：

rosservice call /spawn "{x: 2.0, y: 2.0, theta: 0.0, name: 'turtle2'}"

但不能通过参数指定具体使用哪一张外形图片。

如果确实需要自定义外形、大小和颜色，需要修改 Turtlesim 源码或图片资源，或者改用 Gazebo + URDF 机器人模型。

———

## 10. 常用话题和服务

### 常用话题

| 名称 | 作用 |
|---|---|
| /turtle1/cmd_vel | 控制乌龟速度 |
| /turtle1/pose | 获取乌龟位置和朝向 |
| /turtle1/color_sensor | 获取乌龟下方颜色 |

查看当前所有话题：

rostopic list

查看乌龟位置：

rostopic echo /turtle1/pose

### 常用服务

| 名称 | 作用 |
|---|---|
| /clear | 清除轨迹 |
| /reset | 重置仿真 |
| /spawn | 生成新乌龟 |
| /kill | 删除乌龟 |
| /turtle1/set_pen | 设置画笔 |
| /turtle1/teleport_absolute | 移动到绝对位置 |
| /turtle1/teleport_relative | 相对移动 |

———

## 11. 注意事项

- 所有命令都需要先执行 source /opt/ros/noetic/setup.bash。
- 修改背景颜色后必须调用 /clear。
- 修改 scale_linear 和 scale_angular 后，需要重新启动 turtle_teleop_key。
- turtlesim 是 2D 入门仿真，不适合模拟真实机器人结构。
- 需要关节、碰撞、摄像头、激光雷达等完整机器人仿真时，应使用 Gazebo。
  Worked for 14s • 3:14 PM

