# 仿真操作与验收

## 先分清三个窗口

Ubuntu 主机运行 CLion；名为 hero-stage0 的 Docker 容器运行 ROS Noetic、Gazebo 和编译器。
Gazebo 显示机器人，RViz 显示机器人坐标和里程计，rqt_plot 显示轮速曲线。

工作空间有两种路径，但它们指向同一批源文件：主机中的课程工作空间，在容器内叫
`/root/catkin_ws`。本包位于它下面的 `src/hero_chassis_controller`。

课程仓库中这份文件位于 `2027/SaoF-A/homework/hero_chassis_controller`，是提交用的副本。
原课程环境的 Docker 仍读取原工作空间的源码。修改提交副本后，需要把包同步到
catkin 工作空间的 src 中再编译，修改才会体现在仿真中。单纯在这里编辑不会改变正在运行的控制器。

下文标明“主机”的命令在普通 Ubuntu 终端运行；标明“容器”的命令先进入容器再运行。
不要在 Ubuntu 24.04 主机中直接执行 roslaunch，因为 ROS 安装在容器里。

## 1. 进入 ROS 环境

主机终端：

```bash
docker start hero-stage0
docker exec -it -e DISPLAY=:0 -e XAUTHORITY=/tmp/hero-stage0.Xauthority \
  -e LIBGL_ALWAYS_SOFTWARE=1 hero-stage0 bash
```

进入后，终端提示符通常以 root 开头。每打开一个新的容器终端，都执行：

```bash
source /opt/ros/noetic/setup.bash
source /root/catkin_ws/devel/setup.bash
cd /root/catkin_ws
```

第一条让终端找到 ROS 命令，第二条让 ROS 找到自己编译的包，第三条进入工作空间。
用 `rosversion -d` 检查，应该输出 `noetic`。若重启后图形窗口无法连接显示器，
在主机课程工作空间运行 `bash scripts/start_stage0_container.sh`，它会刷新 X11 访问凭据。

## 2. 用 CLion 编译和启动

打开课程工作空间；当前 CMake 根目录应是 `src/hero_chassis_controller`。
在“设置 → 构建、执行、部署 → CMake”选择 `Hero Controller Debug`，工具链为“远程主机”。
工具链实际连接 Docker 容器中的 `root@localhost:2222`。

右上角先选择 `Hero Controller Build` 并运行。它先编译动态库，再运行 `/usr/bin/true`；
正常结果是 Build 成功、程序退出码 0。随后选择 `Hero Simulation` 并运行。
该配置也会先编译，然后启动 ROS、空世界、机器人和两个控制器。

启动成功的标志：Gazebo 中出现英雄底盘；下列命令中两个控制器均为 `running`：

```bash
rosservice call /controller_manager/list_controllers
```

两个名字应是 `joint_state_controller` 和 `hero_chassis_controller`。
前者发布关节测量，后者控制四个主轮。只启动一次仿真，不要同时运行旧参考仿真。

命令行备用启动方式如下；课程展示仍使用 CLion 的 Build、Run 和 Debug：

```bash
roslaunch hero_chassis_controller simulation.launch
```

`Hero Reference Simulation` 属于必做参考工程，只循环输出固定力矩，不能用于测试新包的 /cmd_vel。

## 3. 先让机器人前进，再检查停车

在另一个已加载 ROS 环境的容器终端运行：

```bash
rostopic pub -r 20 /cmd_vel geometry_msgs/Twist '{linear: {x: 0.2}}'
```

含义是每秒发送 20 次“向车头方向以 0.2 米/秒移动”。省略的字段默认为零。
底盘会逐渐加速，四个目标轮速最后约为 2.623 rad/s。按 Ctrl+C 停止发送。
超过 0.3 秒未收到新指令后，目标轮速立即归零，PID 让车轮减速；物理车体不会瞬间静止。

分别运行下面每一条，观察后按 Ctrl+C，再运行下一条：

```bash
# 后退
rostopic pub -r 20 /cmd_vel geometry_msgs/Twist '{linear: {x: -0.2}}'
# 向车体左侧横移
rostopic pub -r 20 /cmd_vel geometry_msgs/Twist '{linear: {y: 0.15}}'
# 逆时针原地转向
rostopic pub -r 20 /cmd_vel geometry_msgs/Twist '{angular: {z: 0.5}}'
```

