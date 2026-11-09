# 第三次培训课笔记（ros noetic）
先学好ros里的c++再来看,

## source
个人理解来说，source 后面会跟一个脚本，source <脚本> 这个指令就会把这个脚本在当前终端里隐形地执行了，然后脚本里设置的东西（比如`export`的环境变量）就被留在了当前终端里，后面就能用

所以 `source /opt/ros/noetic/setup.bash` 的作用是：把 ROS 的安装路径"注入"到你当前这个终端的环境里，之后 roscore、rosrun 才认得

## ROS Master
- Master到底管什么：
    1. 名字注册：谁发布了什么话题、谁提供了什么服务
    2. 名字查询：按名字把双方介绍给对方
    3. 参数服务器：全局参数的存放地（后面的YAML就住这）（**参数服务器里面本来没有参数 只有我写了 `launch` 文件把 `YAML` 文件存进去才会有参数**）
    4. 状态监控：谁还活着、谁掉线了
- 注意：数据根本不经过Master
- 过程：
    1. 注册：节点A（发布者）和节点B（订阅者）启动时，都要去ROS Master那里报个到
    2. 查询地址：告诉节点A和节点B对方的IP地址在哪里
    3. 数据直连TCP：一旦A和B相互认识，他们就直接建立一条TCP网络连接。之后 A 发送的所有数据，都是直接传给 B 的，完全不经过 Master
- roscore=启动Master+rosout(日志)+参数服务器 ，一条命令全起

## 先理解ROS的几个核心概念

### 节点 Node

一个**可执行程序**运行起来后的形态，干一件小事（如读摄像头、控制电机））（注意c++与python的情况不太一样，python的作了解即可，后续核心节点统一用C++写）
- Python:`.py`本身直接就是可执行程序，不需要编译，没有编译产物，运行起来就直接是一个node
- c++: `,cpp`需要经过编译，在devel/里生成可执行程序才是node
- 也就是说Python 的 node 在包里（`包/src/` 里那个.py文件）；而C++的 node 在`devel`里（`devel/lib/包名/`下的那个二进制程序）

### 话题 Topic
- 节点之间单向广播消息的通道（发布/订阅）
- 话题的底层：连接是怎么建立的（上述有）
    1. 发布者启动 → 向 ROS Master 注册："我发布 `/chatter`，类型 `std_msgs/String`，我的地址是 X"
    2. 订阅者启动 → 向 ROS Master 注册："我订阅 `/chatter`，类型 `std_msgs/String`，我的地址是 Y"
    3. Master 一看：有人发有人收，类型匹配 → 把发布者地址告诉订阅者
    4. 订阅者直接连发布者（TCPROS），之后消息 P2P （点对点）直传，不再经过 Master
    - Master只负责牵线，不转发数据
    - 消息是单向的：发布者 → 订阅者
- 入门阶段知道 **话题=发布者+订阅者+中间那条通道** 就行
- 机器人的速度、轮子角速度啥的这些就是话题上流动的实时数据消息
### 消息 Message
话题上传输的数据（如 std_msgs/String）
1. 消息是**有固定格式**的，结构定义在 `.msg` 文件里
    - `.msg`文件：`消息.msg`文件全都放在包目录的`msg/`目录里（`包/msg/`）
    - `消息名.msg`与`包/消息名`是指同一个东西，只是ros用的是后者作为消息的称呼（`包名/消息名`）（比如`my_pkg/String`）
    - .msg只是声明结构，不是代码

2. 消息是可以嵌套的：消息里套消息


### 服务 Service
- 节点间请求-应答式通信（同步，一问一答）
- 服务的三个要素：
    1. **服务名**：字符串，标识一个服务 （例子：/`add_two_ints`、`/spawn`）
    2. **服务类型**：`.srv` 文件定义，分请求和应答两部分 (例子：`rospy_tutorials/AddTwoInts`)
    3. **服务端/客户端**：提供服务的一方 / 请求服务的一方 （例子：计算器节点 / 调用者）（服务端和客户端就是写在`包/src`下的源代码，只是运行的时候就变成了节点）
