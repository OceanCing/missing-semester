# hero_chassis_controller

**基于 `rm_template` 模板开发的英雄机器人底盘控制器工程包**

## 功能简介

本包基于 `rm_template` 模板创建，实现了一个基于 `ros_control` 的英雄机器人麦克纳姆轮底盘控制器，包含：

- 四路独立 PID 轮速控制（`control_toolbox::Pid`，参数可在 `config/controllers.yaml` 中配置）
- 逆运动学：将 `/cmd_vel` 指令解算为四个轮子的期望转速
- 正运动学：由四轮实际转速积分得到里程计，发布 `/odom` 与 `odom → base_link` 变换

## 依赖与安装

### 系统环境

| 项 | 版本 |
|---|---|
| OS | Ubuntu 20.04 (Focal) |
| ROS | Noetic |
| Gazebo | 11 |
| 构建工具 | `catkin_tools`（`catkin build`，**不使用** `catkin_make`） |
| 运行方式 | Docker 容器（镜像 `my_ros_gpu_base:v2`，工作区挂载到 `/root/catkin_ws_backup`） |

### 依赖清单

| 包 | 用途 | 类型 |
|---|---|---|
| `roscpp` | ROS C++ 客户端库 | 编译 + 运行 |
| `controller_interface` | `ControllerBase` 基类，也是 pluginlib 的导出标签名 | 编译 |
| `hardware_interface` | `EffortJointInterface` / `JointHandle` | 编译 |
| `pluginlib` | `PLUGINLIB_EXPORT_CLASS` 与插件发现 | 编译 + 运行 |
| `control_toolbox` | `control_toolbox::Pid` 与 dynamic_reconfigure 支持 | 编译 + 运行 |
| `geometry_msgs` | `Twist`（`/cmd_vel`） | 编译 |
| `nav_msgs` | `Odometry`（`/odom`） | 编译 |
| `tf` | `odom → base_link` 变换广播 | 编译 + 运行 |
| `joint_state_controller` | 发布 `/joint_states` | 运行 |
| `controller_manager` | `spawner`：加载 / 启停控制器 | 运行 |
| `rm_gazebo` | 提供 `librm_robot_hw_sim.so`，在 Gazebo 内创建 `controller_manager` | 运行 |
| `rqt_plot` / `rqt_reconfigure` | 调参与演示 | 调试工具 |

### 安装

```bash
# 1) ROS 基础依赖（Noetic）
apt install -y ros-noetic-control-toolbox ros-noetic-pluginlib \
               ros-noetic-hardware-interface ros-noetic-controller-interface \
               ros-noetic-joint-state-controller ros-noetic-controller-manager \
               ros-noetic-geometry-msgs ros-noetic-nav-msgs ros-noetic-tf \
               ros-noetic-xacro

# 2) 仿真依赖：rm_description_for_task 的 URDF 里引用了 librm_robot_hw_sim.so，
#    该库由 rm_gazebo 提供。缺失时 Gazebo 日志会报
#    "Failed to load plugin librm_robot_hw_sim.so"，且 controller_manager 不会出现。
apt install -y ros-noetic-rm-gazebo

# 3) 调参工具（可选，演示 rqt_plot / rqt_reconfigure 需要）
apt install -y ros-noetic-rqt-plot ros-noetic-rqt-reconfigure

# 4) 构建工具
pip3 install catkin_tools          # 或 apt install -y python3-catkin-tools
```

> **注意**：`rosdep install --from-paths src --ignore-src -r -y` **不会**装上
> `rm_gazebo`，因为 `rm_description_for_task/package.xml` 没有声明这个依赖
> （它只在 `hero.urdf.xacro` 的 `<plugin>` 标签里被引用）。必须手动安装，否则仿真里
> 不会生成 `controller_manager`，`spawner` 会一直卡在
> `Waiting for service controller_manager/load_controller`。

### 安装验证

```bash
rospack find rm_gazebo                                    # 应输出 /opt/ros/noetic/share/rm_gazebo
ls /opt/ros/noetic/lib/librm_robot_hw_sim.so              # 文件应存在
rospack plugins --attrib=plugin gazebo_ros_control        # 应出现 rm_gazebo/RmRobotHWSim
```

## 编译

在本工作区执行：

```bash
source /opt/ros/noetic/setup.bash
cd ~/catkin_ws_backup
catkin build hero_chassis_controller
```

## 运行

**顺序不能反**：`controller_manager` 是 gzserver 里的插件创建的，仿真没起来就没有它。

终端 1（先起仿真）：

```bash
roslaunch rm_description_for_task hero.launch
```

等到 `Loaded gazebo_ros_control.`

终端 2（再加载控制器）：

```bash
roslaunch hero_chassis_controller load_controller.launch
```

等到 `Started controllers: ...`

## 验证

```bash
rosservice call /controller_manager/list_controllers   # 两个 running
rostopic echo -n1 /odom                                # 有数据
```

