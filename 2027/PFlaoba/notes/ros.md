# DX控制组ROS第1课笔记

## 目录

- **一、 概述与基础**
- **二、 通信**
- **三、 运行管理**
- **四、 总结**

## 一、 概述与基础

### 0. 概念

ROS（Robot Operating System，机器人操作系统）是一个面向机器人的开源软件框架，为机器人软件的开发、运行和调试提供一系列工具、库和通信机制

### 1. 特点

- 开源与社区支持
- 模块化开发
- 跨语言支持
- 便于集成不同设备
- ROS 的版本与发展

    ROS 主要有 ROS 1 和 ROS 2 两个系列

    ROS 1：拥有丰富的教程、软件包和成熟的社区资源。常见版本包括 Melodic 和 Noetic

    ROS 2：在通信机制、实时性支持、安全性和多平台支持等方面进行了改进，更适合多样化的机器人应用需求

    二者在部分概念和开发方式上相似，但二者并不完全兼容，软件包通常需要根据对应版本进行适配

### 2. 开发环境与常用工具

#### 2.1 开发环境

- **ROS Noetic**：ROS 1 的最后一个正式发行版系列，主要面向 Ubuntu 20.04，并以 C++、Python 3、catkin 和 ROS 1 通信机制为核心开发基础

- **Docker**：容器化工具，可以将 ROS 及其依赖放在相对独立的环境中运行，减少不同项目之间的软件依赖冲突,使用容器时注意文件共享与环境变量

#### 2.2 Terminator 终端模拟器

Terminator 是一种终端模拟器，提供图形化的终端窗口，使用户能够通过 Shell 与 Linux 系统交互
Terminator 本身不是 Shell，也不是编译器
Terminator 负责提供终端界面，而具体命令通常由 Bash 等 Shell 解释和执行

#### 2.3 Visual Studio Code

**VS Code** 是一款跨平台的源代码编辑器，具有语法高亮、代码补全、错误提示、调试和插件扩展等功能

通过安装适当的扩展，可以将 VS Code 用于 C++、Python、CMake 和 ROS 开发

#### 2.4 launch

在 ROS 1 中，launch 文件是一种使用 XML 格式编写的配置文件，通常以 .launch 为扩展名

它用于描述如何启动一个或多个 ROS 节点，以及如何设置参数、传递启动参数和组织相关启动配置

如果逐个启动多个 ROS 节点，需要在终端中输入多条命令。使用 launch 文件可以将这些启动配置集中管理，便于重复运行和维护

launch 文件负责组织程序的启动，不负责代替 CMake 或 catkin 编译源代码

在一般开发流程中，先编写代码和配置文件，再使用 catkin 构建软件包，最后通过 rosrun 或 roslaunch 运行编译得到的节点

如果修改了 C++ 源代码，通常需要重新构建；如果只修改启动参数或节点组合，则可能不需要重新编译

执行 roslaunch 时，如果当前环境中还没有运行 ROS Master，且没有配置外部 Master，roslaunch 通常会自动启动一个 ROS Master

演示：
```xml
<launch>
    <node pkg="demo1"
          type="hello_node"
          name="hello_node"
          output="screen"/>
</launch>
```
其中：

- <launch>：整个 launch 文件的根标签
- <node>：声明需要启动的 ROS 节点
- pkg：节点所属的软件包名称
- type：需要执行的节点可执行文件名称
- name：运行时使用的节点名称
- output：设置日志的输出目标（output="screen"：将节点的输出显示在终端中）

### 3. 架构

#### 3.1 ROS 文件系统及命令

Linux 文件系统负责存放文件，ROS 文件系统则在此基础上组织 ROS 软件包及其资源

```
/root/catkin_ws/
├── src/
│   └── demo1/
│       ├── package.xml
│       ├── CMakeLists.txt
│       ├── src/
│       │   └── hello.cpp
│       └── launch/
│           └── hello.launch
├── build/
├── devel/
└── install/
```

