# 控制第三课笔记

## 第 0 章  ROS环境

### 0.1 装在 Docker里的 ROS 

两个概念先分清，后面所有路径都靠它：

| 在哪 | 路径 | 干什么 |
|---|---|---|
| 宿主机终端（Ubuntu 自带的终端） | `~/ros1_ws` | `docker start`、改文件属主 |
| 容器窗口（VS Code 连进容器之后） | `/root/ros1_ws` | 写代码、编译、跑节点 |

同一个目录，挂载出来两个路径写法，内容完全一样。宿主机终端只在开机、修权限的时候用一下。

### 0.2 每次开工的三条命令

```
xhost +local:root   # 在宿主机任意终端放行图形界面（每次重启电脑后要做一次）
docker start ros1 && docker exec -it ros1 bash  # 进容器（宿主终端）
source /opt/ros/noetic/setup.bash  # 让 ROS 的命令能用（容器内）
```

`start` 是开机；`exec -it` 是再开一个终端进同一个容器。为什么不用 `docker run`？`run` 是从镜像新造一个容器，你前装的东西就会没。
提示符变成 `root@一串数字:~#`，说明已经在容器里。
**第三条每开一个新终端都要敲一次**，不然报 `roscore: command not found`。这是环境变量没加载。把它写进容器的 `~/.bashrc`，之后 `docker exec` 进去就可以自动打开。

> 但后续我已经用 VS Code 连进容器，第二~三步都可以省：集成终端本来就开在容器里，而且 `/root/.bashrc` 第 100 行已经有 `source /opt/ros/noetic/setup.bash`，一开终端 ROS 环境就在。这两条只在「宿主终端」里手工敲的时候才用得上。

### 0.3 编辑器：VS Code 直接连进容器（本笔记的用法）

先给结论：**不用往容器里装编辑器，也不用在两个窗口之间来回切。** 宿主机的 VS Code 加一个 Dev Containers 扩展，attach 到 `ros1`，这个窗口里：

- 左侧文件树看到的就是容器内的 `/root/ros1_ws`，改了就是容器里的文件，不用复制粘贴
- 按 `Ctrl` + 反引号（Esc 下面那个键）打开的终端就是容器内的 shell，`catkin_make`、`rosrun` 直接跑
- 语法高亮、跳转、C++ 补全都在，因为扩展是装进容器里的，宿主那边不受影响

六步装法（本机已走通；容器重建后第 3、5 步要重走）：

1. 宿主终端装扩展：`code --install-extension ms-vscode-remote.remote-containers`
2. 确认容器在跑：`docker start ros1 && docker ps --filter name=ros1`
3. VS Code 里按 `F1` → 输入 `attach` → 选「Dev Containers: 附加到正在运行的容器」→ 点 `ros1`，等右下角进度条把 server 装完
4. 「文件」→「打开文件夹」→ 输入 `/root/ros1_ws` → 确定
5. 扩展面板搜 `C++` → 点「**在容器中安装**」。这步最容易漏：宿主的扩展不会自动跟到容器里，漏了 `#include <ros/ros.h>` 还是红线
6. 按 `Ctrl` + 反引号开集成终端，`cd /root/ros1_ws && catkin_make`

这是ai给出的四个坑：

1. **属主会反过来。** 容器窗口以 root 身份读写，你在里面**新建**的文件属主是 root，回到宿主机改不动，保存报 `EACCES`。改回来：`docker exec ros1 chown -R 1000:1000 /root/ros1_ws`（`1000:1000` 在宿主机上就是你的用户，两个系统的用户 ID 是同一个数）。现有文件都还是 1000，所以暂时没事；等你第一次在容器里新建文件才会遇到。
2. **别两边同时开同一个目录。** 宿主窗口开 `~/ros1_ws`、容器窗口开 `/root/ros1_ws`，同一份文件两个身份，谁后保存谁覆盖；而且两边都在索引 `build/`，会卡。只留容器那个窗口。
3. **容器重建要重新 attach。** server 和扩展装在容器自己那层，`docker rm` 一次就没了；代码在挂载卷里，不会丢。这正是「代码必须写在 `~/ros1_ws` 里」的理由。
4. **红线不等于编不过。** 就算第 5 步做了，cpptools 也可能不知道 ROS 头文件在哪，`#include <ros/ros.h>` 照样画红线，但 `catkin_make` 是过的。想把红线也消掉：先 `catkin_make -DCMAKE_EXPORT_COMPILE_COMMANDS=1` 生成编译数据库，用 `find /root/ros1_ws/build -name compile_commands.json` 找到它，再在 `.vscode/c_cpp_properties.json` 里让 `compileCommands` 指向那个文件。

### 0.4 三步报错：

| 环节 | 谁负责 | 报错 |
|---|---|---|
| 编出来 | `catkin_make` | 一堆编译错误、`undefined reference` |
| 找得到 | `source devel/setup.bash` | `[rospack] Error: package 'xxx' not found` |
| 连得上 | `roscore` 在跑 | `Unable to communicate with master` |

## 第 1 章 认识 ROS

### 1.1 ROS = 通信中间件 + 工具链 + 软件生态。

