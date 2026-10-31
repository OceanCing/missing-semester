# 10：话题连接、消息类型与频率排查

## 学了什么

节点成功启动后，还要检查它连接的名称、是否有发布者、消息类型是否匹配，以及实际能否持续收到消息。

| 命令 | 查看什么 |
| --- | --- |
| `rosnode info /debug_listener` | 节点的订阅、发布和连接信息 |
| `rostopic info /turtle1/pose` | 话题的类型、发布者和订阅者 |
| `rostopic type /turtle1/pose` | 话题使用的消息类型 |
| `rosmsg show turtlesim/Pose` | 消息类型包含的字段 |
| `rostopic hz /turtle1/pose` | 这个测量工具实际收到消息的频率 |

## 实验：订阅到没有发布者的话题

保持海龟运行，在一个容器开发终端启动使用错误订阅目标的实例：

```bash
rosrun turtle_control turtle_pose_listener \
  __name:=debug_listener \
  turtle1/pose:=/wrong_pose
```

程序应显示正在监听 `/wrong_pose`，随后等待消息。在另一个终端检查：

```bash
rosnode info /debug_listener
rostopic info /wrong_pose
rostopic info /turtle1/pose
```

预期在节点的 Subscriptions 中发现 `/wrong_pose`；这个话题有订阅者 `/debug_listener`，但 Publishers 为 None。真正的位置话题 `/turtle1/pose` 则有发布者 `/turtlesim`。

在 listener 终端按 Ctrl+C，去掉错误重映射后重新运行：

```bash
rosrun turtle_control turtle_pose_listener __name:=debug_listener
```

程序恢复使用默认的位置话题，应该重新打印坐标。这个例子通过修改启动时的连接配置修正问题，无需重新编译 C++。

## 实验：查看类型和字段

```bash
rostopic type /turtle1/pose
rosmsg show turtlesim/Pose
```

位置消息类型为 `turtlesim/Pose`，字段有 `x`、`y`、`theta`、`linear_velocity`、`angular_velocity`。排查连接时，话题名称和消息格式都要与订阅代码一致。

## 实际测得的位置消息频率

```bash
rostopic hz /turtle1/pose
```

本次输出：

```text
average rate: 62.498
    min: 0.015s max: 0.017s std dev: 0.00040s window: 1126
```

| 字段 | 本次含义 |
| --- | --- |
| average rate | 平均每秒收到约 62.5 条消息，相邻消息的平均间隔约 16 ms |
| min、max | 统计窗口内，相邻消息的最短、最长间隔约为 15 ms、17 ms |
| std dev | 相邻消息间隔的标准差约为 0.4 ms，描述间隔的波动 |
| window | 本次统计使用了 1126 条消息 |

这段测量中，消息持续到达，间隔波动较小。按 Ctrl+C 可以结束测量工具。

## 观察与解释：接收频率与日志频率

listener 的回调使用了：

```cpp
ROS_INFO_THROTTLE(1.0, "Pose: x=%.2f y=%.2f theta=%.2f rad", ...);
```

这限制的是这条日志最多每秒打印一次。回调仍会处理收到的位置消息，不能通过终端打印行数判断消息接收频率。

可以按“实际订阅名称 → 发布者 → 消息类型 → 接收频率”的顺序排查。存在已登记的发布者，也不代表它正在持续发送消息；频率测量可以进一步检查数据是否到达。