```
Linux 文件系统
    └── ROS 工作空间
          └── ROS 软件包
                ├── 源代码
                ├── 构建配置
                ├── 启动文件
                ├── 参数配置
                └── 消息 / 服务 / 动作定义
```

#### 3.2 ROS 文件系统命令

增、删、改的命令与shell一致

查：
1. `rospack find`	查找软件包路径	  
   例：`rospack find demo1`
2. `roscd` 切换到 ROS 包或相关目录  
   例：`roscd demo1`
3. `rosls` 查看 ROS 包中的目录内容  
   例：`rosls demo1`
4. `rosnode list`	查看当前运行的 ROS 节点  
5. `rostopic list` 查看当前 ROS 话题  

检验：
1. `echo $ROS_DISTRO`	查看当前 ROS 发行版环境变量
2. `echo $ROS_PACKAGE_PATH`	查看 ROS 软件包搜索路径
3. `rospack find demo1`	检验 ROS 是否能定位 demo1
4. `catkin_make`	构建工作空间
5. `rosnode list`	查看运行中的 ROS 节点
6. `rosnode info /hello_node`	查看指定节点的信息
7. `rostopic list`	查看当前 ROS 话题
8. `roslaunch demo1 hello.launch`	按 launch 文件启动节点

#### 3.3 计算图

ROS 计算图是 ROS 系统运行时的逻辑结构，用来描述节点之间的关系，以及节点通过哪些通信接口交换数据

ROS 文件系统描述代码和资源存放在哪里；计算图描述程序运行后，节点如何协作

计算图核心组成：
|组成|含义|
|-|-|
|Node（节点）|执行具体任务的 ROS 进程|
|Topic（话题）|节点之间进行发布/订阅式通信的通道|
|Message（消息）|在话题上交换的数据结构|
|Service（服务）|请求—响应式通信接口|
|Action（动作）|适合较长任务、反馈和取消的通信机制|
|ROS Master|帮助节点发现彼此并协商连接的中心服务|

## 二、 通信

### 0. 概念

#### 0.1 Talker Listener Server

#### 0.2 协议

**协议（Protocol）**就是通信双方共同遵守的一套规则

#### 0.3 TCP

TCP 的全称是 Transmission Control Protocol（传输控制协议）

作用：连接、可靠传输、按序交付字节流

TCP 只提供字节流，并不天然知道一条完整消息在哪里结束

TCP 也不保证数据一定能送达：如果连接彻底中断，应用程序仍然需要处理失败

#### 0.4 RPC

RPC 的全称是 Remote Procedure Call（远程过程调用）

作用：编码参数、发起请求、解析响应

### 1. 话题通信

#### 1.1 发布方与订阅方

##### 1.1.1 发布方实现
```cpp
//1.包含头文件
   
   #include"ros/ros.h"
   //ROS 中文本类型：
   #include"std_msgs/String.h"
//2.初始化 ROS 节点
   ros::init(argc,argv,"node_name");
//3.创建节点句柄
   ros::NodeHandle nh;
//4.创建发布者对象
       ros::Publisher pub = nh.advertise<std_msgs::String>("topic_name", 10);  
//5. 编写发布逻辑并发布数据
   //先创建被发布的消息
   std_msgs::String msg;
   msg.data = "hello"; 
   //编写循环，循环中发布数据
   while (ros::ok())
   {
      pub.publish(msg);
   }
```

##### 1.1.2 发布逻辑

接上，更改一下第5点
```cpp
std_msgs::String msg;
    ros::Rate rate(10);
    int count = 0;
    while (ros::ok())
   {
    count++;
    std::stringstream ss;
    ss << "hello-->" << count;
    msg.data = ss.str();
    pub.publish(msg);
    ROS_INFO("发布的数据是：%s",ss.str().c_str());
    rate.sleep();
   }
```

##### 1.1.3 订阅方实现

