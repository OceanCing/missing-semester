# ROS
## 什么是ros？
- ROS 是一个开源的元操作系统：它提供操作系统该提供的那些服务一—硬件抽象、底层设备控制、常用功能的实现、进程间消息传递、软件包管理- 跑在Linux之上的机器人的中间件和工具箱
## ros在车上应用
- 相机驱动
> /image_raw
- 识别
> tack
- 解算
> /gimbal cmd
> cmd_ chassis
> /omd_vel等等
- 决策
> power limt
> heat imit
- 通信中枢
> EtherCAT-电机
> CAN 帧
## 为什么用ros
- ROS 是为机器人量身定做的系统
> 发布/订阅、坐标变换、参数、录包…
> 机器人要的东西，它一开始就当成一等公民
- 生态丰富，调试工具齐全，拥有庞大的开源社区遇到报错先搜一遍，通常已经有人踩过；算法包拿来改，比从零写快一个数量级
- 代码可视化，调试方便，便于团队分工分模块合作
> rqt_graph 一眼看清谁连谁；每人只碰自己那个节点，接口谈好就能并行- 核心节点统一用C++写 (roscpp)
> 性能可控；OpenCV、PCL、Eigen这些库本来就是 C++接口，不用为调库再套一层
- 可回放：rosbag把现场数据录下来
> 赛场上出的问题，回实验室能一帧一帧重放，而不是「当时就那样，复现不了」
- 可仿真：Gazebo 里先把逻辑跑通
> 新队员不碰真车也能练；控制律先在虚拟场地里撞够，再上实车
###纯上位机- 上位机→EtherCAT→电机×n
## ros架构
- **文件系统层**
- **计算图层**
- **通信层**
### ros基本零件
- 节点
- 话题
- 消息
- 服务
- 参数
### master
- 名字注册
- 名字查询
- 参数服务器
- 状态监控
> roscore = Master + rosout （日志）+参数服务器
### ros_control
- 话题 /cmd_vel
- Controller
- RobotHW 接口
- 下位机→电机
### ros包中
- catkin_ws/
- src
- CMakeLists.txt
- my_pkg/
- build/
- devel/
- setup.bash
## 创建ROS包
- package.xml
- CMakeList.txt
## 三大通信
- 话题
- 服务
- 参数
## 配置文件
- 空格缩进
- 冒号后加空格
- 大小写敏感
- #是注释
- ---可以分割文件