### 1.2 三件东西，各管一段

| 三件东西 | 干什么 | 具体 |
|---|---|---|
| 通信中间件 | 节点之间怎么说话，这是唯一的对话方式 | 话题（连续数据流）、服务（一次问答）、参数（全局配置） |
| 工具链 | 编译、启动、看、录 | `catkin` 编译、`roslaunch` 编排、`rviz`/`rqt` 看、`rosbag` 录 |
| 软件生态 | 几万个现成的包，C++ 接口居多 | OpenCV、PCL、Eigen、Navigation、MoveIt、相机雷达 IMU 驱动 |

生态这条最值钱：别人写的驱动包，你装上来就能用，不用从寄存器开始啃。

### 1.4 在车上：一条从相机到轮子的流水线
车上的数据是这么流的：

```
相机驱动 → /image_raw → 识别节点 → 解算节点 → /cmd_vel → 底盘控制 → EtherCAT → CAN → 电机
```

中间穿插着：驱动与硬件抽象（相机、激光、IMU、CAN 都有现成包）、算法库（OpenCV/PCL/Eigen/tf2）、调试可视化（rviz 看点云和 TF，rosbag 录现场回放，plotjuggler 看曲线）、仿真（Gazebo 里先把逻辑跑通再上实车）。

**方案：纯上位机。** 视觉和控制逻辑全跑在上位机（一台 miniPC）的 ROS 节点里，EtherCAT 只做「按时把这帧送到」，下位机里没有控制逻辑，只做帧转换和急停兜底。

好处：改闭环、改状态机、改参数，全在上位机 C++ 里，重新 `catkin make` 就行，不用烧固件、不用跟另一块板子对协议。代价是实时性弱一些。

另一种做法是把闭环放到小板上：实时性强，但逻辑分两处，改一个 PID 要烧固件，出问题两端一起查。

### 1.5 为什么用 ROS

1. 为机器人量身定做：发布订阅、坐标变换、参数、录包，一开始就是一等公民。
2. 生态大：报错先搜，通常有人踩过；算法包拿来改。
3. 可视化、好分工：`rqt_graph` 一眼看清谁连谁，每人只碰自己那个节点，接口谈好就能并行。
4. 核心节点统一 C++：性能可控，而且 OpenCV/PCL/Eigen 本来就是 C++ 接口，不用套壳。
5. 可回放：赛场出的问题，回实验室一帧一帧重放，不是「当时就那样，复现不了」。
6. 可仿真：新队员不碰真车也能练。

## 第 2 章 ROS架构

### 2.1 三层架构

| 层 | 解决什么问题 | 里面有什么 | 报错 |
|---|---|---|---|
| 文件系统层 | 代码怎么组织 | 工作空间 → 功能包 → 源码/配置；`package.xml`、`CMakeLists.txt`、`msg`/`srv`、`launch`、`config` | 找不到包、编译不过 |
| 计算图层 | 程序怎么协作 | 节点、话题、服务、参数、消息；谁发布、谁订阅、谁提供什么服务 | 连不上、收不到、名字或类型不对 |
| 通信层 | 数据怎么走 | TCPROS / UDPROS，建连接、序列化、队列、断线重连 | 延迟高、丢数据 |

**排错口诀**（PPT 原话）：

- 找不到包？→ 文件系统层：`source` 了吗？包真在 `src` 里吗？
- 连不上 / 收不到？→ 计算图层：名字对不对？类型对不对？
- 延迟高 / 丢数据？→ 通信层：队列太浅？带宽不够？
- 节点一启动就退？→ 先看终端里第一行红色 `FATAL`，它会告诉你缺什么。

### 2.2 程序的五个基本零件

| 零件 | 一句话 | 类比 | 查它的命令 |
|---|---|---|---|
| 节点 Node | 一个独立跑着的进程，干一件小事 | 一个工人 | `rosnode list` / `rosnode info` |
| 话题 Topic | 一条有名字的广播频道，发布者写、订阅者读 | 广播电台 | `rostopic list` / `rostopic echo` |
| 消息 Message | 话题上传输的数据结构，由 `.msg` 定义字段 | 信封的格式 | `rosmsg show` |
| 服务 Service | 一问一答的远程调用，有请求有应答 | 打一次电话 | `rosservice call` |
| 参数 Parameter | 全局共享的配置表，key-value | 贴在墙上的配置表 | `rosparam get` / `set` |

这五个零件的关系：**节点**通过**话题**或**服务**说话，说的内容格式是**消息**，全局的配置叫**参数**。

### 2.3 ROS Master：数据根本不经过

Master 是个「介绍所」，跑在 11311 端口。它干四件事：名字注册（谁发布了什么话题、谁提供什么服务）、名字查询（把双方介绍给对方）、参数服务器（全局参数住这儿）、状态监控（谁还活着）。

`roscore` 这一条命令，起的是三样东西：**Master + rosout（日志）+ 参数服务器**。

注册完、握手完，数据就在两个节点之间直连，**Master 不再参与**。

所以：**Master 挂了，已经连上的节点照样跑，但新来的连不上。** 中途 `roscore` 意外退出，你不会立刻发现程序停，反而会在启动新节点时才报错。

