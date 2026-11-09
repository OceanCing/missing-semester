# ROS 学习笔记（配合本次作业）

> 这份笔记按"概念 + 我自己的实测"整理。前面是查资料能对上的结论，【实测】都是我在这台机器上跑出来的。

---

## 1. ROS 到底是什么

不是操作系统内核，而是**跑在 Linux 之上的一层中间件 + 工具箱**：
- 通信中间件：话题（数据流）、服务（一问一答）、参数（配置表）
- 工具链：`catkin_make` 编译、`roslaunch` 编排、`rviz`/`rqt` 看、`rosbag` 录
- 生态：几万个现成的包（本作业里的 `smb_gazebo`、`pointcloud_to_laserscan` 就是别人的包）

**排错先分层**（三层架构）：

| 层 | 管什么 | 典型报错 |
|---|---|---|
| 文件系统层 | 包在哪、源码怎么组织 | `Package 'xxx' not found`（没 source 或没放进 src） |
| 计算图层 | 谁发布、谁订阅、名字/类型对不对 | 话题有但收不到数据（类型不匹配、名字不对） |
| 通信层 | TCP 连接、队列、序列化 | 延迟高、丢数据（`queue_size` 太浅） |

---

## 2. 工作空间与包

```
~/ros_ws/               工作空间
├── src/                源码（进版本库的只有它）
├── build/              编译中间文件（可删）
└── devel/              产物：可执行文件、生成的头文件、setup.bash
```

- 包 = 一个目录 + `package.xml` + `CMakeLists.txt`
- **每开一个新终端都要 `source devel/setup.bash`**，否则找不到自己的包
- `package.xml`：名字/版本/依赖；`<depend>` = 编译+运行都要
- 依赖分为"编译要的头文件"和"运行要的库/消息类型"

**【实测】第一次编译失败**

报的是：

```
fatal error: smb_highlevel_controller/ObstacleMonitorConfig.h: No such file or directory
```

我一开始以为是 CMakeLists 里 `generate_dynamic_reconfigure_options` 写错了，反复改了两次都没用。
后来才明白：`cfg/ObstacleMonitor.cfg` 是个 **Python 脚本**，catkin 只有在这个文件**可执行**的时候才会去跑它、进而生成
`ObstacleMonitorConfig.h`。我拷文件时权限丢了，所以头文件根本没被生成。

```bash
chmod +x cfg/ObstacleMonitor.cfg   # 加完这条，重新 catkin_make 就过了
```

这件事让我记住了：**"文件在"不等于"文件能用"**，权限也参与编译。

---

## 3. 五个零件（用类比帮助记忆）

| 零件 | 一句话 | 类比 | 本作业里 |
|---|---|---|---|
| 节点 node | 一个进程干一件事 | 工人 | `scan_processor`、`obstacle_monitor` |
| 话题 topic | 多对多、异步的数据流 | 广播电台 | `/scan`、`/rslidar_points`、`/homework/obstacle` |
| 消息 message | 数据格式（`.msg` 定义） | 信封格式 | `ObstacleDistance` |
| 服务 service | 一对一、同步的一问一答 | 打电话 | `/homework/set_warning_distance` |
| 参数 param | 全局配置表 | 墙上的配置表 | `~warning_distance`、`~queue_size` |

**Master 只管注册和牵线，数据不经过它** —— 所以 `roscore` 挂了，已经连上的节点还能继续跑。

---

## 4. 激光：LaserScan 与"无效测量" 【重点】

`LaserScan` 关键字段：`angle_min`、`angle_max`、`angle_increment`、`range_min`、`range_max`、`ranges[]`。

**无效测量有四种写法**（不同驱动不一样）：
1. `NaN` —— 没有回波
2. `+Inf` —— 超出量程
3. `0` 或 `< range_min` —— 太近，测不到
4. `> range_max` —— 太远

所以过滤必须四件事一起做：

```cpp
std::isfinite(v) && v > 0 && v >= range_min && v <= range_max
```

**整帧无有效数据必须单独当一种状态**：`valid = false` + `min_distance = 0`。
不能把"没数据"当成"距离 0 米"，否则下游会以为贴着障碍物。

**【实测】第一次看到 `valid=false`**

刚起仿真、世界是空的时候，日志一直在刷：

```
[WARN] LaserScan[rslidar] 整帧无有效测量（全部 NaN/Inf 或越界），发布 valid=false, min_distance=0
```

我一开始以为是我的过滤写错了、把好数据也扔了。后来想明白：**空世界里所有射线都打不到东西，返回的就是 `Inf`**，
过滤之后自然是 0 根有效光束。所以那不是 bug，是"这一帧确实没有障碍物"。

**【实测】有效光束的数字很关键**

