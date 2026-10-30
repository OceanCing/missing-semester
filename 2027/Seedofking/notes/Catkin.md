## Catkin

#### 构建过程
![构建过程](https://bee-reg-ab.imagency.cn/mr/202610/23/6adb11f2f10c7.png)

#### catkin工作空间层级

catkin_ws/
├── .catkin_tools/              # catkin init 生成的隐藏配置目录 (保存构建配置)
│   ├── CATKIN_IGNORE
│   └── profiles/
│       └── default/
│           └── config.yaml
├── **CMakeLists.txt**/ # 顶层CMakeLists.txt负责管理包, 无需自己修改
├── **build**/                      # 编译中间文件 (catkin build 自动生成)
│   └── my_first_pkg/           # 每个包一个独立的编译目录
│       └── ...
├── **devel**/                      # 开发空间 (存放环境变量、可执行文件等)
│   ├── setup.bash              # source 这个文件来配置环境
│   ├── setup.sh
│   ├── env.sh
│   ├── local_setup.bash
│   └── ...
├── **logs**/                       # 构建日志 (包含编译记录)
│   └── my_first_pkg/
│       └── build.catkin-tools.<timestamp>.log
└── **src**/                        # 源码空间 (你主要工作的地方，存放所有功能包)
    └── my_first_pkg/           # 你的具体功能包 (Package)
        ├── CMakeLists.txt      # package的编译规则, 用于管理包内代码编译 (必须)
        ├── package.xml         # package的描述信息 (必须)
        ├── src/                # 源代码文件 (如 .cpp 节点代码)
        ├── include/            # C++ 头文件 (如 .h, .hpp)
        ├── scripts/            # 可执行脚本 (如 .py 节点代码)
        ├── msg/                # 自定义消息 (自定义的 .msg 文件)
        ├── srv/                # 自定义服务 (自定义的 .srv 文件)
        ├── models/             # 3D模型文件 (如 URDF 引用的 .dae, .stl)
        ├── urdf/               # urdf文件 (机器人模型描述)
        └── launch/             # launch文件 (用于一次性启动多个节点)



#### package层级
![package层级](https://bee-reg-ab.imagency.cn/mr/202610/24/6adc8b51a2cc3.png)


### 初始化catkin工作空间:

#### 1.创建工作空间文件夹:
    先创建要作为工作空间的文件夹, 常命名为 工程名_ws, 而该文件夹内的文件夹名须采用默认名如src

#### 2.初始化工作空间:
    catkin_init_workspace

#### 3.在工作空间创建src和功能包:
    cd catkin_ws
    mkdir src
    cd src

    catkin_create_pkg 包名 依赖列表
    catkin_create_pkg test_pkg rospy roscpp std_msgs


#### 4.编译工作空间:
    cd catkin_ws
    catkin build
###### 若编译失败, 显示找不到src, 有可能是catkin_ws以上的目录存在.catkin_tools, 需要删除

###### 至此, catkin_ws下便有了CMakeLists.txt, build, cmake-build-debug, devel, logs, src

#### 5.加载ros系统和工作空间环境变量:
    source /opt/ros/noetic/setup.bash
    source /root/catkin_ws/devel/setup.bash
###### 这两个命令可以放在容器系统里的 .bashrc, 每次启动自动加载

#### (查看是否成功加载:)
    printenv | grep ROS
###### 若看见类似 ROS_DISTRO=noetic, ROS_VERSION=1等变量则成功加载ros系统环境变量
    echo $ROS_PACKAGE_PATH
###### 若输出包含catkin_ws/src路径, 例如/rose_ws/src/rostest_pkg:/opt/ros/noetic/share
###### 则说明成功加载, 若只有/opt/ros/noetic/share则没有成功source /devel/setup.bash


#### 在Clion添加环境变量
在 设置/构建、执行、部署/CMake 右侧选项栏中的 CMake选项 点+号插入宏, 选择项目根路径插入 
![插入宏](https://s41.ax1x.com/2026/10/25/pnD7wkQ.png)



添加ros系统环境变量(加一个空格与前面分隔):
 -DCMAKE_PREFIX_PATH=/opt/ros/noetic


![添加ros变量](https://s41.ax1x.com/2026/10/25/pnD70Yj.png)


### 代码开发:
在catkin_ws/src/test_pkg/src下, 可以开始使用clion开发代码
catkin_ws下的src是用于存放依赖包的, 而包文件夹下的src是用于存放这个包的代码的
写代码时先在包的CMakeLists.txt里面添加.cpp文件, 否则无法识别到外部库

### 移动工程:
###### catkin编译的产物以及ros对包的识别都是使用绝对路径的, 移动之后都会失效
#### 0.停止所有占用进程, 如Clion远程连接, roscore, 节点
#### 1.移动前清空构建产物, 只留源码:
    cd /rose_ws && catkin clean -y
#### 2.移动
    mv 路径 新路径
#### 3.改.bashrc里面的路径
    source /root/rose_ws/devel/setup.bash
#### 4.在新的位置构建
    cd /root/rose_ws && catkin build
#### 5.验证
    source ~/.bashrc
    rospack find rosetest_pkg