# ROS1 学习笔记 2：ros_control、pluginlib 与 Gazebo 仿真
以hero_chassis_controller为例

这份笔记对应的“控制器怎么被加载、怎么和 Gazebo 交互”部分。
重点：控制器不是独立节点，而是 ros_control 里的一个插件。

## 1. 先看整体架构

```
Gazebo / gzserver
  │
  ├── 物理引擎：重力、碰撞、摩擦、轮子转动
  ├── 机器人模型：URDF/xacro
  ├── transmission：力矩和关节运动之间的传动
  └── controller_manager
        ├── joint_state_controller
        └── hero_chassis_controller
              ├── init()
              └── update()
```

从控制角度看：
 `ros_control` 的核心循环：

```
控制器读关节状态
  算控制量
  写关节命令
 Gazebo 执行
  下一周期再读状态
```

```
read → update → write
```
## 2. `ros_control` 是什么

`ros_control` 是 ROS 的一套通用控制框架。它把“控制算法”和“硬件/仿真”分开：
- 控制器只关心 JointHandle
- Gazebo 或实车只负责提供硬件接口

好处：
- 同一套控制器代码可以用于仿真和实车
- 换硬件时主要替换 hardware_interface


本次使用的接口：

```cpp
hardware_interface::EffortJointInterface
```

`Effort` 表示力矩/力，不是位置，也不是速度。

## 3. 控制器类

### 3.1 继承关系

```cpp
class HeroChassisController
  : public controller_interface::Controller<
        hardware_interface::EffortJointInterface>
```

含义：这是一个 ros_control 控制器，使用 EffortJointInterface，它会被 controller_manager 加载。


### 3.2 两个核心函数

```cpp
bool init(
    hardware_interface::EffortJointInterface* effort,
    ros::NodeHandle& root_nh,
    ros::NodeHandle& nh) override;

void update(
    const ros::Time& time,
    const ros::Duration& dt) override;
```

`init()`：

```
控制器加载时调用一次
拿关节句柄
读 YAML 参数
初始化 PID
订阅 /cmd_vel
广告 /odom
```

`update()`：

```
每个控制周期调用
读指令
算目标轮速
算力矩
写命令
算里程计
发布 /odom 和 TF
```

### 3.3 `init()` 的参数

```cpp
hardware_interface::EffortJointInterface* effort
```

- 硬件层递进来的“关节池”，用来 `getHandle()`。

```cpp
ros::NodeHandle& root_nh
```

- 管理器节点句柄。用它发布 `/odom`、订阅 `/cmd_vel`。

```cpp
ros::NodeHandle& nh
```

- 控制器自己的命名空间，例如：

```text
/controller/hero_chassis_controller
```

- 用它读 `wheel_radius`、`left_front_pid/p` 等参数。

## 4. `JointHandle`：控制器的手

### 4.1 获取句柄

```cpp
joint_lf = effort->getHandle("left_front_wheel_joint");
joint_rf = effort->getHandle("right_front_wheel_joint");
joint_lb = effort->getHandle("left_back_wheel_joint");
joint_rb = effort->getHandle("right_back_wheel_joint");
```

关节名必须和 URDF 里的名字完全一致。

### 4.2 常用操作

```cpp
joint_lf.getName();
joint_lf.getVelocity();
joint_lf.getPosition();
joint_lf.getEffort();
joint_lf.setCommand(u);
```

在 `EffortJointInterface` 里，`setCommand()` 写的是力矩。

### 4.3 为什么一个轮子一个句柄

四个轮子是四个独立关节，摩擦、负载、装配都不同。四个句柄分别读、分别写，才能做四轮独立 PID。

---

## 5. `pluginlib`：控制器的插头机制

### 5.1 为什么需要 pluginlib

如果控制器写死在 `controller_manager` 里，每次加一个新控制器都要改管理器的源码。`pluginlib` 允许：

```text
控制器编译成 .so
运行时按名字加载
不用重新编译 controller_manager
```

### 5.2 三个名字

