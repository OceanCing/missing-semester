# 第二课 Cmake 和 Catkin

## <span style="background:#90EE90">**学习链接**</span>:
* **详细的编译过程教程**：https://zhuanlan.zhihu.com/p/476697014
* **Cmake基础**：https://www.runoob.com/cmake/cmake-basic.html
* **catkin官方教程**：https://wiki.ros.org/catkin/Tutorials

## **一.从源代码到可执行文件(gcc)**

### **1.演化过程**：
```
gcc/g++  →  Makefile  →  CMake  →  catkin
手敲命令    写说明书     跨平台生成器   ROS 定制版
```

***每上一层，功能越齐全***

---
### **2.编译的四个阶段**:
|阶段|对应的任务|产生的效果|
|:---|:---|:---|
|预处理|抄`#include`的头文件，删去注释，展开宏|从源程序`.c`，变成`.i`|
|编译|将`.i`文件翻译成汇编语言|从`.i`变成汇编`.s`|
|汇编|将汇编语言变成机器语言指令（二进制）|从`.s`变成`.o`|
|链接|把多个`.o`库拼在一起，让其可以互相调用|从`.o`变成可执行文件（可执行文件没有后缀）|
---

**Example:**
```bash
gcc -E main.cpp -o main.i
# -E 只做预处理这步；-o 是 output，指定输出文件名
# 跑完你会看到 main.i 巨长,因为 iostream 的内容全被展开了，自己的代码在很后面

g++ -S main.i -o main.s
# 编译：把预处理结果翻译成汇编代码

g++ -c main.s -o main.o
# 汇编：把汇编翻译成二进制目标文件（.o）

g++ main.o -o demo
# 链接：把 .o 和它调用的库（如 printf）拼成一个可执行文件 demo（终端里显示为绿色）
# 然后执行./demo

gcc -o hello hello.c
# 不加特殊参数时，gcc 一口气跑完四个阶段，直接输出可执行文件 hello，然后执行./hello
```
---
### **3.链接**：
#### **功能**：把各种代码和数据片段收集组合成一个单一文件
#### **执行时机**：
- **编译时**：源码翻译成机器代码时（最常见）
- **加载时**：程序被加载器载入内存时
- **运行时**：由应用程序自己执行

从**意义**</span>：链接使<span style="color:yellow">**分离编译**</span>成为可能,可以拆成小模块独立改、独立编译；改一个模块只需重编它再重新链接，其他文件不用动

## **Example of Link:**


假设一个计算器项目有 4 个源文件：`main.c`（总入口）、`add.c`（加法）、`sub.c`（减法）、`mul.c`（乘法）。

### 前4行：
```bash
gcc -c main.c -o main.o
#把 `main.c` 编译成半成品 `main.o`
gcc -c add.c -o add.o
#把 `add.c` 编译成半成品 `add.o`
gcc -c sub.c -o sub.o
#把 `sub.c` 编译成半成品 `sub.o`
gcc -c mul.c -o mul.o
#把 `mul.c` 编译成半成品 `mul.o`
到这里，四个源文件各变成了一个 `.o` 半成品。它们是**分开做的**，互不打扰
```
### 第5行：链接
```bash
gcc main.o add.o sub.o mul.o -o calc
#这一行没有 `-c`，意思是"做最后一步——链接"。它把四个 `.o` 半成品**拼成**一个能直接跑的完整程序，名字叫 `calc`
```

---

## **二.Makefile**
<span style="color :yellow">
makefile前必须是一个TAB的缩进</span>


* **Example:**
1. 第一步：链接
```makefile
demo: main.o add.o
    g++ main.o add.o -o demo
```
. demo: 目标变成名为demo的可执行文件

· main.o add.o：要生成 demo，你必须先有 main.o 和 add.o 这两个依赖

· g++ main.o add.o -o demo：执行的链接命令

2. 第二步：编译 main.cpp

```makefile
main.o: main.cpp
	g++ -c main.cpp -o main.o
```

· main.o: 这是第一个子目标

· main.cpp：依赖。要生成 main.o，必须先有 main.cpp

· g++ -c main.cpp -o main.o：只编译不链接（-c），生成目标文件

3. 第三步：编译 add.cpp

```makefile
add.o: add.cpp
	g++ -c add.cpp -o add.o
```

· 同理，编译 main.cpp

***以后只用打`make`，就会自动执行需要的文件***

* **缺点：**

1. 不能跨平台
2. 文件多，难维护

---

## **三.Cmake**
### **1.Cmake的基本工作流程**：
```
编写CmakeList.txt文件 -> 使用Cmake生成对应构建系统文件（例如Makefile文件）-> 执行构建系统文件（例如make）
```
---
### **2.CmakeLists.txt**：

