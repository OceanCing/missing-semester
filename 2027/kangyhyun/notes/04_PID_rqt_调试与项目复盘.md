# ROS1 学习笔记 4：PID、rqt、调试与项目复盘
这份笔记包含 PID、rqt、CLion 断点调试

## 1. PID 在控制什么
`P`按当前误差出力 ,若误差越大，推得越猛 
`I` 累积历史误差
`D` 看误差变化速度 ,快到头时往回拉，抑制超调 
每个轮子一个 `control_toolbox::Pid`。
输入：
```
e = 目标轮速 - 实际轮速
```

输出：
```
u = 给轮子的力矩
```

公式：
```
u = Kp * e + Ki * ∫e dt + Kd * de/dt
```
四个轮子用四个 PID，因为摩擦、负载、离地状态和装配误差不同。

## 2. 代码里的 PID

```cpp
double e = speed[0] - joint_lf.getVelocity();
double u = pid_lf.computeCommand(e, dt);
joint_lf.setCommand(u);
```

`computeCommand(e, dt)` 返回力矩，`setCommand(u)` 把力矩写给关节。

### `readPid()` 逐词解释

```cpp
static void readPid(ros::NodeHandle nh, std::string name,
                    control_toolbox::Pid& pid)
{
    double p = nh.param(name + "/p", 1.0);
    double i = nh.param(name + "/i", 0.0);
    double d = nh.param(name + "/d", 0.0);
    double limit = nh.param(name + "/i_clamp", 1.0);
    pid.initPid(p, i, d, limit, -limit);
}
```
- `void` 不返回值，只修改传入的 PID 
- `ros::NodeHandle nh` 读取 ROS 参数的句柄 
- `std::string name`  参数前缀，例如 `left_front_pid` 
- `control_toolbox::Pid& pid`  引用，直接修改调用者的 PID
- `nh.param("...", 1.0)`  读参数，找不到就用默认值 
- `name + "/p"`  拼出 `left_front_pid/p` 
- `i_clamp`  积分限幅 
- `initPid(..., +limit, -limit)`  设置积分上下限 

`initPid()` 返回 `void`，所以不能写：

```cpp
return pid.initPid(...);   // 错误
```

只能先调用，再 `return true`。

## 3. YAML 和 PID 的对应关系

```yaml
controller:
  hero_chassis_controller:
    left_front_pid: {p: 1.0, i: 1.0, d: 0.0, i_clamp: 2.0}
```

代码：

```cpp
readPid(nh, "left_front_pid", pid_lf);
```

对应关系：

```
left_front_pid/p       -> p
left_front_pid/i       -> i
left_front_pid/d       -> d
left_front_pid/i_clamp -> 积分限幅
```

参数只在 `init()` 时读取一次，所以修改 YAML 后必须重启 `roslaunch`。

## 4. PID 调参顺序

1. 先把 `i` 和 `d` 设为 0，`p` 从 1.0 开始。
2. 发 `linear.x = 0.2`，用 `rqt_plot` 看 `/joint_states/velocity[0]`。
3. 太慢、太肉：把 `p` 加到 2、3、5。
4. 来回抖：把 `p` 减半。
5. P 稳定后加 `i`，一次加 0.2～0.5，消掉稳态误差。
6. `i_clamp` 保持 1～2，防止积分爆炸。
7. D 先保持 0；明显超调时再加 0.01～0.05；更抖就回 0。
8. 四个轮子分别调，最后看直线是否跑偏。

## 5. rqt 工具


- `rqt_plot`  画 Topic 曲线，例如轮速 
- `rqt_reconfigure`  在线修改 `dynamic_reconfigure` 参数 
- `rqt_graph`  查看节点和 Topic 连接关系 
- `rqt_console`  查看 ROS 日志 

### `rqt_plot`
```
rosrun rqt_plot rqt_plot \
  /joint_states/velocity[0] \
  /joint_states/velocity[1] \
  /joint_states/velocity[2] \
  /joint_states/velocity[3]
```

### `rqt_reconfigure`

要在线调 PID，节点必须：

1. 有 `.cfg` 配置文件；
2. CMake 生成 dynamic_reconfigure 代码；
3. 启动 `dynamic_reconfigure::Server`；
4. 回调里更新 PID。

## 6. CLion 调试

### 不能直接 Run

控制器不是独立进程，而是被 `controller_manager` 加载进 `gzserver` 的插件，所以只能：
通过Attach to Process 找到id最新的gzserver


### 正确步骤

```
catkin build simple_chassis_controller \
  --cmake-args -DCMAKE_BUILD_TYPE=Debug

roslaunch simple_chassis_controller run_simulation_and_controller.launch
```

Gazebo 点 play，然后：

```
pgrep -a gzserver
```

CLion：Ctrl+Shift+A → Attach to Process → 选 gzserver


断点：

```cpp
state_ = (state_ + 1) % 6;
```

点 Resume / F9，等断点命中。此时变量窗口才会显示 `state_`、`cmd_` 等。

### “帧不可用”是什么意思

它表示调试器已经连接，但当前没有停在源码栈帧里：

- 程序还在运行；
- 或 Gazebo 暂停；
- 或断点没有命中；
- 或 attach 到了旧/错误进程。


### 结束调试

使用：Ctrl+Shift+A → 断开连接


## 7. 可能遇到的真实问题

### YAML 改错后改回来仍跑不了

原因不是 YAML，而是旧 `gzserver/gzclient` 没有退出，产生了僵尸进程和停止状态进程。反复启动 `roslaunch` 后，控制器一直在等 Gazebo 服务。

解决：

```
pgrep -a gzserver
pgrep -a gzclient
ps -eo pid,ppid,stat,cmd | grep -E 'gzserver|gzclient' | grep -v grep
```

- `Z` 是僵尸，不能 kill，只能重启容器/宿主机；
- `tLl` 是被调试器暂停的活进程，必须先结束旧 GDB；
- 清理后只启动一次 launch。

### `process already traced by process`

说明旧 GDB 还占着 `gzserver`：

```
grep TracerPid /proc/<gzserverPID>/status
```

找到 tracer PID，结束它，再结束 `gzserver`，重新启动仿真。

### 僵尸进程

僵尸进程 PPID 为 1，是容器 PID 1 没回收。普通 kill 无效，重启容器或宿主机即可清除。
##总结
 PID 让轮子追上目标速度，rqt_plot 用来看轮速曲线，CLion 通过 attach 到 gzserver 调试插件；YAML 参数只在 init 时读取，修改后必须重启 launch。