```cpp
#include<ros/ros.h>
#include<std_msgs/String.h>
void doMsg(const std_msgs::String::ConstPtr& msg )
{
    ROS_INFO("订阅的数据是：%s",msg->data.c_str());
}
int main(int argc,char *argv[])
{
    setlocale(LC_CTYPE,"zh_CN.UTF-8");
    ros::init(argc,argv,"sub_node");
    ros::NodeHandle nh;
    //创建订阅者对象
    ros::Subscriber sub = nh.subscribe("topic",10,doMsg);
    //设置循环调用回调函数
    ros::spin();//循环读取接收的数据，并调用回调函数处理
    return 0;
}
```

#### 1.2 注意事项

订阅消息流失：
```cpp
//等待3秒
ros::Duration(3).sleep()
```

#### 1.3 自定义msg

1. 定义msg文件
2. 编辑配置文件
3. 编译

### 2. 服务通信

#### 2.1 自定义srv

1. 定义srv文件
2. 编辑配置文件
3. 编译

#### 2.2 服务端实现

接上自定义了srv
```srv
int32 num1;
int32 num2;
---
int32 sum
```

```cpp
#include<ros/ros.h>
#include<demo3/Add.h>
bool doNum (demo3::Add::Request& request,
    demo3::Add::Response& response) {
    int num1 = request.num1;
    int num2 = request.num2;
    ROS_INFO("收到请求的数据是：num1= %d, num2= %d", num1, num2);
    int sum = num1 + num2;
    response.sum = sum;
    ROS_INFO("响应的数据是：num3= %d", sum);
    return true;
}
int main(int argc, char *argv[]) {
    setlocale(LC_CTYPE,"zh_CN.UTF-8");
    ROS_INFO("服务端已启动");
    ros::init(argc, argv, "srv_name");
    ros::NodeHandle nh;
    ros::ServiceServer server = nh.advertiseService("Addtp",doNum);
    ros::spin();
    return 0;
}

```


使用`rosservice call`实现请求和响应

#### 2.3 客户端实现

参数动态提交：
```cpp
#include <ros/ros.h>
#include <demo3/Add.h>
int main(int argc, char* argv[]) {
    setlocale(LC_CTYPE,"zh_CN.UTF-8");
    if (argc != 3) {ROS_INFO("参数设置失败");return 1;}
    ros::init(argc, argv, "cli_name");
    ros::NodeHandle nh;
    ros::ServiceClient client = nh.serviceClient<demo3::Add>("Addtp");
    demo3::Add ai;
    ai.request.num1 = atoi(argv[1]);
    ai.request.num2 = atoi(argv[2]);
    bool result = client.call(ai);
    if (!result) {
        ROS_INFO("处理失败");
    } else {
        ROS_INFO("响应成功");
    }
    ros::spin();
    return 0;
}
```

#### 2.3 注意事项

先启动客户端再启动服务器：
```cpp
//调用判断服务器异常的函数
client.waitForExistence();
```

### 3. 参数服务器（Parameter Server）

1. NodeHandle

| 操作    | 函数                 | 示例                                         | 作用                       |
| ----- | ------------------ | ------------------------------------------ | ------------------------ |
| 增 / 改 | `nh.setParam()`    | `nh.setParam("radius", 0.3);`              | 设置参数，存在则修改，不存在则新增        |
| 查     | `nh.getParam()`    | `nh.getParam("radius", radius);`           | 获取参数，返回 `true` 或 `false` |
| 查     | `nh.param()`       | `nh.param("radius", radius, 0.5);`         | 获取参数，不存在则使用默认值           |
| 查     | `nh.param()`       | `double radius = nh.param("radius", 0.5);` | 获取参数并返回参数值，不存在则返回默认值     |
| 查     | `nh.hasParam()`    | `nh.hasParam("radius");`                   | 判断参数是否存在                 |
| 删     | `nh.deleteParam()` | `nh.deleteParam("radius");`                | 删除指定参数                   |

2. rosparam