想主动减速停止，可先结束上一条发布命令，再运行：

```bash
rostopic pub -1 /cmd_vel geometry_msgs/Twist '{}'
```

不要同时打开多个速度发布器，否则它们会交替覆盖对方的指令。

## 4. 看轮速曲线和在线调参

新容器终端运行：

```bash
roslaunch hero_chassis_controller inspect.launch
```

它打开 RViz、rqt_plot 和 rqt_reconfigure。只需其中部分窗口时，可加
`rviz:=false`、`plot:=false` 或 `tune:=false`。

rqt_plot 默认显示左前轮三个量：`set_point` 是目标轮速，`process_value` 是实际轮速，
`error` 是两者之差。横轴是时间，纵轴是 rad/s。发送前进指令后，目标从零爬升，实际值跟随；
停车后两者应回到零附近。小幅抖动来自轮地接触和测量变化，并不要求误差永远为零。

rqt_reconfigure 中展开 `/hero_chassis_controller/pid/front_left`。
P 决定看到误差后用多大力，I 补偿长期误差，D 对误差变化作出反应。
可以先把 p 从 0.8 改为 0.7，重复同一条前进指令，比较实际轮速跟随情况，再恢复 0.8。
右前、左后、右后在另外三个独立目录中，各有自己的 PID 和积分状态。

本次保存的四轮参数相同：`p=0.8, i=0.1, d=0.001, i_clamp=0.5`，力矩上限 5 N·m。
在线修改只影响当前运行；确定使用新值后，要同步修改 `config/controllers.yaml` 才能下次继续使用。
YAML 的 `&wheel_pid` 和 `*wheel_pid` 只是复用初始数值，不会让四个运行中的 PID 共用状态。

## 5. 理解里程计和 TF

```bash
rostopic echo -n 1 /odom
rosrun tf tf_echo odom base_link
```

`/odom` 中 `header.frame_id` 应为 odom，`child_frame_id` 应为 base_link。
pose 表示车在 odom 中的位置和朝向，twist 表示车体自身坐标下的实际速度。
`tf_echo` 显示同一组坐标关系，按 Ctrl+C 退出。
RViz 的 Fixed Frame 已设为 odom，能看到模型、TF 坐标轴和里程计箭头。

这里的里程计是“用实际轮速估计车走了多少”，并非读取 Gazebo 真值。
轮子打滑时会有误差；重启控制器后里程计原点会重置。

## 6. 对比世界模式和底盘模式

先用第 3 节转向命令，让车头转约 90°，然后停止发布。
默认底盘模式再发送 `linear.x=0.2`，车沿当前车头方向走。

结束发布后，在调参窗口选择 `/hero_chassis_controller`，勾选 `world_mode`。
再发送完全相同的 `linear.x=0.2`，车会沿初始 odom 的 X 轴走，不会随车头方向改变。
也可通过命令切换：

```bash
rosrun dynamic_reconfigure dynparam set /hero_chassis_controller world_mode true
# 观察后恢复底盘模式
rosrun dynamic_reconfigure dynparam set /hero_chassis_controller world_mode false
```

世界模式使用 TF 把输入速度转换到车体坐标。若指定的 world_frame 不存在或变换过期，
控制器会打印 `World command stopped` 并清零目标轮速。
世界指令中的“世界”默认是轮式里程计 odom，并不是 Gazebo 的绝对世界坐标。

## 7. 键盘驾驶

结束所有 rostopic 速度发布器，在容器终端运行：

```bash
roslaunch hero_chassis_controller teleop.launch
```

在弹出的键盘终端中操作：i 前进，逗号后退，j/l 转向，k 停车；
按大写 J/L（Shift+j/l）横移。必须把输入焦点放在该终端。
松开按键约 0.2 秒后键盘节点发送零速度；即使它退出，控制器的 0.3 秒超时仍会停车。
Ctrl+C 关闭键盘节点。默认速度 0.2 m/s、转速 0.5 rad/s，并限制加速度避免突变。

