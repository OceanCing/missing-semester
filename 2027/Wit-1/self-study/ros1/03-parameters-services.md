# 03：参数、服务与请求响应

## 学了什么

这几种机制解决的问题不同：

| 机制 | 用途 | 本次例子 |
| --- | --- | --- |
| 话题 topic | 发布、接收消息，适合持续传递状态或指令 | 位置、速度 |
| 参数 parameter | 保存可供节点读取的配置值 | 目标 x、最高速度 |
| 服务 service | 发出一次请求，等待一次响应 | 复位、瞬移、创建海龟 |

## 做过的实验：启动参数

```bash
rosservice call /reset
rosrun turtle_control turtle_go_to_x _target_x:=9 _max_speed:=1
rosparam get /turtle_go_to_x/target_x
```

`_target_x:=9` 设置当前节点的私有参数。完整名称中 `/turtle_go_to_x` 是节点的私有命名空间，`target_x` 是参数名。

修改启动参数不用重新编译，因为没有修改 C++ 处理逻辑，只是程序读到了不同值。

### 参数存在哪里、什么时候读取

这些配置值保存在运行中的 ROS 参数服务器里。节点退出后，参数通常仍可查询；当前 ROS master 结束后，不会自动保留成下一次运行的持久配置。需要复用的设置可以写进启动文件等配置中。

本例只在启动时读取一次参数：

```cpp
private_nh.param("target_x", target_x, 8.0);
private_nh.param("max_speed", max_speed, 1.0);
```

所以改变参数服务器里的值，不会自动更新程序已经读入的变量。想动态改变运行目标，需要程序另外实现相应机制，这是后续内容。

## 运行时修改参数与实际速度

保持海龟窗口运行，先结束上一轮运动，在一个开发终端让它持续画圆：

```bash
rosservice call /reset
rosrun turtle_control turtle_motion _linear:=1 _angular:=1 _duration:=120
```

在运动期间，另一个开发终端修改并查询参数：

```bash
rosparam set /turtle_motion/linear 2.0
rosparam get /turtle_motion/linear
rostopic echo -n 1 /turtle1/cmd_vel
```

`-n 1` 表示只查看一条话题消息，然后退出。按当前代码，参数查询应得到 `2.0`，但速度消息中的 `linear.x` 仍为 `1.0`。

### 参数值、程序变量和速度消息是三份数据

| 数据位置 | 启动时 | 运行期间将参数设为 2 后 |
| --- | --- | --- |
| 参数服务器中的 `/turtle_motion/linear` | 1 | 2 |
| 控制程序的 C++ 变量 `linear` | 1 | 1 |
| 发布的消息 `command.linear.x` | 1 | 1 |

现有程序只在启动时读取参数，并将数值复制到变量，再填入速度消息：

```cpp
private_nh.param("linear", linear, 1.0);
command.linear.x = linear;
```

运动循环持续执行的是：

```cpp
publisher.publish(command);
```

这只会发送已经填好的消息，没有重新读取参数，也没有向参数服务器写值。因此参数仍是 2，控制程序仍发送速度 1；并不是参数先变成 2，又被循环改回 1。

海龟执行的是 `/turtle1/cmd_vel` 收到的速度指令。它不会主动读取控制节点的 `/turtle_motion/linear` 参数：

```text
参数服务器
    → 控制节点启动时读取一次
    → C++ 变量
    → 填入并发布速度消息
    → 海龟接收指令并运动
```

### 配置参数的实际用途

参数适合保存模型路径、相机配置、检测阈值和速度上限等设置。常用做法是写在 launch 或 YAML 文件中，启动时加载；运行时更新参数是否立即生效，取决于节点是否实现了重新读取和更新行为。

对本例这个只读取一次参数的运行实例，单独执行 `rosparam set` 不会改变运动。若想实时调参，需要程序支持相应机制。

重启也要看启动配置：若 launch 或命令行又设置了 `linear:=1`，就会覆盖之前临时设置的 2。需要长期保留的设置应修改对应配置文件。

观察完后，在控制程序终端按 Ctrl+C 停止运动。launch 中 `arg` 与 `param` 的连接见[启动文件笔记](06-launch-namespaces.md#arg-和-param-的连接)。

## 做过的实验：调用服务

### 复位

```bash
rosservice info /reset
rosservice type /reset
rosservice call /reset
```

服务由 `/turtlesim` 节点提供，类型是 `std_srvs/Empty`。`Empty` 的请求、响应都没有字段，但服务仍会执行动作并返回响应。

### 指定坐标和朝向

```bash
rossrv show turtlesim/TeleportAbsolute
rosservice call /turtle1/teleport_absolute "{x: 3.0, y: 7.0, theta: 1.5708}"
```

接口定义是：

```text
float32 x
float32 y
float32 theta
---
```

`---` 上面是请求字段，下面是响应字段。请求中指定位置和朝向，theta 单位是弧度；约 1.5708 弧度表示 90°。响应不带字段，命令可能显示 `{}`，这是空响应，不代表没有执行。

这里的 `x`、`y`、`theta` 是这次服务请求的数据，不是往参数服务器里保存 ROS 参数。瞬移是模拟器提供的功能，不是让真实机器人沿路径走过去的控制方法。

### 创建第二只海龟，观察返回数据

```bash
rossrv show turtlesim/Spawn
rosservice call /spawn "{x: 3.0, y: 3.0, theta: 0.0, name: 'turtle2'}"
```

接口的请求和响应都有数据：

```text
float32 x
float32 y
float32 theta
string name
---
string name
```

本次请求创建 `turtle2`，响应返回创建的名称。若已经存在同名海龟，重复创建会失败；此时先使用已有对象，或换一个名字。`/reset` 会恢复默认海龟状态，之后需要重新创建第二只才能重复多海龟实验。

## 观察与解释

### 为什么命令没写节点名，也能找到海龟

海龟节点启动时注册 `/reset`、`/spawn` 等服务。调用者先通过 ROS master 查询对应服务地址，再向实际提供服务的节点发送请求，不需要把节点名写在每次调用命令里。

```text
rosservice call /reset
    → 查询服务地址
    → 请求 /turtlesim 执行 /reset
    → 海龟节点执行回调并返回响应
```

`/reset` 是官方海龟程序提供的服务名。通用工具是 `rosservice`；ROS 不会仅凭“reset”这个单词就自动知道如何复位。

代码中的注册逻辑可简化理解为：

```cpp
auto service = nh.advertiseService("reset", resetCallback);
```

服务请求到来后，框架调用登记的服务回调函数；具体动作写在服务端程序里。上面只是说明注册关系的片段，不是官方源码的完整实现。

### rosservice 和 rossrv 为什么不是同一个工具

| 命令 | 看哪个对象 | 解决什么问题 |
| --- | --- | --- |
| `rosservice info /reset` | 运行中的服务名称 | 谁提供服务、类型和联系地址是什么 |
| `rosservice type /turtle1/teleport_absolute` | 运行中的服务名称 | 使用哪一种接口类型 |
| `rosservice call /reset` | 运行中的服务名称 | 实际发送请求 |
| `rossrv show turtlesim/TeleportAbsolute` | 服务接口类型 | 请求和响应各有哪些字段 |

因此，`/turtle1/teleport_absolute` 是服务名称，`turtlesim/TeleportAbsolute` 是类型名称。只要安装了对应定义并加载环境，通常不启动海龟也能查看类型；实际调用需要有节点提供服务。

`.srv` 文件定义请求与响应的格式；它不是运行中服务的磁盘消息存储文件。一个接口类型也可以被多个服务名使用。