- `.srv`文件：用`---`把请求和应答分开：
```test
int64 a
int64 b
---
int64 sum
```
- `---` 上面：请求的字段（客户端发什么）

- `---` 下面：应答的字段（服务端返回什么）
---
- 话题与服务的区别：要持续的数据传输的（或一次性但高频调用）用话题（比如：激光雷达持续发数据、底盘持续收速度指令），一次性的数据传输用服务（比如：触发一次抓取并等结果）
- 补充：长的、要进度/可取消的一次性操作用 Action（比如：让机器人移动到 10 米外的目标点）

### 参数 Parameter
- 存在参数服务器上的配置值（键值对），节点启动时或运行中读取，用来配置行为，改参数不用改代码、不用重新编译

- 参数服务器上存什么：就是一堆**键值对**，键是字符串路径，值是各种类型：
```yaml
/robot/max_speed: 0.5          # 浮点数
/robot/name: "my_robot"        # 字符串
/lidar/port: "/dev/ttyUSB0"    # 字符串
/camera/enabled: true          # 布尔
/navigation/obstacle_distance: 0.3
```

**值的类型**：int、float、bool、string、列表、字典（嵌套）

- 比如：
    - `max_speed = rospy.get_param('/robot/max_linear_speed', 0.5)` 这就是一个参数，限制最大速度
    - `wheel_radius = rospy.get_param('/base/wheel_radius', 0.05)` 轮子半径的参数
    - 典型场景：阈值、相机内参、PID参数、串口波特率

#### YAML文件
YAML 是 ROS 里**写参数配置**的标准格式，是一种"人类可读"的数据格式，用缩进表示层级，用来存配置/数据。 文件名后缀 `.yaml`（或 `.yml`）(YAML文件放`包/comfig/`里)
```yaml
robot:
  max_speed: 0.5
  name: my_robot
```
- 基本语法：
    1. 键值对：`键: 值` （冒号前面的叫 **键**，冒号后面的叫 **值**）（**注意冒号后面要有个空格**）
    ```yaml
    max_speed: 0.5
    name: my_robot
    enabled: true
    ```

    2. 层级用缩进（空格，不能用 Tab）
    ```yaml
    robot:
      max_speed: 0.5
      name: my_robot
    lidar:
      port: /dev/ttyUSB0
    ```
    3. 列表：用 `-`
    ```yaml
    fruits:
      - apple
      - banana
      - orange
    ```
    或行内写法：
    ```yaml
    fruits: [apple, banana, orange]
    ```
    4. 注释：`#`

### launch 文件
一次性启动 多个节点+加载参数 的**启动脚本**，避免了一个一个rosrun,而是写一个launch文件，`roslaunch`一跑，全跑起来

还可以把 `yaml` 参数文件存进参数服务器

- launch文件后缀是`.launch`，放在`包/launch/`里，内容用的是xml语法，用`roslaunch`运行（此外`roslaunch`会检测到没有`roscore`时自动生成一个）

- launch文件长啥样呢
```xml
<launch>
    <node pkg="my_first_pkg" type="talker" name="talker" output="screen" />
    <node pkg="my_first_pkg" type="listener" name="listener" output="screen" />
</launch>
```
运行：
```bash
roslaunch my_first_pkg demo.launch 
# roslaunch <包名> <launch文件名>
```
这样就启动了talker+listener两个节点，还自动起了roscore
- `<launch>`标签：所有launch文件的根标签
    - `deprecated` = “弃用说明”
    ```xml
    <launch deprecated="此文件已经过时，不建议使用">
    ```
    - 执行 `roslaunch` 的时候就会弹出warnning

