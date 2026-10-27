# ROS

## 计算图层

**节点（Node）**：ROS1 的基本执行单元。一个机器人系统通常由多个功能单一的节点组成（如一个节点控制激光雷达，一个节点负责路径规划）。节点通常使用 roscpp（C++）编写。

**主节点（Master）**：ROS1 的中央协调器，提供名称注册与查找服务。节点启动后需向 Master 注册，以便其他节点能够找到它。节点间的实际数据传输是直接建立的，不经过 Master。

**话题（Topic）**：基于**发布/订阅**模式的异步通信方式。节点将消息发布到某个话题，其他节点通过订阅该话题来接收数据。发布者与订阅者互不知晓对方的存在，实现了信息的解耦。

**服务（Service）**：基于**请求/响应**模式的同步通信方式，类似于远程过程调用。适用于需要即时反馈的交互场景。

**消息（Message）**：节点间通信的数据结构，由类型化的字段组成（类似 结构体），支持标准数据类型及嵌套。

**参数服务器（Parameter Server）**：一个可通过键值对存储全局配置参数的中央位置，是 Master 的一部分。

![](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-27 09-52-34.png)

## ros_control

使同一套控制算法能不加修改地运行在不同机器人上

### 控制循环

**读取（read）**：从硬件抽象层读取当前关节状态（位置、速度、力矩）。

**更新（update）**：控制器管理器调用所有已加载控制器的 `update` 方法，计算新的控制指令。

**写入（write）**：将计算出的指令写回硬件抽象层，最终发送给电机。

**休眠（sleep）**：等待下一个控制周期。



### 常用controller

**joint_state_controller** 读取关节状态，发布到 `/joint_states`（用于RViz可视化和上层算法）。

**joint_trajectory_controller** 执行关节轨迹（带时间戳的路径点）

**effort_controllers (PID)** 力矩/位置控制，如 `JointPositionController`，接收位置指令，内部通过PID算出力矩输出，适用于需要柔顺或力控的关节。

**diff_drive_controller** 差速底盘控制。订阅 `cmd_vel` 速度指令，转换为左右轮转速并计算里程计。



### RobotHW

**核心工作**：写一个 C++ 类继承 `hardware_interface::RobotHW`。

**实现接口**：必须重写 `read()` 和 `write()` 两个核心函数。

`read()`：从底层（如 EtherCAT/CAN 总线）读取电机编码器数据，填入 ROS 的关节状态接口。

`write()`：从 ROS 的关节指令接口获取数据，打包发送给底层（如 EtherCAT/CAN 总线）驱动电机。

**作用（透明化）**：把复杂的底层总线通信封装起来，对上位机（控制器等）来说，这个机器人就变成了一个“标准模型”，上层完全不需要关心底层是何种总线。



## package.xml(名字 版本 依赖)

```` xml
<?xml version="1.0"?>
<package format="2">
  <!-- 名字  -->
  <name>my_pkg</name> <!-- 必须与文件夹同名 -->
  <version>0.1.0</version>
  <description>我的第一个 ROS 包</description>
  <maintainer email="you@example.com">Your Name</maintainer>
  <license>MIT</license>

  <!-- 构建工具依赖 -->
  <buildtool_depend>catkin</buildtool_depend>

  <!-- 功能依赖声明 -->
  <!-- 编译期需要：头文件、库、消息包 -->
  <build_depend>std_msgs</build_depend> 
  
  <!-- 运行期需要：动态库、消息运行库、launch 调用的包 -->
  <exec_depend>message_runtime</exec_depend> 
  
  <!-- 三合一（最省事）：等于 build + build_export + exec -->
  <depend>roscpp</depend> 
</package>
````



## CMakeLists.txt(编译规则：编什么，链什么)

```` cmake
cmake_minimum_required(VERSION 3.0.2)
project(my_pkg)  # 替换为包名，需与 package.xml 一致

#  找依赖（包名必须和 package.xml 里的依赖对得上）
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  # message_generation # 如果有自定义 msg/srv，取消这行注释
)

