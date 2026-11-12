


## 第一章ros概述
 ## 1.1ros简介

ros=通信（plumbing）+tools+capabilities+ecosystem
，精简（
ros设计目标：代码复用，分布式（nodes，节点，进程），松耦合（行业标准使不同的功能都能接入）......
# 1.2ros快速体验

实现流程：
1.创建工作空间
mkdir -p 自定义空间名称/src
cd 名称
catkin_make

2.创建功能包
cd src
catkin_create_pkg 自定义ros包名 roscpp rospy std_msgs


3..编辑源文件
src——ros包名——src——创建源文件.cpp
```#include "ros/ros.h"

int main(int argc, char *argv[])
{
    std::setlocale(LC_ALL, "C.UTF-8");
    
    // 1. 初始化 ROS
    ros::init(argc, argv, "节点名称");
    //无const
    

    // 2. 创建节点句柄，供后续发布、订阅等操作使用
    ros::NodeHandle nh;

    // 3. 在这里写你希望程序完成的事情
    ROS_INFO("hello world!");

    // 4. 正常结束程序
    return 0;
}
```

4.编辑配置文件
打开`CMakeLists.txt`
搜索 `add_executable`，找到对应的配置，取消注释并填写程序名、源文件路径
add_executable(可执行程序名 源文件路径)
搜索 `target_link_libraries`，取消对应配置的注释，并填写相同的程序名target_link_libraries(程序名
  ${catkin_LIBRARIES}
)
按 `Ctrl + S` 保存
回到工作空间根目录，执行 `catkin_make`

5.编译并执行
新开窗口启动核心：roscore
旧窗口
rosrun 包名 程序名
# 1.3launch
在功能包下面新建launch文件夹
在launch文件夹下面新建文件 xxx.launch
在xxx.launch里编辑
```
<launch>
<node 
pkg="节点所属功能包名" 
type="程序名" 
name="节点名" 
output="screen"/>
(选填：args="xxxx"（传入参数），machine="机器名"（在指定机器上启动节点），respawn="true"(退出自动重启，防止意外退出)，respawn_delay="s"(先设置respawn，然后延迟s秒启动)，required="true"(如果当前节点关闭了，则整个ros都被关闭)，ns="xxx"(设置命名空间)，clear_params="true"(在启动前，删除节点的私有空间的所有参数))
</launch>
```
在终端里:
roslaunch  所属功能包名   launch文件

<launch deprecated>：弃用声明
<include file="$(find 包名)/xxx/xxx(要包含的功能包的路径)“/>

# 命令
roscore：启动rosmaster，参数服务器，日志节点
rosrun：rosrun 包名 可执行文件名
roslaunch：roslaunch 包名 文件名
catkin_create_pkg 自定义包名  依赖包：创建新的功能包
sudo apt install xxx:安装功能包
sudo apt purge xxx：删除功能包
rospack list：列出功能包
rospack find 包名：查找此包是否存在，如果存在则返回安装路径
rosls 包名：列出此包下的文件
apt search xxx：搜索
rosed 包名  文件名：编辑
roscd xxx：进入 
# 计算图
rqt_graph

## ros的通信机制 

三种实现策略：
话题通信（发布订阅）
服务通信（请求响应）
参数服务器（参数共享）

# 话题通讯：适用于不断更新的数据传输
1.角色：
master:管理者
takler：发布者
listener:订阅者
2.流程：
双方均给出话题给master，发布者提供RCP地址给master，话题相同master则提供RCP地址给listener，listener根据RCP地址找到tacker，talker提供调TCP地址给listener，listener通过TCP地址访问talker
3.注意
地址有RPC和TCP
talker和listener可以有多个
连接好后master可关闭
上述流程已封装好了，之后调用即可

## 4.实操：
......
### 发布方：
创建源文件
编辑源文件
```
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>
using namespace std;
using namespace ros;

int main(int argc, char *argv[])
{
   setlocale(LC_ALL, ""); 
   ros::init(argc, argv, "demo01_pub");//“节点名称”
   ros::NodeHandle nh;
   //创建发布者对象：
   ros::Publisher pub = nh.advertise<std_msgs::String>("fang",10);//参数：话题，最大保存消息数，超出时，先传进的先销毁
   //编写发布逻辑并发布数据：
   //创建被发布的消息：
   std_msgs::String msg;  //需要写std_msgs：：
//编写循环，循环发布消息：
Rate rate(10);    //  创建一个频率rate10HZ
int count=0;
Duration(3).sleep; //防止消息
while (ros::ok())   //ros::ok() 用来判断程序是否应该继续运行
{
    count++;
    stringstream ss;  
//stringstream用于加工不同类型的数据为字符串，string也可以，不过拼接数字时，通常需要先转换：
string result = text + to_string(count); 
    ss << "hello world" << count;//右边传入左边
    msg.data = ss.str();
    pub.publish(msg);
    ROS_INFO("发布的数据：%s", msg.data.c_str());
    rate.sleep();   //调用rate
}
return 0;
}
```
编辑配置文件

### 订阅方：
```
//#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>
#include <ros/node_handle.h>
#include <ros/subscriber.h>
using namespace std;
using namespace ros;

void doMsg(const std_msgs::String::ConstPtr &msg) //Constptr为智能指针，通常无须自己delete，用&避免复制一份
{
    ROS_INFO("Received msg: %s", msg->data.c_str());
}
int main(int argc, char *argv[])
{
    setlocale(LC_ALL, ""); 
    setlocale(LC_ALL, "");
    init(argc, argv, "demo02_sub");
    NodeHandle nh;
    Subscriber sub = nh.subscribe<std_msgs::String>("fang", 10, doMsg); //这里的 doMsg 告诉 ROS：收到消息后，用这个函数处理。
    ros::spin();  // 停留在这里，持续处理回调
    return 0;
}



```

编辑配置文件

## 控制器内部代码：
## simple_chassis_controller.h
```//
// Created by qiayuan on 2/6/21.
//

#ifndef SIMPLE_CHASSIS_CONTROLLER_SIMPLE_CHASSIS_CONTROLLER_H
#define SIMPLE_CHASSIS_CONTROLLER_SIMPLE_CHASSIS_CONTROLLER_H
//头文件保护，防止重复定义
#include <controller_interface/controller.h>
#include <hardware_interface/joint_command_interface.h>

namespace simple_chassis_controller {

class SimpleChassisController : public  //继承controller_interface::Controller<hardware_interface::EffortJointInterface> {
 public:
  SimpleChassisController() = default; //默认构造函数
  ~SimpleChassisController() override = default;  //默认析构函数

  bool init(hardware_interface::EffortJointInterface *effort_joint_interface,
            ros::NodeHandle &root_nh, ros::NodeHandle &controller_nh) override; //声明init函数

  void update(const ros::Time &time, const ros::Duration &period) override; //声update函数
  hardware_interface::JointHandle 
  front_left_joint_, 
  front_right_joint_,
   back_left_joint_, 
   back_right_joint_; //声明变量
 private:
  int state_{};  //{}使其初始化为0，表示_cmd里的行数
  ros::Time last_change_;
};
}// namespace simple_chassis_controller

#endif //SIMPLE_CHASSIS_CONTROLLER_SIMPLE_CHASSIS_CONTROLLER_H

```
## simple_chassis_controller.cpp
```//
// Created by qiayuan on 2/6/21.
//

#include "simple_chassis_controller/simple_chassis_controller.h"
#include <pluginlib/class_list_macros.hpp>  //引入 ROS 的 pluginlib 宏。文件最后会用它把控制器注册成一个插件，使控制器管理器能够加载它

namespace simple_chassis_controller //命名空间，避免和其他代码中的同名类发生冲突
{
bool SimpleChassisController::init(hardware_interface::EffortJointInterface *effort_joint_interface,ros::NodeHandle &root_nh, ros::NodeHandle &controller_nh)  //init 是初始化控制器，控制器被加载时调用，true 表示初始化成功，
                   root_nh: ROS 根节点句柄，可用于访问话题、参数等
                   controller_nh: 控制器自己的节点句柄，常用于读取控制器参数
      {
  front_left_joint_ =
      effort_joint_interface->getHandle("left_front_wheel_joint");
  front_right_joint_ =
      effort_joint_interface->getHandle("right_front_wheel_joint");
  back_left_joint_ = 
      effort_joint_interface->getHandle("left_back_wheel_joint");
  back_right_joint_ =
      effort_joint_interface->getHandle("right_back_wheel_joint");
//getHandle：ROS提供的获取各个轮子的句柄
  return true;
      }

void SimpleChassisController::update(const ros::Time &time, const ros::Duration &period)  //time：当前时间，period：距离上一次更新过去的时间，预设六个动作的力矩
      {
  double tau = 1.0;  // Temporary simulation test; original example uses 0.2.
  // NOTE: DON'T COPY THESE NAIVE TESTING CODES !!! USE INVERSE KINEMATICS !!!
  // NOTE: DON'T COPY THESE NAIVE TESTING CODES !!! USE INVERSE KINEMATICS !!!
  // NOTE: DON'T COPY THESE NAIVE TESTING CODES !!! USE INVERSE KINEMATICS !!!
  static double cmd_[6][4] = {{tau, tau, tau, tau}, //forward
                              {-2 * tau, -2 * tau, -2 * tau, -2 * tau},//backward
                              {-tau, tau, tau, -tau}, //  left
                              {2 * tau, -2 * tau, -2 * tau, 2 * tau}, //right
                              {2 * tau, -2 * tau, 2 * tau, -2 * tau}, //clockwise
                              {-tau, tau, -tau, tau}  //counterclockwise
                              };  
  if ((time - last_change_).toSec() > 2) 
   {
    state_ = (state_ + 1) % 6;
    last_change_ = time;
   }  //每隔约两秒切换一组动作，toSec:ROS提供的转换成秒的函数，state：当前cmd_选择的行数，%6:使得在0-5之间循环
  front_left_joint_.setCommand(cmd_[state_][0]);
  front_right_joint_.setCommand(cmd_[state_][1]);
  back_left_joint_.setCommand(cmd_[state_][2]);
  back_right_joint_.setCommand(cmd_[state_][3]);
}
//从当前动作 state_ 对应的那一行里，取出第[]值，作为某个轮子的命令，即：左前，右前，左后，右后
PLUGINLIB_EXPORT_CLASS(simple_chassis_controller::SimpleChassisController, controller_interface::ControllerBase);
//这行把 SimpleChassisController导出为一个控制器插件。配合插件描述文件和配置文件，控制器管理器才能找到并加载它
}

```

