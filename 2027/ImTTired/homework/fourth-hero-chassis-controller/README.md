# 第四次作业完成情况介绍：

26生工1 王子豪

## 已完成：（95+）
  1. 必做（10分）；
  2.  选做：（85+）
            1. 创建新的 ros_package
            2.完善的版本管理
            3. 正确的代码规范
            4. 使用 PID 控制轮子的速度
            5. 使用逆运动学计算各个轮子的期望速度
            6. 使用正运动学实现里程计
            7. 使用tf计算实现世界坐标下的速度控制
            8. 其他特色功能（超时停车、速度限制）

## 2. 开发环境

| 软件 | 环境 |
|---|---|
| 操作系统 | Ubuntu 20.04 LTS（WSL2） |
| ROS | ROS Noetic |
| 仿真器 | Gazebo Classic 11 |
| 构建系统 | Catkin Tools / CMake |
| 编程语言 | C++ |
| IDE | CLion |
| 调试器 | GDB |

主要文件说明：

| 文件 | 作用 |
|---|---|
| `hero_chassis_controller.h` | 控制器类声明、状态变量及接口 |
| `hero_chassis_controller.cpp` | PID、运动学、里程计与坐标变换实现 |
| `controllers.yaml` | 控制器、PID 与底盘参数 |
| `hero_chassis_controller_plugins.xml` | ROS Control 插件注册 |
| `CMakeLists.txt` | CMake 构建配置 |
| `package.xml` | ROS 软件包依赖与元数据 |

### 5.1 轮子定义

采用以下轮序：

1. FL：左前轮
2. FR：右前轮
3. RL：左后轮
4. RR：右后轮

底盘坐标系约定：

- X 正方向：机器人前方
- Y 正方向：机器人左方
- Z 正方向：机器人上方
- 绕 Z 轴正向旋转：逆时针

本项目使用的模型参数：

| 参数 | 数值 |
|---|---|
| 车轮半径 r | 0.07625 m |
| 左右轮距 | 0.4 m |
| 前后轮距 | 0.4 m |

### 5.2 逆运动学

输入底盘目标速度：

```text
vx：前后线速度（m/s）
vy：左右线速度（m/s）
wz：绕 Z 轴角速度（rad/s）
```

计算四轮目标角速度：

```text
w_FL = (vx - vy - k*wz) / r
w_FR = (vx + vy + k*wz) / r
w_RL = (vx + vy - k*wz) / r
w_RR = (vx - vy + k*wz) / r
```
### 5.3 正运动学
```text
vx = r * (w_FL + w_FR + w_RL + w_RR) / 4

vy = r * (-w_FL + w_FR + w_RL - w_RR) / 4

wz = r * (-w_FL + w_FR - w_RL + w_RR) / (4*k)
```
## 11. 开发历程

### V0.1：ROS Control 控制器骨架

- 完成控制器类及插件注册
- 获取四个车轮的 EffortJointInterface
- 实现安全零力矩输出
- 验证 Catkin 构建与插件加载

### V0.2：底盘速度控制

- 实现四轮麦克纳姆逆运动学
- 为四轮分别建立 PID 控制器
- 接收 `/cmd_vel` 指令
- 实现速度与力矩限制
- 增加速度指令超时保护
- 在 Gazebo 中测试前进、横移、旋转和斜移

### V0.3：正运动学与里程计

- 读取四轮实际关节角速度
- 实现麦克纳姆轮正运动学
- 通过速度积分估计机器人位姿
- 发布 `/odom`
- 发布 `odom -> base_link` TF
- 验证位置、朝向及 TF 一致性

### V0.4：odom 坐标系速度控制

- 保留原有 `/cmd_vel`
- 新增 `/cmd_vel_world`
- 使用 yaw 完成速度坐标变换
- 复用四轮逆运动学与 PID
- 在 Gazebo 中验证旋转后的固定参考方向运动
