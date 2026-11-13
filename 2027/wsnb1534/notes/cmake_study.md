# CMake 与 catkin 学习笔记
## 1. CMake
用来构建 C/C++ 项目的跨平台自动化构建系统。它不直接编译代码，而是根据 `CMakeLists.txt` 生成对应平台的本地构建文件（如 Linux 上的 Makefile、Windows 上的 Visual Studio 工程），再由底层工具完成实际编译。

### 1.1 核心概念

- **CMakeLists.txt**：CMake 的配置文件，定义项目的构建规则，是 CMake 的唯一输入
- **out-of-source build**：推荐在单独的 `build/` 目录中构建，不污染源码目录
- **target**：构建目标，可以是可执行文件（`add_executable`）或库（`add_library`）

### 1.2 常用指令

| 指令 | 作用 | 示例 |
|------|------|------|
| `cmake_minimum_required` | 声明最低 CMake 版本 | `cmake_minimum_required(VERSION 3.8)` |
| `project` | 定义项目名称，同时创建 `PROJECT_NAME` 变量 | `project(my_app)` |
| `find_package` | 查找并加载外部依赖的配置 | `find_package(ament_cmake REQUIRED)` |
| `add_executable` | 定义可执行文件目标 | `add_executable(my_node src/main.cpp)` |
| `add_library` | 定义库目标 | `add_library(my_lib src/lib.cpp)` |
| `target_link_libraries` | 为目标链接库 | `target_link_libraries(my_node my_lib)` |
| `add_compile_options` | 添加编译选项 | `add_compile_options(-Wall -Wextra)` |
| `install` | 指定安装规则 | `install(TARGETS my_node DESTINATION lib)` |
| `set` | 设置变量 | `set(MY_VAR "hello")` |
| `message` | 输出调试信息 | `message(STATUS "value = ${MY_VAR}")` |

### 1.3 典型 CMakeLists.txt 结构

```cmake
cmake_minimum_required(VERSION 3.8)
project(my_project)

# 查找依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

# 定义可执行文件
add_executable(my_node src/main.cpp)

# 链接依赖
ament_target_dependencies(my_node rclcpp)

# 安装
install(TARGETS my_node
  DESTINATION lib/${PROJECT_NAME}
)

# 打包
ament_package()
```

### 1.4 构建流程

```
CMakeLists.txt  →  cmake ..  →  Makefile / ninja.build  →  make  →  可执行文件/库
     (配置)          (生成)        (本地构建文件)            (编译)      (产物)
```

## 2. catkin

### 2.1 catkin 和 CMake 的关系

- **catkin 是 ROS1 的元构建系统**，它在 CMake 之上封装了一层，增加了 ROS 特有的功能：自动依赖推断、消息/服务生成、跨包 `cfg` 文件处理等
- **ament_cmake 是 ROS2 的对应物**，设计理念相同但做了简化和改进，去掉了 catkin 中一些历史包袱
- 本质上：`catkin/ament_cmake = CMake + ROS 专用宏和函数`
- 用户写的 `CMakeLists.txt` 里既有纯 CMake 指令（`add_executable`、`install`），也有 catkin/ament 专用指令（`find_package(catkin REQUIRED)`、`catkin_package()`、`ament_package()`）
- catkin/ament 的工作方式：先收集所有包的信息，再按依赖拓扑排序，依次对每个包调用 CMake

### 2.2 工作空间结构

- `src/`：存放功能包的源码，每个子目录是一个 ROS 包（含 `package.xml` 和 `CMakeLists.txt`）
- `build/`：CMake 的构建中间产物和生成的 Makefile，由构建系统自动生成，一般不手动修改
- `devel/`（ROS1）/ `install/`（ROS2）：构建产物的安装位置，`devel/` 下的 `setup.bash` 用于将当前工作空间加入 ROS 环境变量
- `log/`（ROS2）：构建日志，记录每个包的构建输出和错误信息


### 2.3 常用命令

- `catkin_create_pkg`：创建新的 ROS1 功能包，自动生成 `package.xml`、`CMakeLists.txt` 等模板文件。ROS2 中对应 `ros2 pkg create`
- `catkin_make`：在 ROS1 工作空间根目录执行构建，相当于对 `src/` 下所有包依次调用 CMake + make。ROS2 中对应 `colcon build`
- `source devel/setup.bash`：将当前工作空间的构建产物加入环境变量（`ROS_PACKAGE_PATH`、`PYTHONPATH` 等），使得 `rosrun`、`roslaunch` 能找到该工作空间的包。ROS2 中对应 `source install/setup.bash`

> **注意**：ROS2 中使用 `colcon` 替代 `catkin_make`，使用 `source install/setup.bash` 替代 `source devel/setup.bash`

### 2.4 package.xml 和 CMakeLists.txt 的分工

| 文件 | 职责 | 典型内容 |
|------|------|----------|
| `package.xml` | **声明式**：描述包的元信息和依赖关系，供构建系统和包管理器读取 | 包名、版本、维护者、许可证、`<depend>`、`<buildtool_depend>`、`<exec_depend>`、`<test_depend>` |
| `CMakeLists.txt` | **命令式**：定义实际的构建规则和流程 | `find_package`、`add_executable`、`target_link_libraries`、`install`、`ament_package()` |

**关键区别**：
- `package.xml` 中的 `<depend>` 声明依赖，是给 **rosdep** 等工具解析用的（安装系统依赖、解决跨包依赖）
- `CMakeLists.txt` 中的 `find_package()` 才是让 **CMake 真正找到** 该依赖的头文件和库
- 两者必须保持一致：`package.xml` 中声明了依赖，`CMakeLists.txt` 中也要 `find_package` 对应的包