## 运行流程
1.hero.launch
启动 Gazebo、加载机器人模型
2.hero.urdf.xacro
告诉 Gazebo 加载仿真控制插件
3.rm_robot_hw_sim.cpp
插件运行，Gazebo 中出现 controller_manager
4.load_controller_for_task.launch
加载配置，并请求启动控制器
 5.controllers.yaml
指定要加载的控制器类型
6.package.xml + simple_chassis_controller_plugins.xml
让 pluginlib 找到控制器对应的类和库
7.simple_chassis_controller.cpp
先执行 init()，运行时反复执行 update()
轮子收到力矩命令

## 逆运动学和PID控制轮速

逆运动学：
ωᵢ = sᵢ × [vx + kᵢvy + (−yᵢ + kᵢxᵢ)w] ÷ rᵢ
vx、vy整车在底盘坐标系下的前后、左右速度
w整车的旋转角速度
xᵢ、yᵢ第 `i` 个轮子相对底盘中心的位置
rᵢ第 `i` 个轮子的半径
kᵢ与该轮滚子倾斜方向和角度有关的系数
sᵢ关节转动正方向的修正系数，通常取 +1 或 -1


目标轮速：我们希望轮子转多快
实际轮速：仿真中轮子现在转多快
力矩：控制器施加给轮子的转动力，用来影响轮子的转动

误差=目标-实际

P:比例项，eg：P=0.5，P 项输出 = 0.5 × 误差
I：积分项，随时间增长
D：微分项，误差变化多快

流程：
初始化PID
收到整车速度指令 /cmd_vel
逆运动学算出四个目标轮速
在头文件中声明PID
获取实际轮速
计算误差
用PID计算力矩发送力矩


## pluginlib 插件库

概念：是一个c+=库，用来从ros功能包里加载和卸载插件，动态加载插件

可以把它理解为：
**基类**：制定规则，规定插件需要提供哪些功能。
**插件类**：按照规则实现具体功能。
**插件库**：将插件类编译成动态库，例如 `libplus.so`。
**加载程序**：通过 pluginlib 找到插件，并创建插件对象。

这次的例子中：
基类：`DBX_Base`
三角形插件：`SanBian`
四边形插件：`SiBian`
插件库：`libplus.so`
加载程序：`use_plus`

使用实例：
 1.准备：  
创建功能包xxx并导入依赖rsocpp pluginlib

```
demo03_plugin/
├── CMakeLists.txt
├── package.xml
├── plus.xml
├── include/
│   └── demo03_plugin/
│       ├── dbx_base.h
│       └── dbx_plugins.h
└── src/
    ├── plus.cpp
    └── use_plus.cpp
```

2.创建基类,并编辑：  
在创建的功能包下的include下的xxx下创建c++头文件.h，所有插件类都要继承此基类

```
#ifndef DEMO03_PLUGIN_DBX_BASE_H
#define DEMO03_PLUGIN_DBX_BASE_H

namespace dbx_base_ns
{

class DBX_Base
{
protected:
    // 无参构造函数。
    // 子类创建对象时，需要能够调用基类的无参构造函数。
    DBX_Base() = default;

public:
    // 虚析构函数。
    // 通过基类指针销毁子类对象时，确保子类资源正确释放。
    virtual ~DBX_Base() = default;

    // 初始化边长：由子类实现。
    virtual void init(double side_length) = 0;

    // 计算周长：由子类实现。
    virtual double getlength() = 0;
};

}  

#endif  // DEMO03_PLUGIN_DBX_BASE_H
```

3.创建插件类，并编写：  
在创建的功能包下的include下的xxx下创建c++头文件

```
#ifndef DEMO03_PLUGIN_DBX_PLUGINS_H
#define DEMO03_PLUGIN_DBX_PLUGINS_H

#include "demo03_plugin/dbx_base.h"

namespace dbx_plugins_ns
{

// 正三角形插件：继承 DBX_Base。
class SanBian : public dbx_base_ns::DBX_Base
{
private:
    double side_length;

public:
    // 无参构造函数。
    SanBian() : side_length(0.0)//创建 SanBian 对象时，把成员变量 side_length初始化为 0.0
    {
    }

    // 设置边长。
    void init(double side_length) override
    {
        this->side_length = side_length;
    }

    // 计算正三角形周长。
    double getlength() override
    {
        return side_length * 3;
    }
};

// 正方形插件：继承 DBX_Base。
class SiBian : public dbx_base_ns::DBX_Base
{
private:
    double side_length;

public:
    // 无参构造函数。
    SiBian() : side_length(0.0)
    {
    }

    // 设置边长。
    void init(double side_length) override
    {
        this->side_length = side_length;
    }

    // 计算正方形周长。
    double getlength() override
    {
        return side_length * 4;
    }
};

}  // namespace dbx_plugins_ns

#endif  // DEMO03_PLUGIN_DBX_PLUGINS_H
```

4.注册插件：  
在src下新建.cpp文件

```
#include <pluginlib/class_list_macros.hpp>

#include "demo03_plugin/dbx_base.h"
#include "demo03_plugin/dbx_plugins.h"

// 注册正三角形插件。
// 参数 1：插件类
// 参数 2：插件类继承的基类
PLUGINLIB_EXPORT_CLASS(        //注册工具
    dbx_plugins_ns::SanBian,   //把SanBian作为DBX_Base的插件导出
    dbx_base_ns::DBX_Base
)

// 注册正方形插件。
PLUGINLIB_EXPORT_CLASS(
    dbx_plugins_ns::SiBian,
    dbx_base_ns::DBX_Base
)
```

5.构建插件库：  
在CMakelists.txt里修改

1.查找依赖

```
find_package(catkin REQUIRED COMPONENTS
  roscpp
  pluginlib
)
```

2.指定头文件搜索目录

```
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)
```

//include：当前功能包自己的头文件目录,${catkin_INCLUDE_DIRS}：ROS 依赖提供的头文件目录。

3.生成插件动态库

```
add_library(plus SHARED
  src/plus.cpp
)

target_link_libraries(plus
  ${catkin_LIBRARIES}
)
```

//  
plus：动态库目标的名称。  
SHARED：明确要求生成动态库。  
src/plus.cpp：用于生成该动态库的源文件。  
${catkin_LIBRARIES}：链接 ROS 依赖所需的库。  
在 Linux 下，这里生成的动态库通常叫：libplus.so

4.生成加载插件的程序

```
add_executable(use_plus
  src/use_plus.cpp
)

target_link_libraries(use_plus
  ${catkin_LIBRARIES}
)
```

6.使插件可用于ROS工具链：  
1.配置xml  
在src-src下新建.xml文件

```
<library
    <class type="dbx_plugins_ns::SanBian" base_class_type="dbx_base_ns::DBX_Base">
        <description>这是正三边形插件</description>
    </class>
    <class type="dbx_plugins_ns::SiBian" base_class_type="dbx_base_ns::DBX_Base">
        <description>这是正四边形插件</description>
    </class>
</library>
```

library path="lib/libplus"`：指定插件所在的动态库。  
type：插件类的完整名称。  
base_class_type：插件继承的基类完整名称。  
description：对插件的文字说明。

2.导出插件描述文件  
在package.xml结尾的两个中间添加  
<demo03_plugin plugin="${prefix}/plus.xml" />

demo03_plugin：本例中加载器使用的基类所属功能包名。  
${prefix}：功能包所在目录。  
plus.xml：插件描述文件。

7.使用插件：  
在src-src下新建.cpp文件

```
#include <ros/ros.h>
#include <pluginlib/class_loader.h>

#include "demo03_plugin/dbx_base.h"

#include <clocale>

