# Gazebo 启动与轮子/关节状态检查命令

> 容器名：`rm_task`  
> 容器内工作区：`/root/chassis_ws`  
> 宿主机工作区：`/home/kang/chassis_ws`  
> ROS：Noetic  

## 0. 进入 Docker 容器

在 Ubuntu 主机终端执行：

```
sudo docker exec -it rm_task bash
```

---
流程：启动 Gazebo
→ 加载机器人模型
→ 加载控制器
→ 再解除暂停
## 1. 终端 A：启动 Gazebo

```
source /opt/ros/noetic/setup.bash
source /root/chassis_ws/devel/setup.bash

export LIBGL_ALWAYS_SOFTWARE=1 //强制用cpu渲染

roslaunch rm_description_for_task hero.launch \
  paused:=true \
  roller_type:=simple
  //启动英雄机器人的 Gazebo 仿真，但先暂停物理仿真，并使用简化的麦克纳姆轮滚子模型。反斜杠 \ 只是 shell 的换行符，等价于一行
```

明天测试 `realistic` 滚子时改成：

```
roslaunch rm_description_for_task hero.launch \
  paused:=true \
  roller_type:=realistic
```
如果一开始不加 paused:=true，模型会立即开始受重力和物理作用，适合直接运行，但调试时不如先暂停稳定。
simple     使用球体近似滚子，仿真速度快
realistic  使用真实滚子网格/胶囊体，接触更接近真实，但计算量大

看到以下内容后保持终端 A 不动：

```text
Successfully spawned entity
Loaded gazebo_ros_control
spawn_urdf process has finished cleanly
```

终端 A 正在前台运行 `roslaunch`，不要在这个终端里输入其他命令。

---

## 2. 终端 B：加载控制器配置

另开一个容器终端：

```
docker exec -it rm_task bash

source /opt/ros/noetic/setup.bash
source /root/chassis_ws/devel/setup.bash

rosparam load /root/chassis_ws/src/simple_chassis_controller/config/controllers.yaml     该行是为了把 controllers.yaml 文件中的内容加载到 ROS 参数服务器中。但是输入后无返回。
```
rosparam   ROS 参数服务器操作工具
load       从 YAML 文件读取参数
/root/chassis_ws/src/simple_chassis_controller/config/controllers.yaml
          容器内 YAML 文件的绝对路径

确认配置已加载：

```
rosparam get /controller/simple_chassis_controller/type
rosparam get /controller/joint_state_controller/type
```

应返回：

```
simple_chassis_controller/SimpleChassisController
joint_state_controller/JointStateController
```

查看所有相关参数
```
rosparam list | grep '^/controller'
```

返回：
```
/controller/joint_state_controller/publish_rate
/controller/joint_state_controller/type
/controller/simple_chassis_controller/type
```

每次 ROS Master 重启后，都需要重新执行 `rosparam load`。

**流程**：
rosparam load->参数服务器获得控制器类型->load_controller->controller_manager根据 type 找到插件并加载->switch_controller->控制器开始运行

因为`controller_manager`的加载服务只知道控制器名字（如`controller/simple_chassis_controller`)，然后它会去参数服务器查找`/controller/simple_chassis_controller/type`。而执行`rosparm load`就是为了让服务器中载入这个参数，避免报错。


---
## 3. 终端 B：加载控制器

```
rosservice call /controller_manager/load_controller \
  "name: 'controller/simple_chassis_controller'"

rosservice call /controller_manager/load_controller \
  "name: 'controller/joint_state_controller'"
```

每次应返回：

```
ok: True
```

查看加载状态：

```
rosservice call /controller_manager/list_controllers
```

此时状态一般是：
（initialized代表初始化，running代表运行）
```text
state: "initialized"
```

注意：`load_controller` 的请求字段是 `name`，不是 `controller`。

---

## 4. 终端 B：解除暂停并启动控制器

先解除 Gazebo 暂停：

```
rosservice call /gazebo/unpause_physics
```

这个服务没有返回内容，直接出现下一个提示符就是正常的。

再启动控制器：

```
rosservice call /controller_manager/switch_controller \
"{start_controllers: ['controller/simple_chassis_controller', 'controller/joint_state_controller'], stop_controllers: [], strictness: 2}"
```

应返回：

```
ok: True
```

再次确认状态：

```
rosservice call /controller_manager/list_controllers
```

两个控制器应变成：

```
state: "running"
```

如果 Gazebo 还处于暂停状态，`switch_controller` 可能一直等待不返回。应先调用 `unpause_physics`。

---

## 5. 查看轮子和关节状态

查看一次完整的 `/joint_states`：

```
rostopic echo -n 1 /joint_states
```

重点字段：

```
name
position
velocity
effort
```

四个轮关节应是：

```text
left_front_wheel_joint
right_front_wheel_joint
left_back_wheel_joint
right_back_wheel_joint
```

持续查看：

```
rostopic echo /joint_states
```

按 `Ctrl+C` 退出。

查看发布频率：

```
rostopic hz /joint_states
```

正常约为：

```
average rate: 50.000
```

---

## 6. 查看 Gazebo 中的底盘位姿

```
rosservice call /gazebo/get_model_state \
  "{model_name: 'hero'}"
```

返回中重点看：

```
pose.position.x
pose.position.y
pose.position.z
twist.linear
twist.angular
```

连续比较两次：

```
rosservice call /gazebo/get_model_state "{model_name: 'hero'}"; \
sleep 3; \
rosservice call /gazebo/get_model_state "{model_name: 'hero'}"
```

---

## 7. 查看 Gazebo 是否暂停

```
rosservice call /gazebo/get_physics_properties
```

重点看：

```
pause: False
```

---

## 8. 查看 ROS 节点和服务

```
rosnode list
rosservice list | grep controller_manager
```

正常应看到：

```
/controller_manager/list_controller_types
/controller_manager/list_controllers
/controller_manager/load_controller
/controller_manager/reload_controller_libraries
/controller_manager/switch_controller
/controller_manager/unload_controller
```

## 9. 今天验证到的状态

今天 `/joint_states` 的结果：

- 发布频率约 `50 Hz`；
- 四个轮关节都有数据；
- `effort` 为 `0.2`，对应原始 `simple_chassis_controller` 的固定力矩；
- 轮子 `velocity` 约 `0.08 rad/s`；
- 但 `get_model_state` 显示底盘位置和 `twist` 几乎不变。

这说明：

- ROS 控制器加载链路已经正常；
- 轮关节确实收到了 effort 命令；
- 车体不前进的问题在机械接触、滚子模型或固定力矩大小，不在插件加载。

下一步优先用 `roller_type:=realistic` 重新测试；如果仍不动，再临时增大 `tau` 做诊断。

---

## 10. 结束仿真

在终端 A 按：

```text
Ctrl+C
```

如果终端卡住，在 Ubuntu 主机终端执行：

```
docker restart rm_task
```

---

## 11. 终端使用原则

- 终端 A：只运行 Gazebo，不输入其他命令；
- 终端 B：执行 `rosparam`、`rosservice`、`rostopic`；
- 所有终端必须在同一个 Docker 容器、同一个 ROS Master 下；
- ROS Master 重启后，必须重新加载 `controllers.yaml` 和控制器；
- 不要在 Gazebo 运行时重复启动第二个 `hero.launch`。