- `<node>`标签：启动节点
    ```xml
    <node pkg="包名" type="可执行文件名" name="节点名" output="screen" />
    ```
    - `type` :可执行文件名 （文件名，磁盘上的可执行文件）
    - `name` :运行时的节点名 （运行时的节点名（ros里的标识））
    - `output=screen | log` :日志输出到哪：screen（终端）或 log（文件）（日志输出就是程序运行时打印的**运行信息**，用来告诉你程序在干什么、有没有出错）
    - `machine=“机器名”` :在指定机器上启动节点
    - `args=“xxx xxx xxx”` :将参数传递给节点 (例子：`args="--debug"`)
    - `respawn="ture|false"` :节点挂了是否自动重启这个节点	（例子：`respawn="true"`）
    - `respawn_delay="N"` :如果respawn 为 true ，那么延迟N秒后重启节点
    - `required` :节点挂了是否整个 launch 退出 （例子：`required="true"`）
    - `ns="xxx"` :设置节点的命名空间（在/name前面加个namespace /xxx ,变成`/xxx/name`）(name是上面的name属性)（可避免重名问题）

- `<param>` 标签和 `<rosparam>`标签：加载参数
    - `<param>`：单个参数
    ```xml
    <param name="robot/max_speed" value="0.5" type="double" />
    <param name="robot/name" value="my_robot" type="string" />
    <param name="camera/enabled" value="true" type="bool" />
    ```
    加载后参数服务器上有 `/robot/max_speed = 0.5`

    两种格式：
    ```xml
    <!-- 格式1：launch下，node外 -->
    <param name="param_A" type="int" value="100" />
    <node pkg="turtlesim" type="turtlesim_node" name="my_turtle" output="screen">
    <!-- 格式2：node下 -->
        <param name="param_B" type="double" value="3.14" />
    </node>
    ```
    区别：格式一的键是：`/param_A` ； 格式二的键是 `/my_turtle/param_B`

    - `<rosparam>`：从 YAML 文件批量加载
    ```xml
    <rosparam file="$(find my_first_pkg)/config/params.yaml" command="load" />
    ```
    这就把整个 YAML 文件（params.yaml）加载进参数服务器

    **属性：**

    1. `file="$(find 包名)/xxx/xxx.yaml"`: YAML文件的路径
    2. `command="load | dump | delete"` :加载、导出或删除参数 （导出参数服务器的参数就用dump，导出路径看file）
    3. `param="参数名称"` 
    4. `ns="xxx"` :命名空间（可选）
    
    与 `<param>` 同理，`<rosparam>` 也有两种格式，与上相同。两种格式的区别就是键的命名空间不同

    注意：`<rosparam>`的启动优先级很高，roslaunch的时候会先启动里面的<rosparam> ，再启动其他节点比如<node> , <param>


- `<remap>`标签：话题重映射
    - **作用**：把节点代码里写的话题名，运行时换成另一个名字，不用改代码
    ```xml
    <node pkg="my_pkg" type="listener" name="listener">
        <!-- 从连接chatter 变成连接my_topic话题 -->
        <remap from="/chatter" to="/my_topic" />
    </node>
    ```
    - listener 代码里写的是订阅 /chatter，但运行时实际订阅 /my_topic

    - 有什么用呢：
        - 同一份代码，不同实例接不同话题（多机器人）
        - 适配别人的节点（它发 /scan，你的代码用 /lidar，重映射一下）
        - `<remap>` 也可以放在 `<launch>` 顶层，对所有节点生效：
        ```xml
        <launch>
            <remap from="/chatter" to="/my_topic" />
            <node ... />
            ...
        </launch>
        ```

- `<include>`标签：包含其他 launch 文件
    - **作用**：把另一个 launch 文件的内容"合并"进来，方便拆分组织，可模块化 ; 同时可避免代码重复
    - 应用场景：大项目可拆成多个launch，一个主launch可以由多个模块launch组合，可维护性好
    - 属性：
        - `file="$(find 包名)/xxx/xxx.launch"` :要包含的文件路径
        - `ns="xxx"`（可选） ：在指定命名空间导入文件

