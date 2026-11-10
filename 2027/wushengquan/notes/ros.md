# ROS

## 基本

**通信中间件**     **工具链**      **软件生态**

Linux和机器人之间的工具件和工具箱



相机驱动    ->    识别    ->    解算    ->    决策    ->     通信中枢

有驱动和硬件抽象     算法库   调试与可视化   仿真与回放

节点（干活的）      话题（它们之间传什么）

​           **上位机**      

​         **EtherCAT**

​              **电机**

## 架构

文件系统层

解决【代码怎么组织】：package.xml,CMakeLists,msg/srv,launch,config

计算图层     节点     话题    服务    参数    信息   

解决【程序怎么协作】：谁发布、谁订阅、谁提供什么服务

通信层

解决【数据怎么走】：连接建立、序列化、队列、断线重连

### 排错口诀

1. 找不到包？    ->文件系统层：source了吗？包在src里吗？
2. 连不上/收不到？->计算图层：名字对对不对？类型（传的信息等）对不对？
3. 延迟高/丢数据？->通信层：队列太浅？带宽够不够？
4. 节点一启动就退？->先看终端里第一行红色FATAL

**节点**   独立干活一个.cpp    rosnode list/info

**话题**   一个广播   发布者写，订阅者读    rostopic list/ echo

**消息**   .msg定义的字段    话题上传输数据结构    rosmsg show

**服务**   精准命令    rosservice call

**参数**   墙上的数据表  全局共享  key-value   rosparam get / set

---

记名字       名字查询     参数服务器（全局参数存放地（YAML在这））状态监控  

当管家  数据是节点间传

roscore = Master + rosout(日志) + 参数服务器，一条命令全起。

---

- **Controller Manager**
  - 加载、启动、切换控制器
  - 管理控制器的生命周期
  - spawner 一行起一个
- **常用 Controller**
  - joint_state / joint_trajectory / effort (PID) / diff_drive —— 直接用，不用自己写
  - effort_controllers/JointPositionController
- **RobotHW（自己写）**
  - 写一个硬件类，把 EtherCAT 或 CAN 的收发包成 read() / write()，对上位机就是透明
  - 队里一般自己写一个
- **joint_limits 与传动**
  - 限位、最大速度、加速度；传动比与「关节 ↔ 电机」的映射也在这里
  - 云台 / 底盘的限位都在这

**底部总结文字：**
**好处：换电机只改 hardware_interface，换算法只换 controller；仿真和实车共用同一套 controller。**

**话题 /cmd_vel**
controller 的输入

**Controller**
PID · 轨迹 · 差速

**RobotHW 接口**
read() / write()

**下位机 -> 电机**
CAN

------

**流程图（六个步骤）：**

- **① 启动 Master**
  roscore 起在 11311 端口，等别人来注册
  *(箭头指向 ②)*
- **② 发布者报到**
  节点启动即向 Master 注册：我发布 /cmd_vel，类型 Twist
  *(箭头指向 ③)*
- **③ 订阅者报到**
  订阅者注册：我要订阅 /cmd_vel
  *(箭头指向 ⑤)*
- **④ Master 回地址**
  把发布者的 IP 和端口告诉订阅者（只此一次）
  *(箭头指向 ⑤)*
- **⑤ 订阅者去连接**
  订阅者主动连发布者，握手确认消息类型一致
  *(箭头指向 ⑥)*
- **⑥ 直连传数据**
  之后每一条消息都走这条 TCP 连接，Master 不再出现

---

dynamicx@standard:~/catkin_ws$ catkin_ws —— 工作空间 (workspace)
catkin_ws/
├── src/            # 你的代码都在这里 (进版本库)
│   ├── CMakeLists.txt  ← 软链接到 /opt/ros/...
│   └── my_pkg/         ← 一个功能包
├── build/          # 编译中间文件 (可删，别进 git)
├── devel/          # 编译产物: setup.bash、可执行文件
├── setup.bash      # 每开一个终端都要 source 它

└──包

---