本项目的 `hero_chassis_controller_plugins.xml`：

```xml
<library path="lib/libhero_chassis_controller">
  <class
    name="hero_chassis_controller/HeroChassisController"
    type="hero_chassis_controller::HeroChassisController"
    base_class_type="controller_interface::ControllerBase">
    <description>Mecanum wheel chassis controller.</description>
  </class>
</library>
```

对应关系：

- 插件名 ：`hero_chassis_controller/HeroChassisController` | YAML 的 `type`、XML 的 `name` |
- C++ 类型 ： `hero_chassis_controller::HeroChassisController` | XML 的 `type` |
- 动态库 ： `lib/libhero_chassis_controller` | XML 的 `library path` |

### 5.3 CMake 生成动态库

```cmake
add_library(${PROJECT_NAME} SHARED
  src/hero_chassis_controller.cpp
  src/mecanum_kinematics.cpp
)
```

生成：
```
devel/.private/hero_chassis_controller/lib/libhero_chassis_controller.so
```

### 5.4 C++ 注册

文件最后：

```cpp
PLUGINLIB_EXPORT_CLASS(
    hero_chassis_controller::HeroChassisController,
    controller_interface::ControllerBase)
```
把 HeroChassisController 注册成 ControllerBase 插件


### 5.5 package.xml 导出插件

```
##pluginlib 通过这一段知道插件 XML 在哪。
xml
<export>
  <controller_interface
      plugin="${prefix}/hero_chassis_controller_plugins.xml"/>
</export>
```


### 5.6 从 YAML 到对象

```
controllers.yaml type: hero_chassis_controller/HeroChassisController
        
controller_spawner 读 type
        
controller_manager 查 pluginlib
        
找到 XML 里的 class
    
加载 .so
    
创建 HeroChassisController 对象
    
调用 init()
```
## 6. Gazebo 仿真

### 6.1 `gzserver` 和 `gzclient`

```
gzserver：物理引擎、传感器、控制器、ROS 接口
gzclient：Gazebo 图形界面
```

调试控制器要 attach 到：`gzserver`。

不要 attach 到 `gzclient`，也不要 attach 到 `systemd`。

### 6.2 Gazebo 启动流程

以hero_chassis_controller为例的 launch 包含：

```xml
<include file="$(find rm_description_for_task)/launch/hero.launch">
  <arg name="load_chassis" value="true"/>
  <arg name="roller_type" value="realistic"/>
  <arg name="paused" value="true"/>
</include>
```

`hero.launch` 做：
1. 读取 hero.urdf.xacro
2. 生成 robot_description
3. 启动 empty_world.launch
4. 用 spawn_model 把车放进 Gazebo


`paused=true` 表示 Gazebo 一开始暂停，要点左下角 play。

### 6.3 URDF/xacro

URDF 描述机器人：

```
link：车体、轮子
joint：车体和轮子之间的关节
collision：碰撞体
visual：外观
inertial：质量、惯量
transmission：传动
```

xacro 是带宏和变量的 URDF 生成器。项目里：

```
wheel_radius = 0.07625
wheel_base   = 0.4
wheel_track  = 0.4
```

### 6.4 transmission

`mecanum_wheel.urdf.xacro` 里有：

```xml
<transmission name="${prefix}_wheel_trans">
  <type>transmission_interface/SimpleTransmission</type>
  <actuator>
    <hardwareInterface>hardware_interface/EffortJointInterface</hardwareInterface>
    <mechanicalReduction>${mechanical_reduction}</mechanicalReduction>
  </actuator>
  <joint name="${prefix}_wheel_joint">
    <hardwareInterface>hardware_interface/EffortJointInterface</hardwareInterface>
  </joint>
</transmission>
```

作用：

```
告诉 Gazebo 这个关节接受 EffortJointInterface 的力矩命令
```

减速比：

```
left_front   +19.2032
right_front  -19.2032
left_back    +19.2032
right_back   -19.2032
```

左右符号不同，和轮子的安装方向有关。

### 6.5 `joint_state_controller`

