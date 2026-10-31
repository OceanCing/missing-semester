# ROS概念
- ROS是适用机器人的开源**元操作**系统
- ROS是进程（也称为Nodes）的分布式框架。 因为这些进程甚至还可分布于不同主机，不同主机协同工作，从而分散计算压力

# ROS文件架构
WorkSpace --- 自定义的工作空间 （可由catkin_make创建，但有时会有bug）

    |--- build:编译空间，用于存放CMake和catkin的缓存信息、配置信息和其他中间文件。

    |--- devel:开发空间，用于存放编译后生成的目标文件，包括头文件、动态&静态链接库、可执行文件等。

    |--- src: 源码

        |-- package：功能包(ROS基本单元)包含多个节点、库与配置文件，包名所有字母小写，只能由字母、数字与下划线组成 （可用ros_create_pkg创建）

            |-- CMakeLists.txt 配置编译规则，比如源文件、依赖项、目标文件

            |-- package.xml 包信息，比如:包名、版本、作者、依赖项...(以前版本是 manifest.xml)

            |-- scripts 存储python文件

            |-- src 存储C++源文件

            |-- include 头文件

            |-- msg 消息通信格式文件

            |-- srv 服务通信格式文件

            |-- action 动作格式文件

            |-- launch 可一次性运行多个节点 

            |-- config 配置信息

    |-- CMakeLists.txt: 编译的基本配置

# 功能包的编写
## 创建
可以用ros_create_pkg name <依赖> ...
## 写节点代码
## 编辑 ros 包下的 Cmakelist.txt文件
```
add_executable(步骤3的源文件名
  src/步骤3的源文件名.cpp
)
target_link_libraries(步骤3的源文件名
  ${catkin_LIBRARIES}
)
```
# launch文件
launch 是 ROS 的启动脚本，后缀  .launch ，XML格式，用来一次性启动多个节点、加载参数、设置命名空间，不用一个个敲 rosrun
语法以标签为主，可以单标签为一个语块，如<node ……/>，也可以<标签>……<标签/>为一个语块，都要添加/作为闭合标志
## 常见标签
### <launch>开头
### <node>
```xml
<node name="scan_processor" pkg="smb_highlevel_controller" type="scan_processor" output="screen"/>
```
pkg ：功能包名字
​type ：可执行文件名字（CMake里add_executable的名字）
​name ：节点运行时名字，可以覆盖代码里的节点名
output="screen" ：把日志打印到终端（不然看不到ROS_INFO）

###  <rosparam> 
加载yaml参数文件
```xml
<rosparam file="$(find smb_highlevel_controller)/config/config.yaml" command="load"/>
```
/$(find 包名)  自动找到这个包的路径，不用写绝对路径

### <include>
导入别的launch文件
```xml
<include file="$(find smb_gazebo)/launch/smb_gazebo.launch">
    <arg name="xxx" value="1"/>
</include>
```
 用来嵌套启动别的launch，比如先启动gazebo仿真，再启动控制器
 传参数用  <arg> ，被包含的launch必须声明这个arg，不然报unused args

# 动态参数
## yaml文件
YAML是ROS用来存放参数的配置文件，专门给参数服务器（Parameter Server）用，放传感器、控制器、算法的各种参数，不会写死在代码里。
可以使参数实时动态发生变化，不用重启
一般放在config/文件夹里面
＃＃＃ 基础语法
冒号后面必须带空格
​用空格缩进，不能用Tab
/#  是注释
```yaml
# config.yaml 示例
laser_topic: /scan
min_distance: 0.1
max_distance: 5.0
debug: true
pid:
  kp: 0.8
  ki: 0.01
  kd: 0.05
```

### 实现代码
编写yaml文件
代码读取参数
```C++
// 读取参数，第一个是yaml里的键名，第二个是变量
nh.getParam("laser_topic", laser_topic);
```
加载参数
```xml
<launch>
  <!-- 加载yaml，参数放到全局 -->
  <rosparam file="$(find smb_highlevel_controller)/config/smb_param.yaml" command="load"/>

  <node name="scan_processor" pkg="smb_highlevel_controller" type="scan_processor" output="screen"/>
</launch>

<launch>
  <node name="scan_processor" pkg="smb_highlevel_controller" type="scan_processor" output="screen">
    <!-- 私有参数，名字前面带 /scan_processor/ -->
    <rosparam file="$(find smb_highlevel_controller)/config/smb_param.yaml" command="load"/>
  </node>
</launch>
```

### 命令行调试
```
# 查看所有参数
rosparam list
# 看某个参数的值
rosparam get /scan_processor/pid/kp
# 把当前所有参数导出成yaml备份
rosparam dump backup.yaml
# 手动加载yaml
rosparam load smb_param.yaml
```
### 常见坑
冒号后没空格如```kp:0.5```