- `<group>`标签：可让节点归属某个命名空间，属性是 `ns="xxx"`（可选）
```xml
<launch>
    <group ns="first">
        <include file="$(find launch01_basic)/launch/start_turtle.launch" />
    </group>

    <group ns="second">
        <include file="$(find launch01_basic)/launch/start_turtle.launch" />
    </group>
</launch>
```

- `<arg>`标签：动态传参
属性：
    - `name="参数名称"`
    - `default="默认值"` (可选)
```xml
<launch>
    <arg name="car_length" default="0.5"/>
    <param name="A" value="$(arg car_length)" />
    <param name="B" value="$(arg car_length)" />
    <param name="C" value="$(arg car_length)" />
</launch>
```
这样只需修改arg里的值就能一键修改所有应用了arg的param的值

此外，还可用指令来修改： `roslaunch 包名 含有arg的.launch文件 arg里的name:=要改的值` ， 比如 `roslaunch roslaunch01_basic arg.launch car_length:=0.6` 这样就把参数改成了0.6



### 工作空间 Workspace
你放自己代码的目录，如 ~/catkin_ws

### 功能包 Package
- 工作空间里的最小组织单位，一个包=一组节点/消息/配置

- ros包最常用的文件结构
```text
my_pkg/                          ← 包目录（名字就是包名）
├── package.xml                  ← 【必需】包的"身份证"
├── CMakeLists.txt               ← 【必需】编译规则
├── src/                         ← 节点源码（.py / .cpp）
│   ├── talker.py
│   └── listener.cpp
├── msg/                         ← 自定义消息（.msg），可选
│   └── MyMsg.msg
├── srv/                         ← 自定义服务（.srv），可选
│   └── MyService.srv
├── action/                      ← 自定义 Action（.action），可选
│   └── MyAction.action
├── launch/                      ← launch 启动文件（.launch），可选
│   └── demo.launch
├── config/                      ← 参数配置文件（.yaml），可选
│   └── params.yaml
├── include/包名/                ← C++ 头文件（.h/.hpp），可选
│   └── my_pkg/
│       └── helper.h
├── scripts/                     ← 可执行的 Python 脚本（可选，和 src/ 二选一）
│   └── tool.py
└── README.md                    ← 说明文档（可选）
```

## 话题

### 话题通信基本操作

#### 流程
1. 编写发布方实现
2. 编写订阅方实现
3. 编辑配置文件
4. 编译并执行

**1.发布方**
```cpp
#include "ros/ros.h"
#include "std_msgs/String.h" //普通文本类型的消息
#include <sstream>

int main(int argc, char  *argv[])
{   
    //设置编码
    setlocale(LC_ALL,"");

    //初始化 ROS 节点
    ros::init(argc,argv,"talker");

    //实例化 ROS 句柄
    ros::NodeHandle nh;//该类封装了 ROS 中的一些常用功能

    //实例化 发布者 对象
    ros::Publisher pub = nh.advertise<std_msgs::String>("chatter",10);

    std_msgs::String msg;
    std::string msg_front = "Hello 你好！"; //消息前缀
    int count = 0; //消息计数器

    //一秒1次
    ros::Rate r(1);

    while (ros::ok())
    {
        //使用 stringstream 拼接字符串与编号
        std::stringstream ss;
        ss << msg_front << count;
        msg.data = ss.str();
        //发布消息
        pub.publish(msg);
        //加入调试，打印发送的消息
        ROS_INFO("发送的消息:%s",msg.data.c_str());

        //根据前面制定的发送贫频率自动休眠 休眠时间 = 1/频率；
        r.sleep();
        count++;//循环结束前，让 count 自增

        ros::spinOnce();
    }


    return 0;
}

```
**2.订阅方**
```cpp
#include "ros/ros.h"
#include "std_msgs/String.h"

//回调函数
void doMsg(const std_msgs::String::ConstPtr& msg_p){ //注意msg_p是一个指针！访问成员变量的时候用 -> (msg_p->data)
    ROS_INFO("我听见:%s",msg_p->data.c_str());
    // ROS_INFO("我听见:%s",(*msg_p).data.c_str());
}
int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");

    //初始化 ROS 节点:命名
    ros::init(argc,argv,"listener");

    //实例化 ROS 句柄
    ros::NodeHandle nh;

    //实例化 订阅者 对象
    ros::Subscriber sub = nh.subscribe<std_msgs::String>("chatter",10,doMsg);

    ros::spin();//循环读取接收的数据，并调用回调函数处理

    return 0;
}
```

