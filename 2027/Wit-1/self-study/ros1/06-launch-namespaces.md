# 06：launch、节点命名与命名空间

## 学了什么

launch 文件用来集中组织节点启动、参数设置和名称重映射。一个启动文件可以运行多个程序，也可以让同一个程序运行多个实例。

| 内容 | 作用 |
| --- | --- |
| `node` | 指定要启动的程序和这次运行的节点名 |
| `arg` | 接收 launch 的启动选项 |
| `param` | 设置 ROS 参数服务器中的配置值 |
| `group ns` | 给一组节点设置命名空间 |
| `remap` | 将程序使用的名称连接到另一个名称 |

本次使用的文件是 [lesson.launch](turtle_control/launch/lesson.launch) 和 [listeners.launch](turtle_control/launch/listeners.launch)。

## roscore、rosrun 和 roslaunch

| 命令 | 作用 | 本次使用方式 |
| --- | --- | --- |
| `roscore` | 启动 Master、参数服务器和 rosout 日志节点 | 单独启动 ROS 基础服务 |
| `rosrun` | 从指定包中找到并执行一个程序 | `rosrun turtlesim turtlesim_node` 打开海龟窗口 |
| `roslaunch` | 读取启动文件，组织节点、参数和重映射 | `roslaunch turtle_control lesson.launch` 按配置运行海龟与控制节点 |

从尚未启动 ROS 的状态开始，单独用 rosrun 打开海龟，需要先在一个终端运行 `roscore`，保持运行，再在另一个终端运行海龟程序。我们当前的本机配置中，roslaunch 会在需要时自动启动 Master，已有 Master 则复用。

`lesson.launch` 中配置了海龟与运动控制两个节点，因此默认还会执行一轮运动。`rosrun turtlesim turtlesim_node` 运行的是官方海龟程序本身。

## 实验：通过 launch 传入参数

在容器开发终端运行；若之前的 `lesson.launch` 还在运行，先在它的终端按 Ctrl+C 结束上一轮启动：

```bash
roslaunch turtle_control lesson.launch linear:=0.5 angular:=0 duration:=3
```

海龟缓慢直行约 3 秒后停止，窗口保留。保持这个 launch 运行，在另一个开发终端查询：

```bash
rosparam get /turtle_motion/linear
```

本次查询结果为 `0.5`，运动表现也符合设置。

### pkg、type 和 name 的区别

启动文件中的控制节点配置为：

```xml
<node pkg="turtle_control"
      type="turtle_motion"
      name="turtle_motion"
      output="screen">
  <param name="linear" value="$(arg linear)" type="double"/>
</node>
```

这是从现有文件中摘出的节点片段，`linear` 的 arg 在节点外声明。

| 字段 | 在本例中的含义 |
| --- | --- |
| `pkg` | 到 `turtle_control` 软件包中找程序 |
| `type` | 运行编译生成的 `turtle_motion` 可执行程序 |
| `name` | 将这次运行的节点命名为 `turtle_motion` |
| `output="screen"` | 将节点日志显示在终端 |

程序名和运行时节点名是两个概念。同一个可执行程序可以运行多次，各实例使用不同的完整节点名。

### arg 和 param 的连接

文件中先声明启动选项：

```xml
<arg name="linear" default="1.0"/>
```

再在节点内部设置参数：

```xml
<param name="linear" value="$(arg linear)" type="double"/>
```

`$(arg linear)` 表示取出这个启动选项的值。数据经过下面的路径进入控制程序：

```text
命令中的 linear:=0.5
    → launch 的 arg
    → param 写入 /turtle_motion/linear
    → 控制程序启动时读取参数
    → 填入速度消息并发布
```

`arg` 本身不会自动成为 ROS 参数。这里通过 `<param>` 将它写入参数服务器。因为 param 放在该 node 内部，名称为 `/turtle_motion/linear`。

C++ 通过私有句柄读取这个参数：

```cpp
ros::NodeHandle private_nh("~");
private_nh.param("linear", linear, 1.0);
```

`~` 表示当前节点的私有命名空间。参数何时生效见[参数笔记](03-parameters-services.md#运行时修改参数与实际速度)。

## 实验：同一个程序运行两个实例

保持海龟及 `lesson.launch` 运行，在另一个开发终端执行：

```bash
roslaunch turtle_control listeners.launch
```

这个文件复用 `turtle_pose_listener` 程序，启动两个独立节点：

```text
/observer_a/pose_listener
/observer_b/pose_listener
```

两个实例运行相同代码，有各自的进程和订阅。它们当前都订阅 `/turtle1/pose`，因此接收同一只海龟的位置。

在第三个开发终端可以核对：

```bash
rosnode list
rostopic info /turtle1/pose
```

节点列表和话题的 Subscribers 中应包含上述两个名称。

## 观察与解释：命名空间和话题重映射

其中一个实例的启动配置是：

```xml
<group ns="observer_a">
  <node pkg="turtle_control" type="turtle_pose_listener"
        name="pose_listener" output="screen">
    <remap from="turtle1/pose" to="/turtle1/pose"/>
  </node>
</group>
```

命名空间是 ROS 名称的分组前缀，不需要在磁盘上建立同名文件夹。两个节点虽然末尾都叫 `pose_listener`，完整名称不同，因此可以同时运行。

程序中的订阅名称为：

```cpp
auto subscriber = nh.subscribe("turtle1/pose", 10, poseCallback);
```

在普通 `ros::NodeHandle nh` 下，名称开头是否带 `/` 会影响解析结果：

| 代码中的名称 | 节点位于 `/observer_a` 时，重映射前的完整名称 |
| --- | --- |
| `turtle1/pose` | `/observer_a/turtle1/pose` |
| `/turtle1/pose` | `/turtle1/pose` |

`from` 写程序原本使用的名称，`to` 写实际连接的目标。这里把原本会落在 observer_a 命名空间中的订阅，连接到现有海龟的 `/turtle1/pose`。

若把 C++ 的订阅直接改成绝对名称 `/turtle1/pose`，在本例中就不需要这条重映射，但修改 C++ 后需要重新编译。保留相对名称、通过 launch 决定连接目标，可以复用同一个程序。

例如，要让第二个实例观察海龟2，在 `observer_b` 组中使用：

```xml
<remap from="turtle1/pose" to="/turtle2/pose"/>
```

前提是已经创建 `turtle2`，让它发布位置。连接关系会成为：

```text
/turtle1/pose → /observer_a/pose_listener
/turtle2/pose → /observer_b/pose_listener
```

在当前源码工作空间中，调整这些 launch 配置后重新启动对应 launch 即可；无需重新编译 C++。
