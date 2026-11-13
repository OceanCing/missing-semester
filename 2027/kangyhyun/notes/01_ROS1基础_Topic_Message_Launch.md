# ROS1 学习笔记 1：基础、Topic、Message、Launch
 这份笔记对应本次 `hero_chassis_controller` 作业的基础部分。

---

## 1. ROS1 到底在解决什么问题

一个机器人程序通常不是“一个巨大的 main 函数”，而是拆成很多小节点：

```text
传感器节点
控制节点
仿真节点
显示节点
导航节点
```

节点之间通过 Topic、Service、Parameter 等方式交换数据。ROS1 负责：

```text
启动节点
管理进程
传递消息
保存参数
提供工具
```

程序里至少有这些角色：

```text
Gazebo / gzserver
controller_manager
joint_state_controller
hero_chassis_controller
robot_state_publisher
```

---

## 2. 工作空间、包、节点

### 2.1 目录结构：

```text
chassis_ws/
├── src/         源码
├── build/       catkin 编译中间文件
├── devel/       开发环境，source 这里
└── logs/        编译日志
```

进入终端后要先 source：

```bash
source /opt/ros/noetic/setup.bash
source /root/chassis_ws/devel/setup.bash
```

`source` 做的事情：


    把 ROS 的可执行文件、库、Python 包和当前工作空间加入环境变量

每个新终端都要重新 source 两次。

### 2.2 包

包是 ROS 软件的基本组织单位。一个包通常有：

    package.xml      包名、作者、依赖
    CMakeLists.txt   编译规则
    src/             C++ 源码
    include/         头文件
    launch/          启动文件
    config/          YAML 配置


本次的包：

```
hero_chassis_controller
```

查包路径：

```
rospack find hero_chassis_controller
```

进入包目录：

```
roscd hero_chassis_controller
```

### 2.3 节点

节点是运行中的 ROS 进程。例如：

```
roslaunch 启动的 controller_spawner 是一个节点
joint_state_controller 是 controller_manager 里的控制器
hero_chassis_controller 不是独立节点，是插件
```

查看节点：

```
rosnode list
rosnode info /controller_spawner
```

---

## 3. Topic：节点之间的广播

### 3.1 Topic 是什么

Topic 是“发布者/订阅者”模型：

```
Publisher ── 消息 ──> Topic ── 消息 ──> Subscriber
```

特点：
    一对多
    发布者不需要知道订阅者是谁


本次最关键的 Topic：

     Topic  类型  方向  作用 
     `/cmd_vel`  `geometry_msgs/Twist`  控制器订阅  速度指令 
     `/odom` | `nav_msgs/Odometry`  控制器发布  里程计 
     `/joint_states`  `sensor_msgs/JointState`  `joint_state_controller` 发布  四个轮子的位置 ，速度和力矩
     `/tf`  `tf2_msgs/TFMessage`  TF broadcaster 发布  坐标变换 
     `/clock`  `rosgraph_msgs/Clock`  Gazebo 发布  仿真时间 
### 3.2 查看 Topic

```
rostopic list
rostopic info /cmd_vel
rostopic type /cmd_vel
rostopic echo /cmd_vel
rostopic hz /cmd_vel
```

`rostopic info` 能看到：

```
消息类型
发布者
订阅者
```

### 3.3 发布 Topic

```
rostopic pub -r 10 /cmd_vel geometry_msgs/Twist \
  '{linear: {x: 0.2}}'
```
含义：
```
`rostopic pub`  发布消息 
`-r 10` 每秒 10 次 
`/cmd_vel` 话题名 
| `geometry_msgs/Twist` 消息类型 
`'{linear: {x: 0.2}}'`  消息内容 

`-r 10` 会一直发布，直到按 `Ctrl+C`。
```
### 3.4 本项目的 Topic 链路

```
rostopic pub /cmd_vel
HeroChassisController::cmdCallback()
cmd 成员变量
update()
四个轮子 setCommand()
```

另一条链：

```
Gazebo 关节状态
joint_state_controller
/joint_states
robot_state_publisher + 控制器 getVelocity()
```

---

## 4. Message：Topic 里传的数据

### 4.1 Message 是什么

Message 是 ROS 定义的数据结构，相当于 C++ 里的结构体。

查看消息定义：

```bash
rosmsg show geometry_msgs/Twist
rosmsg show nav_msgs/Odometry
rosmsg show sensor_msgs/JointState
```

### 4.2 `geometry_msgs/Twist`

```
Vector3 linear
  float64 x
  float64 y
  float64 z

Vector3 angular
  float64 x
  float64 y
  float64 z
```

麦轮底盘使用：

```
linear.x    前进/后退
linear.y    左移/右移
angular.z   原地旋转
```

注意：

```
linear.z、angular.x、angular.y 在平面麦轮底盘里不用
```

### 4.3 `nav_msgs/Odometry`

```
Header header
  frame_id = odom
string child_frame_id
  base_link

PoseWithCovariance pose
  position x/y/z
  orientation 四元数

TwistWithCovariance twist
  linear x/y/z
  angular x/y/z
```

`Odometry` 同时给出：

```
车在哪          pose
车现在多快      twist
这两个量有多确定 covariance
```

### 4.4 `sensor_msgs/JointState`

```
Header header
string[] name
float64[] position
float64[] velocity
float64[] effort
```

四个轮子的名字：

```
left_front_wheel_joint
right_front_wheel_joint
left_back_wheel_joint
right_back_wheel_joint
```