**3.配置 CMakeLists.txt**
```cmake
add_executable(Hello_pub
  src/Hello_pub.cpp
)
add_executable(Hello_sub
  src/Hello_sub.cpp
)

target_link_libraries(Hello_pub
  ${catkin_LIBRARIES}
)
target_link_libraries(Hello_sub
  ${catkin_LIBRARIES}
)

```

**4.执行**

1.启动 roscore

2.启动发布节点

3.启动订阅节点

### 话题通信自定义msg
在 `包/msg/` 下写 `.msg` 文件来自定义消息

1. 写 .msg 文件 （ `包/msg/RobotStatus.msg` ）


## 服务端与客户端

1. **客户端（Client）**：发出请求的人。它向服务名发送请求，并**阻塞等待**服务端返回结果
2. **服务端（Server）**：提供服务的人。它注册一个服务名，并在后台等待别人的请求

**通信格式**：服务不使用 `.msg` 文件，而是使用 `.srv` 文件（就是后缀是 `.srv` 的文件）
- `.srv`文件：用`---`把请求和应答分开：
```test
int64 a
int64 b
---
int64 sum
```
- `---` 上面：请求的字段（客户端发什么）

- `---` 下面：应答的字段（服务端返回什么）

**代码实现**

1. 客户端（Client）
```cpp
#include "ros/ros.h"
#include "my_pkg/AddTwoInts.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "add_client");
    ros::NodeHandle n;

    // 创建客户端，绑定到 "add_two_ints" 服务
    ros::ServiceClient client = n.serviceClient<my_pkg::AddTwoInts>("add_two_ints");

    // 实例化服务数据结构
    my_pkg::AddTwoInts srv;
    srv.request.a = 10;
    srv.request.b = 20;

    // 发起调用（这里会阻塞！直到服务端返回）
    if (client.call(srv)) {
        ROS_INFO("调用成功！结果是: %ld", (long)srv.response.sum);
    } else {
        ROS_ERROR("调用失败！服务端可能没启动。");
    }

    return 0;
}
```
`client.call(srv)` 这一行：
如果服务端没启动，它会返回 `false`；如果启动了，程序会卡在这一行，直到服务端算完并把数据塞进 `srv.response` 才继续往下走

2. 服务端（Server）
```cpp
#include "ros/ros.h"
#include "my_pkg/AddTwoInts.h" // 自动生成的服务头文件

// 回调函数：必须接收 req 和 res，返回 bool
bool add(my_pkg::AddTwoInts::Request &req,
         my_pkg::AddTwoInts::Response &res)
{
    res.sum = req.a + req.b; // 把计算结果填入响应
    ROS_INFO("收到请求: %ld + %ld = %ld", (long)req.a, (long)req.b, (long)res.sum);
    return true; // 返回 true 表示处理成功（如果返回 false，客户端会知道服务失败）
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "add_server");
    ros::NodeHandle n;

    // 注册服务：服务名叫 "add_two_ints"，收到请求就执行 add 函数
    ros::ServiceServer service = n.advertiseService("add_two_ints", add);
    
    ROS_INFO("服务端已就绪，等待请求...");
    ros::spin(); // 依然是靠 spin 来驱动服务回调
    return 0;
}
```