## 8. 在 CLion 中看 PID 的真实计算

先启动 Hero Simulation。在 `src/chassis_controller.cpp` 找到
`joints_[i].setCommand(signs_[i] * effort_[i]);` 这一行设置断点。
断点条件可设为 `i == 0 && active`，只观察左前轮收到有效指令的周期。

通过“运行 → 附加到进程”，选择 `gzserver`，调试器选择 `Hero Attach via Docker`。
不要选择名字相似的 gzclient，它负责图形显示，不执行控制器。
在另一个终端发送前进命令。命中后查看：

| 表达式 | 含义 |
|---|---|
| `dt` | 本周期长度，实测约 0.001 秒 |
| `target_[0]` | 左前轮目标速度 |
| `measured_[0]` | 左前轮实际速度 |
| `error_[0]` | 目标减实际 |
| `pids_[0].i_error_` | 累积的误差积分；乘 I 后才是积分力矩 |
| `pids_[0].d_error_` | 误差变化率 |
| `effort_[0]` | 经过限幅的输出力矩 |

断点会暂停整个物理仿真，曲线停止更新是正常现象。看完先禁用断点，再点继续；
否则 1 ms 后可能再次停住。不要在暂停状态下判断超时功能是否正常。

## 9. 重跑自动验收

保持仿真运行，关闭其他速度发布器，并禁用所有演示断点。在容器终端运行：

```bash
python3 /root/catkin_ws/src/hero_chassis_controller/test/verify_simulation.py \
  --output /tmp/hero-simulation.json
```

脚本会实际驾驶机器人，打印每项 PASS/FAIL，把原始测量保存在指定 JSON 中。
包括前后横移转向、在线修改增益、力矩饱和、超时、世界模式、TF 故障停车。
TF 故障测试会短暂重新加载控制器，结束时恢复 odom 和底盘模式，因此里程计原点会重置。
应在空世界中运行，不要和手动驾驶同时进行。

运动学单元测试不需要启动 Gazebo：

```bash
cd /root/catkin_ws
catkin build hero_chassis_controller --no-deps --no-status \
  --make-args run_tests_hero_chassis_controller
catkin_test_results build/hero_chassis_controller/test_results
```

第一条只运行本包测试，避免把本包专属的 make 目标传给第三方包。第二条汇总测试结果。
任何 FAIL 都要查看具体输出，不能只看文件是否生成。

## 10. 常见问题

| 现象 | 首先检查 |
|---|---|
| roslaunch 找不到命令 | 是否已进入容器并 source `/opt/ros/noetic/setup.bash` |
| 找不到自己的包 | 是否 source `/root/catkin_ws/devel/setup.bash`，包是否编译成功 |
| 车一直自行循环运动 | 是否误启动了 Hero Reference Simulation |
| 有窗口但车不动 | Gazebo 是否暂停、控制器是否 running、是否仍停在断点 |
| /cmd_vel 有值但很快停 | 单次指令会超时，手动驾驶用 `-r 20` 持续发布 |
| 世界模式车不动 | 查看 World command stopped 警告和 odom→base_link TF |
| 图形窗口打不开 | 检查 DISPLAY/XAUTHORITY，运行主机重启脚本刷新凭据 |
| CLion 搜索不到 gzserver | 先启动仿真；用课程配置的 Docker GDB 调试器 |

## 11. 干净提交

Git 仓库根目录是 missing-semester，当前目录是其中的一个 ROS 包。
在 missing-semester 根目录检查和暂存本次作业：

```bash
git status --short
git diff --check
git add -- 2027/SaoF-A/homework/hero_chassis_controller
git diff --cached --stat
git diff --cached --name-only
```

只提交源码、配置、启动文件、测试、许可和使用文档。工作说明、对话记录、
CLion 私有设置、日志、构建目录和第三方仓库都不应出现在文件列表中。
确认暂存内容后创建本地提交，消息使用英文大写开头并以句号结束：

```bash
git commit -m "Add mecanum chassis controller and verification documents."
```

commit 保存本机版本；push 才会上传到远程。不要把其他尚未检查的学习笔记混入本次代码提交。