### 2.4 一次话题通信的六步

```
1. roscore 起 Master，在 11311 等别人来注册
2. 发布者报到：我发布 /cmd_vel，类型 Twist
3. 订阅者报到：我要订阅 /cmd_vel
4. Master 回地址：把发布者的 IP 和端口告诉订阅者（只此一次）
5. 订阅者主动去连发布者，握手时确认消息类型一致
6. 之后每条消息都走这条 TCP 直连，Master 不再出现
```

> 注意：**Master 返回的是发布者的地址，不是数据本身。** 所以第 5 步是订阅者去连发布者。

第 5 步的「握手确认类型一致」解释了另一个常见的错误：两个节点话题名一模一样，就是连不上，因为一个用 `std_msgs/String`，一个用 `geometry_msgs/Twist`。名字对了，类型不对，照样不通。

**验证**：
```
rosnode info /turtlesim
```
再开一个终端敲：
```
rqt_graph
```

### 2.5 ros_control

要控制一个电机，代码里大概的样子：读编码器 → 算 PID → 下发指令 → 处理限位。四个电机关节写四遍，换成另一种电机又要改一遍，仿真和实车还是两套代码。**ros_control 把这四步拆到固定的位置，让「换硬件」和「换算法」互不影响。**

它不发明新的通信方式，只是把话题和服务用在控制上。三层：

| 层 | 是谁 | 干什么 |
|---|---|---|
| Controller Manager | 管理器 | 加载、启动、切换控制器，管它们的生命周期；`spawner` 一行命令起一个 |
| Controller | 现成的算法 | `joint_state`（发布关节状态）、`joint_trajectory`（走轨迹）、`effort`（PID 力矩）、`diff_drive`（差速底盘），直接用，不用自己写 |
| RobotHW | **唯一自己写** | 写一个硬件类，`write()` 把指令打包发给电机，`read()` 把编码器读数收上来，对接 EtherCAT 或 CAN |
| joint_limits + 传动 | 约束 | 限位、最大速度、加速度；传动比和「关节 ↔ 电机」的映射；云台和底盘的限位都在这 |

控制器这一层的输入，最常见的还是那个 `/cmd_vel` 话题（`geometry_msgs/Twist`）。

好处：**换电机只改 hardware_interface，换算法只换 controller，仿真和实车共用同一套 controller。**
查看控制器：
```
rostopic info /cmd_vel
rostopic info /joint_states
```
查看这两个话题的发布者与订阅者

## 第 3 章 建立 ROS 包

### 3.1 工作空间：一间厨房的四个房间

```
catkin_ws/
├── src/          # 菜谱：你的代码，进 git
├── build/        # 切菜台：编译中间文件，可删，不进git
├── devel/        # 出菜窗口：编译产物 + setup.bash
└── logs/         # 记账本：编译日志
```

后三个都是 `catkin_make` 自动生成的，**不要手改、不要进版本库**。别人 clone 我的仓库只拿到 `src`，他自己编译一次就有了。

三个重要的点：

1. 每开一个终端，`source devel/setup.bash` 一次（它把「你的包在哪」登记给 `rosrun`）
2. 一个工作空间可以放很多包；**包名必须全局唯一**，重名了 `rosrun` 会认错
3. 上车部署就三句：`git clone` 到上位机 → `catkin_make` → `source devel/setup.bash`。车上没图形界面，编译和运行都在 SSH 里做

实际路径：容器窗口里 `/root/ros1_ws`（宿主机上同一个目录叫 `~/ros1_ws`）

### 3.2 一个包里到底有什么

package（功能包）是 ROS 里**最小的复用单位**。目录：

| 东西 | 放什么 | 注意 |
|---|---|---|
| `CMakeLists.txt` | 编译规则：编什么、链什么 | 最关键的两个文件之一 |
| `package.xml` | 名片：名字、版本、依赖谁 | 最关键的两个文件之二 |
| `include/包名/` | 对外暴露的头文件 `.h` / `.hpp` | 「给别人用的」 |
| `src/` | C++ 源文件 `.cpp` | 「给自己编的」 |
| `scripts/` | 可执行脚本（Python 小工具） | --- |
| `msg/` | 自定义消息 `.msg` | 光建目录没用，CMakeLists 里要声明 |
| `srv/` | 自定义服务 `.srv` | **叫 srv，不叫 service**，建成 `service/` 编译时找不到 |
| `launch/` | 启动文件 `.launch`，一次拉起多个节点 | 不进编译，但别人接手你的包最先看它 |
| `config/` | 参数文件 `.yaml` | 同上 |
| `README.md` | 给人看的：怎么编、怎么跑 | --- |

`include` 和 `src` 的区别有个好记的说法：`include` 是「我提供给别人的接口」，`src` 是「我自己内部的实现」。只写 `src` 也能跑

### 3.3 package.xml：决定谁能找到你