## PID参数

### PID数学公式

PID数学公式中有三个项：比例项（P）、积分项（I）、微分项（D）
1. 比例项（P）：提供及时响应
2. 积分项（I）：误差积累，可用于克服阻力、贴近期望值等
3. 微分项（D）：提供阻尼、刹车，防止冲过头或缓解振荡

三个参数：KP、KI、KD，分别对应 比例项P、积分项I、微分项D 的系数
### PID的代码

PID的节柄是简单的NodeHandle类，因为它没有涉及到ros_control的内容

#### control_tools::Pid类
```cpp
namespace control_toolbox {

class Pid {
private:
    // ==========================================
    // 1. 核心数据成员（内部存了什么数字？）
    // ==========================================
    double p_gain_;  // 比例系数
    double i_gain_;  // 积分系数
    double d_gain_;  // 微分系数

    double i_term_;       // 积分累加器（核心记忆体，你调参时的噩梦来源）
    double last_error_;   // 保存上一次的误差，用来算微分
    double last_output_;  // 保存上一次的输出值

    // 积分限幅（防止积分爆炸，Windup）
    double i_max_;
    double i_min_;

public:
    // ==========================================
    // 2. 构造函数与初始化
    // ==========================================
    Pid() : p_gain_(0.0), i_gain_(0.0), d_gain_(0.0), 
            i_term_(0.0), last_error_(0.0), i_max_(0.0), i_min_(0.0) {}

    // 我的代码里传入了 fl_nh，底层就是去参数服务器里捞 p、i、d
    bool init(ros::NodeHandle& nh) {
        nh.param("p", p_gain_, 0.0);
        nh.param("i", i_gain_, 0.0);
        nh.param("d", d_gain_, 0.0);
        nh.param("i_clamp_max", i_max_, 0.0);
        nh.param("i_clamp_min", i_min_, 0.0);
        return true;
    }

    // ==========================================
    // 3. 重置（失忆）
    // ==========================================
    void reset() {
        i_term_ = 0.0;      // 清空积分历史
        last_error_ = 0.0;  // 清空上次误差
        last_output_ = 0.0; // 清空上次输出
    }

    // ==========================================
    // 4. 核心计算引擎 1： updatePid 
    // (你一开始用的那个，容易翻车的那个)
    // ==========================================
    double updatePid(double error, ros::Duration dt) {
        double p_term = p_gain_ * error; // P项：纯比例

        // I项：累加历史误差
        i_term_ += i_gain_ * error * dt.toSec();
        // 安全限制（防爆炸）
        if (i_term_ > i_max_) i_term_ = i_max_;
        if (i_term_ < i_min_) i_term_ = i_min_;

        // D项：算误差变化率 (当前误差 - 上次误差) / 时间
        double d_term = 0.0;
        if (dt.toSec() > 0.0) {
            d_term = d_gain_ * (error - last_error_) / dt.toSec();
        }
        last_error_ = error; // 记住这次的误差，留给下次用

        // 三大项相加，得到最终力矩
        double output = p_term + i_term_ + d_term;
        return output;
    }

    // ==========================================
    // 5. 核心计算引擎 2： computeCommand 
    // ==========================================
    double computeCommand(double target, double current, ros::Duration dt) {
        // 它内部自动帮你做减法，这就不会弄反方向了！
        double error = target - current; 
        
        // 然后调用上面的 updatePid
        return updatePid(error, dt);
    }
};

} // namespace control_toolbox
```

#### 我的代码的讲解

```cpp
    ros::NodeHandle pid_nh(controller_nh, "pid");

    ros::NodeHandle fl_nh(pid_nh, "front_left");
    pid_front_left_.init(fl_nh);

    ros::NodeHandle fr_nh(pid_nh, "front_right");
    pid_front_right_.init(fr_nh);

    ros::NodeHandle bl_nh(pid_nh, "back_left");
    pid_back_left_.init(bl_nh);

    ros::NodeHandle br_nh(pid_nh, "back_right");
    pid_back_right_.init(br_nh);
```

