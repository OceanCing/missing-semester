# ROS笔记

## 1. ROS Master 与通信流程
Master 负责名称注册和查询，帮助节点发现彼此；参数服务器提供配置存取。话题数据由节点直接传输，不经过 Master。

### 1.1 话题通信的建立过程：
1. 启动 Master。
2. 发布者注册发布信息。
3. 订阅者注册订阅信息。
4. Master 向订阅者提供发布者地址。
5. 订阅者与发布者协商传输方式并建立连接，检查消息类型是否匹配。
6. 节点之间直接传输数据。

### 1.2 节点启动与管理命令
| 命令 | 作用 |
| --- | --- |
| `roscore` | 启动 ROS 基础服务 |
| `rosrun <包名> <可执行文件> [参数…]` | 运行节点 |
| `roslaunch <包名> <启动文件.launch>` | 按启动文件运行节点和配置 |
| `rosnode list` | 列出已注册的节点 |
| `rosnode info <节点名>` | 查看节点的发布、订阅和服务信息 |
| `rosnode ping <节点名>` | 检查节点是否响应 |
| `rosnode kill <节点名>` | 请求关闭节点 |
## 2. 文件系统与工作空间
### 2.1 工作空间
| 目录 | 作用 |
| --- | --- |
| `src/` | 功能包源码 |
| `build/` | 编译中间文件 |
| `devel/` | 开发环境中的编译产物及环境脚本 |

### 2.2 功能包
| 文件或目录 | 作用 |
| --- | --- |
| `package.xml` | 声明包名、版本、维护者、许可证和依赖 |
| `CMakeLists.txt` | 定义编译、消息生成和链接规则 |
| `include/<包名>/` | 头文件 |
| `src/` | C++ 源文件 |
| `scripts/` | 可执行脚本与辅助工具 |
| `msg/` | 自定义消息定义 |
| `srv/` | 自定义服务定义 |
| `launch/` | 启动配置文件 |
| `config/` | 参数配置文件 |
| `README.md` | 编译和使用说明 |
### 2.3 package.xml
| 标签 | 作用 |
| --- | --- |
| `name` | 声明包名 |
| `buildtool_depend` | 声明构建工具依赖 |
| `build_depend` | 声明编译依赖 |
| `exec_depend` | 声明运行依赖 |
| `depend` | 同时声明编译、导出和运行依赖 |

### 2.4 CmakeLists.txt
| 指令 | 作用 |
| --- | --- |
| `cmake_minimum_required()` | 指定 CMake 最低版本 |
| `project()` | 声明项目名称 |
| `find_package()` | 查找 catkin 及所需依赖 |
| `add_message_files()` | 登记自定义消息文件 |
| `add_service_files()` | 登记自定义服务文件 |
| `generate_messages()` | 生成消息和服务对应的代码 |
| `catkin_package()` | 声明包的导出信息与依赖 |
| `include_directories()` | 添加头文件搜索路径 |
| `add_executable()` | 创建可执行目标 |
| `target_link_libraries()` | 为目标链接库 |
| `add_dependencies()` | 设置目标之间的构建依赖 |

### 2.5 常用命令
| 命令 | 作用 |
| --- | --- |
| `rospack find <包名>` | 查找功能包路径 |
| `roscd <包名>` | 切换到功能包目录 |
| `rosls <包名>` | 查看功能包目录内容 |
| `catkin_create_pkg <包名> [依赖包…]` | 创建功能包 |
| `catkin_make` | 在工作空间根目录编译 |
| `catkin build` | 使用 catkin_tools 编译 |
| `source devel/setup.bash` | 加载当前工作空间环境 |

## 3. 三种通信机制
### 3.1 话题通信
- 发布者负责发送消息，订阅者通过回调函数处理消息。
- 话题名和消息类型需要匹配。
- 队列用于缓冲消息，满时可能丢弃消息。
- 回调中长时间执行任务会影响后续消息处理。
- 晚启动的订阅者通常接收之后发布的消息；启用 latch 时，新订阅者还会收到发布者保留的最后一条消息。

