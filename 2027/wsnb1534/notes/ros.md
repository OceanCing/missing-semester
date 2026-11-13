# ROS1 通信学习笔记

ROS 程序由一个个**节点（node）**组成，节点之间通过话题、服务、动作、参数这几种机制交换数据，所有节点和它们之间的连线构成**计算图**。

ROS1 是中心化的：运行前先启动 `roscore`，它包含 **master**（名字服务，负责节点注册和查找）、**参数服务器** 和 **rosout**（日志收集）。节点启动后先向 master 注册自己发布/订阅了什么，master 告诉它对方在哪里，然后两个节点直接建立 TCP 连接传数据（也可以用 UDP）。

## 1. 节点（Node）

节点是最小的执行单元，通常一个节点只做一件事（读传感器、做规划、控制电机）。一个可执行文件里可以起多个节点。

用 `rosnode list` 查看当前节点，`rosnode info` 查看某个节点发布/订阅了哪些话题、提供了哪些服务，`rosnode ping` 测试连通性，`rosrun rqt_graph rqt_graph` 可以图形化看到节点和通信连线。

## 2. 话题（Topic）：发布/订阅

最常用的通信方式。发布者（publisher）往话题上发消息，订阅者（subscriber）从话题收消息，双方**不需要知道对方存在**，只靠话题名和消息类型对上。

- 异步、多对多：可以有多个发布者、多个订阅者
- 消息类型由 `.msg` 文件定义，例如 `std_msgs/String`、`geometry_msgs/Twist`
- 适合传感器数据、里程计、控制指令这类**连续的数据流**

常用命令有 `rostopic list`（列出话题）、`rostopic echo /chatter`（打印内容）、`rostopic info /chatter`（类型和收发双方）、`rostopic hz /chatter`（发布频率）、`rostopic bw /chatter`（带宽）、`rostopic type /chatter`（消息类型），以及 `rostopic pub` 手动往话题里发消息。

要点：

- 发布/订阅是异步的，发布者不知道有没有人收
- `latch=True` 可以“锁存”最后一条消息，后加入的订阅者也能收到，适合地图、静态配置这类只发一次的数据
- 回调函数在单独的线程里执行，不要在回调里做长时间阻塞的操作

## 3. 服务（Service）：请求/响应

客户端发一个请求，服务端处理完返回一个响应，是**一问一答**的模式。

- 同步：客户端调用后会阻塞，直到服务端返回或超时
- 一个服务可以有多个客户端，但同一时刻每个请求由一个服务端处理
- 适合查询状态、做一次性的计算、触发某个动作
- 接口由 `.srv` 定义：上半部分是请求字段，中间用 `---` 分隔，下半部分是响应字段。比如两个 `int64` 输入、一个 `int64` 输出

常用命令有 `rosservice list`（列出服务）、`rosservice type`、`rosservice info`、`rosservice find`（查哪些节点提供某个服务）、`rosservice call`（手动调用，参数按顺序或按字段名传）、`rossrv show`（查看接口定义）。

几个要点：

- 服务端用 `rospy.Service` 注册回调，回调返回响应对象
- 客户端调用前必须 `rospy.wait_for_service()`，否则会抛 `ServiceException`
- 多次调用默认每次重新建立连接，高频调用可以用持久连接（persistent）省掉建连开销
- 服务不能用于耗时任务：服务端处理期间客户端一直阻塞，也没法中途取消，所以有了动作

## 4. 动作（Action）：带反馈的长任务

动作基于 **actionlib**，本质是“话题 + 服务”的组合，专门解决耗时任务：客户端发目标（goal），服务端持续反馈进度（feedback），最后返回结果（result），期间客户端可以**取消**。

- 底层用了 5 个话题（goal、cancel、status、feedback、result）和 2 个服务（send_goal、cancel）
- 典型场景：导航到某个点、机械臂运动、抓取
- `.action` 文件分三段，用两个 `---` 隔开：目标、结果、反馈

代码里一般用封装好的 `SimpleActionClient` / `SimpleActionServer`，比直接操作话题和服务简单得多。客户端要先 `wait_for_server()` 再发目标，发目标时可以注册反馈回调和完成回调；服务端在 `execute_cb` 里持续 `publish_feedback`，结束时 `set_succeeded`，收到取消请求时 `is_preempt_requested` 为真。

命令行可以用 `rosrun actionlib_tools axclient.py` 图形化发目标、看反馈，也可以用 `rostopic echo` 观察 feedback 话题。

## 5. 参数（Parameter）

参数是挂在**参数服务器**上的键值配置（由 master 管理），可以是整数、浮点、字符串、布尔、列表、字典。

- 常用来配置频率、阈值、话题名，也可以运行时动态修改
- 参数不是话题：别的节点不能“订阅”参数变化，只能主动查询
- 参数是全局共享的，但习惯上用私有命名空间 `~` 让每个节点有自己的配置

常用命令有 `rosparam list`、`rosparam get`、`rosparam set`、`rosparam delete`，以及 `rosparam dump` / `rosparam load` 做导出导入。