```xml
<?xml version="1.0"?>
<package format="2">
  <name>my_pkg</name>              <!-- 必须与文件夹同名 -->
  <version>0.1.0</version>
  <description>我的第一个 ROS 包</description>
  <maintainer email="you@example.com">Your Name</maintainer>
  <license>MIT</license>

  <buildtool_depend>catkin</buildtool_depend>
  <build_depend>std_msgs</build_depend>        <!-- 编译期需要 -->
  <exec_depend>message_runtime</exec_depend>   <!-- 运行期需要 -->
  <depend>roscpp</depend>                      <!-- 三合一 -->
</package>
```

三种依赖的区别：

| 标签 | 什么时候需要 | 例子 |
|---|---|---|
| `build_depend` | 编译这个包时 | 头文件、库、消息包 |
| `exec_depend` | 运行时 | 动态库、消息运行库、launch 里调用的包 |
| `depend` | 三合一（build + build_export + exec） | 一般写这个最省事 |

自定义 msg/srv 还要额外加两个：`message_generation`（编译期，负责生成头文件）和 `message_runtime`（运行期，负责读写）。

> 注意：`<name>` 必须和文件夹同名，改了文件夹名不会自动改这里。包名别用中文、别带空格，还要注意大小写。新建包先编译一次再写代码。

### 3.4 CMakeLists.txt：编什么、链什么

```cmake
# 1.声明
cmake_minimum_required(VERSION 3.0.2)
project(my_pkg)

# 2.找依赖（包名要和 package.xml 对得上）
find_package(catkin REQUIRED COMPONENTS
  roscpp std_msgs message_generation)

# 3.自定义消息 / 服务（用到才写，三个函数要配套）
add_message_files(FILES MyMsg.msg)
add_service_files(FILES MySrv.srv)
generate_messages(DEPENDENCIES std_msgs)
catkin_package(CATKIN_DEPENDS roscpp std_msgs message_runtime)

# 4.可执行目标
include_directories(${catkin_INCLUDE_DIRS})
add_executable(talker src/talker.cpp)
target_link_libraries(talker ${catkin_LIBRARIES})
add_dependencies(talker ${${PROJECT_NAME}_EXPORTED_TARGETS})
```

四个坑：

1. **每加一个 `.cpp` 就加一条 `add_executable`**，否则不会被编译（找不到节点）
2. 用了自定义消息，忘了 `add_dependencies(...${PROJECT_NAME}_EXPORTED_TARGETS)`，报「找不到 MyMsg.h」
3. 改了 `.msg` 不重新 `catkin_make`，用的是旧消息
4. 第三段的三个函数（`add_message_files` / `generate_messages` / `catkin_package` 里的 `message_runtime`）是配套的，少一个就出怪错

### 3.5 从零跑通一个包（容器里，按顺序敲）
（ai给出的实践流程，其实就是我在第二课笔记里提到的练习）
**第一步**，建包（下面这些命令都在 VS Code 的集成终端里敲，那就是容器里的终端）：

```
source /opt/ros/noetic/setup.bash && cd /root/ros1_ws/src && catkin_create_pkg demo_topic roscpp std_msgs
```

会看到新建的目录里有 `CMakeLists.txt`、`package.xml`、`src`、`include`。

前面那句 `source` 通常可以省：集成终端一打开就执行了 `~/.bashrc`，里面已经 source 过 ROS。拿不准就先敲 `which catkin_create_pkg`，有输出说明能直接用。

**第二步**，顺手把属主改回自己（VS Code 是在容器里以 root 身份建的包）：

```
docker exec ros1 chown -R 1000:1000 /root/ros1_ws
```

这条在**宿主机终端**敲。如果就在容器终端里，直接 `chown -R 1000:1000 /root/ros1_ws`。现在不做也行，等哪天宿主上报 `EACCES` 再回来做。

**第三步**，在 VS Code 左侧文件树里进 `src/demo_topic/src/`，新建 `talker.cpp` 和 `listener.cpp`（代码在第 4 章），写完 `Ctrl+S`。

**第四步**，打开 `src/demo_topic/CMakeLists.txt`，在**末尾**追加四行（CMake 顺序执行，追加合法，比在模板里找注释行改省事）：

```cmake
add_executable(talker src/talker.cpp)
target_link_libraries(talker ${catkin_LIBRARIES})
add_executable(listener src/listener.cpp)
target_link_libraries(listener ${catkin_LIBRARIES})
```

**第五步**，集成终端里编译：

```
cd /root/ros1_ws && catkin_make
```

（用 `docker exec` 从宿主敲也行：`docker exec -it ros1 bash -c "source /opt/ros/noetic/setup.bash && cd /root/ros1_ws && catkin_make"`。）

> 注意：`catkin_make` 必须**在工作空间根目录**跑（`/root/ros1_ws`），在包目录里跑会报 `does not appear to contain CMakeLists.txt`。

编成功会看到 `[100%] Built target talker` 和 `Built target listener`。空包没有源文件时一条 `Built target` 都不打印

**第六步**，验证产物真的在：

```
ls /root/ros1_ws/devel/lib/demo_topic/
```

看到 `talker` 和 `listener` 两个文件，就是「编译成功」的物证，比读一堆 make 输出可靠。

