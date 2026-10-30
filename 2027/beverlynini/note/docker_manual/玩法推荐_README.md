## 1. 两只乌龟：第二只跟着第一只走

roslaunch turtle_tf2 turtle_tf2_demo.launch

会启动小乌龟、遥控和 TF 跟随程序。用方向键控制 turtle1，turtle2 会跟着它走，很适合理解坐标变换。

## 2. 机器人模型 + 关节滑条

roslaunch urdf_tutorial display.launch

会打开 RViz，并显示一个机器人模型。界面里有滑条，可以拖动关节看模型变化。

换成更复杂的模型：

roslaunch urdf_tutorial display.launch \
  model:=$(rospack find urdf_tutorial)/urdf/06-flexible.urdf

## 3. Gazebo 里的完整机器人

roslaunch urdf_sim_tutorial 13-diffdrive.launch

会同时打开：

- Gazebo 3D 仿真
- RViz
- 机器人模型
- rqt_robot_steering 遥控窗口

这是一个带轮子、关节、控制器的小车机器人，可以用转向窗口控制它移动。第一次启动会慢一些。

## 4. 3D 物理世界

roslaunch gazebo_ros shapes_world.launch

打开一个有各种几何物体的 3D 世界，可以看物体受重力、碰撞和摩擦影响。

其他自带世界：

roslaunch gazebo_ros empty_world.launch
roslaunch gazebo_ros willowgarage_world.launch
roslaunch gazebo_ros rubble_world.launch
roslaunch gazebo_ros mud_world.launch

## 5. 2D 机器人仿真：Stage

rosrun stage_ros stageros \
  /opt/ros/noetic/share/stage_ros/world/willow-erratic.world

这是 2D 机器人仿真器，适合看机器人在地图里移动、避障和测距。

## 6. ROS 可视化工具

这些不是机器人本身，但配合小乌龟和机器人很好玩：

rqt_graph

查看节点和话题的连接关系。

rqt_plot /turtle1/pose/x /turtle1/pose/y

把乌龟的坐标画成曲线。

rqt_robot_steering

打开一个类似游戏手柄的遥控窗口。

rqt_tf_tree

查看机器人各个坐标系之间的父子关系。

rqt_image_view

查看摄像头图像，但需要有摄像头话题才能看到画面。

## 7. 小乌龟自带的另外两个程序

启动小乌龟后，还可以运行：

rosrun turtlesim draw_square

让小乌龟自动画一个正方形。

rosrun turtlesim mimic

让小乌龟模仿另一只乌龟的运动。

## 8. 动作服务器示例

rosrun turtle_actionlib shape_server

再开一个终端：

rosrun turtle_actionlib shape_client

这是 actionlib 的示例，用来理解“长时间任务”和“目标/反馈/结果”的机制。

## 推荐游玩顺序

1. turtle_tf2_demo.launch：两只乌龟跟随
2. urdf_tutorial display.launch：机器人模型和关节滑条
3. urdf_sim_tutorial 13-diffdrive.launch：Gazebo 小车
4. gazebo_ros shapes_world.launch：3D 物理世界
5. stage_ros：2D 机器人仿真