**源码和产物分离**
src 进 Git，build/devel 不进。别人 clone 下来重新编译即可

**每次开终端先 source**
source devel/setup.bash，或者写进 ~/.bashrc 一劳永逸

**一个空间可以放很多包**
包名必须全局唯一，否则 rosrun 会认错

上车部署：git clone 到上位机 -> catkin_make -> source devel/setup.bash。车上没有图形界面，编译和运行都在 SSH 里做。

---

环境变量+包清单=找的到包

---

## 建包

dynamicx@standard:~/catkin_ws$ package —— 功能包 (ROS 里最小的复用单位)

```
cd ~/catkin_ws/src

catkin_create_pkg smb_highlevel_controller roscpp rospy std_msgs cv_bridge image_transport tf2 dynamic_reconfigure rqt_gui rviz rosbag


```

my_pkg/
├── CMakeLists.txt  # 编译规则：编什么、链什么
├── package.xml     # 名片：名字、版本、依赖
├── include/my_pkg/ # 对外暴露的头文件 .h /.hpp
├── src/            # C++ 源文件 .cpp
├── scripts/        # 可执行脚本 (我们以C++为主，这里放小工具)
├── msg/            # 自定义消息 .msg
├── srv/            # 自定义服务 .srv
├── launch/         # 启动文件 .launch (一次拉起多个节点)
├── config/         # 参数文件 .yaml
└── README.md       # 给人看的：怎么编、怎么跑

**include vs src**
include 是「给别人用的」，src 是「给自己编的」；只写 src 也能跑，但别把 .h 乱塞

**msg / srv 要配合编译**
光建目录没用，CMakeLists 里要 add_message_files 才会生成头文件

**launch / config 不进编译**
但它们决定了「怎么跑」，是别人接手包时最先看的东西

服务目录在 ROS 里叫 srv/，别建成 service/，编译时会找不到。

- **name**
  包名，必须和文件夹同名；改了文件夹不会自动改这里

- **build_depend**
  编译这个包时需要的东西：头文件、库、消息包

- **exec_depend**
  运行时需要的东西：动态库、消息运行库、launch 里调用的包

- **depend**
  = build + build_export + exec，一般写这个最省事

- **自定义 msg/srv**
  要额外加 message_generation（编译期）和 message_runtime（运行期）

  

  坑：包名不能有中文、空格和大写歧义；新建包后先编译一次再写代码。



**选择口诀：连续的数据流 → 话题；要一次结果 → 服务；配置项 → 参数；跑得久、要进度、能取消 → action。**



# main函数里面

## 基本

```cpp
int main(int argc, char  *argv[])
{   
    //设置编码
    setlocale(LC_ALL,"");

    //2.初始化 ROS 节点:命名(唯一)
    // 参数1和参数2 后期为节点传值会使用
    // 参数3 是节点名称，是一个标识符，需要保证运行后，在 ROS 网络拓扑中唯一
    ros::init(argc,argv,"node_name");
    //3.实例化 ROS 句柄
    ros::NodeHandle nh;//该类封装了 ROS 中的一些常用功能
    
    
    
    //body
    
    
    
    
    


    return 0;
}
```

  

## 发布

标号和其他一一对应

```cpp
//4.实例化 发布者 对象
    //泛型: 发布的消息类型
    //参数1: 要发布到的话题
    //参数2: 队列中最大保存的消息数，超出此阀值时，先进的先销毁(时间早的先销毁)
    ros::Publisher pub = nh.advertise<std_msgs::String>("topic_name",10);//<-保留10条
/*std_msgs::String  是 （包名) :: (具体的消息类)。std_msgs：最基础的包。比如std_msgs::String（字符串）、std_msgs::Int32（整数）、std_msgs::Bool（布尔值）等，一般带有不少包都可以调或自定义，总之就是这个结构来用（包名) :: (具体的消息类)*/



    //5.组织被发布的数据，并编写逻辑发布数据
    //数据(动态组织)
    std_msgs::String msg;
demo02_talker_listener::Person p;
    p.name = "sunwukong";
    p.age = 2000;
    p.height = 1.45;
//和类一样新建一个信息
/*只要你要发送（发布）一条消息，就必须先“造”一个对应类型的对象出来，把数据装进去，然后再发送。

用 . 访问数据：msg.data = "hello"，data 就是这个类里的成员变量（属性）。

用 -> 访问指针：之前回调函数里的 msg_p->data，因为传进来的是类对象的指针（ConstPtr），所以用 -> 访问成员。
建对象之后   对象.定义信息文件里面的那个名*/



//发布消息（有时加在while）
    pub.publish(msg);
    //加入调试，打印发送的消息
    ROS_INFO("发送的消息:%s",msg.data.c_str());

```