代码里用 `rospy.get_param('~rate', 10.0)` 读取（`~` 表示该节点的私有命名空间，第二个参数是默认值），`rospy.set_param` 写入。launch 文件里可以直接用 `<param>` 传单个参数，用 `<rosparam>` 加载 YAML 文件，用 `<remap>` 重映射话题名，也可以在 `<node>` 上写 `ns` 指定命名空间。

## 6. 消息与接口定义

| `.msg` | 话题 | 包的 `msg/` 目录 |
| `.srv` | 服务 | 包的 `srv/` 目录 |
| `.action` | 动作 | 包的 `action/` 目录 |

字段格式是 `类型 名称`，支持 `int32`、`float64`、`string`、`bool`、`time`、数组 `int32[]`、定长数组 `int32[5]`，也可以嵌套其他消息类型（如 `geometry_msgs/Pose`）。

自定义消息的构建要点（catkin）：

- `package.xml` 里声明 `message_generation`（build）和 `message_runtime`（run）依赖
- `CMakeLists.txt` 里用 `add_message_files`、`add_service_files` 列出接口文件，用 `generate_messages` 声明生成时依赖的消息包，并在 `catkin_package` 的 `CATKIN_DEPENDS` 里带上 `message_runtime`
- Python 从 `包名.msg` 导入，C++ 包含 `包名/消息名.h`
- 编译后生成的头文件和 Python 模块在 `devel/` 下，改了 msg 要重新 `catkin_make`，依赖它的包也要重编

## 7. 三种通信机制对比

| 机制 | 模式 | 同步/异步 | 关系 | 典型用途 |
|------|------|-----------|------|----------|
| 话题 | 发布/订阅 | 异步 | 多对多 | 传感器流、里程计、控制指令 |
| 服务 | 请求/响应 | 同步 | 多客户端对一个服务端 | 查询、一次性计算、切换模式 |
| 动作 | 目标/反馈/结果 | 异步，可取消 | 客户端/服务端 | 导航、机械臂等耗时任务 |
| 参数 | 键值配置 | 直接读写 | 全局共享 | 启动配置、运行中调参 |

选择思路：**连续的数据流用话题，一问一答用服务，耗时且需要进度/取消用动作，配置项用参数。** 话题最灵活，所以很多功能都会先考虑用话题解耦。

## 8. 命名、命名空间与重映射

理解名字解析是排查通信问题的关键。

- 全局名：`/robot1/chatter`
- 相对名：`chatter`，会拼在当前命名空间后面
- 私有名：`~rate`，指向 `<命名空间>/<节点名>/rate`
- 重映射：launch 里写 `<remap from="chatter" to="chatter2"/>`，运行时也可以 `chatter:=chatter2`
- 命名空间：`<node ... ns="robot1">`，多机器人时避免话题串数据

同一个话题名，消息类型必须一致；类型不同不会报错，但收不到数据。用 `rostopic info` 能看到类型，用 `rqt_graph` 能看清连线。

## 9. 写代码和调试时容易踩的坑

- **忘了 `roscore`**：所有节点都起不来，先检查 master 是否在跑。
- **先建 Publisher 后 `init_node`**：名字解析会出错，甚至抛异常。
- **回调里阻塞**：rospy 每个订阅者有独立线程，但回调卡死会导致该话题消息堆积，队列满了就丢消息。
- **订阅者启动晚**：话题默认不锁存，之前的消息收不到，需要历史数据就用 `latch=True` 或 rosbag。
- **消息队列太小**：高频率话题 `queue_size` 设 1~10，处理不过来时丢数据是正常的，要看 `rostopic hz` 确认。
- **服务调用没等服务器**：必须 `wait_for_service()`，否则抛 `ServiceException`；服务端处理慢会导致客户端一直阻塞。
- **动作没等服务器**：`wait_for_server()` 之后再发目标，否则目标直接丢失。
- **调试顺序**：`rosnode list` → `rostopic list -v` → `rostopic echo/hz` → `rqt_graph`，基本能定位大部分“收不到数据”的问题。
- **rosbag 抓包**：`rosbag record -a`、`rosbag record /chatter`、`rosbag play xxx.bag`、`rosbag info`，排查偶发问题很有用。

## 10. 命令速查

| 用途 | 命令 |
|------|------|
| 启动核心 | `roscore` |
| 运行节点 | `rosrun 包名 节点名`、`roslaunch 包名 xxx.launch` |
| 节点 | `rosnode list/info/ping/kill` |
| 话题 | `rostopic list/echo/info/hz/bw/type/pub` |
| 服务 | `rosservice list/type/info/call/find` |
| 动作 | `rosrun actionlib_tools axclient.py` |
| 参数 | `rosparam list/get/set/delete/dump/load` |
| 消息 | `rosmsg show/list`、`rossrv show/list` |
| 数据记录 | `rosbag record/play/info` |
| 可视化 | `rqt_graph`、`rqt_plot`、`rqt_console` |