#  生成消息（仅当有自定义 .msg / .srv 时才保留这三段）
# add_message_files(FILES MyMsg.msg)
# add_service_files(FILES MySrv.srv)
# generate_messages(DEPENDENCIES std_msgs)

#  配置 catkin（如果有自定义消息，需包含 message_runtime）
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES my_pkg
#  CATKIN_DEPENDS roscpp std_msgs message_runtime
)

#  包含头文件目录
include_directories(
  ${catkin_INCLUDE_DIRS}
)

#  编译节点（有几个 .cpp 就写几条 add_executable）
add_executable(talker_node src/talker.cpp)

# 如果这个节点用到了自定义消息，必须加这一句（防报“找不到头文件”错）
# add_dependencies(talker_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

#  链接库（每个节点都要配一条 target_link_libraries）
target_link_libraries(talker_node
  ${catkin_LIBRARIES}
)
````



## 三大通信

### 话题通信（Topic）:发布/订阅

单向传输，发布者只管发，不管有没有人收；订阅者只管收，不管是谁发的。两者完全解耦。

适用于**持续高频**的数据流。如：激光雷达数据（`/scan`）、相机图像（`/image_raw`）、底盘速度指令（`/cmd_vel`）

**常用排查命令**：`rostopic list`（列表）、`rostopic echo`（打印数据）、`rostopic info`（查发布订阅关系）、`rostopic hz`（测频率）



#### 发布端（Publisher）:talker.cpp

```` cpp
#include <ros/ros.h>
#include <std_msgs/String.h> // 替换为需要的消息类型头文件

int main(int argc, char **argv) {
    // 1. 初始化节点 (必须最先，参数：命令行参数，节点名称)
    ros::init(argc, argv, "talker_node");
    
    // 2. 创建节点句柄
    ros::NodeHandle nh;

    // 3. 创建发布者
    // 参数1: 话题名称 ("chatter")
    // 参数2: 队列长度 (10)，缓冲区大小
    ros::Publisher pub = nh.advertise<std_msgs::String>("chatter", 10);

    // 4. 设置循环频率 (1 Hz，即 1秒发一次)
    ros::Rate rate(1);

    // 5. 主循环
    while (ros::ok()) {
        // 定义并填充消息内容
        std_msgs::String msg;
        msg.data = "hello world";
        
        // 打印日志并发布
        ROS_INFO("Publishing: %s", msg.data.c_str());
        pub.publish(msg);

        // 按照设定的频率休眠
        rate.sleep();
    }
    return 0;
}
````

#### 订阅端（Subscriber）:listener.cpp

```` c++
#include <ros/ros.h>
#include <std_msgs/String.h> // 替换为需要的消息类型头文件

// 回调函数：当收到消息时自动触发
// 注意：不要在这里做耗时的操作（如长时间 sleep），否则会阻塞后续消息
void callback(const std_msgs::String::ConstPtr& msg) {
    // msg->data 获取消息内容
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv) {
    // 1. 初始化节点
    ros::init(argc, argv, "listener_node");
    
    // 2. 创建节点句柄
    ros::NodeHandle nh;

    // 3. 创建订阅者
    // 参数1: 话题名称 ("chatter")，必须与发布端一致
    // 参数2: 队列长度 (10)
    // 参数3: 回调函数名 (callback)
    ros::Subscriber sub = nh.subscribe("chatter", 10, callback);

    // 4. 进入自旋等待 (死循环)
    // 程序会卡在这里，等待消息到来并触发回调函数
    ros::spin(); 
    
    return 0;
}
````



### 服务通信（Service）：请求/应答

一对一（一个服务只能有一个服务端）客户端发请求，服务端处理后返回结果。代码停在原地一直等，直到拿到结果或超时。

适用于低频、需要即时反馈的操作。

**常用命令**

**`osservice list`**：列出当前活跃服务（查服务名对不对）。

**`rosservice type`**：查服务的消息类型（如 `turtlesim/Spawn`）。

**`rossrv show`**：看srv字段定义（里面有哪些参数）。

**`rosservice call`**：手动发起请求测试（联调最快的验证方式）。

**Tips:**

**名字写错会一直等**：如果服务名拼错，客户端会卡死没反应。排查方法：用 `rosservice list` 核对名字。

