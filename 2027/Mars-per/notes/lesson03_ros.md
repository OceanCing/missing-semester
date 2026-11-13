#1. ROS的介绍

## 1.1 ROS是什么
- ROS的全称是Robot Operating System (机器人操作系统）
- 虽然名称为操作系统，但严格意义上，其不是像Windows，Linux那样的真正的操作系统，而是跑在Linux上的一套软件框架*（元操作系统）*，更像是“机器人中间件+工具箱”
- 里面包含了许多节点，即机器人程序被拆成的许多独立进程，其之间通过消息互相通信。

## 1.2 组成
| 部分 | 是什么 | 例子 |
| --- | --- | --- |
| 通信中间件 | 节点之间对话的唯一方式 | 话题（连续数据流，与订阅有关）、服务（一次问答）、参数（全局配置）|
| 工具链 | 编译、启动、看、录 | catkin编译、launch编排、rviz/rqt可视化、rosbag录包回放 |
| 软件生态 | 开源包（C++居多） | OPenCV/PCL/Eigen、Navigation/MoveIt、相机IMU驱动 |

## 1.3 为什么要使用ROS
- 是为机器人量身定做的：发布/订阅；坐标变换、参数、录包……机器人要的东西，它一开始就是一等公民
- 生态丰富：遇到报错先搜一遍，通常已经有人踩过；算法包拿来改，比从零写快一个数量级
- 便于团队分工：`rqt_graph`一眼看清谁连谁；每个人只接触自己的节点，接口连接好就可以并行发行
- 核心用C++（roscpp）：OpenCV、PCL、Eigen 本来就是 C++ 接口，不用为调库再套一层
- 可回放：`rosbag`把现场数据录下来，赛场上出的问题回实验室一帧一帧重放
- 可仿真：Gazebo里先把逻辑跑通再上实车
# 2.ROS的架构（三层）

| 名称 | 作用 | 简述 |
| --- | --- | --- |
| 文件系统层 | 工作空间-功能包-源码/配置 | 解决代码怎么组织 |
| 计算图层 | 节点、话题、服务、参数、消息 | 解决程序怎么协作 |
| 通信层 | TCPROS/UDPROS：节点之间建一条TCP | 解决数据怎么走 |

## 2.1 文件系统层
工作空间catkin_ws的四个目录：
```
catkin_ws/
├── src/            # 代码都在这里（进版本库）
│   └── my_pkg/     # 一个功能包
├── build/          # 编译中间文件（可删，别进 git）
├── devel/          # 编译产物：setup.bash、可执行文件
└── setup.bash      # 每开一个终端都要 source 它
```
**有几点要注意一下**
- 源码和产物分离：src进Git保存，build/devel不进，别人clone下来重新编译就行
- 每次开终端都要先source：source devel/setup.bash
- 包名必须全局唯一，否则rosrun会认错
一个功能包的八样东西（ROS的最小复用单位）
```
my_pkg/
├── CMakeLists.txt      # 编译规则：编什么、链什么
├── package.xml         # 名片：名字、版本、依赖
├── include/my_pkg/     # 对外暴露的头文件 .h/.hpp
├── src/                # C++ 源文件 .cpp
├── scripts/            # 可执行脚本
├── msg/                # 自定义消息 .msg
├── srv/                # 自定义服务 .srv（注意不是 service/！）
├── launch/             # 启动文件 .launch（一次拉起多个节点）
├── config/             # 参数文件 .yaml
└── README.md           # 给人看的：怎么编、怎么跑
```
**两个很重要的文件**
- package.xml（名片）：包名（必须与文件夹同名）、版本、依赖
 `build_depend`：编译期需要（头文件、库、消息包）
 `exec_depend` ：运行期需要（动态库、消息运行库）
 `depend`：build+build_export+exec
 自定义msg/srv要额外加message_generation(编译期）和message_runtime（运行期）

- CMakeListstxt.txt（编译规则）
 `find_package(catkin REQUIRED COMPONENTS roscpp std_msgs)`--包名要和package.xml对得上
 `add_message_files`/`add_service_files`/`generate_messages`--三个函数配套出现
 每加一个cpp就加一条`add_executable+targe_link_libraries`

## 2.2 计算图层
| 零件 | 英文 | 是什么 | 
| --- | --- | --- |
| 节点 | Node | 一个独立跑着的进程，干一件小事 |
| 话题 | Topic | 一条有名字的广播频道，发布者写，订阅者读 |
| 消息 | Message | 话题上传输的数据结构 | 
| 服务 | Service | 一问一答的远程调用，有请求也有应答 |
| 参数 | Parameter | 全局共享的配置表key-alue |

## 2.3 通信层
- 节点之间通过TCPROS连接，大数据/实时场景有UDPROS
- 它负责：连接建设、序列化、队列、断线重连

# 3.节点间的通信
## 3.1 节点Node
- 节点是一个独立跑着的进程，干一件小事
- 一个机器人系统= 很多节点的组合：相机节点，识别节点，决策节点，控制节点......
- 节点之间只能通过话题/服务/参数通信，没有共享内存、全局变量那一套

## 3.2 ROS Master
**数据根本不经过Master**
- Master只干四件事
 1.名字注册：谁发布了什么话题、谁提供了什么服务
 2.名字查询：按名字把双方介绍给对方
 3.参数服务器：全局参数的存放地
 4.状态监控：谁还活着，谁掉线了
- roscore = Mastr + rosout（日志） + 参数服务器
*推论：Marster挂了，已经连上的节点照常通信，新来的节点连不上。*

## 3.3 一次话题通信的完整六步
- 启动 Master：roscore 起在 11311 端口，等别人来注册
-  发布者报到：节点启动即向 Master 注册：我发布 /cmd_vel，类型 Twist
- 订阅者报到： 订阅者注册：我要订阅 /cmd_vel
- Master 回地址： 把发布者的 IP 和端口告诉订阅者（只此一次）不是数据本身，而是地址
- 订阅者去连接： 订阅者主动连发布者，握手确认消息类型一致
- 直连传数据：之后每条消息都走这条 TCP 连接，Master 不再出现

## 3.4 话题通信Topic（发布/订阅）
### 3.4.1 特点
- 异步：发完就返回，不等任何人
- 多对多：一个话题可以有很多发布者和订阅者
- 有队列：`queue_size`是缓存深度，满了会丢数据
- 断线自动重连：订阅者晚启动也能接上

### 3.4.2 组成：四个角色
- 发布者Publisher：往话题里写数据的节点
- 订阅者Subscriber：从话题里读数据的节点
- 消息Message：数据的类型，由.msg定义
- 名字+类型：话题名+消息类型都相同才能连上

### 3.4.3 发布端代码示例
```
// talker.cpp
#include <ros/ros.h>  //引入ROS的基础接口
#include <std_msgs/String.h>

int main(int argc, char** argv) {   //arge和argv保存命令行参数
    ros::init(argc, argv, "talker");    //创建一个节点：talker 
    ros::NodeHandle nh;                        // NodeHandle：ROS的操作入口，通过它才能创建发布者、订阅者以及访问ROS参数
    ros::Publisher pub = nh.advertise<std_msgs::String>("/chatter", 10);  // 创建一个发布者，发布到/chatter话题，消息类型是std_msgs::String,发送队列长度是10
    ros::Rate rate(1);                         // 设置循环频率（这里是一秒一次）
    while (ros::ok()) {  //ROS正常运行，循环就会继续
        std_msgs::String msg;   //每个循环都创建一个字符串消息对象
        msg.data = "hello";    // 填写消息内容
        pub.publish(msg);   //把消息发送到/chatter话题
        rate.sleep();   //等待到下一个周期
    }
    return 0;
}
```

### 3.4.4 订阅端代码示例
```
// listener.cpp
#include <ros/ros.h>
#include <std_msgs/String.h>

void cb(const std_msgs::String::ConstPtr& msg) {   //cd是回调函数，订阅者收到消息后调用这个函数
    ROS_INFO("I heard: %s", msg->data.c_str());
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "listener");   //创建名为listener的ROS节点
    ros::NodeHandle nh;   //创建连接入口
    ros::Subscriber sub = nh.subscribe("/chatter", 10, cb);   // 订阅 /chatter 话题；接收消息类型根据 cb 的参数推断为 std_msgs::String；接收队列长度为 10
    ros::spin();   //进入ROS的回调处理循环
    return 0;
}
```

### 3.4.5 命令行
```
rostopic list   # 列出所有活跃话题
rostopic echo /topic   # 把话题上的内容打到终端
rostopic info /topic   # 谁在发布、谁在订阅、类型是什么
rostopic hz /topic      # 测频率
rostopic pub -r 10 /topic <type> '{...}'   # 手动发一条
```

## 3.5 服务通信Service（请求/应答）
**先定义，后调用**
### 3.5.1 特点
- 同步阻塞：调用期间客户端线程停住,直到拿到结果或者超时
- 一对一：一个服务对应一个实现
- 高频场景不要用：每次调用都要建连，序列化

### 3.5.2 组成：两个角色+一份契约
- 服务端Server：提供能力，注册一个回调处理请求
- 客户端Client：主动发起调用，等结果
- Request/Response：请求和问答两段数据结构
- .srv文件 上下两段用---隔开，上面请求、下面应答

### 3.5.3 .srv文件
```
# srv/Spawn.srv —— 上段请求，下段应答
float32 x
float32 y
float32 theta
string name
---
string name      # 返回：新乌龟的名字
```

### 3.5.4 命令行
```
rosservice list   # 列出当前所有服务
rosservice type /spawn     # 看这个服务的类型
rosservice call /spawn "x: 2.0 y: 3.0 theta: 0.0 name: 't2'"   # 手工填参数调一次，最快的验证方法
rossrv show turtlesim/Spawn   # 看.srv的字段定义
```

## 3.6 参数服务器Parameter Server
**本身不流动，该一次所有人都能看**
### 3.6.1 组成
- 全局key-value，存在Master里，所有节点都能访问
- 树形命名空间：`/robot/camera/exposure`这样一层一层往下
- 六种类型：int、double、bool、string、list、dict
- 随roscore存在，roscore一关就没了

### 3.6.2 特点别把它当作数据库
- 以静态配置为主：启动时读一次，改参数不用重启节点
- 谁都能set，改完所有节点下次get都能看到
- ROS里改参数不会触发回调，不用重启节点

### 3.6.3 C++读写
```
// 读一个值，带默认值最稳
int exposure;
nh.param<int>("camera/exposure", exposure, 30);

// 私有参数：只属于这个节点
ros::NodeHandle pnh("~");
double gain = 1.0;
pnh.param<double>("gain", gain, 1.0);

// 读整棵子树
XmlRpc::XmlRpcValue tree;
nh.getParam("camera", tree);
```

### 3.6.4 三种参数名作用域
| 写法 | 含义 |
| --- | --- |
|`/robot/speed` | 绝对名：从根开始，换命名空间也指同一个地方 |
|`~speed` | 私有名：等价于`/节点名/speed`，只属于这个节点 |
|`speed` | 相对名：拼在当前命名空间后面，随launch的ns变 |

### 3.6.5 命名行
```
rosparam list                    # 列出所有参数（配合 grep 过滤）
rosparam get /cam/exposure       # 读一个
rosparam set /cam/exposure 30    # 改一个
rosparam load my_params.yaml /cam   # 从 YAML 灌入
rosparam dump my_params.yaml /cam   # 导出成 YAML
rosparam delete /cam             # 删掉一棵子树
```
# 4.YAML
## 4.1 五条硬规矩*报错`Error parsing YAML`,九成是五条里的一条*
- 只用空格缩进，绝不用Tab  两个空格算一级，同一层必须严格对齐
- 冒号后面必须有空格  key: value对；key:value错————它会被当成一个字符串
- 大小写敏感  True和true不是一回事；ROS中按bool解析的是true/false，不要写yes/no
- #是注释  可以整行，也可以在行尾；值里要用#就加引号
- `---`可以分隔多个文档  空文件也是合法的，等于一个空字典

## 4.2 YAML的三种结构：标量、列表、字典
*所有复杂的东西，都是这三种嵌套出来的
### 4.2.1 标量(scalar)
```
speed: 1.5   # 数字
name: "sentry"   # 字符串
debug: true   # 布尔
empty: null   # 空值
```

### 4.2.2 列表(list)
```
ports: 
  - 8080
  -8081
# 行内写法
ids: [1, 2, 3]
```

### 4.2.3 字典(dict)
```
camera: 
  width: 640
  height: 480
# 行内写法
point: {x: 1, y: 2}
```

## 4.3 三种加载方式
### 4.3.1 命令行：临时调试最方便
`$ rosparam load config/params.yaml /my_ns`

### 4.3.2 launch 文件里（最常用）
```
<rosparams file="$(find my_pkg)/config/params.yaml"
  command="load" ns="my_ns"/>
```

### 4.3.3 代码里读（带默认值最稳）
```
ros::NodeHandle pnh("~"); /
int exposure;
```

# 5.ros_control
## 5.1 解决什么问题 
- 传统机器人控制：算法（控制器）和硬件（电机、编码器）绑死，换电机要改算法，换算法要改硬件代码
- ros_control思路：把“控制器”和“硬件”拆开，中间立一个统一接口

## 5.2 整体框架
1. 话题 /cmd_vel(controller的输入)
2. Controller Manage
    加载、启动、切换控制器；管理控制器的生命周期
    spawner一行起一个
3. Controller(joint_state/joint_trajectory/effort/diff_drive)
    PID.轨迹.差速（算法部分，现成的直接用）
4. read()/write()   RobotHW接口（唯一要自己写的）
5. RobotHW(hardware_interface)
    把EtherCAT或CAN的收发包成read()/write()
    对上位机是透明的
6. CAN/EtherCAT
7. 下位机--电机

## 5.3 数据是怎么流动的
1. 别人通过话题发`/cmd_vel`(目标速度指令)
2. Controller收到目标，算出每个关节/轮子该给多少力矩（或速度、位置）
3. 通过`write()`把指令打包下发（CAN/EtherCAT帧）
4. 通过`read()`把编辑器读数收上来，作为下一次计算的反馈
5. 这个“读-算-写”的循环以固定频率（如1kHz）反复执行

## 5.4 常用Controller(直接用，不用自己写)

| 控制器 | 用途 |
| --- | --- |
| `joint_state_controller` |把关节状态发布成话题（/joint_states) |
| `joint_position_controller` | 位置控制（到指定角度） |
| `joint_velocity_controller` | 速度控制 |
| `effort_controllers/JointPositionController` | 力矩输出+内置PID的位置控制（云台常用） |
| `diff_drive_controller` | 差速底盘：/cmd_vel --左右轮速度 |
| `joint_trajectory_controller` | 轨迹跟踪（机械臂） |

## 5.5 RobotHW
- 写一个硬件类，实现`read()`和`write()`
- `write()`: 把控制器算出来的指令打包成CAN/EtherCAT帧发下去
- `read()`: 把编译器读数、电流等状态等收上来
- 对上位机的控制算法来说，硬件从此是透明的————它只知道“我能读写这些关节”

## 5.6 好处和队内用法
- **换电机只改hardware_interface,换算法只换controller**
- **仿真和实车共用同一套controller**,Gazebo里用仿真RobotHW，实车用真RobotHW，控制器代码一模一样
- 队内的方案（纯上位机）：视觉和控制逻辑全跑在上位机(NUC)的ROS节点里，下位机（EtherCAT--CAN)只做帧转换、心跳/急停兜底，**没有控制逻辑**