int main(int argc, char *argv[])
{
    // 使用当前环境的本地化设置，便于终端正常显示中文。
    setlocale(LC_ALL, "");

    // 初始化 ROS 节点。
    ros::init(argc, argv, "use_plus");

    // 创建插件加载器。
    //
    // 模板参数：插件基类的 C++ 类型。
    // 参数 1：基类所属的功能包名。
    // 参数 2：基类的完整名称。
    pluginlib::ClassLoader<dbx_base_ns::DBX_Base> loader(
        "demo03_plugin",
        "dbx_base_ns::DBX_Base"
    );        //创建加载器对象并调用此类的构造函数（因为ClassLoader里面有构造函数）并传入两个参数
    // 创建正三角形插件对象。
    boost::shared_ptr<dbx_base_ns::DBX_Base> san =
        loader.createInstance("dbx_plugins_ns::SanBian");
        //让加载器 loader`创建一个 SanBian三角形插件对象，把返回的智能指针保存到 san 中，之后通过 san 按照 DBX_Base基类规定的接口使用这个对象。createInstance()是 pluginlib 的 ClassLoader 类提供的成员函数，它会根据插件描述信息找到对应的动态库和插件类，然后创建插件对象，
    // 设置边长为 10
    san->init(10);

    // 调用插件对象的方法，计算周长。
    double length = san->getlength();

    // 输出结果。
    ROS_INFO("三角形周长：%.2f", length);

    return 0;
}
```

8.执行：  
编译ros1工作空间  
进入ros容器输入  
source ~/catkin_ws/devel_cb/setup.bash  
rosrun demo03_plugin use_plus




## 参数服务器
# 概念
以共享的方式实现不同节点之间数据交互的通信模式
（最好存储静态的非二进制的简单数据）


talker向参数服务器发送参数，ros master将参数保存到参数列表中
listener通过RPC向参数服务器发送查找请求
ros master查找参数值，并通过RPC发送给listener

操作流程：
新建功能包，新建.cpp文件
```#include <ros/ros.h>
int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "get_param_c");
    ros::NodeHandle nh;
    //参数查询
    //nh大方案：
    //方案1 param
    double radius=nh.param("radius",0.5);
    ROS_INFO("radius: %.2f",radius);
//若名称存在返回对应值，若不存在则返回默认值（自己写的）
    //方案2 get param
    double radius2=0.0;
    bool result=nh.getParam("radius",radius2);
    if(result)
    {
        ROS_INFO("radius2: %.2f",radius2);
    }
    else {
        ROS_INFO("查询的变量不存在");
    }
    //方案3  getParamCached
    double radius3=0.0;
    bool result1=nh.getParamCached("radius",radius3);
    if(result1)
    {
        ROS_INFO("radius2: %.2f",radius2);
    }
    else {
        ROS_INFO("查询的变量不存在");
    }
    //相比getParam性能有提升，如果之前缓存过则更快一点
    //方案2 getParamNames
    std::vector<std::string> names;
    nh.getParamNames(names);
    for (auto &&name:names) {
        ROS_INFO("遍历元素%s",name.c_str());
    }
    //方案5 hasParam
    bool result3=nh.hasParam("radius");
    bool result4=nh.hasParam("radiusxxx");
    ROS_INFO("radius 存在吗？%d",result3);
    ROS_INFO("radiusxxx 存在吗？%d",result4);
    //方案6 searchParam
    std::string key;
    nh.searchParam("radius",key);
    ROS_INFO("搜索结果：%s",key.c_str());
    return 0;
    //param大方案，整体和nh的差不多，只是前缀改为ros：：param：：
}
```


## rosmsg


rosmsg show :显示消息描述
rosmsg info：显示消息信息
rosmsg list：列出所有消息
rosmsg md5：加密后的消息
rosmsg package：显示某个功能包下的所有信息
rosmsg packages ：列出包含信息的功能包


## rosparam

rsoparam set:设置参数
rsoparam get:获取参数
rsoparam load:从外部文件加载参数
rsoparam dump:将参数写入磁盘文件
rsoparam delete删除参数
rsoparam list:列出所有参数




## 复习总流程

# 功能包：
```
hero_chassis_controller/
├── include/
│   └── hero_chassis_controller/
│       └── hero_chassis_controller.h   控制器的“声明”
├── src/
│   └── hero_chassis_controller.cpp     控制器的“实现”
├── config/                             控制器参数
├── launch/                             启动配置
├── CMakeLists.txt                      编译配置
├── package.xml                         功能包信息和依赖
└── 插件描述 XML                         告诉 ROS 怎样找到插件
```

# 头文件：声明类以及成员
```
控制器
├── 四个轮子的关节句柄
├── 四个 PID 控制器
├── 目标速度等数据
├── init() 函数
└── update() 函数
```

# 源文件：控制器具体怎么工作
```
init()
├── 获取轮子关节句柄
├── 读取需要的参数
└── 初始化 PID

update()
├── 获取目标轮速
├── 读取实际轮速
├── 计算误差
├── 通过 PID 计算力矩
└── 向轮子发送力矩
```

### 配置和插件文件：让控制器能被使用

- `config/`：存放需要从外部读取的配置，比如 PID 参数。
- `CMakeLists.txt` 和 `package.xml`：告诉编译工具需要哪些依赖、生成什么。
- 插件描述 XML：让控制器管理器能够找到你的控制器插件。
- `launch/`：帮助启动或加载相关节点和控制器。


## 动态参数

概念：一种可以在运行时更新参数而无须重启节点的参数配置

rqt_reconfigure 修改参数
    ↓
dynamic_reconfigure 服务端收到新值
    ↓
调用 cb(config, level)
    ↓
通过 config.int_param 读取并输出新值



客户端：
1.新建功能包
2.新建目录cfg
3.在目录下新建 .cfg文件
4.在此文件中打开终端输入：chmod +x *.cfg
5.在CMakelists中：
generate_dynamic_reconfigure_options(
         cfg/dr.py
 )
 6.编译cfg文件（可先换成.py文件方便补齐,最后再改回来）：
 ```#!/usr/bin/env python3
// 当这个脚本被直接执行时，让系统通过 env找到 python3来运行它
from dynamic_reconfigure.parameter_generator_catkin import *     //导包
gen = ParameterGenerator()       //创建参数配置生成器名字叫gen
gen.add("int_param",int_t,0,"整数参数",10,1,100)    //定义一个叫 int_param 的整数参数，默认值为 10，允许范围为 1～100，说明文字是“整数参数”
exit(gen.generate("demo02_dr","dr_client","dr"))   
//配置节点并退出 ："demo02_dr"：功能包名称。
"dr_client"：生成配置时使用的节点名称信息，不会因此启动一个叫这个名字的节点
"dr"：配置名称，应与 `dr.cfg` 的文件名对应，用于生成 drConfig.h
 ```

服务端：
```

#include "ros/ros.h"
#include "dynamic_reconfigure/server.h"
#include "demo02_dr/drConfig.h"

void cb(demo02_dr::drConfig &config, uint32_t level) {
    ROS_INFO("修改后的整型数据是:%d " , config.int_param );
}       //回调函数demo02_
dr::drConfig：生成的参数配置类型，&config：用引用接收配置，可以读取或修改其中的参数，uint32_t level：接收参数变化的级别标记，目前代码没有使用它，config.int_param：读取这次配置中的整数参数

int main(int argc, char **argv) {
setlocale(LC_ALL, "");
ros::init(argc, argv, "dr01_server");
dynamic_reconfigure::Server<demo02_dr::drConfig> server;
    //创建一个叫 server 的动态参数服务端，使用 drConfig 定义的参数dynamic_reconfigure::Server   ：动态参数服务端类
server.setCallback(boost::bind(&cb, _1, _2));
    // 把 cb()注册为这个服务端的回调函数，创建一个可以被调用的对象，并安排好参数如何传给 cb()，_1：调用时传入的第一个参数，对应 config。
_2：调用时传入的第二个参数，对应 level
ros::spin();
return 0;
}
```

流程：
编译CMake命名空间
终端：rqt——plugins——configuration——dynamic reconfigure
rosparam get  /xxxxx/xxxxx/xxxx

启动底盘：roslaunch rm_description_for_task hero.launch
启动控制器：roslaunch hero_chassis_controller load_hero_controller.launch
启动曲线：rqt_plot '/joint_states/velocity[1]'
检查控制器状态：rosservice call /controller_manager/list_controllers
查看轮子状态：rostopic echo -n 1 /joint_states

## rqt

查看节点和话题关系：rosnode list、rostopic list等对应rqt_graph：Plugins → Introspection → Node Graph
查看话题数据：rostopic echo等等对应Topic Monitor：Plugins → Topics → Topic Monitor
             数字也可以用 rqt_plot画曲线：Plugins → Visualization → Plot
调整动态参数：可以通过动态参数相关命令或程序对应rqt_reconfigure：**Plugins → Configuration → Dynamic Reconfigure**
调用服务：rosservice call对应Service Caller：Plugins → Services → Service Caller




## /cmd_vel 接收、四轮目标速度计算，以及 Gazebo 测试:

1.修改 `package.xml`
添加：
<depend>geometry_msgs</depend>   //提供速度消息 `Twist`
<depend>realtime_tools</depend>   //在话题回调和控制器更新之间传递数据
2.修改 `CMakeLists.txt`
找到 `find_package` 中的依赖列表，加入两行：

```
find_package(catkin REQUIRED
        COMPONENTS
        roscpp
        geometry_msgs
        realtime_tools
        sensor_msgs
        roslint
        controller_interface
        hardware_interface
        pluginlib
        control_toolbox
)
```

再找到 `catkin_package` 中的 `CATKIN_DEPENDS`，同样加入：

```
        CATKIN_DEPENDS
        roscpp
        geometry_msgs
        realtime_tools
        sensor_msgs
        controller_interface
        hardware_interface
        pluginlib
        control_toolbox