YAML：

```yaml
joint_state_controller:
  type: joint_state_controller/JointStateController
  publish_rate: 50
```

作用：

```
读取 Gazebo 四个关节的状态
发布 /joint_states
```

`/joint_states` 包含：

```
name
position
velocity
effort
```

`robot_state_publisher` 用这些数据发布轮子 TF。

### 6.6 仿真时间

Gazebo 会发布 `/clock`。控制器 `update(time, dt)` 里的 `time` 可能是仿真时间，不是墙钟时间。

这意味着：

```
暂停 Gazebo 时，仿真时间不动
调试断点时，仿真也可能被暂停
里程计积分用的是仿真 dt
```

## 7. 加载失败的排查顺序

### 7.1 典型报错

```
Failed to load controller 'controller/hero_chassis_controller'
```

### 7.2 按顺序查

```
1. YAML 里的 type 是否等于 XML 里的 name
2. XML 里的 type 是否是 C++ 类的完整名字
3. XML 的 library path 是否等于 CMake 生成的库名
4. .so 是否真的存在
5. package.xml 是否 export 了 plugin XML
6. PLUGINLIB_EXPORT_CLASS 是否写了
7. 新终端是否 source 了工作空间
```

### 7.3 查看库是否存在

```
ls -l /root/chassis_ws/devel/.private/hero_chassis_controller/lib/libhero_chassis_controller.so
```

### 7.4 查看插件是否被识别

```
rospack plugins --attrib=plugin controller_interface
```

如果输出里没插件，说明 plugin XML、`package.xml` 或 source 有问题。

## 8. 控制器调用链

```
roslaunch
  
controller_spawner
  
controller_manager
  
pluginlib 找到 HeroChassisController
  
init()
  ├── getHandle 四次
  ├── nh.param 读尺寸和模式
  ├── readPid 四次
  ├── 订阅 /cmd_vel
  └── 广告 /odom
  
每个控制周期 update()
  ├── mecanum.inverse()
  ├── 四个 PID
  ├── 四个 setCommand()
  ├── 四个 getVelocity()
  ├── mecanum.forward()
  ├── 积分位姿
  └── sendOdom()
```

---

## 9. Gazebo 调试注意事项

### 9.1 Gazebo 没点 ▶

现象：

```
launch 成功，控制器加载成功，但车不动
```

解决：

```
点 Gazebo 左下角 play
```

### 9.2 轮子转但车不走

可能：

```text
轮子方向反了
地面摩擦太小
车体穿模
轮子没有接触地面
```

先看 `/joint_states` 的轮速，再看 Gazebo 里的接触和碰撞。

### 9.3 控制器加载了但 update 不执行

检查：

```
Gazebo 是否暂停
controller_manager 是否真的 spawn 成功
spawner 输出有没有报错
```

### 9.4 仿真很卡

Debug 版会慢，录完调试后重新编 Release：

```
catkin build simple_chassis_controller
catkin build hero_chassis_controller
```

##10.笔记总结：
1. `gzserver` 和 `gzclient` 分别负责什么？
2. 为什么控制器不能直接 `rosrun`？
3. `EffortJointInterface` 的含义是什么？
4. `JointHandle` 怎么读速度、怎么写命令？
5. `pluginlib` 解决了什么问题？
6. 插件 XML 里的 `name`、`type`、`library path` 分别是什么？
7. YAML 的 `type` 对应插件 XML 的哪个字段？
8. `controller_spawner` 做了什么？
9. `joint_state_controller` 发布什么？
10. `transmission` 在 Gazebo 里有什么作用？
11. 为什么 Gazebo 里要点 play？
12. 加载失败应该先查哪三处名字？

## 11. 一句话总结

> `ros_control` 把控制器和 Gazebo 解耦，`pluginlib` 让控制器能在运行时被 `controller_manager` 加载；控制器通过 `JointHandle` 读轮速、写力矩，`joint_state_controller` 和 `robot_state_publisher` 再把关节状态变成 `/joint_states` 和 TF。
