# ROS理论

## ROS概述与环境搭建
- ROS简介
  - Robot Operating System
  - 可复用、可多主机工作、入门快（松耦合）、精简、语言弱相关、易测试、免费开源

- ROS简单运行（hello world的实现）
  - 大致流程
    - 先创建一个工作空间
    - 再创建一个功能包
    - 编辑源文件
    - 编辑配置文件
    - 编译并执行

**以下是上述流程的详细介绍**

1. 创建工作空间并初始化
```bash
mkdir -p catkin_ws/src
cd catkin_ws/src
catkin_init_workspace
# 我用的工作空间名称为catkin_ws
```
*该命令会创建工作空间和src子目录，然后进入工作空间调用`catkin_make`命令编译*

2. 进入`src`创建`ros`包并添加依赖
```bash
cd ~/catkin_ws/src
# 语法：catkin_create_pkg 自定义ROS包名 依赖库1 依赖库2 依赖库3
catkin_create_pkg ros1_homework1 roscpp rospy std_msgs
```
*其中roscpp是使用C++实现的库、rospy则是使用python实现的库、std_msgs是标准信息库，我们往后用C++*

3. 进入`ros`包的`src`目录编辑源文件
```bash
cd 自定义的包
```
C++源码实现（文件名自定义）
```cpp
#include "ros/ros.h"

int main(int argc, char *argv[])
{
    //执行 ros 节点初始化
    ros::init(argc,argv,"hello");
    //创建 ros 节点句柄(非必须)
    ros::NodeHandle n;
    //控制台输出 hello world
    ROS_INFO("hello world!");

    return 0;
}
```
> *注意：要记得先在src目录中用vim写下C++源码*：
```bash
cd ros1_homework1/src
vim hello.cpp
```

4. 编辑 ros 包下的 Cmakelist.txt文件
```bash
add_executable(步骤3的源文件名
  src/步骤3的源文件名.cpp
)
target_link_libraries(步骤3的源文件名
  ${catkin_LIBRARIES}
)
```

5. 进入工作空间目录并编译
```bash
cd 自定义空间名称
catkin_make
```

6. 执行
在终端一：
```bash
roscore
```

在终端二：
```bash
cd 工作空间
source ./devel/setup.zsh # .zsh也可以
rosrun 包名 C++节点
```

或者：
```bash
rosrun ros1_homework1 ros1_homework1_node
# 语法：rosrun <包名> <可执行文件名>
```

- 终端（使用Terminator）
  - 常用快捷键：
    - Ctrl+Shift+O/E        水平/垂直切割终端
    - Ctrl+Shift+Right/Left 对垂直分割线向右/左移动
    - Ctrl+Shift+Up/Down    对水平分割线向上/下移动
    - Ctrl+Shift+W          关闭当前终端
    - Ctrl+Shift+F          搜索

- launch文件
  - 减少多开节点的情况

- ROS文件系统
```text
WorkSpace --- 自定义的工作空间

    |--- build:编译空间，用于存放CMake和catkin的缓存信息、配置信息和其他中间文件。

    |--- devel:开发空间，用于存放编译后生成的目标文件，包括头文件、动态&静态链接库、可执行文件等。

    |--- src: 源码

        |-- package：功能包(ROS基本单元)包含多个节点、库与配置文件，包名所有字母小写，只能由字母、数字与下划线组成

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
```

## ROS通信机制
- ROS是进程的分布式框架
- ROS的基本通信机制有三种实现策略
  - 话题通信（发布订阅模式）
  - 服务通信（请求相应模式）
  - 参数服务器（参数共享模式）

> *补充：由其笔者时间紧张，故在这里暂时只做简略的介绍，后期会详细补上*

1. 话题通信（发布，订阅）
    - 角色：ROS Master（管理者）、Talker（发布者）、Listener（订阅者）
    - ROS Master：名字注册管理、名字查询、参数服务器所在地、状态监控
    - Talker：写数据节点
    - Listener：读数据节点
*当然，发布者和订阅者之间的桥梁，只能是消息，即数据结构，由msg定义*
    - 常见命令：
```text
rostopic list //列出所有活跃话题
rostopic echo //把话题上的内容打到终端
rostopic info //谁在发布、谁在订阅、类型是啥
rostopic hz   //测频率
```
    - 两个进程：发布端和订阅端是两个独立进程

2. 服务通信（先定义，再调用）
    - 角色：Server（服务端）、Client（客户端）
    - 服务端：注册一个回调处理请求
    - 客户端：发起调用，等结果
    - srv文件：上下两端用`---`隔开，上请求，下应答
    - 常见命令：
```text
rosservice list //列出当前所有服务
rosservice type //看服务类型
rosservice call //手工该参数（仅一次），用于验证
rossrv show     //看srv字段定义
```

3. 参数服务器（set 和 load）
    - 角色：Talker（参数设置者）、Listener（参数调用者）
    - 发送类型：int、double、bool、string、list、dict
    - 生命周期：一次完整的roscore（即关闭后结束）
    - **set & load（？？？这是啥，后面关注的难点）**

## YAML相关规矩
1. 只用空格缩进，绝不用Tab
> 即语法上严格对齐
2. 冒号后面必须有空格
> 否则一串代码很看成字符串
3. 大小写敏感
4. `#`是注释
> 若在值里用‵#‵，就加引号
5. `---`可以分隔多个文档
> 空文件也合法，等于一个空字典

- 结构：**标量、列表、字典**
  - 标量：
```bash
speed: 1.5       # 数字 (float/double)
name: "sentry"   # 字符串 (string)
debug: true      # 布尔 (boolean)
```
  - 列表：
```bash
# 摄像头配置
camera:
  exposure: 30      # 曝光时间
  ids: [1, 2, 3]    # 行内列表写法 (Flow Style)

# 其他参数
parts: "something"  # 不知道具体是什么，暂且给个字符串
thresholds: [0.3, 0.6, 0.9]  # 修正了拼写和数组格式
port: 8080
device_id: "a081"
```
  - 字典：
```bash
camera:             # 顶层键
  width: 640        # 宽度，数字类型
  height: 480       # 高度，数字类型
  
  # 行内写法 (注意这里必须是半角的 #)
  point: [1, 2, 3]  # 假设你想写个坐标，用行内列表表示
  # 或者用行内字典写法：
  # point: {x: 1, y: 2, z: 3} 
```

> 总结：实在过于简化，后期会出第二版
