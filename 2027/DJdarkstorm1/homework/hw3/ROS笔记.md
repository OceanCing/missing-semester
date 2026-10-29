# ROS项目流程

## 1. 建工作空间

```
mkdir catkin_ws/src
catkin init
catkin_make
```

## 2. 创建ros包

```
catkin_create_pkg <name> roscpp rospy <dependency>
catkin_make
```

## 3. 在src下新建源代码文件
## 4. 自定义msgs数据类型
#### 1. 创建my_msgs.msg文件

```
# my_msgs.msg
uint16 age
string name
flaot64 height
```

#### 2. 更新package.xml文件

```
增加
<build_depend>message_generation</build_depend>
<exec_depend>message_runtime</exec_depend>
```

#### 3. 更新CMakeLists.txt

```
增加
find_package(message_generation)
add_message_files(ObstacleDistance.msg)
generate_messages(
    DEPENDENCIES #自定义消息实现依赖于标准消息
    std_msgs
)
catkin_package(CATKIN_DEPENDS roscpp rospy std_msgs message_runtime)
```

#### 4. 编译一下

    ```
    catkin_make
    # 在 ~/workspace/devel/include/包名 文件夹下 生成对应的头文件
    ```

#### 5. 写发布者/订阅者

```
/*
1.包含头文件
2.初始化ros节点
3.创建节点句柄
4.创建发布者/订阅者对象
5.编写发布逻辑，发布数据
	5.1.创建被发布的数据
	5.2.设置发布频率
	5.3.循环发布数据		
6.(订阅)spin()
*/
```



## 5.服务通信

### 1. 自定义服务通信

#### 1. 数据
1.新建.srv文件
```xml
int32 num1# 客户端发送的请求数据,一定要按文件设置的格式发送数据，否则无法识别
int32 num2
---
int32 sum# 服务端发送的响应数据，运行时必须先启动服务器再启动客户端
```
> 若需要客户端挂起，等待服务器
>调用`client.waitForExistence();`
>或`ros::service::waitForService("myService");`

2.更新package.xml
```xml
<build_depend>message_generation</build_depend>
<exec_depend>message_runtime</exec_depend>
```
3.更新CMakeLists.txt
```txt
find_package(message_generation)
add_service_files(
    FILES
    myService.srv
)
generate_messages(
    DEPENDENCIES
    std_msgs
)
catkin_package(CATKIN_DEPENDS roscpp rospy std_msgs message_runtime)
#编译可执行文件时
add_dependencies(<myServer_node> ${PROJECT_NAME}_gencpp)
```

#### 2. 服务端实现

   ```txt
   1.包含头文件
   2.初始化节点
   3.创建节点句柄
   4.创建服务端对象
   5.处理请求响应
   6.spin()
   ```

#### 3. 客户端

```
1.包含头文件
2.初始化节点
3.创建节点句柄
4.创建客户端对象
5.提交响应请求
	5.1.组织请求
	5.2.处理响应
```

### 2.参数服务--"接线电话"

#### 1. 增删改查
1. 增加
```c++
ros::NodeHandle nh;
nh.setParam("key", value);
ros::param::set("key", value);
```
2. 修改
```c++
# 在设置后又设置即可覆盖修改
ros::NodeHandle nh;
nh.setParam("key", value);
ros::param::set("key", value);
```
3. 查找获取
```c++
ros::NodeHandle nh;
nh.param("key", 默认值);//存在，返回对应的值，否则返回默认值
nh.getParam("key",item)//存在，返回true,并将对应的值存在item中；否则仅返回false
nh.getParamCached("key",item);//同上，获取效率高,实时性差
nh.getParamName(vector<string>);获取所有的key放在vector中
nh.hasParam("key");//是否有该参数
nh.searchParam("key",is_has);

ros::param::param("key", value);//与上大同小异
ros::param::get("key", item);
ros::param::search("key", is_has);
......
```
4. 删除
```c++
ros::NodeHandle nh;
nh.delParam("key");//存在删除成功返回true，不存在删除返回false
ros::param::del("key");//存在删除成功返回true，不存在删除返回false
```
**6.在参数服务器修改参数不会立即修改话题里的参数，要重新向服务器发送请求才能获得最新参数**

