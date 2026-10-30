# ROS 学习笔记

> ROS（Robot Operating System）不是操作系统，而是一套机器人分布式通信框架。节点之间通过话题、服务等机制交换数据。

## 1. 核心概念

| 概念 | 说明 |
| --- | --- |
| Master | 名字服务与注册中心，`roscore` 会启动它 |
| Node | 独立运行的程序，通常只负责一件事 |
| Topic | 异步发布/订阅通道，支持多对多通信 |
| Message | 话题上传输的有类型数据 |
| Service | 同步的请求—响应调用 |
| Parameter | 存放在参数服务器的全局键值对 |
| Package | ROS 代码的组织单位 |
| Nodelet | 运行在同一进程内的节点，可零拷贝传输 |

| 对比项 | 话题 Topic | 服务 Service |
| --- | --- | --- |
| 通信方式 | 异步发布/订阅 | 同步请求/响应 |
| 方向 | 一对多、多对多 | 客户端到服务器 |
| 适合场景 | 持续数据流：传感器、里程计 | 偶尔调用：计算、查询、触发动作 |

`roscore` 启动 Master、参数服务器和 `/rosout`。使用 `roslaunch` 时通常无需手动启动 `roscore`。

## 2. 工作空间与功能包

```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

新终端需 `source devel/setup.bash`（可写入 `~/.bashrc`）；编译前还需 `source /opt/ros/<发行版>/setup.bash`。

```bash
cd ~/catkin_ws/src
catkin_create_pkg my_pkg roscpp rospy std_msgs
```

```text
my_pkg/
├── CMakeLists.txt      # 编译规则
├── package.xml         # 包名、版本、依赖
├── include/my_pkg/     # 头文件
├── src/                # C++ 源文件
├── launch/             # 启动文件
├── msg/  srv/          # 自定义消息和服务
└── config/             # YAML 参数
```

```bash
cd ~/catkin_ws
catkin_make              # 编译全部
catkin_make --pkg my_pkg # 只编译某个包
```

## 3. 节点与 rosrun

```bash
rosrun <功能包名> <可执行文件名> [参数...]
rosrun turtlesim turtlesim_node
```

- 后面的参数会原样传给程序，成为 `argv`。
- `rosnode list` 查看节点，`rosnode info <节点名>` 查看详情，`rosnode kill <节点名>` 结束节点。

```bash
rosrun my_pkg my_node _speed:=2.0          # 私有参数：/节点名/speed
rosrun my_pkg my_node __name:=my_node_2    # 改节点名
rosrun my_pkg my_node __ns:=robot1         # 改命名空间
```

- `_参数:=值` 是私有参数。
- 全局参数用 `rosparam set` 或 launch 的 `<param>` 设置。
- 话题重映射写成 `旧话题:=新话题`，不要和私有参数混淆。

## 4. 初始化：ros::init

```cpp
#include <ros/ros.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "my_node");
    ros::NodeHandle nh;
    // ...
}
```

- 节点名在同一网络中应当唯一。
- `ros::init` 要在创建 `NodeHandle` 之前调用，且只调用一次。
- `NodeHandle` 负责创建发布者、订阅者、服务等资源，析构时自动注销。

| 启动选项 | 作用 |
| --- | --- |
| `AnonymousName` | 节点名加随机后缀，避免重名 |
| `NoSigintHandler` | 不让 ROS 接管 Ctrl+C |
| `NoRosout` | 不向 `/rosout` 发布日志 |

```cpp
ros::init(argc, argv, "my_node", ros::init_options::AnonymousName);

ros::NodeHandle nh;           // 默认命名空间
ros::NodeHandle pnh("~");     // 私有命名空间 /节点名
ros::NodeHandle nh("robot1"); // 相对命名空间 /robot1
```

## 5. 发布者与订阅者

```cpp
#include <std_msgs/String.h>

