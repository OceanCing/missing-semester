# CMake 与 Catkin 学习笔记

## 目录
- 一、cmake所解决的
- 二、源代码到可执行程序
- 三、CMake/Makefile生成器
- 四、Catkin，ROS/CMake 的扩展
- 五、常见指令表

---

## 一、cmake所解决的


```text
   CMakeLists.txt
        │  cmake（生成器）
        ▼
   Makefile / VS 工程 / Xcode 工程
        │  make（执行器）
        ▼
   g++ 预处理→编译→汇编→链接
        │
        ▼
     可执行文件
```

>CMake,不是编译器，也不是构建工具，是构建系统的生成器

---

## 二、源代码到可执行程序

翻译过程拆成四步：

| 阶段 | 谁在干活 | 做什么 | 产物 | 单独执行的命令 |
| :--- | :--- | :--- | :--- | :--- |
| **预处理** | 预处理器 `cpp` | 展开 `#include`、替换 `#define` 宏、处理 `#ifdef` 条件编译、删注释、加行号 | `hello.i`（还是文本） | `g++ -E hello.cpp -o hello.i` |
| **编译** | 编译器 `cc1plus` | 词法→语法→语义分析，优化，翻译成汇编 | `hello.s`（汇编，还是文本） | `g++ -S hello.i -o hello.s` |
| **汇编** | 汇编器 `as` | 把汇编指令逐条翻译成机器指令 | `hello.o`（**可重定位目标文件**，二进制） | `g++ -c hello.s -o hello.o` |
| **链接** | 链接器 `ld` | 把多个 `.o` 和库拼在一起，做符号解析 + 重定位 | `hello`（可执行文件） | `g++ hello.o -o hello` |


**报错发生所处阶段**

- 预处理阶段出错 →  `#include` （`fatal error: xxx.h: No such file or directory`）
- 编译阶段出错 → 语法错、类型不匹配（`error: expected ';'`）
- 链接阶段出错 → `undefined reference to 'xxx'`。代码语法正确，只是找不到函数实现：忘了把某个 `.cpp` 加进去，或者忘了 `-l` 某个库

---

## 三、CMake/Makefile生成器

### 3.1 最小可用 CMakeLists.txt

```cmake
# 1. 最低 CMake 版本，放前面
cmake_minimum_required(VERSION 3.10)

# 2. 项目名（会自动设置 PROJECT_NAME、PROJECT_SOURCE_DIR 等变量）
project(hello CXX)

# 3. 指定 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 生成可执行文件
add_executable(hello main.cpp utils.cpp)
```

### 3.2 编译流程：源外构建

```bash
mkdir build && cd build     # ← 关键：不要在源码目录里直接 cmake
cmake ..                    # 读 ../CMakeLists.txt，生成本地 Makefile
make                        # 真正调用 g++ 编译
./hello
```

> **源外构建（out-of-source build）是 CMake 的强约定**：所有生成物都扔进 `build/`，源码目录保持干净。好处很实在——想彻底清理就 `rm -rf build`，不用去猜哪些文件是生成的、哪些是手写的。这也是 `.gitignore` 里必须写 `build/` 的原因。

`cmake ..` 之后，`build/` 里除了 Makefile 还会有一个 `CMakeCache.txt`，它缓存了上次配置的所有变量，**后面很多"改了不生效"的怪问题都和它有关**。

### 3.3 常用命令

| 命令 | 作用 |
| :--- | :--- |
| `cmake_minimum_required(VERSION 3.10)` | 声明最低版本，必须第一行 |
| `project(名 语言)` | 定义项目名与语言（`CXX` / `C`） |
| `add_executable(名 源文件...)` | 生成可执行文件 |
| `add_library(名 STATIC\|SHARED 源文件...)` | 生成库：`STATIC`→`.a`，`SHARED`→`.so` |
| `target_link_libraries(目标 库...)` | 给某个目标链接库 |
| `target_include_directories(目标 可见性 路径)` | 给某个目标加头文件搜索路径（推荐） |
| `include_directories(路径)` | 全局加头文件路径（旧写法，不推荐） |
| `find_package(包 [版本] REQUIRED)` | 查找外部依赖 |
| `add_subdirectory(目录)` | 递归处理子目录的 `CMakeLists.txt` |
| `aux_source_directory(目录 变量)` | 收集目录下所有源文件到变量 |
| `set(变量 值)` / `${变量}` | 定义 / 引用变量 |
| `install(TARGETS 目标 DESTINATION 路径)` | 定义 `make install` 的安装位置 |
| `message(STATUS "文本")` | 打印信息，调试用 |
| `if() / elseif() / else() / endif()` | 条件判断 |