# ROS的通信
ROS 中的基本通信机制主要有如下三种实现策略:
  - 话题通信(发布订阅模式) --- 发布话题，订阅话题
  - 服务通信(请求响应模式) --- 发布服务，使用服务
  - 参数服务器(参数共享模式) --- 读取参数，动态调整，一般用于业务，测试期间可以用rqt

## 话题通信
1. 理论模型
  - ROS Master (管理者)
  - Talker (发布者)
  - Listener (订阅者)
  ROS Master 负责保管 Talker 和 Listener 注册的信息，并匹配话题相同的 Talker 与 Listener，帮助 Talker 与 Listener 建立连接，连接建立后，Talker 可以发布消息，且发布的消息会被 Listener 订阅。
2. 实现操作
  1. talker向ros master进行注册，其中包含发布的话题名称
  2. lister也向ros master进行注册，其中包含订阅话题的名称
  3. rosmaster进行两者的匹配
  4. 两者互相确立请求，建立连接
3. 实现代码
  - 发布方
  ``` C++
  //初始化节点和名称
  ros::init(argc,argv,"talker");
  //实例化 ROS 句柄
  ros::NodeHandle nh;//该类封装了 ROS 中的一些常用功能

  //实例化 发布者 对象
  //泛型: 发布的消息类型
  //参数1: 要发布到的话题
  //参数2: 队列中最大保存的消息数，超出此阀值时，先进的先销毁(时间早的先销毁)
  ros::Publisher pub = nh.advertise<std_msgs::String>("chatter",10);

  //组织被发布的数据，并编写逻辑发布数据
  --code--

  //发布
  pub.publish(msg);
  ros::spin();
  ```

  - 订阅方
  ```C++
  void doMsg(const <信息所处的空间>::ConstPtr& msg_p){
    ...
  }
  int main(int argc, char  *argv[])
  {
    //初始化 ROS 节点:命名(唯一)
    ros::init(argc,argv,"listener");
    //实例化 ROS 句柄
    ros::NodeHandle nh;

    //4.实例化 订阅者 对象
    ros::Subscriber sub = nh.subscribe<std_msgs::String>("chatter",10,doMsg);
                                                                        ^
                                                                        |
                                                              //处理订阅的消息(回调函数)

    //设置循环调用回调函数
    ros::spin();//循环读取接收的数据，并调用回调函数处理

    return 0;
  }
  ```

### 自定义信息
- 有些时候普通的数据类型并不能满足我们发布的需求，这时候我们可以考虑自定义信息
msgs只是简单的文本文件，每行具有字段类型和字段名称，可以使用的字段类型有：
  - int8, int16, int32, int64 (或者无符号类型: uint*)
  - float32, float64
  - string
  - time, duration
  - other msg files
  - variable-length array[] and fixed-length array[C]
1. 定义msg信息
在功能包中创建msg文件夹，创建.msg来创建自定义信息
```
string name
uint16 agee
```
2. 改配置文件
  1. package.xml
  ``` xml
  <build_depend>message_generation</build_depend>
  <exec_depend>message_runtime</exec_depend>
  ```
message/_generation：编译阶段用，将.msg生成C++/python代码
message/_runtime：程序运行阶段用

  2. CmakeList.txt
  ``` CMake
  find_package(
	message_generation
  )

  add_message_files(
    FILES
    .msg
  )

  generate_messages(
    DEPENDENCIES
    std_msgs		#声明依赖std_msgs
  )

  #执行时依赖
  catkin_package(
    CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
  )
  ```
3. 编译
编译后的中间文件查看:
C++ 需要调用的中间文件(.../工作空间/devel/include/包名/xxx.h),后续在这里调用

## 服务通信
1. 理论模型
ROS Master 负责保管 Server 和 Client 注册的信息，并匹配话题相同的 Server 与 Client ，帮助 Server 与 Client 建立连接，连接建立后，Client 发送请求信息，Server 返回响应信息。
  - ROS master(管理者)
  - Server(服务端)
  - Client(客户端)
2. 实现操作
  0. Server注册
  Server 启动后，会通过RPC在 ROS Master 中注册自身信息，其中包含提供的服务的名称。ROS Master 会将节点的注册信息加入到注册表中。
  1. Client注册
  Client 启动后，也会通过RPC在 ROS Master 中注册自身信息，包含需要请求的服务的名称。ROS Master 会将节点的注册信息加入到注册表中。
  2. ROS Master实现信息匹配
  ROS Master 会根据注册表中的信息匹配Server和 Client，并通过 RPC 向 Client 发送 Server 的 TCP 地址信息。
  3. Client发送请求
  Client 根据步骤2 响应的信息，使用 TCP 与 Server 建立网络连接，并发送请求数据。
  4. Server发送响应
### 服务通信自定义srv
- 流程:
srv 文件内的可用数据类型与 msg 文件一致，且定义 srv 实现流程与自定义 msg 实现流程类似:
    按照固定格式创建srv文件
    编辑配置文件
    编译生成中间文件
