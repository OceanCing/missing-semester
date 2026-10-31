# 04：话题重映射与位置反馈

## 学了什么

同一个程序可以连接不同话题；同一个节点也可以既订阅状态，又发布控制指令。

本次练习把两个问题分开看：

- **重映射**解决“数据连接到哪个对象”。
- **反馈控制**解决“收到当前状态后，下一条指令怎么决定”。

## 做过的实验：复用程序控制第二只海龟

先确保海龟窗口正在运行，并且已经按[服务练习](03-parameters-services.md)创建 `turtle2`。

两只海龟由同一个 `/turtlesim` 节点管理，并不是创建了两个海龟节点。它们各自使用不同前缀的话题：

| 对象 | 接收速度 | 发布位置 |
| --- | --- | --- |
| turtle1 | `/turtle1/cmd_vel` | `/turtle1/pose` |
| turtle2 | `/turtle2/cmd_vel` | `/turtle2/pose` |

只让第二只前进：

```bash
rosrun turtle_control turtle_motion _linear:=1 _angular:=0 _duration:=3 turtle1/cmd_vel:=turtle2/cmd_vel
```

最后一项把程序原本使用的速度话题改为第二只海龟的话题。我在实验中确认，只有新创建的海龟向右移动。

接收程序也可以复用。在原来运行 listener 的终端按 Ctrl+C 结束旧实例，再运行：

```bash
rosrun turtle_control turtle_pose_listener turtle1/pose:=turtle2/pose
```

可用以下命令确认实际连接：

```bash
rostopic info /turtle2/pose
```

订阅者应包含 `/turtle_pose_listener`。回调代码仍处理相同类型的 Pose 消息，变化的是输入来自哪只海龟。

### 遇到的提示问题

早期代码把启动提示 `Listening to /turtle1/pose` 写成固定字符串，重映射后提示文字没有跟着改变。实际连接需要以 `rostopic info` 为准。

后来提示已改为使用 `subscriber.getTopic()`，重新编译并启动的版本会显示真实订阅话题。字符串日志不一定是框架自动生成的信息，需要结合代码和连接状态判断。

## 做过的实验：根据位置停车

本例只控制第一只海龟的 x 方向，先复位到朝右状态，等上一轮运动结束后运行：

```bash
rosservice call /reset
rosrun turtle_control turtle_go_to_x _target_x:=8 _max_speed:=1
```

再次复位，再比较更高的速度上限：

```bash
rosservice call /reset
rosrun turtle_control turtle_go_to_x _target_x:=8 _max_speed:=2
```

观察的是运行 `turtle_go_to_x` 的那个终端，而不是只看位置接收终端。程序约每半秒打印：

```text
x=5.544 target=8.000 error=2.456 speed=1.000
...
x=7.800 target=8.000 error=0.200 speed=0.200
```

这些是示意数值。跑完后可以向上滚动终端，比较前后行：

| 字段 | 含义 |
| --- | --- |
| x | 当前收到的位置 |
| target | 目标位置 |
| error | 目标位置减当前位置 |
| speed | 本次发送给海龟的速度指令，不是另一次实测速度 |

## 观察与解释

### 发布和订阅连成反馈闭环

```text
海龟发布位置 → 控制节点计算误差 → 发布速度 → 海龟移动 → 下一条位置消息
```

在回调中先计算误差：

```cpp
const double error = target_x - message->x;
```

然后接近目标时减速，并限制速度范围：

```cpp
command.linear.x = std::max(-max_speed, std::min(max_speed, error));
```

这里固定比例增益为 1/s。误差大时速度可能达到上限；误差小时速度也变小；负误差会产生倒退速度。误差绝对值不超过 0.05 时，程序发送零速度并结束。

`max_speed` 是上限，不是全程固定速度。因此上限翻倍，接近目标时仍会减速，总时间不会严格减半。

### 自动验证与我的理解

一次自动验证中，速度上限为 1、2 时，分别停在 x=7.951 和 x=7.952，均在目标误差范围内。验证还检查了已经到达时不移动、Ctrl+C 发出停止指令，以及朝向不适合本课时提示先复位。

我在问答中确认：停车条件不变，改变速度主要影响到达时间，不会让目标坐标也翻倍。

### 重跑时为什么可以保留窗口

初次用 roslaunch 启动海龟和控制程序后，控制节点运动结束会退出，海龟节点继续运行。之后用 rosrun 只启动控制程序，就可以让现有海龟继续运动。按方向键 ↑ 再回车可以重跑上一条命令；若想从中心重新实验，先调用 `/reset`。
