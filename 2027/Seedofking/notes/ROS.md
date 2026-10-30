# ROS Knowledge

## Topic
1.话题Topic可以让节点间进行持续通讯
2.两个节点通过话题名称建立通讯连接
3.话题中通讯的数据叫消息Message
4.Message常常按频率持续发送
5.消息发布方叫Publisher, 接收方叫Subscriber
6.一个ros节点网络中可以有多个话题
7.一个话题可以有多个发布者多个订阅者
8.一个节点可以发布多个话题, 也可以订阅多个话题

## Message
由多种基础的数据类型嵌套、组合而成
ros的消息类型不是手写的, 而是由.msg文件所定义, 由编码生成器生成c++头文件
如.msg的 float32->float(c++), float64->double, type[N]->std::Array, type[]->std::vector<type>
ros消息的字段名也是固定的
如String.msg内容为string data, 其成员变量就是一个data
而geometry_msgs/Point的.msg是 float64 x; float64 y; float64 z;要写p.x, p.y, p.z;






# ROS Command

### 启动一个ROS节点

#### 0.为.bashrc添加环境变量
###### 为了让catkin和ros中的环境变量(功能包路径和可执行文件路径)加载到终端会话中, 需要添加环境变量
    source /opt/ros/noetic/setup.bash
    source ~/catkin_ws/devel/setup.bash

#### 1.启动roscore
    roscore

#### 2.启动节点
    rosrun rosetest_pkg rosetest_pkg_node
###### 若显示在包下找不到节点:
1.检查是否已经编译
2.检查是否已经有 rose_ws/devel/lib/rosetest_pkg/rosetest_pkg/rosetest_pkg_node
3.可能需要重新source opt/ros/noetic/setup.bash 和 source /rose_ws/devel/setup.bash


### 查看

#### 查看软件包地址
    roscd 包名
    roscd roscpp
#### 查看有什么软件包
    rospack list
#### 查看有没有这个软件包
    rospack find 软件包
    rospack find rosetest_pkg
#### 查看正在运行的节点
    rosnode list
#### 查看一个节点的详情(向谁发布, 被谁订阅)
    rosnode info /节点名


### 管理
#### 关掉一个节点
    rosnode kill /节点名


### 话题指令
#### 列出当前正在活跃的话题
    rostopic list
#### 图形化显示话题关系
    rqt_graph
#### 查看话题消息内容
    rostopic echo /话题名
###### (若是中文, 则需要将输出的字符用`echo -e "字符"`来转换显示)
#### 查看话题发送频率
    rostopic hz /话题名







# ROS Code

### 头文件:
    #include <ros/ros.h>
    #include <std_msg/Int32.h>
    #include <std_msgs/String>

### using namespace:
不建议在ros中使用任何using namespace std等, 因为这会把整个程序放在std作用域下, 污染命名空间
容易造成某个命名与标准库相同而产生冲突, 所以标准库提供的功能要加std::, 而ros提供的功能加ros::
如果实在是出现频率极高, 那么可以在某个.cpp文件开头或者某个函数内写 using std::string, 但不建议多用
    std::cout, cin, endl, to_string
    std::string, vector, map, array
    

### 主函数:
    int main(int argc, char **argv) {}
###### 意为:主函数需要传入的参数, argc: argument count: 主函数需要传进的数据数
###### **argv: argumrnt vector: 主函数需要传进的数据vector, 类型是字符串数组
###### 用途: 在启动程序时将命令行的指令塞给main函数:`rosrun rosetest_pkg rosetest_pkg_node __name:=hello_node`
arg[0] = rosrun, argv[1] = my_pkg argv[2] = my_node argv[3] = __name:=hello
此时argc就等于4

### ros::init:
    ros::init(argc, argv, "rosetest_pkg_node")
###### 将主函数接收的启动指令传给ros:init函数, 其中第三个参数"rosetest_pkg_node"是节点的名字
###### 若主函数含有__name:=hello则会强制改名为hello, init的名字是跑起来后显示的节点名字
###### CMakeLists.txt里面的包和节点名字将决定rosrun启动时需要输入的名字

### 保持程序执行:
    while(ros::ok())
###### 如果不加循环的话, 程序只会从头到尾执行一次, 加`while(ture)`的话程序不会响应终端的Ctrl+C

### 延时
#### ros::Rate延时:
    ros::Rate loop_rate(5);