```

4.修改头文件:
```#ifndef HERO_CHASSIS_CONTROLLER_H
#define HERO_CHASSIS_CONTROLLER_H

#include <control_toolbox/pid.h>
#include <controller_interface/controller.h>
#include <geometry_msgs/Twist.h>
#include <hardware_interface/joint_command_interface.h>
#include <realtime_tools/realtime_buffer.h>

namespace hero_chassis_controller
{

    class HeroChassisController
        : public controller_interface::Controller<
              hardware_interface::EffortJointInterface>
    {
    public:
        bool init(hardware_interface::EffortJointInterface* effort_joint_interface,
                  ros::NodeHandle& root_nh,
                  ros::NodeHandle& controller_nh) override;

        void update(const ros::Time& time,
                    const ros::Duration& period) override;

    private:
        // 保存底盘速度指令及收到指令的时间。
        struct VelocityCommand
        {
            double vx = 0.0;
            double vy = 0.0;
            double wz = 0.0;
            ros::Time stamp;
        };

        // 收到 /cmd_vel 消息时执行的回调函数。
        void commandCallback(const geometry_msgs::TwistConstPtr& msg);

        // 速度指令订阅器。
        ros::Subscriber command_sub_;

        // 在回调函数和控制器更新之间传递速度指令。
        realtime_tools::RealtimeBuffer<VelocityCommand> command_buffer_;

        // 轮子半径和底盘半尺寸，单位：米。
        double wheel_radius_ = 0.07625;
        double half_wheelbase_ = 0.2;
        double half_track_ = 0.2;

        // 指令超过这个时间没有更新时，目标速度归零，单位：秒。
        double command_timeout_ = 0.5;

        // 左前轮关节句柄，后面通过它读取轮速、发送力矩。
        hardware_interface::JointHandle front_left_joint_;
        // 左前轮对应的 PID 控制器。
        control_toolbox::Pid front_left_pid_;

        // 右前轮关节句柄和 PID 控制器。
        hardware_interface::JointHandle front_right_joint_;
        control_toolbox::Pid front_right_pid_;

        // 左后轮关节句柄和 PID 控制器。
        hardware_interface::JointHandle back_left_joint_;
        control_toolbox::Pid back_left_pid_;

        // 右后轮关节句柄和 PID 控制器。
        hardware_interface::JointHandle back_right_joint_;
        control_toolbox::Pid back_right_pid_;
    };

}  // namespace hero_chassis_controller

#endif  // HERO_CHASSIS_CONTROLLER_H
```
5.修改 `hero_chassis_controller.cpp`
```//
// Created by qi on 2026/11/2.
//

#include "hero_chassis_controller/hero_chassis_controller.h"

#include <algorithm>
#include <cmath>
#include <pluginlib/class_list_macros.hpp>

namespace hero_chassis_controller
{

    bool HeroChassisController::init(
        hardware_interface::EffortJointInterface* effort_joint_interface,
        ros::NodeHandle& root_nh,
        ros::NodeHandle& controller_nh)
    {
        // 获取四个轮子的关节句柄。
        front_left_joint_ =
            effort_joint_interface->getHandle("left_front_wheel_joint");
        front_right_joint_ =
            effort_joint_interface->getHandle("right_front_wheel_joint");
        back_left_joint_ =
            effort_joint_interface->getHandle("left_back_wheel_joint");
        back_right_joint_ =
            effort_joint_interface->getHandle("right_back_wheel_joint");

        // 为每个 PID 创建对应的参数位置。
        ros::NodeHandle front_left_pid_nh(controller_nh, "front_left_pid");
        ros::NodeHandle front_right_pid_nh(controller_nh, "front_right_pid");
        ros::NodeHandle back_left_pid_nh(controller_nh, "back_left_pid");
        ros::NodeHandle back_right_pid_nh(controller_nh, "back_right_pid");

        // 使用参数服务器中的配置初始化四个 PID。
        if (!front_left_pid_.init(front_left_pid_nh))
        {
            ROS_ERROR("左前轮 PID 参数初始化失败");
            return false;
        }

        if (!front_right_pid_.init(front_right_pid_nh))
        {
            ROS_ERROR("右前轮 PID 参数初始化失败");
            return false;
        }

        if (!back_left_pid_.init(back_left_pid_nh))
        {
            ROS_ERROR("左后轮 PID 参数初始化失败");
            return false;
        }

        if (!back_right_pid_.init(back_right_pid_nh))
        {
            ROS_ERROR("右后轮 PID 参数初始化失败");
            return false;
        }

        // 读取底盘尺寸和指令超时时间；未配置时使用给定默认值。
        controller_nh.param("wheel_radius", wheel_radius_, 0.07625);
        controller_nh.param("half_wheelbase", half_wheelbase_, 0.2);
        controller_nh.param("half_track", half_track_, 0.2);
        controller_nh.param("command_timeout", command_timeout_, 0.5);

        // 防止无效的轮径和超时时间。
        if (!std::isfinite(wheel_radius_) || wheel_radius_ <= 0.0 ||
            !std::isfinite(half_wheelbase_) || half_wheelbase_ <= 0.0 ||
            !std::isfinite(half_track_) || half_track_ <= 0.0 ||
            !std::isfinite(command_timeout_) || command_timeout_ <= 0.0)
        {
            ROS_ERROR("底盘尺寸或指令超时时间无效");
            return false;
        }

        // 初始指令为零：没有收到 /cmd_vel 时不会自动运动，使其不会保留上一次结束时的数据
        command_buffer_.writeFromNonRT(VelocityCommand{});

        // 订阅根命名空间下的 /cmd_vel 话题。
        command_sub_ = root_nh.subscribe(
            "/cmd_vel",
            1,    //缓存区为1
            &HeroChassisController::commandCallback,
            this);

        return true;
    }

    void HeroChassisController::commandCallback(
        const geometry_msgs::TwistConstPtr& msg)
    {
        // 忽略不是有效数字的指令。
        if (!std::isfinite(msg->linear.x) ||
            !std::isfinite(msg->linear.y) ||
            !std::isfinite(msg->angular.z))
        {
            ROS_WARN("收到无效的底盘速度指令");
            return;
        }

        VelocityCommand command;
        command.vx = msg->linear.x;
        command.vy = msg->linear.y;
        command.wz = msg->angular.z;
        command.stamp = ros::Time::now();

        // 将最新指令交给控制器更新函数。
        command_buffer_.writeFromNonRT(command);
    }

    void HeroChassisController::update(
        const ros::Time& time,
        const ros::Duration& period)
    {
        // 默认目标为零：没有新指令或指令超时时停车。
        double vx = 0.0;
        double vy = 0.0;
        double wz = 0.0;

        const VelocityCommand* command = command_buffer_.readFromRT();

        if (command != nullptr && !command->stamp.isZero())    //time：当前控制器更新时的时间，command->stamp：收到这条速度指令时记录的时间。time - command->stamp：两者相差多久。.toSec()：把这个时间差转换成秒。age：这条指令已经过了多少秒
        {
            const double age = (time - command->stamp).toSec();

            if (age >= 0.0 && age <= command_timeout_)
            {
                vx = command->vx;
                vy = command->vy;
                wz = command->wz;
            }
        }

        // 麦克纳姆轮逆运动学：底盘速度转换为四个轮子的目标角速度。
        const double rotation = (half_wheelbase_ + half_track_) * wz;

        const double front_left_target =
            (vx - vy - rotation) / wheel_radius_;
        const double front_right_target =
            (vx + vy + rotation) / wheel_radius_;
        const double back_left_target =
            (vx + vy - rotation) / wheel_radius_;
        const double back_right_target =
            (vx - vy + rotation) / wheel_radius_;

        // 读取四个轮子的实际角速度，并计算轮速误差。
        const double front_left_error =
            front_left_target - front_left_joint_.getVelocity();
        const double front_right_error =
            front_right_target - front_right_joint_.getVelocity();
        const double back_left_error =
            back_left_target - back_left_joint_.getVelocity();
        const double back_right_error =
            back_right_target - back_right_joint_.getVelocity();

        // 每个轮子使用各自的 PID，根据误差计算力矩。
        double front_left_effort =
            front_left_pid_.computeCommand(front_left_error, period);
        double front_right_effort =
            front_right_pid_.computeCommand(front_right_error, period);
        double back_left_effort =
            back_left_pid_.computeCommand(back_left_error, period);
        double back_right_effort =
            back_right_pid_.computeCommand(back_right_error, period);

        // 临时限制每个轮子的力矩范围，便于在仿真中观察。
        front_left_effort = std::max(-1.0, std::min(front_left_effort, 1.0));
        front_right_effort = std::max(-1.0, std::min(front_right_effort, 1.0));
        back_left_effort = std::max(-1.0, std::min(back_left_effort, 1.0));
        back_right_effort = std::max(-1.0, std::min(back_right_effort, 1.0));

        // 向四个轮子的关节发送力矩。
        front_left_joint_.setCommand(front_left_effort);
        front_right_joint_.setCommand(front_right_effort);
        back_left_joint_.setCommand(back_left_effort);
        back_right_joint_.setCommand(back_right_effort);
    }
}  // namespace hero_chassis_controller