**高频调用会拖垮节点**：每次调用都要建连、序列化。如果频率大于 10Hz，果断改用 **话题（Topic）**。

**超时要自己加**：`call()` 默认会死等。在比赛或工程中，必须自己包一层超时判断（如 `waitForExistence`），否则一旦卡死，全队遭殃。

eg.

```` cpp
#include <ros/ros.h>
#include <turtlesim/Spawn.h> // 1. 包含对应的服务类型头文件

int main(int argc, char** argv) {
    ros::init(argc, argv, "spawn_client");
    ros::NodeHandle nh;

    // 2. 创建客户端（指定服务名和类型）
    ros::ServiceClient client = nh.serviceClient<turtlesim::Spawn>("/spawn");

    // 3. 实例化服务数据结构
    turtlesim::Spawn srv;
    
    // 4. 填充 Request 参数
    srv.request.x = 2.0; 
    srv.request.y = 3.0; 
    srv.request.name = "turtle2";

    // 5. 发起调用（call 会阻塞，直到返回结果）
    if (client.call(srv)) {
        // 调用成功，打印 Response 结果
        ROS_INFO("new: %s", srv.response.name.c_str());
    } else {
        // 调用失败
        ROS_ERROR("Failed to call service");
    }
    return 0;
}
````



### 参数服务器（Parameter Server）：全局的配置墙

ROS Master 里的一个全局字典（Key-Value），只用来**存静态配置**，绝不用于高频节点间传数据。roscore一关就没了

**三大命名空间（极易混淆）**：

- `/speed`：绝对名，全局找。
- `~speed`：私有名，属于当前节点专属（如 `/节点名/speed`）。
- `speed`：相对名，拼在当前命名空间后。

**常用命令**

`rosparam list`：列出所有参数

`rosparam get / set`：读/改参数

`rosparam dump / load`：导出/导入 YAML 文件

`rosparam delete`：删参数树



```` c++
#include <ros/ros.h>
#include <xmlrpcpp/XmlRpcValue.h> // 如果需要读整棵树(方法3)，需要这个头文件

int main(int argc, char **argv) {
    ros::init(argc, argv, "param_node");
    ros::NodeHandle nh;
    
    // 方法1：读取普通参数（带默认值，最稳妥）
    int exposure;
    // 如果 /cam/exposure 没设置，exposure 默认取 30，不会崩溃
    nh.param<int>("cam/exposure", exposure, 30);
    ROS_INFO("Exposure: %d", exposure);

    // 方法2：读取私有参数（只属于当前节点）
    // 使用 "~" 创建私有句柄
    ros::NodeHandle pnh("~"); 
    double gain = 1.0;
    // 实际读取的是 /param_node/gain (假设节点名叫 param_node)
    pnh.param<double>("gain", gain, 1.0);
    ROS_INFO("Gain: %f", gain);

    // 方法3：读取整棵参数树（如字典/列表）
    XmlRpc::XmlRpcValue tree;
    // 读取名为 my_tree 的整个字典
    if (nh.getParam("my_tree", tree)) {
        ROS_INFO("Successfully loaded tree parameters.");
    } else {
        ROS_WARN("my_tree not found!");
    }

    return 0;
}
````



## 配置文件

### YAML的五条规矩

1. **只用空格缩进，绝不用 Tab**：这是 YAML 最严苛的规矩。必须用空格（通常 2 个空格代表一级），同一层级必须严格对齐。

2. **冒号后面必须有空格**：`key: value` 是对的；`key:value` 会被识别成一个普通字符串。

3. **大小写敏感**：`True` 和 `true` 不一样。ROS 里只认小写 `true` / `false`。

4.  **`#` 是注释**：可以放在行首或行尾，但如果值里本身就带 `#`，必须加引号（如 `"cam # 1"`）。

5. **`---` 分割文档**：一个文件里可以写多个配置块，用 `---` 隔开。



### 三种结构

1.  **标量**：单一的值。

​	支持数字（1.5）、字符串（"sentry"）、布尔值（true）、空值（null）。

2. **列表**：用 `-` 表示（破折号后面有空格）。

​	多行写法：

