# 第三次培训笔记

## 网络课程笔记

### ROS概述/环境搭建

#### ROS简介

机器人(复用的分工合作)（组成复杂）
- 硬件设计
- 嵌入式设计（小脑）
- 上层软件设计（大脑）【采集信息，下达命令到小脑】（ROS）
- 机械结构设计
- 机械加工
ROS：机器人通用的软件框架（机器人操作系统）

#### ROS概念

- 适用于机器人的开源操作系统    <!-- 开源：免费-->
- ROS 集成了大量的工具/库/协议，提供类似OS所提供的功能<分布式>
- 用于多台计算机上获取/构建/编写/运行代码的工具和库
- ROS设计者
<!--
- 小机器人在自己的处理器（性能不行）
- 分布式<br/>再使用PC机器感知信息，汇总运算，产生结果
再发送到小机器人身上
-->
ROS=通信+工具+功能+生态
- 通信：（核心）数据交换，雷达数据和摄像头数据传输处理
- 工具：例如：仿真功能模拟机器人
- 功能：算法调参
- 生态：话语权

#### ROS设计目标

- 代码复用
- 分布式：进程（节点）分布式框架，分布于不同主机协同工作
- 松耦合：输入输出标准化，ROS中功能米快分装于独立的功能包或者元功能包，便于分享。功能包的模块以节点为单位运行
- 精简
- 语言独立性：语言弱相关
- 易于测试rostest的内置单元/继承测试框架，可以轻松安装和拆卸测试工具
- 大量应用：适用于大型运行的时候系统的开发流程
- 丰富的组件化工具包：RVIZ（3D可视化工具）
- 免费且开源

### ROS下载和安装：
1、官网
2、Ubuntu的使用和优化
    1、安装
    2、启动文件交互模式
    3、安装拓展插件
    4、其他/输入法

- 如果需要卸载：
'''
sudo apt-get remove ros-noetic-*
source ~/.bashrc
'''

- 安装ros里面功能包：
'''
sudo apt install ros-noetic-PACKAGE
'''

- 搜索ros功能包：
'''
apt research ros-noetic
'''
---
安装构建依赖：
- 工具
ros中使用许多工具箱要求需要需要初始化rosdep（可以安装系统依赖）
'''
sudo apt install python3-rosdep
'''

- 初始化rosdep
'''
sudo rosdep init
rosdep update
'''

网站内容下载失败：有些资源在国外,屏蔽

- 更改/etc/hosts文件命令：
'''
sudo gdit /etc/hosts
'''

1、下载备份到gitee
2、进入“usr/lib/pythons3/dest-packages”查找rosdep中和raw.githubusercomtent.com
3、修改文件
github改为gitee

### ros的快速体验

#### helloworld实现流程

1、创建空间
---
'''
mkdir -p 自定义名字_ws/src
cd 自定义名字
catkin_make

<!--
_ws 不可以省略
a如果有问题就cd退出之后再“ /demo_ws  ”-->
'''

创建成功之后会在文件里面出现/src之外的“build目录”等

2、进入src并创建ros包并添加依赖
---
'''
cd src
catkin_create_pkg *名字* roscpp rospy std_msgs   <!--创建功能包-->
<!--  std_msgs   是标准消息库  -->
'''

#### cpp实现helloworld

1、在新建的文件里面的src（存放cpp源代码的地方新建一个cpp文件）
**如果vim进不去cpp文件，因为docker里面啥也没安装。。。**
**用 “cat > 文件名字” 强行写入**

之后就可以开始写代码了

'''
<A包含ros的头文件>
#include "ros/ros.h"

<a编写main函数>
int main(int argc,char *argv[]){

    # 初始化ros节点
    ros::init(argc,argv,"节点名称为字符串，需要自己定义");

    ros::NodeHandle n;
<!--创建ros节点-->*非必要*

    # 输出日志（helloworld）
    ROS_INFO("hello world");
**ROS_INFO要大写**
    return 0;}
'''
写完时候按“ctrl+D”==EOF