// 将自己的控制器注册成插件。
PLUGINLIB_EXPORT_CLASS(
    hero_chassis_controller::HeroChassisController,
    controller_interface::ControllerBase
)
```
6.在`config/controllers.yaml`中添加
找到：

```
  hero_chassis_controller:
    type: hero_chassis_controller/HeroChassisController
```

紧接着添加：

```
    wheel_radius: 0.07625
    half_wheelbase: 0.2
    half_track: 0.2
    command_timeout: 0.5
```

7.编译和启动

1. 保存所有文件。
    
2. 在 CLion 选择 **工具 → ROS1（Noetic 容器）→ 编译 ROS1 工作空间**。
    
3. 停止之前运行的控制器，再停止 Gazebo。
    
4. 重新启动 Gazebo：
    
    ```
    roslaunch rm_description_for_task hero.launch
    ```
    
5. 在另一个 ROS 容器终端启动控制器：
    
    ```
    roslaunch hero_chassis_controller load_hero_controller.launch
    ```

8.更改数据：
终端输入rqt
plugins——topics——meesage publisher
topics选择/cmd_vel
type选择geometry_msgs/Twist
点+，并更改数据
最后勾选





## /cmd_vel控制与里程计

```
通过 /cmd_vel 发送底盘速度指令
                 ↓
控制器接收 geometry_msgs/Twist 消息
                 ↓
读取 vx、vy、wz
                 ↓
逆运动学计算四个轮子的目标角速度
                 ↓
读取每个轮子的实际角速度
                 ↓
计算：目标角速度 - 实际角速度
                 ↓
每个轮子的 PID 根据误差计算力矩
                 ↓
向四个轮子发送力矩
```



# 流程：
接收 `/cmd_vel`

### 1. 添加依赖

#### `package.xml`

添加：

```
<depend>geometry_msgs</depend>
<depend>realtime_tools</depend>
```

- `geometry_msgs`：提供 `Twist` 速度消息。
- `realtime_tools`：用于在回调函数和控制器更新函数之间传递数据。

#### `CMakeLists.txt`

在下面两处依赖列表中添加：

```
geometry_msgs
realtime_tools
```

两处分别是：

```
find_package(catkin REQUIRED COMPONENTS
  # ...
)
```

和：

```
catkin_package(
  CATKIN_DEPENDS
  # ...
)
```

### 2. 在头文件中添加速度指令相关成员

```
#include <geometry_msgs/Twist.h>
#include <realtime_tools/realtime_buffer.h>
```

在控制器类的 `private:` 中添加：

```
// 保存底盘速度指令及收到指令的时间。
struct VelocityCommand
{
    double vx = 0.0;
    double vy = 0.0;
    double wz = 0.0;
    ros::Time stamp;
};

// 收到 /cmd_vel 消息时执行。
void commandCallback(const geometry_msgs::TwistConstPtr& msg);

// 速度指令订阅器。
ros::Subscriber command_sub_;

// 缓存区
realtime_tools::RealtimeBuffer<VelocityCommand> command_buffer_;

// 底盘尺寸，单位：米。
double wheel_radius_ = 0.07625;
double half_wheelbase_ = 0.2;
double half_track_ = 0.2;

// 指令超过这个时间没有更新时，目标速度归零，单位：秒。
double command_timeout_ = 0.5;
```

其中：

- `VelocityCommand`：把三种速度和收到消息的时间放在一起。
- `command_sub_`：负责订阅速度话题。
- `command_buffer_`：负责把回调收到的指令交给 `update()`。
- `command_timeout_`：用于判断指令是否已经过期。

### 3. 在 `init()` 中读取参数并订阅话题

```
// 读取底盘尺寸和指令超时时间；未配置时使用给定默认值。
controller_nh.param("wheel_radius", wheel_radius_, 0.07625);
controller_nh.param("half_wheelbase", half_wheelbase_, 0.2);
controller_nh.param("half_track", half_track_, 0.2);
controller_nh.param("command_timeout", command_timeout_, 0.5);

// 防止无效的底盘尺寸和超时时间,isfinite可以检测是否为有限数
if (!std::isfinite(wheel_radius_) || wheel_radius_ <= 0.0 ||
    !std::isfinite(half_wheelbase_) || half_wheelbase_ <= 0.0 ||
    !std::isfinite(half_track_) || half_track_ <= 0.0 ||
    !std::isfinite(command_timeout_) || command_timeout_ <= 0.0)
{
    ROS_ERROR("底盘尺寸或指令超时时间无效");
    return false;
}

// 初始指令为零：没有收到 /cmd_vel 时不会自动运动。
command_buffer_.writeFromNonRT(VelocityCommand{});

// 订阅 /cmd_vel 话题。
command_sub_ = root_nh.subscribe(
    "/cmd_vel",
    1,
    &HeroChassisController::commandCallback,
    this);
```

参数读取的含义例如：

```
controller_nh.param("wheel_radius", wheel_radius_, 0.07625);
```

表示：

```
查找 wheel_radius 参数
        ↓
找到：保存到 wheel_radius_
没找到：使用默认值 0.07625
```

### 4. 添加速度回调函数

```
void HeroChassisController::commandCallback(
    const geometry_msgs::TwistConstPtr& msg)
{
    // 忽略不是有效数字的指令。
    if (!std::isfinite(msg->linear.x) ||
        !std::isfinite(msg->linear.y) ||
        !std::isfinite(msg->angular.z))
    {
        ROS_WARN("收到无效的底盘速度指令");
        return;
    }

    VelocityCommand command;

    // 读取底盘前后、左右和旋转速度。
    command.vx = msg->linear.x;
    command.vy = msg->linear.y;
    command.wz = msg->angular.z;

    // 记录收到消息的时间。
    command.stamp = ros::Time::now();

    // 把最新指令交给控制器更新函数。
    command_buffer_.writeFromNonRT(command);
}
```

这里的流程是：

```
收到 Twist 消息
        ↓
检查速度是否有效
        ↓
读取 vx、vy、wz
        ↓
记录时间
        ↓
放入 command_buffer_
```

### 5. 在 `update()` 中读取指令并判断超时

```
// 默认目标为零：没有新指令或指令超时时停车。
double vx = 0.0;
double vy = 0.0;
double wz = 0.0;

const VelocityCommand* command = command_buffer_.readFromRT();

if (command != nullptr && !command->stamp.isZero())
{
    // 当前时间 - 收到指令的时间。
    const double age = (time - command->stamp).toSec();

    if (age >= 0.0 && age <= command_timeout_)
    {
        vx = command->vx;
        vy = command->vy;
        wz = command->wz;
    }
}
```

理解：

- 一开始先把速度设成零。
- 如果存在有效且没有超时的指令，就使用收到的值。
- 否则保持零目标速度。

### 6. 通过逆运动学计算四轮目标速度

```
// 旋转速度对轮速的影响。
const double rotation = (half_wheelbase_ + half_track_) * wz;

// 计算四个轮子的目标角速度。
const double front_left_target =
    (vx - vy - rotation) / wheel_radius_;

const double front_right_target =
    (vx + vy + rotation) / wheel_radius_;

const double back_left_target =
    (vx + vy - rotation) / wheel_radius_;

const double back_right_target =
    (vx - vy + rotation) / wheel_radius_;
```

接着沿用之前的四轮 PID：

```
// 以左前轮为例：目标速度减去实际速度。
const double front_left_error =
    front_left_target - front_left_joint_.getVelocity();

// PID 根据误差计算力矩。
double front_left_effort =
    front_left_pid_.computeCommand(front_left_error, period);

// 临时限制力矩。
front_left_effort =
    std::max(-1.0, std::min(front_left_effort, 1.0));

// 向左前轮发送力矩。
front_left_joint_.setCommand(front_left_effort);
```

另外三个轮子按照相同流程处理，只是使用各自的目标值、关节句柄和 PID。

---

## 八、代码修改：发布里程计和 TF

### 1. 添加依赖

#### `package.xml`

```
<depend>nav_msgs</depend>
<depend>tf2_msgs</depend>
```

- `nav_msgs`：提供 `Odometry` 里程计消息。
- `tf2_msgs`：提供 TF 变换消息。

#### `CMakeLists.txt`

在 `find_package` 和 `catkin_package` 的依赖列表中都添加：

```
nav_msgs
tf2_msgs
```

### 2. 在头文件中添加里程计成员

添加头文件：

```
#include <memory>
#include <nav_msgs/Odometry.h>
#include <realtime_tools/realtime_publisher.h>
#include <tf2_msgs/TFMessage.h>
```

在 `public:` 中添加：

```
void starting(const ros::Time& time) override;
```

在 `private:` 中添加：                                               //
                                    控制器主要对外接口          → 通常 public
                                    内部变量、内部辅助函数      → 通常 privat
```
// 里程计估计的位置和朝向。
double odom_x_ = 0.0;
double odom_y_ = 0.0;
double odom_yaw_ = 0.0;

// 上一次发布里程计的时间。
ros::Time last_odom_publish_time_;

// /odom 发布器
std::unique_ptr<realtime_tools::RealtimePublisher<nav_msgs::Odometry>>
    odom_pub_;    

// /tf 发布器。
std::unique_ptr<realtime_tools::RealtimePublisher<tf2_msgs::TFMessage>>
    tf_pub_;