* **2.1作用**：定义构建规则，依赖关系和编译选项


* **2.2最小CmakeLists.txt文件演示**：
```cmake
cmake_minimum_required(VERSION 3.16)
# 声明本项目最低需要的 cmake 版本，必须放第一行
```
```cmake
project(demo)
# 定义项目名为demo。执行后 cmake 会自动把 PROJECT_NAME 等变量设好
# 当什么都不指定时cmake会默认开启C语言和C++环境
```
**标准的语法**：project (<项目名> [<语言>...])

**example**:

`roject (demo LANGUAGES CXX)` 指定使用C++语言，不考虑C语言

`project (demo LANGUAGES C C++)` 同时考虑C语言和C++


```cmake
add_executable(demo main.cpp add.cpp)
# 声明要生成的可执行文件 demo，源文件是 main.cpp 和 add.cpp
```

```bash
cmake .
# 读取当前目录的 CMakeLists.txt，生成 Makefile、CMakeCache.txt 等
# .指当前目录
make
# 用生成的 Makefile 编译出 demo
```
* **2.3CMakeLists.txt 常用指令**：

<span style="color :yellow">**格式：`指令名(参数)`**</span>

```cmake
cmake_minimum_required(VERSION 3.10)
# 版本声明
```
```cmake
project(MyProject VERSION 1.0.0 LANGUAGES CXX)
```
```cmake
set(CMAKE_CXX_STANDARD 17)
# set 是"定义变量"。这里把 C++ 标准设为 17，按C++17的标准来编译
# 不设的话老编译器可能默认按 C++98 编译，你的新语法会报错

set(SOURCES main.cpp utils.cpp helper.cpp)
# 把三个源文件名存进 SOURCES 这个变量
```
```cmake
add_executable(MyApp ${SOURCES})
# 声明一个叫 MyApp 的可执行文件，源码就是 ${SOURCES} 里那三个文件
```

* ***${xxx} 是 cmake 里"取出变量值"的写法，类似 shell 里的 $xxx***

```cmake
add_library(MyLib STATIC library.cpp)
# 生成一个叫 MyLib 的库。STATIC 表示静态库（`.a`，不依赖外部环境编译时直接嵌入可执行文件），SHARED 表示动态库（`.so`，运行时加载）
```
* **标准语法**：add_library (<目标名> [STATIC | SHARED | MODULE] <源文件>)
* **动态库和静态库对比**：静态库方便独立，而动态库省空间
```cmake
target_include_directories(demo PRIVATE ${PROJECT_SOURCE_DIR}/include)
# 编译demo时，去xxx/include 这个xxx目录里找 #include 的头文件
# PRIVATE 的意思是：这个路径只有 MyApp 自己用，不往外传
```
* **语法**：target_include_directories(<目标>  <PUBLIC | PRIVATE | INTERFACE> <路径...>)

* **PUBLIC**：公有

* **PRIVATE**：私有
 
* **INTERFACE**：自己不可用，依赖我的可用

* **{PROJECT_SOURCE_DIR}**：代表当前Cmakelist.txt的绝对路径

```cmake
target_link_libraries(MyApp PRIVATE MyLib)
# 把 MyLib 库"接"到 MyApp 上。MyApp 里可以调用的 MyLib 的函数
```
* **标准语法**：target_link_libraries(xxx PRIVATE 官方规定的名字)
---
### **3.执行Cmake流程**：
```bash
mkdir build && cd build
# 新建 build 目录并进去
# 想重来直接删掉 build 目录就行，防止弄脏源码目录

cmake ..
# 读取上一级目录（..）里的 CMakeLists.txt，生成 Makefile、CmakeCache.txt等

cmake --build .
# 真正开始编译，相当于在 build 目录里执行 make，生成可执行文件
```
---
### **4.Cmake构建类型控制**:
```cmake
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")   
	#CMAKE_CXX_FLAGS:额外的参数包
	#"${CMAKE_CXX_FLAGS}即保留原来的参数包，再塞一个-g
```
#### 4.1.`Debug `模式加 `-g`，保留调试信息，方便打bug的断点；`Release` 模式加`-O2`,开优化，跑得快
#### 4.2.`STREQUAL`即为C++中的==
#### 4.3.`CMAKE_BUILD_TYPE`:是Cmake的一个内置变量，通常有两个值“Debug”和“Release”，手动设定，即`catkin_make-D CMAKE_BUILD_TYPE=Debug`或`catkin_make-D CMAKE_BUILD_TYPE=Release`
#### 4.4.<span style="color:pink">Debug 和 Release 的取舍</span>：一个好调试，一个跑得快

```cmake
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")  
	#Release类型
endif()
# endif()和 if()像大括号，成对出现
```
#### 4.5.`if`/`elseif`/`else`/`endif` 是 cmake 的条件语句

