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

# 动态参数