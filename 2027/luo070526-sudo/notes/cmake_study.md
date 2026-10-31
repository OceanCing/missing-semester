---
tags:
  - type/lesson
  - topic/cmake
  - topic/catkin
  - topic/robomaster
  - topic/testing
  - status/done
date: 2026-10-29
---

# 第二课：CMake、Catkin 与构建验证

本文的库、可执行程序和 ROS 节点统一使用 C++17；CMake、Catkin 与 Linux 命令只负责构建和运行。

本课学习的是“源码怎样可靠地变成可执行程序或 ROS 节点”。

```text
C++ 源码
  → g++ 编译与链接
  → Makefile 管理依赖
  → CMake 生成构建系统
  → Catkin 组织 ROS 功能包
```

## 1. g++、Make、CMake 与 Catkin

| 工具 | 作用 | 不是 |
| --- | --- | --- |
| `g++` | 编译、汇编并链接 C++ | 项目管理工具 |
| `make` | 按 Makefile 的规则构建目标 | C++ 编译器 |
| CMake | 从 `CMakeLists.txt` 生成构建系统 | 编译器 |
| Catkin | 在 ROS1 中组织基于 CMake 的功能包 | ROS 节点本身 |

一个程序大致经历：

1. 预处理：展开头文件和宏；
2. 编译：检查语法并生成汇编；
3. 汇编：生成目标文件 `.o`；
4. 链接：把目标文件与库组合成可执行文件。

手动编译示例：

```bash
g++ -Iinclude -c src/main.cpp -o main.o
g++ -Iinclude -c src/diff_drive.cpp -o diff_drive.o
g++ main.o diff_drive.o -o dx_demo
```

`-Iinclude` 添加头文件搜索路径，`-c` 表示只编译、不执行链接。

## 2. Makefile

Makefile 把重复的编译命令写成“目标—依赖—生成方法”：

```makefile
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude

dx_demo: main.o diff_drive.o
	$(CXX) main.o diff_drive.o -o dx_demo

main.o: src/main.cpp include/diff_drive.h
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o main.o

diff_drive.o: src/diff_drive.cpp include/diff_drive.h
	$(CXX) $(CXXFLAGS) -c src/diff_drive.cpp -o diff_drive.o

.PHONY: clean
clean:
	rm -f dx_demo main.o diff_drive.o
```

`make` 会根据文件修改时间只重建受影响的目标。规则中的命令必须用 Tab 缩进。

## 3. CMake

CMake 用更高层的方式描述目标、源码、头文件和库，再为当前平台生成 Makefile 等构建文件。

### 3.1 差速底盘工程

本次工程位于：

```text
~/下载/DynamicX_第二课_差速底盘实战/native/
├── CMakeLists.txt
├── include/
├── src/
│   ├── main.cpp
│   └── diff_drive.cpp
└── tests/
    └── test.cpp
```

差速底盘运动学：

```text
linear  = radius * (leftWheel + rightWheel) / 2
angular = radius * (rightWheel - leftWheel) / wheelBase
```

当左右轮角速度分别为 `8 rad/s`、`12 rad/s`，轮半径为 `0.05 m`，轮距为 `0.30 m` 时：

```text
linear  = 0.500000 m/s
angular = 0.666667 rad/s
```