### msg

话题通讯自定义信息类

ros::Publisher pub = nh.advertise< std_msgs::String>("topic_name",10);里面的std_msgs::String中的String

首先在包里建msg文件夹，建.msg文件

内容：

类型 名字

 例：

float32 min_distance
bool valid



### srv

服务通信自定义

首先在包里建srv文件夹，建.srv文件

内容：

类型 名字

 例：

```cpp
float32 distance//传入
---
bool success//传出
string message
```





### 参数操作（黑板）

YAML是初始数值

**改数值**`nh.setParam(键名, 值)`：这是往参数服务器里存数据的其中一种写法。

这里换了一种写法：`ros::param::set(键名, 值)`

**取值**  :  

```cpp
// NodeHandle 写法
int res1 = nh.param("nh_int", 100); 
// 全局写法
int res3 = ros::param::param("param_int", 20); 

nh.param<double>("/my_node/max_speed", speed, 1.0);//参数服务器       提前定义好的变量   如果没有取的数值

nh.getParamCached("nh_int", nh_int_value);//带缓存的读取
ros::param::getCached("param_int", param_int_value);
```

​                                                         

```cpp
nh.param<type>(name, var, default)
```

**判断参数是否存在 (`hasParam`)**

```cpp
if (ros::param::has("/my_node/max_speed")) {
    // 存在则执行某些逻辑
}
bool exist = nh.hasParam("nh_int"); 
bool exist2 = ros::param::has("param_int");
```

**搜索参数**（结合命名空间）

```cpp
std::string key;
nh.searchParam("nh_int", key);
//它会从当前节点的命名空间开始，逐级向上（直到全局根目录）去寻找这个参数。
```



**删除参数 (`del` / `delete_parameter`)**

cpp

```cpp
ros::param::del("/my_node/max_speed"); 
bool r1 = nh.deleteParam("nh_int");
    ROS_INFO("nh 删除结果:%d",r1);

    bool r2 = ros::param::del("param_int");
    ROS_INFO("param 删除结果:%d",r2);
```

---

### 动态参数配置 (Dynamic Reconfigure)

```cpp
#include "dynamic_reconfigure/server.h"
#include <smb_highlevel_controller/switchConfig.h>
// 首先定义 CallbackType 类型的变量
dynamic_reconfigure::Server<smb_highlevel_controller::switchConfig> dr_server;/*尖括号里需要填写编译生成的动态参数配置类（通常与 .cfg 文件同名，首字母大写并加上 Config 后缀）。*/

void cb(smb_highlevel_controller::switchConfig& config, uint32_t level)//回调
dr_server.setCallback(cbType);// 最后设置回调

bool alarm_enabled = config.alarm_enabled; //定为全局好
    ROS_INFO("当前报警开关状态: %s", alarm_enabled ? "开启" : "关闭")

```



---

### tips：

**命名空间**

**绝对路径（全局）：** `"/max_speed"`。所有节点都能看到，不管谁启动的。

**相对路径：** `"max_speed"`。会解析为当前节点的命名空间，比如 `/my_robot_node/max_speed`。

**私有路径（最常用）：** `"~max_speed"`。只在当前节点内部有效，解析为 `/my_robot_node/max_speed`。这是为了防止不同节点使用同名参数冲突的最佳实践。

---

