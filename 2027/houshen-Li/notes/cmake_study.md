# catkin&cmake笔记
## cmake

### 1. 编译的四个阶段
| 阶段 | 主要作用 |
| --- | --- |
| 预处理 | 处理头文件包含、宏展开和条件编译 |
| 编译 | 将预处理后的代码转换为汇编代码 |
| 汇编 | 将汇编代码转换为机器码，生成目标文件 |
| 链接 | 合并目标文件和所需库，解析符号引用，生成可执行文件 |

### 2. cmake常用指令
| 指令用法 | 作用与参数 |
| --- | --- |
| `cmake_minimum_required(VERSION 3.16)` | 要求 CMake 版本至少为 3.16 |
| `project(my_project LANGUAGES CXX)` | 设置项目名；`CXX` 表示 C++ |
| `set(CMAKE_CXX_STANDARD 17)` | 为随后创建的目标设置 C++17 标准 |
| `set(CMAKE_CXX_STANDARD_REQUIRED ON)` | 要求支持指定标准，不能退回较低标准 |
| `set(SOURCES main.cpp utils.cpp)` | 定义源文件列表，通过 `${SOURCES}` 引用 |
| `add_executable(app ${SOURCES})` | 使用列表中的源文件定义可执行目标 |
| `add_library(mylib STATIC lib.cpp)` | 定义静态库目标 |
| `add_library(mylib SHARED lib.cpp)` | 定义动态库目标；与上一行是两种选择 |
| `target_include_directories(app PRIVATE include)` | 为目标添加头文件搜索目录 |
| `target_link_libraries(app PRIVATE mylib)` | 为目标设置链接依赖；目标需先定义 |
| `find_package(catkin REQUIRED COMPONENTS roscpp)` | 查找 catkin 及 roscpp；找不到必需依赖时报错 |
| `add_subdirectory(src)` | 处理子目录中的 CMakeLists.txt |
| `message(STATUS "${PROJECT_NAME}")` | 在配置时输出项目名称 |
| `install(TARGETS app RUNTIME DESTINATION bin)` | 声明可执行目标的安装位置，相对于安装前缀 |

### 3. 构建项目终端常用命令
```
# 在项目根目录下创建 build 目录（推荐外部构建）
$ mkdir build && cd build

# 生成构建文件
$ cmake ..

# 编译项目
$ cmake --build .

# 安装到系统目录（可选）
$ cmake --install .
```
### 4. 静态库和动态库的区别
| 对比项 | 静态库 | 动态库（共享库） |
| --- | --- | --- |
| Linux 常见后缀 | `.a` | `.so` |
| 使用方式 | 链接时将所需目标代码并入最终程序 | 程序保留依赖信息，运行时由加载器加载库 |
| 可执行文件大小 | 通常较大 | 通常较小，库文件另行提供 |
| 运行依赖 | 不需要原来的该静态库文件 | 需要可找到且兼容的动态库 |
| 更新库 | 通常需要重新链接程序 | 保持二进制接口兼容时，通常可以替换库 |

- 静态库的代码是在链接阶段并入程序，并非一定把整个库原样复制进去。 使用一个静态库也不代表程序的所有依赖都静态链接。
- 动态库通常在程序启动时加载，也可以由程序运行中主动加载；不是每次调用函数都重新寻找库文件。

### 5. 使用第三方库示例
```
# 文件路径：项目根目录/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(MyBoostProject CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 查找 Boost 库，如果找不到则报错退出
find_package(Boost REQUIRED)
### 指定需要库的哪个部分
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs)
# 指定库的路径
find_package(OpenCV REQUIRED PATHS /path/to/opencv)

# 添加可执行文件
add_executable(MyApp main.cpp)

# 使用导入目标链接 Boost
# Boost::Boost 会自动设置头文件路径和库文件路径
target_link_libraries(MyApp PRIVATE Boost::Boost)
```

## catkin

catkin 是 ROS 1 基于 CMake 的构建系统。一个工作空间可以包含多个功能包。

### 1. 工作空间

| 目录 | 作用 |
| --- | --- |
| `src` | 功能包源码 |
| `build` | 构建缓存和中间文件 |
| `devel` | 开发时使用的程序、库和环境脚本 |
| `install` | 按安装规则生成的文件 |

```bash
source /opt/ros/noetic/setup.bash  # 加载 ROS，版本名按实际修改
mkdir -p ~/catkin_ws/src          # 创建工作空间
cd ~/catkin_ws
catkin_make                      # 初始化并构建
source devel/setup.bash           # 加载工作空间环境
```

### 2. 创建功能包

| 文件 | 作用 |
| --- | --- |
| `package.xml` | 描述包名、版本、维护者、许可证及依赖 |
| `CMakeLists.txt` | 描述构建目标、头文件目录、链接关系和安装规则 |

```bash
cd ~/catkin_ws/src
catkin_create_pkg test_pkg std_msgs rospy roscpp
```

- `test_pkg`：包名；后面是依赖包。
- `std_msgs`：标准消息；`rospy`、`roscpp`：Python、C++ 客户端库。
- `package.xml`：包信息和依赖；`CMakeLists.txt`：构建规则。
- 创建包不会自动完成节点代码或安装所有依赖。


### 3. 常用命令

构建和安装命令在工作空间根目录执行。

| 命令 | 作用 |
| --- | --- |
| `catkin_make` | 构建工作空间 |
| `catkin_make install` | 构建并按规则安装 |
| `source devel/setup.bash` | 加载开发空间环境 |
| `source install/setup.bash` | 加载已生成的安装空间环境 |
| `rospack find test_pkg` | 查找包路径 |
| `rospack depends1 test_pkg` | 查看直接依赖 |
| `rospack depends test_pkg` | 查看直接和间接依赖 |

**构建生成产物，source 设置当前终端环境。** 新终端需要重新加载环境，除非已配置启动脚本。`catkin_make` 与 `catkin build` 不要在同一构建空间混用。

#### 环境变量

| 变量 | 主要用途 |
| --- | --- |
| `CMAKE_PREFIX_PATH` | 帮助 CMake 查找包，并参与 catkin 工作空间叠加 |
| `ROS_PACKAGE_PATH` | 帮助 ROS 查找功能包 |
| `LD_LIBRARY_PATH` | Linux 运行时动态库搜索路径 |
| `PATH` | 可执行命令搜索路径 |
| `PYTHONPATH` | Python 模块搜索路径 |

可以用 `echo "$CMAKE_PREFIX_PATH"` 或 `echo "$ROS_PACKAGE_PATH"` 检查当前设置。新开的终端需要重新加载环境，除非已配置启动脚本。

### 4. 工作空间叠加

上层空间可以使用底层已构建的包，同名包优先使用上层版本，不会改写底层文件。

```bash
source ~/ws_base/devel/setup.bash  # 底层空间须已构建
cd ~/ws_dev                      # 上层新工作空间须已有 src
catkin_make                      # 基于底层环境构建
source devel/setup.bash           # 加载上层环境
```

顺序：**构建底层 → 加载底层 → 构建上层 → 加载上层**。替换同名包时需检查兼容性；更换已有空间的底层时，应重新配置，避免沿用旧缓存。


