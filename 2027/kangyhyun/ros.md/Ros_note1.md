#ROS笔记1
#26智能感知工程康宸斌

#学习资料：
1.视频资源：https://www.bilibili.com/video/BV1Ci4y1L7ZZ/?vd_source=239e69d170cafabced8f0e819a593318
2.讲义：https://www.autolabor.com.cn/book/ROSTutorials/chapter1/11-rosjian-jie-yu-an-zhuang/111rosgai-nian.html

#一、ROS概念
-1.ROS（Robot Operating System)(机器人操作系统）是适用于机器人的开源元操作系统。
-2.Ros=Plumbing（通讯机制）+ Tools(工具软件包） + Capabilities（机器人高层技能）+ -Ecosystem（机器人生态系统）的集合体。

#二.ROS优点
-1.**支持代码复用**。
-2.具有**进程（Nodes）分布式框架**，可适用于不同主机协同工作。
-3.**松耦合**：ROS的功能模块封装于独立的功能包，便于分享。功能模块以节点为单位运行，以ROS标准的IO作为接口，开发者只需要了解接口规则就能实现复用。

-例：底盘控制、雷达驱动和摄像头控制都是独立的功能模块，就算其中一个崩溃，其他模块也不会受到影响。

-4.**语言独立性**：具有与语言弱相关的结构。即各个模块无论用什么语言写都不影响模块相互交流。

-例子：（1）.C++节点发布的数据是由ROS定义好的标准数据格式，而Python节点只需要认识这个格式就能接受和解析。
（2）.如终端写的`rostopic list` `roslaunch` `rosbag`等命令可以直接管理底下的节点。

-5.**易于测试**：ROS具有称为rostest的内置单元/集成测试框架，可轻松安装和拆卸测试工具。
-6.**适用于大型项目的运行**。
-7.**免费且开源**。

-以`sample_chassis_conmtroller`举例解释
（1）.gazebo是一个独立的进程，负责物理引擎的计算，并不断把四个轮子的实时状态（joint_states)发出。
（2）.`simple_chassis_controller`也是一个独立的进程。收到轮子的实时状态的数据后算出PID再把控制指令发送会gazebo。
（3).终端（teleop_twist_keyboard)也是第三个进程，负责发/cmd_vel。