| 命令 | 作用 |
| --- | --- |
| `rostopic list` | 列出话题 |
| `rostopic echo <话题名>` | 输出话题消息内容 |
| `rostopic info <话题名>` | 查看消息类型、发布者与订阅者 |
| `rostopic hz <话题名>` | 测量话题接收频率 |
| `rostopic pub <话题名> <消息类型> '<消息内容>'` | 从命令行发布消息 |
| `rostopic pub -r <频率> <话题名> <消息类型> '<消息内容>'` | 按指定频率持续发布消息 |
| `rosmsg show <包名/消息类型>` | 查看消息字段定义 |

### 3.2 服务通信
- 服务端提供处理能力，客户端主动调用。
- .srv 文件用 --- 分隔请求与响应。
- 普通服务调用会阻塞当前调用线程，直到完成或失败。
- 调用前可以等待服务上线；等待上线与等待调用完成是不同阶段。
- 服务适合一次请求与结果返回，不适合作为持续数据流的主要传输方式。

| 命令 | 作用 |
| --- | --- |
| `rosservice list` | 列出服务 |
| `rosservice type <服务名>` | 查看服务类型 |
| `rosservice call <服务名> ['<请求内容>']` | 调用服务 |
| `rosservice find <服务类型>` | 查找指定类型的服务 |
| `rossrv show <包名/服务类型>` | 查看请求和响应字段 |

- **.srv文件：**
```
float32 x #请求
---
string name #响应
```

### 3.3 参数服务器
- 使用键值对保存配置，按树形命名空间组织。
- 支持整数、浮点数、布尔值、字符串、列表和字典。
- 参数通常随参数服务器存在，重启后需重新加载。
- 修改参数不会自动更新节点已读取的变量，节点需要再次读取。
- 适合配置存取，不适合高频数据交换。

| 命令 | 作用 |
| --- | --- |
| `rosparam list` | 列出参数 |
| `rosparam get <参数名或命名空间>` | 读取参数或参数子树 |
| `rosparam set <参数名> <值>` | 设置参数 |
| `rosparam load <文件.yaml> [命名空间]` | 从 YAML 加载参数 |
| `rosparam dump <文件.yaml> [命名空间]` | 将参数导出为 YAML |
| `rosparam delete <参数名或命名空间>` | 删除参数或参数子树 |
| `rosparam list \| grep <关键词>` | 按关键词筛选参数名称 |

## 4. YAML 配置文件
YAML 用于以易读的形式组织配置数据。
- 使用空格缩进，不使用 Tab，同层内容保持对齐。
- 键值对中的冒号后留空格。
- 使用 # 表示注释。
- 推荐统一使用 true、false 表示布尔值。
- 注意数字、字符串等类型的区别，必要时用引号明确字符串。
- --- 可作为文档分隔符。
### 4.1 三种基本结构：
| 结构 | 含义 |
| --- | --- |
| 标量 | 单个值 |
| 列表 | 按顺序排列的多个值 |
| 字典 | 由键值对组成的集合，可嵌套 |


### 4.2 三种加载方式
ROS 中的配置流程是：编写 YAML，通过命令行或 launch 加载到参数服务器，再由节点读取。
launch 中使用 <rosparam> 加载配置；file 指定路径，command="load" 指定加载操作，ns 指定命名空间。$(find <包名>) 用于定位功能包。加载位置需要与代码中的参数访问路径对应。
#### 4.2.1 命令行加载
```
rosparam load config/params.yaml /my_ns
```
#### 4.2.2 launch 文件加载
```
<launch>
  <rosparam file="$(find my_pkg)/config/params.yaml"
            command="load" ns="my_ns"/>
</launch>
```
#### 4.2.3 代码中读取
```
ros::NodeHandle nh;
int exposure;
nh.param<int>("/my_ns/camera/exposure", exposure, 30);
```

## 5. launch文件
launch 使用 XML 格式，统一启动节点、设置参数和加载配置。
### 5.1 常用标签
| 标签 | 作用 |
| --- | --- |
| `<launch>` | 根标签 |
| `<node>` | 启动节点 |
| `<arg>` | 定义或传递启动参数 |
| `<param>` | 设置 ROS 参数 |
| `<rosparam>` | 加载 YAML 配置 |
| `<include>` | 引用其他 launch 文件 |
| `<group ns="...">` | 分组并设置命名空间 |
| `<remap>` | 重映射话题、服务等名称 |