```yaml、
ports:
  - 8080
  - 8081
```

单行写法：`ids: [1, 2, 3]`

3. **字典：键值对。

多行写法（最常用）：

```yaml
camera:
  width: 640
  height: 480
```

单行写法：`point: {x: 1, y: 2}`



### 三种加载方式

```` yaml 
# ① 命令行加载（临时调试最方便）
rosparam load config/params.yaml /my_ns
# 把文件里的参数批量灌入参数服务器，/my_ns 是可选命名空间。

# ② launch 文件加载（最常用）
# 写在 .launch 文件里，随节点一起启动。
<rosparam file="$(find my_pkg)/config/params.yaml" command="load" ns="my_ns"/>
# $(find)自动找包路径，command="load"执行加载，ns隔离命名空间防冲突。

# ③ 代码中读取（节点获取参数）
# 在 C++ 的 main 函数里，用 NodeHandle 取值。
pnh.param<int>("exposure", exposure, 30);
# 节点启动时去参数服务器里读值。第三个参数 30 是默认值。
# 必须带默认值！防漏配参数导致节点崩溃。
````



## 入门

**启动主节点**

```` bash
roscore
````

**新开一个终端，启动界面**

```` bash
rosrun turtlesim turtlesim_node
````

**再新开一个终端，进容器**

```` bash
rosrun turtlesim turtle_teleop_key
````

小Tips：用鼠标点一下那个显示着 `Reading from keyboard...` 的终端窗口（确保它的光标在闪烁，或者窗口在最前面）。按键盘上的方向键（上下左右）才可以动起来

![ ](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-27 11-29-25.png)

```` bash
# 把背景改成红色 (255)
rosparam set /turtlesim/background_r 255
# 调用清屏服务，让新颜色生效
rosservice call /clear
````

### rosnode 节点排雷神器

1. **谁在跑？**

   ```` bash
   rosnode list
   ````

   这个命令能列出你当前开着的所有节点。能看到 `/turtlesim`（海龟本体）和 `/turtle_teleop_key`（遥控器）。

2. **它在和谁打交道？**

   ```` bash
   rosnode info ...
   ````

   如果敲 `rosnode info /turtlesim`，会看到它订阅了 `/turtle1/cmd_vel`。如果想让海龟动，必须往这个话题里发消息。

3. **它还活着吗？**

   ````bash
   rosnode ping ...
   ````

   有时候节点卡死了，但名字还在列表里。`ping` 一下，能看出它是不是真的在响应。

4. **干掉它！**

   ````bash
   rosnode kill ...
   ````

   相当于远程关掉某个节点。比直接按 `Ctrl+C` 更优雅，特别是对于后台运行的节点。

![](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-27 13-02-50.png)

看全局

````bash
rqt_graph
````

![](/home/zkz/missing-semester/2027/zkz/notes/picture/Screenshot from 2026-10-27 13-00-49.png)



### 常用命令

```` bash
# 运行与节点 
roscore                              # 启动 ROS Master (核心中枢)
rosrun <pkg> <node> <args>           # 运行指定包里的单个节点
roslaunch <pkg> <file.launch>        # 根据 launch 文件一键启动多个节点

# 服务与消息 
rosservice list                      # 列出所有活跃的服务
rosservice type <srv_name>           # 查看该服务的数据类型
rosservice call <srv_name> <args>    # 手动调用服务 (最快验证方式)
rosservice find <srv_type>           # 根据服务类型反查提供该服务的节点
# 消息与服务结构查看
rosmsg show <pkg/Type>               # 查看 .msg 的具体字段结构
rossrv show <pkg/Type>               # 查看 .srv 的具体字段

# 3. 调试与录制
rviz                                 # 启动 3D 可视化工具 (看模型/雷达/坐标系)
rosbag record -a                     # 录制当前系统所有话题数据 (-a 代表 all)
rosbag info <bag_file>               # 查看 bag 文件的时长、大小、包含话题
rosbag play <bag_file>               # 回放 bag 文件 (复现当时的运行状态)
# 注：除了命令行回放，也可以把 bag 拖入 PlotJuggler 软件进行曲线可视化分析。
````