ros::Publisher pub = nh.advertise<std_msgs::String>("chatter", 10);
std_msgs::String msg;
msg.data = "hello";
pub.publish(msg);
```

```cpp
Publisher advertise(const std::string& topic,
                    uint32_t queue_size,
                    bool latch = false);
```

| 参数 | 含义 |
| --- | --- |
| `topic` | 话题名 |
| `queue_size` | 发送队列长度，常用 10；太小会丢消息，不建议用 0 |
| `latch` | 保存最后一条消息，默认 `false` |

`latch = true` 时，后连接的订阅者会立即收到最后一条消息，适合地图、静态坐标变换等低频不变数据。`Publisher` 是句柄，`advertise` 通常只调用一次。

```cpp
void chatterCallback(const std_msgs::String::ConstPtr& msg) {
    ROS_INFO("I heard: %s", msg->data.c_str());
}

ros::Subscriber sub = nh.subscribe("chatter", 10, chatterCallback);
```

- 回调参数常用 `const T::ConstPtr&`，避免拷贝。
- 订阅队列 `10` 用于缓存回调处理不过来的消息。

```cpp
ros::spin();      // 阻塞，持续处理回调
ros::spinOnce();  // 只处理一轮，适合放进自己的循环
```

## 6. 消息与话题工具

```bash
rostopic list                     # 列出话题
rostopic echo /chatter            # 打印内容
rostopic hz /chatter              # 查看频率
rostopic info /chatter            # 发布者、订阅者、类型
rostopic pub -r 1 /chatter std_msgs/String "data: hello"
```

自定义 `msg/Person.msg`：

```text
string name
uint8 age
float32 score
```

`package.xml`：

```xml
<build_depend>message_generation</build_depend>
<exec_depend>message_runtime</exec_depend>
```

`CMakeLists.txt`：

```cmake
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs message_generation)

add_message_files(FILES Person.msg)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(CATKIN_DEPENDS message_runtime roscpp rospy std_msgs)
```

编译后生成 `my_pkg/Person.h`，可直接使用：`my_pkg::Person p; p.name = "Tom"; p.age = 20;`

内置类型包括 `bool`、`int8`～`int64`、`uint8`～`uint64`、`float32/64`、`string`、`time`、`duration` 以及数组。若使用 `Header`，字段名通常叫 `header` 且必须放在第一位。

## 7. 参数服务器

参数服务器适合放配置项，不适合放高频变化的数据。

```bash
rosparam list                 # 列出参数
rosparam get /speed           # 读取
rosparam set /speed 2.0       # 设置
rosparam dump params.yaml     # 导出
rosparam load params.yaml     # 导入
```

```cpp
double speed;
nh.param<double>("speed", speed, 1.0);   // 参数名、变量、默认值

if (nh.getParam("speed", speed)) {
    ROS_INFO("speed = %.2f", speed);
}

ros::NodeHandle pnh("~");
pnh.param<double>("speed", speed, 1.0);  // 实际读取 /节点名/speed
```

launch 中设置：

```xml
<param name="speed" value="2.0"/>
<rosparam file="$(find my_pkg)/config/params.yaml" command="load"/>
```

## 8. 时间 Time 与 Duration

- `ros::Time`：绝对时刻。
- `ros::Duration`：时间长度。

```cpp
ros::Time now = ros::Time::now();
double secs = now.toSec();      // 转成秒
uint32_t s = now.sec;           // 整数秒
uint32_t ns = now.nsec;         // 纳秒部分
ROS_INFO("当前时刻: %.2f", now.toSec());

ros::Duration du(10.0);
du.sleep();                     // 休眠 10 秒

```

固定频率循环：

```cpp
ros::Rate rate(20);             // 20 Hz
while (ros::ok()) {
    // 干活
    rate.sleep();               // 自动补足剩余时间
}
```

定时器由 `ros::spin()` 或 `spinOnce()` 驱动，不需要自己写循环：

```cpp
void timerCallback(const ros::TimerEvent&) { ROS_INFO("timer fired"); }