| 方块尺寸 | 打到它的光束 |
|---|---|
| 0.4 × 0.4 × 1.0 | **16 / 362** |
| 0.9 × 0.9 × 1.5 | **约 30 / 362**（放进去后 24~40 波动） |

362 是一帧的总光束数。方块越小，占的角度越窄，**稍微挪一点就可能一根都打不到** ——
这也解释了后面录 bag 为什么老是录成"全 0"。

---

## 5. 点云：PointCloud2 的点数

- 结构：`width`、`height`（无序点云 height=1）、`fields[]`、`point_step`、`row_step`、`data`
- **点数 = width × height**（不是字节数除以步长，那样会算进 padding）
- 本作业只要求点数，所以不需要真的拆 x/y/z；要拆可以用 `pcl::fromROSMsg`

**【实测】**日志里看到的是 `3520`、`3616`、`3712`、`3760`、`3840` 这些数字，
`height=1`、`point_step=22`。方块靠近时点数会变多（因为打到方块的 3D 点也算进去了）。

---

## 6. 参数服务器

```cpp
pnh_.param<std::string>("laser_topic", laser_topic_, "/scan");  // 读一次，存进成员变量
```

**关键结论**：节点只在构造时读一次，之后**参数服务器再变，正在跑的节点不会变**。

所以：
- `rosparam set` 改完 → `rosparam get` 是新值，但**节点行为不变**，要重启才生效
- 想"改了立刻生效"，只有三条路：回调里实时 `get`、监听 `parameter_events`、用 `dynamic_reconfigure`

**【实测】我自己做的实验**

```bash
rosparam get /obstacle_monitor/warning_distance     # 1.0
rosparam set /obstacle_monitor/warning_distance 0.5
rosparam get /obstacle_monitor/warning_distance     # 0.5  ← 服务器上确实变了
```

但终端 2 的日志里，节点还在打印：

```
距离 = 2.52 m，阈值 = 0.800 m → 正常
```

**阈值还是 0.8**（那是我之前用服务改过的值），完全不受 `rosparam set` 影响。
这一下就把"参数服务器只是存值的地方"这件事看明白了 —— 节点不看它，看自己内存里的那份。

---

## 7. dynamic_reconfigure 【作业第 9 题】

链路：`cfg/*.cfg`（Python 声明参数）→ `generate_dynamic_reconfigure_options()` 生成 `Config` 类
→ 节点里 `dynamic_reconfigure::Server<Config>` + `setCallback(...)`

**它本质上就是"参数服务器 + 一个变化回调"**，所以能在线生效。

设计取舍：本作业 cfg 里**只有** `warning_enabled`，不放 `warning_distance` ——
一个参数只让一套机制管，避免"rqt 改了又被服务覆盖"。

**【实测】在 rqt_reconfigure 里取消勾选之后**

- 终端 2 立刻多一行：`[INFO] [dynamic_reconfigure] warning_enabled = false`
- 之后**不再打印 `[报警]`**，但 `距离 = ...` 还在照常刷 —— 说明节点没退订、只是不报警了
- 这时候再调服务，仍然返回 `success: True`，阈值也确实被改了

也就是说：**开关只控制"报不报警"，不控制"收不收数据"、也不控制"服不服务"**。
勾回来之后立刻恢复正常报警。这个效果用普通 `rosparam` 是做不到的（要重启节点）。

---

## 8. 服务设计：`return true` 和 `success=false` 为什么能同时出现

```cpp
res.success = false;   // 业务失败：拒绝了非法值
return true;           // 通信成功：请求确实送达并处理了
```

如果非法请求返回 `false`，客户端会以为**通信出错**，拿不到 `message` 里的拒绝原因。
作业要求"拒绝修改并保留原值"，所以必须让客户端能看到原因。

**【实测】四条非法值的真实输出**

```
distance=0.0    -> success=False 拒绝：报警阈值必须大于 0（收到 0.000000），当前阈值保持 0.800000 m
distance=-1.5   -> success=False 拒绝：报警阈值必须大于 0（收到 -1.500000），当前阈值保持 0.800000 m
distance=nan    -> success=False 拒绝：报警阈值必须是有限值（收到 NaN 或 Inf），当前阈值保持 0.800000 m
distance=inf    -> success=False 拒绝：报警阈值必须是有限值（收到 NaN 或 Inf），当前阈值保持 0.800000 m
```

这是用 Python 客户端直接传 `float('nan')` / `float('inf')` 测出来的。

**顺便学到的**：一开始我用 `rosservice call ... "distance: inf"`，报的是

```
ERROR: Unable to send request. One of the fields has an incorrect type:
  field distance must be float type
```

命令行把 `inf` 当成字符串，转不成 `float32`，**请求压根没发出去**。
所以那不算"服务端拒绝"，拿它当证据是错的 —— 必须用真正的客户端发。