---

### **5.找第三方库**：
```cmake
find_package(Boost 1.70 REQUIRED)
# 在系统里找 Boost 库（版本至少 1.70）
# REQUIRED:找不到就直接报错停止

target_link_libraries(MyApp PRIVATE Boost::Boost)
```
#### `::`的使用:包名（package）::模块名(module)
* 不知道模块名可以让其报错，后抄上正确的

---

### **6.cmake偷懒技巧**：
* **自动找全`.cpp`文件**:
```cmake
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
# 把东西都装入SOURCES变量中
```
* **file(GLOB ...)**：这是 CMake 的“扫描仪”，去磁盘里把符合条件的文件全找出来

* **${PROJECT_SOURCE_DIR}**：代表你当前工程的最顶层根目录

* **src/*.cpp**：通配符，意思是 src 文件夹下所有的 .cpp 结尾的文件
---

* **大项目中的小改动**：
```cmake 
add_executable(demo main.cpp add.cpp)
# 只加一个名字，重新cmake+make就可以
```
---

## **四.catkin：ROS的cmake**
### **1.创建catkin工作区**：
***以下都要在容器docker内部操作***
```bash
catkin --help
# 检查 catkin 是否安装（能弹出帮助就说明装好了）
```
---
* **建工作区，并在里面建 src 子目录（所有 ROS 包必须放 src 里）**:
```bash
mkdir catkin_ws
cd catkin_ws && mkdir src
cd src
``` 
```bash
mkdir  -p ~/catkin_ws/src
cd ~/catkin_ws/
```
```bash
catkin_init_workspace
# 初始化工作空间：在 src 里生成一个 CMakeLists.txt 链接，标记"这里是 catkin 工作区"
```
```bash
cd ..
# 返回上一级目录
catkin build
# 编译整个工作区（内部就是 cmake + make），自动生成 build/ 和 devel/
```
* **编译后目录结构**：
```
catkin_ws/
├── src/      ← 自己写代码的地方（所有 ROS 包放这里）
├── build/    ← 编译过程的中间文件、二进制文件，基本用不到
└── devel/    ← 开发空间：开发过程中的可执行文件和库都在这
```
* **编译后要加载
```bash
source devel/setup.bash
# 把编译产物"注册"进当前终端，rosrun 等命令才能找到你的包
```
* **<span style="color:yellow"> 每开一个新终端都要重新 source 一次!!!</span>**
---
### **2.创建ROS功能包**：
```bash
cd ~/catkin_ws/src/
# 先进入 src 目录（ROS包必须建在 src 里）
```
```bash
catkin_create_pkg test_pkg std_msgs rospy roscpp
# 一条命令生成新包骨架：包名 test_pkg，后面三个是依赖的库
```
* **标准语法**：`catkin_create_pkg <package name> [依赖1] [依赖2] [...]`
* **自动生成两个关键文件：**
* **`package.ml`**:包的"身份证"：名字、版本、依赖了谁（给 catkin 看的依赖清单）
* **`CMakeLists.txt`**:编译的说明书，即怎么编译（给 catkin/cmake 看的
* **<span style="color:pink">`rospy = ROS` 的 python 依赖；`roscpp = ROS` 的 C++ 依赖；`std_msgs `标准消息</span>**
---
### **2.1 catkin生成的CmakeList.txt举例**：
```cmake
cmake_minimum_required(VERSION 3.0.2)
# 声明最低 cmake 版本

project(demo1)
```
```cmake
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs)
# 去系统里找 catkin 及这三个依赖包；`REQUIRED` = 找不到就报错停
# `COMPONENTS` 后面对应了3个依赖包
```
#### <span style="color:pink">---- 下面三个是 ROS 三种通信类型对应的文件声明 ----
```cmake
add_message_files(...)   
# 消息文件 → 话题通信（topic）
# 单向发送
# 适用场景：持续不断的数据流
# 例子：你定义一个 Person.msg（包含名字、年龄），程序A疯狂发布这个人的最新状态，程序B在后台一直默默接收
```
* **括号中（...）格式:FILES + 你的 .msg 文件名（可以写多个）**
```cmake
add_service_files(...)   
# 服务文件 → 服务通信（service）
# 对应作用，一问一答式
# 适用场景：偶尔触发、需要结果的操作。比如控制机器人开一个灯
# 例子：你定义一个 GetNumber.srv（请求发一个字符串，返回一个整数）。程序A问：“帮我查一下张三的学号”，程序B查了一下返回：“12345”
```
* **括号中（...）格式:FILES + 你的 .srv 文件名**
```cmake
add_action_files(...)    
# 动作文件 → 动作通信（action）
# 既能双向通信（发送目标），又能持续反馈进度，而且中途可以取消
# 时间长，且需要实时反馈的任务
# 例子：你定义一个 MoveRobot.action（目标：去厨房；反馈：当前坐标；结果：成功到达或失败）。机器人会一边走一边给你发反馈，你随时可以喊停
```
* **括号中（...）格式:FILES + 你的 .action 文件名**
```cmake
generate_messages(...)   
# 生成消息对应的代码
```
* **括号中（...）格式:DEPENDENCIES + 你的自定义文件里用到的其他 ROS 包的名字**
```cmake
catkin_package(...)
# 声明这个包的依赖，让别人拿到你的包也能一样编译

# dynamic_reconfigure：ROS 的动态调参系统，需要配 config 文件（模板里已留好），即不用重启也可以随时改参数
```
```cmake
install(...)
# 安装规则
```


* **<span style="color:yellow">通信完整实操演示:</span>**

假设需求是：自定义一个话题消息
1. 在包里建一个 msg 文件夹

2. 里面建个 Person.msg，写入：

```text
string name
int32 age
```
3. 在 CMakeLists.txt 中，把注释解开，写成这样：

```cmake
add_message_files(
  FILES
  Person.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)
# 用于编译
# 当没有使用其他特殊的ros包时，只用使用std_msgs
```
4. 千万别忘了这两步！（不然编译绝对报错）：

把 catkin_package(...) 这行改成：

```cmake
catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
)
# CATKIN_DEPENDS 即运行时有以下依赖 roscpp， rospy， std_msgs， message_runtime
# message_runtime为消息运行库，在有自定义消息时必须有！！！！！
```
5. <span style="color:yellow">记得打开同目录下的 `package.xml` 文件，在 `<build_depend>` 和 `<exec_depend>` 里加上 `message_generation` 和 `message_runtime`
---
### **2.2 package.xml实例**:
```XML
<name>demo1</name>     
<!--文件名字-->  
<description>The demo1 package</description>  
<!--备注说明作用-->
<maintainer email="...">作者</maintainer>  
<!--有问题找谁，即邮箱的主人-->   
<build_depend>roscpp</build_depend>   
<build_depend>rospy</build_depend> 
<build_depend>std_msgs</build_depend> 
<!--要这个包成功编译需要什么-->
```
---
### **2.3 一个package的目录结构**:
```
package/
├── src/      # C++ 源码
├── include/  # 头文件
├── scripts/  # Python 脚本
├── msg/      # 话题消息文件
├── srv/      # 服务文件
├── action/   # 动作文件
├── models/ urdf/  # 机器人模型
└── launch/   # 一键启动文件
```

---
### **3.cmake 和 catkin 的区别**：

|      | 普通 cmake 项目 | ROS catkin 项目 |
| :--- | :--- | :--- |
| 配置文件 | CMakeLists.txt | CMakeLists.txt + package.xml |
| 构建命令 | cmake .. + make | catkin build（内部就是 cmake+make |
| 构建目录 | 手动 mkdir build | 自动生成 build/ 和 devel/ |
| 编译完的环境 | 无需额外操作 | 必须 **source devel/setup.bash** |   
---

## **五.大模型的使用**
---
[AI编程助手的本质——新手解读](https://a2070230df5ac0cef.app.workbuddy.host/?sharecode=4KQYKJC5JN)

---
### **1. Agent VS 网页聊天**：
* **网页聊天**：生成一些文本和图片，其余要自己手动完成
* **Agent**：操作本地的文件，帮你直接全部完成

  ***Agent可以与外界交互，持续执行任务***
---
### **2. 用大模型帮助学习**：

1. **收集资料** —— "去网上搜索关于 xx 的最佳实践/优秀参考，给出信息来源"
2. **告诉 AI 你学过什么，设计针对性课程** —— "我学过 C++，请通过交互式提问了解我的水平，并设计一套 Python 学习课程"
3. **学一个课题时，自己讲一遍让 AI 检查** —— 费曼学习法，讲得出来才是真懂
4. **让 AI 出练习题** —— 用题目检验自己是不是真的会了
---
### **3. 用大模型帮助开发**:

1. 清楚说明目标
2. 提供必要上下文
3. 明确重要约束
4. 规定验证标准
5. 允许 Agent 自主探索
6. 避免过度规定执行步骤
---
### **4. skill 和 AGENTS.md**

- **Skill（技能）**：一组可复用的任务指令 + 配套资源（脚本、参考文档、模板等），用来指导 Agent 完成特定类型的任务，Agent 在需要时加载
- **AGENTS.md**：类似给人看的 README.md，**AGENTS.md 是给 AI 看的项目说明书**，用来向 AI 编程 Agent 提供项目背景、开发规范和工作指引