1. 定义srv文件
服务通信中，数据分成两部分，请求与响应，在 srv 文件中请求和响应使用---分割
```xml
# 客户端请求时发送的两个数字
int32 num1
int32 num2
---
# 服务器响应发送的数据
int32 sum
```
2. 编辑配置文件
package.xml中添加编译依赖与执行依赖
```
  <build_depend>message_generation</build_depend>
  <exec_depend>message_runtime</exec_depend>
```
CMakeLists.txt编辑 srv 相关配置
```
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)
# 需要加入 message_generation,必须有 std_msgs

add_service_files(
  FILES
  AddInts.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)
```
3. 编译
编译后的中间文件查看:
C++ 需要调用的中间文件(.../工作空间/devel/include/包名/xxx.h)
后续调用相关 srv 时，是从这些中间文件调用的

#### 代码调用
0. vscode配置
```
{
    "configurations": [
        {
            "browse": {
                "databaseFilename": "",
                "limitSymbolsToIncludedHeaders": true
            },
            "includePath": [
                "/opt/ros/noetic/include/**",
                "/usr/include/**",
                "/xxx/yyy工作空间/devel/include/**" //配置 head 文件的路径 
            ],
            "name": "ROS",
            "intelliSenseMode": "gcc-x64",
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "c11",
            "cppStandard": "c++17"
        }
    ],
    "version": 4
}
```
1. 服务端
```c++
// bool 返回值由于标志是否处理成功
bool doReq(demo03_server_client::AddInts::Request& req,
          demo03_server_client::AddInts::Response& resp){
    int num1 = req.num1;
    //逻辑处理
    return true;
}

...
    // 3.创建 ROS 句柄
    ros::NodeHandle nh;
    // 4.创建 服务 对象
    ros::ServiceServer server = nh.advertiseService("AddInts",doReq);
...
```
2. 客户端
```c++
void reconfigCallBack(smb_highlevel_controller::WarningConfig &config, uint32_t level){
    warning_enabled = config.warning_enabled;
}

...
dynamic_reconfigure::Server<smb_highlevel_controller::WarningConfig> server;
    server.setCallback(boost::bind(&reconfigCallBack,_1,_2));
...
```

# 该笔记记录做smb小车仿真遇到的问题和解决

## 1.Gazebo GUI 图形界面崩溃
简单来说，docker 容器默认**没有 X11 图形转发**，Gazebo 的 GUI（gzclient）要渲染窗口，找不到显示器，直接崩溃。已经运行的容器无法添加，只能重新创建容器并配置 X11 转发
```
docker run -it \
  --name ros-noetic-x11 \
  --env DISPLAY=$DISPLAY \
  --env QT_X11_NO_MITSHM=1 \
  --volume /tmp/.X11-unix:/tmp/.X11-unix \
  --volume ~/dockerFileShare:/root/download \
  --network host \
  osrf/ros:noetic-desktop-full
```

当需要用x11时，在终端机先放行 X 访问
```
xhost +local:root
```

用完记得回收，也可以不用
```
xhost -local:root
```

## 2.Gazebo仿真为什么可以和ros通信
靠的是**gazebo_ros**包---他编译时把ros的库链接进gazebo，gazebo本身变成一个**ROS节点**（名为gazebo），所以可以进行通信

## 3.需要什么依赖包
```
geometry_msgs
sensor_msgs
nav_msgs
```
msgs意味着信息

## 3. sensor_msgs/LaserScan.msg
数组代表激光束，从左到右

## 4.无效测量
`sensor_msgs/LaserScan` 里的规则：

- `inf`（无穷大）：激光打出去，没有碰到障碍物，超出雷达最大测距；
- `nan`（Not a Number）：传感器出错、读数非法；
- `value < msg->range_min`：离雷达太近，超出雷达最小测量下限；
- `value > msg->range_max`：超过雷达最大量程。

# 5.无法找到包，无法启用环境
```
cd ~/catkin_smb
rm -rf build devel
source /opt/ros/noetic/setup.bash
catkin build
source devel/setup.bash
```
## 6.没有scan
缺少 `pointcloud_to_laserscan` 这个包

## 7.ros::spin()
只是循环回调函数，main只执行一次

##8. ros::Subscriber sub1 = nh.subscribe<sensor_msgs::LaserScan>("/scan",10,boost::bind(scanCallback,_1,boost::ref(pub)));
用bind必须表明类型

## 9.launch文件，<node>包裹<rosparam>
这使参数变为这个节点的私有变量，引用要加上这个节点的名字name/

## 10.有关launch语法
/是闭合，类似于{} 

## 11.关于rviz
rviz是ros的可视化工具，能将话题的信息可视化，可以理解为传感器的视野

## 12.一旦 `source devel/setup.bash`，PATH 被污染，ros 基础命令直接消失
老版本bug，改用 catkin build