| **`ROS_DEBUG`** | 默认隐藏 | 调试信息（最低） | 开发阶段查看内部变量，平时不输出。                     |
| --------------- | -------- | ---------------- | ------------------------------------------------------ |
| **`ROS_INFO`**  | 白色     | 普通信息         | 节点启动、状态变化、正常运行的提示。                   |
| **`ROS_WARN`**  | 黄色     | 警告信息         | 程序没崩，但有隐患（如：电量低、丢数据）。             |
| **`ROS_ERROR`** | 红色     | 错误信息         | 功能失败（如：未找到设备、通信断开），节点未死。       |
| **`ROS_FATAL`** | 红底白字 | 致命错误（最高） | 无法恢复的错误（如：核心驱动加载失败），通常伴随崩溃。 |



## 订阅



```cpp
//4.实例化 订阅者 对象
    ros::Subscriber sub = nh.subscribe<std_msgs::String>("topic_name",10,doMsg);
    //5.处理订阅的消息(回调函数)
```



### 结尾

**（一般，用ros::spin();也有，看主体）**

```cpp
int count = 0; //消息计数器
//逻辑(一秒10次)
ros::Rate r(1);

//节点不死
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
    //暂无应用
    ros::spinOnce();
}
```

# CMakeLists.txt 

> 1. `add_executable` 必须配对 `target_link_libraries`（否则链接不到 ROS 库）。
> 2. 用到自定义 `msg`、`srv` 或 `dynamic_reconfigure`，**强烈建议加上 `add_dependencies`**，防止编译时找不到生成的头文件。
> 3. 注意 CMake 注释使用 `#`，变量之间必须留空格。

---

## 一、 节点编译基础模板

### 1. 基础节点 (无自定义消息/服务)

```cmake
add_executable(node_name src/node_name.cpp)
target_link_libraries(node_name
  ${catkin_LIBRARIES}
)
```

### 2. 节点依赖自定义 msg

```cmake
add_executable(node_name src/node_name.cpp)
add_dependencies(node_name 
  ${PROJECT_NAME}_generate_messages_cpp
)
target_link_libraries(node_name ${catkin_LIBRARIES})
```

### 3. 节点依赖自定义 srv

```cmake
add_executable(node_name src/node_name.cpp)
add_dependencies(node_name 
  ${PROJECT_NAME}_gencpp 
  ${catkin_EXPORTED_TARGETS} # 两个变量之间必须有空格
)
target_link_libraries(node_name ${catkin_LIBRARIES})
```

### 4. 节点依赖动态参数 (Dynamic Reconfigure)

```cmake
add_executable(demo01_dr_server src/demo01_dr_server.cpp)
add_dependencies(demo01_dr_server 
  ${${PROJECT_NAME}_EXPORTED_TARGETS} 
  ${catkin_EXPORTED_TARGETS} # 两个变量之间必须有空格
)
target_link_libraries(demo01_dr_server ${catkin_LIBRARIES})
```

### 5. 终极缝合怪 (同时依赖 msg + srv + 动态参数)

> **注意**：`${${PROJECT_NAME}_EXPORTED_TARGETS}` 已经包含了 msg 和 srv 的所有生成目标，用这一条即可一劳永逸。

```cmake
add_executable(obstacle_monitor src/obstacle_monitor.cpp)
add_dependencies(obstacle_monitor 
  ${${PROJECT_NAME}_EXPORTED_TARGETS} 
  ${catkin_EXPORTED_TARGETS} # 这一条包含了 msg 和 srv 的所有生成目标
)
target_link_libraries(obstacle_monitor ${catkin_LIBRARIES})
```

---

## 二、 复杂工程必备配置项

如果项目中包含了自定义消息、服务或动态参数，除了编写上述节点编译代码外，还需要在 `CMakeLists.txt` 的开头完成以下配置：

### 1. 查找依赖包 (`find_package`)

必须把消息、服务、动态参数相关的包加上：

```cmake
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation   # 用于生成 msg 和 srv
  dynamic_reconfigure  # 用于动态参数
)
```

### 2. 消息与服务生成 (`add_message_files` / `add_service_files`)

