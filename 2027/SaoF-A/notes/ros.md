# ROS学习笔记

## 1. ROS 与 ros_control 基础

ROS 提供程序之间的通信方式。节点通过话题持续收发消息，通过服务完成一次请求和响应。本次 `/cmd_vel` 使用 `geometry_msgs/Twist`，输入底盘的前进、横移和转向速度；`/joint_states` 提供关节状态；`/odom` 提供轮式里程计。消息的类型说明数据格式，但速度属于哪个坐标系，还需要根据接口约定判断。

catkin 在 CMake 基础上组织 ROS 包。`package.xml` 声明包和依赖，`CMakeLists.txt` 决定编译哪些源文件、链接哪些库。编译完成后，`source devel/setup.bash` 让当前终端找到工作空间中的包。本课程使用 `catkin build`，与前面 CMake 笔记中介绍的 `catkin_make` 区分开，不在同一个工作空间混用。

`ros_control` 负责把控制算法与关节硬件接口连接起来。硬件层提供实际状态和命令入口，控制器读取状态并计算输出，`controller_manager` 负责加载、启停和周期更新控制器。它会检查控制器声明占用的关节资源，避免两个控制器同时争用同一组命令资源。

本次使用 `hardware_interface::EffortJointInterface`。通过 `getHandle()` 按关节名取得 `JointHandle` 后，可以用 `getVelocity()` 读取实际角速度，用 `setCommand()` 写入力矩。这里“控制速度”是控制目标，“输出力矩”是执行手段，两者的单位也不同：轮速是 rad/s，力矩是 N·m。

控制器的 `init()` 读取参数、获取关节句柄并建立通信接口；`starting()` 在启动时重置内部状态；`update()` 每个控制周期计算输出；`stopping()` 在停用时做收尾。本包在停止时将四轮命令设为零。成功加载一个控制器，并不代表它已经处于 `running` 状态。

在 Gazebo 中，`gazebo_ros_control` 把模拟关节接到这些接口上。一个控制周期可以理解为：

```text
读取模拟关节状态 → controller_manager 更新控制器 → 写入关节命令 → 继续物理仿真
```

控制器被编译成 `.so` 动态库，通过 pluginlib 加载。YAML 中的 `type` 是插件查找名，插件 XML 将它对应到 C++ 类和动态库，`package.xml` 导出 XML，CMake 负责生成和安装库。本次类继承 `Controller<EffortJointInterface>`，因此仿真必须提供匹配的接口和关节。控制器没有自己的 `main()`，也不必是一个独立 ROS 节点。

四轮速度控制中，逆运动学先把底盘速度换成四个目标轮速，再由四个独立的 `control_toolbox::Pid` 计算力矩。`update()` 中的核心计算是：

```cpp
measured_[i] = signs_[i] * joints_[i].getVelocity();
error_[i] = target_[i] - measured_[i];
effort_[i] = clamp(pids_[i].computeCommand(error_[i], period), effort_limits_[i]);
joints_[i].setCommand(signs_[i] * effort_[i]);
```

P 根据当前误差出力，I 补偿持续存在的误差，D 根据误差变化率作出反应。四个 PID 必须各自保存积分状态；配置中复用相同初值，不代表运行时共用一个 PID。积分限幅与最终力矩限幅也不同，前者限制积分项，后者限制整个输出。

话题接收频率与控制频率不必相同。本次断点中控制周期约为 1 ms，而轮速诊断约 50 Hz 发布。回调接收指令后通过 `RealtimeBuffer` 传给更新循环，`update()` 按自身周期使用命令。TF 查询放在回调中，状态发布使用 `trylock()`，减少控制循环等待其他线程的情况。

## 2. 遇到的困难

**先把运行环境与构建环境对齐。** 主机是 Ubuntu 24.04，ROS Noetic 和 Gazebo 放在 Ubuntu 20.04 容器中。CLion 使用容器里的编译器和调试器。遇到找不到 ROS 命令或包时，要先检查是否进入了容器、是否加载 ROS 和工作空间环境，而不是直接修改 CMake。`source` 只影响当前终端，新终端需要重新加载。

**参考工程的依赖不能只按文件名猜。** 模型包实际叫 `rm_description_for_task`，参考启动文件却引用 `rm_description`，原模型顶层文件还依赖缺少的 `rm_gazebo` 组件。解决时保留第三方源码，在自己的包中增加兼容 launch 和 xacro，复用底盘模型，改用空世界和 `gazebo_ros_control/DefaultRobotHWSim`。同时检查模型中关节名、transmission 声明与控制器所需接口是否一致。

