# hero_chassis_controller

基于 ros_control 的**麦克纳姆轮底盘控制器**，用于控制 Gazebo 中的 RoboMaster 英雄机器人底盘
（模型来自 [rm_description_for_task](https://github.com/Bluefairy-ljy/rm_description_for_task.git)，仿真接口来自 `rm_gazebo`）。

**关键词：** RoboMaster, mecanum, ros_control, PID, odometry, Gazebo

## 概述

- 以 ros_control **控制器插件** 的形式运行，通过 `pluginlib` 加载进 Gazebo 仿真（`EffortJointInterface`）；
- 订阅 `/cmd_vel`（[geometry_msgs/Twist](http://docs.ros.org/en/noetic/api/geometry_msgs/html/msg/Twist.html)），
  支持**底盘坐标系 / 全局坐标系**两种速度指令模式（`velocity_mode` 参数），
  经**麦轮逆运动学**解算四个车轮的期望转速；
- 用四个 `control_toolbox::Pid` **分别闭环控制四个车轮的速度**（速度误差 → 力矩），
  并叠加摩擦前馈，PID 参数可在 yaml 配置文件中设置；
- 由四个车轮的**实际转速**经正运动学得到底盘实际速度，积分实现**里程计**，
  以 [nav_msgs/Odometry](http://docs.ros.org/en/noetic/api/nav_msgs/html/msg/Odometry.html) 发布到 `/odom`，
  并发布 `odom` → `base_link` 的 TF 变换；
- 源码遵循 **Google C++ Style**（附 `.clang-format`），带 gtest 单元测试与 roslint 静态检查。

## 作业需求完成情况

对应[考核规则及评分细则](https://github.com/Bluefairy-ljy/simple_chassis_controller/blob/master/doc/requirement.md)（simple_chassis_controller 仓库 master 分支）：

| 需求 | 状态 | 说明 |
|---|---|---|
| 1. 创建新的 ros_package（5 分） | ✅ 已完成 | 以 rm_template 为模板创建 hero_chassis_controller，CMakeLists.txt、package.xml、README.md 已正确填写并持续更新 |
| 2. 完善的版本管理（5 分） | ✅ 已完成 | 提交信息清晰，仓库历史含合并冲突处理记录 |
| 3. 正确的代码规范（10 分） | ✅ 已完成 | 采用 Google C++ Style，附 .clang-format，可用 roslint 静态检查 |
| 4. 使用 PID 控制轮子速度（20 分） | ✅ 已完成 | 一轮一 PID（`control_toolbox::Pid`），参数在 `config/controllers.yaml` 中配置，另加摩擦前馈 |
| 5. 逆运动学计算轮子期望速度（15 分） | ✅ 已完成 | 订阅 `/cmd_vel`，底盘坐标系速度指令 → 四轮目标转速 |
| 6. 正运动学实现里程计（15 分） | ✅ 已完成 | 实际轮速 → 底盘速度 → 积分位姿，发布 `/odom` 与 `odom→base_link` TF |
| 7. tf 实现世界坐标系速度控制（15 分） | ✅ 已完成 | `velocity_mode` 参数支持 chassis / world 两种模式；world 模式用里程计航向角把世界系指令旋转到底盘系（等价于 odom→base_link 的 tf 变换） |
| 8. 其他特色功能（5 分） | ✅ 已完成 | 摩擦前馈（消除横移跑偏）+ teleop_twist_keyboard 键盘操控 |

> 该表格随需求推进持续更新。

必做部分（10 分：运行 rm_description_for_task 模型、学习理解并运行 simple_chassis_controller、CLion 编译调试）属于环境与理解类任务，在汇报 PPT 与视频中展示。

## 运动学模型

底盘坐标系参照仿真中的 `base_link`：`+x` 向前，`+y` 向左，`+wz` 逆时针。
轮序为 **[左前, 右前, 左后, 右后]**。

车轮半径 `R = 0.07625 m`；`L = a + b = 0.4 m`，其中半轴距
`a = wheel_base / 2 = 0.2 m`，半轮距 `b = wheel_track / 2 = 0.2 m`。

**逆运动学**（底盘速度 → 四轮目标角速度）：

```
w_fl = (vx - vy - L * wz) / R
w_fr = (vx + vy + L * wz) / R
w_bl = (vx + vy - L * wz) / R
w_br = (vx - vy + L * wz) / R
```

**正运动学**（四轮实际角速度 → 底盘速度）：

```
vx = R / 4       * ( w_fl + w_fr + w_bl + w_br)
vy = R / 4       * (-w_fl + w_fr + w_bl - w_br)
wz = R / (4 * L) * (-w_fl + w_fr - w_bl + w_br)
```

里程计：四轮实际转速 → 正运动学 → 底盘实际速度 → 以中间航向角进行位置积分 → `/odom` 与 TF。

## 构建

要求使用 **catkin-tools**（`catkin build`），环境为 ROS Noetic。

```bash
cd ~/ros_ws
catkin build hero_chassis_controller
```

依赖：`rm_description_for_task`、`rm_gazebo`（仿真模型与插件）、`controller_interface`、
`control_toolbox`、`realtime_tools`、`pluginlib`、`nav_msgs`、`tf2_msgs`、`geometry_msgs`、
`Eigen`、`Boost`。

## 运行

```bash
# 启动 Gazebo 英雄底盘模型并加载控制器
roslaunch hero_chassis_controller run_hero_chassis_controller.launch

# 另开终端：前进 0.2 m/s
rostopic pub -r 10 /cmd_vel geometry_msgs/Twist '{linear: {x: 0.2, y: 0.0}, angular: {z: 0.0}}'

# 左移 0.2 m/s
rostopic pub -r 10 /cmd_vel geometry_msgs/Twist '{linear: {x: 0.0, y: 0.2}, angular: {z: 0.0}}'

# 原地逆时针旋转 0.5 rad/s
rostopic pub -r 10 /cmd_vel geometry_msgs/Twist '{linear: {x: 0.0, y: 0.0}, angular: {z: 0.5}}'

# 键盘操控(选做8): i/, 前进后退, j/l 原地转向, k 停止;
# 按住 Shift + J/L 为左右横移, Shift + U/O/M/. 为斜向移动
# 方式一(推荐): 编译一次后可用 rosrun
cd ~/catkin_ws && catkin build teleop_twist_keyboard
rosrun teleop_twist_keyboard teleop_twist_keyboard
# 方式二: 直接运行脚本(需交互式终端, 点击该窗口获得焦点后再按键)
python3 ~/catkin_ws/src/teleop_twist_keyboard/teleop_twist_keyboard.py
```

观察与调试：

```bash
rostopic echo /odom              # 里程计
rostopic echo /tf                # odom -> base_link 变换
rqt_plot /joint_states/velocity[0]:velocity[1]:velocity[2]:velocity[3]  # 四轮实际转速曲线
rosrun rqt_reconfigure rqt_reconfigure   # 在线调 PID 参数（配合 dynamic_reconfigure 使用）
```

## 节点：controller/hero_chassis_controller

**订阅话题**

- **`/cmd_vel`**（[geometry_msgs/Twist](http://docs.ros.org/en/noetic/api/geometry_msgs/html/msg/Twist.html)）
  底盘速度指令 `(vx, vy, wz)`，当前按**底盘坐标系**解释。

**发布话题**

- **`/odom`**（[nav_msgs/Odometry](http://docs.ros.org/en/noetic/api/nav_msgs/html/msg/Odometry.html)）
  里程计，`header.frame_id = "odom"`，`child_frame_id = "base_link"`。
- **`/tf`**（tf2_msgs/TFMessage）
  `odom` → `base_link` 的坐标变换。

**参数**（命名空间 `controller/hero_chassis_controller`，见 `config/controllers.yaml`）

- **`subscriber_topic`**（string，默认 `"/cmd_vel"`）：速度指令话题名。
- **`cmd_vel_timeout`**（double，默认 `0.5`）：指令超时时间 (s)，超时后底盘速度置零。
- **`velocity_mode`**（string，默认 `"chassis"`）：速度指令坐标系模式，
  `"chassis"` 为底盘坐标系，`"world"` 为全局（世界）坐标系。
- **`feedforward_gain`**（double，默认 `0.05`）：摩擦前馈增益 (N·m·s/rad)。
- **`pid_front_left` / `pid_front_right` / `pid_back_left` / `pid_back_right`**：
  各轮速度 PID 的 `p`、`i`、`d`、`i_clamp` 参数。

## 配置文件

- **`config/controllers.yaml`**：控制器类型、话题名、超时、摩擦前馈及四轮 PID 参数。
- **`config/default.yaml`**：模板默认参数（保留示例）。

## 启动文件

- **`launch/run_hero_chassis_controller.launch`**：启动英雄底盘模型与 Gazebo
  （`roller_type=realistic`、仿真暂停等待），加载控制器参数，
  并通过 `controller_manager` spawner 启动 `hero_chassis_controller` 与 `joint_state_controller`。

## 测试与代码检查

```bash
catkin test hero_chassis_controller                                  # 单元测试 (gtest)
catkin build hero_chassis_controller --make-args roslint             # 静态代码检查 (roslint)
```

## 目录结构

```
include/hero_chassis_controller/  头文件（Algorithm、HeroChassisController 等）
src/                              实现（运动学、控制器插件、示例节点）
test/                             gtest 单元测试
config/                           PID 等参数配置
launch/                           启动文件
.clang-format                     Google C++ Style 格式化配置
```

## 许可证

源码遵循 [BSD 3-Clause license](LICENSE)。

**作者：** bingo<br />
**仓库：** <https://github.com/ycdjj4-dev/hero_chassis_controller>