```cmake
add_message_files(
  FILES
  ObstacleDistance.msg
)

add_service_files(
  FILES
  SetWarningDistance.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs  # 如果你的 msg 里用到了 float32 等标准类型，必须加这个
)

# 动态参数生成配置
generate_dynamic_reconfigure_options(
  cfg/switchConfig.cfg
)
```

### 3. 别忘了 `package.xml`

`CMakeLists.txt` 改完后，必须在 `package.xml` 中加上相应的依赖，否则 `catkin_make` 会报错：

```xml
<build_depend>message_generation</build_depend>
<build_depend>dynamic_reconfigure</build_depend>
<exec_depend>message_runtime</exec_depend>
<exec_depend>dynamic_reconfigure</exec_depend>
```

---

## (避坑)

1. **注释符错误**：CMake 的注释是 `#`，千万不要写成 `//`（如 `src/node.cpp//<-文件名` 会导致路径解析错误）。
2. **括号未闭合**：`add_executable(node_name src/node.cpp)` 末尾一定要检查是否有 `)`。
3. **变量之间缺空格**：`${PROJECT_NAME}_gencpp${catkin_EXPORTED_TARGETS}` 是错的，必须写成 `${PROJECT_NAME}_gencpp ${catkin_EXPORTED_TARGETS}`，否则 CMake 会将其视为一个不存在的超长变量名。



# ROS 1 YAML 

> **核心逻辑**：在包里建 `config` 文件夹并创建 `.yaml` 文件 -> 在 `.launch` 文件中通过 `<rosparam>` 加载 -> 在 C++/Python 代码中读取。

在 ROS 1 中，`.yaml` 文件主要用于**配置参数**（存储初始数值），加载后存入参数服务器，最后由节点代码读取。

---

## 一、 YAML 文件的基本语法规范

1. **键值对**：使用冒号 `:` 分隔键和值，冒号后**必须加一个空格**。
2. **层级关系**：使用**空格**进行缩进（通常为 2 个空格，**绝对不能用 Tab 键**）。
3. **数据类型**：YAML 会自动推断类型（整数、浮点数、布尔值、字符串、列表、字典）。
4. **注释**：使用 `#`。

### 示例：

```yaml
# 1. 基础数据类型
min_limit: 1.5            # 浮点数 (对应代码里的 float/double)
max_retries: 10           # 整数 (对应代码里的 int)
alarm_enabled: true       # 布尔值 (对应代码里的 bool，也可以用 True/TRUE)
robot_name: "smb_robot"   # 字符串 (对应代码里的 string，引号可选)

# 2. 列表/数组 (对应 C++ 里的 std::vector)
sensor_ids: [1, 2, 3, 4]

# 3. 嵌套字典/命名空间 (对应 C++ 里的 std::map 或带命名空间的参数)
controller:
  p_gain: 0.8
  i_gain: 0.1
  d_gain: 0.05

# 4. 私有参数 (对应私有 NodeHandle "~" 或代码里的 ~ 前缀)
~private_param: "only_for_this_node"
```

---

## 二、 如何在 Launch 文件中加载 YAML

写好 YAML 后，必须在 `.launch` 文件中将其加载到 ROS 参数服务器。

### 1. 加载到全局命名空间 (根目录 `/`)

```xml
<launch>
    <!-- 将 yaml 文件里的所有参数加载到全局，比如变成 /min_limit -->
    <rosparam file="$(find your_package_name)/config/params.yaml" command="load" />
</launch>
```

### 2. 加载到节点的私有命名空间 (推荐做法)

为了防止不同节点之间参数名冲突，通常将参数加载到对应节点自己的命名空间下。

```xml
<launch>
    <node pkg="smb_highlevel_controller" type="obstacle_monitor" name="obstacle_monitor" output="screen">
        <!-- 使用 ns 属性将参数放在 /obstacle_monitor/ 下，比如变成 /obstacle_monitor/min_limit -->
        <rosparam file="$(find your_package_name)/config/params.yaml" command="load" ns="obstacle_monitor" />
    </node>
</launch>
```