底层 ros::NodeHandle 的重载构造函数长这样（极简版）：
```cpp
NodeHandle(const NodeHandle& parent, const std::string& ns) {
    // 把父路径和子名字拼起来，中间加个斜杠
    this->namespace_ = parent.namespace_ + "/" + ns;
}
```
就是套娃，来区分四个轮子，使其各自独立能设置各自的pid参数，代码节柄路径就是controller_nh/pid_nh/fl_nh（这个pid_nh也可以不要，也能区分，但要改一下.yaml文件）

而/hero_chassis_controller/pid/front_left则是对应了.yaml文件当中的路径

## ros_control

我的代码只需和hard_interface打交道 这个纯虚基类就会自动和硬件打交道；我根本不用管这个纯虚基类怎么和硬件对接的 我只用关心我的代码是怎么和这个纯虚基类对接

### Interface（接口）

Interface 分为 Write Interface（读）和 Read Interface（写）

### 理解一下ros_controll当中常见的类（用c++来呈现助于理解）

#### 一、底层核心：Controller 的生命周期框架
```cpp

// ==================== 第一层：纯虚基类（规则） ====================
namespace controller_interface {

// 1. 最顶层的爷爷：定义所有控制器的基本生命周期
class ControllerBase {
public:
    virtual ~ControllerBase() = default;
    
    // 纯虚函数：强制所有子类必须实现
    virtual void starting(const ros::Time& time) = 0;
    virtual void stopping(const ros::Time& time) = 0;
    virtual void update(const ros::Time& time, const ros::Duration& period) = 0; 
};

// ==================== 第二层：模板类（适配器） ====================
// 2. 爸爸：模板类，继承了爷爷 ControllerBase
// 这里的 <class T> 就是你尖括号里传入的接口类型
template <class T>
class Controller : public ControllerBase {
public:
    // 增加了一个带特定硬件指针的纯虚 init 函数
    virtual bool init(T* hw_interface, ros::NodeHandle& root_nh, ros::NodeHandle& controller_nh) = 0;
};

} // namespace controller_interface
```
template是模板，里面的T可类比为函数参数，T是什么下面的T就是什么（比如T可以是`hardware_interface::EffortJointInterface` ，写代码时这个类就是`controller_interface::Controller<hardware_interface::EffortJointInterface>` ）

T可以是：
- hardware_interface::EffortInterface
- hardware_interface::VelocityInterface
- hardware_interface::PositionInterface

注意这三个里都有getHandle函数，但是不一样的（简单来说就是后续读写操作时物理单位不一样）

这里的T是后面给init函数作参数的指针类型的，后续写代码实现init这个纯虚函数时，比如
```cpp
bool HeroChassisController::init(hardware_interface::EffortJointInterface *effort_joint_interface,
                                   ros::NodeHandle &root_nh, ros::NodeHandle &controller_nh) {

    front_left_joint_ = effort_joint_interface->getHandle("left_front_wheel_joint");
    front_right_joint_ = effort_joint_interface->getHandle("right_front_wheel_joint");
    back_left_joint_ = effort_joint_interface->getHandle("left_back_wheel_joint");
    back_right_joint_ = effort_joint_interface->getHandle("right_back_wheel_joint");
    }                                
```
第一个参数的`effort_joint_interface`这个指针就是硬件接口指针

而第二个参数root_nh和第三个参数controller_nh都是句柄，区别在于：
- **root_nh（根节点句柄）：**它的内部路径是 `/` （全局根目录），一般用不到这个参数
- **controller_nh（控制器句柄）：**它的内部路径是 `/hero_chassis_controller/` （被底层框架自动限定在了你的专属命名空间下），这个参数比较常用