**第七步**，跑起来。VS Code 集成终端右上角的 `+` 可以开多个终端，它们都在同一个容器里：一个敲 `roscore`，另两个分别敲 `rosrun demo_topic talker` 和 `rosrun demo_topic listener`。（在宿主终端里开三个 `docker exec` 效果一样，只是要各自 source 一次。）

### 3.6 报错对照表

| 报错 | 真正的病因 | 解法 |
|---|---|---|
| `catkin_make: command not found` | 没 source ROS 环境 | `source /opt/ros/noetic/setup.bash` |
| `[rospack] Error: package 'x' not found` | 没 source 工作区 devel | `source /root/ros1_ws/devel/setup.bash` |
| `does not appear to contain CMakeLists.txt` | 在包目录里跑了 catkin_make | 回工作区根目录 |
| `Cannot locate node` | CMakeLists 缺 `add_executable` 或名字拼错 | 检查追加的四行 |
| `No such file: src/x.cpp` | 文件建在包根而不是 `src/` | 挪进 `pkg/src/` |
| `EACCES: permission denied` | 容器 root 建的文件，宿主用户没权限 | `docker exec ros1 chown -R 1000:1000 /root/ros1_ws` |

还有一条不算报错：容器窗口里 `#include <ros/ros.h>` 画红线、提示「无法打开源文件」。先不用管，真正的判据是 `catkin_make` 过没过，过了就是没问题。

这一步正好验证了其本质就是cmake

## 第 4 章 三大通信：节点之间的交流方式

### 4.1 概况

| | 话题 Topic | 服务 Service | 参数 Parameter |
|---|---|---|---|
| 类比 | 广播电台 | 打一次电话 | 贴在墙上的配置表 |
| 模式 | 发布 / 订阅 | 请求 / 应答 | 共享配置 key-value |
| 同步性 | 异步，发完就返回 | 同步阻塞，等结果 | 不流动，谁读谁取 |
| 节点关系 | 多对多，谁都不认识谁 | 一对一（一个服务一个实现） | 全局，谁都能读写 |
| 数据载体 | `.msg` | `.srv`（`---` 分请求/应答） | int / double / bool / string / list / dict |
| 有队列吗 | 有 `queue_size`，满了丢旧数据 | 无，每次调用都建连 | 无 |
| 典型场景 | 传感器数据、里程计、检测结果、控制指令 | 切换模式、取一次地图、清空轨迹、生成一只乌龟 | 阈值、相机内参、PID 参数、串口波特率 |
| 什么时候别用 | 需要「确认对方收到并处理」 | 高频（10 Hz 以上）、要实时 | 当数据库用（不持久，roscore 关了就没了） |

**选择**：连续的数据流 → 话题；要一次结果 → 服务；配置项 → 参数；跑得久、要进度、能取消 → action。

### 4.2 话题：发布 / 订阅

四个角色：**发布者**（往话题写数据）、**订阅者**（从话题读数据）、**消息**（数据的结构，由 `.msg` 定义）、**话题名**（两边对的暗号）。

对上的条件：**同名 + 同类型**。（一个都不能错）

为什么它适合数据流：异步（发完就返回）、多对多（一个话题可以有很多发布者和订阅者）、有队列（`queue_size` 是缓冲深度，满了丢旧数据）、断线自动重连（订阅者晚启动也能接上，除非用了 latching）。

**API：三行代码就能发**：

```cpp
advertise()   // 创建发布者：话题名、类型、队列长度
publish()     // 发一条，立刻返回
subscribe()   // 创建订阅者：话题名、队列长度、回调函数
回调线程　消息到了就在回调里处理，别做耗时操作
```
以之前的练习为例：
`talker.cpp`（发布端）：

```cpp
// talker.cpp —— 发布端
#include <ros/ros.h>
#include <std_msgs/String.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "talker");          // 必须最先，注册节点名
    ros::NodeHandle nh;                        // 句柄：所有通信从它发起
    ros::Publisher pub = nh.advertise<std_msgs::String>("/chatter", 10);  // 10 = 队列长度
    ros::Rate rate(1);                         // 1 Hz

    while (ros::ok()) {
        std_msgs::String msg;
        msg.data = "hello";
        pub.publish(msg);
        rate.sleep();
    }
}
```

`listener.cpp`（订阅端）：

```cpp
// listener.cpp —— 订阅端
#include <ros/ros.h>
#include <std_msgs/String.h>

void cb(const std_msgs::String::ConstPtr& msg) {      // 回调：消息到了就走这里
    ROS_INFO("I heard: %s", msg->data.c_str());
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "listener");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("/chatter", 10, cb);
    ros::spin();                               // 卡住，一直等回调
}
```

三个坑：

1. `ros::init` 必须最先。init 之后才能造 `NodeHandle`；漏了它，节点名和参数都认不出来。
2. `advertise` / `subscribe` 第二个参数是**队列长度**。发送比接收快，队列满了就丢旧数据；这是「我发了但对方漏了」的头号原因。
3. **回调里别做耗时操作**。回调跑在 `spin` 的线程里，卡住就把后面的消息全堵住了。重活（跑个神经网络、存个图）丢给别的节点。

以及小乌龟里的例子：`/turtle1/cmd_vel（geometry_msgs/Twist）`—— 键盘节点发布，乌龟节点订阅

