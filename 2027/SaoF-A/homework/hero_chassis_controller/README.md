# hero_chassis_controller

ROS 1 Noetic 麦克纳姆轮底盘控制器，使用四个独立的速度 PID，通过 EffortJointInterface 输出轮子力矩。
支持 `/cmd_vel`、轮式里程计、`odom → base_link` TF、世界坐标速度、在线调参和键盘驾驶。

本目录是 missing-semester 课程仓库中的 ROS 包。提交文件位于
`2027/SaoF-A/homework/hero_chassis_controller`；原有课程资料保留在仓库其他目录中。

## 使用环境

已验证 Ubuntu 20.04、ROS Noetic、Gazebo 11、GCC 9.4 和 CLion 2026.3。
Ubuntu 24.04 主机使用 Docker 运行这套 ROS 环境。

在 Ubuntu 20.04 / Noetic 容器中安装依赖：

```bash
apt-get update
apt-get install -y python3-catkin-tools ros-noetic-gazebo-ros-control \
  ros-noetic-controller-manager ros-noetic-joint-state-controller \
  ros-noetic-control-toolbox ros-noetic-control-msgs ros-noetic-realtime-tools \
  ros-noetic-dynamic-reconfigure ros-noetic-rqt-plot ros-noetic-rqt-reconfigure \
  ros-noetic-teleop-twist-keyboard ros-noetic-robot-state-publisher \
  ros-noetic-xacro ros-noetic-rviz python3-matplotlib build-essential gdb xterm
```

把当前目录（包含 package.xml 的这一层）复制到 catkin 工作空间的
`src/hero_chassis_controller`。另外在同一 src 中取得模型和参考工程：

```bash
cd /root/catkin_ws/src
git clone https://github.com/Bluefairy-ljy/rm_description_for_task.git
git clone https://github.com/gdut-dynamic-x/simple_chassis_controller.git
cd /root/catkin_ws
source /opt/ros/noetic/setup.bash
catkin build hero_chassis_controller --cmake-args -DCMAKE_BUILD_TYPE=Debug
source devel/setup.bash
roslaunch hero_chassis_controller simulation.launch
```

已有目录时不用重复 clone。CLion 编译、运行和附加调试步骤见操作指南。

## 文档

- [仿真操作与验收](docs/operation-guide.md)：从进入容器到发指令、调参、查看坐标和排查问题。
- [从速度指令读懂代码](docs/code-walkthrough.md)：分六节学习控制链，每节附小实验。
- [控制器设计](docs/design.md)：轮序、公式、里程计与线程处理。
- [实测结果](docs/acceptance.md)：动作数据、PID 曲线和测试范围。
- [Git 分支与冲突记录](docs/git-workflow.md)。

## 接口

| 接口 | 作用 |
|---|---|
| `/cmd_vel` | geometry_msgs/Twist，输入 vx、vy、wz |
| `/odom` | nav_msgs/Odometry，轮速估计的车体位置和速度 |
| `odom → base_link` | TF 坐标关系 |
| `/hero_chassis_controller/<wheel>/state` | 目标轮速、实际轮速、误差和输出力矩 |
| `/hero_chassis_controller/pid/<wheel>` | 四轮各自的在线 PID 参数 |
| `/hero_chassis_controller` | 在线切换 world_mode、超时和加速度 |

`<wheel>` 为 front_left、front_right、back_left、back_right。
启动配置在 config/controllers.yaml，默认底盘模式，超时 0.3 秒停车，力矩限幅 ±5 N·m。

## 启动文件

- `simulation.launch`：新控制器仿真；参数 cmd_vel_mode=base_link/world、world_frame=odom、gui=true。
- `teleop.launch`：独立键盘终端，先启动仿真后使用。
- `inspect.launch`：RViz、轮速曲线和调参界面。
- `required_reference.launch`：保留必做参考控制器的演示。

## 模型兼容

题目模型的包名为 rm_description_for_task；参考启动文件引用 rm_description，
原模型顶层文件还引用独立的 rm_gazebo 插件。本包自己的 xacro 使用原底盘宏，
配合空世界和 gazebo_ros_control/DefaultRobotHWSim，保留两个第三方仓库的源码不改。
仿真使用 realistic 滚子，轮半径 0.07625 m，轴距/轮距均 0.4 m。
模板结构参考 [rm_template](https://github.com/gdut-dynamic-x/rm_template)，保留 BSD 许可。

## 测试

```bash
cd /root/catkin_ws
catkin build hero_chassis_controller --no-deps --make-args run_tests_hero_chassis_controller
catkin_test_results build/hero_chassis_controller/test_results
# 保持仿真运行，结束其他速度发布器后执行：
python3 src/hero_chassis_controller/test/verify_simulation.py --output /tmp/hero-simulation.json
```

仿真测试会驾驶底盘并短暂重载控制器检查 TF 失败停车。轮式里程计有打滑误差，
参数也与模型质量、接触条件有关；本包的验证范围是上述仿真环境。

## 许可

BSD-3-Clause，见 LICENSE。