ros::Timer timer = nh.createTimer(ros::Duration(0.1), timerCallback);
// 第三个参数传 true 表示只执行一次
```

`/use_sim_time` 为 `true` 时，`ros::Time::now()` 使用仿真时钟；`ros::WallTime` / `ros::WallDuration` 始终使用真实时间，不受仿真影响。常用写法：`ros::WallDuration(1.0).sleep()`、`ros::Time::sleepUntil(...)`。

## 9. 服务通信 Service

### 9.1 定义 srv

`AddInts.srv`，请求和响应用 `---` 分隔：

```text
int32 num1
int32 num2
---
int32 sum
```

### 9.2 配置

`package.xml`：

```xml
<build_depend>message_generation</build_depend>
<exec_depend>message_runtime</exec_depend>
```

注意旧教程的 `run_depend` 已废弃，现在用 `exec_depend`。

`CMakeLists.txt`：

```cmake
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs message_generation)

add_service_files(FILES AddInts.srv)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(CATKIN_DEPENDS message_runtime roscpp rospy std_msgs)
```

编译后生成 `AddInts.h`，包含 `AddInts::Request` 和 `AddInts::Response`。若提示找不到头文件，给目标加上 `add_dependencies(my_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})`。

### 9.3 服务端与客户端

```cpp
#include <my_pkg/AddInts.h>

bool add(my_pkg::AddInts::Request &req,
         my_pkg::AddInts::Response &res) {
    res.sum = req.num1 + req.num2;
    return true;
}

ros::ServiceServer server = nh.advertiseService("add_ints", add);
```

```cpp
ros::ServiceClient client = nh.serviceClient<my_pkg::AddInts>("add_ints");
ros::service::waitForService("add_ints");   // 等待服务端启动

my_pkg::AddInts srv;
srv.request.num1 = 3;
srv.request.num2 = 5;

if (client.call(srv)) {
    ROS_INFO("sum = %d", srv.response.sum);
} else {
    ROS_ERROR("Failed to call service");
}
```

`call()` 同步等待响应，不要在回调里长时间阻塞；必要时另开线程或改用话题。

```bash
rosservice list
rosservice type /add_ints
rosservice info /add_ints
rosservice call /add_ints "{num1: 3, num2: 5}"
```

## 10. launch 文件

```xml
<launch>
  <arg name="rate" default="20"/>
  <param name="use_sim_time" value="false"/>

  <node pkg="my_pkg" type="my_node" name="my_node" output="screen">
    <param name="speed" value="2.0"/>
    <remap from="chatter" to="robot/chatter"/>
  </node>

  <node pkg="turtlesim" type="turtlesim_node" name="sim"/>

  <group ns="robot1">
    <node pkg="my_pkg" type="sensor_node" name="sensor"/>
  </group>

  <include file="$(find other_pkg)/launch/other.launch"/>
</launch>
```

```bash
roslaunch my_pkg start.launch
roslaunch my_pkg start.launch rate:=10
```

| 标签 | 作用 |
| --- | --- |
| `<node>` | 启动节点 |
| `<param>` / `<rosparam>` | 设置参数 / 加载 YAML |
| `<remap>` | 重映射话题名 |
| `<arg>` | launch 内部变量，可从命令行传入 |
| `<group ns="...">` | 给一组节点加命名空间 |
| `<include>` | 引入另一个 launch 文件 |

- `pkg` 是包名，`type` 是可执行文件名，`name` 是运行时节点名。
- `output="screen"` 让日志直接打印到终端。
- 没有 Master 时，`roslaunch` 会自动启动一个。

## 11. Nodelet

普通节点是独立进程，大数据需要拷贝。Nodelet 把多个节点加载到同一进程，通过共享指针实现零拷贝，延迟更低。

```cpp
#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>

class MyNodelet : public nodelet::Nodelet {
public:
    void onInit() override {
        ros::NodeHandle &nh = getNodeHandle();
        ros::NodeHandle &pnh = getPrivateNodeHandle();
        // 初始化发布者、订阅者等
    }
};