不写代码也能看（排错的主力）：

```
rostopic list        # 现在有哪些话题是活的
rostopic echo /chatter   # 把内容打到终端
rostopic info /chatter   # 谁在发布、谁在订阅、类型是什么
rostopic hz /chatter     # 测频率，看是不是真的 1 Hz
rostopic pub -r 10 /chatter std_msgs/String "data: hello"   # 手动发
```

### 4.3 服务：请求 / 应答

两个角色 + 一份契约：
**服务端**（提供能力，注册一个回调处理请求）、
**客户端**（主动调用，等结果）、
**Request/Response**（两段数据结构，写在 `.srv` 里，中间用 `---` 隔开，上面请求、下面应答）。

特点,也是它不能乱用的原因：**同步阻塞**（调用期间客户端线程就停在那儿，直到拿到结果或超时）、**一对一**、服务端没起来就报错（要 `wait_for_service` 等它）。高频场景别用：每次调用都有建连和序列化的开销。10 Hz 以上就该考虑话题。

API：调用与提供服务
serviceClient<T>()　创建客户端，指定服务名和 srv 类型
ServiceServer　注册服务名、类型、处理函数
waitForExistence()　等对方上线，避免直接报错
cli.call(srv)　阻塞，直到返回 response

命令：命令行也能调
rosservice list　列出当前所有服务
rosservice type　看这个服务的类型
rosservice call　手工填参数调一次，最快的验证方式
rossrv show　看 .srv 的字段定义

`.srv` 长这样（以PPT上小乌龟的 `/spawn`为例）：

```
# srv/Spawn.srv —— 上段请求，下段应答
float32 x
float32 y
float32 theta
string name
---
string name      # 返回：新乌龟的名字
```

定义好之后 CMakeLists 里加两行（跟第 3 章第 3 段（自定义消息）一样）：

```cmake
add_service_files(FILES Spawn.srv)
generate_messages(DEPENDENCIES std_msgs)
```

C++ 客户端：

```cpp
ros::ServiceClient cli = nh.serviceClient<turtlesim::Spawn>("/spawn");
turtlesim::Spawn srv;
srv.request.x = 2.0;
srv.request.name = "t2";
if (cli.call(srv))                            // 阻塞，直到返回
    ROS_INFO("new: %s", srv.response.name.c_str());
```

命令行调用（最快的验证方式，先验证服务活着再写代码）：

```
rosservice list
rosservice type /spawn
rosservice call /spawn "x: 2.0 y: 3.0 theta: 0.0 name: 't2'"
rossrv show　看 .srv 的字段定义
```

坑：

1.**服务名写错会一直等**，现象是「程序卡住没反应」。先用 `rosservice list` 核对名字，别靠猜。
2.超时要自己加：`call()` 默认一直等。比赛里必须自己包一层超时，否则一次卡死全队。
3.高频调用会拖垮节点: 每次调用都有建连和序列化开销；10 Hz 以上考虑用话题
4. 返回值的坑：`srv.response.name`，不是 `srv.name`。

记住：话题是「持续广播」，服务是「一次问答」。频率高、要求实时 → 话题；偶尔一次、要结果 → 服务。

### 4.4 参数：全局的配置墙

**1. 组成**

它**不在节点之间传数据**，只是让大家读同一份配置。

| 组成 | 是什么 |
|---|---|
| 全局 key-value | 存在 Master 里，所有节点都能访问 |
| 树形命名空间 | `/robot/camera/exposure` 这样一层层往下 |
| 六种类型 | int、double、bool、string、list、dict |
| 生命周期 | 随 roscore 存在，roscore 一关就没了 —— 所以别把它当数据库 |

**2. 格式**

| 操作 | PPT 上的写法（rospy） | 我们写的 C++（roscpp） | 什么时候用 |
|---|---|---|---|
| 读，给默认值最稳 | `get_param(name, default)` | `nh.param<T>(name, var, default)` | 默认选它；参数文件漏写一项不该让节点崩 |
| 写 | `set_param(name, value)` | `nh.setParam(name, value)` | 临时调参，或把状态回写给别人看 |
| 判断 / 删除 | `has_param` / `delete_param` | `nh.hasParam` / `nh.deleteParam` | 启动时检查，清掉上一轮留下的键 |
| 带缓存，高频读用它 | `get_param_cached` | `nh.getParamCached` | 控制循环里反复读同一个键，不想每次都走网络 |

中间那列是 PPT 上的原话，用法是：**背左列（干什么）+ 右列（我们敲什么）**，中间列当"看别人的 Python 代码"时的翻译表。

PPT 那页还漏了一行，可在 C++ 里它是真正常用的读法：

| 操作 | PPT 上的写法（rospy） | 我们写的 C++（roscpp） | 什么时候用 |
|---|---|---|---|
| 读，不带默认值 | `get_param(name)` | `nh.getParam(name, var)` | 参数必须存在；缺了 Python 抛 KeyError、C++ 返回 false 不崩 |

名字之外只有一处是**本质**差别：「给默认值」在 C++ 里是另一个函数（`param<T>`），不是同一个函数加个参数；类型还得在尖括号里写死。