`/joint_states` 是 `robot_state_publisher` 发布轮子 TF 的依据，也是调试 PID 时看轮速的数据源。

### 4.5 Message 和 C++ 代码的对应

订阅：

```cpp
cmd_sub = root_nh.subscribe<geometry_msgs::Twist>(
    "cmd_vel", 1, &HeroChassisController::cmdCallback, this);
```

回调参数：

```cpp
void cmdCallback(
    const geometry_msgs::Twist::ConstPtr& msg)
```

发布：

```cpp
odom_pub = root_nh.advertise<nav_msgs::Odometry>("odom", 10);
odom_pub.publish(odom);
```

---

## 5. Parameter 和 YAML

### 5.1 Parameter Server

参数服务器保存键值：

```
/controller/hero_chassis_controller/wheel_radius
/controller/hero_chassis_controller/world_frame_mode
/controller/hero_chassis_controller/left_front_pid/p
```

代码读取：

```cpp
wheel_r = nh.param("wheel_radius", 0.07625);
use_global = nh.param("world_frame_mode", false);
```

第二个参数是默认值：参数不存在时使用它。

### 5.2 查看和修改参数

```
rosparam list
rosparam get /controller/hero_chassis_controller/wheel_radius
rosparam set /controller/hero_chassis_controller/world_frame_mode true
```

**注意**：控制器里的很多参数只在 `init()` 读一次，运行时用 `rosparam set` 改了不一定生效。最稳妥的方式是改 YAML 后重启 `roslaunch`。

### 5.3 YAML 与命名空间

`controllers.yaml`：

```yaml
controller:
  hero_chassis_controller:
    wheel_radius: 0.07625
    world_frame_mode: false
    left_front_pid: {p: 1.0, i: 1.0, d: 0.0, i_clamp: 2.0}
```

对应：

```
controller/hero_chassis_controller/wheel_radius
controller/hero_chassis_controller/world_frame_mode
controller/hero_chassis_controller/left_front_pid/p
```

## 6. Launch：一次启动多个东西

### 6.1 为什么需要 launch

手动启动要开很多终端：

```
rosrun ...
rosrun ...
rosrun ...
```

`launch` 可以：

```
一次启动多个节点
加载 YAML
设置参数
传参数给 include
```

### 6.2launch
以hero_chassis_controller举例：
```xml
<include file="$(find rm_description_for_task)/launch/hero.launch">
  <arg name="load_chassis" value="true"/>
  <arg name="roller_type" value="realistic"/>
  <arg name="paused" value="true"/>
</include>

<rosparam file="$(find hero_chassis_controller)/config/controllers.yaml"
          command="load"/>

<node name="controller_spawner"
      pkg="controller_manager"
      type="spawner"
      args="controller/hero_chassis_controller
            controller/joint_state_controller"/>
```

流程：


- include hero.launch
- 加载 Gazebo 世界和机器人模型
- rosparam load
- 把 YAML 灌进参数服务器
- controller_spawner
- 请求 controller_manager 加载两个控制器


### 6.3 `roslaunch` 和 `rosrun` 的区别

`rosrun` 运行一个可执行文件 
`roslaunch`按 XML 启动多个节点/参数 

`roslaunch` 会自动启动 `roscore`。

### 6.4 Gazebo 的暂停参数

hero的 launch 里：

```xml
<arg name="paused" value="true"/>
```

**注意**：所以 Gazebo 启动后默认暂停，需要点左下角 play。如果忘了点，车不会动，`update()` 也可能不按预期运行。

## 7. 编译和常用检查命令

### 7.1 编译

```
source /opt/ros/noetic/setup.bash
source /root/chassis_ws/devel/setup.bash
cd /root/chassis_ws
catkin build hero_chassis_controller
```

### 7.2 查看包和节点

```
rospack find hero_chassis_controller
rosnode list
rosnode info /controller_manager
```

### 7.3 查看 Topic

```
rostopic list
rostopic info /cmd_vel
rostopic echo /odom
rostopic echo /joint_states -n 1
```

### 7.4 查看参数

```
rosparam list
rosparam get /controller/hero_chassis_controller/base_frame
```


## 8. 在进行仿真和调试里最容易犯的 Topic/Message 错误
以hero举例

### 错误 1：新终端没有 source

现象：

```
rostopic: command not found
```

解决：

```
source /opt/ros/noetic/setup.bash
source /root/chassis_ws/devel/setup.bash
```

### 错误 2：消息类型写错

错误：

```
rostopic pub /cmd_vel geometry_msgs/TwistStamped ...
```

正确：

```
rostopic pub /cmd_vel geometry_msgs/Twist ...
```

控制器订阅的是 `geometry_msgs/Twist`，不是 `TwistStamped`。

### 错误 3：以为 `/odom` 是指令

`/cmd_vel` 是输入指令，`/odom` 是输出估计。两者不要混。

### 错误 4：改了 YAML 但没重启

很多参数只在 `init()` 读取一次，改完要重新 `roslaunch`。

### 错误 5：把 `/joint_states` 的索引弄错

`/joint_states` 的数组顺序不一定等于预设的轮子顺序。调试时先看 `name` 数组，再对应速度。

## 9. 总结

 ROS1 用 Topic 传消息，用 Message 定义数据结构，用 Parameter/YAML 保存配置，用 Launch 一次启动 Gazebo、控制器和参数；本次作业的输入是 `/cmd_vel`，输出是 `/odom` 和 `/joint_states`，控制器则是被 `controller_manager` 加载的插件。