退出之后，进入和src同级的CMakeLists.txt
1、第十二行：rospy（确认是否有误）
2、
改“add_executable”(136行，把#删掉)
149的target_link_libraries和下两行的#删掉

3、在add_executable里面有个helloworld的命名，去修改
后面的改成你的文件名字（cpp），前面修改变量（随机命名）
4、在target里面改第一个变量为前面executable里面的

***改完需要退回到容器的目录再catkin！！！***

- 执行
1、新开终端
进入自己新建的    
**~/目录——ws**
2、roscore启动
测试catkin的地方：source ./devel/setup.bash
3、rosrun helloword 自己命名的东西（上面进入CNakeLists调试的）(功能包的名字+进入CMakeLists里面随机命名的比如haha)


***  如果后续有问题改动了cpp的源代码，则需要重新
*** source ./devel/setup.bash  ***
***  caekin_make  ***


方便在任意终端使用ros
---
添加以下代码：
'''
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
'''

自定义功能包也可以通过上面的方法、
---
ctrl+h：
*调出隐藏包，找到/.bashrc   这个文件，然后进去在文件最后一行 添加   source ～/目录名字_ws/devel/setup.bash*

保存之后，打开新终端，重新加载：
'''
source .bashrc
'''

再新建一个终端:
'''
rosrun helloworld(在src里面写的文件名字) hello（在CMakeList里面修改的自定义字符串名字）
'''

<a ，后面方便这个步骤可做可不做，做了以后直接rosrun就行，反之每次都要激活>

#### helloworld 的python版本

1、先进入“——ws”工作空间

2、进入src目录

3、进入helloworld的功能包

4、新建文件夹：scripts       <a脚本>

5、进入并添加python文件**————.py**
'''
<指定解释器>
*** #！usr/bin/env python***
<A导包>
import rospy
<a编写主入口>
if __name__=='__main__' :
**记得是单引号，而不是双引号！！**
**注意：等号两边都需要加空格，后面的__main__左右写的是双引号而不是括号**
    rospy.init.node("hello_p");   <a初始化ros节点>
    rospy.loginfo("helloworld");

6、添加可执行权限：
打开终端指导script中（并进入），ll查看权限
再补上chmod +x python文件名字

7、配置CMakeLists(如果之前有用来修改过cpp的话，建立在那个的基础上就行，不用新建CMakeLists  ！！)**一个pkg里面不能出现两个CMakeListts.txt**
找到：
catkin_install_python这一行，总计两行删除井号四个
改法：第二行的scripts文件后面需要加上python的文件名字（什么什么,py）
<a作用:为python文件选择恰当的解释器>

在写完源文件之后
**python文件添加可执行权限**
xhost + 自定义文件名字.py



### 进入ROS里面的ubuntu20.04

'''
docker ps             <!--在输出列表里面找到自己的容器名字，记录名字-->
docker exec -it 名字 /bin/bash
<!--
-it:保持交互并分配终端
/bin/bash：启动bash shell，
  如果报错可以使用 /bin/sh
  成功之后命令提示符会变成容器内的，通常显示为  root@<名字>:/# 之类

如果docker ps 步骤没有看见，则先启动它：
docker start 名字
-->
source /opt/ros/noetic/setup.bash   <!--加载环境变量-->

## ros.wiki笔记

1、预备工作：
如果ros不是完整桌面版本，那么：
'''
sudo apt-get install ros-<distro>-ros-tutorials
'''
将<distro>替换成安装的ros发行版简称

2、概念
Manifests (package.xml)： 清单（Manifest）是对软件包的描述。它用于定义软件包之间的依赖关系，并记录有关软件包的元信息，如版本、维护者、许可证等

3、文件系统工具
    1、rospack（获取软件包的有关信息）
'''
$ rospack find [package_name]
'''
*上面中括号可以省略*

会输出：
'''
YOUR_INSTALL_PATH/share/roscpp
'''
<!--在ubuntu上通过apt安装的话-->
'''
/opt/ros/<distro>/share/roscpp
'''
---
    2、roscd
rosbash命令集的一部分，它允许你直接切换目录（cd）到某个软件包或者软件包集当中

'''
$ roscd [locationname[subdir]]
'''
<!--验证能否切换到roscpp的位置-->
'''
$ roscd roscpp
'''

使用Unix命令pwd输出工作目录:
'''
$ pwd
'''
会看到：
'''
YOUR_INSTALL_PATH/share/roscpp
'''

**roscd只能切换到那些路径已经包含在ROS_PACKAGE_PATH环境变量中的软件包**

ROS_PACKAGE_PATH环境变量应该包含那些保存有ROS软件包的路径，并且每个路径之间用冒号（:）分隔开来。
一个典型的ROS_PACKAGE_PATH环境变量如下：
'''
/opt/ros/<distro>/base/install/share
'''
### catkin 软件包
最简单的catkin软件包看起来就像这样：
'''
my_package/
CMakeLists.txt
package.xml
'''
开发catkin软件包的推荐方法是使用catkin工作空间，但是你也可以单独开发catkin软件包。一个简单的工作空间如下所示：
'''
    workspace_folder/        -- WORKSPACE
      src/                   -- SOURCE SPACE
        CMakeLists.txt       -- 'Toplevel' CMake file, provided by catkin
        package_1/
          CMakeLists.txt     -- CMakeLists.txt file for package_1
          package.xml        -- Package manifest for package_1
        ...
        package_n/
          CMakeLists.txt     -- CMakeLists.txt file for package_n
          package.xml        -- Package manifest for package_n
'''

#### 创建catkin软件包
'''
$ cd ~/catkin_ws/src
$ catkin_create_pkg beginner_tutorials std_msgs rospy roscpp

*catkin_create_pkg命令会要求你输入package_name，如有需要还可以在后面添加一些需要依赖的其它软件包：*
'''
<!--This is an example, do not try to run this
 catkin_create_pkg <package_name> [depend1] [depend2] [depend3]
-->
'''

#### 创建catkin工作区+生效

'''
cd ~/catkin_ws
catkin_make
'''

#### 软件包依赖关系

##### 一级依赖

使用rospack命令工具来查看这些一级依赖包。
'''
rospack depends1 beginner_tutorials 
    std_msgs
    rospy
    roscpp
'''
这些依赖关系存储在package.xml文件中
'''
roscd beginner_tutorials
cat package.xml

<package>
...
  <buildtool_depend>catkin</buildtool_depend>
  <build_depend>roscpp</build_depend>
  <build_depend>rospy</build_depend>
  <build_depend>std_msgs</build_depend>
...
</package>

##### 间接依赖：

一个依赖包还会有它自己的依赖关系

在使用rospack可以递归检测出所有嵌套的依赖包：
'''
rospack depends beginner_tutorials
---
cpp_common
rostime
roscpp_traits
roscpp_serialization
catkin
genmsg
genpy
message_runtime
gencpp
geneus
gennodejs
genlisp
message_generation
rosbuild
rosconsole
std_msgs
rosgraph_msgs
xmlrpcpp
roscpp
rosgraph
ros_environment
rospack
roslib
rospy
'''

#### 自定义软件包

- 描述标签
---
description标签
<description>The beginner_tutorials package</description>
<!--将描述信息修改为任何你喜欢的内容，但是按照惯例，第一句话应该简短一些，因为它包括了软件包的范围。如果用一句话难以描述完全那就需要换行了。 -->

- 维护者标签
maintainer标签
'''
<!-- One maintainer tag required, multiple allowed, one person per tag --> 
<!-- Example:  -->
<!-- <maintainer email="jane.doe@example.com">Jane Doe</maintainer> -->
<maintainer email="user@todo.todo">user</maintainer>
'''

它能够让其他人联系到软件包的相关人员。至少需要填写一个维护者，但如果你想的话可以添加多个。除了在标签里面填写维护者的名字外，还应该在标签的email属性中填写电子邮件地址
'''
<maintainer email="you@yourdomain.tld">Your Name</maintainer>
'''

---

## ros 笔记

### ros相关概念

*题目1：编写ros程序，在控制台输出文本 Hello World，分别使用cpp和python*

1. 创建工作空间,并初始化
'''
mkdir -p 空间名字_ws/src
cd 空间
catkin_make
'''

2.进入src创建ros包并添加依赖
'''
cd src
catkin_create_pkg 包名字 roscpp rospy std_msgs
'''

std_msgs:标准消息库

3. 进入包中  /src
'''
cd 包名字
'''

4. 在src中创建一个cpp源文件
'''
touch 名字.cpp
'''

5. cpp写

'''
#include"ros/ros.h"
int main(int msga,char *msgv[])
{
    ros::init(msga,msgv,"自己起名字，字符串名字")；
    ROS_INFO(“hello world");
return 0;
]
'''

6. 改动CMakeLists
找到并改动
'''
add_executable(自己随便起一个名字作为执行文件的名字 src/cpp的名字 )
target_link_libraries(上面起的名字
)
'''

7. 终端再次catkin_make

---

8. 执行文件开始之前需要再开一个终端先激活核心：
终端1：
'''
roscore
'''

终端2：
'''
source devel/setup.bash
'''

9. rosrun 执行文件
'''
rosrun pkg名字 可执行文件的名字
'''

在任意窗口使用rosrun：
'''
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
'''

source ～/工作空间 devel/setup.bash
也可以写进去

---
### 集成开发环境的搭建

#### 安装终端

快捷键盘：
ctrl+shift+O = 水平分割终端
ctrl+shift+E = 垂直分割终端
alt+up = 向上移动窗口
atl+下箭头 = 向下移动窗口
ctrl+alt+C/V = 作用相同
ctrl+alt+W = 关闭当前终端
ctrl+alt+X = 最大化
ctrl+alt+S = 恢复


#### vscode 里面开发ros

1. 创建空间

2. 进入空间_ws 并启动vs code
'''
cd XXX_ws
code .
'''

3. vs code 翻译
快捷键“ctrl + shift + B”调用编译，选择catkin_make:build

4. 创建ros功能包

5. 编写ros功能包

6. 添加以后用到的依赖

7. 写完代码之后改正CmakeLists里面的add executable（136行大概）

add executable （可执行文件的名字，自命名） src/cpp的名字）


下面几行里面选target_link_libraries(自定义的可执行文件的名字


)


编译（ctrl+shift+B）

---
中文乱码问题
在cpp中加入一行文字：
'''
setlocale(LC_ALL,"");
'''
---
中文乱码还可以用：
'''
setlocal(LC_CTYPE,"zh_CN.utf8")
'''
---
ros的官网：http：//wiki.ros.org/IDEs
---

### launch 文件

优化每次调用roscore

#### 流程

1. 新建launch文件夹
2. 添加launch文件 
3. 编辑launch文件内容
  申明以下要启动的节点，节点名字叫做“node”
node：某个节点
pkg：功能包
name：为节点命名
output：设置日志的输出目标
---
格式：
'''
<launch>
    <添加要执行的节点>
</launch>

*再“ctrl+～”运行*
节点编写格式：
<node pkg="" type="" name="" />



- 节点：
在rosrun turtlesim turtlesim_node   (启动海龟用的)
第二个turtlesim是pkg，turtlesim_node 是type（节点），name自己随便起名字

- 键盘输出：
rosrun turtlesim turtle_teleop_key
(节点名字发生变化)

- 注意写<node>节点的时候记得闭合（前面由<node>,后面要有</node>）



4. 输出launch文件
roslaunch

---
如果要显示在屏幕上的话，需要：在node里面补上“output=“screen””


---
如果出现docker无法调用显卡的问题，可以尝试
export LIBGL_ALWAYS_SOFTWARE=1
再
roslaunch

### ros 架构

- 设计者
通讯机制+工具软件包+机器人高层技能+机器人生态系统

- 维护者
main
核心部分

universe
全球范围（基本库的代码“opencv”//）

- 系统结构
os层：操作层面(元操作系统，需要依赖真正意义上的操作系统)
中间层：ros封装的关于机器人的开发中间件（TCP//UDP//Nodelet//机器人开发实现库）
应用层面：功能包（master turtlesim）

- 自身结构
文件系统：
硬盘上查看的ros源代码的组织形式

计算图：
启动新终端，输入“rqt_graph rqt_graph”,可以查看网络拓扑图

开源社区
发行版：官方发行版本

软件库：

ros维基：
wiki:网址：“https：//wiki.ros.org/”

bug提交系统

ros问答
“https：//answers.ros.org/quetions/”

博客/ros系统：
"https://discourse.ros.org"
---
workspace：工作空间
build:存放缓存信息、配置信息和中间文件
devel：开放空间，用于存放编译后生成的文件
src：源码
package：功能包
include：头文件
srv：服务通信
msg：消息
action：动作格式文件
config：配置信息


---
### ros通信机制：

进程的分布框架

1. 话题通信
发布方--订阅方

2. 服务通信：
服务端和客户端


3. 参数服务器（参数共享）
共享

---
#### 话题订阅

（雷达--导航--底盘）

角色-流程-注意
master-->管理者（建立链接）（管理和匹配话题）
talker-->发布者
listener-->订阅者


流程：
0：talker提交个人信息
1. listener提交个人需求
2. master把talker“rpc”信息发送给listener
3. listener以tcp地址访问talker
4. talker响应，给一个新的tcp地址
5. listener根据tcp访问talker
6. talker发布消息


注意：
1. rcp远程调用和TCP
0--4：rpc
5,6：tcp

2. 步骤零一不可以调换

talker listener可以存在多个

3. talker和listener建立联系之后master可以关闭

4. 上述实现流程有的已经封装，直接调用就行


---
关注点
0. 大部分实现已经被封装
1. 话题设置
2. 关注发布者实现
3. 关注订阅者实现
4. 关注消息载体

#### 实操

关注：发布方、接收方、数据


- 发布方：
1. 新建功能包
2. 创建发布文件
3. 编写
    1. 包含头文件
    2. 初始化变量
    3. 创建节点句柄
    4. 编写发布逻辑并编写数据
'''
#include<ros/ros.h>
#include<std_msgs/String.h>
*发布文本也算一个头文件msgs/。string.h*
'''

创建节点句柄：
'''
ros::NodeHandle nh;
'''

创建发布对象：
'
ros::Publisher pub = nh.advertise<std_msgs::String>("定义话题名字"，缓存的长度)；
'''

编写发布数据：
'''

'''

- nh.advertise函数
与多个重载
- queue_size :长度（处理阻塞数据）
- 


4.
5.
6.

这是最基础的操作层面知识。你需要理解 ROS 消息（Message）是如何定义和传输的。

    消息定义：CameraInfo 是一个标准的 ROS 消息类型。你需要熟悉其字段结构：
        std_msgs/Header header：包含时间戳 (stamp) 和坐标系 ID (frame_id)。
        uint32 height, width：图像的像素尺寸。
        string distortion_model：畸变模型名称（如 "plumb_bob"）。
        float64[] D：畸变系数数组。
        float64[9] K：3x3 相机内参矩阵（行优先存储）。
        float64[9] R：3x3 旋转矩阵（通常用于立体校正，单目常为单位阵）。
        float64[12] P：3x4 投影矩阵。
    数据访问：在 C++ 或 Python 节点中，通过订阅者回调函数获取消息指针或对象后，直接访问成员变量。例如在 C++ 中访问 msg->K[0]


2. 相机成像几何模型（针孔相机模型）

提取出的数值并非孤立存在，它们代表了物理相机的几何属性。

    内参矩阵 KK：
    K=[fx0cx0fycy001]
    K=
    fx,0,0,
    0,fy,0,
    cx,cy,1
    
        fx,fyfx,fy：焦距（以像素为单位）。如果像素不是正方形，这两个值会不同。
        cx,cycx,cy：主点坐标（光轴与图像平面的交点），通常接近图像中心 (width/2,height/2)(width/2,height/2)。
        知识要点：理解这些参数如何将三维相机坐标系下的点 (X,Y,Z)(X,Y,Z) 投影到二维像素坐标系 (u,v)(u,v)。
    图像尺寸：width 和 height 定义了像素坐标系的边界。在进行像素坐标转换时，必须确保坐标点在 [0,width−1][0,width−1] 和 [0,height−1][0,height−1] 范围内，否则会导致越界错误。


 镜头畸变模型

D 数组和 distortion_model 描述了真实镜头与理想针孔模型之间的偏差。

    畸变模型：最常见的是 plumb_bob（即 OpenCV 中的 Brown-Conrady 模型），还有 equidistant（鱼眼模型）等。不同的模型对应不同的数学公式。
    畸变系数 DD：
        对于 plumb_bob，D=[k1,k2,t1,t2,k3]D=[k1,k2,t1,t2,k3]。
        k1,k2,k3k1,k2,k3：径向畸变系数，导致直线弯曲（桶形或枕形畸变）。
        t1,t2t1,t2（有时记为 p1,p2p1,p2）：切向畸变系数，由镜头安装倾斜引起。
        知识要点：理解这些系数用于去畸变（Undistortion）过程。在将像素点反投影到三维空间或进行立体匹配前，通常需要利用 KK 和 DD 将原始像素坐标映射到无畸变的归一化平面坐标。

4. 坐标系与时间同步（TF 与 Header）

header.stamp 和 header.frame_id 是将视觉数据融入机器人系统的关键。

    时间同步 (stamp)：
        视觉数据必须与其他传感器（如激光雷达、IMU、里程计）在时间上对齐。
        知识要点：在处理多传感器融合（如 VIO、SLAM）时，需利用 stamp 进行插值或匹配，确保处理的是同一时刻的状态。如果时间戳不准确或未同步，会导致轨迹漂移或建图错位。
    坐标系关联 (frame_id)
        标识图像数据所属的物理坐标系（通常是相机光学中心坐标系，如 camera_link 或 optical_frame）。
        知识要点：结合 TF (Transform) 树，可以将图像中的像素点转换到世界坐标系、基座标系 (base_link) 或地图坐标系 (map)。没有正确的 frame_id，就无法知道相机在机器人上的位置和朝向，也就无法进行空间定位。

5. 编程实现与库的使用

在实际代码中，通常结合以下库进行操作：

    OpenCV：ROS 的 cv_bridge 包负责将 sensor_msgs/Image 转换为 cv::Mat。虽然 CameraInfo 不直接包含图像数据，但它的 KK 和 DD 通常直接传递给 OpenCV 函数，如 cv::undistort()、cv::initUndistortRectifyMap() 或 cv::projectPoints()。
    Eigen 库：在 C++ 中，通常将 KK 数组转换为 Eigen::Matrix3d 以便进行矩阵运算。
    ROS API：使用 image_transport::CameraSubscriber 可以同时订阅图像和相机信息，确保两者在时间上和逻辑上是配对的，避免图像和内参不匹配的问题。




---
# 多终端打开仿真和rviz

- 终端1：
打开仿真
roslaunch smb_gazebo smb_gazebo.launch laser_enable:=true

- 终端2：开伪2d雷达（启动点云转换）
rosrun pointcloud_to_laserscan pointcloud_to_laserscan_node cloud_in:/rslidir_points

**启动前都需要激活devel**

- 终端3：(启动侦察兵)
rosrun smb_highlevel_controller scan_processer 

- 终端4：（启动保安）
rosrun smb——和ighlevel_controller obstacle_monitor



yaml
相当于键值对

rosparam load ~/catkin_ws/src/smb_highlevel_controller/config/yaml.yaml
rosparam list之后成功注册yaml到ros



launch文件

    启动节点

<launch>
    <node pkg="节点所在的功能包名称"  type="节点的可执行文件名称"  name="节点运行时的名称"/>
</launch>

<!-- 
    output:打印输出位置(screen)
    respawn:失效后是否重新启动
    required:是否把此节点设置为必须的节点
    ns:命名空间
    args:输入参数
 -->

参数
<!--  参数设置:单个参数设置、多个参数设置  -->
<param name="参数名" value="参数值"/>
<rosparam file="params.yaml" command="load" ns="params"/>

<!--  局部变量参数设置与调用  -->
<arg name="参数名" value="参数值" />
<param name="foo" value="$(arg arg-name)">


    重映射ROS计算图资源的命名

<remap from="原名" to="映射后的名称" />

    嵌套保护其他launch文件

<include file="$(dirname)other.launch"/>









2. 设置Fixed Frame参数

在Rviz中，Fixed Frame决定了整个可视化世界的参考坐标系。通常，该帧设置为“map”或“odom”等全局坐标系。
Frame Name	Description
map	全局地图坐标系，适用于SLAM或导航任务
odom	里程计坐标系，通常由运动模型估计而来
base_link	机器人本体坐标系，通常作为机器人的中心参考点

之前需要roscore（录制完毕之后）
录包：
rosbag record /homework/obstacle -O my_test.bag

播放1、
```bash
rosrun smb_highlevel_controller obstacle_monitor
```
播放2、
basht
rosbag play my_test.bag

动态参数展示：
rosrun rqt_reconfigure rqt_reconfigure

服务修改阈值：
rosparam get /warning_distance
(没有修改过的会输出1.0)

打电话修改阈值：
rosservice call /homework/set_warning_distance "distance: 3.0"


测试防止呆逻辑：输入负数

---------
## 第二次作业笔记

### cv_bridge

   1 namespace cv_bridge {
   2 
   3 class CvImage
   4 {
   5 public:
   6   std_msgs::Header header;  //存ros的头信息（时间戳/坐标ID）
   7   std::string encoding;     //encoding编码格式
   8   cv::Mat image;          //像素矩阵
   9 };
  10 
  11 typedef boost::shared_ptr<CvImage> CvImagePtr;
  12 typedef boost::shared_ptr<CvImage const> CvImageConstPtr;
  13 
  14 }



// Case 1: Always copy, returning a mutable CvImage
   2 CvImagePtr toCvCopy(const sensor_msgs::ImageConstPtr& source,
   3                     const std::string& encoding = std::string());
   4 CvImagePtr toCvCopy(const sensor_msgs::Image& source,
   5                     const std::string& encoding = std::string());
   6 
   7 // Case 2: Share if possible, returning a const CvImage
   8 CvImageConstPtr toCvShare(const sensor_msgs::ImageConstPtr& source,
   9                           const std::string& encoding = std::string());
  10 CvImageConstPtr toCvShare(const sensor_msgs::Image& source,
  11                           const boost::shared_ptr<void const>& tracked_object,
  12                           const std::string& encoding = std::string());