- 在我的代码中有：
```cpp
// 订阅速度指令（相对路径）
cmd_vel_sub_ = controller_nh.subscribe("cmd_vel", 1, ...);
// 读取底盘参数
controller_nh.param("use_global_frame", use_global_frame_, false);

// 建立 PID 句柄的基座
ros::NodeHandle pid_nh(controller_nh, "pid"); 
```

**代码绑定机器人的关键是getHandle这个函数,只有用getHandle绑定了才能进行读写操作。**
**getHandle()括号里的字符串要和URDF中的Joint一样才能进行对接**

#### 二、硬件抽象：数据与接口的管理
```cpp
namespace hardware_interface {

// ==================== 第一层：状态句柄（只读） ====================
// 3. 关节状态句柄：里面装了三个只读指针，指向底层的物理内存
class JointStateHandle {
protected:
    const double* pos_;
    const double* vel_;
    const double* eff_;
public:
    double getVelocity() const { return *vel_; } // 你代码里用的 getVelocity 就在这
};

// ==================== 第二层：命令句柄（可读写） ====================
// 4. 关节命令句柄：继承自状态句柄，多了一个可写的控制指针
class JointHandle : public JointStateHandle {
private:
    double* cmd_; // 指向底层电机的指令内存
public:
    // 你代码里用的 setCommand 就在这！
    void setCommand(double command) { *cmd_ = command; } 
};

// ==================== 第三层：资源管理池（模板类） ====================
// 5. 资源管理池：用来装一堆 Handle
template <class Handle>
class HardwareResourceManager {
private:
    std::map<std::string, Handle> handles_; // 内部是个字典，靠名字找句柄
public:
    Handle getHandle(const std::string& name) { return handles_.at(name); }
};

// ==================== 第四层：具体接口类型 ====================
// 6. 力矩控制接口：继承自资源池，专门装 JointHandle
// 这就是你在尖括号里填的那个类型！
class EffortJointInterface : public HardwareResourceManager<JointHandle> {};

// 同理，还有速度控制接口
class VelocityJointInterface : public HardwareResourceManager<JointHandle> {};

} // namespace hardware_interface
```

上述当中的Controller和JointHandle继承了ControllerBase和JointStateHandle，所以平常写代码可只用前面那两个就行了（JointHandle可读可写）

#### 三、就是我写的实现类（控制器）了
```cpp
namespace hero_chassis_controller {

// 我的控制器（HeroChassisController）：继承自 Controller 模板（并将模板实例化为力矩接口）
class HeroChassisController : public controller_interface::Controller<hardware_interface::EffortJointInterface> {
public:
    // 实现了爷爷的纯虚函数
    void starting(const ros::Time& time) override;
    void stopping(const ros::Time& time) override;
    void update(const ros::Time& time, const ros::Duration& period) override;

    // 实现了爸爸的纯虚函数（注意参数类型！）
    bool init(hardware_interface::EffortJointInterface* effort_joint_interface,
              ros::NodeHandle& root_nh, ros::NodeHandle& controller_nh) override;

    // 我自己的回调函数
    void cmdVelCallback(const geometry_msgs::Twist& msg);

private:
    // 【核心绑定点】你持有的是具体的 JointHandle
    hardware_interface::JointHandle front_left_joint_;
    hardware_interface::JointHandle front_right_joint_;
    hardware_interface::JointHandle back_left_joint_;
    hardware_interface::JointHandle back_right_joint_;

    // 其他工具类（不属于上述继承体系，是独立的类）
    control_toolbox::Pid pid_front_left_;
    // ... 其他 PID

    realtime_tools::RealtimeBuffer<geometry_msgs::Twist> cmd_vel_buffer_;
    
    tf::TransformBroadcaster odom_broadcaster_;
    tf::TransformListener tf_listener_;

    // 我自己的状态变量
    double x_, y_, th_;
    // ...
};

} // namespace hero_chassis_controller
```
