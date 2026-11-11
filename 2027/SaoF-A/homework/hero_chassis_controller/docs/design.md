# 控制器结构

包目录按 rm_template 的 include、src、config、launch、test 和安装规则整理。
模板来源：https://github.com/gdut-dynamic-x/rm_template 。模板的 BSD 许可保存在 LICENSE 中。

参考控制器只给四轮写固定力矩；本包读取轮速，用四个速度 PID 计算关节力矩。
Gazebo 通过 gazebo_ros_control 提供 EffortJointInterface，controller_manager 负责加载插件。

轮子顺序：左前、右前、左后、右后。
速度输入：线速度用 m/s，角速度用 rad/s；轮速用 rad/s，力矩用 N·m。
速度输入为 base_link 下的 vx、vy、wz，x 朝前、y 朝左，wz 逆时针为正。

设轮半径 r=0.07625 m，L=(轴距+轮距)/2=0.4 m：

```text
左前 = (vx - vy - L*wz) / r
右前 = (vx + vy + L*wz) / r
左后 = (vx + vy - L*wz) / r
右后 = (vx - vy + L*wz) / r
```

| 动作 | 左前 | 右前 | 左后 | 右后 |
|---|---|---|---|---|
| 前进 | + | + | + | + |
| 后退 | - | - | - | - |
| 左移 | - | + | + | - |
| 逆时针转向 | - | + | - | + |

wheel_signs 将数学轮速与关节正方向对应。本模型 DefaultRobotHWSim 接口使用四个 +1。
其他模型不能仅凭电机减速比的正负套用本配置，应重新检查实际运动方向。

## 里程计怎样得到位置

update 从四个 JointHandle 读取实际轮速，先乘 wheel_signs，再按下式求底盘速度。
这里不能使用目标轮速，否则轮子没跟上指令时，里程计也会错误地认为已经移动。

```text
vx = r/4 * (左前 + 右前 + 左后 + 右后)
vy = r/4 * (-左前 + 右前 + 左后 - 右后)
wz = r/(4L) * (-左前 + 右前 - 左后 + 右后)
```

每个更新周期用当前朝向把 vx、vy 转到 odom 坐标后积分。平移使用周期中点朝向，
yaw 保持在 [-pi, pi]。周期来自 Gazebo 的仿真时钟；暂停仿真时位置不会继续累计。

/odom 的 pose 在 odom 坐标中，twist 在 base_link 中；TF 同时发布 odom → base_link。
轮式里程计假设轮子与地面滚动接触，因此打滑会累积位置误差。它不是 Gazebo 真值定位。
重启控制器会把里程计原点归零；切换坐标模式无需重启，不会归零。

## PID 与实时更新

四个 control_toolbox::Pid 的输入都是“目标轮速减实际轮速”，输出是关节力矩。
各 PID 使用独立积分状态，积分项限幅 0.5 N·m，最终力矩限制为 ±5 N·m。
PID 自带的 dynamic_reconfigure 服务允许分别修改四个轮子的增益。
回调线程通过 realtime buffer 传递命令与参数；update 不执行 TF 查询或等待话题发布锁。
超时或 TF 无效会立即清零目标轮速，PID 继续制动，车体停止仍需要一定物理时间。

## 世界坐标控制与加速度限制

base_link 模式的“前进”始终沿车头；world 模式的输入沿 world_frame，默认为 odom。
非实时 ROS 定时器每 20 ms 用 TF transformVector 转换线速度，查询失败或变换超过
0.2 秒未更新时使命令无效。角速度在平面模型中始终绕 z 轴。
模式可由启动参数 cmd_vel_mode 选择，也可由在线参数 world_mode 切换。

速度输入先检查有限数值，并限制到 x/y ±0.6 m/s、wz ±1.2 rad/s，再进入斜坡限制。
斜坡约束每周期速度变化不大于“最大加速度 × 周期”，随后计算目标轮速。
默认 x/y 加速度 0.5 m/s²，转向加速度 1 rad/s²。四轮超过最大轮速时等比例缩放，
保留运动方向。停止发布指令超过 0.3 秒会触发超时停车。