###### 意为定义一个名叫loop_rate的对象, 设定循环频率, 这里是每秒执行5次
###### 常搭配`loop_rate.sleep();`使用, 根据上一次调用经过的时间, 自动休眠到下一个周期点
###### 若sleep设定为200ms一次, 则循环执行时间+实际sleep时间 = 200ms
###### 如果sleep所在循环执行的时间的时间大于sleep所设定时间, 则不会执行休眠, 下一个循环会立即开始

#### ros::Duration延时:
    ros:Duration(0.5).sleep()
###### 会在循环执行时间的基础上继续休眠0.5秒

## Topic

### NodeHandle
##### 作用：NodeHandle 是一个节点向Master注册自己、声明名字和要发布、订阅哪些话题的操作句柄
##### 参数：`ros::NodeHandle nh("robot1");`可以指定该节点作用的ros命名空间为/robot1, 默认为/全局命名空间
##### 创建通信对象：通过其成员函数advertise(), subscribe(), serviceClient(), param()等创建发布者, 订阅者, 服务客户端, 读取参数 


### Publilsher
#### 类型:Publisher是roscpp里的一个类, 内部持有话题名, 消息类型, 队列等状态, 提供publish()方法
#### 初始化:`ros::Publisher pub = nh.advertise<std_msgs::Srring>("TopicY", 10);`
#### 初始化的含义:
pub = nh.advertise是一个拷贝初始化, nh.advertise返回Publisher类型的对象, 用于指定pub将要发布的消息类型为std_msgs::String, 指定pub发布的话题名为TopicY, 发布消息的缓存队列为10, 即指定的发送速度大于ROS底层可支持的发布速度时, 最多缓存10条信息, 超过10条是会丢弃最旧的信息
#### 方法:publish:`pub.publish(msg);`可以把msg交给ros底层去发送

##### 发布者例:
``` cpp

#include <ros/ros.h>
#include <std_msgs/String.h>

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "yeah_node");
    std::cout << "Topic started";

    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::String>("TopicY", 10);

    ros::Rate loop_rate(5);

    while(ros::ok)
    {
        std::cout << "Topic going on" << std::endl;
        std_msgs::String msg;
        msg.data = "YEAHHH ROS!"
        pub.publish(msg);

        loop_rate.sleep();
    }
    return 0;
}

```

### Subscriber
#### 类型:一个类, 内部持有话题名, 消息类型, 回调队列等状态, 提供回调注册与消息处理机制
#### 初始化:`ros::Subscriber sub = nh.subscribe("TopicY", 10, callback)
#### 初始化的含义:
nh.subscribe()返回Subscriber类型的对象, 其订阅的话题名为"TopicY", 缓存的长度为10, 会丢弃先到达的超出数据包, callback是需要自己定义的回调函数
#### 方法:回调机制:
##### 当TopicY上有消息到达时, ros底层就会自动接收消息, 并存进队列等待回调函数处理
##### 每在循环里调用一次ros::spinOnce, 就会执行一次回调函数callback(type msg)


##### 接收者例:

``` cpp

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>

//回调函数
void Sub_Callback(std_msgs::String msg)
{
    std::stringstream ss;
    ss << "Sub_Callback is working: " << msg.data;
    std::cout << ss.str() << std::endl;

    ROS_INFO("%s", msg.data.c_str());
}

void Sub_Callback_Pub2(std_msgs::String msg)
{
    std::stringstream ss;
    ss << "Sub_Callback_Pub2 is working: " << msg.data;
    std::cout << ss.str() << std::endl;

    ROS_INFO("%s", msg.data.c_str());
}

int main(int argc, char **argv)
{
    std::cout << "Rosetest Sub is listening" << std::endl;

    ros::init(argc, argv, "rosetestsub_pkg_node");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("TopicY", 10, Sub_Callback);
    ros::Subscriber sub2 = nh.subscribe("Topic2", 10, Sub_Callback_Pub2);

    while (ros::ok())
    {
        ros::spinOnce();
    }
    return 0;
}

```







### Message
#### std_msgs::String
#### 类型:std_msgs::String是消息类, data是其一个成员变量
#### 赋值:std_msgs::String msg; msg.data = "YEAH";即可为msg赋值

#### ROS_INFO日志信息打印
##### C语言格式化显示风格:(.c_str可以把string转为字符数组)
    ROS_INFO("%s", msg.data.c_str);
##### 流式显示:(可以直接显示string)
    ROS_INFO_STREAM(msg.data);

#### ROS_WARN打印
    ROS_WARN(msg.data.c_str)