## 控制底盘

```bash
# 前进 / 横移 / 自转
rostopic pub -r 10 /cmd_vel geometry_msgs/Twist '{linear: {x: 0.3}}'
rostopic pub -r 10 /cmd_vel geometry_msgs/Twist '{linear: {y: 0.3}}'
rostopic pub -r 10 /cmd_vel geometry_msgs/Twist '{angular: {z: 0.5}}'

# 停止
rostopic pub -1 /cmd_vel geometry_msgs/Twist '{}'
```

## 调参

```bash
# 四轮转速曲线
rqt_plot '/joint_states/velocity[0]' '/joint_states/velocity[1]' '/joint_states/velocity[2]' '/joint_states/velocity[3]'

# 在线修改 PID 参数
rosrun rqt_reconfigure rqt_reconfigure
```

## 停止

1. 终端 2 `Ctrl-C`，等提示符返回
2. 终端 1 `Ctrl-C` 两次

## 话题接口

| 话题 | 类型 | 方向 | 说明 |
|---|---|---|---|
| `/cmd_vel` | `geometry_msgs/Twist` | 订阅 | 底盘速度指令，**base_link 坐标系**：`linear.x`(m/s)、`linear.y`(m/s)、`angular.z`(rad/s) |
| `/odom` | `nav_msgs/Odometry` | 发布 | 里程计，`frame_id=odom`，`child_frame_id=base_link`；`twist` 为实测速度 |
| `/tf` | `tf2_msgs/TFMessage` | 发布 | `odom → base_link` 变换 |
| `/joint_states` | `sensor_msgs/JointState` | 发布 | 四轮关节状态，由 `joint_state_controller` 以 50 Hz 发布 |
| `/controller/hero_chassis_controller/<轮>/parameter_updates` | `dynamic_reconfigure/Config` | 发布 | 四组 PID 参数，供 `rqt_reconfigure` 使用 |

`<轮>` 取值：`left_front` / `right_front` / `left_back` / `right_back`

### 参数

| 参数 | 单位 | 说明 |
|---|---|---|
| `effort_limit` | N·m | 输出力矩限幅（默认 1.0） |
| `<轮>/p`、`i`、`d` | — | PID 增益 |
| `<轮>/i_clamp_max`、`i_clamp_min` | N·m | 积分项限幅 |
| `<轮>/antiwindup` | bool | 抗积分饱和 |

## 逆运动学验证

对同一台底盘依次下发三种速度指令，记录四个轮子的转速符号与底盘运动：

| `/cmd_vel` 指令 | FL | FR | BL | BR | 底盘现象 |
|---|---|---|---|---|---|
| `linear.x = +0.3` | ＋ | ＋ | ＋ | ＋ | 沿 +x 直线前进 |
| `linear.y = +0.3` | － | ＋ | ＋ | － | 沿 +y 横移，车头不转 |
| `angular.z = +0.5` | － | ＋ | － | ＋ | 原地逆时针自转 |

三种指令下四轮转速的正负组合互不相同，与逆运动学矩阵给出的符号一致。

$$
\begin{bmatrix}\omega_{FL}\\ \omega_{FR}\\ \omega_{BL}\\ \omega_{BR}\end{bmatrix}
= \frac{1}{r}
\begin{bmatrix}
1 & -1 & -k\\
1 & 1 & k\\
1 & 1 & -k\\
1 & -1 & k
\end{bmatrix}
\begin{bmatrix}v_x\\ v_y\\ \omega_z\end{bmatrix},
\qquad r = 0.07625\ \text{m},\ k = l_x + l_y = 0.4\ \text{m}
$$

## 正运动学里程计精度

以 0.3 m/s 指令驱动 2.2 s（理论位移约 0.83 m），对比正运动学积分结果与 Gazebo 真值：

| 量 | `/odom` | Gazebo 真值 | 误差 |
|---|---|---|---|
| 前进位移 x | **0.840 m** | **0.829 m** | **1.1 cm（1.4%）** |
| 横向 y | −0.0002 m | −0.0009 m | 无明显横向漂移 |

$$
v_x = \frac{r}{4}\sum_i \omega_i,\qquad
v_y = \frac{r}{4}(-\omega_{FL}+\omega_{FR}+\omega_{BL}-\omega_{BR}),\qquad
\omega_z = \frac{r}{4k}(\omega_{FR}-\omega_{FL}+\omega_{BR}-\omega_{BL})
$$

积分采用欧拉法，`Δt` 取自 `update(time, period)` 的 `period`：

$$
x_{k+1} = x_k + (v_x\cos\theta_k - v_y\sin\theta_k)\Delta t,\qquad
y_{k+1} = y_k + (v_x\sin\theta_k + v_y\cos\theta_k)\Delta t,\qquad
\theta_{k+1} = \theta_k + \omega_z\Delta t
$$

误差主要来自一阶积分累积与轮子打滑，长距离会漂移。