### 5.2 node 常用属性
| 属性 | 作用 |
| --- | --- |
| `pkg` | 所属功能包 |
| `type` | 可执行文件名 |
| `name` | 运行时节点名 |
| `args` | 程序命令行参数 |
| `output="screen"` | 输出到终端 |
| `ns` | 命名空间 |
| `respawn="true"` | 退出后自动重启 |
| `required="true"` | 退出时关闭整个 launch |
### 5.3 arg,args和param的区别
| 写法 | 用途 | 交给谁使用 |
|---|---|---|
| `<arg .../>` | 定义或传递 launch 的启动参数 | launch 文件 |
| `<node args="..."/>` | 向程序传递命令行参数 | 被启动的程序 |
| `<param .../>` | 将参数写入 ROS 参数服务器 | 节点通过参数接口读取 |

## 6. nodelet
### 6.1 基本概念
- nodelet 是加载到 manager 进程中运行的插件。
- 多个 nodelet 可以运行在同一个 manager 中。
- 同一进程内满足条件时，可通过共享指针实现消息零拷贝，适合图像、点云等大数据。
- 一个 nodelet 崩溃可能影响同一 manager 中的其他 nodelet。
### 6.2 代码结构
- 继承 nodelet::Nodelet。
- 重写 onInit()，在其中读取参数、创建发布者和订阅者。
- 使用 getNodeHandle() 获取普通句柄。
- 使用 getPrivateNodeHandle() 获取私有句柄，读取自身参数。
- 使用 PLUGINLIB_EXPORT_CLASS 导出插件类。
- 不需要自己编写 main()，也不需要调用 ros::init() 和 ros::spin()。
- onInit() 应及时返回，不放阻塞循环。
### 6.3 编译与注册
- add_library()：将 nodelet 源码编译为库。
- 插件 XML：指定共享库路径、插件名称和 C++ 类名。
- package.xml：通过 <export> 登记插件 XML。
- catkin_package(LIBRARIES ...)：向其他包导出库信息，本身不负责创建库。
### 6.4 launch 启动
```
<launch>
  <node pkg="nodelet" type="nodelet" name="my_manager"
        args="manager" output="screen"/>

  <node pkg="nodelet" type="nodelet" name="processor"
        args="load my_pkg/MyNodelet /my_manager"
        output="screen">
    <param name="queue_size" value="10" type="int"/>
  </node>
</launch>
```
- args="manager"：启动 manager。
- load：向 manager 请求加载插件。
- my_pkg/MyNodelet：插件 XML 中的插件名称。
- /my_manager：目标 manager 名称。
- processor：此次加载的 nodelet 实例名。
- queue_size 的路径为 /processor/queue_size，使用 getPrivateNodeHandle() 读取。

### 6.5常用命令
| 功能 | 命令 |
|---|---|
| 启动 manager | `rosrun nodelet nodelet manager __name:=homework_manager` |
| 加载插件 | `rosrun nodelet nodelet load smb_highlevel_controller/ScanProcessorNodelet /homework_manager __name:=scan_processor_nodelet` |
| 查看已加载的插件 | `rosservice call /homework_manager/list "{}"` |
| 卸载插件 | `rosrun nodelet nodelet unload /scan_processor_nodelet /homework_manager` |
| 单独运行插件 | `rosrun nodelet nodelet standalone smb_highlevel_controller/ScanProcessorNodelet __name:=scan_standalone` |
| 查看可发现的插件类型 | `rosrun nodelet declared_nodelets` |

## 7.dynamic_reconfigure

### 7.1 简介
dynamic_reconfigure 用于在节点运行时修改参数，无需重启节点。参数变化后触发回调函数，使程序及时应用新配置。
### 7.2 使用流程
1. 添加依赖：在 package.xml 和 CMakeLists.txt 中声明 dynamic_reconfigure。
2. 编写配置：在 cfg/ 下创建 .cfg 文件，定义参数名称、类型、默认值和范围，如：
```
#!/usr/bin/env python
PACKAGE = "smb_highlevel_controller"

from dynamic_reconfigure.parameter_generator_catkin import *

gen = ParameterGenerator()
gen.add("warning_distance", double_t, 0,
        "Warning distance", 1.0, 0.1, 5.0)

exit(gen.generate(PACKAGE, PACKAGE, "warning"))
```
3. 配置编译：使用 generate_dynamic_reconfigure_options() 生成配置代码，并为相关目标添加构建依赖。
4. 创建服务端：包含生成的配置头文件，创建并保持 Server 存活。
5. 注册回调：在回调中读取配置值、更新程序变量；注册时会先执行一次。
6. 编译并启动节点，打开调参界面：
   `rosrun rqt_reconfigure rqt_reconfigure`