| 操作    | 指令                | 示例                          | 作用                |
| ----- | ----------------- | --------------------------- | ----------------- |
| 增 / 改 | `rosparam set`    | `rosparam set /radius 0.3`  | 设置参数，存在则修改，不存在则新增 |
| 查     | `rosparam get`    | `rosparam get /radius`      | 获取指定参数的值          |
| 查     | `rosparam list`   | `rosparam list`             | 查看所有参数名称          |
| 查     | `rosparam get /`  | `rosparam get /`            | 查看所有参数及其值         |
| 删     | `rosparam delete` | `rosparam delete /radius`   | 删除指定参数            |
| 导出    | `rosparam dump`   | `rosparam dump params.yaml` | 将参数保存到 YAML 文件    |
| 导入    | `rosparam load`   | `rosparam load params.yaml` | 从 YAML 文件加载参数     |

注意：
- `NodeHandle`：在 C++ 程序中操作参数服务器
- `rosparam`：在终端中操作参数服务器，也可以在程序中使用`ros::param::xxx`的方法操作参数服务器
- 两者操作的是同一个参数服务器

### 4. ROS 常用命令

1. rosnode
2. rostopic
3. rosservice
4. rosmsg
5. rossrv
6. rosparam

rosnode:
```bash
# 查看所有节点
rosnode list

# 查看指定节点信息
rosnode info /talker

# 检查节点是否正常响应
rosnode ping /talker

# 关闭指定节点
rosnode kill /talker

# 列出指定设备上的节点
rosnode machine

# 清除不可连接的节点
rosnode cleanup
```

rostopic:
```bash
# 查看所有话题
rostopic list

# 查看带有 header 主题的延迟
rostopic delay

# 根据类型查看主题
rostopic find

# 查看话题信息
rostopic info /chatter

# 查看消息类型
rostopic type /chatter

# 实时查看话题消息
rostopic echo /chatter

# 查看话题发布频率
rostopic hz /chatter

# 查看话题带宽
rostopic bw /chatter
```

rosservice:
```bash
# 查看所有服务
rosservice list

# 查看服务信息
rosservice info /clear

# 查看服务类型
rosservice type /clear

# 查看服务请求参数
rosservice args /clear
```

rosmsg:
```bash
# 查看所有消息类型
rosmsg list

# 查看 String 消息结构
rosmsg show std_msgs/String

# 查看 geometry_msgs 中的消息类型
rosmsg package geometry_msgs

# 查看所有包含消息定义的功能包
rosmsg packages
```

rossrv:
```bash
# 查看所有服务类型
rossrv list

# 查看 SetBool 服务结构
rossrv show std_srvs/SetBool

# 查看 std_srvs 包中的服务类型
rossrv package std_srvs

# 查看所有包含服务定义的功能包
rossrv packages
```

rosparam:
```bash
# 设置参数
rosparam set /radius 0.3

# 查看参数
rosparam get /radius

# 修改参数
rosparam set /radius 0.5

# 删除参数
rosparam delete /radius
```

### 5. 通信机制比较

| 对比项目       | Topic                  | Service         | Parameter Server |
| ---------- | ---------------------- | --------------- | ---------------- |
| 中文名称       | 话题通信                   | 服务通信            | 参数服务器            |
| 通信模式       | 发布/订阅                  | 请求/响应           | 参数读写             |
| 通信关系       | Publisher / Subscriber | Client / Server | 节点与参数服务器         |
| 是否需要对方主动响应 | 不需要                    | 需要              | 不需要              |
| 数据传输       | 连续消息                   | 一次请求、一次响应       | 参数读写             |
| 是否有返回值     | 没有直接返回值                | 有响应数据           | 读取时返回参数值         |
| 适用场景       | 连续数据传输                 | 请求某项操作并获取结果     | 保存配置参数           |

### 6. 常用API

#### 6.1 `ros::init` 初始化

argc —— 命令行参数数量

- 表示命令行参数的数量
- 是 `main()` 函数传入的参数
- ROS 使用它来解析命令行参数