**3. 三种写法，作用域完全不同**

| 写法 | 例子 | 含义 | 什么时候用 |
|---|---|---|---|
| 绝对名 | `/robot/speed` | 从根开始，换命名空间也指同一个地方 | 全局唯一的配置（机器人型号、串口设备号） |
| 私有名 | `~speed` | 等价于 `/节点名/speed`，只属于这个节点 | 节点自己的调参（PID、阈值），同名节点互不干扰 |
| 相对名 | `speed` | 拼在当前命名空间后面，随 launch 的 `ns` 变 | 想让 launch 自动隔离的（多台同款车） |

**4. 实例：C++（roscpp）**

五种操作 → 五行：

```cpp
nh.param<int>("camera/exposure", exposure, 30);   // 读，带默认值（首选）
nh.getParam("speed", v);                          // 读，不带默认值：读不到返回 false，不崩
nh.setParam("speed", 1.5);                        // 写
nh.hasParam("speed");  nh.deleteParam("speed");   // 判断 / 删除
nh.getParamCached("speed", v);                    // 带缓存，高频读用它
```

私有参数要配私有句柄，参数名里**就不要再带 `~`**：

```cpp
ros::NodeHandle pnh("~");                 // 私有句柄
double gain = 1.0;
pnh.param<double>("gain", gain, 1.0);     // 实际读的是 /节点名/gain
```

launch 里写在 `<node>` 内部的 `<param>` 就是私有参数：

```xml
<node name="my_node" pkg="my_first_pkg" type="talker">
  <param name="gain" value="2.5"/>
</node>
```

读整棵子树：

```cpp
XmlRpc::XmlRpcValue tree;
nh.getParam("camera", tree);              // 把 camera 下面所有内容一次取成一棵树
int w = (int)tree["width"];               // 取出来是"万能盒子"，要自己 cast
```

**Python（rospy）**：见第 2 点那张表左边的两列。唯一的写法差异是私有参数 —— Python 没有句柄这一层，直接把 `~` 写进名字里（`get_param("~gain", 1.0)`）。

**5. 类型提醒**

`30` 是 int，`"30"` 是 string，`3.0` 是 double，`true` 是 bool 

- C++：`param<int>` 遇上类型不符 → 返回 `false`，**静默用默认值**，就像"参数没生效"。
- Python：拿到的是服务器上的真类型，要去算才报错（`'30' + 1` → TypeError）。

排查：启动时打印实际值

```cpp
ROS_INFO("exposure = %d", exposure);   // 别信"我写了就应该生效"
```

**6. 命令四件套 + 两个坑**

```
rosparam list | grep cam      # 看全部，再过滤
rosparam get /cam/exposure    # 读一个
rosparam set /cam/exposure 30 # 改一个
rosparam load / dump          # 从 YAML 灌入 / 导出成 YAML
rosparam delete /cam          # 删掉一棵子树
```

- 一：`dump` 出来的文件带命名空间前缀，`load` 时再加一次会叠成 `/cam/cam/exposure`。
- 二：`rosparam set` 之后窗口/程序不动，是正常的 —— 值改了，但节点没被通知（见第 8 点）。


### 4.5 小乌龟的三种通信

| 类型 | 名字与类型 | 谁和谁 |
|---|---|---|
| 话题 | `/turtle1/cmd_vel`（`geometry_msgs/Twist`） | 键盘节点发布 → 乌龟订阅 |
| 话题 | `/turtle1/pose`（`turtlesim/Pose`） | 乌龟发布 → 你订阅 |
| 服务 | `/spawn`、`/kill`、`/clear`、`/turtle1/set_pen` | 乌龟提供 |
| 参数 | `/turtlesim/background_r` 等 | 背景色、速度上限 |

**1. 话题**：不用键盘，让乌龟自己转圈：

```
rostopic pub -r 10 /turtle1/cmd_vel geometry_msgs/Twist '{linear: {x: 0.5}, angular: {z: 0.5}}'
```

`linear.x` 是前进速度，`angular.z` 是转向，两个都给就是画圆。`-r 10` 是每秒发 10 次。**为什么必须一直发**？因为乌龟收到一条只走一小段就停，它是「听到指令才动」，没有指令就停。

**2. 服务**：再放一只乌龟出来：

```
rosservice call /spawn "x: 2.0 y: 3.0 theta: 0.0 name: 't2'"
```

**3. 参数**：改背景色：

```
rosparam set /turtlesim/background_r 255
rosservice call /clear
```

注意第二步：改完参数要 `clear` 一次才生效，因为背景是画上去的，得重画。

```
rosnode info /turtlesim
```
`Subscriptions` 里应该有 `/turtle1/cmd_vel`，`Publications` 里有 `/turtle1/pose`。

## 第 5 章 YAML：参数的写法

参数一多，`rosparam set` 一条条敲就不现实了，得写进文件，一次灌进去。这份文件的语言叫 YAML。

YAML 的设计目标是「写给人看」。代价是：**它用缩进表达层级，所以对齐就是语法本身**，跟 C++ 里分号一个地位。

### 5.1 五条硬规矩