```

其中：

- `odom_x_`、`odom_y_`：累计的位置。
- `odom_yaw_`：累计的水平朝向。
- `odom_pub_`：发布 `/odom`。
- `tf_pub_`：发布坐标变换。

### 3. 在 `init()` 中创建发布器

```
// 创建 /odom 发布器。
odom_pub_.reset(
    new realtime_tools::RealtimePublisher<nav_msgs::Odometry>(
        root_nh, "/odom", 10));     //用new创建一个发布器（没有名字）。reset() 会先释放原来的对象，并根据参数决定接管新对象还是变为空，传入创建发布器所需的参数：root_nhROS 节点句柄，发布器通过它和 ROS 通信"/odom"要发布的话题名称10发布队列的大小。让odom_pub_来接管这个发布器

// 设置里程计消息中的坐标系名称。
odom_pub_->msg_.header.frame_id = "odom";
odom_pub_->msg_.child_frame_id = "base_link";

// 创建 /tf 发布器。
tf_pub_.reset(
    new realtime_tools::RealtimePublisher<tf2_msgs::TFMessage>(
        root_nh, "/tf", 10));

// 提前准备一条 odom → base_link 变换。
tf_pub_->msg_.transforms.resize(1);
tf_pub_->msg_.transforms[0].header.frame_id = "odom";
tf_pub_->msg_.transforms[0].child_frame_id = "base_link";
```

### 4. 添加 `starting()` 函数

```
void HeroChassisController::starting(const ros::Time& time)
{
    // 每次启动控制器时，从零开始累计里程计。
    odom_x_ = 0.0;
    odom_y_ = 0.0;
    odom_yaw_ = 0.0;
    last_odom_publish_time_ = time;
}
```

`starting()` 在控制器启动时执行。当前代码选择每次启动都把里程计位置和朝向重置为零。

### 5. 在 `update()` 中读取四个实际轮速

```
// 读取四个轮子的实际角速度，单位 rad/s。
const double fl = front_left_joint_.getVelocity();
const double fr = front_right_joint_.getVelocity();
const double bl = back_left_joint_.getVelocity();
const double br = back_right_joint_.getVelocity();
```

这里读取的是**实际速度**，不是 `/cmd_vel` 指令计算出的目标速度。

### 6. 通过正运动学计算底盘实际速度

```
// 正运动学：轮速转换为底盘坐标系下的实际速度。
const double actual_vx =
    wheel_radius_ * (fl + fr + bl + br) / 4.0;

const double actual_vy =
    wheel_radius_ * (-fl + fr + bl - br) / 4.0;

const double actual_wz =
    wheel_radius_ * (-fl + fr - bl + br) /
    (4.0 * (half_wheelbase_ + half_track_));
```

对应关系：

```
四轮实际速度
      ↓
actual_vx：底盘实际前后速度
actual_vy：底盘实际左右速度
actual_wz：底盘实际旋转角速度
```

### 7. 累计位置和朝向

```
const double dt = period.toSec();

// 只有时间间隔有效时才累计位置。
if (dt > 0.0)
{
    // 根据底盘当前朝向，将速度转换到 odom 坐标系。
    const double cos_yaw = std::cos(odom_yaw_);
    const double sin_yaw = std::sin(odom_yaw_);

    const double odom_vx =
        actual_vx * cos_yaw - actual_vy * sin_yaw;

    const double odom_vy =
        actual_vx * sin_yaw + actual_vy * cos_yaw;

    // 位移 = 速度 × 时间。
    odom_x_ += odom_vx * dt;
    odom_y_ += odom_vy * dt;

    // 朝向变化 = 旋转角速度 × 时间。
    odom_yaw_ += actual_wz * dt;
}
```

`period` 是两次控制器更新之间的时间间隔；`period.toSec()` 将它转换成秒。

### 8. 发布 `/odom`

代码中通过 `odom_pub_` 填写并发布里程计：

```
if (odom_pub_->trylock())
{
    nav_msgs::Odometry& odom = odom_pub_->msg_;

    odom.header.stamp = time;

    // 位置和朝向，相对于 odom 坐标系。
    odom.pose.pose.position.x = odom_x_;
    odom.pose.pose.position.y = odom_y_;
    odom.pose.pose.position.z = 0.0;

    odom.pose.pose.orientation.x = 0.0;
    odom.pose.pose.orientation.y = 0.0;
    odom.pose.pose.orientation.z = qz;
    odom.pose.pose.orientation.w = qw;

    // 速度，相对于 base_link 坐标系。
    odom.twist.twist.linear.x = actual_vx;
    odom.twist.twist.linear.y = actual_vy;
    odom.twist.twist.angular.z = actual_wz;

    odom_pub_->unlockAndPublish();
}
```

其中 `qz`、`qw` 由平面朝向转换得到：

```
const double qz = std::sin(odom_yaw_ / 2.0);
const double qw = std::cos(odom_yaw_ / 2.0);
```

它们是四元数中用于表示当前平面旋转的两个分量。

### 9. 发布 `odom → base_link`

```
if (tf_pub_->trylock())
{
    geometry_msgs::TransformStamped& transform =
        tf_pub_->msg_.transforms[0];

    transform.header.stamp = time;

    transform.transform.translation.x = odom_x_;
    transform.transform.translation.y = odom_y_;
    transform.transform.translation.z = 0.0;

    transform.transform.rotation.x = 0.0;
    transform.transform.rotation.y = 0.0;
    transform.transform.rotation.z = qz;
    transform.transform.rotation.w = qw;

    tf_pub_->unlockAndPublish();
}
```

`/odom` 和 TF 使用的是同一组累计位置与朝向：

```
odom_x_、odom_y_、odom_yaw_
          ├── 填入 /odom 消息
          └── 填入 odom → base_link 变换
```

### 10. 控制发布频率

实际代码把 `/odom` 和 TF 的发布放在这个判断中：

```
// 约每 0.02 秒发布一次，即约 50 Hz。
if ((time - last_odom_publish_time_).toSec() >= 0.02)
{
    last_odom_publish_time_ = time;

    // 发布 /odom 和 TF。
}
```

轮速读取和里程计累计仍在每次 `update()` 时进行；这里限制的是**消息发布频率**。

---

## 九、配置文件修改

在 `controllers.yaml` 中，底盘尺寸和超时时间放在 `hero_chassis_controller` 下面：

```
controller:
  hero_chassis_controller:
    type: hero_chassis_controller/HeroChassisController

    wheel_radius: 0.07625
    half_wheelbase: 0.2
    half_track: 0.2
    command_timeout: 0.5

    front_left_pid:
      p: 1.0
      i: 0.0
      d: 0.0
      i_clamp: 0.0

    # 这里继续保留右前、左后、右后三组 PID 参数。
```

关节状态控制器单独配置：

```
  joint_state_controller:
    type: joint_state_controller/JointStateController
    publish_rate: 50
```

之前这四个底盘参数被放到了 `joint_state_controller` 下面，后来移到了 `hero_chassis_controller` 下面。程序之前仍能运行，是因为代码提供了默认值；放到正确位置后，控制器才能按预期读取这组配置。





# 智能指针

std::unique_ptr：独占管理一个对象，管理它的 `unique_ptr` 被销毁或放弃管理时释放
std::shared_ptr：多个 shared_ptr共同管理一个对象，最后一个拥有它的 `shared_ptr` 放弃管理时释放
std::weak_pt：观察一个由 `shared_ptr` 管理的对象，**不参与共同拥有**|它的存在不会让对象延长寿命
boost::shared_ptr：Boost 库提供的共享智能指针|与 `std::shared_ptr` 一样，最后一个共享拥有者放弃管理时



# TF坐标变换
常用依赖：
if2_geometry_msgs:将ros消息转化为tf2消息
tf2:封装了坐标变换的常用消息
tf2_ros:为tf2提供了roscpp和rsopy绑定，封装了坐标常用api



# 坐标msg消息
两种消息类型：
## 1.geometry_msgs/PointStamped:
传输某个坐标系内坐标点的位置
std_msgs/Header header   //有一个叫 `header` 的字段，它的类型是 `std_msgs/Header`
  uint32 seq   //消息序号
  time stamp  //时间
  string frame_id   坐标系名称
geometry_msgs/Point point   //有一个叫 `point` 的字段，它的类型是 `geometry_msgs/Point`，里面包含：
  float64 x
  float64 y
  float64 z


## 2.geometry_msgs/TransformStamped:
传输坐标系相对位置信息
std_msgs/Header header
  uint32 seq
  time stamp
  string frame_id                  / /坐标系1的名字
string child_frame_id         //坐标系2的名字
geometry_msgs/Transform transform      2相对于1的偏移量
  geometry_msgs/Vector3 translation     //平移
    float64 x
    float64 y
    float64 z
  geometry_msgs/Quaternion rotation   //旋转
    float64 x
    float64 y
    float64 z
    float64 w




# 静态坐标变换
创建功能包
依赖：roscpp rospy std_msgs tf2 tf2_ros tf2_geometry_msgs geometry_msgs
创建.cpp：
## 发布方：
```#include "ros/ros.h"
#include "tf2_ros/static_transform_broadcaster.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2/LinearMath/Quaternion.h"