argv —— 命令行参数内容

```
char **argv
```

- 保存命令行参数的具体内容
- 与 `argc` 配合使用
- ROS 通过它识别节点名称重映射等参数

例如：

```
rosrun demo1 hello_node __name:=my_node
```

 `__name:=my_node` 就是 ROS 的特殊命令行参数之一

name —— 节点名称

- 指定当前 ROS 节点的默认名称
- 可以自定义
- 可以通过 ROS 命令查看运行中的节点

options —— 初始化选项

常见选项：

| 选项                                   | 作用                       |
| ------------------------------------ | ------------------------ |
| `ros::init_options::AnonymousName`   | 自动给节点名称添加随机后缀，避免名称冲突     |
| `ros::init_options::NoSigintHandler` | 不安装 ROS 默认的 SIGINT 信号处理器 |
| `ros::init_options::NoRosout`        | 不启动 ROS 日志输出相关功能         |

| 参数        | 类型/形式      | 主要作用      |
| --------- | ---------- | --------- |
| `argc`    | `int`      | 命令行参数数量   |
| `argv`    | `char**`   | 命令行参数内容   |
| `name`    | 字符串        | 当前节点的默认名称 |
| `options` | `uint32_t` | 节点初始化选项   |

#### 6.2 `ros::NodeHandle` 话题与服务相关对象

`ros::NodeHandle`（节点句柄）是 ROS1 中一个非常重要的类，主要用于创建和管理节点的通信对象

- 类型：类（Class）
- 作用：创建和管理 ROS 通信对象

常用通信对象：

| 对象                   | 对应 API                  |
| -------------------- | ----------------------- |
| `ros::Publisher`     | `nh.advertise()`        |
| `ros::Subscriber`    | `nh.subscribe()`        |
| `ros::ServiceServer` | `nh.advertiseService()` |
| `ros::ServiceClient` | `nh.serviceClient()`    |

默认latch参数是false，作用详见导航模块

#### 6.3 回旋函数

`ros::spin()` ：处理回调函数，循环执行任务
`ros::spinOnce()`：处理一次回调函数，接收信息

#### 6.4 时间

`ros::Time` 是 ROS1 中用来表示和获取时间的类
`ros::Time::now()` 获取的是从 Unix 时间起点（1970-01-01 00:00:00 UTC）开始经过的秒数

```cpp
ros::Time current_time = ros::Time::now();

ROS_INFO("当前时间：%f", current_time.toSec());
```

`toSec()`作用是将 ROS 时间转换为以秒为单位的 `double` 数值。

```cpp
ros::Time t = ros::Time::now();

ROS_INFO("秒：%u", t.sec);
ROS_INFO("纳秒：%u", t.nsec);
```

```cpp
ros::Time start_time = ros::Time::now();

// 执行某些任务

ros::Time end_time = ros::Time::now();

ros::Duration duration = end_time - start_time;

ROS_INFO("执行耗时：%f 秒", duration.toSec());
```

`ros::Duration`：表示一段时间，也就是时间间隔

| 类型              | 含义    | 例子     |
| --------------- | ----- | ------ |
| `ros::Time`     | 某个时间点 | 现在是几点  |
| `ros::Duration` | 一段时间  | 经过了多少秒 |

| 常用API             | 作用                    |
| ------------------ | --------------------- |
| `ros::Time::now()` | 获取当前 ROS 时间           |
| `t.toSec()`        | 转换成秒（double）          |
| `t.sec`            | 获取秒部分                 |
| `t.nsec`           | 获取纳秒部分                |
| `t1 - t2`          | 计算两个时间点的差，得到 Duration |
| `t1 + duration`    | 时间点加上时间间隔，得到 Time     |

#### 6.5 计时器

`ros::Rate`：运行频率，配合`sleep()`使用