---

## 9. launch

```xml
<arg name="laser_enabled" default="true"/>
<include file="$(find smb_gazebo)/launch/smb_gazebo.launch">
  <arg name="laser_enabled" value="$(arg laser_enabled)"/>
</include>
<rosparam command="load" file="$(find smb_highlevel_controller)/config/obstacle.yaml"/>
<node pkg="..." type="scan_processor" name="scan_processor" output="screen"/>
```

- `$(find 包名)` = 可迁移路径（换机器不用改）
- `type` = 可执行文件名，`name` = 节点在计算图里的名字（两者可以不同）
- `rosparam load` 的 YAML 第一层键要和节点 `name` 一致，参数才会落进私有命名空间

**【实测】YAML 第一层为什么要写节点名**

`obstacle.yaml` 第一层写的是 `scan_processor:` 和 `obstacle_monitor:`，
和 launch 里 `<node ... name="scan_processor">` 完全对齐。

因为 `rosparam load` 是把整棵树灌进参数服务器，键会变成
`/scan_processor/laser_topic`、`/obstacle_monitor/warning_distance`；
而节点用的是私有句柄 `NodeHandle("~")`，它的前缀正好就是 `/节点名`。
**两边名字对不上，节点就读不到，会静默用代码里的默认值** —— 这种错最难查，因为不报错。

---

## 10. RViz 与 TF

- **Fixed Frame 是所有显示的参考系**。选 `odom` 是因为它连续、无跳变；`map` 需要仿真发 `map→odom`，没有就报错。
- 保存配置就是覆盖 `.rviz`（YAML 文件）。
- **星号 `*` 的含义**：配置被修改过。注意 —— 你保存完再转动视角，它会重新出现，不代表保存失败。

**【实测】PointCloud2 的 Size**

一开始 Size 是 `0.03`，画面里几乎看不到点云，我以为点云没数据；
把 `Size (m)` 改成 `0.1`、`Color Transformer` 改成按轴/按高度上色之后，方块的点云变成一块很明显的彩色方柱，
和红色的激光同心圆一眼就能分开。

**星号那件事**：我保存了好几次，标题里 `homework.rviz*` 的 `*` 一直在，我以为没保存成功。
后来对比了两版 `.rviz` 文件，发现差异只有相机视角参数（Pitch/Yaw/Distance），配置项都存进去了 ——
**是因为我保存完又转了视角，RViz 就又把它标成"已修改"**。

---

## 11. rosbag

```bash
rosbag record -O out.bag /homework/obstacle
rosbag info out.bag
rosbag play out.bag
```

- bag 里存的是**话题消息 + 连接信息**，不含服务调用、不含参数修改
- **msg 定义改了，老 bag 可能读不了**（MD5 变了）

**【实测】为什么录了 250 多条却全是 `valid=false`**

这是这次花时间最多的一段。终端一直在刷日志，看起来一切正常，
但把 bag 解出来看，里面每一帧都是 `valid=false`、`min_distance=0`。

原因有两个：

1. 我用的方块只有 0.4 m，只有 **16/362** 根光束打到它，容错极小；
2. 我用 `gz model -x 0.7` 把它推到离车只有 0.7 m，**进了雷达的最小量程**，之后就一直无效了。

改成大方块（0.9 m）、并且只让它在 **1.5~3.0 m** 之间移动之后才正常。
另外发现 `gz model` 这个命令行工具在我的环境里不太可靠（报过 `An instance of Gazebo is not running.`），
换成 `rosservice call /gazebo/set_model_state` 才稳定。

**【实测】最后合格的 bag**

```
messages   : 246
duration   : 24.6 s
valid true : 246 / 246  (100%)
range      : 0.92 ~ 2.53 m
```

距离变化是 `2.52 → 1.52 → 0.92 → 2.02` 这样一档一档的，正好对应方块 3.0 → 2.0 → 1.4 → 2.5 的位置。

---

## 12. nodelet 【加分项】

- 问题：点云/图像很大，`node→node` 要走序列化 + TCP，浪费 CPU 和延迟
- 办法：写成插件（`.so`），被 manager 加载进**它的进程**，同 manager 内用 `shared_ptr` 零拷贝
- 代价：一个 nodelet 崩了会带走整个 manager
- 三件套：`.cpp` 里 `PLUGINLIB_EXPORT_CLASS` + `.xml` 描述 + `package.xml` 的 `<export>`

**【实测】"插件不是节点"这件事**

我本来以为加了 nodelet 后 `rosnode list` 里应该只剩 manager。实际跑起来是这样的：

```bash
rosnode list | grep -E 'scan_processor|obstacle_monitor|manager'
# /obstacle_monitor
# /scan_processor
# /smb_highlevel_controller_manager
```