#### 2.yaml参数文件的加载
1. 写`.yaml`文件
```yaml
smb_highlevel_controller://参数空间
    laser_topic: /scan
    pointcloud_topic: /rslidar_points
    queue_size: 10
    warning_distanse: 1.0
```
2. launch文件中加载（用rosparam命令也行）
```xml
<rosparam command="load" file="$(find 包名)/config/config.yaml" />

```
### 3. 动态参数
#### 1. 客户端实现
1. 写`.cfg`文件
```python
1.导入包
2.创建参数生成器
3.往参数生成器加入参数
4.配置节点，退出
e.g
#!/usr/bin/env python
# coding=utf-8

PACKAGE = "your_pkg_name"  # 替换为你的功能包名

from dynamic_reconfigure.parameter_generator_catkin import *

gen = ParameterGenerator()

# ====== 整数型 (int_t) ======
# gen.add(name, type, level, description, default, min, max)
gen.add("int_param",    int_t,    0, "An integer parameter",   50,  0,   100)

# ====== 浮点型 (double_t) ======
gen.add("double_param", double_t, 0, "A double parameter",    .5, 0.0, 1.0)

# ====== 布尔型 (bool_t) ======
gen.add("bool_param",   bool_t,   0, "A boolean parameter",   True)

# ====== 字符串型 (str_t) ======
gen.add("str_param",    str_t,    0, "A string parameter",    "Hello World")

# ====== 枚举型 (enum) ======
# 枚举基于整数，每个常量需要名称、类型、整数值、描述
size_enum = gen.enum([
    gen.const("Small",      int_t, 0, "Small size"),
    gen.const("Medium",     int_t, 1, "Medium size"),
    gen.const("Large",      int_t, 2, "Large size"),
    gen.const("ExtraLarge", int_t, 3, "Extra large size")
], "An enum to set size")

gen.add("size",         int_t,    0, "A size parameter",    0, 0, 3, edit_method=size_enum)

# 退出并生成代码：包名, 节点名, 生成文件名（对应生成 XxxConfig.h / .py）
exit(gen.generate(PACKAGE, "your_node_name", "MyNodeParams"))
```

2. 更新CMakeLists.txt
```txt
find_package(dynamic_reconfigure)
generate_dynamic_reconfigure_options(
    cfg/warning.cfg
)
```
3. 更新package.xml
```xml
<build_depend>dynamic_reconfigure</build_depend>
<exec_depend>dynamic_reconfigure</exec_depend>
```
#### 2.服务端实现
```c++
1.包含头文件
2.初始化
3.创建服务器对象
4.回调函数解析修改后的参数
5.spin()

//4.
void cb(smb_highlevel_controller::warningConfig &config, uint32_t level)
{
}
//3.
dynamic_reconfigure::Server<smb_highlevel_controller::warningConfig> dr_server;
dr_server.setCallback(boost::bind(&cb,_1,_2));//&cb回调函数

```

## 6.nodelet
### 1.nodelet插件

#### 1. 写nodelet_name.cpp
```c++
#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>

namespace nodelet_test
{
     class NodeletTest : public nodelet::Nodelet
     {
     public:
          NodeletTest(){}
          void onInit()//节点初始化
          {
               ROS_INFO("NodeletTest init------------------");
          }

     };
};
PLUGINLIB_EXPORT_CLASS(nodelet_test::NodeletTest, nodelet::Nodelet)//导出插件
```
#### 2. 写nodelet_name.xml

```xml
<library path="lib/libnodelet_test">
    <class type="nodelet_test::NodeletTest" base_class_type="nodelet::Nodelet" name="smb_highlevel_controller/NodeletTest">
        <description>自定义插件</description>
    </class>

</library>
```
### 2.配置
#### 1. 配置CMakeLists.txt

```
find_package(pluginlib nodelet)
add_library(插件名
        src/插件名.cpp
)
```

#### 2. 配置package.xml
```
  <export>
    <!-- Other tools can request additional information be placed here -->
    <!--标签名是基类的包名-->
    <nodelet plugin="${prefix}/nodelet_test.xml" />
  </export>
```



回答一些思考题(具体项目参见smb_highlevel_controller功能包)

**9./gezebo中 的传感器发布/rslidar_points话题，pointcloud_to_laserscan 节点接受并转化为 /scan话题，scan_processor_node接受并读取有效数据（最小值）发布给/homework/obstacle，由obstacle_monitor_node节点来处理障碍判断和规避
**
**10.pointcloud_to_laserscan 节点
作用：将雷达话题/rslidar_points数据转成点云数据并在/scan发布。
输入话题：/rslidar_points
输出话题：/scan
在rviz中/rslidar_points是三维立体的点阵，表现是一个面；/scan是2维，表现就是一条直线
**

**11.  保存了话题数据(Type: smb_highlevel_controller/ObstacleDistance
)，不包含服务调用和动态参数操作。**

**12.node & nodelet & manager**

node 会启动一个单独的进程（不同节点 PID不同）

nodelet的manager也是一个进程（不同manager PID不同）

nodelet插件是一个类、一个动态库.so

nodelet manager启动时会启动多个线程。（不同nodelet PID相同）e.g

```
[INFO] [1793250158.306200883]: Initializing nodelet with 32 worker threads.
```

然后加载nodelet插件(一个类、一个动态库.so)，但都是在一个进程里，访问manager进程里的内存
