# 11：用 YAML 保存启动参数

## 学了什么

YAML 可以将配置值保存成文件。启动时，launch 将文件中的值加载到 ROS 参数服务器，程序再读取这些参数。

```text
YAML 文件
    → launch 加载
    → ROS 参数服务器
    → C++ 读取到变量
    → 生成并发布速度指令
```

本次使用 [motion.yaml](turtle_control/config/motion.yaml) 和 [configured_motion.launch](turtle_control/launch/configured_motion.launch)。

## 做过的实验：加载并查询配置

配置文件内容为：

```yaml
linear: 0.5
angular: 1.0
duration: 6.3
```

每行使用“名字: 值”的格式，冒号后保留空格。这里保存前进速度、转弯角速度和运行时长，全部使用数值。

保持海龟及 ROS master 运行，确保上一轮控制程序已经结束，在容器开发终端执行：

```bash
roslaunch turtle_control configured_motion.launch
```

运行结束后，在原来的 master 仍运行时查询：

```bash
rosparam get /turtle_motion
```

本次已确认查询中的三个值与配置文件一致：linear 为 0.5、angular 为 1.0、duration 为 6.3。查询结果中字段顺序可以不同。

## 关键配置

启动文件先声明配置文件路径，再在控制节点内部加载：

```xml
<arg name="config_file" default="$(find turtle_control)/config/motion.yaml"/>
<node pkg="turtle_control" type="turtle_motion" name="turtle_motion" output="screen">
  <rosparam command="load" file="$(arg config_file)"/>
</node>
```

`$(find turtle_control)` 找到软件包目录，`$(arg config_file)` 取出本次启动使用的配置路径。`rosparam` 放在 node 内部，所以参数加载到该节点的私有命名空间，例如 `/turtle_motion/linear`。

软件包名用于寻找程序，节点名决定本次实例的私有参数前缀。这里二者都沿用现有配置，C++ 通过私有句柄 `~` 读取参数。

## 观察与解释

YAML 文件保存磁盘上的配置，参数服务器保存本次运行加载的值，C++ 变量保存程序已读取的数值。这三处需要通过加载和读取操作传递数据。

当前控制程序只在启动时读取一次参数。修改 YAML 后，需要重新运行 `configured_motion.launch`，让它重新加载文件并启动控制程序。

只修改 YAML 不需要重新编译 C++。如果改的是 `.cpp` 中的处理逻辑，就需要先编译，再启动新的程序。文件保存下来的配置可以随源码一起管理，方便在其他环境复现。
