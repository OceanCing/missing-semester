# 第二课上课笔记 Cmake与catkin

## Cmake
- 编译的流程
    1. 预处理
    2. 编译
    3. 汇编
    4. 链接
- Cmake是对make工具的生成器
- 编译方式的转变:`gcc/g++` -> `Makefile` -> `CMake` （带来编译效率的提高）

## catkin

1. catkin概念理解
    - catkin是ROS1的编译系统（类似单个源文件的`g++`），负责把许多个“软件包”编译成能运行的程序，并生成一套环境变量
    - catkin本身基于Cmake，所以会调用make工具，是CMake的上位工具

2. 工作空间的结构
    ```text
    ├── CMakeLists.txt        # package 的编译规则（必须）
    ├── package.xml           # package 的描述信息（必须）
    ├── src/                  # 源代码文件
    ├── include/              # C++ 头文件
    ├── scripts/              # 可执行脚本
    ├── msg/                  # 自定义消息
    ├── srv/                  # 自定义服务
    ├── models/               # 3D 模型文件
    ├── urdf/                 # urdf 文件
    └── launch/               # launch 文件
    ```

3. 创建
    - catkin工作空间 :`~/ros_ws/catkin_ws/src`
    - 创建pkg:`catkin_create_pkg 名字 roscpp rospy std_msgs`
    - 创建后包含:`CMakeList.txt`和` package.xml`