int main(int argc, char *argv[])
{
    // 2.设置编码 节点初始化 NodeHandle;
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"static_pub");
    ros::NodeHandle nh;

    // 3.创建发布对象;
    tf2_ros::StaticTransformBroadcaster pub;

    // 4.组织被发布的消息;
    geometry_msgs::TransformStamped tfs;
    tfs.header.stamp = ros::Time::now();
    tfs.header.frame_id = "base_link";//相对坐标系关系中被参考的那一个
    tfs.child_frame_id = "laser";
    tfs.transform.translation.x = 0.2;
    tfs.transform.translation.y = 0.0;
    tfs.transform.translation.z = 0.5;

    //需要根据欧拉角转换
    tf2::Quaternion qtn; // 创建 四元数 对象
    //向该对象设置欧拉角，这个对象可以将欧拉角转换成四元数
    qtn.setRPY(0,0,0); // 欧拉角的单位是弧度

    tfs.transform.rotation.x = qtn.getX();
    tfs.transform.rotation.y = qtn.getY();
    tfs.transform.rotation.z = qtn.getZ();
    tfs.transform.rotation.w = qtn.getW();

    // 5.发布数据;
    pub.sendTransform(tfs);

    // 6.spin();
    ros::spin();
    return 0;
}
```
编译ros空间
rosrun tf01_static demo01_staric_pub


##查看：
1.终端：rostopic echo /tf_static
2.图形化：rviz
更改fixed frame
add TF

## 订阅方
```#include "ros/ros.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "geometry_msgs/PointStamped.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

/*
订阅方:订阅发布的坐标系相对关系,传入一个坐标点,调用 tf 实
流程:
    1.包含头文件;
    2.编码、初始化、NodeHandle(必须的);
    3.创建订阅对象; ---> 订阅坐标系相对关系
    4.组织一个坐标点数据;
    5.转换算法, 需要调用TF内置实现;
    6.最后输出。
*/

int main(int argc, char *argv[])
{
    // 2.编码、初始化、NodeHandle(必须的);
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"static_sub");
    ros::NodeHandle nh;
    // 3.创建订阅对象; ---> 订阅坐标系相对关系
    // 3-1.创建一个 buffer 缓存
    tf2_ros::Buffer buffer;
    // 3-2.再创建监听对象(监听对象可以将订阅的数据存入buffer)
    tf2_ros::TransformListener listener(buffer);

    // 4.组织一个坐标点数据;
    geometry_msgs::PointStamped ps;
    ps.header.frame_id = "laser";
    ps.header.stamp = ros::Time::now();
    ps.point.x = 2.0;
    ps.point.y = 3.0;
    ps.point.z = 5.0;

    // 5.转换算法, 需要调用TF内置实现;
    ros::Rate rate(10);
    while (ros::ok())
    {
        //核心代码 ---- 将 ps 转换成相对于 base_link 的坐标点
        geometry_msgs::PointStamped ps_out;
        ps_out = buffer.transform(ps,"base_link");
        // 6.最后输出。
        ROS_INFO("转换后的坐标值:(%.2f,%.2f,%.2f),参考的坐标系:%s",
                 ps_out.point.x,
                 ps_out.point.y,
                 ps_out.point.z,
                 ps_out.header.frame_id.c_str()
                );
        rate.sleep();
        ros::spinOnce();
    }

    return 0;
}
```

解释：```
ps：laser 坐标系下的点
          ↓
buffer.transform(ps, "base_link")
          ↓
ps_out：base_link 坐标系下的同一个点
```




# 动态坐标变换

......

##  发布方
``````

```#include <ros/ros.h>
#include <clocale>
#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2/LinearMath/Quaternion.h"

/*
发布方：需要订阅乌龟的位姿信息，转换成相对于窗体的坐标关系，并发布
准 备
    话题：/turtle1/pose
    消息：turtlesim/Pose

流程：
    1.包含头文件；
    2.设置编码、初始化、NodeHandle;
    3.创建订阅对象，订阅 /turtle1/pose;
    4.回调函数处理订阅的消息: 将位姿信息转换成坐标相对关系并发布(关注)
    5.spin()
*/

void doPose(const turtlesim::Pose::ConstPtr& pose)
{
    //获取位姿信息，转换成坐标系相对关系(核心)，并发布
    //a.创建发布对象;
    static tf2_ros::TransformBroadcaster pub;
    //b.组织被发布的数据;
    geometry_msgs::TransformStamped ts;

    ts.header.frame_id = "world";
    ts.header.stamp = ros::Time::now();
    ts.child_frame_id = "turtle1";

    //坐标系偏移量设置
    ts.transform.translation.x = pose->x;
    ts.transform.translation.y = pose->y;
    ts.transform.translation.z = 0;

    //坐标系四元数
    /*
    位姿信息中没有四元数，但是有个偏航角度，又已知乌龟是 2D，没有翻滚与俯仰角度，所以可以得出乌龟
    的欧拉角： 0 0 theta
    */
    tf2::Quaternion qtn;
    qtn.setRPY(0,0,pose->theta);

    ts.transform.rotation.x = qtn.getX();
    ts.transform.rotation.y = qtn.getY();
    ts.transform.rotation.z = qtn.getZ();
    ts.transform.rotation.w = qtn.getW();

    //c.发布。
    pub.sendTransform(ts);
}

int main(int argc, char *argv[])
{
    // 2. 设置编码、初始化、NodeHandle;
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"dynamic_pub");
    ros::NodeHandle nh;

    //3.创建订阅对象
    ros::Subscriber sub = nh.subscribe<turtlesim::Pose>("/turtle1/pose",100,doPose);

    ros::spin();
    return 0;
}
```


## 订阅方：

.....

```#include "ros/ros.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "geometry_msgs/PointStamped.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

/*
订阅方：订阅发布的坐标系相对关系，传入一个坐标点，调用 tf 实现转换

流程：
    1.包含头文件；
    2.编码、初始化、NodeHandle(必须的)；
    3.创建订阅对象； ---> 订阅坐标系相对关系
    4.组织一个坐标点数据；
    5.转换算法，需要调用TF内置实现；
    6.最后输出。
*/

int main(int argc, char *argv[])
{
    // 2.编码、初始化、NodeHandle(必须的)；
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"static_sub");
    ros::NodeHandle nh;

    // 3.创建订阅对象； ---> 订阅坐标系相对关系
    // 3‑1.创建一个 buffer 缓存
    tf2_ros::Buffer buffer;
    // 3‑2.再创建监听对象(监听对象可以将订阅的数据存入buffer)
    tf2_ros::TransformListener listener(buffer);

    // 4.组织一个坐标点数据
    geometry_msgs::PointStamped ps;
    // 参考的坐标系
    ps.header.frame_id = "turtle1";
    
    ps.header.stamp = ros::Time(0.0);      //动态订阅的时间戳不能用now，防止时间差异过大
    ps.point.x = 2.0;
    ps.point.y = 3.0;
    ps.point.z = 5.0;

    //添加休眠
    // ros::Duration(2).sleep();

    // 5.转换算法，需要调用TF内置实现；
    ros::Rate rate(10);
    while (ros::ok())
    {
        //核心代码 ---- 将 ps 转换成相对于 world 的坐标点
        geometry_msgs::PointStamped ps_out;
        /*
        调用了 buffer 的转换函数 transform
        参数1：被转换的坐标点
        参数2：目标坐标系
        返回值：输出的坐标点

        PS1：调用时必须包含头文件 tf2_geometry_msgs/tf2_geometry_msgs.h
        PS2：运行时存在的问题:抛出一个异常 base_link 不存在
        原因：订阅数据是一个耗时操作，可能再调用 transform 转换函数时，坐标系的
            相对关系还没有订阅到，因此出现异常
        解决：
            方案1：在调用转换函数前，执行休眠
            方案2：进行异常处理(建议)
        */
        try
        {
            ps_out = buffer.transform(ps,"world");
            // 6.最后输出。
            ROS_INFO("转换后的坐标值:(%.2f,%.2f,%.2f),参考的坐标系:%s",
                    ps_out.point.x,
                    ps_out.point.y,
                    ps_out.point.z,
                    ps_out.header.frame_id.c_str()
                    );
        }
        catch(const std::exception& e)
        {
            // std::cerr << e.what() << '\n';
            ROS_INFO("异常消息:%s",e.what());
        }

        rate.sleep();
        ros::spinOnce();
    }

    return 0;
}
```



# odom和base
在controllers.yaml里切换
odom：沿着坐标轴方向运动（有误差）
base：沿着底盘的方向


# 键盘键盘移动

rosrun teleop_twist_keyboard teleop_twist_keyboard.py

```
 i：前进
 j：左转   
 k：停止   
 l：右转 
 ,：后退
```

注意：这里的 `j` 和 `l` 是让底盘**原地旋转**，不是左右横移。

### 麦克纳姆轮左右横移

按住 **Shift**，再按对应字母：

```
Shift + J：向左横移
Shift + L：向右横移
```
### 调整速度
```
q：同时提高线速度和旋转速度
z：同时降低线速度和旋转速度

w：只提高线速度
x：只降低线速度

e：只提高旋转速度
c：只降低旋转速度
```

建议先按几次 `z` 降低速度，再小范围测试。

### 停止和退出

- **停止移动**：按 `k` 或空格键。
- **退出键盘控制**：按 `Ctrl+C`。



#  加速度限制
## 1. 修改头文件

打开 `hero_chassis_controller.h`，找到：

```
        // 指令超过这个时间没有更新时，目标速度归零，单位：秒。
        double command_timeout_ = 0.5;