PLUGINLIB_EXPORT_CLASS(MyNodelet, nodelet::Nodelet)
```

- 在 `onInit()` 里初始化，而不是构造函数。
- 用 `PLUGINLIB_EXPORT_CLASS` 导出插件。
- 不要在 `onInit()` 中写死循环或长时间阻塞操作，否则会卡住整个 manager。
- Nodelet 不需要自己调用 `ros::spin()`，回调由 manager 统一处理。

`nodelet_plugins.xml`：

```xml
<library path="lib/libmy_nodelet">
  <class name="my_pkg/MyNodelet"
         type="MyNodelet"
         base_class_type="nodelet::Nodelet">
    <description>My first nodelet.</description>
  </class>
</library>
```

`package.xml`：

```xml
<export>
  <nodelet plugin="${prefix}/nodelet_plugins.xml"/>
</export>
```

`CMakeLists.txt`：

```cmake
add_library(my_nodelet src/my_nodelet.cpp)
target_link_libraries(my_nodelet ${catkin_LIBRARIES})
```

启动：

```xml
<node pkg="nodelet" type="nodelet" name="manager" args="manager"/>
<node pkg="nodelet" type="nodelet" name="my_nodelet"
      args="load my_pkg/MyNodelet manager"/>
```

```bash
nodelet list          # 已加载的 nodelet
nodelet info manager  # manager 状态
```

一个 nodelet 崩溃可能影响同 manager 的其他 nodelet，调试时可先用普通节点。

## 12. 调试工具

| 工具 | 作用 |
| --- | --- |
| `rqt_graph` | 节点与话题连接图 |
| `rqt_console` | 查看、过滤日志 |
| `rqt_plot` | 实时绘制数值曲线 |
| `rviz` | 三维可视化传感器、TF、地图等 |
| `rosbag` | 录制、回放话题数据 |
| `rosnode` / `rostopic` | 调试节点 / 话题 |
| `rosservice` / `rosparam` | 调试服务 / 参数 |
| `rqt_reconfigure`（图形界面，推荐）`rosrun   rqt_reconfigure rqt_reconfigure` 开关用 dynamic_reconfigure 提供的一个布尔型动态参数 |

```bash
rosbag record -a                 # 录制所有话题
rosbag record /chatter /scan     # 只录制指定话题
rosbag info demo.bag
rosbag play demo.bag
rosbag play demo.bag --rate 2.0  # 两倍速
```

回放时配合 `--clock` 和 `/use_sim_time`，节点会读取录制时的仿真时间。

## 13. 速查表

| 内容 | 命令 / 写法 |
| --- | --- |
| 启动 Master | `roscore` |
| 启动节点 | `rosrun <包> <可执行文件>` |
| 启动 launch | `roslaunch <包> <文件.launch>` |
| 编译 | `catkin_make` |
| 创建功能包 | `catkin_create_pkg <包名> <依赖...>` |
| 查节点 | `rosnode list` / `rosnode info` |
| 查话题 | `rostopic list` / `echo` / `hz` |
| 查服务 | `rosservice list` / `call` |
| 查参数 | `rosparam list` / `get` / `set` |
| 录包 | `rosbag record` / `play` |
| 初始化 | `ros::init(argc, argv, "name")` |
| 发布者 | `nh.advertise<T>("topic", 10)` |
| 订阅者 | `nh.subscribe("topic", 10, callback)` |
| 发布消息 | `pub.publish(msg)` |
| 处理回调 | `ros::spin()` / `spinOnce()` |
| 私有参数 | `pnh.param<T>("name", var, default)` |
| 当前时刻 | `ros::Time::now()` |
| 时间长度 | `ros::Duration(10.0)` |
| 固定频率 | `ros::Rate rate(20); rate.sleep();` |
| 定时器 | `nh.createTimer(ros::Duration(0.1), cb)` |
| 服务端 | `nh.advertiseService("name", cb)` |
| 客户端 | `nh.serviceClient<T>("name")` |