```cpp
#include "ros/ros.h"

// 定时器回调函数
void timerCallback(const ros::TimerEvent& event)
{
    ROS_INFO("定时器触发！");
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "timer_node");
    ros::NodeHandle nh;
    // 创建定时器，每隔1秒触发一次
    ros::Timer timer = nh.createTimer(
        ros::Duration(1.0),
        timerCallback
    );
    ros::spin();
    return 0;
}
```

`ros::Rate`：控制自己的循环频率

```cpp
ros::Rate rate(10);
while (ros::ok())
{
    ROS_INFO("执行循环任务");
    ros::spinOnce();
    rate.sleep();
}
```

`ros::Timer`：让 ROS 定时调用指定的回调函数

```cpp
ros::Timer timer = nh.createTimer(
    ros::Duration(1.0),
    timerCallback
);
ros::spin();
```

| 对比        | `ros::Rate`   | `ros::Timer` |
| --------- | ------------- | ------------ |
| 作用        | 控制循环频率        | 定时触发回调函数     |
| 使用方式      | 配合 `while` 循环 | 配合回调函数       |
| 是否需要自己写循环 | 需要            | 不需要          |
| 是否需要处理回调  | 如果有回调，需要      | 需要           |

| 常用API               | 作用          |
| -------------------- | ----------- |
| `nh.createTimer()`   | 创建定时器       |
| `timer.stop()`       | 停止定时器       |
| `timer.start()`      | 启动定时器       |
| `timer.hasStarted()` | 判断定时器是否已经启动 |
| `timer.setPeriod()`  | 修改定时周期      |

定时器默认会重复触发，与`nh.createTimer()`第三个参数`oneshot`（单次触发）和第四个参数`autostart`（自动启动）有关

#### 6.6 节点生命周期

`ros::ok()`：判断节点是否结束
`ros::shutdown()`：关闭节点

#### 6.7 日志输出

|常用API| 日志级别 |	用途  |
|-----------|-----|-------------------|
|ROS_DEBUG()|DEBUG| 调试信息，排查程序问题|
|ROS_INFO()	|INFO | 普通运行信息        |
|ROS_WARN()	|WARN | 警告信息，提醒潜在问题|
|ROS_ERROR()|ERROR| 错误信息           |
|ROS_FATAL()|FATAL|	致命错误，通常意味着程序无法继续正常工作|

## 三、 运行管理

### 1. launch

ROS1 中的 Launch 文件标签（Tag） 是指 `.launch` 文件中的 XML 标签，用来配置和启动 ROS 节点、设置参数、管理节点之间的关系等

作用：把原本需要在多个终端中执行的 ROS 命令，集中写在一个 XML 文件中，一次启动

#### 1.1 常用标签总览

常见的 Launch 标签：

| 标签           | 作用                     |
| ------------ | ---------------------- |
| `<launch>`   | Launch 文件的根标签          |
| `<node>`     | 启动 ROS 节点              |
| `<param>`    | 设置参数服务器参数              |
| `<rosparam>` | 加载、删除或设置 YAML 参数       |
| `<arg>`      | 定义 Launch 参数           |
| `<remap>`    | 重映射 Topic 或 Service 名称 |
| `<include>`  | 引入其他 Launch 文件         |
| `<group>`    | 对一组节点和参数进行分组           |
| `<env>`      | 设置节点运行时的环境变量           |
| `<machine>`  | 指定远程机器                 |
| `<test>`     | 启动测试节点                 |

#### 1.2 `<launch>`：根标签

每个 ROS1 Launch 文件都需要以它作为根标签

```xml
<launch>

    <!-- 在这里编写启动配置 -->

</launch>
```

注意：一个 Launch 文件通常只有一个 `<launch>` 根标签

#### 1.3 `<node>`：启动节点

```xml
<launch>

    <node pkg="demo1"
          type="hello_node"
          name="hello"
          output="screen"/>

</launch>
```

各属性的含义：

| 属性       | 含义            |
| -------- | ------------- |
| `pkg`    | ROS 功能包名称     |
| `type`   | 可执行文件名称       |
| `name`   | 启动后 ROS 节点的名称 |
| `output` | 节点日志输出位置      |