```

在下面添加：

```
        // 底盘线加速度和角加速度限制。
        double max_linear_accel_ = 0.4;   // m/s²
        double max_angular_accel_ = 0.8;  // rad/s²

        // 上一次限制后的底盘目标速度。
        double limited_vx_ = 0.0;
        double limited_vy_ = 0.0;
        double limited_wz_ = 0.0;
```

## 2. 在 `init()` 中读取配置

打开 `hero_chassis_controller.cpp`，找到：

```
        controller_nh.param("command_timeout", command_timeout_, 0.5);
```

在下面添加：

```
        // 读取底盘加速度限制。
        controller_nh.param("max_linear_accel", max_linear_accel_, 0.4);
        controller_nh.param("max_angular_accel", max_angular_accel_, 0.8);

        if (!std::isfinite(max_linear_accel_) || max_linear_accel_ <= 0.0 ||
            !std::isfinite(max_angular_accel_) || max_angular_accel_ <= 0.0)
        {
            ROS_ERROR("加速度限制必须是大于零的有效数值");
            return false;
        }
```

如果配置文件中没有这两个参数，就使用代码里的默认值。

## 3. 启动控制器时清零

找到：

```
    void HeroChassisController::starting(const ros::Time& time)
```

在这个函数中加入三行。修改后该函数是：

```
    void HeroChassisController::starting(const ros::Time& time)
    {
        // 每次启动控制器时，从零开始累计里程计。
        odom_x_ = 0.0;
        odom_y_ = 0.0;
        odom_yaw_ = 0.0;
        last_odom_publish_time_ = time;

        // 限速后的目标速度从零开始。
        limited_vx_ = 0.0;
        limited_vy_ = 0.0;
        limited_wz_ = 0.0;
    }
```

## 4. 在 `update()` 中限制速度变化

找到 `update()` 里这一段：

```
        if (command != nullptr && !command->stamp.isZero())
        {
            const double age = (time - command->stamp).toSec();

            if (age >= 0.0 && age <= command_timeout_)
            {
                vx = command->vx;
                vy = command->vy;
                wz = command->wz;
            }
        }
```

**将这一段替换为：**

```
        // 判断当前指令是否有效。
        bool command_valid = false;

        if (command != nullptr && !command->stamp.isZero())
        {
            const double age = (time - command->stamp).toSec();

            if (age >= 0.0 && age <= command_timeout_)
            {
                vx = command->vx;
                vy = command->vy;
                wz = command->wz;
                command_valid = true;
            }
        }

        const double limit_dt = period.toSec();

        if (command_valid && std::isfinite(limit_dt) && limit_dt > 0.0)
        {
            // 本次更新允许变化的最大速度。
            const double max_linear_change = max_linear_accel_ * limit_dt;
            const double max_angular_change = max_angular_accel_ * limit_dt;

            // 限制前后速度的变化。
            limited_vx_ += std::max(
                -max_linear_change,
                std::min(vx - limited_vx_, max_linear_change));

            // 限制左右速度的变化。
            limited_vy_ += std::max(
                -max_linear_change,
                std::min(vy - limited_vy_, max_linear_change));

            // 限制旋转速度的变化。
            limited_wz_ += std::max(
                -max_angular_change,
                std::min(wz - limited_wz_, max_angular_change));

            // 使用限制后的速度计算轮子的目标速度。
            vx = limited_vx_;
            vy = limited_vy_;
            wz = limited_wz_;
        }
        else
        {
            // 指令无效、超时或时间间隔异常时，目标速度直接归零。
            limited_vx_ = 0.0;
            limited_vy_ = 0.0;
            limited_wz_ = 0.0;

            vx = 0.0;
            vy = 0.0;
            wz = 0.0;
        }
```

后面的逆运动学和 PID 代码保持不变。

这段的核心公式是：

```
每次允许的最大速度变化 = 最大加速度 × 本次更新间隔
```

例如最大线加速度为 `0.4 m/s²`，如果目标速度从 `0` 变成 `0.4 m/s`，按这个限制至少需要约 1 秒才能把**限制后的目标值**增加到 `0.4 m/s`。实际轮速和底盘速度还会受到 PID 和仿真物理影响。

> 这里分别限制 `x` 和 `y` 两个方向的变化；两个方向同时运动时，合成加速度可能大于单独配置的 `0.4 m/s²`。

## 5. 修改 `controllers.yaml`

在 `hero_chassis_controller:` 下，与 `wheel_radius` 同一级的位置添加：

```
    # 底盘目标速度变化限制。
    max_linear_accel: 0.4
    max_angular_accel: 0.8
```

例如：

```
  hero_chassis_controller:
    type: hero_chassis_controller/HeroChassisController

    command_frame_mode: "odom"
    tf_timeout: 0.3

    wheel_radius: 0.07625
    half_wheelbase: 0.2
    half_track: 0.2
    command_timeout: 0.5

    max_linear_accel: 0.4
    max_angular_accel: 0.8
```

后面的四组 PID 配置继续保留。

## 6. 编译并测试

1. 保存文件，在 CLion 点击 **工具 → ROS1（Noetic 容器）→ 编译 ROS1 工作空间**。
    
2. 编译成功后，停止控制器和 Gazebo，再重新启动它们，让 Gazebo 加载新编译的插件。
    
3. 使用键盘控制或 rqt Message Publisher，给一个持续的前进指令，观察底盘是否平缓加速。
    
4. 在 rqt 中选择 **Plugins → Visualization → Plot**，观察：
    
    ```
    /odom/twist/twist/linear/x
    ```
    
    这条曲线显示的是**估计的实际底盘速度**，可用来观察运动变化；它不是代码内部限制后的目标速度。
    
5. 停止发送指令，确认超时后仍能停车。



# 多坐标变换

#### 1.创建功能包

创建项目功能包依赖于 tf2、tf2_ros、tf2_geometry_msgs、roscpp rospy std_msgs geometry_msgs、turtlesim

#### 2.发布方

为了方便，使用静态坐标变换发布

```
<launch>
    <node pkg="tf2_ros" type="static_transform_publisher" name="son1" args="0.2 0.8 0.3 0 0 0 /world /son1" output="screen" />
    <node pkg="tf2_ros" type="static_transform_publisher" name="son2" args="0.5 0 0 0 0 0 /world /son2" output="screen" />
</launch>
```

#### 3.订阅方

```
/*

需求:
    现有坐标系统，父级坐标系统 world,下有两子级系统 son1，son2，
    son1 相对于 world，以及 son2 相对于 world 的关系是已知的，
    求 son1 与 son2中的坐标关系，又已知在 son1中一点的坐标，要求求出该点在 son2 中的坐标
实现流程:
    1.包含头文件
    2.初始化 ros 节点
    3.创建 ros 句柄
    4.创建 TF 订阅对象
    5.解析订阅信息中获取 son1 坐标系原点在 son2 中的坐标
      解析 son1 中的点相对于 son2 的坐标
    6.spin

*/
//1.包含头文件
#include "ros/ros.h"
#include "tf2_ros/transform_listener.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/PointStamped.h"

int main(int argc, char *argv[])
{   setlocale(LC_ALL,"");
    // 2.初始化 ros 节点
    ros::init(argc,argv,"sub_frames");
    // 3.创建 ros 句柄
    ros::NodeHandle nh;
    // 4.创建 TF 订阅对象
    tf2_ros::Buffer buffer; 
    tf2_ros::TransformListener listener(buffer);
    // 5.解析订阅信息中获取 son1 坐标系原点在 son2 中的坐标
    ros::Rate r(1);
    while (ros::ok())
    {
        try  //异常处理语法，先尝试执行大括号里的代码，如果发生了可以捕获的异常，就交给后面的catch`处理
        {
        //   解析 son1 中的点相对于 son2 的坐标
            geometry_msgs::TransformStamped tfs = buffer.lookupTransform("son2","son1",ros::Time(0));  //查询 最新的son1相对于 son2的变换
            ROS_INFO("Son1 相对于 Son2 的坐标关系:父坐标系ID=%s",tfs.header.frame_id.c_str());
            ROS_INFO("Son1 相对于 Son2 的坐标关系:子坐标系ID=%s",tfs.child_frame_id.c_str());
            ROS_INFO("Son1 相对于 Son2 的坐标关系:x=%.2f,y=%.2f,z=%.2f",
                    tfs.transform.translation.x,
                    tfs.transform.translation.y,
                    tfs.transform.translation.z
                    );

            // 坐标点解析
            geometry_msgs::PointStamped ps;
            ps.header.frame_id = "son1";
            ps.header.stamp = ros::Time::now();
            ps.point.x = 1.0;
            ps.point.y = 2.0;
            ps.point.z = 3.0;

            geometry_msgs::PointStamped psAtSon2;
            psAtSon2 = buffer.transform(ps,"son2");  //把 ps 这个点转换到 son2 坐标系下
            ROS_INFO("在 Son2 中的坐标:x=%.2f,y=%.2f,z=%.2f",
                    psAtSon2.point.x,
                    psAtSon2.point.y,
                    psAtSon2.point.z
                    );
        }
        catch(const std::exception& e) 
        {
            // std::cerr << e.what() << '\n';
            ROS_INFO("异常信息:%s",e.what());  //e.what()：获取异常的说明文字
        }


        r.sleep();
        // 6.spin
        ros::spinOnce();
    }
    return 0;
}
```