---

## 三、 YAML 与 C++ 代码的联动对应关系

结合之前的 C++ 代码（`obstacle_monitor.cpp`），参数读取的对应关系如下：

| YAML 中的写法                    | Launch 中的 `ns`        | C++ 代码中的读取方式 (NodeHandle `nh1`)          | 最终解析的绝对路径                |
| :------------------------------- | :---------------------- | :----------------------------------------------- | :-------------------------------- |
| `min_limit: 1.5`                 | `ns="obstacle_monitor"` | `nh1.param<float>("min_limit", lim, 0);`         | `/obstacle_monitor/min_limit`     |
| `alarm_enabled: true`            | `ns="obstacle_monitor"` | `nh1.param<bool>("alarm_enabled", flag, false);` | `/obstacle_monitor/alarm_enabled` |
| `controller:`<br>`  p_gain: 0.8` | 无 (加载到全局)         | `nh1.getParam("/controller/p_gain", p);`         | `/controller/p_gain`              |

### C++ 代码读取复杂结构示例

如果你的 YAML 里写了嵌套结构或列表，C++ 读取代码如下：

```cpp
// 读取列表 (vector)
std::vector<int> ids;
nh1.getParam("sensor_ids", ids);

// 读取字典 (map)
std::map<std::string, double> controller_gains;
nh1.getParam("controller", controller_gains);

// 或者单独读取嵌套字典中的某个键
double p_gain;
nh1.param<double>("controller/p_gain", p_gain, 0.0);
```

---

## 四、 避坑指南 (YAML 常见错误)

1. **缩进错误**：千万**不要混用 Tab 和空格**，否则 ROS 解析 YAML 时会直接报错 (`Error: Invalid YAML`)。统一使用 2 个或 4 个空格。
2. **冒号后缺空格**：`min_limit:1.5` 是错的，必须是 `min_limit: 1.5`。
3. **类型不匹配**：如果 YAML 里写了 `min_limit: 1.5` (浮点型)，但在 C++ 代码里强行用 `nh1.param<int>("min_limit", ...)` 去读，会导致读取失败或类型转换异常。
4. **路径找不到**：Launch 文件里的 `$(find 包名)` 必须确保包名拼写完全正确，且包已经被 `catkin_make` 编译过。




# Launch 文件

## 核心语法

### 1. 参数加载：`<rosparam>`

```xml
<rosparam file="$(find smb_highlevel_controller)/config/params.yaml" command="load" />
```

*   **作用**：将 `params.yaml` 文件中的参数加载到 ROS 参数服务器中。

### 2. 节点启动：`<node>`

```xml
<node pkg="smb_highlevel_controller" type="scan_processor" name="scan_processor" output="screen" />
<node pkg="smb_highlevel_controller" type="obstacle_monitor" name="obstacle_monitor" output="screen" />
```

*   **属性解析**：
    *   `pkg`：节点所在的 ROS 包名（必须是 `catkin_make` 编译过的包）。
    *   `type`：可执行文件的名字（通常在 `CMakeLists.txt` 中通过 `add_executable` 定义）。
    *   `name`：节点在 ROS 网络中的运行名称（**非常重要**，决定了节点的私有命名空间。如果你在 `CMakeLists.txt` 中把可执行文件命名为 `obstacle_monitor`，这里可以随便改名，例如 `name="my_monitor"`）。
    *   `output="screen"`：将节点内部的 `ROS_INFO`、`ROS_WARN` 等日志直接打印到终端屏幕上。如果不加，日志会被写入 `~/.ros/log/` 下的日志文件中。

### 3. 可视化：RViz

```xml
<node pkg="rviz" type="rviz" name="rviz" args="-d $(find smb_highlevel_controller)/config/homework_rviz.rviz" />
```

*   **作用**：启动 RViz。
*   **关键参数**：`args="-d 路径"`，表示启动时自动加载指定的 `.rviz` 配置文件，省去每次手动添加 Topic 和 Display 的麻烦。