7. 修改参数：选择对应节点，调整参数，通过回调应用新配置。

## 8. URDF
URDF 使用 XML 描述机器人的结构，文件通常放在 urdf/ 目录。
### 8.1 基本组成
| 内容 | 作用 |
| --- | --- |
| `link` | 刚性部件 |
| `joint` | 连接两个 link，定义相对位置和运动方式 |
| `visual` | 外观 |
| `collision` | 碰撞形状 |
| `inertial` | 质量、质心和惯性矩阵 |
| `origin` | 相对位置 `xyz` 和姿态 `rpy` |
| `axis` | 关节运动轴 |
| `limit` | 关节位置、速度和力或力矩限制 |


- 关节常用类型：fixed 固定、revolute 有限旋转、continuous 连续旋转、prismatic 直线移动。
- 模型为树形结构，不能形成闭环；长度用米，角度用弧度。
- Xacro 通过变量和宏减少重复内容，使用前展开为 URDF。
### 8.2 使用流程与命令
加载模型到 robot_description → 发布 /joint_states → robot_state_publisher 计算并发布 TF → RViz 显示。
joint_state_publisher 提供关节状态，robot_state_publisher 提供坐标变换。
| 命令 | 作用 |
| --- | --- |
| `check_urdf robot.urdf` | 检查模型结构 |
| `rosrun xacro xacro robot.urdf.xacro > robot.urdf` | 展开 Xacro |
| `rosrun joint_state_publisher_gui joint_state_publisher_gui` | 调节关节并发布状态 |
| `rosrun robot_state_publisher robot_state_publisher` | 发布模型的 TF |
| `rviz` | 显示模型，添加 RobotModel 和 TF |命令	作用
check_urdf robot.urdf	检查模型结构
rosrun xacro xacro robot.urdf.xacro > robot.urdf	展开 Xacro
rosrun joint_state_publisher_gui joint_state_publisher_gui	调节关节并发布状态
rosrun robot_state_publisher robot_state_publisher	发布模型的 TF
rviz	显示模型，添加 RobotModel 和 TF


launch 中加载 URDF：
```
<param name="robot_description"textfile="$(find my_robot_description)/urdf/robot.urdf"/>
```
## 9. tf2
tf2 管理不同坐标系之间的位置、姿态和时间关系，用于坐标转换。
### 9.1 基本概念
- 变换包含平移和旋转，旋转通常用四元数表示。
- 坐标系组成树，每个子坐标系只有一个父坐标系，不能形成环。
- /tf 发布动态变换，/tf_static 发布静态变换。
- TransformBroadcaster 发布变换；TransformListener 接收变换并写入 Buffer。
- URDF 描述结构，/joint_states 提供关节位置，robot_state_publisher 将两者结合生成 TF。
### 9.2 查询变换
buffer.lookupTransform(target_frame, source_frame,
                       ros::Time(0), ros::Duration(1.0));
- 参数顺序：目标坐标系、源坐标系、查询时间、最长等待时间。
- 返回的变换用于将源坐标系中的数据转换到目标坐标系。
- ros::Time(0) 查询最新可用变换；处理传感器数据时通常使用消息时间戳。
- 查询可能失败，需要捕获 tf2::TransformException。
### 9.3 常用命令
| 命令 | 作用 |
| --- | --- |
| `rosrun tf2_ros static_transform_publisher x y z yaw pitch roll parent child` | 发布静态变换 |
| `rosrun tf tf_echo <目标坐标系> <源坐标系>` | 查看坐标变换 |
| `rosrun tf2_tools view_frames.py` | 生成 TF 树图 |
| `rosrun tf tf_monitor` | 查看频率、延迟等信息 |
| `rostopic echo /tf` | 查看动态变换消息 |
| `rostopic echo /tf_static` | 查看静态变换消息 |


静态发布命令的角度顺序是 yaw pitch roll，URDF 的 rpy 是 roll pitch yaw，均使用弧度。RViz 提示缺少变换时，检查 Fixed Frame、frame_id 和 TF 树是否连通。