报错 `Error parsing YAML` 的时候，九成是这五条里的一条。

1. **只用空格缩进，绝不用 Tab**。两个空格算一级，同一层必须严格对齐。Tab 肉眼和空格几乎没差别，YAML 直接报错。
2. **冒号后面必须有空格**。`key: value` 对，`key:value` 错。后者会被当成一个字符串（整行都是 key）。
3. **大小写敏感**。`True` 和 `true` 不是一回事；ROS 里按 bool 解析的是 `true` / `false`，别写 `yes` / `no`。
4. **`#` 是注释**。可以整行，也可以在行尾；值里真要出现 `#`，加引号。
5. **`---` 可以分隔多个文档**。空文件也是合法的，等于一个空字典。

对与错：

```yaml
# 对的
camera:
  width: 640
  height: 480
  name: "front"    # 注释
  debug: true
```

```yaml
# 错的
camera:
  width:640        # 冒号后没空格
  name: front cam  # 有空格没引号
	height: 480    # Tab 缩进
```

三个检查：

1. 编辑器里把 Tab 显示出来（VS Code 右下角）
2. 看行号旁的缩进参考线有没有对齐
3. **用 `rosparam load` 试灌一遍**：

```
rosparam load p.yaml /t && rosparam get /t
```

先灌进去、类型也对，语法就过。不过再修改

### 5.2 只有三种结构

所有复杂的东西，都是这三样嵌套出来的。

```yaml
# 标量
speed: 1.5          # 数字
name: "sentry"      # 字符串
debug: true         # 布尔
empty: null         # 空值

# 列表
ports:
  - 8080
  - 8081
ids: [1, 2, 3]      # 行内写法

# 字典
camera:
  width: 640
  height: 480
point: {x: 1, y: 2} # 行内写法
```

一个真实的 ROS 参数文件：

```yaml
turtlesim:
  background_r: 69
  background_g: 86
  background_b: 255
  holonomic: false

my_node:
  thresholds: [0.3, 0.6, 0.9]
  camera:
    exposure: 30
    gain: 1.2
  targets:
    - {name: "hero", id: 1}
    - {name: "sentry", id: 7}
```

读法：**缩进决定层级，冒号左边是名字、右边是值，`-` 表示列表的一项。**

### 5.3 在 ROS 里怎么用 YAML：三种加载方式

文件写对了，还得「加载对地方」。

```bash
# 1. 命令行：临时调试最方便
rosparam load config/params.yaml /my_ns
```

```xml
<!-- 2. launch 文件里（最常用） -->
<rosparam file="$(find my_pkg)/config/params.yaml" command="load" ns="my_ns"/>
```

```cpp
// 3. 代码里读，带默认值最稳
ros::NodeHandle pnh("~");
int exposure;
pnh.param<int>("camera/exposure", exposure, 30);
```

## 第 6 章 跑起来：小乌龟与常用命令
（部分指令放在第四章了）
### 6.1 三行命令，一个完整的 ROS 系统

三个终端，都在 VS Code 里开（集成终端右上角 `+`，每个都开在容器内），不用各自 `docker exec`。要是用宿主终端，就每条前面套一层 `docker exec -it ros1 bash` 再 `source`：

```
终端 1：roscore                              # 启动 Master 与参数服务器
终端 2：rosrun turtlesim turtlesim_node      # 打开乌龟窗口
终端 3：rosrun turtlesim turtle_teleop_key   # 键盘遥控
```
### 6.2 rosnode：排查问题

```
rosnode list                # 谁在跑
rosnode info /turtlesim     # 它在和谁打交道：Publications / Subscriptions / Services
rosnode ping /turtlesim     # 还活着吗，看延迟
rosnode kill /turtlesim     # 干掉它（先注销，比 kill -9 干净）
```

排查顺序：

1. **先 `list`，再 `info`**。「我以为它订阅了」是最常见的误判，`info` 会把真相打出来。
2. **`ping` 看存活**。节点假死时 `list` 里还在，`ping` 才看得出来。
3. 三个工具分工：`rqt_graph` 看全局（谁连谁）、`rosnode info` 看单点（连了什么）、`rostopic echo` 看内容（到底传的什么）。

### 6.3 命令速查表（贴墙用）

| 干什么 | 命令 |
|---|---|
| 包与编译 | `rospack find <pkg>`、`roscd <pkg>`、`rosls <pkg>`、`catkin_create_pkg <name> <deps>`、`catkin_make` |
| 运行与节点 | `roscore`、`rosrun <pkg> <node> [参数]`、`roslaunch <pkg> <file.launch>`、`rosnode list/info/ping/kill` |
| 话题 | `rostopic list`、`rostopic echo <topic>`、`rostopic info <topic>`、`rostopic hz <topic>`、`rostopic pub -r 10 <topic> <type> <data>` |
| 服务与消息 | `rosservice list/type/call/find`、`rosmsg show <pkg/Type>`、`rossrv show <pkg/Type>` |
| 参数 | `rosparam list/get/set/load/dump/delete` |
| 调试与录制 | `rqt_graph`、`rviz`、`rosbag record -a`、`rosbag info/play <bag>`、`plotjuggler` |

