# 培训第二课笔记

## cmake

### 从代码源到可执行程序
- 具体转换的过程分为以下几步：
    - 1.预处理（cpp） （得到修改了的源文件 （ .i文件））
    - 2.编译 （ccl） (得到汇编程序（ .s文件）)
    - 3.汇编 （as） (得到可重定位目标程序 （ .o文件）（二进制）)
    - 4.链接 （ld）（得到可执行目标程序 （文件无后缀） （二进制））

### Makefile
- Makefile如同一份自动编译说明书，它告诉make工具:哪些文件要编译、编译顺序是什么、需要哪些命令
- Makefile实现了编译程序、构建程序的自动化提高了人们的效率。通过编写Makefile，在我们每次编写修改好代码后，只需要敲一次make命令便能够得到你想要的目标程序

### 什么是Cmake?
- CMake 本身不编译代码，它是一个构建系统生成器。通过编写`CMakeLists.txt` 告诉它项目结构，它生成对应平台的标准构建文件（Linux 下是 Makefile），最后由本地工具完成编译
- 不要在源码目录里直接跑 Cmake .，必须创建一个独立的 `build` 文件夹。（因为跑了cmake指令后会生成一些文件在你所在的目录 那个可执行程序也在里面）这样可以避免生成的垃圾文件污染源码，删构建结果时直接删文件夹即可

### 我的理解历程
- 首先要知道要编译多个源文件时需要手写makefile,很麻烦，而cmake就是解决这个麻烦的工具，可自动帮我生成makefile，输入make后即可完成编译
- cmake只负责完成怎么调用本地编译器（g++），但编译还是由本地编译器（g++）完成
- 当项目比较大时，Cmake就能帮助整合源代码且进行编译得到可执行程序

### CMakeLists.txt文件
- 注意文件名大小写敏感
- 先看看里面写了什么基本内容
```cmake
cmake_minimum_required(VERSION 3.10)  # 用什么版本

project(HelloCMake)                   # 项目叫啥名

add_executable(hello main.cpp)        # 我要一个叫 hello 的程序，用 main.cpp 做
```
- `add_executable`中的main.cpp就是自己写的源代码，hello就是最终要生成的可执行文件（这个文件不需要自己建）

- 注意`CMakeLists.txt`这个文件要和源代码放在同一个目录（文件夹）下

- `add_executable`里只能有且仅有一个源代码里有一个main函数

- 源代码里的所有函数要放在一个.h文件里进行声明，且含有main函数的那个源代码（以及其他需要用到函数的源代码）要include这个`.h`头文件来声明函数,但这个`.h`头文件不用放进`add_executable`里
```h
#ifndef _HEAD_H
#define _HEAD_H

int add(int a,int b);
int sub(int a,int b);
int mult(int a,int b);
int divide(int a,int b);

#endif
```

### 在终端中的命令
- cmake 后面要跟源代码所在的目录（即找到`CMakeLists.txt`这个文件在哪），这样才能生成Makefile文件 （比如`CMakeLists.txt`在上一级目录，则输入cmake ..）
- 生成Makefile文件后，接下来输入make指令，就能生成指定可执行程序
- 最后 ./ 来运行 （./hello）

### 宏
宏：用最通俗的话来说，宏就是一个“提前定好的名字”或者“代号”
- CMake 里的宏（其实就是变量）
    - 例子：`CMAKE_CURRENT_SOURCE_DIR` 就是 CMake 内置的一个宏（也就是一个预先定义好的变量）
    - 它的背后存着一长串路径，比如 `/home/dabing/code/demo/v1`
    - 每次写 `${CMAKE_CURRENT_SOURCE_DIR}`，CMake 就会自动把它替换成那个长路径
- 在 C++ 代码里的“宏”
(上面.h头文件里就有)
```cpp
#ifndef _HEAD_H
#define _HEAD_H
// ...
#endif
```
这里的`#define` 就是定义宏