### 3.2 典型 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)
project(dx_diff_drive LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(dx_kinematics src/diff_drive.cpp)
target_include_directories(
  dx_kinematics
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(dx_demo src/main.cpp)
target_link_libraries(dx_demo PRIVATE dx_kinematics)

enable_testing()
add_executable(dx_test tests/test.cpp)
target_link_libraries(dx_test PRIVATE dx_kinematics)
add_test(NAME diff_drive_math COMMAND dx_test)
```

关键关系：

- `add_library`：把运动学函数封装成可复用的库；
- `target_include_directories`：为目标添加头文件目录；
- `add_executable`：生成演示程序和测试程序；
- `target_link_libraries`：把运动学库链接到两个程序；
- `add_test`：把 `dx_test` 注册给 CTest。

这比把所有 `.cpp` 堆进一个可执行程序更容易复用和测试。

### 3.3 配置、构建、运行与测试

```bash
cd ~/下载/DynamicX_第二课_差速底盘实战/native
cmake -S . -B cmake_build
cmake --build cmake_build
./cmake_build/dx_demo
ctest --test-dir cmake_build --output-on-failure
```

- `-S .`：源码目录是当前目录；
- `-B cmake_build`：生成文件放入单独的构建目录；
- `cmake --build`：调用已经生成的构建系统；
- `ctest`：运行 CMake 注册的测试。

本机记录的结果是演示程序得到 `0.500000 m/s` 和 `0.666667 rad/s`，CTest 为 `1/1` 通过。

### 3.4 我遇到的路径错误

我曾在 `~/下载/build` 中执行：

```bash
cmake ..
make
```

报错：

```text
The source directory "/home/luoman/下载" does not appear to contain CMakeLists.txt.
No targets specified and no makefile found.
```

原因链：

```text
build 建错位置
  → cmake .. 指向 ~/下载
  → 该目录没有 CMakeLists.txt
  → CMake 没生成 Makefile
  → make 无目标可构建
```

因此，构建前先检查：

```bash
pwd
ls
find .. -maxdepth 2 -name CMakeLists.txt
```

## 4. Catkin

Catkin 建立在 CMake 之上，负责组织 ROS1 工作空间和功能包。

```text
catkin_ws/
├── src/       # 功能包源码
├── build/     # 构建中间文件
├── devel/     # 开发环境和可执行结果
└── logs/      # catkin_tools 日志
```

功能包中的两个核心文件：

| 文件 | 作用 |
| --- | --- |
| `package.xml` | 包信息、许可证与依赖 |
| `CMakeLists.txt` | 查找依赖、编译目标和链接库 |

依赖应同时在两个文件中正确声明。只改其中一个，可能导致构建或依赖解析异常。

### 4.1 典型 ROS 包 CMakeLists.txt

```cmake
find_package(catkin REQUIRED COMPONENTS
  roscpp
  geometry_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES dx_kinematics
  CATKIN_DEPENDS roscpp geometry_msgs
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(dx_velocity_node src/velocity_node.cpp)
target_link_libraries(
  dx_velocity_node
  dx_kinematics
  ${catkin_LIBRARIES}
)
```

### 4.2 工作空间构建

```bash
source /opt/ros/noetic/setup.bash
mkdir -p ~/dx_lesson2_ws/src
cd ~/dx_lesson2_ws
catkin_make
source devel/setup.bash
```

`source` 不是安装 ROS，而是把环境变量加载到当前 Shell。新开终端后需要重新加载。

`catkin_make` 和 `catkin build` 是两种构建方式，同一工作空间内最好固定使用一种。

### 4.3 本次 Catkin 实操

工作空间包含：

- `dx_kinematics`：运动学计算库；
- `dx_velocity_node`：计算并发布速度消息的节点。

第一次构建报错：

```text
fatal error: diff_drive.h: No such file or directory
```

真实头文件位于 `include/dx_kinematics/diff_drive.h`，所以引用应写为：

```cpp
#include "dx_kinematics/diff_drive.h"
```

重新 `catkin_make` 并加载环境后：

```bash
roslaunch dx_velocity_node demo.launch
```

终端持续显示：

```text
v=0.500 m/s, w=0.667 rad/s
```

另一终端验证通信：

```bash
source /opt/ros/noetic/setup.bash
source ~/dx_lesson2_ws/devel/setup.bash
rosnode list
rostopic type /dx_demo/cmd_vel
rostopic echo -n 1 /dx_demo/cmd_vel
```

实际消息包含：

```text
linear.x  = 0.5
angular.z = 0.6666666666666667
```

这证明节点不仅打印了计算结果，还真正发布了 `geometry_msgs/Twist`。

## 5. 测试与参数边界

“正常数据算对”不等于程序可靠。运动学函数还应处理：

- `radius <= 0`；
- `wheelBase <= 0`；
- 输入出现 `NaN` 或 `Inf`；
- 有限输入导致结果溢出；
- 负轮速表示倒车，应当允许。

典型检查：

```cpp
if (!std::isfinite(leftWheel) || !std::isfinite(rightWheel) ||
    !std::isfinite(radius) || !std::isfinite(wheelBase) ||
    radius <= 0.0 || wheelBase <= 0.0) {
  throw std::invalid_argument("Invalid wheel parameters");
}
```

改进测试覆盖直行、转弯、原地旋转、静止、倒车以及非法参数，共 14 个场景。

证据必须区分：

| 内容 | 验证状态 |
| --- | --- |
| 本机 CMake 构建、运行、原始 CTest | 已完成 |
| 本机 Catkin 双包构建与 Topic 验证 | 已完成 |
| 加强版 14 项测试 | AI 独立 Linux 环境通过 |
| 本机执行加强版修复脚本 | 待复现 |

## 6. 使用 AI 辅助排错

有效流程是：

```text
提供真实路径和第一条错误
  → 判断错误发生在哪一层
  → 进行最小修改
  → 重新构建
  → 运行程序
  → 用测试和 ROS 工具验证
```

AI 适合解释报错和补充边界情况，但最终应以 `pwd`、`ls`、编译输出、CTest 结果和 ROS 消息为准。不能把 AI 环境通过写成本机已经完成。

## 7. 基础排错顺序

1. 用 `pwd` 确认所在目录；
2. 用 `ls`、`find` 确认文件存在；
3. 确认源码目录中有 `CMakeLists.txt`；
4. 先让 CMake 配置成功，再执行构建；
5. 头文件错误检查 include 路径；
6. `undefined reference` 检查目标与链接库；
7. ROS 包找不到时检查依赖和 `source`；
8. 构建成功后仍要运行目标并检查输出。

## 8. 从练习工程到战队工程

单个差速底盘程序只需要一个库和一个可执行文件，但整车工程会同时包含消息接口、视觉、控制、硬件通信和启动配置。拆包的依据应是职责与依赖，而不是文件数量。

一个可能的结构是：

| 功能包 | 职责 | 不应包含 |
| --- | --- | --- |
| `dx_msgs` | 自定义消息和服务 | 控制算法与硬件代码 |
| `dx_vision` | 图像输入、识别与目标解算 | 电机收发 |
| `dx_control` | 底盘、云台控制器与安全限制 | 相机驱动 |
| `dx_hardware` | EtherCAT、CAN、串口和设备状态 | 战术决策 |
| `dx_bringup` | launch、YAML 和整车启动入口 | 大量算法实现 |

这样做的意义是：视觉算法变化时不必重新理解 CAN 代码，更换硬件时也不必重写控制器。包之间通过明确的消息和库接口协作。

### 8.1 以目标为中心写 CMake

```cmake
add_library(dx_kinematics src/diff_drive.cpp)
target_compile_features(dx_kinematics PUBLIC cxx_std_17)
target_compile_options(dx_kinematics PRIVATE
  -Wall
  -Wextra
  -Wpedantic
)
target_include_directories(dx_kinematics
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(chassis_node src/chassis_node.cpp)
target_link_libraries(chassis_node
  PRIVATE
    dx_kinematics
    ${catkin_LIBRARIES}
)
```

`PUBLIC` 表示依赖既用于当前目标，也需要传递给使用该库的目标；`PRIVATE` 只影响当前目标。按目标设置头文件、编译选项和链接库，比全局堆在 `include_directories` 或 `CMAKE_CXX_FLAGS` 中更容易定位依赖。

警告选项先用于自己维护的目标。不要轻易把整个工作空间都设成 `-Werror`，第三方包的一个警告可能导致全部构建失败。

### 8.2 自定义消息的构建顺序

如果节点使用本包生成的消息，需要让可执行目标等待消息头文件生成：

```cmake
add_executable(target_solver src/target_solver.cpp)
add_dependencies(target_solver
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)
target_link_libraries(target_solver ${catkin_LIBRARIES})
```

出现“偶尔第一次编译找不到消息头、再编一次又成功”时，应先检查依赖顺序，而不是把重复编译当作正常流程。

## 9. Debug、Release 与可诊断性

### 9.1 构建类型

```bash
cd ~/catkin_ws
catkin_make -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

`Debug` 便于单步调试，但速度可能明显低于比赛构建；`Release` 优化较强，但现场崩溃时缺少调试信息；`RelWithDebInfo` 同时保留优化和符号，适合作为整车测试阶段的选择。

优化后才出现的错误通常暴露了未初始化变量、越界、生命周期或数据竞争问题，不能简单归因于“编译器优化有 bug”。

### 9.2 用 Sanitizer 找内存与未定义行为

在练习工程或离线测试环境中可以单独建立检查构建：

```bash
cmake -S . -B build-sanitize \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer"
cmake --build build-sanitize
ctest --test-dir build-sanitize --output-on-failure
```

Sanitizer 会降低性能，不应不经测试直接用于比赛运行版本。它的作用是在上车前暴露越界、释放后使用和未定义行为。

## 10. 测试不只检查一个公式

差速运动学的单元测试验证数学函数；整车还需要逐层验证：

| 层级 | 示例 | 证明什么 |
| --- | --- | --- |
| 单元测试 | 左右轮速度换算 | 算法边界和符号正确 |
| 节点测试 | 发布一条速度消息 | 节点能接收、计算和发布 |
| 接口测试 | 检查 Topic 类型和频率 | 上下游契约一致 |
| 集成测试 | 启动控制器和仿真底盘 | launch、参数和依赖能协同 |
| 故障测试 | 停止上游或断开设备 | 超时与安全状态生效 |

CTest 与 Catkin 测试：

```bash
ctest --test-dir cmake_build --output-on-failure
cd ~/catkin_ws
catkin_make run_tests
catkin_test_results
```

只写测试文件但不执行、只看到编译成功但不检查返回值，都不能算验证完成。测试失败时先保存第一个失败用例和输入数据。

## 11. 安装与上车部署

开发空间中的 `devel/` 适合本机迭代，整车部署更需要明确哪些程序和配置必须随包安装：

```cmake
install(TARGETS chassis_node dx_kinematics
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch config
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
```

安装验证：

```bash
cd ~/catkin_ws
catkin_make install
source install/setup.bash
rospack find dx_bringup
roslaunch dx_bringup robot.launch
```

如果只在 `devel/` 环境能运行，换电脑或清理工作空间后失败，通常说明安装规则、运行依赖或资源路径不完整。

部署时还要记录：

- Git Commit 或稳定标签；
- Ubuntu、ROS 和编译器版本；
- 第三方库版本；
- 实际使用的 YAML 与 launch；
- 下位机固件和通信协议版本。

代码一致而参数或协议不一致，机器人表现仍可能完全不同。

## 12. RoboMaster 工程中的常见构建故障

| 现象 | 可能原因 | 检查方法 |
| --- | --- | --- |
| 找不到包 | 工作空间未编译或 `source` 顺序错误 | `echo $CMAKE_PREFIX_PATH`、`rospack find` |
| 找不到头文件 | include 路径、依赖声明或生成顺序错误 | 查看完整编译命令和目标依赖 |
| `undefined reference` | 只有声明没有实现，或忘记链接库 | `target_link_libraries`、`nm` |
| 运行时找不到 `.so` | 库未安装或环境未加载 | `ldd`、`LD_LIBRARY_PATH` |
| 修改代码但行为没变 | 运行了旧工作空间中的同名节点 | `which`、`rospack find`、`rosnode info` |
| Debug 正常、Release 异常 | 未初始化、越界或数据竞争 | 警告、Sanitizer、最小复现 |
| 清空 build 后失败 | 之前依赖了缓存或未声明的系统环境 | 从干净工作空间重建 |
| 一台车能编译，另一台不行 | 依赖和系统版本漂移 | 比较版本与安装清单 |

不要同时对同一工作空间混用 `catkin_make` 和 `catkin build`。两者的目录布局和缓存不同，混用会让错误来源变得不清楚。

## 13. 上车前的构建验收

1. 从干净目录完成一次完整构建；
2. 编译警告已经阅读，不能只把输出滚过去；
3. 单元测试、节点测试与故障测试通过；
4. `source` 新环境后仍能 `rospack find` 和 `roslaunch`；
5. 运行中的节点来自预期工作空间；
6. 配置、模型和 launch 已纳入版本管理；
7. 稳定 Commit、依赖、固件与协议版本有记录；
8. 新版本异常时有明确回退方案。

CMake 和 Catkin 的价值不是替人少敲几条编译命令，而是把依赖、生成顺序、测试和部署过程写成可重复执行的工程规则。
