# CMakeLists.txt 使用笔记

`CMakeLists.txt` 是 CMake 项目的核心配置文件。CMake 通过其中的指令了解项目结构、源文件列表和编译要求。

## 常用指令

### 基础配置

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0.0 LANGUAGES CXX)
```

- `cmake_minimum_required()`：指定最低 CMake 版本，建议放在文件最前面。
- `project()`：定义项目名称、版本和使用的语言。

### 创建可执行文件和库

```cmake
add_executable(MyApp
    src/main.cpp
    src/utils.cpp
)

add_library(MyLib STATIC
    src/lib/core.cpp
    src/lib/helper.cpp
)
```

`add_library()` 可以指定 `STATIC`、`SHARED` 或 `MODULE`。不指定类型时，结果由创建目标之前设置的 `BUILD_SHARED_LIBS` 决定。

### 链接库和头文件路径

```cmake
target_link_libraries(MyApp PRIVATE MyLib)

target_include_directories(MyLib PUBLIC include)
target_include_directories(MyApp PRIVATE src)
```

作用域的含义：

| 作用域 | 当前目标可用 | 依赖当前目标的其他目标可用 |
| --- | :---: | :---: |
| `PRIVATE` | 是 | 否 |
| `PUBLIC` | 是 | 是 |
| `INTERFACE` | 否 | 是 |

### 普通变量

```cmake
set(MY_VAR value)
set(MY_LIST a b c)
```

变量通过 `${变量名}` 引用。后面给出多个值时，它们会组成列表。

### 缓存变量

```cmake
set(MY_OPTION "default" CACHE STRING "选项说明")
```

常用类型包括 `STRING`、`BOOL`、`PATH`、`FILEPATH` 和 `INTERNAL`。需要强制覆盖已有缓存值时，可以在末尾添加 `FORCE`。

### 条件语句

```cmake
if(<条件>)
    # ...
elseif(<条件>)
    # ...
else()
    # ...
endif()
```

### 自定义构建命令

```cmake
add_custom_command(
    TARGET MyApp
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Build finished"
    COMMENT "构建完成"
    VERBATIM
)
```

- `PRE_BUILD`：在目标构建前执行；不同生成器下的行为可能不同。
- `PRE_LINK`：在链接前执行。
- `POST_BUILD`：在目标构建完成后执行。

### 安装规则

```cmake
install(TARGETS MyApp
    RUNTIME DESTINATION bin
)

install(TARGETS MyLib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
)
```

- `RUNTIME`：可执行文件，以及部分平台上的动态库。
- `LIBRARY`：动态库。
- `ARCHIVE`：静态库。
- `INCLUDES DESTINATION`：设置导出目标的头文件接口目录，但不会复制头文件。

## 完整示例

```cmake
# 文件路径：项目根目录/CMakeLists.txt

# 1. 指定 CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 2. 定义项目名称、版本和语言
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

# 3. 使用 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. 添加静态库
add_library(MyLib STATIC
    src/lib/core.cpp
    src/lib/helper.cpp
)

# 5. 添加可执行文件
add_executable(MyApp
    src/main.cpp
    src/utils.cpp
)

# 6. 设置头文件路径
target_include_directories(MyLib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(MyApp PRIVATE ${PROJECT_SOURCE_DIR}/src)

# 7. 将 MyLib 链接到 MyApp
target_link_libraries(MyApp PRIVATE MyLib)

# 8. 设置安装规则
install(TARGETS MyApp
    RUNTIME DESTINATION bin
)

install(TARGETS MyLib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
)
```

## 项目结构

```text
my_project/                  <-- ${PROJECT_SOURCE_DIR}，项目根目录
├── CMakeLists.txt           <-- CMake 配置入口
├── build/                   <-- 构建目录，通常不提交到代码仓库
├── include/                 <-- 公开头文件
│   └── my_header.h
├── src/                     <-- 源文件和私有头文件
│   ├── main.cpp
│   ├── utils.cpp
│   └── lib/
│       ├── core.cpp
│       └── helper.cpp
└── README.md
```



# 我的 Catkin 学习笔记

## catkin 是基于 cmake 的，专门为ros设计的.

```text
catkin_make → cmake → make → g++
```

## 工作空间

```text
/root/catkin_ws/
├── src/
├── build/
└── devel/
```

- `src`：放自己的 ROS 包。
- `build`：编译中间文件，不要乱改。
- `devel`：编译结果，`setup.bash` 也在这里。

## 一个包最少有什么

```text
my_package/
├── package.xml
└── CMakeLists.txt
```

- `package.xml`：包名+依赖什么。
- `CMakeLists.txt`：源码怎么编译。

## 编译运行

```bash
ros1
cd /root/catkin_ws
catkin_make
source devel/setup.bash
rosrun catkin_learning hello_node
```

`rosrun` 后面先写包名，再写程序名。

## 易混的地方

- `catkin_make` 要在 `/root/catkin_ws` 里运行。
- 编译完找不到包，重新 `source devel/setup.bash`。
- 改了 C++ 源码，重新 `catkin_make`。
- 包名看 `package.xml`，程序名看 `add_executable`。
- `build/` 和 `devel/` 不用自己写。

## 总结

```text
package.xml      说需要什么
CMakeLists.txt   说怎么编译
catkin_make      编译
rosrun           运行
```


# AI使用心得

## AI在某些方面很聪明

- 可以帮助配置环境(直接询问AI自己是否配置好了环境)
- 可以根据日志找到报错原因,帮忙分析如何应对
- 可以很方便的将纸质笔记转md,甚至于自动帮你`改进流程图`[点击看例子](https://github.com/user-attachments/assets/aceb3eb7-ad93-4c49-acb2-9cc11569b83f)
- 可以帮你句句分析看不懂的文章,还可以不停追问直至理解(`侧边栏临时对话`很方便,不影响上下文,又不用在新对话框手动输入前后文)



## AI在某些方面会犯蠢

- 比如说我的ubuntu蓝牙连不上,电脑建议禁用window的内置蓝牙并买个蓝牙接收器代替。AI明明知道可以手动删掉设备再重添加就可以连上,但它却不觉得这是最好的方法(明明省钱也不麻烦)
  
  
<br><br><br><br><br><br>

---

### <mark>例子</mark>
由 `一条单向流程图` 变成 `清晰的循环流程图`

原来:

<img width="600" alt="工作全流程图" src="https://github.com/user-attachments/assets/aceb3eb7-ad93-4c49-acb2-9cc11569b83f" />

后来:

```mermaid
graph LR

A[团队仓库] --->|fork| B[个人远程仓库]
B[个人远程仓库] --->|clone| C[本地仓库]
C[本地仓库] --->|branch| D[编辑代码]
D[编辑代码] --->|add| E[暂存区]
E[暂存区] --->|commit| C[本地仓库]
C[本地仓库] --->|push| B[个人远程仓库]
B[个人远程仓库] --->|PR| A[团队仓库]
```

# 皇帝没穿衣服
- ai的实现原理是`大语言模型分析`->`agent干活`->`消息返回到大语言模型`->....(循环直到完成任务)
