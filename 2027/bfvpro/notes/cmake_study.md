# CMake and Catkin note \(OvO)/
> ROS -Ubuntu26.04


## 1. C++从语言到程序


```text
.cpp文件
  1预处理=展开头文件
  2编译=将‘C++’ 转换为汇编代码
  3汇编=生成目标文件‘’文件.o’‘
  4链接=组合目标文件和库，生成可执行程序
```

使用 `g++` 

### 编译工具】

| 工具 | 主要职责 |
| --- | --- |
| `g++` | 处理 C++ 编译流程，生成目标文件或可执行程序。 |
| `make` | 读取 Makefile，按目标、依赖关系和命令执行构建。 |
| CMake | 读取 `CMakeLists.txt`，配置项目并生成 Makefile、Ninja 文件或 IDE 工程等构建系统。 |
| Catkin | 面向 ROS 1 工作空间组织软件包、依赖和构建过程；软件包的具体构建规则仍大量使用 CMake。 |

可以把一般的 CMake 构建流程记为：

```text
CMakeLists.txt → cmake 配置/生成 → Makefile 或 Ninja 文件 → make/ninja 调用编译器
```

## 2. Makefile

Makefile 是交给 `make` 执行的构建说明。它通常会描述：

- 要生成什么目标，例如可执行文件或库；
- 每个目标依赖哪些源文件或其他目标；
- 生成目标时要运行哪些命令。

当依赖关系描述正确时，`make` 可以根据文件变更决定需要重建哪些目标，减少重复编译。项目变大后，手写和维护大量规则会比较繁琐，因此常用 CMake 来生成底层构建文件。

## 3. CMake 基础

CMake 项目通常用 `CMakeLists.txt` 声明项目结构和构建要求。常见命令包括：

- `cmake_minimum_required(...)`：声明最低 CMake 版本要求；
- `project(...)`：声明项目名称和使用的语言；
- `add_executable(...)`：声明可执行程序目标和源文件；
- `add_library(...)`：声明库目标；
- `target_link_libraries(...)`：声明目标要链接的库；
- `target_include_directories(...)`：为指定目标设置头文件搜索路径。

一个很小的 C++ 项目示例：

```cmake
cmake_minimum_required(VERSION 3.10)
project(hello_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hello src/main.cpp)
target_include_directories(hello PRIVATE include)
```

在项目根目录执行：

```bash
cmake -S . -B build
cmake --build build
```



## 4. Catkin 与 ROS 工作空间



- `package.xml`：描述软件包的名称、版本和依赖等元信息‘’类似于签名‘’
- `CMakeLists.txt`：描述构建规则''类似系统的引导‘’

### 目录‘试试表格，应该不会错吧=）’

| 目录 | 用途 |
| --- | --- |
| `src/` | ROS 源代码 |
| `build/` | 配置和编译中间文件 |
| `devel/` | 生成的程序,库等脚本 |
| `install/` | 安装空间 |


### 创建工作空间

使用 `catkin_tools` 的常见流程如下。
```bash
source /opt/ros/lzx/setup.bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws
catkin init
catkin build
```

构建后根目录下能看到 `build/` 和 `devel/`等文件夹，Windows同理
```bash
source devel/setup.bash
```

### 创建 ROS 软件包

- 在 `src/` 目录中执行：
’‘模板’‘
```bash
cd ~/catkin_ws/src
catkin_create_pkg test_pkg std_msgs rospy roscpp
```
''后面3个东西可以理解为模块’‘

- 根目录构建

```bash
cd ~/catkin_ws
catkin build
```



## 5. LLM && Agent

### Agent 软件

Agent 把llm和工具、执行环境连接起来的软件。

```text
用户提出任务
  → 模型判断需要什么工具，并生成调用请求
  → Agent 程序执行工具（例如读文件、列文件、改文件或运行命令）
  → 结果返回

```




### Skill && `AGENTS.md`

- **Skill**：可重复使用的任务指导和相关资源，可包含流程、脚本、参考文档或模板。
- **`AGENTS.md`**：项目级说明文件，可记录项目背景、目录说明、开发规范和验证方法，帮助 Agent 按项目约定工作。