在 C++ 里，宏主要是给预处理器（也就是负责复制粘贴 #include 的那个家伙）看的

### set的使用
set() 就是用来“定义变量”或“给变量赋值”的

基本语法：
```cmake
set(变量名 变量值1 变量值2 ...)

#注意：set 后面不需要加 $ 或 {}，直接写名字就行
```

1. 存单个值（当普通变量用）
好比在代码里写 `int a = 10;`

```cmake
set(MY_AGE 18)    #MY_AGE 就是一个存18的变量
set(MY_NAME "张三")    #MY_NAME 就是一个存 “张三” 的变量
```
如果后面你想用这个变量，只要加上 `${}` 就行：
```cmake
message("我的名字是 ${MY_NAME}，今年 ${MY_AGE} 岁")

#注：message() 是 CMake 里的打印输出命令，相当于 C++ 的 `cout
```

2. 存一堆文件（当列表用）
```cmake
# 定义一个叫 SRC 的变量，里面装 3 个文件名
set(SRC main.cpp add.cpp sub.cpp)

# 然后在 add_executable 里，用 ${SRC} 把它们掏出来
add_executable(app ${SRC})
```
这里 set 的作用是帮你把一堆散乱的 .cpp 文件打包成一个列表

3. 修改系统的默认行为（修改内置宏）
```cmake
set(EXECUTABLE_OUTPUT_PATH /home/dabing/aa/bb/cc)
```
这相当于在给 CMake 下命令：“把默认的生成路径改了，以后生成的可执行文件全放到 `/home/dabing/aa/bb/cc` 这个目录里

4. 取消变量的定义（清空）
```cmake
set(MY_VAR)  # 把 MY_VAR 置空，相当于取消定义
```
### 搜索文件
在 CMake 里，搜索文件通常是指自动去某个文件夹里把所有符合条件的代码文件（比如 .cpp 或 .h）找出来，然后打包交给 `add_executable` 或 `add_library` 去编译

实现这个功能的命令叫 `file(GLOB ...)`

#### 怎么用 file(GLOB) 搜索文件？
- `file`：CMake 的内置命令，专门用来操作文件

- `GLOB`：通配符模式（就像你在终端里敲 `ls *.cpp`）

- `SRC`：把搜到的结果存进这个变量里

- **要搜索的路径和规则**：通常写 `*.cpp` 或者 `${CMAKE_CURRENT_SOURCE_DIR}/*.cpp`

最标准的写法是这样的:
```cmake
file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
#意思是：去当前 CMakeLists.txt 所在的目录，把所有 .cpp 文件找出来，装进 SRC 这个变量里

add_executable(app ${SRC})
# 然后直接用
```

注意：用 `file(GLOB ...)` 的时候，每次增删文件后，手动重新跑一次 `cmake ..`

### 指定头文件目录

场景：如果 .cpp 和 .h 在同一个文件夹，直接 `#include "head.h"` 就能找到；但如果在真实项目里，头文件被放到了专门的 `include` 文件夹里（或其他文件夹里），编译器就会报错,在时候就需要用到 **指定头文件目录**
```cmake
target_include_directories(app PRIVATE include)
```
- `app`：给这个目标（target）指定路径

- `include`：你要指定的目录路径（可以是相对路径，也可以是绝对路径）

- `PRIVATE`：权限控制
    - PRIVATE（私有）：“我自己用，别人别碰”
    - PUBLIC（公开）：“我用，谁链接我，我也把这路径共享给谁”
    - INTERFACE（接口）：“我不用，但谁链接我，我就把这路径送给谁”

```cmake
cmake_minimum_required(VERSION 3.15)
project(test)

# 1. 把 src 下的源文件列表写出来
set(SRC
    src/main.cpp
    src/add.cpp
    src/sub.cpp
)

# 2. 生成可执行文件
add_executable(app ${SRC})

# 3. 关键：告诉 app，去 include 目录里找头文件
target_include_directories(app PRIVATE include)
```
### 通过CMake制作库文件
**库** 就是一个预先写好、打包好、可以直接拿来用的“代码工具包”（比如我写的一系列函数），可以供别人二次开发或集成到他们自己的项目中

- 库分两种：**静态库** 和 **动态库**
    - **静态库**（STATIC）：后缀 `.a`（Linux） 它就像把所有的代码直接“复印”并钉死在别人的程序里。程序会变大，但发给别人时不用带库文件。

    - **动态库**（SHARED）：后缀 `.so`（Linux） 它就像一个外挂包，程序运行时才去调用它。程序体积小，但发给别人时，必须把 `.so` 文件也一起发过去。

#### 制作静态库
1. 编写 `CMakeLists.txt` 造库
```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject)

# 1. 使用 add_library 代替 add_executable 来生成库
# 语法：add_library(库的名字 STATIC 源文件...)
# 注意：这里不要写 main.cpp 因为库里不需要入口，只需要干活的函数
add_library(math_lib STATIC add.cpp sub.cpp mult.cpp divide.cpp)
# 这里math_lib 的_lib不是后缀，而是math_lib整体作目标名

# 2. 把头文件目录也声明给这个库（这样别人链接库的时候，就能顺便拿到头文件路径）
target_include_directories(math_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
``` 

2. 新建 `main.cpp` 测试这个库,并在 `CMakeLists.txt` 里加一个可执行文件
```cmake
# 在刚才的 CMakeLists.txt 下面继续写：

# 3. 生成一个测试程序
add_executable(app main.cpp)

# 4. 把刚才造好的 math_lib 链接给 app

target_link_libraries(app PRIVATE math_lib)
# 这一句的作用：它既把库里的代码给了 app，又把 math_lib 的头文件路径也传给了 app
```
**测试一下：**
去 `build` 里跑 `cmake ..` 和 `make`，会发现目录里生成了两个东西：
1. `libmath_lib.a` （造的静态库，lib 前缀是 CMake 自动加的）
2. `app` （测试程序）

#### 制作动态库
把上面 `STATIC` 换成 `SHARED` 就行
```cmake
add_library(math_lib SHARED add.cpp sub.cpp mult.cpp divde.cpp)
```

### 在程序中链接静态库

假设文件结构如下：
project/
- CMakeLists.txt
- main.cpp           # 我写的新程序
- include/
    - head.h         # 库的头文件（说明书）
- lib/
    - libmath_lib.a  # 我提前编译好的静态库文件

1. 编写 `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.15)
project(UseStaticLib)

# 步骤 1：生成可执行文件
add_executable(app main.cpp)

# 步骤 2：告诉编译器头文件在哪（否则 #include "head.h" 会报错找不到文件）
# 这里的 include 是我的头文件目录
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 步骤 3：告诉链接器库文件在哪
# 这里的 lib 是我的静态库所在目录
target_link_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# 步骤 4：把具体的静态库链接到程序上
# 注意：如果库文件叫 libmath_lib.a，这里只需写 math_lib（去掉 lib 前缀和 .a 后缀）
target_link_libraries(app PRIVATE math_lib)
```
2. 代码中的调用
在 `main.cpp` 中，只需要引入头文件，就可以直接调用库里的函数了：
```cpp
#include <iostream>
#include "head.h" // 引入静态库的头文件

int main() {
    int result = add(10, 20); // 直接调用静态库里的 add 函数
    std::cout << "Result: " << result << std::endl;
    return 0;
}
```
这时候我就有一个疑问：我咋知道头文件叫`head.h`? 原来是别人在提供库的时候会说明要添加什么头文件，就会把`head.h`声明出来

4. 构建与运行

在`biuld`目录下执行 **cmake ..** 然后执行 **make** 最后执行程序 **./app** 

---

## catkin
底层本质：Catkin 底层完全依赖 CMake，是对 CMake 的封装，专门用来管理 ROS 复杂的依赖关系

（进行一些套娃 好比cmake是生成makefile一样 catkin是生成cmake）

### 一、了解一些核心工作空间（Workspace）结构
首先，catkin的"最根"目录是catkin_ws 这个ws就是workspace（工作空间）的缩写 下面列举它的子目录

既然是ros的工具 那么这个ws目录是在在我的docker里的

#### 1. src/ (源码空间)
以cmake类比：src/ 可相当于cmake中存放源代码的目录，不过呢，这个会牛鼻一点，cmake放的是文件级的，src放的是项目级的，里面放的不是.cpp，而是功能包package
- 这个目录下会有`CMakeLists.txt` （这个不要去动它）和 `package`（文件夹）（也不一定是package 这个包名字是自己命名的）,Catkin 是包（Package）的管理系统，而底层 CMake 是项目（Project）的编译系统

- 什么是package（功能包）（也是个文件夹）:总而言之就是用来装代码的，
在这个文件夹下面会有`CMakeLists.txt`（和上面的不同）还有`package.xml`(包的身份证)

    - 要写代码c++时，就得在这个package目录下 mkdir 一个 src ，然后在再 src/ 里 touch 一个 .cpp 文件 再在这个文件里敲代码

    - 写好代码后(比如写了hello.cpp)，要打开**包目录里的 `CMakeLists.txt`** 找到中间空白的地方，填上：
```cmake
add_executable(hello src/hello.cpp)
target_link_libraries(hello ${catkin_LIBRARIES})
include_directories(${catkin_INCLUDE_DIRS})

#把刚写的 hello.cpp 编译成叫 hello 的程序，并连上 ROS 的库”
```
- 最后回到 catkin_ws/ 进行编译
```bash
cd ~/catkin_ws
catkin_make
```
- 运行程序
```bash
source devel/setup.bash    #这一句是固定的
rosrun my_pkg hello    #<包目录> <可执行程序>
```

#### 2.biuld/ (编译空间)
存放 CMake 和 Make 执行过程中产生的所有缓存文件、中间文件以及 Makefile

#### 3.devel/ （开发空间）
成品仓库，日常开发真正需要用到的文件都在这里
- 核心文件：
    - `setup.bash` 每次编译完，必须执行 source devel/setup.bash，系统才能找到我刚生成的程序
    - `lib/`：我的 C++ 可执行程序最终会存放在这个目录下
    - `include/`：如果我写了自定义消息，编译后生成的 C++ 头文件会放在这


### 二、认识一些命令（上面其实也有提到 这里巩固一下）
1. 创建工作空间
```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace  # 初始化，在 src 里生成 CMakeLists.txt 链接

cd ~/catkin_ws
catkin_make            # 首次编译，生成 build 和 devel 文件夹
```
catkin_make命令有啥鸟用呢：会跑到catkin_make目录下，检查有没有biuld文件，没有就建一个 然后跑进biuld里，自动搞上述cmake该搞的流程，非常的方便牛鼻

2. 创建 ROS 功能包
```bash
cd ~/catkin_ws/src

# 语法：catkin_create_pkg <包名> <依赖1> <依赖2> ...
catkin_create_pkg my_pkg roscpp rospy std_msgs
```
- 首先什么是依赖呢：就是别人写好的包 如果上述指令的包后面没有依赖，那么这个包就是空的

- 这个my_pkg就是我创建的包，后面跟依赖(如果要自己写代码，看上面 一、1.里的内容)

- `roscpp`：C++ 接口依赖。

- `rospy`：Python 接口依赖。

- `std_msgs`：标准消息类型依赖（比如字符串、整数）

3. 编译与生效
```bash
cd ~/catkin_ws   # 必须回到根目录
catkin_make      # 编译所有 src 下的包
source devel/setup.bash  # 让系统知道你的包在哪（每次打开新终端都要敲）
```
4. 清理编译缓存

如果遇到莫名其妙的报错，可以清理后重新编译：
```bash
cd ~/catkin_ws
rm -rf build/ devel/
catkin_make
```

### 下面搞一个我实践的例子
先进入容器 然后加载ros变量环境 然后：
```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace

cd ~/catkin_ws
catkin_make

cd ~/catkin_ws/src
catkin_create_pkg my_pkg

cd my_pkg
mkdir src
cd src
touch hello_node.cpp
```
接下来在`hello_node.cpp`里敲代码
```cpp
#include <ros/ros.h>

int main(int argc, char **argv)
{
    // 初始化 ROS 节点，节点名叫 "hello_node"
    ros::init(argc, argv, "hello_node");

    // 创建节点句柄
    ros::NodeHandle nh;

    // 打印一句问候语
    ROS_INFO("Hello, ROS! My first node is running!");


    return 0;
}
```
然后在 **包里的src里的CMakeLists.txt里** 进行修改
1. 把 `find_package(catkin REQUIRED)` 改成

 `find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs)`

 (或者在创建包时直接添加这些包 ： catkin_create_pkg topic_pkg roscpp std_msgs)

2. 在文件末尾加上：
```cpp
add_executable(hello_node src/hello_node.cpp)
target_link_libraries(hello_node ${catkin_LIBRARIES})
include_directories(${catkin_INCLUDE_DIRS})
```

接下来就是编译和运行了
```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
rosrun my_pkg hello_node
```
最后输出：`Hello, ROS! My first node is running!`

---
## 对大模型的认识
先讲讲我是怎么使用ai来学习的吧。其实我一般会先去b站查找有没有相关的教学视频，如果有，我会先选择看视频，遇到看不懂的了，就去问ai，直到我问懂为止；如果b站找不到相关的教学视频，那我就回去问ai这方面的内容。我的经验是，ai一开始不会讲的太详细，每个点简单讲一下就过去了，这时候我可能会非常懵比，好在这也算是ai列出了一个大纲，接下来我就会一点一点的对每一个小点深度追问，直到我问懂为止，然后问的多了就会发现慢慢就对这个东西有理解了。可以边问ai的时候边自己实操一下，可以加深印象和理解。上面笔记里的内容就是b站加上我问ai然后自己手搓markdown写的。其实ai时代很多知识和技术的门槛会变低，运用好ai确实能够高效地提升自己，这真的是一个何时都能秒回、解决你的疑问的虚拟好老师（不过ai有时候可能比较笨，说一些错误的内容，有时候得批判性地看ai的答复，因此也不能完全无条件的信任ai）

### 如何用大模型帮助学习
1. 收集材料
- 要学会怎么给关键词，清除说明目标，提供必要上下文，让ai给你最想要的答复
2. 定制专属课程
- 可以跟ai说自己学过什么内容，让ai通过交互类比我懂的东西来教我新东西，这样确实会有助于理解（有时候ai也会自己搞比喻，不过可能还是看不懂）
3. 主动输出
- 学习某个东西时，自己讲一遍，让ai检查。这样可以让ai给我纠错，同时也可以加深对这个东西的印象
4. 实践
- 一定要自己多实践来加深理解，可以让ai出题，也可以跟着ai进行一些操作。不要空学理论，理论和实践还是有差距

### 辅助工具：skill
skill 是一组可复用的任务指令和配套资源，用于指导Agent完成特定类型的任务

资源包含：可执行的脚本、参考文档、代码模板等

### 辅助工具：AGENTS.md (项目说明书)

向ai编程Agent提供项目背景、开发规范和工作指引，指导它在当前特定项目中如何开展工作

### 最后也是一个小提醒
- 负责任使用 ai：
    - ai是工具，不能盲目复制粘贴ai生成的代码或结论，必须经过人类大脑的review
- 对团队负责，对自己负责
    - 代码或项目出问题，最终担责的是自己，而不是ai。引入ai工具是为了提效，但引入的bug依然是自己的责任