### 3.4 常见问题

- 变量引用必须加 `${}`：`${PROJECT_NAME}`。写成 `PROJECT_NAME` 会被当普通字符串
- 命令名大小写不敏感（`ADD_EXECUTABLE` 也能跑），但变量名敏感。习惯上命令小写、变量大写
- 头文件不用写进 `add_executable`，但要保证能被 `target_include_directories` 找到
- `aux_source_directory` 只在配置阶段扫一次目录，新加了源文件不会自动发现，得重新跑 `cmake`


---

## 四、Catkin，ROS/CMake 的扩展

### 4.1 定位：为什么要再加一层


Catkin 在 CMake 上加的：

1. 规定**工作空间**的目录结构（`src` / `build` / `devel` / `install`）
2. 用 `package.xml` 声明包的身份和依赖
3. 自动解析包之间的依赖顺序，生成环境变量脚本
4. 一条命令编译整个工作空间

Catkin 底层仍是 CMake + Makefile，只是外面套了一层。每个包的 `CMakeLists.txt` 里 `find_package(catkin ...)` 就是在调用 CMake 的机制。

### 4.2 工作空间结构

```text
catkin_ws/                  # 工作空间根目录
├── src/                    # 源码空间：所有功能包放这里（唯一需要 git 管理的）
│   └── CMakeLists.txt      # catkin 提供的顶层 CMake 文件（符号链接）
├── build/                  # 构建空间：CMake 缓存、中间文件（基本不用看）
├── devel/                  # 开发空间：编译产物 + setup.bash
│   └── setup.bash          # ← 必须 source，否则找不到你编的程序
└── install/                # 安装空间：只有 catkin_make install 才有
```


### 4.3 标准流程

```bash
# 0. 检查 catkin 是否安装
catkin --help

# 1. 创建工作空间
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace        # 在 src/ 里生成顶层 CMakeLists.txt

# 2. 回到工作空间根目录编译
cd ~/catkin_ws
catkin_make                  # 或 catkin build（需要先装 catkin_tools）

# 3. 让当前终端认识这个工作空间
source devel/setup.bash
```

`source` 只对当前终端有效，新开一个终端就得重新 source

验证是否生效：

```bash
echo $ROS_PACKAGE_PATH
```

---

## 五、常见指令表

### 编译

```bash
g++ -E main.cpp -o main.i     # 预处理
g++ -S main.i  -o main.s      # 编译
g++ -c main.s  -o main.o      # 汇编
g++ main.o -o main            # 链接
g++ main.cpp -o main          # 一步到位
```

### CMake

```bash
# 传统写法
mkdir build && cd build
cmake ..
make
./hello

# 现代写法（推荐）
cmake -S . -B build
cmake --build build
```

### Catkin

```bash
# 建工作空间
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws && catkin_make
source devel/setup.bash

# 建功能包
cd ~/catkin_ws/src
catkin_create_pkg my_pkg std_msgs rospy roscpp

# 编译
cd ~/catkin_ws && catkin_make
source devel/setup.bash

# 查看依赖
rospack depends1 my_pkg
rospack depends  my_pkg
```

### 排查用的命令

```bash
echo $ROS_PACKAGE_PATH        # 工作空间有没有叠上去
readelf -S main.o             # 看 ELF 段
nm main.o                     # 看符号表
catkin --help                 # catkin 装没装
make -n                       # 只打印要执行的命令，不真跑（调试 Makefile 神器）
```

---