两个插件名字**也在列表里**。但它们其实是 `nodelet load` 起的"加载器"节点，本身不处理数据。
真正的证据是这两条：

```bash
rosservice call /smb_highlevel_controller_manager/list
# nodelets:
#   - /obstacle_monitor
#   - /scan_processor
```

```bash
rostopic info /homework/obstacle
# Publishers:  * /smb_highlevel_controller_manager (http://baozi-DRB-P:41217/)
# Subscribers: * /smb_highlevel_controller_manager (http://baozi-DRB-P:41217/)   ← 同一个端口！
```

**发布者和订阅者都是 manager、而且端口完全相同**，说明两个插件的端点注册在同一个进程里。
再加一条 `rosnode info /smb_highlevel_controller_manager`，能看到

```
* topic: /homework/obstacle
  * to: /smb_highlevel_controller_manager
  * transport: INTRAPROCESS
```

`INTRAPROCESS` 就是"同进程内部传递，没走网络"，这就是零拷贝的直接体现。

---

## 13. 我踩过的坑

| # | 现象 | 原因 | 解决 |
|---|---|---|---|
| 1 | `docker pull` 报 `dial tcp [2a03:2880:...] i/o timeout` | DNS 污染，解析到假 IPv6 | 关 IPv6 + 换镜像源（`hub.rat.dev`） |
| 2 | `unzip` 报 `appears to use backslashes` | Windows 打的 zip 用反斜杠路径 | 用 `tar.gz`，或用 python 把 `\` 换成 `/` |
| 3 | 编译报 `ObstacleMonitorConfig.h: No such file` | `cfg/*.cfg` 没有执行权限 | `chmod +x cfg/ObstacleMonitor.cfg` |
| 4 | `apt` 报 `NO_PUBKEY F42ED6FBAB17C654` | 镜像里没有 ROS 公钥 | `apt-key adv --keyserver ... --recv-key ...` |
| 5 | `container ... is not running` | 重启后容器不会自动起 | 每次开机 `docker start ros_noetic` |
| 6 | `Can't open display: :0`，RViz 崩 | `xhost` 授权重启后失效 | 每次开机在**宿主机** `xhost +local:` |
| 7 | `xhost: command not found` | 在容器里跑了它 | 只在宿主机跑 |
| 8 | 命令敲了没反应 | `rqt_*`/`roslaunch` 占住终端 | 关窗口或 `Ctrl`+`C` |
| 9 | 文件名含 `<` `:`，Windows 打不开 | Linux 允许、Windows 不允许 | 拷贝时用 `sed 's/[:*?"<>|]/_/g'` 改名 |
| 10 | bag 全是 `valid=false` | 场景没障碍物 / 方块贴太近 | 录之前确认日志有"最近有效距离 = X m" |
| 11 | `rostopic hz` 一直刷，后面命令全被吞 | 前台程序占着终端，输入进了它的缓冲区 | 用 `timeout 5 rostopic hz /scan` 让它自己停 |
| 12 | 拉起 Ubuntu 后打不开 D 盘 | Windows 休眠留下锁，Linux 拒绝挂载 | Windows 里 `powercfg -h off` + `chkdsk D: /f` |

**【实测】印象最深的一条**

是第 10 条。因为它不是"报错"，而是"看起来一切正常"——
终端一直在打印，我甚至把它录进视频里，最后才发现里面一条有效数据都没有。
从那以后我判断"做成了没有"，会去找一个**能验证的数字**（有效帧占比、`success` 到底是 True 还是 False），
而不看屏幕上有没有红字。

---

## 14. 我的收获

以前我以为"参数改了节点就会用新值"，做了第 6 题才发现根本不是——
节点在构造时读一次就忘了参数服务器，要改就得重启，或者专门写回调。

以前我以为"终端在打印就是成功"，录 bag 那次吃了大亏：250 多条数据全是无效的，
却因为屏幕上一直有日志而毫无察觉。现在会先解出数据看一眼再说。

对 AI 给的代码，我的态度也变了：它给的 `grep`、`sed`、launch 写法大部分是对的，
但至少有七八处是"跑起来才发现不对"的（`nodelet list` 在 Noetic 里不存在、`rosservice call` 传 `inf` 会被客户端拦下、
`gz model` 在我环境里不可靠、容器里的 `xhost` 根本没有）。所以现在我会**先跑一遍再判断它对不对**，
而不是照着抄。

最后也说个自己的问题：这次的环境折腾得太久（Docker 拉不下来、公钥丢了、磁盘被 Windows 休眠锁住），
真正写代码的时间反而不多。下次做之前应该先把环境跑通、把"怎么验证"想清楚，再动手。