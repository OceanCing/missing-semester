# ROS架构与通信
## ROS
- Robot Operating System，主要作用是连接起各个节点之间的通信
## master
类似节点通讯录，将节点联系在一起

### e.g.
node a和node b通过在master注册名字（谁发布了什么话题，谁传输了什么数据），经master名字查询（按名字分别交叉介绍给两节点），实现数据直连TCP
- 节点之间数据传输完全不经过master

## roscore
=ros master + rosout
### e.g.
- roscore启动了Parameter Server（参数服务器，用来储存全局配置，比如储存了>小乌龟颜色，速度，坐标这些参数）和rosout（日志记录）两个基础服务。
- roscore里面运行着master（主节点），相当于一个通讯录，为ros中各个作为独立
节点（node）的程序建立联系
- 启动小乌龟时，对应节点会向master注册“我叫turtlesim，我在这里”

## ros_control

## rosnode节点 
单用途的可执行程序，单独编译、执行和管理，以软件包的形式组织起来
- rosnode list 列出正在运行的节点
- rosnode info 列出正在运行的节点的信息
## rostopic
节点之间可以通过topic进行通信。节点可以发布或订阅一个topic，通常有一个发布者和若干个订阅者
- rostopic echo /... 将话题（...）正在发布的信息打印出来
- rostopic info /topic 显示topic的信息，比如由谁发布，谁订阅 
## rosmessage 
定义topic类型的数据结构，理解为一个比较特殊的结构体，集成int



























## rosrun
启动单个节点

## .launch文件
一个XML格式的文本文件，可以一键启动多个节点

- 以turtlesim为例，roslaunch turtle_tf2 turtle_tf2_demo.launch同时拉起了第一只小乌龟节点，第二只小乌龟节点，遥控节点，TF跟随节点
- 打开该程序后，新开一个终端进入docker，输入rosnode list，就可以看到现在在运行的节点，也就是这个launch文件具体启动了哪些节点- 除了装程序时已经顺便装好的一些launch文件，也可以自己在docker里面建立文件夹用XML语言书写lanuch文件。

## 查看某个包的源码
1. roscd（瞬间传送）。比如： roscd turtlesim //闪现到小乌龟的系统安装目录里； 再用ls查看launch和源码文件
2. rosed（编辑）：rosed turtlesim turtlesim_node.cpp //打开小乌龟节点底层c++代码，可以进行编辑
3. rospack（查找某个包的具体路径）：rospack find rviz // 查看rviz的安装路径

## XML语法
## tips
1. 显示驱动
后续只有出现下面情况时，再考虑开 GPU：

- Gazebo 场景变大、机器人变多后明显卡顿。
- RViz 加载点云、深度图、大模型时很慢。
- 需要长时间运行仿真，CPU 占用过高或发热严重。
- 需要比较真实的 3D 渲染效果。

