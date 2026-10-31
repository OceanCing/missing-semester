# 01：开发环境、工作空间和编译

## 学了什么

这次使用 **Docker 中的 Ubuntu 20.04 + ROS 1 Noetic**。ROS 1 和 ROS 2 的命令、接口有差别，本次记录都按 ROS 1 Noetic 来写。

宿主机是日常使用的电脑系统，容器是里面运行的开发环境。桌面的“ROS1 开发终端”会先启动容器，再进入容器里的 Bash。多个开发终端连接同一个容器，不是每打开一个终端就新建一套环境。

### 路径和文件

| 位置 | 含义 |
| --- | --- |
| 宿主机的 `self-study/ros1/turtle_control/` | Git 仓库内唯一的一份示例源码 |
| 宿主机开发目录的 `catkin_ws/src/turtle_control` | 指向这份源码的兼容链接 |
| 容器的 `/home/rosdev/catkin_ws` | 开发工作空间 |
| 容器的 `/home/rosdev/catkin_ws/src/turtle_control` | 编译和运行时使用的软件包入口 |
| 工作空间的 `build/`、`devel/`、`logs/` | 构建中间文件、开发环境与程序、构建日志 |

这些路径通过挂载和链接连到同一份源码。镜像提供系统与依赖，源码放在宿主机上持久保存。关闭一个空闲开发终端不会删除源码，也不会自动执行 `docker compose down`。

官方的 `turtlesim` 是安装在容器 ROS 环境中的现成软件包，不是我们在 `turtle_control` 中写的程序。可以用 `rospack find turtlesim` 查询它的包目录；安装的二进制包不一定包含完整 C++ 源码。

### 五个概念的分工

| 名称 | 这次练习中的作用 |
| --- | --- |
| 工作空间 workspace | 容纳和构建一批 ROS 软件包，本次叫 `catkin_ws` |
| 软件包 package | 组织功能、源码和依赖，本次叫 `turtle_control` |
| 节点 node | 运行中的 ROS 程序实例，一个软件包可以提供多个节点程序 |
| catkin | ROS 1 的构建系统，基于 CMake；使用的 `catkin build` 命令来自 catkin_tools |
| CMake | 读取 `CMakeLists.txt`、生成构建规则，再由构建工具调用编译器生成程序 |

`package.xml` 描述包名和依赖；`CMakeLists.txt` 决定哪些源码参与构建、生成什么程序、连接哪些库。文件放进 `src/` 并不自动意味着它会被编译成新程序。

### 依赖声明与构建规则

本包的 [package.xml](turtle_control/package.xml) 中有：

```xml
<depend>tf2_ros</depend>
```

这表示软件包依赖 `tf2_ros`。依赖声明供构建、依赖管理等工具读取，写入这行本身不会下载安装依赖。`<exec_depend>rviz</exec_depend>` 则声明运行相关功能时需要 RViz，本包的 C++ 程序不因此自动链接 RViz。

[CMakeLists.txt](turtle_control/CMakeLists.txt) 中先查找 catkin 和本包使用的组件：

```cmake
find_package(catkin REQUIRED COMPONENTS geometry_msgs roscpp turtlesim tf2 tf2_ros tf2_geometry_msgs)
```

其中一个程序的构建配置是：

```cmake
add_executable(turtle_motion src/turtle_motion.cpp)
target_include_directories(turtle_motion PRIVATE ${catkin_INCLUDE_DIRS})
target_link_libraries(turtle_motion ${catkin_LIBRARIES})
```

| 配置 | 作用 |
| --- | --- |
| `find_package` | 查找需要的组件及其构建信息；`REQUIRED` 表示查找失败就报错 |
| `add_executable` | 指定用哪些源文件构建哪个可执行程序 |
| `target_include_directories` | 为这个构建目标提供查找头文件的目录 |
| `target_link_libraries` | 指定这个程序需要链接的库 |

`${catkin_INCLUDE_DIRS}` 和 `${catkin_LIBRARIES}` 是 CMake 变量，分别提供相关头文件目录和库信息。

新增独立 C++ 节点时，通常要完成“写源码 → 配置依赖和构建目标 → 编译 → 运行”。这部分先理解文件和命令的分工；具体配置、编译和报错定位可以由 AI 协助完成，再对照改动与运行结果核对。

## 做过的实验

第一次创建包时用过以下流程。当前包已经存在，复现现有示例时按[包 README](turtle_control/README.md)编译即可，不再重复创建。

```bash
cd ~/catkin_ws/src
catkin_create_pkg turtle_control roscpp geometry_msgs
cd ~/catkin_ws
catkin init
catkin build turtle_control -j2
source devel/setup.bash
rospack find turtle_control
```

当时 `rospack find` 找到了工作空间中的 `turtle_control` 包。后来增加示例时，也相应增加了 turtlesim、tf2 等依赖。

## 观察与解释：哪些步骤要重复

| 命令 / 情况 | 作用和时机 |
| --- | --- |
| `catkin init` | 配置整个工作空间，通常初始化一次；新建文件不需要重复初始化 |
| `catkin build turtle_control -j2` | 编译包，最多使用两个并行任务；修改参与构建的 C++ 源码或头文件后需要重新构建 |
| 新增 `.cpp` | 若要参与程序，还需要在 CMake 中配置，再编译 |
| 修改 `.launch` 或启动命令的参数 | 重新启动相关节点使配置生效，一般不需要重新编译 C++ |
| 修改 Markdown 笔记 | 不需要构建 |
| `source ~/catkin_ws/devel/setup.bash` | 给当前 Shell 加载包和程序的查找路径；它不负责编译程序 |
| `rospack find turtle_control` | 查询包的位置，是检查工具，不是每次启动前必须做的步骤 |

本机的“ROS1 开发终端”已经自动加载 Noetic 和已有工作空间。若终端是在第一次构建之前打开的，构建后可以手动 source 一次。一个终端 source 的环境不会自动写进其他已经打开的终端。

修改 C++ 后，流程是“修改 → 编译 → 重新运行节点”。正在运行的旧进程不会因为源文件保存了，就自动变成新代码。
