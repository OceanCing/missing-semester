#ROS第一课：架构与通信
##一.对ROS概念的初步认识：之前我一度误以为ROS是一个类似于Windows的独立操作系统。通过这节课的学习，我纠正了这个认知：ROS本质上是一个运行在 Linux 之上的通信中间件与工具箱。通信中间件（节点之间唯一的对话方式）：话题、服务、参数；工具链：（catkin：编译，launch：编排，rviz、rqt：可视化，rosbag：录包回放）；软件生态：OpenCV/PCL/Eigen、Navigation/Movelt、相机、雷达、IMU驱动
##二、架构的三层划分与排错逻辑：
1.文件系统层：解决代码组织问题，包含工作空间、功能包以及CMakeLists、package.xml、msg/srv、、launch、cpnfig
2.计算图层：解决程序协作问题，包含节点、话题、服务、参数和消息
3.通信层：解决数据流转问题，底层依赖TCPROS/UDPROS（节点之间建一条TCP）
（排错口诀记录：找不到包：source了吗；连不上：计算图层名字对不对；延迟高：通信层带宽和队列；启动崩溃直接：终端的FATAL）
##三、三大通信机制的区别：
1.话题（Topic）：连续变化的数据流（传感器数据、里程计、检测结果、控制指令），多对多，发送者不需要管接收者。
2.服务（Service）：需要一个明确的结果，一对一，调用不频繁
3.参数服务器（Parameter Server）：是配置不是数据，启用是读一次就够
##四、 实践操作：
我使用的是较新的Ubuntu 26.04 系统，ROS2 ，与课上ROS1不太一样，于是求助了deeps
在解决了一些环境变量配置和镜像源问题后，我成功使用run turtlesim turtlesim_node和turtle_teleop_key跑通了小乌龟，并通过rqt_graph直观地看到了节点与话题的数据流向，且让deepseek给我出了一些题目，如查找history|grep sudo等检验学习