例如：

```xml
<node pkg="demo1" type="hello_node" name="hello" output="screen"/>
```

相当于执行：

```bash
rosrun demo1 hello_node
```

但节点名称由 Launch 文件中的 `name` 指定

`output="screen"` 表示将节点日志输出到终端

#### 1.4 `<param>`：设置参数

用于向 ROS Parameter Server 设置参数

```xml
<launch>

    <param name="speed" value="1.5"/>

</launch>
```

相当于：

```bash
rosparam set /speed 1.5
```

常用属性：

| 属性      | 含义                                    |
| ------- | ------------------------------------- |
| `name`  | 参数名称                                  |
| `value` | 参数值                                   |
| `type`  | 指定参数类型，例如 `int`、`double`、`str`、`bool` |

例如：

```xml
<param name="speed" value="10" type="int"/>
<param name="robot_name" value="robot1" type="str"/>
<param name="enabled" value="true" type="bool"/>
```

#### 1.5 `<rosparam>`：加载 YAML 参数

与 `<param>` 类似，但它更适合批量处理参数

例如，创建 `config.yaml`：

```yaml
speed: 1.5
radius: 0.3
robot_name: robot1
```

```xml
<launch>

    <rosparam file="$(find demo1)/config/config.yaml"
              command="load"/>

</launch>
```

这里：

- `file`：指定 YAML 文件路径
- `$(find demo1)`：找到 `demo1` 功能包的路径
- `command="load"`：加载参数

#### 1.6 `<arg>`：定义 Launch 参数

可以理解为 Launch 文件中的变量，让启动时能够传入不同的值

```xml
<launch>

    <arg name="robot_name" default="robot1"/>

    <node pkg="demo1"
          type="hello_node"
          name="$(arg robot_name)"
          output="screen"/>

</launch>
```

其中：

```xml
<arg name="robot_name" default="robot1"/>
```

定义了一个名为 `robot_name` 的参数，默认值是 `robot1`

使用`$(arg robot_name)`就可以引用它

启动时也可以传入其他值：

```bash
roslaunch demo1 demo.launch robot_name:=robot2
```

这样节点名称就会变成 `robot2`

#### 1.7 `<remap>`：重映射名称

用于修改节点使用的 Topic 或 Service 名称

例如：

```xml
<node pkg="demo1" type="hello_node" name="hello">

    <remap from="/chatter" to="/new_chatter"/>

</node>
```

节点原本使用 `/chatter`，通过重映射后，实际使用 `/new_chatter`

注意：`<remap>` 不会修改节点源代码，只会改变 ROS 名称解析结果

#### 1.8 `<include>`：包含其他 Launch 文件

当一个机器人项目有很多 Launch 文件时，可以通过它组合启动配置

```xml
<launch>

    <include file="$(find demo1)/launch/robot.launch"/>

</launch>
```

这样就会引入 `robot.launch` 中的启动配置

#### 1.9 `<group>`：分组

可以将多个节点和参数放在同一个组中

```xml
<launch>

    <group ns="robot1">

        <node pkg="demo1"
              type="hello_node"
              name="hello"/>

        <param name="speed" value="1.5"/>

    </group>

</launch>
```

这里 `ns="robot1"` 设置命名空间，组内的节点和参数会受到这个命名空间的影响

#### 1.10 `<env>`：设置环境变量

```xml
<launch>

    <node pkg="demo1" type="hello_node" name="hello">

        <env name="MY_VARIABLE" value="hello"/>

    </node>

</launch>
```

表示在启动该节点的进程时设置环境变量

### 2. YAML

YAML 全称是 YAML Ain't Markup Language，是一种用于存储结构化数据的文本格式

## 四、 总结

半步入门ROS，理解通信与控制

参考文献：
[ROS API](https://wiki.ros.org/APIs)
[roscpp](https://wiki.ros.org/roscpp)
[roslaunch](https://wiki.ros.org/roslaunch)