**先确认控制器加载，再判断控制算法。** 库编译成功，只证明代码能生成动态库。插件名、XML 导出或参数命名空间不匹配，仍可能导致加载失败；关节名写错，则可能在 `init()` 获取句柄时失败。本包加载后的实例名是 `hero_chassis_controller`，插件类型是 `hero_chassis_controller/ChassisController`，这两个名字用途不同。

排查时先看控制器列表，再检查实际反馈：

```bash
rosservice call /controller_manager/list_controllers
rostopic echo -n 1 /joint_states
rostopic echo -n 1 /hero_chassis_controller/front_left/state
```

这些命令应在已加载 ROS 环境、且仿真正在运行的容器终端执行。`joint_state_controller` 发布测量状态，自定义控制器负责输出力矩；有 `/joint_states` 不能单独证明底盘控制器正在工作。

**固定力矩能让轮子转，不等于轮速受到控制。** 参考控制器按预设表输出力矩，新控制器则使用目标轮速与实际轮速的差计算输出。四轮顺序固定为左前、右前、左后、右后。只前进时四轮同号，左移时左前和右后为负。方向不对时先检查轮序和符号，不能靠修改 PID 增益修正运动学错误。

本模型轮半径是 0.07625 m。仅输入前进速度 0.2 m/s 时，四轮目标约为 `0.2 / 0.07625 = 2.623 rad/s`。这个数是 PID 的目标，不能直接作为 Effort 接口的力矩命令。测试还要看 Gazebo 中的实际车体位移：simple 滚子配置下曾出现轮子有反馈、车体位移很小的现象，使用 realistic 滚子后运动恢复正常，因此接触模型也会影响结果。

**调参要看反馈，并保存最终配置。** 使用 rqt_plot 对比 `set_point`、`process_value` 和 `error`，用 rqt_reconfigure 修改各轮 PID。此次测试中左前轮 P 从 0.8 改为 0.7 后生效，随后恢复。在线参数不会自动写回 YAML，确定使用的新值需要保存到配置文件。测试输出限制时，临时增大增益后力矩达到并被限制在 5 N·m。

输入指令跳变后，目标轮速并没有立即跳到最终值，因为前面还有加速度限制。分析曲线时需要分清输入指令、限速后的轮速目标和实际反馈，避免把目标斜坡误判成 PID 跟随太慢。默认停止接收指令超过 0.3 秒后，目标归零，PID 继续制动，车体仍需要一定物理时间才能停下。

**CLion 要附加到真正执行控制器的进程。** 本次控制器运行在 `gzserver` 中，`gzclient` 负责图形显示。对 `init()`、`update()` 和力矩输出位置设置断点后附加 `gzserver`，才能查看关节句柄、误差、积分和输出。断点暂停时仿真时钟也会停，不能在这时判断超时停车是否正常。看完应禁用断点再继续运行，否则高频更新会反复停住。

**里程计和世界模式要明确坐标与时间。** 里程计根据实际轮速做正运动学，再积分位置，不能直接积分目标速度。TF 中 `odom → base_link` 表示车体相对里程计原点的位置和朝向；车体内部各关节的 TF 由 robot_state_publisher 根据关节状态发布。世界模式用 `transformVector()` 把 odom 中的速度方向转到 base_link，TF 缺失或过期时清零目标。位置有平移量，速度向量的方向转换不能直接照搬位置变换。

仿真使用 `/clock` 和 `/use_sim_time`。PID 的积分、微分及里程计积分都依赖周期，不能假设每次更新耗时相同，再用固定常数代替传入的 `period`。

## 3. 反思

接口让同一套控制算法有机会用于仿真和真实机器人，但真实硬件仍需要驱动去读取编码器并发送电机命令。换成实机后，还必须核对传动比、正方向、单位和通信周期。接口类型相同，不意味着仿真中的 PID 参数和力矩限制可以直接照搬。

轮速跟得准，也不代表车体位置一定准。打滑时轮子确实在转，但轮式里程计可能高估位移。后续可以思考如何加入 IMU 或其他定位信息，以及怎样区分“轮速控制误差”和“定位误差”，而不是把所有偏差都归因于 PID。

`ros_control` 提供了组织周期控制的方法，但用了它也不自动得到硬实时保证。TF 等待、线程竞争和仿真计算量都可能影响执行周期。排查抖动时，除了调整增益，也需要检查周期是否稳定、反馈是否及时、是否存在错误的符号或单位。

验证控制器时，正常运动只是其中一部分。指令中断后是否停车、TF 不可用时是否停止、输出触及上限后怎样恢复，都值得单独测试。阅读代码也可以沿着一条具体指令追踪：它在哪里接收，在哪里变成轮速，又在哪里变成力矩，最后通过什么数据判断它真的生效。

